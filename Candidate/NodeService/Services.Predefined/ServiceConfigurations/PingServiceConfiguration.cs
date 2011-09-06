using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.IO;
using NodeService;
using NodeService.Protocols;

namespace ServiceConfigurations
{
    public static class PingServiceHttpConfiguration
    {
        public const string EndpointName = "PingServiceHttp";
        public const string ServerAddress = "http://+:9002/";

        public static INodeEndpointProtocolFactory CreateFactory()
        {
            return new HttpProtocolFactory();
        }

        public static string CreateClientAddress(string machineAddress)
        {
            return ServerAddress.Replace("+", machineAddress);
        }
    }

    public static class PingServiceTcpConfiguration
    {
        public const string EndpointName = "PingServiceTcp";
        public const string ServerAddress = "9001";

        public static INodeEndpointProtocolFactory CreateFactory()
        {
            return new TcpProtocolFactory();
        }

        public static string CreateClientAddress(string machineAddress)
        {
            return machineAddress + ":" + ServerAddress;
        }
    }

    public static class PingServiceNamedPipeConfiguration
    {
        public const string EndpointName = "PingServiceNamedPipe";
        public const string ServerAddress = "PredefinedServices";

        public static INodeEndpointProtocolFactory CreateFactory()
        {
            return new NamedPipeProtocolFactory();
        }

        public static string CreateClientAddress(string machineAddress)
        {
            return machineAddress + "/" + ServerAddress;
        }
    }

    public interface IPingService
    {
        string Hello(string name);
        Stream HelloStream(string name);
    }

    public interface IPingServiceClient : IPingService, INodeEndpointClient
    {
    }
}
