using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;

namespace CodeBoxControl.CodeProvider
{
    public interface ICodeScope
    {
        CodeNodeCollection ScopeNodes { get; }
        ICodeScope ParentScope { get; }
        CodeNode ScopeOwner { get; }
    }
}
