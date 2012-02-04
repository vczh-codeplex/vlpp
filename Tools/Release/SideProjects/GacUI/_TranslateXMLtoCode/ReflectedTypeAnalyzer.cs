using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Xml.Linq;

namespace _TranslateXMLtoCode
{
    static class ReflectedTypeAnalyzer
    {
        #region Name Generating

        static IEnumerable<string> GetSuggestedName(CppName cppName)
        {
            while (cppName != null)
            {
                switch (cppName.Name)
                {
                    case "int":
                    case "wchar_t":
                        yield return cppName.Name;
                        break;
                    default:
                        if (char.IsUpper(cppName.Name[0]))
                        {
                            if (cppName.Parameters.Length > 0)
                            {
                                string parameterName = GetSuggestedName(cppName.Parameters[0])
                                        .Select(n => "_" + n)
                                        .Aggregate((a, b) => a + b)
                                        ;
                                if (cppName.Name == "Ptr")
                                {
                                    yield return parameterName.Substring(1);
                                }
                                else
                                {
                                    yield return cppName.Name + "_of" + parameterName;
                                }
                            }
                            else
                            {
                                yield return cppName.Name;
                            }
                        }
                        break;
                }
                cppName = cppName.Member;
            }
        }

        static RgacUDT CreateUDT(ReflectedTypeAnalyzerInput input, GacUDT udt)
        {
            RgacUDT rudt = new RgacUDT();
            rudt.CppName = CppName.Parse(udt.Name);
            rudt.Name = GetSuggestedName(rudt.CppName).ToArray();
            rudt.IsAbstract = udt.IsAbstract;
            rudt.AssociatedGacType = udt;
            rudt.Descriptable = false;
            if (input.ExportableEnums.ContainsKey(udt.Name))
            {
                rudt.Kind = RgacUDTKind.Enum;
            }
            else if (input.ExportableStructs.ContainsKey(udt.Name))
            {
                rudt.Kind = RgacUDTKind.Struct;
            }
            else
            {
                rudt.Descriptable = input.DescriptableUdts.ContainsKey(udt.Name);
                rudt.Kind = rudt.Name.Last()[0] == 'I' ? RgacUDTKind.Interface : RgacUDTKind.Class;
            }
            return rudt;
        }

        #endregion

        #region FillUDT

        static RgacUDT GetRgacUDT(Dictionary<string, RgacUDT> udts, GacUDT udt)
        {
            RgacUDT result = null;
            udts.TryGetValue(udt.Name, out result);
            return result;
        }

        static bool IsTypeAcceptable(ReflectedTypeAnalyzerInput input, GacType type)
        {
            if (type == null) return true;
            if (type.Kind == GacTypeKind.Function) return false;
            if (type.AssociatedUDT != null)
            {
                bool result = input.AvailableUdts.ContainsKey(type.AssociatedUDT.Name);
                return result;
            }
            if (!IsTypeAcceptable(input, type.ElementType)) return false;
            if (!IsTypeAcceptable(input, type.ReturnType)) return false;
            if (type.ParameterTypes != null)
            {
                foreach (var parameterType in type.ParameterTypes)
                {
                    if (!IsTypeAcceptable(input, parameterType)) return false;
                }
            }
            return true;
        }

