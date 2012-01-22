﻿using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;

namespace _TranslateXMLtoCode
{
    enum RgacTypeKind
    {
        RgacUDTPointer,
        RgacUDTSmartPointer,
        Primitive,
        OtherGacType,
    }

    enum RgacPrimitiveKind
    {
        SInt,
        UInt,
        Char,
        String,
        Bool,
        Float,
        Double,
        Void,
    }

    enum RgacUDTKind
    {
        Struct,
        Class,
        Interface,
        Enum,
    }

    class RgacType
    {
        public RgacTypeKind Kind { get; set; }
        public RgacPrimitiveKind PrimitiveKind { get; set; }
        public RgacUDT AssociatedRgacType { get; set; }
        public GacType OriginalGacType { get; set; }

        public override string ToString()
        {
            return this.OriginalGacType.ToString();
        }
    }

    class RgacUDT
    {
        public string[] Name { get; set; }
        public CppName CppName { get; set; }
        public RgacUDTKind Kind { get; set; }
        public bool IsAbstract { get; set; }
        public GacUDT AssociatedGacType { get; set; }
        public bool Descriptable { get; set; }
        public RgacUDT[] BaseClasses { get; set; }
        public RgacMethod[] Methods { get; set; }
        public RgacMethod[] StaticMethods { get; set; }

        public override string ToString()
        {
            return this.Name.Aggregate((a, b) => a + " :: " + b);
        }
    }

    class RgacMethod
    {
        public string Name { get; set; }
        public RgacType ReturnType { get; set; }
        public RgacType[] ParameterTypes { get; set; }
        public string[] ParameterNames { get; set; }
        public RgacUDT OwnerUDT { get; set; }
        public GacMethod OriginalGacMethod { get; set; }

        public override string ToString()
        {
            return this.Name;
        }
    }
}
