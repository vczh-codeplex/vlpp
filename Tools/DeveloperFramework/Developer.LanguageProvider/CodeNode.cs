using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.CodeDom.Compiler;
using Microsoft.CSharp;
using System.CodeDom;
using System.Reflection;

namespace Developer.LanguageProvider
{
    public abstract class CodeNode
    {
        protected const string PrefixUnit = "    ";

        private static Dictionary<Type, Type> implementationTypes = new Dictionary<Type, Type>();

        #region Interfaces

        public virtual TextPosition Start { get; set; }
        public virtual TextPosition End { get; set; }
        public virtual CodeNode ParentNode { get; protected internal set; }
        public virtual CodeNodeCollection NamedNodes { get; private set; }

        public virtual CodeScope OwningScope { get; private set; }
        public virtual CodeScope Scope { get; private set; }

        public virtual IEnumerable<CodeNode> Nodes
        {
            get
            {
                return this.NamedNodes;
            }
        }

        public CodeNode Root
        {
            get
            {
                CodeNode root = this;
                while (root.ParentNode != null)
                {
                    root = root.ParentNode;
                }
                return root;
            }
        }

        #endregion

        public CodeNode()
        {
            this.NamedNodes = new CodeNodeCollection();
            this.NamedNodes.Owner = this;
        }

        #region Scoping

        protected virtual bool ContainScope
        {
            get
            {
                return false;
            }
        }

        protected virtual void FillScope(CodeNodeCollection nodes)
        {
        }

        public void BuildScope(CodeScope scope)
        {
            if (scope != null)
            {
                this.Scope = scope;
            }
            else if (this.ParentNode != null)
            {
                this.Scope = this.ParentNode.OwningScope ?? this.ParentNode.Scope;
            }
            else
            {
                this.Scope = null;
            }
            if (this.ContainScope)
            {
                this.OwningScope = new CodeScope(this.Scope, this);
            }
            else
            {
                this.OwningScope = null;
            }
            if (this.Scope != null)
            {
                FillScope(this.Scope.ScopeNodes);
            }
            foreach (var node in this.Nodes)
            {
                node.BuildScope(null);
            }
        }

        #endregion

        #region Positioning

        public T FindParent<T>()
            where T : CodeNode
        {
            CodeNode node = this.ParentNode;
            while (node != null)
            {
                T result = node as T;
                if (result != null) return result;
                node = node.ParentNode;
            }
            return null;
        }

        public T FindDeepest<T>(TextPosition position, bool allowFirstPosition = true, bool allowLastPosition = true)
            where T : CodeNode
        {
            CodeNode node = this;
            while (true)
            {
                CodeNode subNode = node.Nodes
                    .Where(n =>
                        (n.Start < position || (allowFirstPosition && n.Start == position)) &&
                        (position < n.End || (allowLastPosition && position == n.End))
                        )
                    .FirstOrDefault();
                if (subNode == null)
                {
                    break;
                }
                else
                {
                    node = subNode;
                }
            }
            while (node != null)
            {
                T result = node as T;
                if (result != null)
                {
                    return result;
                }
                else
                {
                    node = node.ParentNode;
                }
            }
            return null;
        }

        #endregion

        #region ToString

        public virtual void ToString(StringBuilder sb, string prefix)
        {
            Type currentType = this.GetType();
            if (currentType.FullName.EndsWith("." + currentType.BaseType.Name + "Implementation"))
            {
                currentType = currentType.BaseType;
            }
            sb.AppendLine(currentType.Name);
            sb.AppendLine(prefix + "{");
            while (currentType != typeof(CodeNode))
            {
                foreach (PropertyInfo property in currentType.GetProperties().Where(p => p.DeclaringType == currentType))
                {
                    sb.Append(prefix + PrefixUnit + property.Name + " = ");
                    object value = property.GetValue(this, new object[] { });
                    if (value == null)
                    {
                        sb.AppendLine("null");
                    }
                    else if (typeof(CodeNode).IsAssignableFrom(value.GetType()))
                    {
                        (value as CodeNode).ToString(sb, prefix + PrefixUnit);
                    }
                    else
                    {
                        sb.AppendLine(value.ToString());
                    }
                }
                currentType = currentType.BaseType;
            }
            sb.AppendLine(prefix + "}");
        }

        public override string ToString()
        {
            StringBuilder sb = new StringBuilder();
            ToString(sb, "");
            return sb.ToString();
        }

        #endregion

        #region Implementation Class Generator

        private static CodeNamespace GenerateImplementation(Type type, out string namespaceString, out string typeString)
        {
            namespaceString = "CodeBoxControl.CodeProvide.CodeNodeAutoImplementation.OfType" + type.FullName + "CodeNodeAutoNamespace";
            typeString = type.Name + "Implementation";
            CodeNamespace codeNamespace = new CodeNamespace(namespaceString);
            CodeTypeDeclaration classDeclaration = new CodeTypeDeclaration(typeString);
            codeNamespace.Types.Add(classDeclaration);
            classDeclaration.Attributes = MemberAttributes.Public;
            classDeclaration.BaseTypes.Add(type);

            foreach (PropertyInfo propertyInfo in type.GetProperties()
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
            return codeNamespace;
        }

        public static void Preload(params Type[] types)
        {
            types = types.Where(t => !implementationTypes.Keys.Contains(t)).ToArray();
            if (types.Length > 0)
            {
                string[] names = new string[types.Length];
                CodeCompileUnit unit = new CodeCompileUnit();

                for (int i = 0; i < types.Length; i++)
                {
                    Type type = types[i];
                    string namespaceString, typeString;
                    unit.Namespaces.Add(GenerateImplementation(type, out namespaceString, out typeString));

                    string fullNameString = namespaceString + "." + typeString;
                    names[i] = fullNameString;
                }

                string[] assemblies = new string[] { typeof(CodeNode).Assembly.Location }.Concat(types.Select(t => t.Assembly.Location).Distinct()).ToArray();
                Assembly assembly = CodeDomHelper.Compile(unit, assemblies);
                for (int i = 0; i < types.Length; i++)
                {
                    implementationTypes[types[i]] = assembly.GetType(names[i]);
                }
            }
        }

        public static void Preload(Assembly assembly)
        {
            Preload(assembly.GetTypes().Where(t => typeof(CodeNode).IsAssignableFrom(t)).ToArray());
        }

        public static T Create<T>()
            where T : CodeNode
        {
            Preload(typeof(T));
            return (T)implementationTypes[typeof(T)].GetConstructor(new Type[] { }).Invoke(new object[] { });
        }

        #endregion
    }
}
