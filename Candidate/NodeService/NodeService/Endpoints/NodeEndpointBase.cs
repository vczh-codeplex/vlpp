using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading;

namespace NodeService.Endpoints
{
    public abstract class NodeEndpointBase : INodeEndpoint
    {
        private string name;

        public static string GetDefaultNodeEndpointName(Type type)
        {
            return type
                .GetCustomAttributes(typeof(NodeEndpointAttribute), false)
                .Cast<NodeEndpointAttribute>()
                .First()
                .Name;
        }

        public NodeEndpointBase()
        {
            this.name = GetDefaultNodeEndpointName(GetType());
        }

        public NodeEndpointBase(string name)
        {
            this.name = name;
        }

        public string EndpointName
        {
            get
            {
                return this.name;
            }
        }

        public bool EnableAsynchronization { get; set; }

        public virtual void QueueRequest(INodeEndpointRequest request)
        {
            if (this.EnableAsynchronization)
            {
                ThreadPool.QueueUserWorkItem(state =>
                {
                    OnQueueRequest((INodeEndpointRequest)state);
                }, request);
            }
            else
            {
                OnQueueRequest(request);
            }
        }

        public virtual void Dispose()
        {
        }

        protected abstract void OnQueueRequest(INodeEndpointRequest request);
    }

    [AttributeUsage(AttributeTargets.Class)]
    public class NodeEndpointAttribute : Attribute
    {
        public string Name { get; private set; }

        public NodeEndpointAttribute()
        {
        }

        public NodeEndpointAttribute(string name)
        {
            this.Name = name;
        }
    }
}
