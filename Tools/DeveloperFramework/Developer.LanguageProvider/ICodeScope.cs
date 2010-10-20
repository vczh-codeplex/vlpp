using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;

namespace Developer.LanguageProvider
{
    public interface ICodeScope
    {
        CodeNodeCollection ScopeNodes { get; }
        ICodeScope ParentScope { get; }
        CodeNode ScopeOwner { get; }
    }
}