        static RgacType TranslateType(ReflectedTypeAnalyzerInput input, Dictionary<string, RgacUDT> udts, GacType type)
        {
            switch (type.Kind)
            {
                case GacTypeKind.Primitive:
                    switch (type.Name)
                    {
                        case "int":
                        case "signed __int8":
                        case "signed __int16":
                        case "signed __int32":
                            return new RgacType
                            {
                                Kind = RgacTypeKind.Primitive,
                                PrimitiveKind = RgacPrimitiveKind.SInt,
                                OriginalGacType = type,
                            };
                        case "unsigned __int8":
                        case "unsigned __int16":
                        case "unsigned __int32":
                            return new RgacType
                            {
                                Kind = RgacTypeKind.Primitive,
                                PrimitiveKind = RgacPrimitiveKind.UInt,
                                OriginalGacType = type,
                            };
                        case "bool":
                            return new RgacType
                            {
                                Kind = RgacTypeKind.Primitive,
                                PrimitiveKind = RgacPrimitiveKind.Bool,
                                OriginalGacType = type,
                            };
                        case "wchar_t":
                            return new RgacType
                            {
                                Kind = RgacTypeKind.Primitive,
                                PrimitiveKind = RgacPrimitiveKind.Char,
                                OriginalGacType = type,
                            };
                        case "float":
                            return new RgacType
                            {
                                Kind = RgacTypeKind.Primitive,
                                PrimitiveKind = RgacPrimitiveKind.Float,
                                OriginalGacType = type,
                            };
                        case "double":
                            return new RgacType
                            {
                                Kind = RgacTypeKind.Primitive,
                                PrimitiveKind = RgacPrimitiveKind.Double,
                                OriginalGacType = type,
                            };
                        case "void":
                            return new RgacType
                            {
                                Kind = RgacTypeKind.Primitive,
                                PrimitiveKind = RgacPrimitiveKind.Void,
                                OriginalGacType = type,
                            };
                        default:
                            throw new ArgumentException();
                    }
                case GacTypeKind.Pointer:
                    if (type.ElementType.AssociatedUDT != null)
                    {
                        if (input.DescriptableUdts.ContainsKey(type.ElementType.AssociatedUDT.Name)
                            || input.ExportableClasses.ContainsKey(type.ElementType.AssociatedUDT.Name))
                        {
                            return new RgacType
                            {
                                Kind = RgacTypeKind.ClassPointer,
                                AssociatedRgacType = udts[type.ElementType.AssociatedUDT.Name],
                                OriginalGacType = type,
                            };
                        }
                        else if (input.ExportableStructs.ContainsKey(type.ElementType.AssociatedUDT.Name))
                        {
                            return new RgacType
                            {
                                Kind = RgacTypeKind.StructPointer,
                                AssociatedRgacType = udts[type.ElementType.AssociatedUDT.Name],
                                OriginalGacType = type,
                            };
                        }
                    }
                    break;
                case GacTypeKind.SmartPointer:
                    if (type.ElementType.AssociatedUDT != null)
                    {
                        if (input.DescriptableUdts.ContainsKey(type.ElementType.AssociatedUDT.Name)
                            || input.ExportableClasses.ContainsKey(type.ElementType.AssociatedUDT.Name))
                        {
                            return new RgacType
                            {
                                Kind = RgacTypeKind.ClassSmartPointer,
                                AssociatedRgacType = udts[type.ElementType.AssociatedUDT.Name],
                                OriginalGacType = type,
                            };
                        }
                    }
                    break;
                case GacTypeKind.Reference:
                    if (type.ElementType.AssociatedUDT != null)
                    {
                        if (input.ExportableStructs.ContainsKey(type.ElementType.AssociatedUDT.Name))
                        {
                            return new RgacType
                            {
                                Kind = RgacTypeKind.StructReference,
                                AssociatedRgacType = udts[type.ElementType.AssociatedUDT.Name],
                                OriginalGacType = type,
                            };
                        }
                    }
                    else if (type.ElementType.Kind == GacTypeKind.Const)
                    {
                        if (type.ElementType.ElementType.AssociatedUDT != null)
                        {
                            if (type.ElementType.ElementType.AssociatedUDT.Name == "vl::ObjectString<wchar_t>")
                            {
                                return new RgacType
                                {
                                    Kind = RgacTypeKind.Primitive,
                                    PrimitiveKind = RgacPrimitiveKind.ConstStringReference,
                                    OriginalGacType = type,
                                };
                            }
                            else if (input.ExportableStructs.ContainsKey(type.ElementType.ElementType.AssociatedUDT.Name))
                            {
                                return new RgacType
                                {
                                    Kind = RgacTypeKind.ConstStructReference,
                                    AssociatedRgacType = udts[type.ElementType.ElementType.AssociatedUDT.Name],
                                    OriginalGacType = type,
                                };
                            }
                        }
                    }
                    break;
                case GacTypeKind.UDT:
                    if (type.AssociatedUDT != null)
                    {
                        if (type.AssociatedUDT.Name == "vl::ObjectString<wchar_t>")
                        {
                            return new RgacType
                            {
                                Kind = RgacTypeKind.Primitive,
                                PrimitiveKind = RgacPrimitiveKind.String,
                                OriginalGacType = type,
                            };
                        }
                        else if (input.ExportableClasses.ContainsKey(type.AssociatedUDT.Name))
                        {
                            return new RgacType
                            {
                                Kind = RgacTypeKind.Class,
                                AssociatedRgacType = udts[type.AssociatedUDT.Name],
                                OriginalGacType = type,
                            };
                        }
                        else if (input.ExportableStructs.ContainsKey(type.AssociatedUDT.Name))
                        {
                            return new RgacType
                            {
                                Kind = RgacTypeKind.Struct,
                                AssociatedRgacType = udts[type.AssociatedUDT.Name],
                                OriginalGacType = type,
                            };
                        }
                        else if (input.ExportableEnums.ContainsKey(type.AssociatedUDT.Name))
                        {
                            return new RgacType
                            {
                                Kind = RgacTypeKind.Enum,
                                AssociatedRgacType = udts[type.AssociatedUDT.Name],
                                OriginalGacType = type,
                            };
                        }
                    }
                    break;
            }
            if (IsTypeAcceptable(input, type))
            {
                return new RgacType
                {
                    Kind = RgacTypeKind.OtherGacType,
                    OriginalGacType = type,
                };
            }
            throw new ArgumentException();
        }

