using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using ServiceConfigurations;
using NodeService.Endpoints;
using NodeServiceHost.GuardService;
using NodeService;

namespace ConfigurationStorage
{
    class Program
    {
        static void Main(string[] args)
        {
            string serviceName = ConfigurationStorageServiceConfiguration.EndpointName;

            Console.Title = serviceName;
            GuardServiceStarter<ConfigurationStorageService>.LaunchService(
                typeof(Program).Assembly.Location,
                "",
                serviceName,
                ConfigurationStorageServiceConfiguration.CreateFactory(),
                ConfigurationStorageServiceConfiguration.ServerAddress,
                serviceName
                );
        }
    }

    [NodeEndpoint(ConfigurationStorageServiceConfiguration.EndpointName)]
    public class ConfigurationStorageService : StrongTypedNodeEndpoint, IConfigurationStorageService, IDuplexNodeEndpoint<IConfigurationStorageCallbackClient>
    {
        public ConfigurationStorageService()
        {
            this.EnableAsynchronization = true;
        }

        public IConfigurationStorageCallbackClient Callback { get; set; }

        [NodeEndpointMethod]
        public void ConnectConfig(string applicationName)
        {
            throw new NotImplementedException();
        }

        [NodeEndpointMethod]
        public void SetConfig(string content)
        {
            throw new NotImplementedException();
        }

        [NodeEndpointMethod]
        public string GetConfig(string content)
        {
            throw new NotImplementedException();
        }
    }
}
