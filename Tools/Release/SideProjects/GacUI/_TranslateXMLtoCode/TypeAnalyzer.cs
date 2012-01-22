using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Xml.Linq;

namespace _TranslateXMLtoCode
{
    static class TypeAnalyzer
    {
        #region Loading

        static GacType DecorateType(GacType type, XElement typeElement)
        {
            if (typeElement.Attribute("const").Value == "true")
            {
                type = new GacType
                {
                    Name = type.Name + " const",
                    Kind = GacTypeKind.Const,
                    ElementType = type,
                };
            }
            if (typeElement.Attribute("volatile").Value == "true")
            {
                type = new GacType
                {
                    Name = type.Name + " volatile",
                    Kind = GacTypeKind.Volatile,
                    ElementType = type,
                };
            }
            return type;
        }

        static GacType TranslateTypeInternal(Dictionary<string, GacUDT> udts, XElement typeElement)
        {
            switch (typeElement.Name.LocalName)
            {
                case "primitive":
                    return new GacType
                    {
                        Name = typeElement.Attribute("name").Value,
                        Kind = GacTypeKind.Primitive,
                    };
                case "classType":
                case "enumType":
                    {
                        string name = typeElement.Attribute("name").Value;
                        if (name.StartsWith("vl::Ptr<"))
                        {
                            string newName = name.Substring(8, name.Length - 9);
                            return new GacType
                            {
                                Name = name,
                                Kind = GacTypeKind.SmartPointer,
                                ElementType = new GacType
                                {
                                    Name = newName,
                                    Kind = GacTypeKind.UDT,
                                    AssociatedUDT = udts[newName],
                                },
                            };
                        }
                        else
                        {
                            return new GacType
                            {
                                Name = name,
                                Kind = GacTypeKind.UDT,
                                AssociatedUDT = udts[name],
                            };
                        }
                    }
                case "pointer":
                    {
                        GacType elementType = TranslateType(udts, typeElement.Elements().First());
                        return new GacType
                        {
                            Name = elementType.Name + "*",
                            Kind = GacTypeKind.Pointer,
                            ElementType = elementType,
                        };
                    }
                case "reference":
                    {
                        GacType elementType = TranslateType(udts, typeElement.Elements().First());
                        return new GacType
                        {
                            Name = elementType.Name + "&",
                            Kind = GacTypeKind.Reference,
                            ElementType = elementType,
                        };
                    }
                case "rightValueReference":
                    {
                        GacType elementType = TranslateType(udts, typeElement.Elements().First());
                        return new GacType
                        {
                            Name = elementType.Name + "&&",
                            Kind = GacTypeKind.RValueReference,
                            ElementType = elementType,
                        };
                    }
                case "array":
                    {
                        GacType elementType = TranslateType(udts, typeElement.Element("element").Elements().First());
                        int arrayLength = int.Parse(typeElement.Element("count").Attribute("value").Value);
                        return new GacType
                        {
                            Name = elementType.Name + "[" + arrayLength.ToString() + "]",
                            Kind = GacTypeKind.Array,
                            ElementType = elementType,
                            ArrayLength = arrayLength,
                        };
                    }
                case "function":
                    {
                        GacType returnType = TranslateType(udts, typeElement.Element("return").Elements().First());
                        GacType[] parameterTypes = typeElement.Element("arguments").Elements("argument")
                            .Select(e => TranslateType(udts, e.Elements().First()))
                            .ToArray();
                        string callingConversion = typeElement.Element("callconv").Attribute("value").Value;
                        return new GacType
                        {
                            Name = returnType.Name
                                + "("
                                + callingConversion
                                + ")("
                                + parameterTypes.Select(t => t.Name).Aggregate("", (a, b) => a == "" ? b : a + ", " + b)
                                + ")",
                            Kind = GacTypeKind.Function,
                            ReturnType = returnType,
                            ParameterTypes = parameterTypes,
                            CallingConversion = callingConversion,
                        };
                    }
                default:
                    throw new ArgumentException();
            }
        }

        static GacType TranslateType(Dictionary<string, GacUDT> udts, XElement typeElement)
        {
            return DecorateType(TranslateTypeInternal(udts, typeElement), typeElement);
        }

        static GacAccess TranslateAccess(string value)
        {
            switch (value)
            {
                case "private": return GacAccess.Private;
                case "protected": return GacAccess.Protected;
                case "public": return GacAccess.Public;
                case "": return GacAccess.Public;
                default: throw new ArgumentException();
            }
        }

        static GacMethodKind TranslateVirtual(string value)
        {
            switch (value)
            {
                case "pure": return GacMethodKind.Abstract;
                case "virtual": return GacMethodKind.Virtual;
                case "normal": return GacMethodKind.Normal;
                case "": return GacMethodKind.Normal;
                default: throw new ArgumentException();
            }
        }

