using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Xml.Linq;

namespace NodeService.Protocols
{
    public class TranslatorProtocolFactory : INodeEndpointProtocolFactory
    {
        public INodeEndpointProtocolFactory OuterFactory { get; private set; }
        public ITranslatorProtocolHandlerFactory HandlerFactory { get; private set; }

        public TranslatorProtocolFactory(INodeEndpointProtocolFactory outerFactory, ITranslatorProtocolHandlerFactory handlerFactory)
        {
            this.OuterFactory = outerFactory;
            this.HandlerFactory = handlerFactory;
        }

        public INodeEndpointProtocolServerListener CreateServerListener()
        {
            return new ServerListener(this.OuterFactory.CreateServerListener(), this.HandlerFactory, this);
        }

        public INodeEndpointProtocolClient CreateClient()
        {
            return new Client(this.OuterFactory.CreateClient(), this.HandlerFactory, this);
        }

        public XElement[] GetFactoryDescription()
        {
            return new XElement[]
            { 
                new XElement("TranslatorProtocolFactory",
                    this.HandlerFactory.GetFactoryDescription()
                    )
            }
            .Concat(this.OuterFactory.GetFactoryDescription())
            .ToArray();
        }

        class Request : INodeEndpointProtocolRequest
        {
            private INodeEndpointProtocolRequest request;
            private ITranslatorProtocolHandler handler;
            private byte[] message;

            public Request(INodeEndpointProtocolRequest request, ITranslatorProtocolHandler handler)
            {
                this.handler = handler;
                this.request = request;
                this.message = this.handler.Decode(this.request.Message);
            }

            public bool CanRespond
            {
                get
                {
                    return this.request.CanRespond;
                }
            }

            public byte[] Message
            {
                get
                {
                    return this.message;
                }
            }

            public void Respond(byte[] message)
            {
                this.request.Respond(this.handler.Encode(message));
            }
        }

        class RequestListener : INodeEndpointProtocolRequestListener
        {
            private ProtocolBase protocolBase;

            public RequestListener(ProtocolBase protocolBase)
            {
                this.protocolBase = protocolBase;
            }

            public void OnReceivedRequest(INodeEndpointProtocolRequest request)
            {
                ITranslatorProtocolHandler handler = this.protocolBase.Handler;
                bool pass = false;
                try
                {
                    pass = handler.Pass(request);
                }
                catch (Exception)
                {
                    this.protocolBase.Disconnect();
                }
                if (pass)
                {
                    Request translatedRequest = new Request(request, handler);
                    lock (this.protocolBase.Listeners)
                    {
                        foreach (var listener in this.protocolBase.Listeners)
                        {
                            listener.OnReceivedRequest(translatedRequest);
                        }
                    }
                }
            }
        }

        class ServerListener : INodeEndpointProtocolServerListener
        {
            private TranslatorProtocolFactory factory;
            private INodeEndpointProtocolServerListener outerServerListener;
            private ITranslatorProtocolHandlerFactory handlerFactory;

            public ServerListener(INodeEndpointProtocolServerListener outerServerListener, ITranslatorProtocolHandlerFactory handlerFactory, TranslatorProtocolFactory factory)
            {
                this.factory = factory;
                this.outerServerListener = outerServerListener;
                this.handlerFactory = handlerFactory;
            }

            public bool Connected
            {
                get
                {
                    return this.outerServerListener.Connected;
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
                this.outerServerListener.Connect(address, endpointName);
            }

            public void Disconnect()
            {
                this.outerServerListener.Disconnect();
            }

            public INodeEndpointProtocolServer Listen(int timeout)
            {
                INodeEndpointProtocolServer server = this.outerServerListener.Listen(timeout);
                if (server == null)
                {
                    return null;
                }
                else
                {
                    return new Server(server, this.handlerFactory, this);
                }
            }
        }

        class ProtocolBase : INodeEndpointProtocol
        {
            private TranslatorProtocolFactory factory;
            private INodeEndpointProtocol parentProtocol;
            private RequestListener requestListener;

