using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Xml.Linq;

namespace NodeService
{
    public interface INodeEndpointRequest
    {
        string EndpointName { get; }
        string Method { get; }
        string PeerAddress { get; }
        XElement Body { get; }
        bool WaitingForResponse { get; }

        void Respond(XNode response);
        void Respond(Exception exception);
        void Respond();
    }
}
