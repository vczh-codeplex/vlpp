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
            private HttpListenerContext context = null;
            private byte[] message = null;

            public Request(HttpListenerContext context)
            {
                this.context = context;
                this.message = this.context.Request.InputStream.ReadAllBytesAndClose();
            }

            public Request(byte[] message)
            {
                this.message = message;
            }

            public bool CanRespond
            {
                get
                {
                    return this.context != null;
                }
            }

            public byte[] Message
            {
                get
                {
                    return this.message;
                }
            }

            public void Respond(byte[] response)
            {
                if (this.context != null)
                {
                    Stream stream = this.context.Response.OutputStream;
                    stream.Write(response, 0, response.Length);
                    stream.Close();
                }
            }
        }

        class ServerListener : INodeEndpointProtocolServerListener
        {
            private HttpListener listener = null;
            private Server server = null;

            public bool Connected
            {
                get
                {
                    return this.listener != null && this.listener.IsListening;
                }
            }

            public void Connect(string address, string endpointName)
            {
                if (this.Connected)
                {
                    Disconnect();
                }
                this.listener = new HttpListener();
                this.listener.Prefixes.Add(address + endpointName + "/");
                try
                {
                    this.listener.Start();
                }
                catch (HttpListenerException)
                {
                    this.listener.Stop();
                    this.listener.Close();
                    this.listener = null;
                }
            }

            public void Disconnect()
            {
                if (this.listener != null)
                {
                    this.listener.Stop();
                    this.listener.Close();
                    this.listener = null;

                    if (this.server != null)
                    {
                        this.server.Disconnect();
                        this.server = null;
                    }
                }
            }

            public INodeEndpointProtocolServer Listen()
            {
                if (this.Connected && this.server == null)
                {
                    this.server = new Server(this.listener);
                    return this.server;
                }
                else
                {
                    return null;
                }
            }
        }

        class Server : INodeEndpointProtocolServer
        {
            private HttpListener listener = null;
            private INodeEndpointProtocolServer innerProtocol;
            private List<INodeEndpointProtocolRequestListener> listeners = new List<INodeEndpointProtocolRequestListener>();

            public Server(HttpListener listener)
            {
                this.listener = listener;
            }

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
                get
                {
                    return false;
                }
            }

            public bool Connected
            {
                get
                {
                    return this.listener != null;
                }
            }

            public void Disconnect()
            {
                if (this.listener != null)
                {
                    this.listener.Stop();
                    this.listener = null;
                }
            }

            public void BeginListen()
            {
                if (!this.Connected) throw new InvalidOperationException("The protocol is not connected.");
                this.listener.BeginGetContext(a =>
                {
                    HttpListenerContext context = this.listener.EndGetContext(a);
                    Request request = new Request(context);
                    if (request.RequestMessage() == "[CONNECT]")
                    {
                        request.Respond("[CONNECTED]");
                    }
                    else
                    {
                        lock (this.listeners)
                        {
                            foreach (var listener in this.listeners)
                            {
                                listener.OnReceivedRequest(request);
                            }
                        }
                    }
                    if (this.Connected)
                    {
                        BeginListen();
                    }
                }, null);
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

            public void Send(byte[] message)
            {
                throw new NotSupportedException("The protocol is not able to send data directly.");
            }
        }

        class Client : INodeEndpointProtocolClient
        {
            private INodeEndpointProtocolClient innerProtocol;
            private string address;
            private string endpointName;
            private bool connected = false;
            private List<INodeEndpointProtocolRequestListener> listeners = new List<INodeEndpointProtocolRequestListener>();

            private bool SendHttpRequest(byte[] body, bool asynchronized, Action<HttpWebRequest, HttpWebResponse, byte[]> callback)
            {
                HttpWebRequest request = (HttpWebRequest)HttpWebRequest.Create(address + endpointName + "/");
                request.UserAgent = "Vczh-NodeService-Http";
                request.Method = "POST";
                request.ContentType = "text/html; charset=UTF-8";
                request.ContentLength = body.Length;
                Stream requestStream = request.GetRequestStream();
                requestStream.Write(body, 0, body.Length);
                requestStream.Close();

                if (asynchronized)
                {
                    request.BeginGetResponse((a) =>
                    {
                        HttpWebResponse response = (HttpWebResponse)request.EndGetResponse(a);
                        byte[] message = response.GetResponseStream().ReadAllBytesAndClose();
                        callback(request, response, message);
                    }, null);
                }
                else
                {
                    try
                    {
                        HttpWebResponse response = (HttpWebResponse)request.GetResponse();
                        byte[] message = response.GetResponseStream().ReadAllBytesAndClose();
                        callback(request, response, message);
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

                byte[] message = null;
                SendHttpRequest("[CONNECT]".NodeServiceEncode(), false, (a, b, m) => message = m);
                if (message.NodeServiceDecode() == "[CONNECTED]")
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

            public void Send(byte[] message)
            {
                if (!this.Connected) throw new InvalidOperationException("The protocol is not connected.");
                SendHttpRequest(message, true, (a, b, m) =>
                {
                    Request request = new Request(m);
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
