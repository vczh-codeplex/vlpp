using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Xml.Linq;

namespace NodeService.Providers
{
    public class ProtocolEnabledClientProvider : INodeEndpointClientProvider
    {
        private class Response : INodeEndpointResponse
        {
            private INodeEndpointProtocolResponse protocolResponse;
            private volatile RequestState requestState;
            private XNode response;
            private Exception exception;
            private List<Action<INodeEndpointResponse>> callbacks = new List<Action<INodeEndpointResponse>>();

            public Response(INodeEndpointProtocolResponse protocolResponse)
            {
                this.protocolResponse = protocolResponse;
                this.protocolResponse.RegisterCallback(r =>
                {
                    lock (this.callbacks)
                    {
                        foreach (var callback in this.callbacks)
                        {
                            callback(this);
                        }
                    }

                    if (r.Response.StartsWith("[XTEXT]"))
                    {
                        this.requestState = RequestState.ReceivedResponse;
                        this.response = new XText(r.Response.Substring(7));
                    }
                    else if (r.Response.StartsWith("[XELEMENT]"))
                    {
                        this.requestState = RequestState.ReceivedResponse;
                        try
                        {
                            this.response = XElement.Parse(r.Response.Substring(10));
                        }
                        catch (Exception exception)
                        {
                            this.requestState = RequestState.RaisedException;
                            this.exception = new NodeEndpointMessageException("Cannot understand the response format.", exception);
                        }
                    }
                    else if (r.Response.StartsWith("[EXCEPTION]"))
                    {
                        this.requestState = RequestState.RaisedException;
                        this.exception = new NodeEndpointRequestException(r.Response.Substring(11));
                    }
                    else
                    {
                        this.requestState = RequestState.RaisedException;
                        this.exception = new NodeEndpointMessageException("Cannot understand the response format.");
                    }
                });
            }

            public bool EnableAsynchronization
            {
                get
                {
                    return this.protocolResponse.EnableAsynchronization;
                }
            }

            public RequestState RequestState
            {
                get
                {
                    return this.requestState;
                }
            }

            XNode INodeEndpointResponse.Response
            {
                get
                {
                    if (this.requestState != RequestState.ReceivedResponse)
                    {
                        throw new InvalidOperationException("Response does not exist.");
                    }
                    return this.response;
                }
            }

            public Exception Exception
            {
                get
                {
                    if (this.requestState != RequestState.RaisedException)
                    {
                        throw new InvalidOperationException("Exception does not exist.");
                    }
                    return this.exception;
                }
            }

            public void WaitForResponse()
            {
                this.protocolResponse.WaitForResponse();
            }

            public void RegisterCallback(Action<INodeEndpointResponse> callback)
            {
                lock (this.callbacks)
                {
                    callbacks.Add(callback);
                    if (this.requestState != RequestState.WaitingForResponse)
                    {
                        callback(this);
                    }
                }
            }

            public void Dispose()
            {
                this.protocolResponse.Dispose();
            }
        }

        public INodeEndpointProtocol Protocol { get; set; }

        public INodeEndpointResponse Send(string method, XElement body)
        {
            if (this.Protocol == null) throw new InvalidOperationException("Cannot send message when procotol has not been set.");
            if (!this.Protocol.Connected) throw new InvalidOperationException("Cannot send message when protocol does not connected to server.");
            if (this.Protocol.ProtocolSender == null) throw new InvalidOperationException("The protocol does not able to send message.");

            INodeEndpointProtocolResponse protocolResponse = this.Protocol.ProtocolSender.Send(method, body.ToString());
            return new Response(protocolResponse);
        }
    }
}
