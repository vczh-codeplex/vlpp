using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.IO.Pipes;

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

        abstract class Pipe : INodeEndpointProtocol, INodeEndpointProtocolListener, INodeEndpointProtocolSender
        {
            protected PipeStream stream;

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

            public INodeEndpointProtocolListener ProtocolListener
            {
                get
                {
                    return this.Connected ? this : null;
                }
            }

            public INodeEndpointProtocolSender ProtocolSender
            {
                get
                {
                    return this.Connected ? this : null;
                }
            }

            public void Disconnect()
            {
                if (this.stream != null)
                {
                    this.stream.Close();
                    this.stream.Dispose();
                    this.stream = null;
                }
            }

            public INodeEndpointProtocolRequestListener Listener { get; set; }

            public INodeEndpointProtocolResponse Send(string method, string message)
            {
                Guid guid = Guid.NewGuid();
                string pipeMessage = "[REQUEST][" + guid.ToString() + "][" + method + "]" + message;
                return null;
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
                NamedPipeServerStream stream = new NamedPipeServerStream(this.pipeName, PipeDirection.InOut, 255, PipeTransmissionMode.Message, PipeOptions.Asynchronous | PipeOptions.WriteThrough);
                try
                {
                    stream.WaitForConnection();
                    if (stream.IsConnected)
                    {
                        return new Server(stream);
                    }
                }
                catch (Exception)
                {
                }

                if (stream != null)
                {
                    stream.Close();
                    stream.Disconnect();
                }
                return null;
            }
        }

        class Server : Pipe, INodeEndpointProtocolServer
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

        class Client : Pipe, INodeEndpointProtocolClient
        {
            private INodeEndpointProtocolClient innerProtocol;

            public bool Connect(string address, string endpointName, int timeout)
            {
                if (this.stream != null)
                {
                    Disconnect();
                }

                int index = address.IndexOf('/');
                string serverName = address.Substring(0, index - 1);
                string pipeName = address.Substring(index + 1) + "/" + endpointName;
                NamedPipeClientStream clientStream = new NamedPipeClientStream(serverName, pipeName, PipeDirection.InOut, PipeOptions.Asynchronous | PipeOptions.WriteThrough);
                try
                {
                    clientStream.Connect(timeout);
                    if (this.stream.IsConnected)
                    {
                        this.stream = clientStream;
                        if (this.innerProtocol != null)
                        {
                            this.innerProtocol.OnOuterProtocolConnected();
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
