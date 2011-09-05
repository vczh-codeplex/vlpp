using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using NodeService.Endpoints;

namespace MachineInfo
{
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
