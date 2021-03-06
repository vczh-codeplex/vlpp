﻿using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Xml.Linq;
using System.IO;

namespace NodeService.Providers
{
    public class ProtocolEnabledRequestListener : INodeEndpointProtocolRequestTracableListener
    {
        private class Request : INodeEndpointRequest
        {
            private TracerBroadcaster tracerBroadcaster;
            private INodeEndpoint endpoint;
            private INodeEndpointProtocolRequest request;
            private Guid guid;
            private string method;
            private XElement body;
            private bool waitingForResponse = true;

            public Request(TracerBroadcaster tracerBroadcaster, INodeEndpoint endpoint, INodeEndpointProtocolRequest request, Guid guid, string method, XElement body)
            {
                this.tracerBroadcaster = tracerBroadcaster;
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
                this.tracerBroadcaster.OnResponded(response);
            }

            public void Respond(Stream stream)
            {
                if (!this.waitingForResponse)
                {
                    throw new InvalidOperationException("Cannot respond more than once.");
                }
                this.waitingForResponse = false;
                stream.Seek(0, SeekOrigin.Begin);
                byte[] bytes = stream.ReadAllBytesAndClose();
                byte[] body = ProtocolEnabledHelper.BuildResponse(this.guid, bytes);
                this.request.Respond(body);
                this.tracerBroadcaster.OnResponded(bytes);
            }

            public void Respond(Exception exception)
            {
                if (!this.waitingForResponse)
                {
                    throw new InvalidOperationException("Cannot respond more than once.");
                }
                this.waitingForResponse = false;
                Respond("[EXCEPTION]" + exception.Message);
                this.tracerBroadcaster.OnResponded(exception);
            }

            public void Respond()
            {
                if (!this.waitingForResponse)
                {
                    throw new InvalidOperationException("Cannot respond more than once.");
                }
                this.waitingForResponse = false;
                Respond("[XTEXT]");
                this.tracerBroadcaster.OnResponded();
            }

            private void Respond(string message)
            {
                string protocolMessage = ProtocolEnabledHelper.BuildResponse(this.guid, message);
                this.request.Respond(protocolMessage);
            }
        }

        class TracerBroadcaster
        {
            private List<INodeEndpointProtocolRequestListenerTracer> tracers;

            public TracerBroadcaster(List<INodeEndpointProtocolRequestListenerTracer> tracers)
            {
                this.tracers = tracers;
            }

            public void OnReceivedRequest(INodeEndpointRequest request)
            {
                DateTime time = DateTime.Now;
                lock (this.tracers)
                {
                    foreach (var tracer in this.tracers)
                    {
                        tracer.OnReceivedRequest(time, request);
                    }
                }
            }

            public void OnResponded(XNode node)
            {
                DateTime time = DateTime.Now;
                lock (this.tracers)
                {
                    foreach (var tracer in this.tracers)
                    {
                        tracer.OnResponded(time, node);
                    }
                }
            }

            public void OnResponded(byte[] stream)
            {
                DateTime time = DateTime.Now;
                lock (this.tracers)
                {
                    foreach (var tracer in this.tracers)
                    {
                        tracer.OnResponded(time, stream);
                    }
                }
            }

            public void OnResponded(Exception exception)
            {
                DateTime time = DateTime.Now;
                lock (this.tracers)
                {
                    foreach (var tracer in this.tracers)
                    {
                        tracer.OnResponded(time, exception);
                    }
                }
            }

            public void OnResponded()
            {
                DateTime time = DateTime.Now;
                lock (this.tracers)
                {
                    foreach (var tracer in this.tracers)
                    {
                        tracer.OnResponded(time);
                    }
                }
            }
        }

        private INodeEndpoint endpoint;
        private List<INodeEndpointProtocolRequestListenerTracer> tracers;
        private TracerBroadcaster tracerBroadcaster;

        public ProtocolEnabledRequestListener(INodeEndpoint endpoint)
        {
            this.endpoint = endpoint;
            this.tracers = new List<INodeEndpointProtocolRequestListenerTracer>();
            this.tracerBroadcaster = new TracerBroadcaster(this.tracers);
        }

        public void OnReceivedRequest(INodeEndpointProtocolRequest request)
        {
            string requestMessage = request.RequestMessage();
            if (requestMessage.StartsWith("[REQUEST]"))
            {
                Guid guid;
                string method;
                string message;
                if (ProtocolEnabledHelper.SplitRequest(requestMessage, out guid, out method, out message))
                {
                    try
                    {
                        XElement body = XElement.Parse(message);
                        Request wrapper = new Request(this.tracerBroadcaster, this.endpoint, request, guid, method, body);
                        this.tracerBroadcaster.OnReceivedRequest(wrapper);
                        this.endpoint.QueueRequest(wrapper);
                    }
                    catch (Exception exception)
                    {
                        throw new NodeEndpointMessageException("Cannot understand the request format.", exception);
                    }
                }
            }
        }

        public void AddTracer(INodeEndpointProtocolRequestListenerTracer tracer)
        {
            lock (this.tracers)
            {
                if (!this.tracers.Contains(tracer))
                {
                    this.tracers.Add(tracer);
                }
            }
        }

        public void RemoveTracer(INodeEndpointProtocolRequestListenerTracer tracer)
        {
            lock (this.tracers)
            {
                this.tracers.Remove(tracer);
            }
        }

        public INodeEndpointProtocolRequestListenerTracer[] GetTracers()
        {
            lock (this.tracers)
            {
                return this.tracers.ToArray();
            }
        }
    }
}
