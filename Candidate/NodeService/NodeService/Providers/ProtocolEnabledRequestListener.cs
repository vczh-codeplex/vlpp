using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Xml.Linq;

namespace NodeService.Providers
{
    public class ProtocolEnabledRequestListener : INodeEndpointProtocolRequestListener
    {
        private class Request : INodeEndpointRequest
        {
            private INodeEndpoint endpoint;
            private INodeEndpointProtocolRequest request;
            private Guid guid;
            private string method;
            private XElement body;
            private bool waitingForResponse = true;

            public Request(INodeEndpoint endpoint, INodeEndpointProtocolRequest request, Guid guid, string method, XElement body)
            {
                this.endpoint = endpoint;
                this.request = request;
                this.guid = guid;
                this.method = method;
                this.body = body;
            }

            public string EndpointName
            {
                get
                {
                    return this.endpoint.EndpointName;
                }
            }

            public string Method
            {
                get
                {
                    return this.method;
                }
            }

            public string PeerAddress
            {
                get
                {
                    return null;
                }
            }

            public XElement Body
            {
                get
                {
                    return this.body;
                }
            }

            public bool WaitingForResponse
            {
                get
                {
                    return this.waitingForResponse;
                }
            }

            public void Respond(XNode response)
            {
                if (!this.waitingForResponse)
                {
                    throw new InvalidOperationException("Cannot respond more than once.");
                }
                this.waitingForResponse = false;
                if (response is XElement)
                {
                    Respond("[XELEMENT]" + response.ToString());
                }
                else
                {
                    Respond("[XTEXT]" + response.ToString());
                }
            }

            public void Respond(Exception exception)
            {
                if (!this.waitingForResponse)
                {
                    throw new InvalidOperationException("Cannot respond more than once.");
                }
                this.waitingForResponse = false;
                Respond("[EXCEPTION]" + exception.Message);
            }

            public void Respond()
            {
                if (!this.waitingForResponse)
                {
                    throw new InvalidOperationException("Cannot respond more than once.");
                }
                this.waitingForResponse = false;
                Respond("[XTEXT]");
            }

            private void Respond(string message)
            {
                string protocolMessage = ProtocolEnabledHelper.BuildResponse(guid, message);
                this.request.Respond(protocolMessage);
            }
        }

        private INodeEndpoint endpoint;

        public ProtocolEnabledRequestListener(INodeEndpoint endpoint)
        {
            this.endpoint = endpoint;
        }

        public void OnReceivedRequest(INodeEndpointProtocolRequest request)
        {
            if (request.Message.StartsWith("[REQUEST]"))
            {
                Guid guid;
                string method;
                string message;
                if (ProtocolEnabledHelper.SplitRequest(request.Message, out guid, out method, out message))
                {
                    try
                    {
                        XElement body = XElement.Parse(message);
                        Request wrapper = new Request(this.endpoint, request, guid, method, body);
                        this.endpoint.QueueRequest(wrapper);
                    }
                    catch (Exception exception)
                    {
                        throw new NodeEndpointMessageException("Cannot understand the request format.", exception);
                    }
                }
            }
        }
    }
}
