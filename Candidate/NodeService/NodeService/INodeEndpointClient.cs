using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;

namespace NodeService
{
    public interface INodeEndpointClient : IDisposable
    {
        INodeEndpointClientProvider Provider { get; }
    }
}
