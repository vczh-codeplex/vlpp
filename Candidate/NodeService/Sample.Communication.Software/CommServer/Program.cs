using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using NodeService.Endpoints;
using NodeService;
using System.Threading;
using NodeService.Protocols;

namespace CommServer
{
    class Program
    {
        public static List<Tuple<INodeEndpointProtocolServer, CommunicationService>> loggedInServices = new List<Tuple<INodeEndpointProtocolServer, CommunicationService>>();

        public static void SendMessageToAll(string message)
        {
            Console.WriteLine(message);
            List<CommunicationService> copied = null;
            lock (loggedInServices)
            {
                for (int i = loggedInServices.Count - 1; i >= 0; i--)
                {
                    if (!loggedInServices[i].Item1.Connected)
                    {
                        loggedInServices.RemoveAt(i);
                    }
                }
                copied = new List<CommunicationService>(loggedInServices.Select(s => s.Item2));
            }

            ThreadPool.QueueUserWorkItem((o) =>
            {
                foreach (var service in copied)
                {
                    service.Callback.OnMessage(message);
                }
            });
        }

        static void Main(string[] args)
        {
            INodeEndpointProtocolFactory factory = new NamedPipeProtocolFactory();
            INodeEndpointProtocolServerListener listener = factory.CreateServerListener();

            Thread serverThread = new Thread(() =>
            {
                while (true)
                {
                    CommunicationService service = new CommunicationService();
                    INodeEndpointProtocolServer server = listener.WaitForServer("VczhCommunication", service);
                    lock (loggedInServices)
                    {
                        loggedInServices.Add(Tuple.Create(server, service));
                    }
                    SendMessageToAll("Someone logged in");
                }
            });
            serverThread.Start();

            Console.WriteLine("Press [ENTER] to exit.");
            Console.Read();

            serverThread.Abort();
        }
    }

    public interface ICommunicationCallback : INodeEndpointClient
    {
        string GetDisplayName();
        void OnMessage(string message);
    }

    [NodeEndpoint("CommunicaitonService")]
    public class CommunicationService : StrongTypedNodeEndpoint, IDuplexNodeEndpoint<ICommunicationCallback>
    {
        public ICommunicationCallback Callback { get; set; }

        public CommunicationService()
        {
            this.EnableAsynchronization = true;
        }

        [NodeEndpointMethod]
        public void Talk(string message)
        {
            Program.SendMessageToAll(Callback.GetDisplayName() + " says: " + message);
        }

        [NodeEndpointMethod]
        public void ChangeDisplayName(string oldDisplayName)
        {
            Program.SendMessageToAll(oldDisplayName + " changed display name: " + Callback.GetDisplayName());
        }
    }
}
