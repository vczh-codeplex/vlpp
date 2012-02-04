using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;

namespace _TranslateXMLtoCode
{
    enum RgacTypeKind
    {
        Class,
        ClassPointer,
        ClassSmartPointer,
        Struct,
        StructPointer,
        StructReference,
        ConstStructReference,
        Enum,
        Primitive,
        OtherGacType,
    }

    enum RgacPrimitiveKind
    {
        SInt,
        UInt,
        Char,
        String,
        ConstStringReference,
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

    enum RgacMethodKind
    {
        Abstract,
        Virtual,
        Normal,
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
        public RgacMethod[] Constructors { get; set; }
        public RgacMethod[] Methods { get; set; }
        public RgacMethod[] OverridingMethods { get; set; }
        public RgacMethod[] StaticMethods { get; set; }
        public RgacProperty[] Properties { get; set; }
        public RgacProperty[] StaticProperties { get; set; }

        public override string ToString()
        {
            return this.Name.Aggregate((a, b) => a + " :: " + b);
        }
    }

    class RgacMethod
    {
        public string Name { get; set; }
        public RgacMethodKind Kind { get; set; }
        public RgacType ReturnType { get; set; }
        public RgacType[] ParameterTypes { get; set; }
        public string[] ParameterNames { get; set; }
        public RgacUDT OwnerUDT { get; set; }
        public GacMethod OriginalGacMethod { get; set; }

        public List<RgacMethod> OverridingBaseMethods { get; private set; }
        public List<RgacMethod> OverridingDerivedMethods { get; private set; }

        public RgacMethod()
        {
            this.OverridingBaseMethods = new List<RgacMethod>();
            this.OverridingDerivedMethods = new List<RgacMethod>();
        }

        public override string ToString()
        {
            return this.Name;
        }
    }

    class RgacProperty
    {
        public string Name { get; set; }
        public RgacType PropertyType { get; set; }
        public RgacUDT OwnerUDT { get; set; }
        public RgacMethod Getter { get; set; }
        public RgacMethod Setter { get; set; }
        public GacField PublicGacFieldAccessor { get; set; }
        public bool IsEventField { get; set; }

        public override string ToString()
        {
            return this.Name + " {"
                + (Getter == null ? "" : "get; ")
                + (Setter == null ? "" : "set; ")
                + (PublicGacFieldAccessor == null ? "" : IsEventField ? "event; " : "field; ")
                + "}";
        }
    }
}
