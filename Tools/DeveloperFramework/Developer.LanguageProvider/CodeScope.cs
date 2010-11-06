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
    }
}
