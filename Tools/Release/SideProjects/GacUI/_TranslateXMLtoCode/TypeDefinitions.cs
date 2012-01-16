using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;

namespace _TranslateXMLtoCode
{
    enum GacUDTKind
    {
        Class,
        Enum,
    }

    enum GacTypeKind
    {
        Primitive,
        Pointer,
        Reference,
        RValueReference,
        Array,
        Function,
        Const,
        Volatile,
        UDT,
    }

    enum GacAccess
    {
        Private,
        Protected,
        Public,
    }

    class GacUDT
    {
        public string Name { get; set; }
        public GacUDTKind Kind { get; set; }
        public GacUDT[] BaseClasses { get; set; }
        public GacField[] Fields { get; set; }
        public GacField[] StaticFields { get; set; }
        public GacMethod[] Methods { get; set; }
        public GacMethod[] StaticMethods { get; set; }
    }

    class GacType
    {
        public string Name { get; set; }
        public GacTypeKind Kind { get; set; }
        public GacType ElementType { get; set; }
        public int ArrayLength { get; set; }
        public GacType ReturnType { get; set; }
        public GacType[] ParameterTypes { get; set; }
        public string CallingConversion { get; set; }
        public GacUDT AssociatedUDT { get; set; }
    }

    class GacField
    {
        public string Name { get; set; }
        public GacAccess Access { get; set; }
        public GacType Type { get; set; }
    }

    class GacMethod
    {
        public string Name { get; set; }
        public GacAccess Access { get; set; }
        public GacType Type { get; set; }
        public string[] ParameterNames { get; set; }
    }
}
