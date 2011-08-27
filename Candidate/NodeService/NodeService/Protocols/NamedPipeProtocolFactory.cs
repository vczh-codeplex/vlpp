using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.IO.Pipes;
using System.Text.RegularExpressions;
using System.Threading;

namespace NodeService.Protocols
{
    public class NamedPipeProtocolFactory : INodeEndpointProtocolFactory
    {
        public INodeEndpointProtocolServerListener CreateServerListener()
        {
            return new ServerListener();
        }

        public INodeEndpointProtocolClient CreateClient()
        {
            return new Client();
        }

        class Request : INodeEndpointProtocolRequest
        {
            private ProtocolBase protocolBase;
            private string message;

            public Request(ProtocolBase protocolBase, string message)
            {
                this.protocolBase = protocolBase;
                this.message = message;
            }

            public bool CanRespond
            {
                get
                {
                    return true;
                }
            }

            public string Message
            {
                get
                {
                    return this.message;
                }
            }

            public void Respond(string response)
            {
                this.protocolBase.Send(response);
            }
        }

        abstract class ProtocolBase : INodeEndpointProtocol
        {
            protected PipeStream stream;
            private List<INodeEndpointProtocolRequestListener> listeners = new List<INodeEndpointProtocolRequestListener>();

            public bool EnableDuplex
            {
                get
                {
                    return true;
                }
            }

            public bool Connected
            {
                get
                {
                    return this.stream != null && this.stream.IsConnected;
                }
            }

            public INodeEndpointProtocol OuterProtocol
            {
                get
                {
                    return null;
                }
            }

            public abstract INodeEndpointProtocol InnerProtocol { get; }

            public void Disconnect()
            {
                if (this.stream != null)
                {
                    this.stream.Close();
                    this.stream.Dispose();
                    this.stream = null;
                }
            }

            public void BeginListen()
            {
                if (!this.Connected) throw new InvalidOperationException("The protocol is not connected.");
                byte[] lead = new byte[sizeof(int)];
                this.stream.BeginRead(lead, 0, lead.Length, r => ReadCallback(r, lead), null);
            }

            public void AddListener(INodeEndpointProtocolRequestListener listener)
            {
                lock (this.listeners)
                {
                    if (!this.listeners.Contains(listener))
                    {
                        this.listeners.Add(listener);
                    }
                }
            }

            public void RemoveListener(INodeEndpointProtocolRequestListener listener)
            {
                lock (this.listeners)
                {
                    this.listeners.Remove(listener);
                }
            }

            public void Send(string message)
            {
                if (!this.Connected) throw new InvalidOperationException("The protocol is not connected.");
                Write(message);
            }

            private void Write(string message)
            {
                byte[] lead = new byte[sizeof(int)];
                byte[] bytes = Encoding.UTF8.GetBytes(message);
                int length = bytes.Length;
                unsafe
                {
                    byte* plead = (byte*)&length;
                    for (int i = 0; i < sizeof(int); i++)
                    {
                        lead[i] = plead[i];
                    }
                }
                this.stream.Write(lead.Concat(bytes).ToArray(), 0, lead.Length + length);
            }

            private void ReadCallback(IAsyncResult asyncResult, byte[] lead)
            {
                int leadLength = this.stream.EndRead(asyncResult);
                if (leadLength == lead.Length)
                {
                    unsafe
                    {
                        fixed (byte* plead = lead)
                        {
                            leadLength = *(int*)plead;
                        }
                    }
                    byte[] bytes = new byte[leadLength];
                    int messageLength = this.stream.Read(bytes, 0, bytes.Length);
                    if (messageLength == leadLength)
                    {
                        string protocolMessage = Encoding.UTF8.GetString(bytes);
                        Request request = new Request(this, protocolMessage);
                        lock (this.listeners)
                        {
                            foreach (var listener in this.listeners)
                            {
                                listener.OnReceivedRequest(request);
                            }
                        }
                    }
                }
                BeginListen();
            }
        }

        class ServerListener : INodeEndpointProtocolServerListener
        {
            private string pipeName;

            public bool Connected
            {
                get
                {
                    return this.pipeName != null;
                }
            }

            public void Connect(string address, string endpointName)
            {
                if (this.Connected)
                {
                    Disconnect();
                }
                this.pipeName = address + "/" + endpointName;
            }

            public void Disconnect()
            {
                this.pipeName = null;
            }

            public INodeEndpointProtocolServer Listen()
            {
                NamedPipeServerStream serverStream = new NamedPipeServerStream(
                    this.pipeName,
                    PipeDirection.InOut,
                    NamedPipeServerStream.MaxAllowedServerInstances,
                    PipeTransmissionMode.Message,
                    PipeOptions.Asynchronous | PipeOptions.WriteThrough
                    );
                try
                {
                    serverStream.ReadMode = PipeTransmissionMode.Message;
                    serverStream.WaitForConnection();
                    if (serverStream.IsConnected)
                    {
                        return new Server(serverStream);
                    }
                }
                catch (Exception)
                {
                }

                if (serverStream != null)
                {
                    serverStream.Close();
                    serverStream.Disconnect();
                }
                return null;
            }
        }

        class Server : ProtocolBase, INodeEndpointProtocolServer
        {
            private INodeEndpointProtocolServer innerProtocol;

            public Server(NamedPipeServerStream stream)
            {
                this.stream = stream;
            }

            public void SetOuterProtocol(INodeEndpointProtocolServer protocol)
            {
                throw new InvalidOperationException("Named pipe protocol client cannot have a outer protocol.");
            }

            public void OnOuterProtocolListened()
            {
            }

            public void OnInnerProtocolSet(INodeEndpointProtocolServer protocol)
            {
                this.innerProtocol = protocol;
            }

            public override INodeEndpointProtocol InnerProtocol
            {
                get
                {
                    return this.innerProtocol;
                }
            }
        }

        class Client : ProtocolBase, INodeEndpointProtocolClient
        {
            private INodeEndpointProtocolClient innerProtocol;

            public bool Connect(string address, string endpointName, int timeout)
            {
                if (this.stream != null)
                {
                    Disconnect();
                }

                int index = address.IndexOf('/');
                string serverName = address.Substring(0, index);
                string pipeName = address.Substring(index + 1) + "/" + endpointName;
                NamedPipeClientStream clientStream = new NamedPipeClientStream(
                    serverName,
                    pipeName,
                    PipeDirection.InOut,
                    PipeOptions.Asynchronous | PipeOptions.WriteThrough
                    );
                try
                {
                    clientStream.Connect(timeout);
                    if (clientStream.IsConnected)
                    {
                        clientStream.ReadMode = PipeTransmissionMode.Message;
                        this.stream = clientStream;
                        if (this.innerProtocol != null)
                        {
                            this.innerProtocol.OnOuterProtocolConnected();
                        }
                        BeginListen();
                        return true;
                    }
                }
                catch (Exception)
                {
                }

                if (clientStream != null)
                {
                    clientStream.Close();
                    clientStream.Dispose();
                }
                return false;
            }

            public void SetOuterProtocol(INodeEndpointProtocolClient protocol)
            {
                throw new InvalidOperationException("Named pipe protocol client cannot have a outer protocol.");
            }

            public void OnOuterProtocolConnected()
            {
            }

            public void OnInnerProtocolSet(INodeEndpointProtocolClient protocol)
            {
                this.innerProtocol = protocol;
            }

            public override INodeEndpointProtocol InnerProtocol
            {
                get
                {
                    return this.innerProtocol;
                }
            }
        }
    }
}
