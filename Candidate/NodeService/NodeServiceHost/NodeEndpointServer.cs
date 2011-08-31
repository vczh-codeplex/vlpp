using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using NodeService;

namespace NodeServiceHost
{
    public class NodeEndpointServer<T> : NodeEndpointServerBase<T>
        where T : INodeEndpoint
    {
        protected override INodeEndpointProtocolServer CreateProtocolServer(T endpoint, int timeout)
        {
            return this.ServerListener.WaitForServer(this.Callback.ProtocolAddress, endpoint, timeout);
        }
    }
}
