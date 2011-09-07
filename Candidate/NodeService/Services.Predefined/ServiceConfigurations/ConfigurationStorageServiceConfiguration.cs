using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using NodeService;
using NodeService.Protocols;

namespace ServiceConfigurations
{
    public static class ConfigurationStorageServiceConfiguration
    {
        public const string EndpointName = "ConfigurationStorage";
        public const string ServerAddress = "9010";

        public static INodeEndpointProtocolFactory CreateFactory()
        {
            return new TcpShareProtocolFactory();
        }

        public static string CreateClientAddress(string machineAddress)
        {
            return ClientAddress.BuildTcpAddress(ServerAddress, machineAddress);
        }
    }

    public interface IConfigurationStorageService
    {
        void SetConfig(string applicationName, string configuration);
        string GetConfig(string applicationName);
    }

    public interface IConfigurationStorageCallback
    {
        void ConfigModified();
    }

    public interface IConfigurationStorageServiceClient<T> : IConfigurationStorageService, IDuplexNodeEndpointClient<T>
        where T : IConfigurationStorageCallback, INodeEndpoint
    {
    }

    public interface IConfigurationStorageCallbackClient : IConfigurationStorageCallback, INodeEndpointClient
    {
    }
}
