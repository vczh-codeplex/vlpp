using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using NodeService;
using System.Threading;
using System.Diagnostics;
using System.Net;
using System.IO;
using System.Xml.Linq;

namespace NodeServiceHost.GuardService
{
    public class GuardServiceEndpointServer : DuplexNodeEndpointServer<GuardService, IGuardServiceCallbackClient>
    {
        public GuardServiceSharedData SharedData { get; private set; }
        public GuardServiceEndpointServerCallback callback = null;
        private HttpListener httpListener = null;
        private int httpListenerPort;

        public GuardServiceEndpointServer(int httpListenerPort)
        {
            this.SharedData = new GuardServiceSharedData();
            this.callback = new GuardServiceEndpointServerCallback(this.SharedData);
            this.httpListenerPort = httpListenerPort;
        }

        public string GetHttpListenerAddress(string host)
        {
            return string.Format("http://{0}:{1}/NodeServices/", host, this.httpListenerPort);
        }

        public void Start()
        {
            Start(this.callback);

            this.httpListener = new HttpListener();
            this.httpListener.Prefixes.Add(GetHttpListenerAddress("+"));
            this.httpListener.Start();
            this.httpListener.BeginGetContext((a) => HttpListenerProc(this.httpListener, a), null);
        }

        public override void Start(INodeEndpointServerCallback<GuardService> callback)
        {
            base.Start(callback);
        }

        public override void Stop()
        {
            this.httpListener.Close();
            this.SharedData.StopAll();
            base.Stop();
        }

        #region Http Listener

        private void HttpListenerFillResponse(string url, HttpListenerContext context)
        {
            if (url == "/NODESERVICES/" || url == "/NODESERVICES/INDEX.HTM")
            {
                context.Response.ContentType = "text/html; charset=utf-8";
                using (StreamWriter writer = new StreamWriter(context.Response.OutputStream))
                {
                    writer.Write("<html><header><title>Node Services</title></header><body>");
                    writer.Write("<p>Welcome to Node Services!</p>");
                    writer.Write("<p>To get a complete service token list, go to <a href=\"ServiceList.xml\">ServiceList.xml</a>.</p>");
                    writer.Write("<p>Here are the descriptions of all available services:</p>");
                    foreach (var service in this.SharedData.GuardedServices)
                    {
                        writer.Write(string.Format("<p><a href=\"{0}.xml\">{1}</a></p>", service.Token, service.Description.Name));
                    }
                    writer.Write("<p>Powered by: Vczh Node Services Library for .NET <a href=\"http://vlpp.codeplex.com\">http://vlpp.codeplex.com</a></p>");
                    writer.Write("</body></html>");
                }
            }
            else if (url == "/NODESERVICES/SERVICELIST.XML")
            {
                context.Response.ContentType = "text/xml; charset=utf-8";

                using (StreamWriter writer = new StreamWriter(context.Response.OutputStream))
                {
                    writer.Write(
                        new XElement("Services",
                            this.SharedData.GuardedServices
                            .Select(s => new XElement(
                                "Service",
                                new XElement("Name", s.Description.Name),
                                new XElement("Token", s.Token)
                                ))
                            .ToArray()
                            )
                        );
                    writer.Flush();
                }
            }
            else if (url.StartsWith("/NODESERVICES/") && url.EndsWith(".XML"))
            {
                context.Response.ContentType = "text/xml; charset=utf-8";
                Guid token = Guid.Parse(url.Substring(14, url.Length - 18));
                GuardServiceSharedData.ServiceData data = this.SharedData.GetGuardedService(token);
                XElement serviceDescription = data.Service.Callback.GetServiceDescription();

                using (StreamWriter writer = new StreamWriter(context.Response.OutputStream))
                {
                    writer.Write(serviceDescription.ToString());
                    writer.Flush();
                }
            }
        }

        private void HttpListenerProc(HttpListener listener, IAsyncResult asyncResult)
        {
            try
            {
                HttpListenerContext context = listener.EndGetContext(asyncResult);
                string url = context.Request.Url.AbsolutePath.ToUpper();
                try
                {
                    HttpListenerFillResponse(url, context);
                }
                catch (Exception)
                {
                    context.Response.StatusCode = 404;
                    context.Response.Close();
                }
                listener.BeginGetContext((a) => HttpListenerProc(listener, a), null);
            }
            catch (ObjectDisposedException)
            {
            }
        }

        #endregion
    }

    public class GuardServiceSharedData
    {
        private Dictionary<Guid, ServiceData> guardedServices = new Dictionary<Guid, ServiceData>();

        public class ServiceData
        {
            public GuardedServiceDescription Description { get; set; }
            public GuardService Service { get; set; }
            public string SemaphoreName { get; set; }
            public Guid Token { get; set; }
        }

        public ServiceData[] GuardedServices
        {
            get
            {
                lock (this.guardedServices)
                {
                    return this.guardedServices.Values.ToArray();
                }
            }
        }

        public ServiceData GetGuardedService(Guid token)
        {
            lock (this.guardedServices)
            {
                ServiceData data = null;
                this.guardedServices.TryGetValue(token, out data);
                return data;
            }
        }

        public void RestartFailedServices()
        {
            lock (this.guardedServices)
            {
                foreach (var pair in this.guardedServices.ToArray())
                {
                    bool createdNew = false;
                    using (Semaphore semaphore = new Semaphore(0, 1, pair.Value.SemaphoreName, out createdNew))
                    {
                        semaphore.Close();
                    }
                    if (createdNew)
                    {
                        this.guardedServices.Remove(pair.Key);
                        Process.Start(pair.Value.Description.ExecutablePath, pair.Value.Description.Arguments);
                    }
                }
            }
        }

        public void StopAll(Action<Guid> callback = null)
        {
            lock (this.guardedServices)
            {
                foreach (var pair in this.guardedServices)
                {
                    try
                    {
                        pair.Value.Service.Callback.Stop();
                    }
                    catch (Exception)
                    {
                    }
                    if (callback != null)
                    {
                        callback(pair.Value.Token);
                    }
                }
                this.guardedServices.Clear();
            }
        }

        public ServiceData Register(GuardedServiceDescription description, GuardService service)
        {
            lock (this.guardedServices)
            {
                Guid token = Guid.NewGuid();
                ServiceData serviceData = new ServiceData()
                {
                    Description = description,
                    Service = service,
                    SemaphoreName = "NodeServerGuardedService-" + token.ToString(),
                    Token = token,
                };
                this.guardedServices.Add(token, serviceData);
                service.Callback.Start(serviceData.SemaphoreName);
                return serviceData;
            }
        }

        public void Unregister(Guid token)
        {
            lock (this.guardedServices)
            {
                this.guardedServices.Remove(token);
            }
        }

        public void Unregister(GuardService service)
        {
            Guid token = Guid.Empty;
            lock (this.guardedServices)
            {
                foreach (var pair in this.guardedServices)
                {
                    if (pair.Value.Service == service)
                    {
                        token = pair.Value.Token;
                        break;
                    }
                }
            }
            if (token != Guid.Empty)
            {
                Unregister(token);
            }
        }
    };
}
