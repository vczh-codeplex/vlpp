using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using NodeService.Providers;
using System.Xml.Linq;
using NodeService;

namespace NodeServiceHost
{
    public class NodeEndpointServerTracer : INodeEndpointServerTracer, INodeEndpointProtocolRequestListenerTracer
    {
        private volatile bool isTracing = false;
        private List<XElement> tracingResults = new List<XElement>();

        #region INodeEndpointServerTracer Members

        public void StartTracing()
        {
            this.isTracing = true;
            lock (this.tracingResults)
            {
                this.tracingResults.Clear();
            }
        }

        public void StopTracing()
        {
            this.isTracing = false;
        }

        public bool IsTracing()
        {
            return this.isTracing;
        }

        public XElement GetTracingResult()
        {
            lock (this.tracingResults)
            {
                return new XElement(
                    "TracingResult",
                    this.tracingResults.ToArray()
                    );
            }
        }

        #endregion

        #region INodeEndpointProtocolRequestListenerTracer Members

        void INodeEndpointProtocolRequestListenerTracer.OnReceivedRequest(DateTime time, INodeEndpointRequest request)
        {
            if (this.isTracing)
            {
                lock (this.tracingResults)
                {
                    this.tracingResults.Add(new XElement(
                        "Request",
                        new XElement("Date", time.ToLongDateString()),
                        new XElement("Time", time.ToLongTimeString()),
                        new XElement("EndpointName", request.EndpointName),
                        new XElement("Method", request.Method),
                        new XElement("PeerAddress", request.PeerAddress),
                        new XElement("Body", request.Body)
                        ));
                }
            }
        }

        void INodeEndpointProtocolRequestListenerTracer.OnResponded(DateTime time, XNode node)
        {
            if (this.isTracing)
            {
                lock (this.tracingResults)
                {
                    this.tracingResults.Add(new XElement(
                        "RespondNode",
                        new XElement("Date", time.ToLongDateString()),
                        new XElement("Time", time.ToLongTimeString()),
                        new XElement("Body", node)
                        ));
                }
            }
        }

        void INodeEndpointProtocolRequestListenerTracer.OnResponded(DateTime time, byte[] stream)
        {
            if (this.isTracing)
            {
                lock (this.tracingResults)
                {
                    this.tracingResults.Add(new XElement(
                        "RespondStream",
                        new XElement("Date", time.ToLongDateString()),
                        new XElement("Time", time.ToLongTimeString()),
                        new XElement("Body", stream.ByteArrayToHex())
                        ));
                }
            }
        }

        void INodeEndpointProtocolRequestListenerTracer.OnResponded(DateTime time, Exception exception)
        {
            if (this.isTracing)
            {
                lock (this.tracingResults)
                {
                    this.tracingResults.Add(new XElement(
                        "RespondException",
                        new XElement("Date", time.ToLongDateString()),
                        new XElement("Time", time.ToLongTimeString()),
                        new XElement("Exception", exception.Message)
                        ));
                }
            }
        }

        void INodeEndpointProtocolRequestListenerTracer.OnResponded(DateTime time)
        {
            if (this.isTracing)
            {
                lock (this.tracingResults)
                {
                    this.tracingResults.Add(new XElement(
                        "RespondEmpty",
                        new XElement("Date", time.ToLongDateString()),
                        new XElement("Time", time.ToLongTimeString())
                        ));
                }
            }
        }

        #endregion
    }
}
