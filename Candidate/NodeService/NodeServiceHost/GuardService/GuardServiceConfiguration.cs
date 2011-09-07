using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using NodeService;
using NodeService.Protocols;
using System.Threading;

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

    public class GuardServiceStarterServerCallback<TService> : INodeEndpointServerCallback<TService>
        where TService : INodeEndpoint, new()
    {
        private INodeEndpointProtocolFactory protocolFactory;
        private string protocolAddress;
        private string endpointName;

        public GuardServiceStarterServerCallback()
        {
            throw new NotSupportedException();
        }

        public GuardServiceStarterServerCallback(INodeEndpointProtocolFactory protocolFactory, string protocolAddress, string endpointName)
        {
            this.protocolFactory = protocolFactory;
            this.protocolAddress = protocolAddress;
            this.endpointName = endpointName;
        }

        public INodeEndpointProtocolFactory ProtocolFactory
        {
            get
            {
                return this.protocolFactory;
            }
        }

        public string ProtocolAddress
        {
            get
            {
                return this.protocolAddress;
            }
        }

        public string EndpointName
        {
            get
            {
                return this.endpointName;
            }
        }

        public TService CreateEndpoint()
        {
            return new TService();
        }

        public void OnEndpointStart(TService endpoint, INodeEndpointProtocolServer protocolServer)
        {
        }

        public void OnEndpointStopped(TService endpoint, INodeEndpointProtocolServer protocolServer)
        {
        }
    }

    public static class GuardServiceStarterInternal<TService, TCallback>
        where TService : INodeEndpoint
        where TCallback : INodeEndpointServerCallback<TService>, new()
    {
        public interface IGuardServiceClient : IGuardService, IDuplexNodeEndpointClient<NodeEndpointGuardServiceCallback<TService>>
        {
        }

        public static IGuardService ConnectGuardServiceFacade(Action<INodeEndpointServer<TService>> startEventHandler, Action stopEventHandler, TCallback callback)
        {
            var serverCallback = object.ReferenceEquals(callback, default(TCallback)) ? new TCallback() : callback;
            var guardServiceCallback = new NodeEndpointGuardServiceCallback<TService>(serverCallback);
            guardServiceCallback.StartEventHandler = startEventHandler;
            guardServiceCallback.StopEventHandler = stopEventHandler;
            var guardService = GuardServiceConfiguration.CreateProtocolFactory()
                .WaitForClient<
                    IGuardServiceClient,
                    NodeEndpointGuardServiceCallback<TService>
                >(
                    "localhost/" + GuardServiceConfiguration.Address,
                    GuardServiceConfiguration.EndpointName,
                    guardServiceCallback
                );
            return guardService;
        }

        public static void LaunchService(string executablePath, string arguments, string name, TCallback callback = default(TCallback))
        {
            ManualResetEvent exitEvent = new ManualResetEvent(false);
            var guardService = ConnectGuardServiceFacade((s) => { }, () => exitEvent.Set(), callback);
            try
            {
                guardService.Register(new GuardedServiceDescription()
                {
                    ExecutablePath = executablePath,
                    Arguments = arguments,
                    Name = name,
                });
                Console.WriteLine("Server started. To close this server, use NodeServiceGuard.exe.");
                exitEvent.WaitOne();
            }
            catch (Exception ex)
            {
                Console.WriteLine(ex.Message);
                Console.WriteLine("Press [ENTER] to stop.");
                Console.ReadLine();
            }
            exitEvent.Close();
        }
    }

    public static class DuplexGuardServiceStarterInternal<TDuplexService, TDuplexCallback, TCallback>
        where TDuplexService : IDuplexNodeEndpoint<TDuplexCallback>
        where TDuplexCallback : INodeEndpointClient
        where TCallback : INodeEndpointServerCallback<TDuplexService>, new()
    {
        public interface IGuardServiceClient : IGuardService, IDuplexNodeEndpointClient<DuplexNodeEndpointGuardServiceCallback<TDuplexService, TDuplexCallback>>
        {
        }

        public static IGuardService ConnectGuardServiceFacade(Action<INodeEndpointServer<TDuplexService>> startEventHandler, Action stopEventHandler, TCallback callback)
        {
            var serverCallback = object.ReferenceEquals(callback, default(TCallback)) ? new TCallback() : callback;
            var guardServiceCallback = new DuplexNodeEndpointGuardServiceCallback<TDuplexService, TDuplexCallback>(serverCallback);
            guardServiceCallback.StartEventHandler = startEventHandler;
            guardServiceCallback.StopEventHandler = stopEventHandler;
            var guardService = GuardServiceConfiguration.CreateProtocolFactory()
                .WaitForClient<
                    IGuardServiceClient,
                    DuplexNodeEndpointGuardServiceCallback<TDuplexService, TDuplexCallback>
                >(
                    "localhost/" + GuardServiceConfiguration.Address,
                    GuardServiceConfiguration.EndpointName,
                    guardServiceCallback
                );
            return guardService;
        }

        public static void LaunchService(string executablePath, string arguments, string name, TCallback callback = default(TCallback))
        {
            ManualResetEvent exitEvent = new ManualResetEvent(false);
            var guardService = ConnectGuardServiceFacade((s) => { }, () => exitEvent.Set(), callback);
            try
            {
                guardService.Register(new GuardedServiceDescription()
                {
                    ExecutablePath = executablePath,
                    Arguments = arguments,
                    Name = name,
                });
                Console.WriteLine("Server started. To close this server, use NodeServiceGuard.exe.");
                exitEvent.WaitOne();
            }
            catch (Exception ex)
            {
                Console.WriteLine(ex.Message);
                Console.WriteLine("Press [ENTER] to stop.");
                Console.ReadLine();
            }
            exitEvent.Close();
        }
    }

    public static class GuardServiceStarter<TService>
        where TService : INodeEndpoint, new()
    {
        public static void LaunchService(string executablePath, string arguments, string name, INodeEndpointProtocolFactory protocolFactory, string protocolAddress, string endpointName)
        {
            GuardServiceStarterInternal<TService, GuardServiceStarterServerCallback<TService>>.LaunchService(
                executablePath,
                arguments,
                name,
                new GuardServiceStarterServerCallback<TService>(protocolFactory, protocolAddress, endpointName)
                );
        }
    }

    public static class DuplexGuardServiceStarter<TService, TCallback>
        where TService : IDuplexNodeEndpoint<TCallback>, new()
        where TCallback : INodeEndpointClient
    {
        public static void LaunchService(string executablePath, string arguments, string name, INodeEndpointProtocolFactory protocolFactory, string protocolAddress, string endpointName)
        {
            DuplexGuardServiceStarterInternal<TService, TCallback, GuardServiceStarterServerCallback<TService>>.LaunchService(
                executablePath,
                arguments,
                name,
                new GuardServiceStarterServerCallback<TService>(protocolFactory, protocolAddress, endpointName)
                );
        }
    }
}