        static RgacMethod TranslateMethod(ReflectedTypeAnalyzerInput input, Dictionary<string, RgacUDT> udts, GacMethod method)
        {
            try
            {
                RgacMethod result = new RgacMethod()
                {
                    Name = method.Name,
                    Kind =
                        method.Kind == GacMethodKind.Abstract ? RgacMethodKind.Abstract :
                        method.Kind == GacMethodKind.Virtual ? RgacMethodKind.Virtual :
                        RgacMethodKind.Normal,
                    ReturnType = TranslateType(input, udts, method.Type.ReturnType),
                    ParameterTypes = method.Type.ParameterTypes.Select(t => TranslateType(input, udts, t)).ToArray(),
                    ParameterNames = method.ParameterNames,
                    OwnerUDT = udts[method.OwnerUDT.Name],
                    OriginalGacMethod = method,
                };
                return result;
            }
            catch (ArgumentException)
            {
                return null;
            }
        }

        static void FillUDT(ReflectedTypeAnalyzerInput input, Dictionary<string, RgacUDT> udts, RgacUDT udt)
        {
            udt.BaseClasses = udt.AssociatedGacType.BaseClasses
                .Select(t => GetRgacUDT(udts, t))
                .Where(t => t != null)
                .ToArray();

            var instanceMethods = udt.AssociatedGacType.Methods
                .Select(m => TranslateMethod(input, udts, m))
                .Where(m => m != null)
                .ToArray();

            var staticMethods = udt.AssociatedGacType.StaticMethods
                .Select(m => TranslateMethod(input, udts, m))
                .Where(m => m != null)
                .ToArray();

            udt.Methods = instanceMethods;
            udt.StaticMethods = staticMethods;
        }

        #endregion

        #region ProcessOverriding

        static RgacUDT[] GetBaseClasses(RgacUDT udt)
        {
            return udt.BaseClasses.Concat(udt.BaseClasses.SelectMany(GetBaseClasses)).Distinct().ToArray();
        }

        static string GetMethodRealName(string name)
        {
            CppName cppName = CppName.Parse(name);
            while (cppName.Member != null)
            {
                cppName = cppName.Member;
            }
            if (cppName.Parameters.Length > 0)
            {
                throw new ArgumentException();
            }
            return cppName.Name;
        }

