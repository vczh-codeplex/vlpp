using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Xml.Linq;
using System.Reflection;

namespace NodeService.Endpoints
{
    public class StrongTypedNodeEndpoint : NodeEndpointBase
    {
        private Dictionary<string, MethodInfo> methods = new Dictionary<string, MethodInfo>();
        public StrongTypedNodeEndpointSerializer Serializer { get; private set; }

        private void FindMethods()
        {
            this.Serializer = new StrongTypedNodeEndpointSerializer();
            foreach (var methodInfo in this.GetType()
                .GetMethods(BindingFlags.Public | BindingFlags.NonPublic | BindingFlags.Instance)
                .Where(m => m.GetCustomAttributes(typeof(NodeEndpointMethodAttribute), false).Length > 0)
                )
            {
                this.methods.Add(methodInfo.Name, methodInfo);
                foreach (var type in methodInfo
                    .GetParameters()
                    .Select(p => p.ParameterType)
                    .Concat(new Type[] { methodInfo.ReturnType })
                    .Where(t => t != typeof(INodeEndpointRequest) && t != typeof(void))
                    )
                {
                    this.Serializer.AddDefaultSerializer(type);
                }
            }
        }

        public StrongTypedNodeEndpoint()
        {
            FindMethods();
        }

        public StrongTypedNodeEndpoint(string name)
            : base(name)
        {
            FindMethods();
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
            request.Respond(this.Serializer.Serialize(response));
        }

        protected override void OnQueueRequest(INodeEndpointRequest request)
        {
            try
            {
                MethodInfo method = this.methods[request.Method];
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
            catch (TargetInvocationException exception)
            {
                request.Respond(exception.InnerException);
            }
        }
    }

    [AttributeUsage(AttributeTargets.Method)]
    public class NodeEndpointMethodAttribute : Attribute
    {
    }
}
