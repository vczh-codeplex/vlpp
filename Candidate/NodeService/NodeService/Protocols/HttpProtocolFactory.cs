using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Net;
using System.IO;

namespace NodeService.Protocols
{
    public class HttpProtocolFactory : INodeEndpointProtocolFactory
    {
        public INodeEndpointProtocolServerListener CreateServerListener()
        {
            return new ServerListener();
        }

        public INodeEndpointProtocolClient CreateClient()
        {
            return new Client();
        }

        class Request : INodeEndpointProtocolRequest
        {
            private HttpListenerRequest request = null;
            private string message = null;

            public Request(HttpListenerRequest request)
            {
                this.request = request;
            }

            public Request(string message)
            {
                this.message = message;
            }

            public bool CanRespond
            {
                get
                {
                    return this.request != null;
                }
            }

            public string Message
            {
                get
                {
                    return this.message;
                }
            }

            public void Respond(string response)
            {
                throw new NotImplementedException();
            }
        }

        class ServerListener : INodeEndpointProtocolServerListener
        {
            public bool Connected
            {
                get { throw new NotImplementedException(); }
            }

            public void Connect(string address, string endpointName)
            {
                throw new NotImplementedException();
            }

            public void Disconnect()
            {
                throw new NotImplementedException();
            }

            public INodeEndpointProtocolServer Listen()
            {
                throw new NotImplementedException();
            }
        }

        class Server : INodeEndpointProtocolServer
        {
            private INodeEndpointProtocolServer innerProtocol;

            public INodeEndpointProtocolServer OuterProtocol
            {
                get
                {
                    return null;
                }
            }

            public INodeEndpointProtocolServer InnerProtocol
            {
                get
                {
                    return this.innerProtocol;
                }
            }

            public void SetOuterProtocol(INodeEndpointProtocolServer protocol)
            {
                throw new InvalidOperationException("The protocol server cannot have a outer protocol.");
            }

            public void OnInnerProtocolSet(INodeEndpointProtocolServer protocol)
            {
                this.innerProtocol = protocol;
            }

            public bool EnableDuplex
            {
                get { throw new NotImplementedException(); }
            }

            public bool Connected
            {
                get { throw new NotImplementedException(); }
            }

            public void Disconnect()
            {
                throw new NotImplementedException();
            }

            public void BeginListen()
            {
                throw new NotImplementedException();
            }

            public void AddListener(INodeEndpointProtocolRequestListener listener)
            {
                throw new NotImplementedException();
            }

            public void RemoveListener(INodeEndpointProtocolRequestListener listener)
            {
                throw new NotImplementedException();
            }

            public void Send(string message)
            {
                throw new NotImplementedException();
            }
        }

        class Client : INodeEndpointProtocolClient
        {
            private INodeEndpointProtocolClient innerProtocol;
            private string address;
            private string endpointName;
            private bool connected = false;
            private List<INodeEndpointProtocolRequestListener> listeners = new List<INodeEndpointProtocolRequestListener>();

            private bool SendHttpRequest(string body, bool asynchronized, Action<HttpWebRequest, HttpWebResponse, string> callback)
            {
                byte[] bytes = Encoding.UTF8.GetBytes(body);

                HttpWebRequest request = (HttpWebRequest)HttpWebRequest.Create(address + "/" + endpointName + "/");
                request.UserAgent = "Vczh-NodeService-Http";
                request.Method = "POST";
                request.ContentType = "text/html; charset=UTF-8";
                request.ContentLength = bytes.Length;
                Stream requestStream = request.GetRequestStream();
                requestStream.Write(bytes, 0, bytes.Length);
                requestStream.Close();

                if (asynchronized)
                {
                    request.BeginGetResponse((a) =>
                    {
                        HttpWebResponse response = (HttpWebResponse)request.EndGetResponse(a);
                        using (StreamReader reader = new StreamReader(response.GetResponseStream(), Encoding.UTF8))
                        {
                            string message = reader.ReadToEnd();
                            callback(request, response, message);
                        }
                    }, null);
                }
                else
                {
                    try
                    {
                        HttpWebResponse response = (HttpWebResponse)request.GetResponse();
                        using (StreamReader reader = new StreamReader(response.GetResponseStream(), Encoding.UTF8))
                        {
                            string message = reader.ReadToEnd();
                            callback(request, response, message);
                        }
                    }
                    catch (WebException)
                    {
                        return false;
                    }
                }
                return true;
            }

            public INodeEndpointProtocolClient OuterProtocol
            {
                get
                {
                    return null;
                }
            }

            public INodeEndpointProtocolClient InnerProtocol
            {
                get
                {
                    return this.innerProtocol;
                }
            }

            public bool Connect(string address, string endpointName, int timeout)
            {
                Disconnect();
                this.address = address;
                this.endpointName = endpointName;

                string message = "";
                SendHttpRequest("[CONNECT]", false, (a, b, m) => message = m);
                if (message == "[CONNECTED]")
                {
                    if (this.innerProtocol != null)
                    {
                        this.innerProtocol.OnOuterProtocolConnected();
                    }
                    this.connected = true;
                    return true;
                }
                else
                {
                    this.connected = false;
                    return false;
                }
            }

            public void SetOuterProtocol(INodeEndpointProtocolClient protocol)
            {
                throw new InvalidOperationException("The protocol client cannot have a outer protocol.");
            }

            public void OnOuterProtocolConnected()
            {
            }

            public void OnInnerProtocolSet(INodeEndpointProtocolClient protocol)
            {
                this.innerProtocol = protocol;
            }

            public bool EnableDuplex
            {
                get
                {
                    return false;
                }
            }

            public bool Connected
            {
                get
                {
                    return this.connected;
                }
            }

            public void Disconnect()
            {
                this.address = null;
                this.endpointName = null;
                this.connected = false;
            }

            public void BeginListen()
            {
            }

            public void AddListener(INodeEndpointProtocolRequestListener listener)
            {
                lock (this.listeners)
                {
                    if (!this.listeners.Contains(listener))
                    {
                        this.listeners.Add(listener);
                    }
                }
            }

            public void RemoveListener(INodeEndpointProtocolRequestListener listener)
            {
                lock (this.listeners)
                {
                    this.listeners.Remove(listener);
                }
            }

            public void Send(string message)
            {
                if (!this.Connected) throw new InvalidOperationException("The protocol is not connected.");
                SendHttpRequest(message, true, (a, b, m) =>
                {
                    Request request = new Request(message);
                    lock (this.listeners)
                    {
                        foreach (var listener in this.listeners)
                        {
                            listener.OnReceivedRequest(request);
                        }
                    }
                });
            }
        }
    }
}
