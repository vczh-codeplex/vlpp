using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.IO;

namespace _TranslateXMLtoCode.Codegen
{
    class Codegen_GuiTypeDescriptorImpProvider_codegen_cpp : CodeGenerator
    {
        private CodeGeneratorOptions options;

        #region TypeCache

        protected string[] PrimaryTypeNames = new string[]
        {
            "SInt8", "SInt16", "SInt32", "SInt64",
            "UInt8", "UInt16", "UInt32", "UInt64",
            "Float", "Double", "Char", "String", "Bool",
            "Void",
        };

        protected string GetTypeCacheVariableName(RgacUDT udt)
        {
            return "cache_" + udt.Name
                .Aggregate((a, b) => a + "_member_" + b);
        }

        protected string GetPrimaryTypeCacheVariableName(string primaryTypeName)
        {
            return "primary_" + primaryTypeName;
        }

        protected string GetTypeCacheVariable(RgacUDT udt)
        {
            return "(gacui_tpimp_type_cache_table." + GetTypeCacheVariableName(udt) + ")";
        }

        protected string GetPrimaryTypeCacheVariable(string primaryTypeName)
        {
            return "(gacui_tpimp_type_cache_table." + GetPrimaryTypeCacheVariableName(primaryTypeName) + ")";
        }

        protected void GenerateTypeCacheCode()
        {
            WriteSectionComment("Type Caching");
            WriteLine("struct gacui_tpimp_type_cache_table_structure");
            Begin("{");
            foreach (var name in PrimaryTypeNames)
            {
                WriteLine("Type* {0};", GetPrimaryTypeCacheVariableName(name));
            }
            WriteLine("");
            foreach (var udt in this.options.Udts
                .OrderBy(t => (t.Descriptable ? "0" : "1") + GetTypeCacheVariableName(t))
                )
            {
                WriteLine("Type* {0};", GetTypeCacheVariableName(udt));
            }
            End("} gacui_tpimp_type_cache_table;");
        }

        #endregion

        #region TypeProvider

        protected HashSet<string> UnknownTypes = new HashSet<string>();

        protected string GetCppClassName(string name)
        {
            return "gacui_tpimp_" + name;
        }

        protected string GetCppClassName(RgacUDT udt)
        {
            return udt.Name
                .Select(n => GetCppClassName(n))
                .Aggregate((a, b) => a + " :: " + b);
        }

        protected string GetType(GacType type)
        {
            switch (type.Kind)
            {
                case GacTypeKind.Primitive:
                    {
                        switch (type.Name)
                        {
                            case "signed __int8":
                                return GetPrimaryTypeCacheVariable("SInt8");
                            case "signed __int16":
                                return GetPrimaryTypeCacheVariable("SInt16");
                            case "int":
                            case "signed __int32":
                                return GetPrimaryTypeCacheVariable("SInt32");
                            case "signed __int64":
                                return GetPrimaryTypeCacheVariable("SInt64");
                            case "unsigned __int8":
                                return GetPrimaryTypeCacheVariable("UInt8");
                            case "unsigned __int16":
                                return GetPrimaryTypeCacheVariable("UInt16");
                            case "unsigned __int32":
                                return GetPrimaryTypeCacheVariable("UInt32");
                            case "unsigned __int64":
                                return GetPrimaryTypeCacheVariable("UInt64");
                            case "wchar_t":
                                return GetPrimaryTypeCacheVariable("Char");
                            case "bool":
                                return GetPrimaryTypeCacheVariable("Bool");
                            case "float":
                                return GetPrimaryTypeCacheVariable("Float");
                            case "double":
                                return GetPrimaryTypeCacheVariable("Double");
                            case "void":
                                return GetPrimaryTypeCacheVariable("Void");
                        }
                    }
                    break;
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
                            return GetTypeCacheVariable(elementType);
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
                            case RgacPrimitiveKind.SInt8:
                                return GetPrimaryTypeCacheVariable("SInt8");
                            case RgacPrimitiveKind.SInt16:
                                return GetPrimaryTypeCacheVariable("SInt16");
                            case RgacPrimitiveKind.SInt32:
                                return GetPrimaryTypeCacheVariable("SInt32");
                            case RgacPrimitiveKind.SInt64:
                                return GetPrimaryTypeCacheVariable("SInt64");
                            case RgacPrimitiveKind.UInt8:
                                return GetPrimaryTypeCacheVariable("UInt8");
                            case RgacPrimitiveKind.UInt16:
                                return GetPrimaryTypeCacheVariable("UInt16");
                            case RgacPrimitiveKind.UInt32:
                                return GetPrimaryTypeCacheVariable("UInt32");
                            case RgacPrimitiveKind.UInt64:
                                return GetPrimaryTypeCacheVariable("UInt64");
                            case RgacPrimitiveKind.Char:
                                return GetPrimaryTypeCacheVariable("Char");
                            case RgacPrimitiveKind.String:
                                return GetPrimaryTypeCacheVariable("String");
                            case RgacPrimitiveKind.StringReference:
                                return GetPrimaryTypeCacheVariable("String") + "->GetReferenceType()";
                            case RgacPrimitiveKind.ConstStringReference:
                                return GetPrimaryTypeCacheVariable("String") + "->GetConstReferenceType()";
                            case RgacPrimitiveKind.Bool:
                                return GetPrimaryTypeCacheVariable("Bool");
                            case RgacPrimitiveKind.Float:
                                return GetPrimaryTypeCacheVariable("Float");
                            case RgacPrimitiveKind.Double:
                                return GetPrimaryTypeCacheVariable("Double");
                            case RgacPrimitiveKind.Void:
                                return GetPrimaryTypeCacheVariable("Void");
                        }
                    }
                    break;

