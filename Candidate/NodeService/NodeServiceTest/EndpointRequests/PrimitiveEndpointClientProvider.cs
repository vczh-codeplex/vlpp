﻿using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using NodeService;
using System.Xml.Linq;
using NodeService.Endpoints;
using System.IO;

namespace NodeServiceTest.EndpointRequests
{
    class PrimitiveEndpointClientProvider : INodeEndpointClientProvider
    {
        private StrongTypedNodeEndpoint endpoint;

        public PrimitiveEndpointClientProvider(StrongTypedNodeEndpoint endpoint)
        {
            this.endpoint = endpoint;
        }

        public INodeEndpointProtocol Protocol { get; set; }

        public INodeEndpointResponse Send(string method, XElement body)
        {
            PrimitiveEndpointClientRequest request = new PrimitiveEndpointClientRequest(this.endpoint, method, body);
            this.endpoint.QueueRequest(request);
            return request.Response;
        }

        public void Disconnect()
        {
        }
    }

    class PrimitiveEndpointClientRequest : INodeEndpointRequest
    {
        private StrongTypedNodeEndpoint endpoint;
        private string method;
        private XElement body;
        private bool waitingForResponse;
        private PrimitiveEndpointClientResponse response;

        public PrimitiveEndpointClientRequest(StrongTypedNodeEndpoint endpoint, string method, XElement body)
        {
            this.endpoint = endpoint;
            this.method = method;
            this.body = body;
            this.waitingForResponse = false;
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

        public PrimitiveEndpointClientResponse Response
        {
            get
            {
                return this.response;
            }
        }

        public void Respond(XNode response)
        {
            this.waitingForResponse = true;
            this.response = new PrimitiveEndpointClientResponse(this.endpoint, response);
        }

        public void Respond(Stream stream)
        {
            this.waitingForResponse = true;
            this.response = new PrimitiveEndpointClientResponse(this.endpoint, stream);
        }

        public void Respond(Exception exception)
        {
            this.waitingForResponse = true;
            this.response = new PrimitiveEndpointClientResponse(this.endpoint, exception);
        }

        public void Respond()
        {
            this.waitingForResponse = true;
            this.response = new PrimitiveEndpointClientResponse(this.endpoint, new XText(""));
        }
    }

    class PrimitiveEndpointClientResponse : INodeEndpointResponse, IDisposable
    {
        private StrongTypedNodeEndpoint endpoint;
        private RequestState requestState;
        private XNode response;
        private Stream stream;
        private Exception exception;

        public PrimitiveEndpointClientResponse(StrongTypedNodeEndpoint endpoint, XNode response)
        {
            this.endpoint = endpoint;
            this.requestState = NodeService.RequestState.ReceivedResponse;
            this.response = response;
        }

        public PrimitiveEndpointClientResponse(StrongTypedNodeEndpoint endpoint, Stream stream)
        {
            this.endpoint = endpoint;
            this.requestState = NodeService.RequestState.ReceivedStream;
            this.stream = stream;
            this.stream.Seek(0, SeekOrigin.Begin);
        }

        public PrimitiveEndpointClientResponse(StrongTypedNodeEndpoint endpoint, Exception exception)
        {
            this.endpoint = endpoint;
            this.requestState = NodeService.RequestState.RaisedException;
            this.exception = exception;
        }

        public bool EnableAsynchronization
        {
            get
            {
                return false;
            }
        }

        public RequestState RequestState
        {
            get
            {
                return this.requestState;
            }
        }

        public XNode Response
        {
            get
            {
                return this.response;
            }
        }

        public Stream Stream
        {
            get
            {
                return this.stream;
            }
        }

        public Exception Exception
        {
            get
            {
                return this.exception;
            }
        }

        public void WaitForResponse()
        {
        }

        public void RegisterCallback(Action<INodeEndpointResponse> callback)
        {
            callback(this);
        }

        public void Dispose()
        {
        }
    }
}
