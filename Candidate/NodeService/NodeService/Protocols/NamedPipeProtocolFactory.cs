using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.IO.Pipes;
using System.Text.RegularExpressions;
using System.Threading;

namespace NodeService.Protocols
{
    public class NamedPipeProtocolFactory : INodeEndpointProtocolFactory
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
            private ProtocolBase protocolBase;
            private Guid guid;
            private string peerAddress;
            private string method;
            private string message;

            public Request(ProtocolBase protocolBase, Guid guid, string peerAddress, string method, string message)
            {
                this.protocolBase = protocolBase;
                this.guid = guid;
                this.peerAddress = peerAddress;
                this.method = method;
                this.message = message;
            }

            public string Session
            {
                get
                {
                    return this.guid.ToString();
                }
            }

            public string PeerAddress
            {
                get
                {
                    return this.peerAddress;
                }
            }

            public string Method
            {
                get
                {
                    return this.method;
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
                this.protocolBase.Respond(this.guid, response);
            }
        }

        class Response : INodeEndpointProtocolResponse
        {
            private string response;
            private List<Action<INodeEndpointProtocolResponse>> callbacks = new List<Action<INodeEndpointProtocolResponse>>();

            public Response()
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

            public bool ReceivedResponse
            {
                get
                {
                    return this.response != null;
                }
            }

            string INodeEndpointProtocolResponse.Response
            {
                get
                {
                    return this.response;
                }
            }

            public void WaitForResponse()
            {
                if (this.ResponseEvent != null)
                {
                    this.ResponseEvent.WaitOne();
                }
            }