        static GacField TranslateField(Dictionary<string, GacUDT> udts, XElement fieldElement, GacUDT ownerUDT)
        {
            return new GacField
            {
                Name = fieldElement.Attribute("name").Value,
                Access = TranslateAccess(fieldElement.Attribute("access").Value),
                Type = TranslateType(udts, fieldElement.Element("type").Elements().First()),
                OwnerUDT = ownerUDT,
            };
        }

        static GacConst TranslateEnumField(Dictionary<string, GacUDT> udts, XElement fieldElement, GacUDT ownerUDT)
        {
            GacField field = TranslateField(udts, fieldElement, ownerUDT);
            return new GacConst
            {
                Name = field.Name,
                Access = field.Access,
                Type = field.Type,
                OwnerUDT = field.OwnerUDT,
                EnumItemValue = int.Parse(fieldElement.Element("intValue").Attribute("value").Value),
            };
        }

        static GacMethod TranslateMethod(Dictionary<string, GacUDT> udts, XElement methodElement, GacUDT ownerUDT)
        {
            return new GacMethod
            {
                Name = methodElement.Attribute("name").Value,
                Kind = TranslateVirtual(methodElement.Attribute("virtual").Value),
                Access = TranslateAccess(methodElement.Attribute("access").Value),
                Type = TranslateType(udts, methodElement.Element("type").Elements().First()),
                ParameterNames = methodElement.Element("arguments").Elements("argument").Select(e => e.Attribute("name").Value).ToArray(),
                OwnerUDT = ownerUDT,
            };
        }

        static void FillUdt(Dictionary<string, GacUDT> udts, GacUDT udt, XElement udtElement)
        {
            udt.BaseClasses = udtElement.Element("baseClasses") == null ? new GacUDT[0] : udtElement
                .Element("baseClasses")
                .Elements("baseClass")
                .Select(e => udts[e.Attribute("name").Value])
                .ToArray();
            udt.Fields = udtElement.Element("fields") == null ? new GacField[0] : udtElement
                .Element("fields")
                .Elements("field")
                .Select(e => TranslateField(udts, e, udt))
                .ToArray();
            udt.StaticFields = udtElement.Element("fields") == null ? new GacField[0] : udtElement
                .Element("fields")
                .Elements("staticField")
                .Select(e => TranslateField(udts, e, udt))
                .ToArray();
            udt.Constants = udtElement.Element("fields") == null ? new GacConst[0] : udtElement
                .Element("fields")
                .Elements("const")
                .Select(e => TranslateEnumField(udts, e, udt))
                .ToArray();
            udt.Methods = udtElement.Element("methods") == null ? new GacMethod[0] : udtElement
                .Element("methods")
                .Elements("method")
                .Select(e => TranslateMethod(udts, e, udt))
                .ToArray();
            udt.StaticMethods = udtElement.Element("methods") == null ? new GacMethod[0] : udtElement
                .Element("methods")
                .Elements("staticMethod")
                .Select(e => TranslateMethod(udts, e, udt))
                .ToArray();
            udt.IsAbstract = udt.Methods.Any(m => m.Kind == GacMethodKind.Abstract);
        }

        static Dictionary<string, GacUDT> LoadXML(XDocument document)
        {
            Dictionary<string, GacUDT> udts = new Dictionary<string, GacUDT>();
            foreach (var udtElement in document.Root.Elements().Where(e => e.Name != "functions"))
            {
                GacUDT udt = new GacUDT();
                udt.Name = udtElement.Attribute("name").Value;
                switch (udtElement.Name.LocalName)
                {
                    case "class":
                        udt.Kind = GacUDTKind.Class;
                        break;
                    case "enum":
                        udt.Kind = GacUDTKind.Enum;
                        break;
                }
                udts.Add(udt.Name, udt);
            }
            foreach (var udtElement in document.Root.Elements().Where(e => e.Name != "functions"))
            {
                FillUdt(udts, udts[udtElement.Attribute("name").Value], udtElement);
            }
            return udts;
        }

        #endregion

        #region Extracting

        static GacUDT[] ExtractDescriptableUDT(Dictionary<string, GacUDT> udts)
        {
            return udts.Values
                .Where(u => u.BaseClasses.Any(b => b.Name == "vl::presentation::Description<" + u.Name + ">"))
                .ToArray();
        }

        static bool IsUDTImportant(GacUDT udt)
        {
            return !udt.Name.StartsWith("vl::presentation::Description<")
                && !udt.Name.StartsWith("vl::Func<")
                && !udt.Name.StartsWith("vl::presentation::windows::")
                && udt.Name != "vl::presentation::DescriptableValue"
                && udt.Name != "vl::presentation::IType"
                && udt.Name != "vl::presentation::IMemberDescriptor"
                && udt.Name != "vl::presentation::IParameterDescriptor"
                && udt.Name != "vl::presentation::IMethodDescriptor"
                && udt.Name != "vl::presentation::IPropertyDescriptor"
                && udt.Name != "vl::presentation::ITypeDescriptor"
                && udt.Name != "vl::presentation::ITypeProvider"
                && udt.Name != "vl::Interface"
                && udt.Name != "vl::Object"
                ;
        }

