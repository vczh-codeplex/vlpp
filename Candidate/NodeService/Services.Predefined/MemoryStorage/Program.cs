using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using ServiceConfigurations;
using NodeServiceHost.GuardService;
using NodeService.Endpoints;

namespace MemoryStorage
{
    class Program
    {
        static void Main(string[] args)
        {
            string serviceName = MemoryStorageServiceConfiguration.EndpointName;

            Console.Title = serviceName;
            GuardServiceStarter<MemoryStorageServive>.LaunchService(
                typeof(Program).Assembly.Location,
                "",
                serviceName,
                MemoryStorageServiceConfiguration.CreateFactory(),
                MemoryStorageServiceConfiguration.ServerAddress,
                serviceName
                );
        }
    }

    [NodeEndpoint(MemoryStorageServiceConfiguration.EndpointName)]
    public class MemoryStorageServive : StrongTypedNodeEndpoint, IMemoryStorage
    {
        public MemoryStorageServive()
        {
            this.EnableAsynchronization = true;
        }

        [NodeEndpointMethod]
        public void SetCache(string applicationName, string[] keyPath, string content, bool exceptionIfNotExists)
        {
            throw new NotImplementedException();
        }

        [NodeEndpointMethod]
        public string GetCache(string applicationName, string[] keyPath, bool exceptionIfNotExists)
        {
            throw new NotImplementedException();
        }
        [NodeEndpointMethod]

        public bool IsCacheExists(string applicationName, string[] keyPath)
        {
            throw new NotImplementedException();
        }

        [NodeEndpointMethod]
        public string[][] GetKeyPaths(string applicationName)
        {
            throw new NotImplementedException();
        }
    }
}
