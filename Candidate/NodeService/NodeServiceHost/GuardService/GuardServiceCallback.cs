using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using NodeService.Endpoints;
using System.Threading;
using NodeService;
using System.Xml.Linq;

namespace NodeServiceHost.GuardService
{
    public abstract class GuardServiceCallback : StrongTypedNodeEndpoint, IGuardServiceCallback
    {
        private Semaphore semaphore = null;

        private void CloseSemaphore()
        {
            if (this.semaphore != null)
            {
                this.semaphore.Close();
                this.semaphore.Dispose();
                this.semaphore = null;
            }
        }

        public GuardServiceCallback()
        {
            this.EnableAsynchronization = true;
        }

        public override void Dispose()
        {
            CloseSemaphore();
            base.Dispose();
        }

        [NodeEndpointMethod]
        public virtual void Start(string semaphoreName)
        {
            CloseSemaphore();
            bool createdNew = false;
            Semaphore semaphore = new Semaphore(0, 1, semaphoreName, out createdNew);
            if (createdNew)
            {
                this.semaphore = semaphore;
            }
            else
            {
                semaphore.Close();
                semaphore.Dispose();
            }
        }

        [NodeEndpointMethod]
        public virtual void Stop()
        {
            CloseSemaphore();
        }

        [NodeEndpointMethod]
        public virtual XElement GetServiceDescription()
        {
            return new XElement("BackgroundService",
                new XComment("This service does not contains any accessible node endpoint.")
                );
        }

        [NodeEndpointMethod]
        public abstract void StartTracing();

        [NodeEndpointMethod]
        public abstract void StopTracing();

        [NodeEndpointMethod]
        public abstract bool IsTracing();

        [NodeEndpointMethod]
        public abstract XElement GetTracingResult();
    }
}
