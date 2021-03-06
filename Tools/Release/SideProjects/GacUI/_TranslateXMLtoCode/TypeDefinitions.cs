﻿using System;
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
        SmartPointer,
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

    enum GacMethodKind
    {
        Abstract,
        Virtual,
        Normal,
    }

    class GacUDT
    {
        public string Name { get; set; }
        public bool IsAbstract { get; set; }
        public GacUDTKind Kind { get; set; }
        public GacBaseClass[] BaseClasses { get; set; }
        public GacField[] Fields { get; set; }
        public GacField[] StaticFields { get; set; }
        public GacConst[] Constants { get; set; }
        public GacMethod[] Methods { get; set; }
        public GacMethod[] StaticMethods { get; set; }

        public override string ToString()
        {
            return this.Name;
        }
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

        public override string ToString()
        {
            return this.Name;
        }
    }

    class GacBaseClass
    {
        public GacAccess Access { get; set; }
        public GacUDT UDT { get; set; }
    }

    class GacField
    {
        public string Name { get; set; }
        public GacAccess Access { get; set; }
        public GacType Type { get; set; }
        public GacUDT OwnerUDT { get; set; }

        public override string ToString()
        {
            return this.Name;
        }
    }

    class GacConst : GacField
    {
        public int EnumItemValue { get; set; }

        public override string ToString()
        {
            return this.Name + " = " + this.EnumItemValue.ToString();
        }
    }

    class GacMethod
    {
        public string Name { get; set; }
        public GacMethodKind Kind { get; set; }
        public GacAccess Access { get; set; }
        public GacType Type { get; set; }
        public string[] ParameterNames { get; set; }
        public GacUDT OwnerUDT { get; set; }

        public override string ToString()
        {
            return this.Name;
        }
    }
}
