using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;

namespace Developer.LanguageProvider
{
    public class CodeScope
    {
        public CodeNodeCollection ScopeNodes { get; private set; }
        public CodeScope ParentScope { get; private set; }
        public CodeNode ScopeOwner { get; private set; }
        public CodeScope[] ExtraScopes { get; set; }

        internal CodeScope(CodeScope parentScope, CodeNode scopeOwner)
        {
            this.ParentScope = parentScope;
            this.ScopeOwner = scopeOwner;
            this.ScopeNodes = new CodeNodeCollection();
        }

        public CodeNode Find(string index)
        {
            CodeScope scope = this;
            while (scope != null)
            {
                CodeNode node = scope.ScopeNodes[index];
                if (node == null && scope.ExtraScopes != null)
                {
                    node = scope.ExtraScopes.Select(s => s.Find(index)).Where(n => n != null).FirstOrDefault();
                }
                if (node == null)
                {
                    scope = scope.ParentScope;
                }
                else
                {
                    return node;
                }
            }
            return null;
        }

        public List<CodeNode> FindAllDistinct()
        {
            List<CodeNode> nodes = new List<CodeNode>();
            HashSet<string> names = new HashSet<string>();
            CodeScope scope = this;
            while (scope != null)
            {
                foreach (CodeScope currentScope in new CodeScope[] { scope }.Concat(scope.ExtraScopes == null ? new CodeScope[] { } : scope.ExtraScopes))
                {
                    foreach (string key in currentScope.ScopeNodes.Keys)
                    {
                        if (!names.Contains(key))
                        {
                            names.Add(key);
                            nodes.Add(currentScope.ScopeNodes[key]);
                        }
                    }
                }
                scope = scope.ParentScope;
            }
            return nodes;
        }
    }
}