                case RgacTypeKind.Class:
                    return GetTypeCacheVariable(type.AssociatedRgacType);
                case RgacTypeKind.ClassPointer:
                    return GetTypeCacheVariable(type.AssociatedRgacType) + "->GetPointerType()";
                case RgacTypeKind.ClassSmartPointer:
                    return GetTypeCacheVariable(type.AssociatedRgacType) + "->GetSmartPointerType()";
                case RgacTypeKind.ClassReference:
                    return GetTypeCacheVariable(type.AssociatedRgacType) + "->GetReferenceType()";
                case RgacTypeKind.ConstClassReference:
                    return GetTypeCacheVariable(type.AssociatedRgacType) + "->GetConstReferenceType()";

                case RgacTypeKind.Struct:
                    return GetTypeCacheVariable(type.AssociatedRgacType);
                case RgacTypeKind.StructPointer:
                    return GetTypeCacheVariable(type.AssociatedRgacType) + "->GetPointerType()";
                case RgacTypeKind.StructSmartPointer:
                    return GetTypeCacheVariable(type.AssociatedRgacType) + "->GetSmartPointerType()";
                case RgacTypeKind.StructReference:
                    return GetTypeCacheVariable(type.AssociatedRgacType) + "->GetReferenceType()";
                case RgacTypeKind.ConstStructReference:
                    return GetTypeCacheVariable(type.AssociatedRgacType) + "->GetConstReferenceType()";

