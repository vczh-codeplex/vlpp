using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Xml.Linq;

namespace NodeService
{
    public interface INodeEndpointClientProvider
    {
        bool EnableAsynchronization { get; }
        INodeEndpointProtocol Protocol { get; set; }
        INodeEndpointResponse Send(string method, XElement body);
    }
}