        static void ProcessOverriding(ReflectedTypeAnalyzerInput input, Dictionary<string, RgacUDT> udts, RgacUDT udt)
        {
            var bases = GetBaseClasses(udt);
            foreach (var method in udt.Methods.Where(m => m.Kind == RgacMethodKind.Virtual))
            {
                if (method.Name.IndexOf('~') == -1)
                {
                    var methods = bases
                        .SelectMany(t => t.Methods)
                        .Where(m => (m.Kind == RgacMethodKind.Virtual || method.Kind == RgacMethodKind.Abstract)
                            && m.Name.IndexOf('~') == -1
                            && GetMethodRealName(m.Name) == GetMethodRealName(method.Name))
                        .Where(m => m.ReturnType.ToString() == method.ReturnType.ToString())
                        .Where(m => m.ParameterTypes.Length == method.ParameterTypes.Length)
                        .Where(m => m.ParameterTypes.Select(t => t.ToString()).Aggregate("", (a, b) => a + ";" + b)
                            == method.ParameterTypes.Select(t => t.ToString()).Aggregate("", (a, b) => a + ";" + b)
                            )
                        .Distinct()
                        .ToArray();

                    while (methods.Any(m => m.OverridingBaseMethods.Count > 0))
                    {
                        methods = methods.SelectMany(m =>
                            m.OverridingBaseMethods.Count > 0
                            ? m.OverridingBaseMethods.ToArray()
                            : new RgacMethod[] { m }
                            )
                            .Distinct()
                            .ToArray();
                    }

                    if (methods.Length > 0)
                    {
                        method.OverridingBaseMethods.AddRange(methods);

                        foreach (var derived in method.OverridingDerivedMethods)
                        {
                            derived.OverridingBaseMethods.Remove(method);
                            var baseMethods = derived.OverridingBaseMethods.Concat(methods).Distinct().ToArray();
                            derived.OverridingDerivedMethods.Clear();
                            derived.OverridingDerivedMethods.AddRange(baseMethods);
                        }
                    }
                }
            }
        }

        #endregion

        #region CategorizeMethods

        static void CategorizeMethods(
            ReflectedTypeAnalyzerInput input,
            Dictionary<string, RgacUDT> udts,
            RgacUDT udt,
            RgacMethod[] inputMethods,
            GacField[] inputFields,
            out RgacMethod[] outputConstructors,
            out RgacMethod[] outputMethods,
            out RgacMethod[] outputOverridingMethods,
            out RgacProperty[] outputProperties)
        {
            List<RgacMethod> constructors = new List<RgacMethod>();
            List<RgacMethod> methods = new List<RgacMethod>();
            List<RgacMethod> overridingMethods = new List<RgacMethod>();
            List<RgacProperty> properties = new List<RgacProperty>();
            if (inputMethods.Length > 0)
            {
                Dictionary<string, RgacMethod> getters = new Dictionary<string, RgacMethod>();
                Dictionary<string, RgacMethod> setters = new Dictionary<string, RgacMethod>();

                string destructorName = "";
                string constructorName = "";
                string ignoreName = "";
                {
                    var destructor = inputMethods.Where(m => m.Name.Contains('~')).FirstOrDefault();
                    if (destructor != null)
                    {
                        destructorName = destructor.Name;
                        constructorName = destructor.Name.Remove(destructorName.IndexOf('~'), 1);
                        ignoreName = destructorName.Substring(destructorName.IndexOf('~') + 1);
                    }
                    else
                    {
                        foreach (var m in inputMethods)
                        {
                            CppName cppName = CppName.Parse(m.Name);
                            if (cppName.Member != null)
                            {
                                CppName className = cppName;
                                CppName methodName = cppName.Member;
                                while (methodName.Member != null)
                                {
                                    className = methodName;
                                    methodName = methodName.Member;
                                }
                                if (className.Name == methodName.Name)
                                {
                                    constructorName = m.Name;
                                    ignoreName = className.Name;
                                    break;
                                }
                            }
                        }
                    }
                }

                foreach (var m in inputMethods)
                {
                    if (m.Name != ignoreName && !m.Name.StartsWith("_") && m.OriginalGacMethod.Access == GacAccess.Public)
                    {
                        CppName cppName = CppName.Parse(m.Name);
                        CppName lastName = cppName;
                        while (lastName.Member != null)
                        {
                            lastName = lastName.Member;
                        }
                        string oldName = m.Name;
                        m.Name = lastName.Name;

                        bool processed = false;
                        if (oldName == constructorName)
                        {
                            processed = true;
                            m.ReturnType = new RgacType
                            {
                                AssociatedRgacType = udt,
                                Kind = input.DescriptableUdts.ContainsKey(udt.AssociatedGacType.Name)
                                   || input.ExportableClasses.ContainsKey(udt.AssociatedGacType.Name)
                                   ? RgacTypeKind.ClassPointer
                                   : RgacTypeKind.Struct
                            };
                            constructors.Add(m);
                        }
                        else if (oldName == destructorName)
                        {
                            processed = true;
                        }
                        else if (m.OverridingBaseMethods.Count > 0)
                        {
                            overridingMethods.Add(m);
                        }
                        else if (m.Name.StartsWith("Get"))
                        {
                            if (m.ParameterTypes.Length == 0)
                            {
                                processed = true;
                                getters.Add(m.Name.Substring(3), m);
                            }
                        }
                        else if (m.Name.StartsWith("Set"))
                        {
                            if (m.ParameterTypes.Length == 1 && m.ReturnType.Kind == RgacTypeKind.Primitive && m.ReturnType.PrimitiveKind == RgacPrimitiveKind.Void)
                            {
                                processed = true;
                                setters.Add(m.Name.Substring(3), m);
                            }
                        }
                        if (!processed)
                        {
                            methods.Add(m);
                        }
                    }
                }

                foreach (var key in setters.Keys.ToArray())
                {
                    if (!getters.ContainsKey(key))
                    {
                        var setter = setters[key];
                        setters.Remove(key);
                        methods.Add(setter);
                    }
                }

                foreach (var name in getters.Keys.Concat(setters.Keys))
                {
                    RgacMethod getter = null;
                    RgacMethod setter = null;
                    getters.TryGetValue(name, out getter);
                    setters.TryGetValue(name, out setter);

                    RgacProperty prop = new RgacProperty()
                    {
                        Name = name,
                        PropertyType = getter.ReturnType,
                        OwnerUDT = getter.OwnerUDT,
                        Getter = getter,
                        Setter = setter,
                    };
                    properties.Add(prop);

                    if (getter == null)
                    {
                        throw new ArgumentException();
                    }
                }

                foreach (var field in inputFields.Where(f => f.Access == GacAccess.Public))
                {
                    if (field.Type.Kind != GacTypeKind.Const)
                    {
                        RgacProperty prop = new RgacProperty()
                        {
                            Name = field.Name,
                            PropertyType = TranslateType(input, udts, field.Type),
                            OwnerUDT = udt,
                            PublicGacFieldAccessor = field,
                            IsEventField = field.Type.Name.StartsWith("vl::presentation::elements::GuiGraphicsEvent<"),
                        };
                        properties.Add(prop);
                    }
                }
            }
            outputConstructors = constructors.ToArray();
            outputMethods = methods.ToArray();
            outputOverridingMethods = overridingMethods.ToArray();
            outputProperties = properties.ToArray();
        }

