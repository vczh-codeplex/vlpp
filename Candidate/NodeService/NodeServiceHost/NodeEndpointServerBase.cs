using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using NodeService;
using System.Threading;
using System.Xml.Linq;
using NodeService.Endpoints;
using System.Reflection;

namespace NodeServiceHost
{
    public abstract class NodeEndpointServerBase<T> : INodeEndpointServer<T>
        where T : INodeEndpoint
    {
        private NodeEndpointServerState serverState = NodeEndpointServerState.Ready;
        private INodeEndpointServerCallback<T> callback = null;

        public INodeEndpointProtocolFactory ProtocolFactory { get; private set; }
        public INodeEndpointProtocolServerListener ServerListener { get; private set; }

        private volatile bool needToStop = false;
        private Thread serverListenerThread = null;
        private List<Tuple<T, INodeEndpointProtocolServer>> runningServices = new List<Tuple<T, INodeEndpointProtocolServer>>();

        protected abstract INodeEndpointProtocolServer CreateProtocolServer(T endpoint, int timeout);

        private void ClearServices(bool disconnectedOnly)
        {
            lock (this.runningServices)
            {
                for (int i = this.runningServices.Count - 1; i >= 0; i--)
                {
                    var pair = this.runningServices[i];
                    T endpoint = pair.Item1;
                    INodeEndpointProtocolServer server = pair.Item2;

                    if (!disconnectedOnly || !server.Connected)
                    {
                        this.callback.OnEndpointStopped(endpoint, server);
                        server.Disconnect();
                        endpoint.Dispose();
                        this.runningServices.RemoveAt(i);
                    }
                }
            }
        }

        private void ServerListenerThreadProc()
        {
            while (!this.needToStop)
            {
                T endpoint = this.callback.CreateEndpoint();
                INodeEndpointProtocolServer server = CreateProtocolServer(endpoint, NodeEndpointProtocolFactoryExtension.DefaultTimeout);
                if (server == null)
                {
                    endpoint.Dispose();
                }
                else
                {
                    lock (this.runningServices)
                    {
                        this.runningServices.Add(Tuple.Create(endpoint, server));
                        this.callback.OnEndpointStart(endpoint, server);
                    }
                }
                ClearServices(true);
            }
        }

        public NodeEndpointServerState ServerState
        {
            get
            {
                return this.serverState;
            }
        }

        public INodeEndpointServerCallback<T> Callback
        {
            get
            {
                return this.callback;
            }
        }

        public virtual void Start(INodeEndpointServerCallback<T> callback)
        {
            if (this.serverState != NodeEndpointServerState.Ready)
            {
                throw new InvalidOperationException("The server cannot be started.");
            }

            this.callback = callback;
            this.ProtocolFactory = this.callback.ProtocolFactory;
            this.ServerListener = this.ProtocolFactory.CreateServerListener();

            this.serverListenerThread = new Thread(ServerListenerThreadProc);
            this.serverListenerThread.Start();

            this.serverState = NodeEndpointServerState.Running;
        }

        public virtual void Stop()
        {
            if (this.serverState != NodeEndpointServerState.Running)
            {
                throw new InvalidOperationException("The server cannot be stopped.");
            }

            this.needToStop = true;
            if (!this.serverListenerThread.Join(NodeEndpointProtocolFactoryExtension.DefaultTimeout))
            {
                this.serverListenerThread.Abort();
            }
            ClearServices(false);
            this.ServerListener.Disconnect();
            this.ServerListener = null;
            this.serverState = NodeEndpointServerState.Stopped;
        }

        #region Contract Builder

        private static void CollectType(MethodInfo methodInfo, List<Type> types)
        {
            if (methodInfo.ReturnType != typeof(void))
            {
                CollectType(methodInfo.ReturnType, types);
            }
            foreach (var parameterInfo in methodInfo.GetParameters())
            {
                CollectType(parameterInfo.ParameterType, types);
            }
        }

        private static bool IsDataType(Type type)
        {
            return type.GetCustomAttributes(typeof(NodeEndpointDataTypeAttribute), false).Length > 0;
        }

        private static void CollectType(Type type, List<Type> types)
        {
            if (!types.Contains(type))
            {
                types.Add(type);
                if (IsDataType(type))
                {
                    foreach (NodeEndpointKnownTypeAttribute att in type.GetCustomAttributes(typeof(NodeEndpointKnownTypeAttribute), true))
                    {
                        CollectType(att.KnownType, types);
                    }
                }
            }
        }

        private static string GetTypeName(Type type)
        {
            return IsDataType(type) ? type.Name : type.FullName;
        }

        private static XElement GetTypeContractDescription(Type type)
        {
            if (IsDataType(type))
            {
                return new XElement(
                    "Type",
                    new XElement("BaseType", GetTypeName(type.BaseType)),
                    new XElement(
                        "KnownTypes",
                        type.GetCustomAttributes(typeof(NodeEndpointKnownTypeAttribute), true)
                        .Cast<NodeEndpointKnownTypeAttribute>()
                        .Select(att => new XElement("KnownType", GetTypeName(att.KnownType)))
                        .ToArray()
                        ),
                    new XElement(
                        "Members",
                        type
                        .GetFields(BindingFlags.Public | BindingFlags.NonPublic | BindingFlags.Instance)
                        .Where(f => f.GetCustomAttributes(typeof(NodeEndpointDataMemberAttribute), false).Length > 0)
                        .Where(f => f.DeclaringType == type)
                        .Select(f => new XElement(
                                "Member",
                                new XElement("Name", f.Name),
                                new XElement("Type", GetTypeName(f.FieldType))
                                )
                            )
                        .Concat(
                            type
                            .GetProperties(BindingFlags.Public | BindingFlags.NonPublic | BindingFlags.Instance)
                            .Where(f => f.GetCustomAttributes(typeof(NodeEndpointDataMemberAttribute), false).Length > 0)
                            .Where(f => f.DeclaringType == type)
                            .Select(f => new XElement(
                                    "Member",
                                    new XElement("Name", f.Name),
                                    new XElement("Type", GetTypeName(f.PropertyType))
                                    )
                                )
                            )
                        )
                    );
            }
            else
            {
                return null;
            }
        }

        private static XElement GetMethodContractDescription(MethodInfo methodInfo)
        {
            return new XElement(
                "Method",
                new XElement("Name", methodInfo.Name),
                new XElement("ReturnType", GetTypeName(methodInfo.ReturnType)),
                new XElement(
                    "Parameters",
                    methodInfo.GetParameters().Select(
                        p => new XElement(
                            "Parameter",
                            new XElement("Name", p.Name),
                            new XElement("Type", GetTypeName(p.ParameterType))
                            )
                    )
                    .ToArray()
                    )
                );
        }

        private static XElement GetContractDescription(Type serviceType)
        {
            List<Type> types = new List<Type>();
            MethodInfo[] methodInfos = StrongTypedNodeEndpoint.GetMethodInfos(serviceType);
            foreach (var methodInfo in methodInfos)
            {
                CollectType(methodInfo, types);
            }

            return new XElement(
                "Contract",
                new XElement("Methods", methodInfos.Select(GetMethodContractDescription).ToArray()),
                new XElement("Types", types.Select(GetTypeContractDescription).Where(e => e != null).ToArray())
                );
        }

        #endregion

        public virtual XElement GetServiceDescription()
        {
            string endpointName = this.callback.EndpointName;
            if (endpointName == null)
            {
                endpointName = NodeEndpointBase.GetDefaultNodeEndpointName(typeof(T));
            }
            return new XElement(
                "ServiceDescription",
                new XElement(
                    "Protocol",
                    this.ProtocolFactory.GetFactoryDescription()
                    ),
                new XElement(
                    "Address",
                    new XElement("ProtocolAddress", this.callback.ProtocolAddress),
                    new XElement("EndpointName", endpointName)
                    ),
                GetContractDescription(typeof(T))
                );
        }
    }
}
