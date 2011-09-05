using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Xml.Linq;
using System.Threading;
using System.IO;

namespace NodeService.Providers
{
    public class ProtocolEnabledClientProvider : INodeEndpointClientProvider
    {
        private class ClientResponse : INodeEndpointResponse
        {
            private volatile RequestState requestState;
            private XNode response;
            private byte[] stream;
            private Exception exception;
            private List<Action<INodeEndpointResponse>> callbacks = new List<Action<INodeEndpointResponse>>();

            public ClientResponse()
            {
                this.ResponseEvent = new ManualResetEvent(false);
            }

            public bool EnableAsynchronization
            {
                get
                {
                    return true;
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
                    if (this.requestState != RequestState.ReceivedResponse)
                    {
                        throw new InvalidOperationException("Response does not exist.");
                    }
                    return this.response;
                }
            }

            public Stream Stream
            {
                get
                {
                    if (this.requestState != RequestState.ReceivedStream)
                    {
                        throw new InvalidOperationException("Stream does not exist.");
                    }
                    return new MemoryStream(this.stream, false);
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
                if (this.ResponseEvent != null)
                {
                    this.ResponseEvent.WaitOne();
                }
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

            public ManualResetEvent ResponseEvent { get; set; }

            private void CloseEvent()
            {
                if (this.ResponseEvent != null)
                {
                    this.ResponseEvent.Set();
                    this.ResponseEvent.Close();
                    this.ResponseEvent.Dispose();
                    this.ResponseEvent = null;
                }
            }

            public void SetResponse(byte[] stream)
            {
                lock (this.callbacks)
                {
                    this.requestState = NodeService.RequestState.ReceivedStream;
                    this.stream = stream;
                    foreach (var callback in this.callbacks)
                    {
                        callback(this);
                    }
                    CloseEvent();
                }
            }

            public void SetResponse(string response)
            {
                lock (this.callbacks)
                {
                    if (response.StartsWith("[XTEXT]"))
                    {
                        this.requestState = RequestState.ReceivedResponse;
                        this.response = new XText(response.Substring(7));
                    }
                    else if (response.StartsWith("[XELEMENT]"))
                    {
                        this.requestState = RequestState.ReceivedResponse;
                        try
                        {
                            this.response = XElement.Parse(response.Substring(10));
                        }
                        catch (Exception exception)
                        {
                            this.requestState = RequestState.RaisedException;
                            this.exception = new NodeEndpointMessageException("Cannot understand the response format.", exception);
                        }
                    }
                    else if (response.StartsWith("[EXCEPTION]"))
                    {
                        this.requestState = RequestState.RaisedException;
                        this.exception = new NodeEndpointRequestException(response.Substring(11));
                    }
                    else
                    {
                        this.requestState = RequestState.RaisedException;
                        this.exception = new NodeEndpointMessageException("Cannot understand the response format.");
                    }
                    foreach (var callback in this.callbacks)
                    {
                        callback(this);
                    }
                    CloseEvent();
                }
            }

            public void Dispose()
            {
                CloseEvent();
            }
        }

        private class ResponseListener : INodeEndpointProtocolRequestListener
        {
            private ProtocolEnabledClientProvider clientProvider = null;

            public ResponseListener(ProtocolEnabledClientProvider clientProvider)
            {
                this.clientProvider = clientProvider;
            }

            public void OnReceivedRequest(INodeEndpointProtocolRequest request)
            {
                const int streamHeaderLength = 54;
                if (request.Message.Length >= streamHeaderLength)
                {
                    byte[] header = new byte[streamHeaderLength];
                    Array.Copy(request.Message, header, streamHeaderLength);
                    string headerString = header.NodeServiceDecode();
                    if (headerString.StartsWith("[RESPONSESTREAM"))
                    {
                        byte[] stream = new byte[request.Message.Length - streamHeaderLength];
                        Array.Copy(request.Message, streamHeaderLength, stream, 0, stream.Length);
                        this.clientProvider.Receive(headerString, stream);
                    }
                }

                string requestMessage = request.RequestMessage();
                if (requestMessage.StartsWith("[RESPONSE]"))
                {
                    this.clientProvider.Receive(requestMessage);
                }
            }
        }

        private ResponseListener responseListener;
        private INodeEndpointProtocol protocol;
        private Dictionary<Guid, ClientResponse> responses = new Dictionary<Guid, ClientResponse>();

        public ProtocolEnabledClientProvider()
        {
            this.responseListener = new ResponseListener(this);
        }

        public INodeEndpointProtocol Protocol
        {
            get
            {
                return this.protocol;
            }
            set
            {
                if (this.protocol != value)
                {
                    if (this.protocol != null)
                    {
                        this.protocol.RemoveListener(this.responseListener);
                    }
                    this.protocol = value;
                    if (this.protocol != null)
                    {
                        this.protocol.AddListener(this.responseListener);
                    }
                }
            }
        }

        public INodeEndpointResponse Send(string method, XElement body)
        {
            if (this.Protocol == null) throw new InvalidOperationException("Cannot send message when procotol has not been set.");
            if (!this.Protocol.Connected) throw new InvalidOperationException("Cannot send message when protocol does not connected to server.");

            Guid guid = Guid.NewGuid();
            string protocolMessage = ProtocolEnabledHelper.BuildRequest(guid, method, body.ToString());

            ClientResponse response = new ClientResponse();
            lock (this.responses)
            {
                this.responses.Add(guid, response);
            }
            this.Protocol.Send(protocolMessage);
            return response;
        }

        public void Disconnect()
        {
            if (this.protocol != null)
            {
                this.protocol.Disconnect();
                this.protocol = null;
            }
            lock (this.responses)
            {
                foreach (var response in this.responses.Values)
                {
                    response.Dispose();
                }
                this.responses.Clear();
            }
        }

        private void Receive(string protocolMessage, byte[] stream)
        {
            lock (this.responses)
            {
                Guid guid;
                if (ProtocolEnabledHelper.SplitResponse(protocolMessage, out guid))
                {
                    ClientResponse response;
                    if (this.responses.TryGetValue(guid, out response))
                    {
                        this.responses.Remove(guid);
                        response.SetResponse(stream);
                    }
                }
            }
        }

        private void Receive(string protocolMessage)
        {
            lock (this.responses)
            {
                Guid guid;
                string message;
                if (ProtocolEnabledHelper.SplitResponse(protocolMessage, out guid, out message))
                {
                    ClientResponse response;
                    if (this.responses.TryGetValue(guid, out response))
                    {
                        this.responses.Remove(guid);
                        response.SetResponse(message);
                    }
                }
            }
        }
    }
}
