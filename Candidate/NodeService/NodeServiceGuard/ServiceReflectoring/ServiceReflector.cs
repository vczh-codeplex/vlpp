using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Xml.Linq;
using NodeService;
using NodeService.Protocols;
using System.Net.Sockets;
using System.Reflection;
using System.Net;
using System.Text.RegularExpressions;

namespace NodeServiceGuard.ServiceReflectoring
{
    public class ServiceReflector
    {
        public class TypeContract
        {
            public string FullName { get; private set; }

            public TypeContract(string fullname)
            {
                this.FullName = fullname;
            }

            public override string ToString()
            {
                return this.FullName;
            }
        };

        public class ParameterContract
        {
            public string Name { get; private set; }
            public TypeContract Type { get; private set; }

            public ParameterContract(XElement elementParameter)
            {
                this.Name = elementParameter.Element("Name").Value;
                this.Type = new TypeContract(elementParameter.Element("Type").Value);
            }

            public override string ToString()
            {
                return this.Name;
            }
        };

        public class MethodContract
        {
            public string Name { get; private set; }
            public IEnumerable<ParameterContract> Parameters { get; private set; }

            public MethodContract(XElement elementMethod)
            {
                this.Name = elementMethod.Element("Name").Value;
                this.Parameters = elementMethod
                    .Element("Parameters")
                    .Elements("Parameter")
                    .Select(p => new ParameterContract(p))
                    .ToArray();
            }

            public override string ToString()
            {
                return this.Name;
            }
        };

        private INodeEndpointProtocolFactory factory;
        private INodeEndpointProtocolFactory outestFactory;
        private string protocolAddress;
        private string endpointName;
        private MethodContract[] methods;

        public ServiceReflector(XElement serviceDescription)
        {
            this.factory = BuildFactory(serviceDescription.Element("Protocol"), out this.outestFactory);
            this.protocolAddress = serviceDescription.Element("Address").Element("ProtocolAddress").Value;
            this.endpointName = serviceDescription.Element("Address").Element("EndpointName").Value;

            this.methods = serviceDescription
                .Element("Contract")
                .Element("Methods")
                .Elements("Method")
                .Select(m => new MethodContract(m))
                .ToArray();
        }

        public INodeEndpointProtocolFactory Factory
        {
            get
            {
                return this.factory;
            }
        }

        public string ProtocolAddress
        {
            get
            {
                return this.protocolAddress;
            }
        }

        public string EndpointName
        {
            get
            {
                return this.endpointName;
            }
        }

        public string GetSuggestedAddress(string host)
        {
            if (this.outestFactory is NamedPipeProtocolFactory)
            {
                return host + "/" + this.protocolAddress;
            }
            else if (this.outestFactory is TcpProtocolFactory)
            {
                AddressFamily addressFamily = ((TcpProtocolFactory)this.outestFactory).AddressFamily;
                foreach (var ip in Dns.GetHostAddresses(host))
                {
                    if (ip.AddressFamily == addressFamily)
                    {
                        return ip.ToString() + ":" + this.protocolAddress;
                    }
                }
            }
            else if (this.outestFactory is HttpProtocolFactory)
            {
                int index = this.protocolAddress.IndexOfAny("+*".ToCharArray());
                if (index == -1)
                {
                    return this.protocolAddress;
                }
                else
                {
                    string result = this.protocolAddress.Remove(index, 1).Insert(index, host);
                    return result;
                }
            }
            else if (this.outestFactory is TcpShareProtocolFactory)
            {
                foreach (var ip in Dns.GetHostAddresses(host))
                {
                    if (ip.AddressFamily == AddressFamily.InterNetwork)
                    {
                        return ip.ToString() + ":" + this.protocolAddress;
                    }
                }
            }
            return "";
        }

        public IEnumerable<MethodContract> Methods
        {
            get
            {
                return this.methods;
            }
        }

        private static INodeEndpointProtocolFactory BuildFactory(XElement protocolDescription, out INodeEndpointProtocolFactory outestFactory)
        {
            outestFactory = null;
            INodeEndpointProtocolFactory factory = null;
            foreach (var factoryDescription in protocolDescription.Elements().Reverse())
            {
                factory = BuildFactory(factoryDescription, factory);
                if (outestFactory == null)
                {
                    outestFactory = factory;
                }
            }
            return factory;
        }

        private static INodeEndpointProtocolFactory BuildFactory(XElement factoryDescription, INodeEndpointProtocolFactory previousFactory)
        {
            string factoryName = factoryDescription.Name.LocalName;
            switch (factoryName)
            {
                case "NamedPipeProtocolFactory":
                    {
                        if (previousFactory != null)
                        {
                            throw new ArgumentException(factoryName + " should be the outest protocol.");
                        }
                        return new NamedPipeProtocolFactory();
                    }
                case "TcpProtocolFactory":
                    {
                        if (previousFactory != null)
                        {
                            throw new ArgumentException(factoryName + " should be the outest protocol.");
                        }
                        TcpProtocolFactory factory = new TcpProtocolFactory();
                        factory.AddressFamily = (AddressFamily)typeof(AddressFamily)
                            .GetField(factoryDescription.Element("AddressFamily").Value, BindingFlags.Public | BindingFlags.Static)
                            .GetValue(null);
                        factory.ProtocolType = (ProtocolType)typeof(ProtocolType)
                            .GetField(factoryDescription.Element("ProtocolType").Value, BindingFlags.Public | BindingFlags.Static)
                            .GetValue(null);
                        return factory;
                    }
                case "HttpProtocolFactory":
                    {
                        if (previousFactory != null)
                        {
                            throw new ArgumentException(factoryName + " should be the outest protocol.");
                        }
                        return new HttpProtocolFactory();
                    }
                case "TcpShareProtocolFactory":
                    {
                        if (previousFactory != null)
                        {
                            throw new ArgumentException(factoryName + " should be the outest protocol.");
                        }
                        return new TcpShareProtocolFactory();
                    }
                case "TranslatorProtocolFactory":
                    {
                        if (previousFactory == null)
                        {
                            throw new ArgumentException(factoryName + " cannot be the outest protocol.");
                        }
                        XElement handlerDescription = (XElement)factoryDescription.FirstNode;
                        string handlerName = handlerDescription.Name.LocalName;
                        switch (handlerName)
                        {
                            case "TranslatorProtocolHandlerFactorySimple":
                                {
                                    XElement handlerSimpleDescription = (XElement)handlerDescription.FirstNode;
                                    string handlerSimpleName = handlerSimpleDescription.Name.LocalName;
                                    switch (handlerSimpleName)
                                    {
                                        case "GzipProtocolHandler":
                                            return previousFactory.With(new GzipProtocolHandler());
                                        default:
                                            throw new ArgumentException("Don't know how to deal with " + handlerSimpleName + ".");
                                    }
                                }
                            default:
                                throw new ArgumentException("Don't know how to deal with " + handlerName + ".");
                        }
                    }
                default:
                    throw new ArgumentException("Don't know how to deal with " + factoryName + ".");
            }
        }
    }
}
