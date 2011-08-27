using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;

namespace NodeService
{
    public interface INodeEndpointProtocol
    {
        bool Connected { get; }

        bool Connect(string address, string endpointName);
        void Disconnect();
    }
}
