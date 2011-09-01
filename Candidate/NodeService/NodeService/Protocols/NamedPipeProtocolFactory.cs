using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.IO.Pipes;
using System.Text.RegularExpressions;
using System.Threading;
using NodeService.Protocols.StreamBasedProtocolObjects;
using System.Xml.Linq;

namespace NodeService.Protocols
{
    public class NamedPipeProtocolFactory : INodeEndpointProtocolFactory
    {
        public INodeEndpointProtocolServerListener CreateServerListener()
        {
            return new ServerListener(this);
        }

        public INodeEndpointProtocolClient CreateClient()
        {
            return new Client(this);
        }

        public XElement[] GetFactoryDescription()
        {
            return new XElement[]
            { 
                new XElement("NamedPipeProtocolFactory")
            };
        }

        class ServerListener : INodeEndpointProtocolServerListener
        {
            private NamedPipeProtocolFactory factory;
            private string pipeName;

            public ServerListener(NamedPipeProtocolFactory factory)
            {
                this.factory = factory;
            }

            public bool Connected
            {
                get
                {
                    return this.pipeName != null;
                }
            }

            public INodeEndpointProtocolFactory Factory
            {
                get
                {
                    return this.factory;
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

            public INodeEndpointProtocolServer Listen(int timeout)
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
                        return new Server(serverStream, this);
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

        class Server : StreamServerProtocol<NamedPipeServerStream>
        {
            public Server(NamedPipeServerStream stream, ServerListener serverListener)
                : base(serverListener)
            {
                this.Stream = stream;
            }

            public override bool Connected
            {
                get
                {
                    return this.Stream != null && this.Stream.IsConnected;
                }
            }
        }

        class Client : StreamClientProtocol<NamedPipeClientStream>
        {
            public Client(NamedPipeProtocolFactory factory)
                : base(factory)
            {
            }

            public override bool Connect(string address, string endpointName, int timeout)
            {
                Disconnect();
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
                        this.Stream = clientStream;
                        if (this.InnerProtocol != null)
                        {
                            this.InnerProtocol.OnOuterProtocolConnected();
                        }
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

            public override bool Connected
            {
                get
                {
                    return this.Stream != null && this.Stream.IsConnected;
                }
            }
        }
    }
}