            public ITranslatorProtocolHandler Handler { get; protected set; }
            public List<INodeEndpointProtocolRequestListener> Listeners { get; private set; }

            public ProtocolBase(TranslatorProtocolFactory factory)
            {
                this.factory = factory;
                this.Listeners = new List<INodeEndpointProtocolRequestListener>();
                this.requestListener = new RequestListener(this);
            }

            public INodeEndpointProtocol ParentProtocol
            {
                get
                {
                    return this.parentProtocol;
                }
                set
                {
                    if (this.parentProtocol != null)
                    {
                        this.parentProtocol.RemoveListener(this.requestListener);
                    }
                    this.parentProtocol = value;
                    if (this.parentProtocol != null)
                    {
                        this.parentProtocol.AddListener(this.requestListener);
                    }
                }
            }

            public bool EnableDuplex
            {
                get
                {
                    return this.ParentProtocol.EnableDuplex;
                }
            }

            public bool Connected
            {
                get
                {
                    return this.ParentProtocol.Connected;
                }
            }

            public INodeEndpointProtocolFactory Factory
            {
                get
                {
                    return this.factory;
                }
            }

            public void Disconnect()
            {
                this.ParentProtocol.Disconnect();
            }

            public void BeginListen()
            {
                this.ParentProtocol.BeginListen();
            }

            public void AddListener(INodeEndpointProtocolRequestListener listener)
            {
                lock (this.Listeners)
                {
                    if (!this.Listeners.Contains(listener))
                    {
                        this.Listeners.Add(listener);
                    }
                }
            }

            public void RemoveListener(INodeEndpointProtocolRequestListener listener)
            {
                lock (this.Listeners)
                {
                    this.Listeners.Remove(listener);
                }
            }

            public INodeEndpointProtocolRequestListener[] GetListeners()
            {
                lock (this.Listeners)
                {
                    return this.Listeners.ToArray();
                }
            }

            public void Send(byte[] message)
            {
                this.parentProtocol.Send(this.Handler.Encode(message));
            }
        }

        class Server : ProtocolBase, INodeEndpointProtocolServer
        {
            private ServerListener serverListener;
            private INodeEndpointProtocolServer outerProtocol;
            private INodeEndpointProtocolServer innerProtocol;
            private ITranslatorProtocolServerHandler serverHandler;

            public Server(INodeEndpointProtocolServer outerProtocol, ITranslatorProtocolHandlerFactory handlerFactory, ServerListener serverListener)
                : base((TranslatorProtocolFactory)serverListener.Factory)
            {
                this.serverListener = serverListener;
                this.serverHandler = handlerFactory.CreateServerHandler();
                SetOuterProtocol(outerProtocol);
                this.Handler = this.serverHandler;
                this.serverHandler.AttachServer(this);
            }

            public INodeEndpointProtocolServer OuterProtocol
            {
                get
                {
                    return this.outerProtocol;
                }
            }

            public INodeEndpointProtocolServer InnerProtocol
            {
                get
                {
                    return this.innerProtocol;
                }
            }

            public INodeEndpointProtocolServerListener ServerListener
            {
                get
                {
                    return this.serverListener;
                }
            }

            public void SetOuterProtocol(INodeEndpointProtocolServer protocol)
            {
                if (this.outerProtocol != null)
                {
                    this.outerProtocol.OnInnerProtocolSet(null);
                }
                this.outerProtocol = protocol;
                this.ParentProtocol = protocol;
                if (this.outerProtocol != null)
                {
                    this.outerProtocol.OnInnerProtocolSet(this);
                }
            }

            public void OnInnerProtocolSet(INodeEndpointProtocolServer protocol)
            {
                this.innerProtocol = protocol;
            }
        }

        class Client : ProtocolBase, INodeEndpointProtocolClient
        {
            private INodeEndpointProtocolClient outerProtocol;
            private INodeEndpointProtocolClient innerProtocol;
            private ITranslatorProtocolClientHandler clientHandler;

