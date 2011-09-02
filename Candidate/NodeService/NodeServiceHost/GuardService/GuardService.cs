using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using NodeService.Endpoints;
using NodeService;
using System.Threading;
using System.Xml.Linq;

namespace NodeServiceHost.GuardService
{
    [NodeEndpoint(GuardServiceConfiguration.EndpointName)]
    public class GuardService : StrongTypedNodeEndpoint, IDuplexNodeEndpoint<IGuardServiceCallbackClient>
    {
        public IGuardServiceCallbackClient Callback { get; set; }
        public GuardServiceSharedData SharedData { get; private set; }

        public GuardService(GuardServiceSharedData sharedData)
        {
            this.EnableAsynchronization = true;
            this.SharedData = sharedData;
        }

        [NodeEndpointMethod]
        public Guid Register(GuardedServiceDescription description)
        {
            var data = this.SharedData.Register(description, this);
            return data.Token;
        }

        [NodeEndpointMethod]
        public void Unregister(Guid token)
        {
            this.SharedData.Unregister(token);
        }
    }

    public interface IGuardService
    {
        Guid Register(GuardedServiceDescription description);
        void Unregister(Guid token);
    }

    [NodeEndpointDataType]
    public class GuardedServiceDescription
    {
        [NodeEndpointDataMember]
        public string ExecutablePath { get; set; }

        [NodeEndpointDataMember]
        public string Arguments { get; set; }

        [NodeEndpointDataMember]
        public string Name { get; set; }
    }

    public interface IGuardServiceCallback
    {
        void Start(string semaphoreName);
        void Stop();
        XElement GetServiceDescription();

        void StartTracing();
        void StopTracing();
        bool IsTracing();
        XElement GetTracingResult();
    }

    public interface IGuardServiceCallbackClient : IGuardServiceCallback, INodeEndpointClient
    {
    }
}