            public void RegisterCallback(Action<INodeEndpointProtocolResponse> callback)
            {
                lock (this.callbacks)
                {
                    this.callbacks.Add(callback);
                    if (this.ReceivedResponse)
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

            public void SetResponse(string response)
            {
                lock (this.callbacks)
                {
                    this.response = response;
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

        abstract class ProtocolBase : INodeEndpointProtocol
        {
            protected PipeStream stream;
            private Dictionary<Guid, Response> responses = new Dictionary<Guid, Response>();

            public bool EnableDuplex
            {
                get
                {
                    return true;
                }
            }

            public bool Connected
            {
                get
                {
                    return this.stream != null && this.stream.IsConnected;
                }
            }

            public INodeEndpointProtocol OuterProtocol
            {
                get
                {
                    return null;
                }
            }

            public abstract INodeEndpointProtocol InnerProtocol { get; }

            public void Disconnect()
            {
                if (this.stream != null)
                {
                    this.stream.Close();
                    this.stream.Dispose();
                    this.stream = null;
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

            public INodeEndpointProtocolRequestListener Listener { get; set; }

            public void BeginListen()
            {
                if (!this.Connected) throw new InvalidOperationException("The protocol is not connected.");
                byte[] lead = new byte[sizeof(int)];
                this.stream.BeginRead(lead, 0, lead.Length, r => ReadCallback(r, lead), null);
            }

            public INodeEndpointProtocolResponse Send(string method, string message)
            {
                if (!this.Connected) throw new InvalidOperationException("The protocol is not connected.");
                Guid guid = Guid.NewGuid();
                string protocolMessage = BuildRequest(guid, method, message);
                Response response = new Response();
                lock (this.responses)
                {
                    this.responses.Add(guid, response);
                }
                Write(protocolMessage);
                return response;
            }

            internal void Respond(Guid guid, string message)
            {
                string protocolMessage = BuildResponse(guid, message);
                Write(protocolMessage);
            }

            private static Regex requestString = new Regex(@"^\[REQUEST\]\[(?<GUID>[a-zA-Z0-9-]+)\]\[(?<METHOD>\w+)\](?<MESSAGE>.*)$", RegexOptions.Singleline);
            private static Regex responseString = new Regex(@"^\[RESPONSE\]\[(?<GUID>[a-zA-Z0-9-]+)\](?<MESSAGE>.*)$", RegexOptions.Singleline);

            private static string BuildRequest(Guid guid, string method, string message)
            {
                return "[REQUEST][" + guid.ToString() + "][" + method + "]" + message;
            }

            private static string BuildResponse(Guid guid, string message)
            {
                return "[RESPONSE][" + guid.ToString() + "]" + message;
            }

            private static bool SplitRequest(string request, out Guid guid, out string method, out string message)
            {
                Match match = requestString.Match(request);
                if (match.Success)
                {
                    guid = new Guid(match.Groups["GUID"].Value);
                    method = match.Groups["METHOD"].Value;
                    message = match.Groups["MESSAGE"].Value;
                    return true;
                }
                else
                {
                    guid = Guid.Empty;
                    method = null;
                    message = null;
                    return false;
                }
            }

            private static bool SplitResponse(string response, out Guid guid, out string message)
            {
                Match match = responseString.Match(response);
                if (match.Success)
                {
                    guid = new Guid(match.Groups["GUID"].Value);
                    message = match.Groups["MESSAGE"].Value;
                    return true;
                }
                else
                {
                    guid = Guid.Empty;
                    message = null;
                    return false;
                }
            }

            private void Write(string message)
            {
                byte[] lead = new byte[sizeof(int)];
                byte[] bytes = Encoding.UTF8.GetBytes(message);
                int length = bytes.Length;
                unsafe
                {
                    byte* plead = (byte*)&length;
                    for (int i = 0; i < sizeof(int); i++)
                    {
                        lead[i] = plead[i];
                    }
                }
                this.stream.Write(lead.Concat(bytes).ToArray(), 0, lead.Length + length);
            }

            private void ReadCallback(IAsyncResult asyncResult, byte[] lead)
            {
                int leadLength = this.stream.EndRead(asyncResult);
                if (leadLength == lead.Length)
                {
                    unsafe
                    {
                        fixed (byte* plead = lead)
                        {
                            leadLength = *(int*)plead;
                        }
                    }
                    byte[] bytes = new byte[leadLength];
                    int messageLength = this.stream.Read(bytes, 0, bytes.Length);
                    if (messageLength == leadLength)
                    {
                        string protocolMessage = Encoding.UTF8.GetString(bytes);
                        if (protocolMessage.StartsWith("[REQUEST]"))
                        {
                            Guid guid;
                            string method;
                            string message;
                            if (SplitRequest(protocolMessage, out guid, out method, out message))
                            {
                                if (this.Listener != null)
                                {
                                    Request request = new Request(this, guid, null, method, message);
                                    this.Listener.OnReceivedRequest(request);
                                }
                            }
                        }
                        else if (protocolMessage.StartsWith("[RESPONSE]"))
                        {
                            Guid guid;
                            string message;
                            if (SplitResponse(protocolMessage, out guid, out message))
                            {
                                lock (this.responses)
                                {
                                    Response response;
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
                BeginListen();
            }
        }

        class ServerListener : INodeEndpointProtocolServerListener
        {
            private string pipeName;

            public bool Connected
            {
                get
                {
                    return this.pipeName != null;
                }
            }

            public void Connect(string address, string endpointName)
            {
                if (this.Connected)
                {
                    Disconnect();
                }
                this.pipeName = address + "/" + endpointName;
            }

            public void Disconnect()
            {
                this.pipeName = null;
            }

            public INodeEndpointProtocolServer Listen()
            {
                NamedPipeServerStream serverStream = new NamedPipeServerStream(
                    this.pipeName,
                    PipeDirection.InOut,
                    NamedPipeServerStream.MaxAllowedServerInstances,
                    PipeTransmissionMode.Message,
                    PipeOptions.Asynchronous | PipeOptions.WriteThrough
                    );
                try
                {
                    serverStream.ReadMode = PipeTransmissionMode.Message;
                    serverStream.WaitForConnection();
                    if (serverStream.IsConnected)
                    {
                        return new Server(serverStream);
                    }
                }
                catch (Exception)
                {
                }

                if (serverStream != null)
                {
                    serverStream.Close();
                    serverStream.Disconnect();
                }
                return null;
            }
        }

        class Server : ProtocolBase, INodeEndpointProtocolServer
        {
            private INodeEndpointProtocolServer innerProtocol;

            public Server(NamedPipeServerStream stream)
            {
                this.stream = stream;
            }

            public void SetOuterProtocol(INodeEndpointProtocolServer protocol)
            {
                throw new InvalidOperationException("Named pipe protocol client cannot have a outer protocol.");
            }

            public void OnOuterProtocolListened()
            {
            }

            public void OnInnerProtocolSet(INodeEndpointProtocolServer protocol)
            {
                this.innerProtocol = protocol;
            }

            public override INodeEndpointProtocol InnerProtocol
            {
                get
                {
                    return this.innerProtocol;
                }
            }
        }

        class Client : ProtocolBase, INodeEndpointProtocolClient
        {
            private INodeEndpointProtocolClient innerProtocol;

            public bool Connect(string address, string endpointName, int timeout)
            {
                if (this.stream != null)
                {
                    Disconnect();
                }

                int index = address.IndexOf('/');
                string serverName = address.Substring(0, index);
                string pipeName = address.Substring(index + 1) + "/" + endpointName;
                NamedPipeClientStream clientStream = new NamedPipeClientStream(
                    serverName,
                    pipeName,
                    PipeDirection.InOut,
                    PipeOptions.Asynchronous | PipeOptions.WriteThrough
                    );
                try
                {
                    clientStream.Connect(timeout);
                    if (clientStream.IsConnected)
                    {
                        clientStream.ReadMode = PipeTransmissionMode.Message;
                        this.stream = clientStream;
                        if (this.innerProtocol != null)
                        {
                            this.innerProtocol.OnOuterProtocolConnected();
                        }
                        BeginListen();
                        return true;
                    }
                }
                catch (Exception)
                {
                }

                if (clientStream != null)
                {
                    clientStream.Close();
                    clientStream.Dispose();
                }
                return false;
            }

            public void SetOuterProtocol(INodeEndpointProtocolClient protocol)
            {
                throw new InvalidOperationException("Named pipe protocol client cannot have a outer protocol.");
            }

            public void OnOuterProtocolConnected()
            {
            }

            public void OnInnerProtocolSet(INodeEndpointProtocolClient protocol)
            {
                this.innerProtocol = protocol;
            }

            public override INodeEndpointProtocol InnerProtocol
            {
                get
                {
                    return this.innerProtocol;
                }
            }
        }
    }
}
