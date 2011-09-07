using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using NodeService;
using NodeService.Protocols;

namespace ServiceConfigurations
{
    public static class DiskStorageServiceConfiguration
    {
        public const string EndpointName = "DiskStorage";
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

    public interface IDiskStorage
    {
        void SetCache(string applicationName, string[] keyPath, string content, bool exceptionIfNotExists);
        string GetCache(string applicationName, string[] keyPath, bool exceptionIfNotExists);
        bool IsCacheExists(string applicationName, string[] keyPath);
        string[][] GetKeyPaths(string applicationName);
    }

    public interface IDiskStorageClient : IDiskStorage, INodeEndpointClient
    {
    }
}
