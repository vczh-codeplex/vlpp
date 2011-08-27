using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Xml.Linq;
using System.CodeDom.Compiler;
using System.CodeDom;
using System.Threading.Tasks;
using System.Reflection;

namespace NodeService.Endpoints
{
    public abstract class StrongTypedNodeEndpointClient : INodeEndpointClient
    {
        private static Dictionary<Type, Type> clientImplementations = new Dictionary<Type, Type>();

        public INodeEndpointClientProvider Provider { get; private set; }
        public StrongTypedNodeEndpointSerializer Serializer { get; private set; }

        public StrongTypedNodeEndpointClient(INodeEndpointClientProvider provider)
        {
            this.Provider = provider;
        }

        public void Dispose()
        {
            if (this.Provider.Protocol != null)
            {
                this.Provider.Protocol.Disconnect();
            }
        }

        protected void Initialize(Type interfceType)
        {
            this.Serializer = new StrongTypedNodeEndpointSerializer();
            foreach (var methodInfo in this.GetType().GetMethods())
            {
                foreach (var type in methodInfo
                    .GetParameters()
                    .Select(p => p.ParameterType)
                    .Concat(new Type[] { methodInfo.ReturnType })
                    .Where(t => t != typeof(INodeEndpointRequest) && t != typeof(void) && t != typeof(Task))
                    )
                {
                    if (type.IsGenericType && type.GetGenericTypeDefinition() == typeof(Task<>))
                    {
                        this.Serializer.AddDefaultSerializer(type.GetGenericArguments()[0]);
                    }
                    else
                    {
                        this.Serializer.AddDefaultSerializer(type);
                    }
                }
            }
        }

        protected object Execute(string method, Type type, string[] names, object[] arguments)
        {
            XElement body = new XElement("Parameters",
                names
                .Zip(arguments, (a, b) => Tuple.Create(a, b))
                .Select(t => new XElement(t.Item1, this.Serializer.Serialize(t.Item2)))
                );
            var response = this.Provider.Send(method, body);
            response.WaitForResponse();
            if (response.RequestState == RequestState.RaisedException)
            {
                throw response.Exception;
            }
            else if (type != typeof(void))
            {
                return this.Serializer.Deserialize(response.Response, type);
            }
            else
            {
                return null;
            }
        }

        protected Task ExecuteTask(string method, string[] names, object[] arguments)
        {
            Task task = new Task(() => Execute(method, typeof(void), names, arguments));
            task.Start();
            return task;
        }

        protected Task<T> ExecuteTask<T>(string method, string[] names, object[] arguments)
        {
            Task<T> task = new Task<T>(() => (T)Execute(method, typeof(T), names, arguments));
            task.Start();
            return task;
        }

        private static CodeExpression PassParameterNames(MethodInfo methodInfo)
        {
            return new CodeArrayCreateExpression(
                typeof(string),
                methodInfo
                    .GetParameters()
                    .Select(p => new CodePrimitiveExpression(p.Name))
                    .ToArray()
                );
        }

        private static CodeExpression PassParameterValues(MethodInfo methodInfo)
        {
            return new CodeArrayCreateExpression(
                typeof(object),
                methodInfo
                    .GetParameters()
                    .Select(p => new CodeArgumentReferenceExpression(p.Name))
                    .ToArray()
                );
        }

        private static CodeExpression InvokeExecute(MethodInfo methodInfo)
        {
            CodeExpression execution = new CodeMethodInvokeExpression(
                new CodeThisReferenceExpression(),
                "Execute",
                new CodeExpression[] { 
                    new CodePrimitiveExpression(methodInfo.Name),
                    new CodeTypeOfExpression(methodInfo.ReturnType),
                    PassParameterNames(methodInfo),
                    PassParameterValues(methodInfo),
                    }
                .ToArray()
                );
            return execution;
        }

        private static CodeExpression InvokeExecuteTask(MethodInfo methodInfo)
        {
            CodeExpression execution = new CodeMethodInvokeExpression(
                methodInfo.ReturnType == typeof(Task) ?
                    new CodeMethodReferenceExpression(
                        new CodeThisReferenceExpression(),
                        "ExecuteTask"
                        ) :
                    new CodeMethodReferenceExpression(
                        new CodeThisReferenceExpression(),
                        "ExecuteTask",
                        new CodeTypeReference(methodInfo.ReturnType.GetGenericArguments()[0])
                        ),
                new CodeExpression[] { 
                    new CodePrimitiveExpression(methodInfo.Name),
                    PassParameterNames(methodInfo),
                    PassParameterValues(methodInfo),
                    }
                .ToArray()
                );
            return execution;
        }

