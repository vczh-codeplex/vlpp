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

        public static IGuardService ConnectGuardServiceFacade<TService, TCallback>()
            where TService : INodeEndpoint
            where TCallback : INodeEndpointServerCallback<TService>, new()
        {
            var serverCallback = new TCallback();
            var guardServiceCallback = new NodeEndpointGuardServiceCallback<TService>(serverCallback);
            var guardService = GuardServiceConfiguration.CreateProtocolFactory()
                .WaitForClient<
                    IGuardService<
                        NodeEndpointGuardServiceCallback<TService>>,
                        NodeEndpointGuardServiceCallback<TService>
                    >(
                    "localhost/" + GuardServiceConfiguration.Address,
                    GuardServiceConfiguration.EndpointName,
                    guardServiceCallback
                    );
            return guardService;
        }

        public static IGuardService ConnectGuardServiceFacade<TDuplexService, TDuplexCallback, TCallback>()
            where TDuplexService : IDuplexNodeEndpoint<TDuplexCallback>
            where TDuplexCallback : INodeEndpointClient
            where TCallback : INodeEndpointServerCallback<TDuplexService>, new()
        {
            var serverCallback = new TCallback();
            var guardServiceCallback = new DuplexNodeEndpointGuardServiceCallback<TDuplexService, TDuplexCallback>(serverCallback);
            var guardService = GuardServiceConfiguration.CreateProtocolFactory()
                .WaitForClient<
                    IGuardService<
                        DuplexNodeEndpointGuardServiceCallback<TDuplexService, TDuplexCallback>>,
                        DuplexNodeEndpointGuardServiceCallback<TDuplexService, TDuplexCallback>
                    >(
                    "localhost/" + GuardServiceConfiguration.Address,
                    GuardServiceConfiguration.EndpointName,
                    guardServiceCallback
                    );
            return guardService;
        }
    }
}