        static void CategorizeMethods(ReflectedTypeAnalyzerInput input, Dictionary<string, RgacUDT> udts, RgacUDT udt)
        {
            {
                RgacMethod[] constructors;
                RgacMethod[] methods;
                RgacMethod[] overridingMethods;
                RgacProperty[] properties;
                CategorizeMethods(input, udts, udt, udt.Methods, udt.AssociatedGacType.Fields, out constructors, out methods, out overridingMethods, out properties);
                udt.Constructors = constructors;
                udt.Methods = methods;
                udt.OverridingMethods = overridingMethods;
                udt.Properties = properties;
                if (constructors.Length == 0)
                {
                    udt.IsAbstract = true;
                }
            }
            {
                RgacMethod[] constructors;
                RgacMethod[] methods;
                RgacMethod[] overridingMethods;
                RgacProperty[] properties;
                CategorizeMethods(input, udts, udt, udt.StaticMethods, udt.AssociatedGacType.StaticFields, out constructors, out methods, out overridingMethods, out properties);
                if (constructors.Length > 0)
                {
                    throw new ArgumentException();
                }
                if (overridingMethods.Length > 0)
                {
                    throw new ArgumentException();
                }
                udt.StaticMethods = methods;
                udt.StaticProperties = properties;
            }
        }

        #endregion

