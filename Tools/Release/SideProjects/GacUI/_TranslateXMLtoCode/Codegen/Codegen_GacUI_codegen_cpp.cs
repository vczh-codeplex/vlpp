using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;

namespace _TranslateXMLtoCode.Codegen
{
    class Codegen_GacUI_codegen_cpp : Codegen_GacUI_base
    {
        #region GenerateMembers

        protected void GenerateConstructorInitializationList(RgacUDT udt, string internalObjectReferenceInput)
        {
            WriteLine("    :__internal_object_reference({0})", internalObjectReferenceInput);
            foreach (var baseUdt in udt.BaseClasses
                .Where(t => t.Access == GacAccess.Public && this.options.Udts.Contains(t.UDT))
                .Select(t => t.UDT)
                )
            {
                WriteLine("    ,{0}(static_cast<__GacUIInternal<{1}>::InternalObjectType*>((__GacUIInternal<{2}>::InternalObjectType*){3}))",
                    baseUdt.ToString(),
                    baseUdt.ToString(),
                    udt.ToString(),
                    internalObjectReferenceInput
                    );
            }
        }

        protected void GenerateConstructor(RgacMethod method)
        {
            WriteLine("rptr<{0}> {1}CreateRptr({2})",
                GacUdtTypeName(method.OwnerUDT),
                method.OwnerUDT.Name.Aggregate("", (a, b) => a + b + "::"),
                method.ParameterTypes
                    .Zip(method.ParameterNames, Tuple.Create)
                    .Select(t => string.Format("{0} {1}", GetType(t.Item1), t.Item2))
                    .Aggregate("", (a, b) => a == "" ? b : a + ", " + b)
                );
            Begin("{");
            WriteLine("throw 0;");
            End("}");
            WriteLine("");

            if (method.OwnerUDT.Kind == RgacUDTKind.Struct)
            {
                WriteLine("{0} {1}Create({2})",
                    GacUdtTypeName(method.OwnerUDT),
                    method.OwnerUDT.Name.Aggregate("", (a, b) => a + b + "::"),
                    method.ParameterTypes
                        .Zip(method.ParameterNames, Tuple.Create)
                        .Select(t => string.Format("{0} {1}", GetType(t.Item1), t.Item2))
                        .Aggregate("", (a, b) => a == "" ? b : a + ", " + b)
                    );
                Begin("{");
                WriteLine("throw 0;");
                End("}");
                WriteLine("");
            }
        }

        protected void GenerateMethod(RgacMethod method, bool isStatic)
        {
            WriteLine("{0} {1}{2}({3})",
                GetType(method.ReturnType),
                method.OwnerUDT.Name.Aggregate("", (a, b) => a + b + "::"),
                method.Name,
                method.ParameterTypes
                    .Zip(method.ParameterNames, Tuple.Create)
                    .Select(t => string.Format("{0} {1}", GetType(t.Item1), t.Item2))
                    .Aggregate("", (a, b) => a == "" ? b : a + ", " + b)
                );
            Begin("{");
            WriteLine("throw 0;");
            End("}");
            WriteLine("");
        }

        protected void GenerateProperty(RgacProperty property, bool isStatic)
        {
            if (property.Getter != null)
                GenerateMethod(property.Getter, isStatic);
            if (property.Setter != null)
                GenerateMethod(property.Setter, isStatic);
            if (property.PublicGacFieldAccessor != null)
            {
                if (property.IsEventField)
                {
                    WriteLine("{0} {1}on_{2}()",
                        GetType(property.PropertyType),
                        property.OwnerUDT.Name.Aggregate("", (a, b) => a + b + "::"),
                        property.Name
                        );
                    Begin("{");
                    WriteLine("throw 0;");
                    End("}");
                    WriteLine("");
                }
                else
                {
                    {
                        WriteLine("{0} {1}get_{2}()",
                            GetType(property.PropertyType),
                            property.OwnerUDT.Name.Aggregate("", (a, b) => a + b + "::"),
                            property.Name
                            );
                        Begin("{");
                        WriteLine("throw 0;");
                        End("}");
                        WriteLine("");
                    }
                    {
                        WriteLine("void {0}set_{1}({2} value)",
                            property.OwnerUDT.Name.Aggregate("", (a, b) => a + b + "::"),
                            property.Name,
                            GetType(property.PropertyType)
                            );
                        Begin("{");
                        WriteLine("throw 0;");
                        End("}");
                    }
                }
            }
        }

        protected void GenerateMembers(RgacUDT udt)
        {
            {
                WriteLine("template<>");
                WriteLine("class __GacUIInternal<{0}>", udt.ToString());
                WriteLine("{");
                Begin("public:");
                WriteLine("typedef {0} WrappedObjectType;", udt.ToString());
                WriteLine("typedef {0} InternalObjectType;", udt.AssociatedGacType.Name);
                WriteLine("");
                WriteLine("static InternalObjectType* GetInternalObject(const WrappedObjectType& wrappedObject)");
                Begin("{");
                WriteLine("return (InternalObjectType*)wrappedObject.__internal_object_reference;");
                End("}");
                End("};");
                WriteLine("");
            }
            {
                WriteLine("{0}{1}(void* __internal_object_reference_input)",
                    udt.Name.Aggregate("", (a, b) => a + b + "::"),
                    udt.Name.Last()
                    );
                GenerateConstructorInitializationList(udt, "__internal_object_reference_input");
                Begin("{");
                End("}");
                WriteLine("");

                WriteLine("{0}~{1}()",
                    udt.Name.Aggregate("", (a, b) => a + b + "::"),
                    udt.Name.Last()
                    );
                Begin("{");
                if (udt.Kind == RgacUDTKind.Struct)
                {
                    WriteLine("delete __GacUIInternal<{0}>::GetInternalObject(*this);", udt.Name.Last());
                }
                End("}");
                WriteLine("");
            }
            if (!udt.IsAbstract)
            {
                foreach (var m in udt.Constructors)
                {
                    GenerateConstructor(m);
                }
            }

            foreach (var m in udt.Methods)
            {
                GenerateMethod(m, false);
            }

            foreach (var m in udt.StaticMethods)
            {
                GenerateMethod(m, true);
            }

            foreach (var m in udt.Properties)
            {
                GenerateProperty(m, false);
            }

            foreach (var m in udt.StaticProperties)
            {
                GenerateProperty(m, true);
            }
        }

        #endregion

        protected void GenerateCppImpl()
        {
            foreach (var udt in GetSortedUdts().ToArray())
            {
                if (udt.Kind != RgacUDTKind.Enum)
                {
                    WriteSectionComment(udt.ToString());
                    GenerateMembers(udt);
                }
            }
        }

        protected override void GenerateCodeInternal()
        {
            WriteLine("#include \"GacUI.h\"");
            WriteLine("#include \"..\\..\\..\\..\\..\\Candidate\\GUI\\GUI\\GacUI.h\"");
            WriteLine("");
            WriteLine("namespace {0}", Codegen_GacUI_base.GacUINamespace);
            Begin("{");

            WriteLine("template<typename T>");
            WriteLine("class __GacUIInternal{};");

            GenerateCppImpl();

            End("}");
        }

        public Codegen_GacUI_codegen_cpp(CodeGeneratorOptions options)
            : base(options, options.GacuiPath + "GacUI.cpp")
        {
        }
    }
}
