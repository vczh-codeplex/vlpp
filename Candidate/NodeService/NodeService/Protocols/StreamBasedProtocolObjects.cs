using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.IO;
using System.Threading;

namespace NodeService.Protocols
{
    namespace StreamBasedProtocolObjects
    {
        class StreamProtocolRequest<StreamType> : INodeEndpointProtocolRequest
            where StreamType : Stream
        {
            private StreamProtocol<StreamType> protocolBase;
            private byte[] message;

            public StreamProtocolRequest(StreamProtocol<StreamType> protocolBase, byte[] message)
            {
                this.protocolBase = protocolBase;
                this.message = message;
            }

            public virtual bool CanRespond
            {
                get
                {
                    return true;
                }
            }

            public virtual byte[] Message
            {
                get
                {
                    return this.message;
                }
            }

            public virtual void Respond(byte[] response)
            {
                this.protocolBase.Send(response);
            }
        }

        abstract class StreamProtocol<StreamType> : INodeEndpointProtocol
            where StreamType : Stream
        {
            private List<INodeEndpointProtocolRequestListener> listeners = new List<INodeEndpointProtocolRequestListener>();
            private INodeEndpointProtocolFactory factory = null;
            private int beginListenCalled = 0;

            protected StreamType Stream { get; set; }

            public StreamProtocol(INodeEndpointProtocolFactory factory)
            {
                this.factory = factory;
            }

            public virtual bool EnableDuplex
            {
                get
                {
                    return true;
                }
            }

            public abstract bool Connected { get; }

            public virtual INodeEndpointProtocolFactory Factory
            {
                get
                {
                    return this.factory;
                }
            }

            public virtual void Disconnect()
            {
                if (this.Stream != null)
                {
                    this.Stream.Close();
                    this.Stream.Dispose();
                    this.Stream = null;
                }
            }

            public void BeginListen()
            {
                if (!this.Connected)
                {
                    this.beginListenCalled = 0;
                    throw new InvalidOperationException("The protocol is not connected.");
                }
                if (Interlocked.Exchange(ref this.beginListenCalled, 1) == 0)
                {
                    byte[] lead = new byte[sizeof(int)];
                    try
                    {
                        this.Stream.BeginRead(lead, 0, lead.Length, r => ReadCallback(r, lead), null);
                    }
                    catch (ObjectDisposedException)
                    {
                    }
                    catch (IOException)
                    {
                    }
                }
            }

            public virtual void AddListener(INodeEndpointProtocolRequestListener listener)
            {
                lock (this.listeners)
                {
                    if (!this.listeners.Contains(listener))
                    {
                        this.listeners.Add(listener);
                    }
                }
            }

            public virtual void RemoveListener(INodeEndpointProtocolRequestListener listener)
            {
                lock (this.listeners)
                {
                    this.listeners.Remove(listener);
                }
            }

            public virtual INodeEndpointProtocolRequestListener[] GetListeners()
            {
                lock (this.listeners)
                {
                    return this.listeners.ToArray();
                }
            }

            public virtual void Send(byte[] message)
            {
                if (!this.Connected) throw new InvalidOperationException("The protocol is not connected.");
                Write(message);
            }

            public static void WriteLeadBytes(int length, byte[] lead)
            {
                unsafe
                {
                    byte* plead = (byte*)&length;
                    for (int i = 0; i < sizeof(int); i++)
                    {
                        lead[i] = plead[i];
                    }
                }
            }

            public static int ReadLeadBytes(byte[] lead)
            {
                int length = 0;
                unsafe
                {
                    fixed (byte* plead = lead)
                    {
                        length = *(int*)plead;
                    }
                }
                return length;
            }

            public static byte[] InstallLeadBytes(byte[] message)
            {
                byte[] lead = new byte[sizeof(int) + message.Length];
                int length = message.Length;
                WriteLeadBytes(length, lead);
                Array.Copy(message, 0, lead, sizeof(int), message.Length);
                return lead;
            }

            public static byte[] UninstallLeadBytes(byte[] leadBytes, Stream readingStream)
            {
                int leadLength = ReadLeadBytes(leadBytes);
                byte[] bytes = new byte[leadLength];
                int readLength = 0;
                int remainLength = bytes.Length;
                while (true)
                {
                    int currentLength = readingStream.Read(bytes, readLength, remainLength);
                    if (currentLength == 0)
                    {
                        break;
                    }
                    readLength += currentLength;
                    remainLength -= currentLength;
                    if (remainLength == 0)
                    {
                        break;
                    }
                }
                if (readLength == leadLength)
                {
                    return bytes;
                }
                else
                {
                    return null;
                }
            }

            private void Write(byte[] message)
            {
                try
                {
                    byte[] installed = InstallLeadBytes(message);
                    this.Stream.Write(installed, 0, installed.Length);
                    this.Stream.Flush();
                }
                catch (IOException)
                {
                }
            }

            private void ReadCallback(IAsyncResult asyncResult, byte[] lead)
            {
                if (!this.Connected) return;
                try
                {
                    int leadLength = this.Stream.EndRead(asyncResult);
                    this.beginListenCalled = 0;
                    if (leadLength == lead.Length)
                    {
                        byte[] bytes = UninstallLeadBytes(lead, this.Stream);
                        if (bytes != null)
                        {
                            StreamProtocolRequest<StreamType> request = new StreamProtocolRequest<StreamType>(this, bytes);
                            lock (this.listeners)
                            {
                                foreach (var listener in this.listeners)
                                {
                                    listener.OnReceivedRequest(request);
                                }
                            }
                        }
                    }
                }
                catch (ObjectDisposedException)
                {
                }
                catch (IOException)
                {
                }
                if (this.Connected)
                {
                    BeginListen();
                }
            }
        }

