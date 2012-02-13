using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;

namespace _TranslateXMLtoCode.Codegen
{
    abstract class Codegen_GacUI_base : CodeGenerator
    {
        protected CodeGeneratorOptions options;

        protected Codegen_GacUI_base(CodeGeneratorOptions options, string path)
            :base(path)
        {
            this.options = options;
        }

        #region GenerateType

        protected string GacUdtTypeName(RgacUDT udt)
        {
            return udt.Name
                .Aggregate((a, b) => a + " :: " + b);
        }

        protected string GetType(GacType type)
        {
            switch (type.Kind)
            {
                case GacTypeKind.Primitive:
                    {
                        return type.Name;
                    }
                case GacTypeKind.Pointer:
                    {
                        string element = GetType(type.ElementType);
                        if (element != null)
                        {
                            return element + "->GetPointerType()";
                        }
                    }
                    break;
                case GacTypeKind.Reference:
                    {
                        if (type.ElementType.Kind == GacTypeKind.Const)
                        {
                            string element = GetType(type.ElementType);
                            if (element != null)
                            {
                                return element + "->GetConstReferenceType()";
                            }
                        }
                        else
                        {
                            string element = GetType(type.ElementType);
                            if (element != null)
                            {
                                return element + "->GetReferenceType()";
                            }
                        }
                    }
                    break;
                case GacTypeKind.Const:
                    {
                        return GetType(type.ElementType);
                    }
                case GacTypeKind.UDT:
                    {
                        RgacUDT elementType = options.Udts.Where(t => t.AssociatedGacType == type.AssociatedUDT).FirstOrDefault();
                        if (elementType != null)
                        {
                            return GacUdtTypeName(elementType);
                        }
                    }
                    break;
            }
            return null;
        }

        protected string GetType(RgacType type)
        {
            switch (type.Kind)
            {
                case RgacTypeKind.Primitive:
                    {
                        switch (type.PrimitiveKind)
                        {
                            case RgacPrimitiveKind.StringReference:
                                return "GacString&";
                            case RgacPrimitiveKind.ConstStringReference:
                                return "const GacString&";
                            default:
                                return type.OriginalGacType.Name;
                        }
                    }

                case RgacTypeKind.Class:
                    return GacUdtTypeName(type.AssociatedRgacType);
                case RgacTypeKind.ClassPointer:
                    return "rptr<" + GacUdtTypeName(type.AssociatedRgacType) + ">";
                case RgacTypeKind.ClassSmartPointer:
                    return "sptr<" + GacUdtTypeName(type.AssociatedRgacType) + ">";
                case RgacTypeKind.ClassReference:
                    return "rptr<" + GacUdtTypeName(type.AssociatedRgacType) + ">";
                case RgacTypeKind.ConstClassReference:
                    return "rptr<" + GacUdtTypeName(type.AssociatedRgacType) + ">";

                case RgacTypeKind.Struct:
                    return GacUdtTypeName(type.AssociatedRgacType);
                case RgacTypeKind.StructPointer:
                    return "rptr<" + GacUdtTypeName(type.AssociatedRgacType) + ">";
                case RgacTypeKind.StructSmartPointer:
                    return "sptr<" + GacUdtTypeName(type.AssociatedRgacType) + ">";
                case RgacTypeKind.StructReference:
                    return GacUdtTypeName(type.AssociatedRgacType) + "&";
                case RgacTypeKind.ConstStructReference:
                    return "const" + GacUdtTypeName(type.AssociatedRgacType) + "&";

                case RgacTypeKind.Enum:
                    return GacUdtTypeName(type.AssociatedRgacType);
                case RgacTypeKind.OtherGacType:
                    {
                        string result = GetType(type.OriginalGacType);
                        if (result != null) return result;
                    }
                    break;
            }
            return "0 /*UNKNOWN_TYPE[ " + type.ToString() + " ]*/";
        }

        #endregion
    }
}
