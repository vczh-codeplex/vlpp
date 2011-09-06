using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using NodeService;
using System.IO;
using NodeService.Endpoints;
using NodeService.Protocols;

namespace ServiceConfigurations
{
    public static class MachineInfoServiceConfiguration
    {
        public const string EndpointName = "MachineInfo";
        public const string ServerAddress = "http://+:9010/";

        public static INodeEndpointProtocolFactory CreateFactory()
        {
            return new HttpProtocolFactory();
        }

        public static string CreateClientAddress(string machineAddress)
        {
            return ServerAddress.Replace("+", machineAddress);
        }
    }

    public interface IMachineInfoService
    {
        int GetScreenCount();
        ScreenInfo GetScreenInfo(int index);
        Stream GetScreenImage(int index);

        string[] GetDrivers();
        bool IsDirectoryExists(string directory);
        bool IsFileExists(string file);
        string[] GetDirectories(string directory);
        string[] GetFiles(string directory);

        void MouseMove(int x, int y);
        void MouseEvent(string operation, int data);
        void KeyEvent(byte key, bool down);
    }

    public interface IMachineInfoServiceClient : IMachineInfoService, INodeEndpointClient
    {
    }

    [NodeEndpointDataType]
    public class ScreenBounds
    {
        [NodeEndpointDataMember]
        public int Left { get; set; }

        [NodeEndpointDataMember]
        public int Top { get; set; }

        [NodeEndpointDataMember]
        public int Width { get; set; }

        [NodeEndpointDataMember]
        public int Height { get; set; }
    }

    [NodeEndpointDataType]
    public class ScreenInfo
    {
        [NodeEndpointDataMember]
        public string DeviceName { get; set; }

        [NodeEndpointDataMember]
        public bool IsPrimary { get; set; }

        [NodeEndpointDataMember]
        public ScreenBounds Bounds { get; set; }

        [NodeEndpointDataMember]
        public ScreenBounds WorkingArea { get; set; }
    }
}