        private static Type CreateClientImplementation(Type infertaceType)
        {
            Type clientType = null;
            if (!clientImplementations.TryGetValue(infertaceType, out clientType))
            {

                CodeDomProvider codedomProvider = CodeDomProvider.CreateProvider("CSharp");
                string namespaceName = "NodeService.StringTypedNodeEndpointClientAutoGeneratedClients";
                string typeName = "ImplementationOf" + infertaceType.Name;

                CodeCompileUnit codeCompileUnit = new CodeCompileUnit();
                {
                    CodeNamespace codeNamespace = new CodeNamespace(namespaceName);
                    codeCompileUnit.Namespaces.Add(codeNamespace);

                    CodeTypeDeclaration clientDeclaration = new CodeTypeDeclaration(typeName);
                    clientDeclaration.BaseTypes.Add(typeof(StrongTypedNodeEndpointClient));
                    clientDeclaration.BaseTypes.Add(infertaceType);
                    codeNamespace.Types.Add(clientDeclaration);

                    CodeConstructor clientConstructor = new CodeConstructor();
                    clientDeclaration.Members.Add(clientConstructor);
                    clientConstructor.Attributes = MemberAttributes.Public;
                    clientConstructor.Parameters.Add(new CodeParameterDeclarationExpression(typeof(INodeEndpointClientProvider), "provider"));
                    clientConstructor.BaseConstructorArgs.Add(new CodeArgumentReferenceExpression("provider"));
                    clientConstructor.Statements.Add(
                        new CodeExpressionStatement(
                            new CodeMethodInvokeExpression(
                                new CodeThisReferenceExpression(),
                                "Initialize",
                                new CodeTypeOfExpression(typeof(Type))
                                )
                            )
                        );

                    foreach (var methodInfo in infertaceType.GetMethods())
                    {
                        CodeMemberMethod clientMethod = new CodeMemberMethod();
                        clientDeclaration.Members.Add(clientMethod);
                        clientMethod.Attributes = MemberAttributes.Public;
                        clientMethod.Name = methodInfo.Name;
                        clientMethod.ImplementationTypes.Add(infertaceType);
                        clientMethod.ReturnType = new CodeTypeReference(methodInfo.ReturnType);
                        foreach (var parameterInfo in methodInfo.GetParameters())
                        {
                            clientMethod.Parameters.Add(new CodeParameterDeclarationExpression(parameterInfo.ParameterType, parameterInfo.Name));
                        }


                        if (methodInfo.ReturnType == typeof(void))
                        {
                            clientMethod.Statements.Add(
                                new CodeExpressionStatement(
                                    InvokeExecute(methodInfo)
                                    )
                                );
                        }
                        else if (methodInfo.ReturnType == typeof(Task))
                        {
                            clientMethod.Statements.Add(
                                new CodeMethodReturnStatement(
                                    InvokeExecuteTask(methodInfo)
                                    )
                                );
                        }
                        else if (methodInfo.ReturnType.IsGenericType && methodInfo.ReturnType.GetGenericTypeDefinition() == typeof(Task<>))
                        {
                            clientMethod.Statements.Add(
                                new CodeMethodReturnStatement(
                                    InvokeExecuteTask(methodInfo)
                                    )
                                );
                        }
                        else
                        {
                            clientMethod.Statements.Add(
                                new CodeMethodReturnStatement(
                                    new CodeCastExpression(
                                        methodInfo.ReturnType,
                                        InvokeExecute(methodInfo)
                                        )
                                    )
                                );
                        }
                    }
                }

                CompilerParameters options = new CompilerParameters();
                options.GenerateExecutable = false;
                options.GenerateInMemory = true;
                options.IncludeDebugInformation = false;
                options.ReferencedAssemblies.AddRange(
                    new string[] { typeof(StrongTypedNodeEndpointClient).Assembly.Location, infertaceType.Assembly.Location }
                    .Concat(typeof(StrongTypedNodeEndpointClient).Assembly.GetReferencedAssemblies().Select(n => n.CodeBase))
                    .Concat(infertaceType.Assembly.GetReferencedAssemblies().Select(n => n.CodeBase))
                    .Distinct()
                    .ToArray()
                );

                CompilerResults result = codedomProvider.CompileAssemblyFromDom(options, codeCompileUnit);
                codedomProvider.Dispose();
                clientType = result.CompiledAssembly.GetType(namespaceName + "." + typeName);
                clientImplementations.Add(infertaceType, clientType);
            }
            return clientType;
        }

        public static T Create<T>(INodeEndpointClientProvider provider)
            where T : INodeEndpointClient
        {
            Type clientType = CreateClientImplementation(typeof(T));
            return (T)clientType
                .GetConstructor(new Type[] { typeof(INodeEndpointClientProvider) })
                .Invoke(new object[] { provider });
        }
    }
}
