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

    public static class GuardServiceStarter<TService, TCallback>
        where TService : INodeEndpoint
        where TCallback : INodeEndpointServerCallback<TService>, new()
    {
        public interface IGuardServiceInterface : IGuardService, IDuplexNodeEndpointClient<NodeEndpointGuardServiceCallback<TService>>
        {
        }

        public static IGuardService ConnectGuardServiceFacade()
        {
            var serverCallback = new TCallback();
            var guardServiceCallback = new NodeEndpointGuardServiceCallback<TService>(serverCallback);
            var guardService = GuardServiceConfiguration.CreateProtocolFactory()
                .WaitForClient<
                    IGuardServiceInterface,
                    NodeEndpointGuardServiceCallback<TService>
                >(
                    "localhost/" + GuardServiceConfiguration.Address,
                    GuardServiceConfiguration.EndpointName,
                    guardServiceCallback
                );
            return guardService;
        }
    }

    public static class DuplexGuardServiceStarter<TDuplexService, TDuplexCallback, TCallback>
        where TDuplexService : IDuplexNodeEndpoint<TDuplexCallback>
        where TDuplexCallback : INodeEndpointClient
        where TCallback : INodeEndpointServerCallback<TDuplexService>, new()
    {
        public interface IGuardServiceInterface : IGuardService, IDuplexNodeEndpointClient<DuplexNodeEndpointGuardServiceCallback<TDuplexService, TDuplexCallback>>
        {
        }

        public static IGuardService ConnectGuardServiceFacade()
        {
            var serverCallback = new TCallback();
            var guardServiceCallback = new DuplexNodeEndpointGuardServiceCallback<TDuplexService, TDuplexCallback>(serverCallback);
            var guardService = GuardServiceConfiguration.CreateProtocolFactory()
                .WaitForClient<
                    IGuardServiceInterface,
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