        public static ReflectedTypeAnalyzerResult Analyze(ReflectedTypeAnalyzerInput input)
        {
            Dictionary<string, RgacUDT> udts =
                input.DescriptableUdts.Values.Concat(
                input.ExportableClasses.Values.Concat(
                input.ExportableEnums.Values.Concat(
                input.ExportableStructs.Values
                ))).Select(t => CreateUDT(input, t)).ToDictionary(t => t.AssociatedGacType.Name);

            foreach (var t in udts.Values)
            {
                FillUDT(input, udts, t);
            }
            foreach (var t in udts.Values)
            {
                ProcessOverriding(input, udts, t);
            }
            foreach (var t in udts.Values)
            {
                foreach (var m in t.Methods)
                {
                    if (m.OverridingBaseMethods.Count > 0 && m.OverridingDerivedMethods.Count > 0)
                    {
                        throw new ArgumentException();
                    }
                }
            }
            foreach (var t in udts.Values)
            {
                CategorizeMethods(input, udts, t);
            }

            var result = new ReflectedTypeAnalyzerResult
            {
                Udts = udts.Values.OrderBy(t => t.ToString()).ToArray(),
            };

            var duplicates = udts.Values.GroupBy(t => t.ToString()).Where(g => g.Count() > 1).ToArray();
            if (duplicates.Length > 0)
            {
                throw new ArgumentException();
            }
            return result;
        }
    }

    class CppName
    {
        public string Name { get; set; }
        public CppName[] Parameters { get; set; }
        public CppName Member { get; set; }

        #region Parsing

        private static string ParseName(string name, ref int index)
        {
            string result = "";
            while (index < name.Length)
            {
                bool normalChar = true;
                switch (name[index])
                {
                    case '<':
                    case '>':
                    case ',':
                    case ':':
                        normalChar = false;
                        break;
                }
                if (!normalChar) break;
                result += name[index];
                index++;

                if (result == "operator")
                {
                    result += name.Substring(index);
                    index = name.Length;
                    return result.Trim();
                }
            }
            if (string.IsNullOrWhiteSpace(result))
            {
                throw new ArgumentException();
            }
            return result.Trim();
        }

        private static bool ParseChar(string name, char c, ref int index)
        {
            while (index < name.Length && char.IsWhiteSpace(name[index]))
            {
                index++;
            }
            if (index < name.Length && name[index] == c)
            {
                index++;
                return true;
            }
            else
            {
                return false;
            }
        }

        private static CppName ParseCppName(string name, ref int index)
        {
            CppName result = new CppName();
            result.Name = ParseName(name, ref index);
            result.Parameters = new CppName[0];
            if (result.Name.EndsWith("*"))
            {
                result.Name = result.Name.Substring(0, result.Name.Length - 1).Trim() + "_raw_pointer";
            }
            if (ParseChar(name, '<', ref index))
            {
                List<CppName> parameters = new List<CppName>();
                while (true)
                {
                    parameters.Add(ParseCppName(name, ref index));
                    if (ParseChar(name, '>', ref index))
                    {
                        break;
                    }
                    else if (!ParseChar(name, ',', ref index))
                    {
                        throw new ArgumentException();
                    }
                }
                result.Parameters = parameters.ToArray();
            }
            if (ParseChar(name, ':', ref index))
            {
                if (!ParseChar(name, ':', ref index))
                {
                    throw new ArgumentException();
                }
                result.Member = ParseCppName(name, ref index);
            }
            return result;
        }

        public static CppName Parse(string name)
        {
            int index = 0;
            CppName result = ParseCppName(name.Trim(), ref index);
            if (index != name.Length)
            {
                throw new ArgumentException();
            }
            return result;
        }

        #endregion

        public override string ToString()
        {
            return Name
                + (Parameters.Length == 0 ? "" : "<" + Parameters.Select(n => n.ToString()).Aggregate("", (a, b) => a == "" ? b : a + ", " + b) + ">")
                + (Member == null ? "" : " :: " + Member.ToString())
                ;
        }
    }

    class ReflectedTypeAnalyzerInput
    {
        public Dictionary<string, GacUDT> DescriptableUdts { get; set; }
        public Dictionary<string, GacUDT> ExportableClasses { get; set; }
        public Dictionary<string, GacUDT> ExportableStructs { get; set; }
        public Dictionary<string, GacUDT> ExportableEnums { get; set; }
        public Dictionary<string, GacUDT> AvailableUdts { get; set; }
    }

    class ReflectedTypeAnalyzerResult
    {
        public RgacUDT[] Udts { get; set; }

        #region Exporting

