using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using NodeService;

namespace NodeServiceHost.GuardService
{
    public class GuardServiceEndpointServerCallback : INodeEndpointServerCallback<GuardService>
    {
        private INodeEndpointProtocolFactory protocolFactory = null;
        private GuardServiceSharedData sharedData = null;

        public GuardServiceEndpointServerCallback(GuardServiceSharedData sharedData)
        {
            this.sharedData = sharedData;
        }

        public INodeEndpointProtocolFactory ProtocolFactory
        {
            get
            {
                if (this.protocolFactory == null)
                {
                    this.protocolFactory = GuardServiceConfiguration.CreateProtocolFactory();
                }
                return this.protocolFactory;
            }
        }

        public string ProtocolAddress
        {
            get
            {
                return GuardServiceConfiguration.Address;
            }
        }

        public GuardService CreateEndpoint()
        {
            return new GuardService(this.sharedData);
        }

        public void OnEndpointStart(GuardService endpoint, INodeEndpointProtocolServer protocolServer)
        {
        }

        public void OnEndpointStopped(GuardService endpoint, INodeEndpointProtocolServer protocolServer)
        {
            this.sharedData.Unregister(endpoint);
        }
    }
}