                case RgacTypeKind.Enum:
                    return GetTypeCacheVariable(type.AssociatedRgacType);
                case RgacTypeKind.OtherGacType:
                    {
                        string result = GetType(type.OriginalGacType);
                        if (result != null) return result;
                    }
                    break;
            }
            UnknownTypes.Add(type.ToString());
            return "0 /*UNKNOWN_TYPE[ " + type.ToString() + " ]*/";
        }

        protected void GenerateMethodHandler(RgacUDT owner, string methodInvokerName)
        {
            WriteLine("->Handler(MethodDescriptor::HandlerFuncType(&{0}::{1}))", GetCppClassName(owner), methodInvokerName);
        }

        protected void GenerateMethodInvoker(RgacUDT owner, string methodInvokerName, List<Action> methodInvokers, Action action)
        {
            methodInvokers.Add(() =>
            {
                WriteLine("static DescriptableValue {0}(const DescriptableValue& thisObject, const collections::IReadonlyList<DescriptableValue>& parameters)", methodInvokerName);
                Begin("{");
                action();
                End("}");
            });
        }

        protected void GenerateMethod(RgacUDT owner, RgacMethod method, bool isStatic, List<Action> methodInvokers)
        {
            WriteLine("(new MethodDescriptor(L\"{0}\", IMemberDescriptor::{1}))", method.Name, (isStatic ? "Static" : method.Kind.ToString()));
            WriteLine("->ReturnType(" + GetType(method.ReturnType) + ")");
            for (int i = 0; i < method.ParameterTypes.Length; i++)
            {
                WriteLine("->Parameter(L\"{0}\", {1})", method.ParameterNames[i], GetType(method.ParameterTypes[i]));
            }

            string methodName = method.Name;
            if (methodName.StartsWith("operator"))
            {
                switch (methodName)
                {
                    case "operator[]": methodName = "operator_index"; break;
                    case "operator=": methodName = "operator_assign"; break;
                    case "operator<": methodName = "operator_lt"; break;
                    case "operator<=": methodName = "operator_le"; break;
                    case "operator>": methodName = "operator_gt"; break;
                    case "operator>=": methodName = "operator_ge"; break;
                    case "operator==": methodName = "operator_eq"; break;
                    case "operator!=": methodName = "operator_ne"; break;
                    default:
                        throw new ArgumentException();
                }
            }
            string methodInvokerName = "method_handler_" + methodName + "_" + methodInvokers.Count.ToString();
            GenerateMethodHandler(owner, methodInvokerName);
            GenerateMethodInvoker(owner, methodInvokerName, methodInvokers, () =>
            {
                WriteLine("throw 0;");
            });
        }

        protected void GenerateFieldAccessGetter(RgacUDT owner, RgacType type, string name, bool isStatic, List<Action> methodInvokers)
        {
            WriteLine("(new MethodDescriptor(L\"{0}\", IMemberDescriptor::{1}))", "get_" + name, (isStatic ? "Static" : "Normal"));
            WriteLine("->ReturnType(" + GetType(type) + ")");

            string methodInvokerName = "method_handler_get_" + name + "_" + methodInvokers.Count.ToString();
            GenerateMethodHandler(owner, methodInvokerName);
            GenerateMethodInvoker(owner, methodInvokerName, methodInvokers, () =>
            {
                WriteLine("throw 0;");
            });
        }

        protected void GenerateFieldAccessSetter(RgacUDT owner, RgacType type, string name, bool isStatic, List<Action> methodInvokers)
        {
            WriteLine("(new MethodDescriptor(L\"{0}\", IMemberDescriptor::{1}))", "set_" + name, (isStatic ? "Static" : "Normal"));
            WriteLine("->ReturnType(" + GetType(new RgacType
            {
                Kind = RgacTypeKind.Primitive,
                PrimitiveKind = RgacPrimitiveKind.Void,
            }) + ")");
            WriteLine("->Parameter(L\"value\", " + GetType(type) + ")");

            string methodInvokerName = "method_handler_set_" + name + "_" + methodInvokers.Count.ToString();
            GenerateMethodHandler(owner, methodInvokerName);
            GenerateMethodInvoker(owner, methodInvokerName, methodInvokers, () =>
            {
                WriteLine("throw 0;");
            });
        }

        protected void GenerateProperty(RgacUDT owner, RgacProperty property, bool isStatic, List<Action> methodInvokers)
        {
            WriteLine("(new PropertyDescriptor(L\"{0}\", IMemberDescriptor::{1}))", property.Name, (isStatic ? "Static" : "Normal"));
            WriteLine("->PropertyType(" + GetType(property.PropertyType) + ")");
            if (property.PublicGacFieldAccessor == null)
            {
                if (property.Getter != null)
                {
                    Begin("->Getter(");
                    GenerateMethod(owner, property.Getter, isStatic, methodInvokers);
                    End(")");
                }
                if (property.Setter != null)
                {
                    Begin("->Setter(");
                    GenerateMethod(owner, property.Setter, isStatic, methodInvokers);
                    End(")");
                }
            }
            else
            {
                Begin("->Getter(");
                GenerateFieldAccessGetter(owner, property.PropertyType, property.Name, isStatic, methodInvokers);
                End(")");

                Begin("->Setter(");
                GenerateFieldAccessSetter(owner, property.PropertyType, property.Name, isStatic, methodInvokers);
                End(")");
            }
        }

        protected void GenerateEnumItemProperty(RgacUDT owner, RgacType ownerType, GacConst enumItem, List<Action> methodInvokers)
        {
            WriteLine("(new PropertyDescriptor(L\"{0}\", IMemberDescriptor::{1}))", enumItem.Name, "Static");
            WriteLine("->PropertyType(" + GetType(ownerType) + ")");
            Begin("->Getter(");
            GenerateFieldAccessGetter(owner, ownerType, enumItem.Name, true, methodInvokers);
            End(")");
        }

        protected void GenerateTypeDescriptorBody(RgacUDT udt, List<Action> methodInvokers)
        {
            if (udt.Kind == RgacUDTKind.Enum)
            {
                RgacType enumType = new RgacType
                {
                    Kind = RgacTypeKind.Enum,
                    AssociatedRgacType = udt,
                    OriginalGacType = new GacType
                    {
                        Name = udt.AssociatedGacType.Name,
                        Kind = GacTypeKind.UDT,
                        AssociatedUDT = udt.AssociatedGacType,
                    },
                };
                foreach (var c in udt.AssociatedGacType.Constants)
                {
                    Begin("AddProperty(");
                    GenerateEnumItemProperty(udt, enumType, c, methodInvokers);
                    End(");");
                }
            }
            else
            {
                foreach (var t in udt.BaseClasses)
                {
                    if (options.Udts.Contains(t))
                    {
                        WriteLine("AddBaseType(" + GetTypeCacheVariable(t) + ");");
                    }
                }
                if (udt.Constructors.Length > 0)
                {
                    if (!udt.IsAbstract)
                    {
                        foreach (var m in udt.Constructors)
                        {
                            Begin("AddConstructor(");
                            GenerateMethod(udt, m, false, methodInvokers);
                            End(");");
                        }
                    }
                }
                foreach (var m in udt.Methods)
                {
                    Begin("AddMethod(");
                    GenerateMethod(udt, m, false, methodInvokers);
                    End(");");
                }
                foreach (var m in udt.StaticMethods)
                {
                    Begin("AddMethod(");
                    GenerateMethod(udt, m, true, methodInvokers);
                    End(");");
                }
                foreach (var p in udt.Properties)
                {
                    Begin("AddProperty(");
                    GenerateProperty(udt, p, false, methodInvokers);
                    End(");");
                }
                foreach (var p in udt.StaticProperties)
                {
                    Begin("AddProperty(");
                    GenerateProperty(udt, p, true, methodInvokers);
                    End(");");
                }
            }
        }

        protected void GenerateTypeProviderClasses()
        {
            List<string> classNames = new List<string>();
            foreach (var udt in this.options.Udts)
            {
                int commonClassNames = 0;
                for (int i = 0; i < udt.Name.Length; i++)
                {
                    if (i < classNames.Count && classNames[i] == udt.Name[i])
                    {
                        commonClassNames = i + 1;
                    }
                    else
                    {
                        break;
                    }
                }

                for (int i = classNames.Count; i > commonClassNames; i--)
                {
                    End("};");
                    classNames.RemoveAt(i - 1);
                }

                WriteSectionComment(string.Format("{0} ({1})", udt.ToString(), udt.AssociatedGacType.Name));
                for (int i = commonClassNames; i < udt.Name.Length; i++)
                {
                    string className = udt.Name[i];
                    classNames.Add(className);
                    string cppClassName = GetCppClassName(className);

                    if (i == udt.Name.Length - 1)
                    {
                        List<Action> methodInvokers = new List<Action>();
                        WriteLine("class {0} : public TypeDescriptor", cppClassName);
                        WriteLine("{");
                        Begin("protected:");
                        WriteLine("void FillTypeContent()");
                        Begin("{");
                        GenerateTypeDescriptorBody(udt, methodInvokers);
                        End("}");
                        End("");
                        Begin("private:");
                        foreach (var action in methodInvokers)
                        {
                            WriteLine("");
                            action();
                        }
                        End("");
                        Begin("public:");
                    }
                    else
                    {
                        WriteLine("class {0}", cppClassName);
                        WriteLine("{");
                        Begin("public:");
                    }
                }
            }
            for (int i = 0; i < classNames.Count; i++)
            {
                End("};");
            }
        }

        #endregion

        #region TypeRegistering

        protected void GenerateTypeRegisteringCode()
        {
            WriteSectionComment("Helper Functions");
            WriteLine("ITypeProvider* CreateDefaultTypeProvider()");
            Begin("{");
            WriteLine("TypeProvider* typeProvider=new TypeProvider;");
            foreach (var name in PrimaryTypeNames)
            {
                WriteLine("{0} = dynamic_cast<Type*>(typeProvider->{1}());", GetPrimaryTypeCacheVariable(name), name);
            }
            WriteLine("");
            foreach (var udt in this.options.Udts
                .OrderBy(t => (t.Descriptable ? "0" : "1") + t.ToString())
                )
            {
                string creator = string.Format(
                    "({0} = typeProvider->CreateType(IType::{1}, L\"{2}\", new {3}))",
                    GetTypeCacheVariable(udt),
                    udt.Kind,
                    GetCppClassName(udt),
                    GetCppClassName(udt)
                    );
                if (udt.Descriptable)
                {
                    string className = udt.AssociatedGacType.Name;
                    WriteLine("Description<{0}>::SetTypeDescroptorByTypeProvider({1}->GetTypeDescriptor());",
                        className,
                        creator
                        );
                }
                else
                {
                    WriteLine(creator + ";");
                }
            }
            WriteLine("return typeProvider;");
            End("}");
        }

        #endregion

        protected override void GenerateCodeInternal()
        {
            WriteLine("#include \"GuiTypeDescriptorImpProvider_codegen.h\"");
            WriteLine("#include \"..\\GacUI.h\"");
            WriteLine("");
            WriteLine("namespace vl");
            Begin("{");
            WriteLine("namespace presentation");
            Begin("{");
            WriteLine("namespace reflection_implementation");
            Begin("{");

            GenerateTypeCacheCode();
            GenerateTypeProviderClasses();
            GenerateTypeRegisteringCode();

            End("}");
            End("}");
            End("}");

            if (UnknownTypes.Count > 0)
            {
                foreach (var name in UnknownTypes)
                {
                    Console.WriteLine("    [UNKNOWN TYPE]: " + name);
                }
            }
        }

        public Codegen_GuiTypeDescriptorImpProvider_codegen_cpp(CodeGeneratorOptions options)
            : base(options.ReflectionPath + "GuiTypeDescriptorImpProvider_codegen.cpp")
        {
            this.options = options;
        }
    }
}