        private string GetTypeName(RgacType type)
        {
            switch (type.Kind)
            {
                case RgacTypeKind.Primitive:
                    return type.OriginalGacType.Name;

                case RgacTypeKind.Class:
                    return type.AssociatedRgacType.ToString();
                case RgacTypeKind.ClassPointer:
                    return type.AssociatedRgacType.ToString() + "*";
                case RgacTypeKind.ClassSmartPointer:
                    return "Ptr<" + type.AssociatedRgacType.ToString() + ">";

                case RgacTypeKind.Struct:
                    return type.AssociatedRgacType.ToString();
                case RgacTypeKind.StructPointer:
                    return type.AssociatedRgacType.ToString() + "*";
                case RgacTypeKind.StructReference:
                    return type.AssociatedRgacType.ToString() + "&";
                case RgacTypeKind.ConstStructReference:
                    return type.AssociatedRgacType.ToString() + " const&";

                case RgacTypeKind.Enum:
                    return type.AssociatedRgacType.ToString();
                case RgacTypeKind.OtherGacType:
                    return type.OriginalGacType.Name;

                default:
                    throw new ArgumentException();
            }
        }

        private XElement Export(RgacMethod method)
        {
            return new XElement("method",
                new XAttribute("name", method.Name),
                new XAttribute("virtual", method.Kind),
                new XElement("original", new XAttribute("name", method.OriginalGacMethod.Name)),
                new XElement("returnType", new XAttribute("name", GetTypeName(method.ReturnType))),
                new XElement("parameterTypes", method.ParameterTypes.Select(t =>
                    new XElement("parameterType", new XAttribute("name", GetTypeName(t)))
                    )),
                new XElement("parameterNames", method.ParameterNames.Select(n =>
                    new XElement("parameterName", new XAttribute("name", n))
                    ))
                );
        }

        private XElement Export(RgacProperty property)
        {
            XElement e = new XElement("property");
            e.Add(new XAttribute("name", property.Name));
            e.Add(new XAttribute("type", GetTypeName(property.PropertyType)));
            e.Add(new XAttribute("isEvent", property.IsEventField));
            if (property.Getter != null)
            {
                e.Add(Export(property.Getter));
            }
            if (property.Setter != null)
            {
                e.Add(Export(property.Setter));
            }
            if (property.PublicGacFieldAccessor != null)
            {
                e.Add(new XElement("fieldAccessor", new XAttribute("name", property.PublicGacFieldAccessor.Name)));
            }
            return e;
        }

        private XElement Export(RgacUDT udt)
        {
            return new XElement(udt.Kind.ToString(),
                new XAttribute("name", udt.ToString()),
                new XAttribute("isAbstract", udt.IsAbstract),
                new XAttribute("descriptable", udt.Descriptable),
                new XElement("original", new XAttribute("name", udt.AssociatedGacType.Name)),
                (udt.BaseClasses.Length == 0 ? (object)"" : (object)
                new XElement("baseClasses",
                    udt.BaseClasses.Select(t => new XElement("baseClass", new XAttribute("name", t.ToString())))
                    )),
                (udt.Constructors.Length == 0 ? (object)"" : (object)
                new XElement("constructors",
                    udt.Constructors.Select(m => Export(m))
                    )),
                (udt.Methods.Length == 0 ? (object)"" : (object)
                new XElement("methods",
                    udt.Methods.Select(m => Export(m))
                    )),
                (udt.OverridingMethods.Length == 0 ? (object)"" : (object)
                new XElement("overridingMethods",
                    udt.OverridingMethods.Select(m => Export(m))
                    )),
                (udt.StaticMethods.Length == 0 ? (object)"" : (object)
                new XElement("staticMethods",
                    udt.StaticMethods.Select(m => Export(m))
                    )),
                (udt.Properties.Length == 0 ? (object)"" : (object)
                new XElement("properties",
                    udt.Properties.Select(m => Export(m))
                    )),
                (udt.StaticProperties.Length == 0 ? (object)"" : (object)
                new XElement("staticProperties",
                    udt.StaticProperties.Select(m => Export(m))
                    ))
                );
        }

        public XDocument Export()
        {
            XDocument document = new XDocument();
            document.Add(new XElement("pdbAnalysis", Udts.Select(t => Export(t)).ToArray()));
            return document;
        }

        #endregion
    }
}
