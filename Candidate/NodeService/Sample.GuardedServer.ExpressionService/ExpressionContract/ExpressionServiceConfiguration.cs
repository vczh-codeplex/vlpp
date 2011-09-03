using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using NodeService;
using NodeService.Protocols;

namespace ExpressionContract
{
    public static class ExpressionServiceConfiguration
    {
        public const string ProtocolName = "GuardedServiceTest";
        public const string EndpointName = "ExpressionService";

        public static INodeEndpointProtocolFactory CreateProtocolFactory()
        {
            return new NamedPipeProtocolFactory()
                .With(new GzipProtocolHandler())
                ;
        }
    }
}