        static GacUDT[] ExtractRelatedUDT(GacType type)
        {
            if (type == null) return new GacUDT[0];
            return ExtractRelatedUDT(type.ElementType).Concat(
                ExtractRelatedUDT(type.ReturnType).Concat(
                (type.ParameterTypes == null ? new GacUDT[0] : type.ParameterTypes.SelectMany(ExtractRelatedUDT)).Concat(
                type.AssociatedUDT == null ? new GacUDT[0] : new GacUDT[] { type.AssociatedUDT }
                ))).Distinct().ToArray();
        }

        static GacUDT[] ExtractRelatedUDT(GacUDT udt)
        {
            if (udt.Name.StartsWith("vl::presentation::"))
            {
                return udt.BaseClasses.Concat(
                    udt.Fields.Where(f => f.Access == GacAccess.Public).SelectMany(f => ExtractRelatedUDT(f.Type)).Concat(
                    udt.StaticFields.Where(f => f.Access == GacAccess.Public).SelectMany(f => ExtractRelatedUDT(f.Type)).Concat(
                    udt.Methods.Where(f => f.Access == GacAccess.Public).SelectMany(f => ExtractRelatedUDT(f.Type)).Concat(
                    udt.StaticMethods.Where(f => f.Access == GacAccess.Public).SelectMany(f => ExtractRelatedUDT(f.Type))
                    )))).Distinct().ToArray();
            }
            else
            {
                return new GacUDT[0];
            }
        }

        static bool InheritsFromObjectOrInterface(GacUDT udt)
        {
            return udt.BaseClasses.Any(t => t.Name == "vl::Object" || t.Name == "vl::Interface" || InheritsFromObjectOrInterface(t));
        }

        #endregion

        public static TypeAnalyzerResult Analyze(XDocument document)
        {
            var udts = LoadXML(document);
            var methods = document.Root.Element("functions").Elements("method")
                .Select(e => TranslateMethod(udts, e, null))
                .ToArray();

            var descriptableMethods = methods
                .Where(m =>
                    m.Name.StartsWith("vl::presentation::elements::Get") ||
                    m.Name.StartsWith("vl::presentation::controls::Get") ||
                    m.Name.StartsWith("vl::presentation::Get")
                    )
                .ToArray();
            var descriptableUdts = ExtractDescriptableUDT(udts);
            var relatedUdts = descriptableUdts.SelectMany(ExtractRelatedUDT)
                .Concat(descriptableMethods.SelectMany(m => ExtractRelatedUDT(m.Type)))
                .Distinct().ToArray();
            while (true)
            {
                var newRelatedUdts = relatedUdts.Concat(relatedUdts.SelectMany(ExtractRelatedUDT)).Distinct().ToArray();
                if (newRelatedUdts.Length == relatedUdts.Length) break;
                relatedUdts = newRelatedUdts;
            }
            var undescriptableUdts = relatedUdts.Except(descriptableUdts).ToArray();
            var importantUdts = undescriptableUdts.Where(IsUDTImportant).ToArray();
            var exportableUdts = importantUdts.Where(t => t.Name.StartsWith("vl::")).OrderBy(f => f.Name).ToArray();

            var result = new TypeAnalyzerResult();

            result.GlobalFunctions = descriptableMethods
                .OrderBy(f => f.Name
                ).ToArray();

            result.DescriptableUdts = descriptableUdts
                .OrderBy(f => f.Name)
                .ToArray();

            result.ExportableEnums = exportableUdts
                .Where(t => t.Kind == GacUDTKind.Enum)
                .ToArray();

            result.ExportableStructs = exportableUdts
                .Where(t => t.Kind == GacUDTKind.Class)
                .Where(t => !InheritsFromObjectOrInterface(t))
                .ToArray();

            result.ExportableClasses = exportableUdts
                .Where(t => t.Kind == GacUDTKind.Class)
                .Where(t => InheritsFromObjectOrInterface(t))
                .ToArray();

            result.AvailableUdts = result.DescriptableUdts
                .Concat(descriptableUdts.Concat(exportableUdts))
                .Distinct()
                .OrderBy(f => f.Name)
                .ToArray();

            return result;
        }
    }

    class TypeAnalyzerResult
    {
        // global functions to export
        public GacMethod[] GlobalFunctions { get; set; }

        // exportable objects that can retrive the real type (e.g. GuiControl)
        public GacUDT[] DescriptableUdts { get; set; }

        // exportable objects that can only retrive the declarated type (e.g. Object)
        public GacUDT[] ExportableEnums { get; set; }
        public GacUDT[] ExportableStructs { get; set; }
        public GacUDT[] ExportableClasses { get; set; }

        // all acceptable types
        public GacUDT[] AvailableUdts { get; set; }
    };
}
