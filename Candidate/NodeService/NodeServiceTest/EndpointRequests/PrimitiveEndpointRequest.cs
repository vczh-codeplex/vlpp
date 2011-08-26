using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using NodeService;
using System.Xml.Linq;

namespace NodeServiceTest.EndpointRequests
{
    class PrimitiveEndpointRequest<T> : INodeEndpointRequest
    {
        private StrongTypedNodeEndpoint endpoint;
        private string method;
        private XElement body;
        private bool waitingForResponse;
        private Action<T> callback;

        public PrimitiveEndpointRequest(StrongTypedNodeEndpoint endpoint, string method, Action<T> callback)
        {
            this.endpoint = endpoint;
            this.method = method;
            this.waitingForResponse = false;
            this.callback = callback;
            this.body = new XElement("Parameters");
        }

        public PrimitiveEndpointRequest<T> AddParameter<U>(string name, U value)
        {
            this.body.Add(new XElement(name, this.endpoint.Serializer.Serialize(value)));
            return this;
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
            this.waitingForResponse = true;
            this.callback((T)this.endpoint.Serializer.Deserialize(response, typeof(T)));
        }


        public void Respond(Exception exception)
        {
            throw exception;
        }
    }
}
