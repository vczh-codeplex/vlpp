using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Xml.Linq;
using System.Reflection;
using System.IO;

namespace NodeService.Endpoints
{
    public class StrongTypedNodeEndpoint : NodeEndpointBase
    {
        private Dictionary<string, MethodInfo> methods = new Dictionary<string, MethodInfo>();
        public StrongTypedNodeEndpointSerializer Serializer { get; private set; }

        public static MethodInfo[] GetMethodInfos(Type serviceType)
        {
            List<Type> baseTypes = new List<Type>();
            Type currentType = serviceType;
            while (currentType != null)
            {
                baseTypes.Add(currentType);
                currentType = currentType.BaseType;
            }

            MethodInfo[] methodInfos = baseTypes
                .SelectMany(
                    type => type
                    .GetMethods(BindingFlags.Public | BindingFlags.NonPublic | BindingFlags.Instance)
                    .Where(m => m.GetCustomAttributes(typeof(NodeEndpointMethodAttribute), false).Length > 0)
                    )
                .ToArray();
            return methodInfos;
        }

        private void Initialize()
        {
            this.Serializer = new StrongTypedNodeEndpointSerializer();

            foreach (var methodInfo in GetMethodInfos(this.GetType()))
            {
                this.methods.Add(methodInfo.Name, methodInfo);
                foreach (var type in methodInfo
                    .GetParameters()
                    .Select(p => p.ParameterType)
                    .Concat(new Type[] { methodInfo.ReturnType })
                    .Where(t => t != typeof(INodeEndpointRequest) && t != typeof(void) && t != typeof(Stream))
                    )
                {
                    this.Serializer.AddDefaultSerializer(type);
                }
            }
        }

        public StrongTypedNodeEndpoint()
        {
            Initialize();
        }

        public StrongTypedNodeEndpoint(string name)
            : base(name)
        {
            Initialize();
        }

        private object[] Translate(IEnumerable<ParameterInfo> parameters, XElement body)
        {
            return parameters
                .Select(p => Tuple.Create(body.Element(p.Name).FirstNode, p.ParameterType))
                .Select(data => this.Serializer.Deserialize(data.Item1, data.Item2))
                .ToArray();
        }

        protected void Respond(INodeEndpointRequest request, object response)
        {
            Stream streamResponse = response as Stream;
            if (streamResponse != null)
            {
                request.Respond(streamResponse);
            }
            else
            {
                request.Respond(this.Serializer.Serialize(response));
            }
        }

        protected override void OnQueueRequest(INodeEndpointRequest request)
        {
            try
            {
                MethodInfo method = null;
                if (this.methods.TryGetValue(request.Method, out method))
                {
                    ParameterInfo firstParameter = method.GetParameters().FirstOrDefault();
                    if (firstParameter != null && firstParameter.ParameterType == typeof(INodeEndpointRequest))
                    {
                        method.Invoke(this,
                            new object[] { request }
                            .Concat(Translate(method.GetParameters().Skip(1), request.Body))
                            .ToArray()
                            );
                    }
                    else
                    {
                        object result = method.Invoke(this, Translate(method.GetParameters(), request.Body));
                        if (method.ReturnType == typeof(void))
                        {
                            request.Respond();
                        }
                        else
                        {
                            Respond(request, result);
                        }
                    }
                }
                else
                {
                    request.Respond(new InvalidOperationException("Method \"" + request.Method + "\" not exists."));
                }
            }
            catch (TargetInvocationException exception)
            {
                request.Respond(exception.InnerException);
            }
            catch (ArgumentException)
            {
                request.Respond(new InvalidOperationException("Cannot parse parameters."));
            }
            catch (TargetParameterCountException)
            {
                request.Respond(new InvalidOperationException("Cannot parse parameters."));
            }
        }
    }

    [AttributeUsage(AttributeTargets.Method)]
    public class NodeEndpointMethodAttribute : Attribute
    {
    }

    [AttributeUsage(AttributeTargets.Class | AttributeTargets.Struct)]
    public class NodeEndpointDataTypeAttribute : Attribute
    {
    }

    [AttributeUsage(AttributeTargets.Property | AttributeTargets.Field)]
    public class NodeEndpointDataMemberAttribute : Attribute
    {
    }

    [AttributeUsage(AttributeTargets.Class | AttributeTargets.Struct, AllowMultiple = true)]
    public class NodeEndpointKnownTypeAttribute : Attribute
    {
        public Type KnownType { get; private set; }

        public NodeEndpointKnownTypeAttribute(Type knownType)
        {
            this.KnownType = knownType;
        }
    }
}
