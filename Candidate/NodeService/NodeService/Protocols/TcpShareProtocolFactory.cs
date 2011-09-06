using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Xml.Linq;
using System.Net.Sockets;

namespace NodeService.Protocols
{
    public class TcpShareProtocolFactory : INodeEndpointProtocolFactory
    {
        public static string GetInternalPipeAddress(int port)
        {
            return "TcpShareServices/" + port.ToString();
        }

        public INodeEndpointProtocolServerListener CreateServerListener()
        {
            return new ServerListener(this);
        }

        public INodeEndpointProtocolClient CreateClient()
        {
            throw new NotImplementedException();
        }

        public XElement[] GetFactoryDescription()
        {
            return new XElement[]
            { 
                new XElement("TcpShareProtocolFactory")
            };
        }

        class ServerListener : NamedPipeProtocolFactory.ServerListener
        {
            public ServerListener(INodeEndpointProtocolFactory factory)
                : base(factory)
            {
            }

            public override void Connect(string address, string endpointName)
            {
                try
                {
                    int port = int.Parse(address);
                    base.Connect(GetInternalPipeAddress(port), endpointName);
                }
                catch (FormatException)
                {
                }
            }
        }

        class Client : TcpProtocolFactory.Client
        {
            public Client(INodeEndpointProtocolFactory factory)
                : base(AddressFamily.InterNetwork, ProtocolType.Tcp, factory)
            {
            }

            public override bool Connect(string address, string endpointName, int timeout)
            {
                if (base.Connect(address, endpointName, timeout))
                {
                    byte[] endpointBytes = ("[CONNECTENDPOINT]" + endpointName).NodeServiceEncode();
                    byte[] installed = InstallLeadBytes(endpointBytes);
                    this.Stream.Write(installed, 0, installed.Length);
                    this.Stream.Flush();

                    byte[] lead = new byte[sizeof(int)];
                    if (this.Stream.Read(lead, 0, lead.Length) == lead.Length)
                    {
                        byte[] response = UninstallLeadBytes(lead, this.Stream);
                        if (response.NodeServiceDecode() == "[CONNECTED]")
                        {
                            return true;
                        }
                    }
                    Disconnect();
                }
                return false;
            }
        }
    }
}
