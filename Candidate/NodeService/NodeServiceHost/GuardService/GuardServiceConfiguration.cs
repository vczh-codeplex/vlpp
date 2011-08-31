using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using NodeService;
using NodeService.Protocols;

namespace NodeServiceHost.GuardService
{
    public static class GuardServiceConfiguration
    {
        public const string Address = "NodeServer";
        public const string EndpointName = "GuardService";

        public static INodeEndpointProtocolFactory CreateProtocolFactory()
        {
            return new NamedPipeProtocolFactory();
        }
    }
}
