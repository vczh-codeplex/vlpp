using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.IO.Pipes;
using System.Text.RegularExpressions;
using System.Threading;
using NodeService.Protocols.StreamBasedProtocolObjects;
using System.Net.Sockets;
using System.IO;
using System.Net;
using System.Security;
using System.Xml.Linq;

namespace NodeService.Protocols
{
    public class TcpProtocolFactory : INodeEndpointProtocolFactory
    {
        public AddressFamily AddressFamily { get; set; }
        public ProtocolType ProtocolType { get; set; }

        public TcpProtocolFactory()
        {
            this.AddressFamily = AddressFamily.InterNetwork;
            this.ProtocolType = ProtocolType.Tcp;
        }

        public INodeEndpointProtocolServerListener CreateServerListener()
        {
            return new ServerListener(this.AddressFamily, this.ProtocolType, this);
        }

        public INodeEndpointProtocolClient CreateClient()
        {
            return new Client(this.AddressFamily, this.ProtocolType, this);
        }

        public XElement[] GetFactoryDescription()
        {
            return new XElement[] 
            { 
                new XElement("TcpProtocolFactory",
                    new XElement("AddressFamily", this.AddressFamily.ToString()),
                    new XElement("ProtocolType", this.ProtocolType.ToString())
                    )
            };
        }

        class ServerListener : INodeEndpointProtocolServerListener
        {
            private TcpProtocolFactory factory;
            private Socket listenedSocket = null;
            private AddressFamily addressFamily;
            private ProtocolType protocolType;

            public ServerListener(AddressFamily addressFamily, ProtocolType protocolType, TcpProtocolFactory factory)
            {
                this.addressFamily = addressFamily;
                this.protocolType = protocolType;
                this.factory = factory;
            }

            public bool Connected
            {
                get
                {
                    return this.listenedSocket != null;
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
                this.listenedSocket = new Socket(this.addressFamily, SocketType.Stream, this.protocolType);

                foreach (IPAddress hostIP in Dns.GetHostAddresses(Environment.MachineName))
                {
                    IPEndPoint endpoint = new IPEndPoint(hostIP, int.Parse(address));
                    try
                    {
                        this.listenedSocket.Bind(endpoint);
                        return;
                    }
                    catch (SocketException)
                    {
                    }
                    catch (SecurityException)
                    {
                    }
                }
                Disconnect();
            }

            public void Disconnect()
            {
                if (this.listenedSocket != null)
                {
                    this.listenedSocket.Close();
                    this.listenedSocket.Dispose();
                    this.listenedSocket = null;
                }
            }

            public INodeEndpointProtocolServer Listen(int timeout)
            {
                try
                {
                    this.listenedSocket.Listen((int)SocketOptionName.MaxConnections);
                    Socket acceptedSocket = this.listenedSocket.Accept();
                    return new Server(acceptedSocket, this);
                }
                catch (SocketException)
                {
                }
                return null;
            }
        }

        class Server : StreamServerProtocol<NetworkStream>
        {
            private Socket acceptedSocket;

            public Server(Socket acceptedSocket, ServerListener serverListener)
                : base(serverListener)
            {
                this.acceptedSocket = acceptedSocket;
                this.Stream = new NetworkStream(this.acceptedSocket, FileAccess.ReadWrite, false);
            }

            public override bool Connected
            {
                get
                {
                    return this.acceptedSocket != null && this.acceptedSocket.Connected;
                }
            }

            public override void Disconnect()
            {
                base.Disconnect();
                if (this.acceptedSocket != null)
                {
                    this.acceptedSocket.Close();
                    this.acceptedSocket.Dispose();
                    this.acceptedSocket = null;
                }
            }
        }

        class Client : StreamClientProtocol<NetworkStream>
        {
            private Socket connectedSocket;
            private AddressFamily addressFamily;
            private ProtocolType protocolType;

            public Client(AddressFamily addressFamily, ProtocolType protocolType, TcpProtocolFactory factory)
                : base(factory)
            {
                this.addressFamily = addressFamily;
                this.protocolType = protocolType;
            }

            public override bool Connect(string address, string endpointName, int timeout)
            {
                Disconnect();
                try
                {
                    int index = address.IndexOf(':');
                    string addressName = address.Substring(0, index);
                    int addressPort = int.Parse(address.Substring(index + 1));

                    Socket socket = new Socket(this.addressFamily, SocketType.Stream, this.protocolType);
                    try
                    {
                        socket.Connect(addressName, addressPort);
                        this.connectedSocket = socket;
                        this.Stream = new NetworkStream(this.connectedSocket, FileAccess.ReadWrite, false);
                        return true;
                    }
                    catch (SocketException)
                    {
                        socket.Close();
                    }
                }
                catch (ArgumentOutOfRangeException)
                {
                }
                catch (FormatException)
                {
                }
                catch (OverflowException)
                {
                }

                return false;
            }

            public override bool Connected
            {
                get
                {
                    return this.connectedSocket != null && this.connectedSocket.Connected;
                }
            }

            public override void Disconnect()
            {
                base.Disconnect();
                if (this.connectedSocket != null)
                {
                    this.connectedSocket.Close();
                    this.connectedSocket.Dispose();
                    this.connectedSocket = null;
                }
            }
        }
    }
}
