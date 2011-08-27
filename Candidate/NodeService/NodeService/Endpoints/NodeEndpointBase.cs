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

        public NodeEndpointBase()
        {
            this.name = GetType()
                .GetCustomAttributes(typeof(NodeEndpointAttribute), false)
                .Cast<NodeEndpointAttribute>()
                .First()
                .Name;
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
