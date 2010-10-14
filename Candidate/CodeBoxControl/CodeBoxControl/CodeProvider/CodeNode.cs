using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using CodeBoxControl.Core;
using System.CodeDom.Compiler;
using Microsoft.CSharp;
using System.CodeDom;
using System.Reflection;

namespace CodeBoxControl.CodeProvider
{
    public abstract class CodeNode
    {
        private static Dictionary<Type, Type> implementationTypes = new Dictionary<Type, Type>();

        public virtual TextPosition Start { get; protected internal set; }
        public virtual TextPosition End { get; protected internal set; }
        public virtual CodeNode ParentNode { get; protected internal set; }
        public virtual CodeNodeCollection NamedNodes { get; private set; }

        public virtual IEnumerable<CodeNode> Nodes
        {
            get
            {
                return this.NamedNodes;
            }
        }

        public virtual ICodeScope OwningScope
        {
            get
            {
                return null;
            }
        }

        public virtual ICodeScope Scope
        {
            get
            {
                CodeNode parent = this.ParentNode;
                while (parent != null && parent.OwningScope != null)
                {
                    parent = parent.ParentNode;
                }
                return parent == null ? null : parent.OwningScope;
            }
        }

        public CodeNode()
        {
            this.NamedNodes = new CodeNodeCollection();
            this.NamedNodes.Owner = this;
        }

        public static T Create<T>()
            where T : CodeNode
        {
            if (!implementationTypes.Keys.Contains(typeof(T)))
            {
                CodeCompileUnit unit = new CodeCompileUnit();
                string namespaceString = "CodeBoxControl.CodeProvide.CodeNodeAutoImplementation.OfType" + typeof(T).FullName + "CodeNodeAutoNamespace";
                string typeString = typeof(T).Name + "Implementation";
                string fullNameString = namespaceString + "." + typeString;
                {
                    CodeNamespace codeNamespace = new CodeNamespace(namespaceString);
                    unit.Namespaces.Add(codeNamespace);

                    CodeTypeDeclaration classDeclaration = new CodeTypeDeclaration(typeString);
                    codeNamespace.Types.Add(classDeclaration);
                    classDeclaration.Attributes = MemberAttributes.Public;
                    classDeclaration.BaseTypes.Add(typeof(T));

                    foreach (PropertyInfo propertyInfo in typeof(T).GetProperties()
                        .Where(p => typeof(CodeNode).IsAssignableFrom(p.PropertyType))
                        .Where(p => p.GetGetMethod() != null && p.GetGetMethod().IsAbstract && p.GetSetMethod() != null && p.GetSetMethod().IsAbstract)
                        )
                    {
                        CodeMemberProperty property = new CodeMemberProperty();
                        classDeclaration.Members.Add(property);
                        property.Attributes = MemberAttributes.Public | MemberAttributes.Override;
                        property.Name = propertyInfo.Name;
                        property.Type = new CodeTypeReference(propertyInfo.PropertyType);

                        property.GetStatements.Add(
                            new CodeMethodReturnStatement(
                                new CodeCastExpression(
                                    propertyInfo.PropertyType,
                                    new CodeIndexerExpression(
                                        new CodePropertyReferenceExpression(
                                            new CodeThisReferenceExpression(),
                                            "NamedNodes"
                                            ),
                                        new CodePrimitiveExpression(propertyInfo.Name)
                                        )
                                    )
                                )
                            );

                        property.SetStatements.Add(
                            new CodeAssignStatement(
                                new CodeIndexerExpression(
                                    new CodePropertyReferenceExpression(
                                        new CodeThisReferenceExpression(),
                                        "NamedNodes"
                                        ),
                                        new CodePrimitiveExpression(propertyInfo.Name)
                                    ),
                                new CodePropertySetValueReferenceExpression()
                                )
                            );
                    }
                }
                Assembly assembly = CodeDomHelper.Compile(unit, typeof(CodeNode).Assembly.Location, typeof(T).Assembly.Location);
                implementationTypes[typeof(T)] = assembly.GetType(fullNameString);
            }
            return (T)implementationTypes[typeof(T)].GetConstructor(new Type[] { }).Invoke(new object[] { });
        }
    }
}
