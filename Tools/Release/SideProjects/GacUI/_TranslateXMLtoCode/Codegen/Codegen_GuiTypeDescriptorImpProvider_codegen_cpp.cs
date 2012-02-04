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

        protected string GetTypeCacheVariableName(RgacUDT udt)
        {
            return "cache_" + udt.Name
                .Aggregate((a, b) => a + "_member_" + b);
        }

        protected string GetTypeCacheVariable(RgacUDT udt)
        {
            return "(gacui_tpimp_type_cache_table." + GetTypeCacheVariableName(udt) + ")";
        }

        protected void GenerateTypeCacheCode()
        {
            WriteSectionComment("Type Caching");
            WriteLine("struct gacui_tpimp_type_cache_table_structure");
            Begin("{");
            foreach (var udt in this.options.Udts
                .OrderBy(t => (t.Descriptable ? "0" : "1") + t.ToString())
                )
            {
                WriteLine("Type* {0};", GetTypeCacheVariableName(udt));
            }
            End("} gacui_tpimp_type_cache_table;");
        }

        #endregion

        #region TypeProvider

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

        protected string GetType(RgacType type)
        {
            return "0";
        }

        protected void GenerateMethod(RgacUDT owner, RgacMethod method, bool isStatic)
        {
            WriteLine("(new MethodDescriptor(L\"{0}\", IMemberDescriptor::{1}))", method.Name, (isStatic ? "Static" : method.Kind.ToString()));
            WriteLine("->ReturnType(" + GetType(method.ReturnType) + ")");
            for (int i = 0; i < method.ParameterTypes.Length; i++)
            {
                if (method.ParameterNames.Length > 0)
                {
                    WriteLine("->Parameter(L\"{0}\", {1})", method.ParameterNames[i], GetType(method.ParameterTypes[i]));
                }
            }
        }

        protected void GenerateFieldAccessGetter(RgacUDT owner, RgacType type, string name, bool isStatic)
        {
            WriteLine("(new MethodDescriptor(L\"{0}\", IMemberDescriptor::{1}))", "get_" + name, (isStatic ? "Static" : "Normal"));
            WriteLine("->ReturnType(" + GetType(type) + ")");
        }

        protected void GenerateFieldAccessSetter(RgacUDT owner, RgacType type, string name, bool isStatic)
        {
            WriteLine("(new MethodDescriptor(L\"{0}\", IMemberDescriptor::{1}))", "set_" + name, (isStatic ? "Static" : "Normal"));
            WriteLine("->ReturnType(" + GetType(new RgacType
            {
                Kind = RgacTypeKind.Primitive,
                PrimitiveKind = RgacPrimitiveKind.Void,
            }) + ")");
            WriteLine("->Parameter(L\"value\", " + GetType(type) + ")");
        }

        protected void GenerateProperty(RgacUDT owner, RgacProperty property, bool isStatic)
        {
            WriteLine("(new PropertyDescriptor(L\"{0}\", IMemberDescriptor::{1}))", property.Name, (isStatic ? "Static" : "Normal"));
            WriteLine("->PropertyType(" + GetType(property.PropertyType) + ")");
            if (property.PublicGacFieldAccessor == null)
            {
                if (property.Getter != null)
                {
                    Begin("->Getter(");
                    GenerateMethod(owner, property.Getter, isStatic);
                    End(")");
                }
                if (property.Setter != null)
                {
                    Begin("->Setter(");
                    GenerateMethod(owner, property.Setter, isStatic);
                    End(")");
                }
            }
            else
            {
                Begin("->Getter(");
                GenerateFieldAccessGetter(owner, property.PropertyType, property.Name, isStatic);
                End(")");

                Begin("->Setter(");
                GenerateFieldAccessSetter(owner, property.PropertyType, property.Name, isStatic);
                End(")");
            }
        }

        protected void GenerateEnumItemProperty(RgacUDT owner, RgacType ownerType, GacConst enumItem)
        {
            WriteLine("(new PropertyDescriptor(L\"{0}\", IMemberDescriptor::{1}))", enumItem.Name, "Static");
            WriteLine("->PropertyType(" + GetType(ownerType) + ")");
            Begin("->Getter(");
            GenerateFieldAccessGetter(owner, ownerType, enumItem.Name, true);
            End(")");
        }

        protected void GenerateTypeDescriptorBody(RgacUDT udt)
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
                    GenerateEnumItemProperty(udt, enumType, c);
                    End(");");
                }
            }
            else
            {
                if (udt.Constructors.Length > 0)
                {
                    foreach (var m in udt.Constructors)
                    {
                        Begin("AddConstructor(");
                        GenerateMethod(udt, m, false);
                        End(");");
                    }
                }
                foreach (var m in udt.Methods)
                {
                    Begin("AddMethod(");
                    GenerateMethod(udt, m, false);
                    End(");");
                }
                foreach (var m in udt.StaticMethods)
                {
                    Begin("AddMethod(");
                    GenerateMethod(udt, m, true);
                    End(");");
                }
                foreach (var p in udt.Properties)
                {
                    Begin("AddProperty(");
                    GenerateProperty(udt, p, false);
                    End(");");
                }
                foreach (var p in udt.StaticProperties)
                {
                    Begin("AddProperty(");
                    GenerateProperty(udt, p, true);
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
                    if (i == udt.Name.Length - 1)
                    {
                        WriteLine("class {0} : public TypeDescriptor", GetCppClassName(className));
                        WriteLine("{");
                        Begin("protected:");
                        WriteLine("void FillTypeContent()");
                        Begin("{");
                        GenerateTypeDescriptorBody(udt);
                        End("}");
                        End("");
                        Begin("public:");
                    }
                    else
                    {
                        WriteLine("class {0}", GetCppClassName(className));
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
        }

        public Codegen_GuiTypeDescriptorImpProvider_codegen_cpp(CodeGeneratorOptions options)
            : base(options.ReflectionPath + "GuiTypeDescriptorImpProvider_codegen.cpp")
        {
            this.options = options;
        }
    }
}