        abstract class StreamServerProtocol<StreamType> : StreamProtocol<StreamType>, INodeEndpointProtocolServer
            where StreamType : Stream
        {
            private INodeEndpointProtocolServerListener serverListener;
            private INodeEndpointProtocolServer innerProtocol;

            public StreamServerProtocol(INodeEndpointProtocolServerListener serverListener)
                : base(serverListener.Factory)
            {
                this.serverListener = serverListener;
            }

            public virtual INodeEndpointProtocolServer OuterProtocol
            {
                get
                {
                    return null;
                }
            }

            public virtual INodeEndpointProtocolServer InnerProtocol
            {
                get
                {
                    return this.innerProtocol;
                }
            }

            public virtual INodeEndpointProtocolServerListener ServerListener
            {
                get
                {
                    return this.serverListener;
                }
            }

            public virtual void SetOuterProtocol(INodeEndpointProtocolServer protocol)
            {
                throw new InvalidOperationException("The protocol server cannot have a outer protocol.");
            }

            public virtual void OnInnerProtocolSet(INodeEndpointProtocolServer protocol)
            {
                this.innerProtocol = protocol;
            }
        }

        abstract class StreamClientProtocol<StreamType> : StreamProtocol<StreamType>, INodeEndpointProtocolClient
            where StreamType : Stream
        {
            private INodeEndpointProtocolClient innerProtocol;

            public StreamClientProtocol(INodeEndpointProtocolFactory factory)
                : base(factory)
            {
            }

            public virtual INodeEndpointProtocolClient OuterProtocol
            {
                get
                {
                    return null;
                }
            }

            public virtual INodeEndpointProtocolClient InnerProtocol
            {
                get
                {
                    return this.innerProtocol;
                }
            }

            public abstract bool Connect(string address, string endpointName, int timeout);

            public virtual void SetOuterProtocol(INodeEndpointProtocolClient protocol)
            {
                throw new InvalidOperationException("The protocol client cannot have a outer protocol.");
            }

            public virtual void OnOuterProtocolConnected()
            {
            }

            public virtual void OnInnerProtocolSet(INodeEndpointProtocolClient protocol)
            {
                this.innerProtocol = protocol;
            }
        }
    }
}
