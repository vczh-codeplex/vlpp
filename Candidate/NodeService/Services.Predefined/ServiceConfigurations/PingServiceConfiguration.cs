using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.IO;
using NodeService;
using NodeService.Protocols;

/*
 * PingNamedPipe        : \\.\pipe\PingServiceNamedPipe/PredefinedServices
 * PingTcp              : .:9001
 * PingHttp             : http://.:9002/PingServiceHttp/
 * PingTcpShare         : .:9003/PingServiceTcpShare
 * TcpShareProvider     : .:9004
 * MachineInfo          : http://.:9010/MachineInfo/
 */

namespace ServiceConfigurations
{
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
            return ClientAddress.BuildNamedPipeAddress(ServerAddress, machineAddress);
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
            return ClientAddress.BuildTcpAddress(ServerAddress, machineAddress);
        }
    }

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
            return ClientAddress.BuildHttpAddress(ServerAddress, machineAddress);
        }
    }

    public static class PingServiceTcpShareConfiguration
    {
        public const string EndpointName = "PingServiceTcpShare";
        public const string ServerAddress = "9003";

        public static INodeEndpointProtocolFactory CreateFactory()
        {
            return new TcpShareProtocolFactory();
        }

        public static string CreateClientAddress(string machineAddress)
        {
            return ClientAddress.BuildTcpAddress(ServerAddress, machineAddress);
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
