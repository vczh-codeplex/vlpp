using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Xml.Linq;
using System.Security.Cryptography;
using NodeService.Protocols.StreamBasedProtocolObjects;
using System.IO;

namespace NodeService.Protocols
{
    public class AuthenticationProtocolHandlerFactory : ITranslatorProtocolHandlerFactory
    {
        private IAuthenticationProvider authenticationProvider;
        private string userName;
        private byte[] passwordMD5;

        public AuthenticationProtocolHandlerFactory(IAuthenticationProvider authenticationProvider)
        {
            this.authenticationProvider = authenticationProvider;
        }

        public AuthenticationProtocolHandlerFactory(string userName, byte[] passwordMD5)
        {
            this.userName = userName;
            this.passwordMD5 = passwordMD5.ToList().ToArray();
        }

        public ITranslatorProtocolServerHandler CreateServerHandler()
        {
            if (this.authenticationProvider == null)
            {
                throw new NotSupportedException();
            }
            return new ServerHandler(this.authenticationProvider);
        }

        public ITranslatorProtocolClientHandler CreateClientHandler()
        {
            if (this.userName == null)
            {
                throw new NotSupportedException();
            }
            return new ClientHandler(this.userName, this.passwordMD5);
        }

        public XElement[] GetFactoryDescription()
        {
            return new XElement[] { new XElement("AuthenticationProtocolHandlerFactory") };
        }

        private abstract class HandlerBase : ITranslatorProtocolHandler
        {
            private byte[] key;
            private byte[] iv;

            protected void SetKey(byte[] key, byte[] iv)
            {
                this.key = key;
                this.iv = iv;
            }

            public byte[] Decode(byte[] bytes)
            {
                if (this.key == null)
                {
                    return bytes;
                }
                else
                {
                    return Crypting.AesDecrypt(bytes, this.key, this.iv);
                }
            }

            public byte[] Encode(byte[] bytes)
            {
                if (this.key == null)
                {
                    return bytes;
                }
                else
                {
                    return Crypting.AesEncrypt(bytes, this.key, this.iv);
                }
            }

            public abstract bool Pass(INodeEndpointProtocolRequest request);

            protected static byte[] Md5AuthDataAndPassword(byte[] authData, byte[] passwordMD5)
            {
                byte[] data = new byte[authData.Length + passwordMD5.Length];
                int length = Math.Max(authData.Length, passwordMD5.Length);
                int current = 0;
                for (int i = 0; i < length; i++)
                {
                    if (i < authData.Length) data[current++] = authData[i];
                    if (i < passwordMD5.Length) data[current++] = passwordMD5[i];
                }
                using (MD5CryptoServiceProvider md5 = new MD5CryptoServiceProvider())
                {
                    return md5.ComputeHash(data);
                }
            }
        }

        /*
         * Process
         *      Client -> UserName
         *      Server -> AuthData
         *      Client -> MD5(Mix(AuthData, PasswordMD5))
         *      Server -> "PASS"|"FAIL"
         *      Client -> RsaPublicKey
         *      Server -> Rsa(int32.keylength + AesKey + AesIv, RsaPublicKey)
         */

        private class ServerHandler : HandlerBase, ITranslatorProtocolServerHandler
        {
            private enum Step
            {
                WaitingForUserName,
                WaitingForEncryptedAuthData,
                WaitingForPublicKey,
                Done,
            }

            private IAuthenticationProvider authenticationProvider;
            private INodeEndpointProtocolServer server;
            private Step currentStep = Step.WaitingForUserName;
            private byte[] expectedMixedAuthDataAndPassword = new byte[] { };
            private object stepLock = new object();

            public ServerHandler(IAuthenticationProvider authenticationProvider)
            {
                this.authenticationProvider = authenticationProvider;
            }

            public void AttachServer(INodeEndpointProtocolServer server)
            {
                this.server = server;
            }

            public override bool Pass(INodeEndpointProtocolRequest request)
            {
                lock (this.stepLock)
                {
                    switch (this.currentStep)
                    {
                        case Step.WaitingForUserName:
                            {
                                string userName = request.RequestMessage();
                                byte[] passwordMD5 = this.authenticationProvider.GetUserNamePasswordMD5(userName);
                                byte[] authData = new byte[128];
                                new Random().NextBytes(authData);

                                if (passwordMD5 != null)
                                {
                                    this.expectedMixedAuthDataAndPassword = Md5AuthDataAndPassword(authData, passwordMD5);
                                }
                                request.Respond(authData);
                                this.currentStep = Step.WaitingForEncryptedAuthData;
                            }
                            return false;
                        case Step.WaitingForEncryptedAuthData:
                            {
                                if (this.expectedMixedAuthDataAndPassword == null)
                                {
                                    request.Respond("FAIL");
                                }
                                else
                                {
                                    byte[] expected = this.expectedMixedAuthDataAndPassword;
                                    byte[] actual = request.Message;
                                    this.expectedMixedAuthDataAndPassword = null;

                                    bool fail = false;
                                    if (expected.Length != actual.Length)
                                    {
                                        fail = true;
                                    }
                                    else
                                    {
                                        for (int i = 0; i < expected.Length; i++)
                                        {
                                            if (expected[i] != actual[i])
                                            {
                                                fail = true;
                                                break;
                                            }
                                        }
                                    }

                                    request.Respond(fail ? "FAIL" : "PASS");
                                    this.currentStep = Step.WaitingForPublicKey;
                                }
                            }
                            return false;
                        case Step.WaitingForPublicKey:
                            {
                                byte[] publicKey = request.Message;
                                byte[] key, iv;
                                Crypting.AesGenerateKey(out key, out iv);

                                byte[] message = new byte[sizeof(int) + key.Length + iv.Length];
                                StreamProtocol<Stream>.WriteLeadBytes(key.Length, message);
                                Array.Copy(key, 0, message, sizeof(int), message.Length);
                                Array.Copy(iv, 0, message, sizeof(int) + message.Length, iv.Length);

                                byte[] encryptedMessage = Crypting.RsaEncrypt(message, publicKey);
                                request.Respond(encryptedMessage);

                                SetKey(key, iv);
                                this.currentStep = Step.Done;
                            }
                            return false;
                    }
                }
                return true;
            }
        }