            public Client(INodeEndpointProtocolClient outerProtocol, ITranslatorProtocolHandlerFactory handlerFactory, TranslatorProtocolFactory factory)
                : base(factory)
            {
                this.clientHandler = handlerFactory.CreateClientHandler();
                SetOuterProtocol(outerProtocol);
                this.Handler = this.clientHandler;
                this.clientHandler.AttachClient(this);
            }

            public INodeEndpointProtocolClient OuterProtocol
            {
                get
                {
                    return this.outerProtocol;
                }
            }

            public INodeEndpointProtocolClient InnerProtocol
            {
                get
                {
                    return this.innerProtocol;
                }
            }

            public bool Connect(string address, string endpointName, int timeout)
            {
                return this.outerProtocol.Connect(address, endpointName, timeout);
            }

            public void SetOuterProtocol(INodeEndpointProtocolClient protocol)
            {
                if (this.outerProtocol != null)
                {
                    this.outerProtocol.OnInnerProtocolSet(null);
                }
                this.outerProtocol = protocol;
                this.ParentProtocol = protocol;
                if (this.outerProtocol != null)
                {
                    this.outerProtocol.OnInnerProtocolSet(this);
                }
            }


            public void OnOuterProtocolConnected()
            {
                if (this.innerProtocol != null)
                {
                    this.innerProtocol.OnOuterProtocolConnected();
                }
                this.clientHandler.OnClientConnected();
            }

            public void OnInnerProtocolSet(INodeEndpointProtocolClient protocol)
            {
                this.innerProtocol = protocol;
            }
        }
    }

    public interface ITranslatorProtocolHandlerFactory
    {
        ITranslatorProtocolServerHandler CreateServerHandler();
        ITranslatorProtocolClientHandler CreateClientHandler();
        XElement[] GetFactoryDescription();
    }

    public interface ITranslatorProtocolHandler
    {
        byte[] Decode(byte[] bytes);
        byte[] Encode(byte[] bytes);
        bool Pass(INodeEndpointProtocolRequest request);
    }

    public interface ITranslatorProtocolServerHandler : ITranslatorProtocolHandler
    {
        void AttachServer(INodeEndpointProtocolServer server);
    }

    public interface ITranslatorProtocolClientHandler : ITranslatorProtocolHandler
    {
        void AttachClient(INodeEndpointProtocolClient client);
        void OnClientConnected();
    }

    public interface ITranslatorProtocolHandlerSimple
    {
        byte[] Decode(byte[] bytes);
        byte[] Encode(byte[] bytes);
        XElement[] GetHandlerDescription();
    }

    public class TranslatorProtocolHandlerFactorySimple : ITranslatorProtocolHandlerFactory
    {
        private ITranslatorProtocolHandlerSimple handler;

        public TranslatorProtocolHandlerFactorySimple(ITranslatorProtocolHandlerSimple handler)
        {
            this.handler = handler;
        }

        public ITranslatorProtocolServerHandler CreateServerHandler()
        {
            return new Handler(this.handler);
        }

        public ITranslatorProtocolClientHandler CreateClientHandler()
        {
            return new Handler(this.handler);
        }

        public XElement[] GetFactoryDescription()
        {
            return new XElement[]
            {
                new XElement("TranslatorProtocolHandlerFactorySimple",
                    this.handler.GetHandlerDescription()
                    )
            };
        }

        public class Handler : ITranslatorProtocolServerHandler, ITranslatorProtocolClientHandler
        {
            private ITranslatorProtocolHandlerSimple handler;

            public Handler(ITranslatorProtocolHandlerSimple handler)
            {
                this.handler = handler;
            }

            public byte[] Decode(byte[] bytes)
            {
                return this.handler.Decode(bytes);
            }

            public byte[] Encode(byte[] bytes)
            {
                return this.handler.Encode(bytes);
            }

            public bool Pass(INodeEndpointProtocolRequest request)
            {
                return true;
            }

            public void AttachServer(INodeEndpointProtocolServer server)
            {
            }

            public void AttachClient(INodeEndpointProtocolClient client)
            {
            }

            public void OnClientConnected()
            {
            }
        }
    }
}
