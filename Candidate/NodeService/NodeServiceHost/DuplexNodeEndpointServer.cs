using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using NodeService;

namespace NodeServiceHost
{
    public class DuplexNodeEndpointServer<T, U> : NodeEndpointServerBase<T>
        where T : IDuplexNodeEndpoint<U>
        where U : INodeEndpointClient
    {
        protected override INodeEndpointProtocolServer CreateProtocolServer(T endpoint, int timeout)
        {
            return this.ServerListener.WaitForServer(this.Callback.ProtocolAddress, endpoint, timeout);
        }
    }
}
