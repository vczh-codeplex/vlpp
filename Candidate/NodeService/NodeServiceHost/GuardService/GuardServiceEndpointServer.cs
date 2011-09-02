using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using NodeService;
using System.Threading;
using System.Diagnostics;

namespace NodeServiceHost.GuardService
{
    public class GuardServiceEndpointServer : DuplexNodeEndpointServer<GuardService, IGuardServiceCallbackClient>
    {
        public GuardServiceSharedData SharedData { get; private set; }
        public GuardServiceEndpointServerCallback callback = null;

        public GuardServiceEndpointServer()
        {
            this.SharedData = new GuardServiceSharedData();
            this.callback = new GuardServiceEndpointServerCallback(this.SharedData);
        }

        public void Start()
        {
            Start(this.callback);
        }

        public override void Start(INodeEndpointServerCallback<GuardService> callback)
        {
            base.Start(callback);
        }

        public override void Stop()
        {
            this.SharedData.StopAll();
            base.Stop();
        }
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
