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
            : base(path)
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
                            return element + "*";
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
                                return "const " + element + "&";
                            }
                        }
                        else
                        {
                            string element = GetType(type.ElementType);
                            if (element != null)
                            {
                                return element + "&";
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
                            case RgacPrimitiveKind.String:
                                return "GacString";
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
                    return "const " + GacUdtTypeName(type.AssociatedRgacType) + "&";

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

        #region UDT Sorter

        protected HashSet<string> PredeclaredClasses = new HashSet<string>
        {
            "GuiGraphicsComposition",
            "GuiControl",
            "GuiControlHost",
            "GuiComponent",
            "IGuiMenuService",
            "IGuiGraphicsElement",
            "IGuiGraphicsRenderer",
            "GuiTableComposition",
            "GuiTab",

            "INodeProvider",
            "INodeRootProvider",
            "INodeItemStyleProvider",
            "MemoryNodeProvider",

            "INativeImage",
            "INativeImageFrame",
            "INativeWindow",
        };

        protected IEnumerable<RgacUDT> GetRelatedUdts(RgacType type)
        {
            if (type.AssociatedRgacType != null)
            {
                yield return type.AssociatedRgacType;
            }
        }

        protected IEnumerable<RgacUDT> GetRelatedUdts(RgacMethod method)
        {
            return GetRelatedUdts(method.ReturnType)
                .Concat(method.ParameterTypes.SelectMany(GetRelatedUdts));
        }

        protected IEnumerable<RgacUDT> GetRelatedUdts(RgacProperty property)
        {
            return GetRelatedUdts(property.PropertyType);
        }

        protected RgacUDT[] GetRelatedUdts(RgacUDT udt)
        {
            return
                udt.BaseClasses.Concat(
                udt.Constructors.SelectMany(GetRelatedUdts).Concat(
                udt.Methods.SelectMany(GetRelatedUdts).Concat(
                udt.StaticMethods.SelectMany(GetRelatedUdts).Concat(
                udt.Properties.SelectMany(GetRelatedUdts).Concat(
                udt.StaticProperties.SelectMany(GetRelatedUdts)
                )))))
                .Distinct()
                .Where(t => this.options.Udts.Contains(t))
                .Where(t => !this.PredeclaredClasses.Contains(t.ToString()))
                .ToArray();
        }

        protected IEnumerable<RgacUDT> GetSortedUdts()
        {
            List<Tuple<RgacUDT[], HashSet<RgacUDT>>> udts = this.options.Udts
                .GroupBy(t => t.Name[0])
                .Select(g => g.ToArray())
                .Select(g => Tuple.Create(g, new HashSet<RgacUDT>(g.SelectMany(GetRelatedUdts).Distinct().Except(g))))
                .ToList();

            while (udts.Count > 0)
            {
                bool found = false;
                for (int i = 0; i < udts.Count; i++)
                {
                    if (udts[i].Item2.Count == 0)
                    {
                        var udtg = udts[i].Item1;
                        udts.RemoveAt(i);
                        foreach (var t in udts)
                        {
                            foreach (var udt in udtg)
                            {
                                t.Item2.Remove(udt);
                            }
                        }
                        foreach (var udt in udtg)
                            yield return udt;
                        found = true;
                    }
                }
                if (!found) break;
            }
            if (udts.Count > 0)
            {
                var needs = udts.SelectMany(t => t.Item2)
                    .GroupBy(t => t)
                    .Select(g => g.ToArray())
                    .OrderByDescending(g => g.Length)
                    .ToArray();
                throw new ArgumentException();
            }
        }

        #endregion
    }
}
