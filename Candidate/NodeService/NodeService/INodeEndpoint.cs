using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;

namespace NodeService
{
    public interface INodeEndpoint
    {
        string EndpointName { get; }

        void QueueRequest(INodeEndpointRequest request);
    }
}
