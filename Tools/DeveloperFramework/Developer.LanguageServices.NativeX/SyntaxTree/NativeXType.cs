using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using Developer.LanguageProvider;

namespace Developer.LanguageServices.NativeX.SyntaxTree
{
    public abstract class NativeXType : NativeXNode
    {
    }

    public abstract class NativeXPointerType : NativeXType
    {
        public abstract NativeXType ElementType { get; set; }
    }

    public abstract class NativeXArrayType : NativeXType
    {
        public abstract NativeXType ElementType { get; set; }
        public abstract NativeXPrimitiveExpression Size { get; set; }
    }

    public abstract class NativeXReferenceType : NativeXType
    {
        public string ReferencedName { get; set; }
    }

    public abstract class NativeXFunctionType : NativeXType
    {
        public abstract NativeXType ReturnType { get; set; }
        public abstract CodeNodeList<NativeXType> Parameters { get; set; }
    }

    public abstract class NativeXInstanciatedType : NativeXType
    {
        public abstract NativeXReferenceType ElementType { get; set; }
        public abstract CodeNodeList<NativeXType> GenericArguments { get; set; }
    }
}