        private class ClientHandler : HandlerBase, ITranslatorProtocolClientHandler
        {
            private enum Step
            {
                Ready,
                WaitingForAuthData,
                WaitingForAuthResult,
                WaitingForAesKey,
                Done,
            }

            private string userName;
            private byte[] passwordMD5;
            private byte[] privateKey;
            private INodeEndpointProtocolClient client;
            private Step currentStep = Step.Ready;
            private object stepLock = new object();

            public ClientHandler(string userName, byte[] passwordMD5)
            {
                this.userName = userName;
                this.passwordMD5 = passwordMD5;
            }

            public void AttachClient(INodeEndpointProtocolClient client)
            {
                this.client = client;
            }

            public void OnClientConnected()
            {
                client.Send(this.userName);
            }

            public override bool Pass(INodeEndpointProtocolRequest request)
            {
                lock (this.stepLock)
                {
                    switch (this.currentStep)
                    {
                        case Step.WaitingForAuthData:
                            {
                                byte[] authData = request.Message;
                                byte[] encryptedAuthData = Md5AuthDataAndPassword(authData, this.passwordMD5);
                                this.client.Send(encryptedAuthData);
                                this.currentStep = Step.WaitingForAuthResult;
                            }
                            return false;
                        case Step.WaitingForAuthResult:
                            {
                                if (request.RequestMessage() != "PASS")
                                {
                                    this.client.Disconnect();
                                }
                                else
                                {
                                    byte[] publicKey, privateKey;
                                    Crypting.RsaGenerateKey(out publicKey, out privateKey);
                                    this.client.Send(publicKey);
                                    this.privateKey = privateKey;
                                    this.currentStep = Step.WaitingForAesKey;
                                }
                            }
                            return false;
                        case Step.WaitingForAesKey:
                            {
                                byte[] encryptedMessage = request.Message;
                                byte[] message = Crypting.RsaDecrypt(encryptedMessage, this.privateKey);
                                this.privateKey = null;

                                int keyLength = StreamProtocol<Stream>.ReadLeadBytes(message);
                                byte[] key = new byte[keyLength];
                                byte[] iv = new byte[message.Length - sizeof(int) - keyLength];
                                Array.Copy(message, sizeof(int), key, 0, key.Length);
                                Array.Copy(message, sizeof(int) + keyLength, iv, 0, iv.Length);
                                SetKey(key, iv);

                                this.currentStep = Step.Done;
                            }
                            return false;
                    }
                }
                return true;
            }
        }

        private static class Crypting
        {
            public static void RsaGenerateKey(out byte[] publicKey, out byte[] privateKey)
            {
                using (RSACryptoServiceProvider provider = new RSACryptoServiceProvider())
                {
                    publicKey = provider.ExportCspBlob(false);
                    privateKey = provider.ExportCspBlob(true);
                }
            }

            public static byte[] RsaEncrypt(byte[] data, byte[] publicKey)
            {
                using (RSACryptoServiceProvider provider = new RSACryptoServiceProvider())
                {
                    provider.ImportCspBlob(publicKey);
                    return provider.Encrypt(data, false);
                }
            }

            public static byte[] RsaDecrypt(byte[] data, byte[] privateKey)
            {
                using (RSACryptoServiceProvider provider = new RSACryptoServiceProvider())
                {
                    provider.ImportCspBlob(privateKey);
                    return provider.Decrypt(data, false);
                }
            }

            public static void AesGenerateKey(out byte[] key, out byte[] IV)
            {
                using (AesCryptoServiceProvider provider = new AesCryptoServiceProvider())
                {
                    key = provider.Key;
                    IV = provider.IV;
                }
            }

            public static byte[] AesEncrypt(byte[] data, byte[] key, byte[] IV)
            {
                using (AesCryptoServiceProvider provider = new AesCryptoServiceProvider())
                {
                    using (ICryptoTransform encryptor = provider.CreateEncryptor(key, IV))
                    {
                        return encryptor.TransformFinalBlock(data, 0, data.Length);
                    }
                }
            }

            public static byte[] AesDecrypt(byte[] data, byte[] key, byte[] IV)
            {
                using (AesCryptoServiceProvider provider = new AesCryptoServiceProvider())
                {
                    using (ICryptoTransform decryptor = provider.CreateDecryptor(key, IV))
                    {
                        return decryptor.TransformFinalBlock(data, 0, data.Length);
                    }
                }
            }
        }
    }

    public interface IAuthenticationProvider
    {
        byte[] GetUserNamePasswordMD5(string userName);
    }
}
