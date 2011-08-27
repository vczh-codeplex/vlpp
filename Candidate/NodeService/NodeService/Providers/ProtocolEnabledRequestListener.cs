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
            private XElement body;
            private bool waitingForResponse = true;

            public Request(INodeEndpoint endpoint, INodeEndpointProtocolRequest request)
            {
                this.endpoint = endpoint;
                this.request = request;
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
                    return this.request.Method;
                }
            }

            public string PeerAddress
            {
                get
                {
                    return this.request.PeerAddress;
                }
            }

            public XElement Body
            {
                get
                {
                    try
                    {
                        if (this.body == null)
                        {
                            this.body = XElement.Parse(this.request.Message);
                        }
                        return this.body;
                    }
                    catch (Exception exception)
                    {
                        throw new NodeEndpointMessageException("Cannot understand the request format.", exception);
                    }
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
                if (this.waitingForResponse)
                {
                    throw new InvalidOperationException("Cannot respond more than once.");
                }
                this.waitingForResponse = false;
                if (response is XElement)
                {
                    this.request.Respond("[XELEMENT]" + response.ToString());
                }
                else
                {
                    this.request.Respond("[XTEXT]" + response.ToString());
                }
            }

            public void Respond(Exception exception)
            {
                if (this.waitingForResponse)
                {
                    throw new InvalidOperationException("Cannot respond more than once.");
                }
                this.waitingForResponse = false;
                this.request.Respond("[EXCEPTION]" + exception.Message);
            }

            public void Respond()
            {
                if (this.waitingForResponse)
                {
                    throw new InvalidOperationException("Cannot respond more than once.");
                }
                this.waitingForResponse = false;
                this.request.Respond("[XTEXT]");
            }
        }

        private INodeEndpoint endpoint;

        public ProtocolEnabledRequestListener(INodeEndpoint endpoint)
        {
            this.endpoint = endpoint;
        }

        public void OnReceivedRequest(INodeEndpointProtocolRequest request)
        {
            Request wrapper = new Request(this.endpoint, request);
            this.endpoint.QueueRequest(wrapper);
        }
    }
}
