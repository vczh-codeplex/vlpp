using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using ServiceConfigurations;
using NodeServiceHost.GuardService;
using NodeService.Endpoints;

namespace DiskStorage
{
    class Program
    {
        static void Main(string[] args)
        {
            string serviceName = DiskStorageServiceConfiguration.EndpointName;

            Console.Title = serviceName;
            GuardServiceStarter<DiskStorageService>.LaunchService(
                typeof(Program).Assembly.Location,
                "",
                serviceName,
                DiskStorageServiceConfiguration.CreateFactory(),
                DiskStorageServiceConfiguration.ServerAddress,
                serviceName
                );
        }
    }

    [NodeEndpoint(DiskStorageServiceConfiguration.EndpointName)]
    public class DiskStorageService : StrongTypedNodeEndpoint, IDiskStorage
    {
        public DiskStorageService()
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
