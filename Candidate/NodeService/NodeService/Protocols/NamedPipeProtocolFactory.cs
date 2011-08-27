using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;

namespace NodeService.Protocols
{
    public class NamedPipeProtocolFactory : INodeEndpointProtocolFactory
    {
        public INodeEndpointProtocolServer CreateServer()
        {
            throw new NotImplementedException();
        }

        public INodeEndpointProtocolClient CreateClient()
        {
            throw new NotImplementedException();
        }
    }
}
