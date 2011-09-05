using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Xml.Linq;
using System.Threading.Tasks;
using System.Reflection;
using System.IO;

namespace NodeService.Endpoints
{
    public abstract class StrongTypedNodeEndpointClient : INodeEndpointClient
    {
        public INodeEndpointClientProvider Provider { get; private set; }
        public StrongTypedNodeEndpointSerializer Serializer { get; private set; }

        public StrongTypedNodeEndpointClient(INodeEndpointClientProvider provider)
        {
            this.Provider = provider;
        }

        public virtual void Dispose()
        {
            this.Provider.Disconnect();
        }

        protected void Initialize(Type interfaceType)
        {
            this.Serializer = new StrongTypedNodeEndpointSerializer();
            var methodInfos = StrongTypedNodeEndpointClientBuilder.GetMethodInfos(interfaceType);

            foreach (var methodInfo in methodInfos)
            {
                foreach (var type in methodInfo
                    .GetParameters()
                    .Select(p => p.ParameterType)
                    .Concat(new Type[] { methodInfo.ReturnType })
                    .Where(t => t != typeof(INodeEndpointRequest) && t != typeof(void) && t != typeof(Task) && t != typeof(Stream))
                    )
                {
                    if (type.IsGenericType && type.GetGenericTypeDefinition() == typeof(Task<>))
                    {
                        this.Serializer.AddDefaultSerializer(type.GetGenericArguments()[0]);
                    }
                    else
                    {
                        this.Serializer.AddDefaultSerializer(type);
                    }
                }
            }
        }

        protected object Execute(string method, Type type, string[] names, object[] arguments)
        {
            XElement body = new XElement("Parameters",
                names
                .Zip(arguments, (a, b) => Tuple.Create(a, b))
                .Select(t => new XElement(t.Item1, this.Serializer.Serialize(t.Item2)))
                );
            var response = this.Provider.Send(method, body);
            response.WaitForResponse();
            switch (response.RequestState)
            {
                case RequestState.ReceivedResponse:
                    if (type != typeof(void))
                    {
                        return this.Serializer.Deserialize(response.Response, type);
                    }
                    break;
                case RequestState.ReceivedStream:
                    if (type == typeof(Stream))
                    {
                        return response.Stream;
                    }
                    else if (type == typeof(byte[]))
                    {
                        return response.Stream.ReadAllBytesAndClose();
                    }
                    else if (type != typeof(void))
                    {
                        throw new InvalidOperationException("Streaming response should be get as a " + typeof(Stream).FullName + " or a byte array.");
                    }
                    break;
                case RequestState.RaisedException:
                    throw response.Exception;
            }
            return null;
        }

        protected Task ExecuteTask(string method, string[] names, object[] arguments)
        {
            Task task = new Task(() => Execute(method, typeof(void), names, arguments));
            task.Start();
            return task;
        }

        protected Task<T> ExecuteTask<T>(string method, string[] names, object[] arguments)
        {
            Task<T> task = new Task<T>(() => (T)Execute(method, typeof(T), names, arguments));
            task.Start();
            return task;
        }
    }

    public abstract class StrongTypedNodeEndpointClient<T> : StrongTypedNodeEndpointClient, IDuplexNodeEndpointClient<T>
        where T : INodeEndpoint
    {
        public virtual T Callback { get; set; }

        public StrongTypedNodeEndpointClient(INodeEndpointClientProvider provider)
            : base(provider)
        {
        }
    }
}
