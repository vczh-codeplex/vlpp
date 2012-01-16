using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Xml.Linq;

namespace _TranslateXMLtoCode
{
    class Program
    {
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
                    return new GacType
                    {
                        Name = typeElement.Attribute("name").Value,
                        Kind = GacTypeKind.UDT,
                        AssociatedUDT = udts[typeElement.Attribute("name").Value],
                    };
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
                default: throw new ArgumentException();
            }
        }

        static GacField TranslateField(Dictionary<string, GacUDT> udts, XElement fieldElement)
        {
            return new GacField
            {
                Name = fieldElement.Attribute("name").Value,
                Access = TranslateAccess(fieldElement.Attribute("access").Value),
                Type = TranslateType(udts, fieldElement.Element("type").Elements().First()),
            };
        }

        static GacMethod TranslateMethod(Dictionary<string, GacUDT> udts, XElement fieldElement)
        {
            return new GacMethod
            {
                Name = fieldElement.Attribute("name").Value,
                Access = TranslateAccess(fieldElement.Attribute("access").Value),
                Type = TranslateType(udts, fieldElement.Element("type").Elements().First()),
                ParameterNames = fieldElement.Element("arguments").Elements("argument").Select(e => e.Attribute("name").Value).ToArray(),
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
                .Select(e => TranslateField(udts, e))
                .ToArray();
            udt.StaticFields = udtElement.Element("fields") == null ? new GacField[0] : udtElement
                .Element("fields")
                .Elements("staticField")
                .Select(e => TranslateField(udts, e))
                .ToArray();
            udt.Methods = udtElement.Element("methods") == null ? new GacMethod[0] : udtElement
                .Element("methods")
                .Elements("method")
                .Select(e => TranslateMethod(udts, e))
                .ToArray();
            udt.StaticMethods = udtElement.Element("methods") == null ? new GacMethod[0] : udtElement
                .Element("methods")
                .Elements("staticMethod")
                .Select(e => TranslateMethod(udts, e))
                .ToArray();
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

        static void Main(string[] args)
        {
            string xmlPath = args[0] + @"\_GenPDB.xml";
            Console.WriteLine("analysing {0}", xmlPath);
            var udts = LoadXML(XDocument.Load(xmlPath));
        }
    }
}
