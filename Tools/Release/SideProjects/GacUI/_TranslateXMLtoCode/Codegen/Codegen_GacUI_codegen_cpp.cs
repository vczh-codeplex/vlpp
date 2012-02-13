using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;

namespace _TranslateXMLtoCode.Codegen
{
    class Codegen_GacUI_codegen_cpp : Codegen_GacUI_base
    {
        #region GenerateMembers

        protected void GenerateConstructor(RgacMethod method)
        {
            WriteLine("rptr<{0}> {1}Create({2});",
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

            if (method.OwnerUDT.Kind == RgacUDTKind.Struct)
            {
                WriteLine("{0}{1}({2});",
                    method.Name.Aggregate("", (a, b) => a + b + "::"),
                    method.OwnerUDT.Name.Last(),
                    method.ParameterTypes
                        .Zip(method.ParameterNames, Tuple.Create)
                        .Select(t => string.Format("{0} {1}", GetType(t.Item1), t.Item2))
                        .Aggregate("", (a, b) => a == "" ? b : a + ", " + b)
                    );
                Begin("{");
                WriteLine("throw 0;");
                End("}");
            }
        }

        protected void GenerateMethod(RgacMethod method, bool isStatic)
        {
            WriteLine("{0} {1}{2}({3});",
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
        }

        protected void GenerateProperty(RgacProperty property, bool isStatic)
        {
            if (property.Getter != null)
                GenerateMethod(property.Getter, isStatic);
            if (property.Setter != null)
                GenerateMethod(property.Setter, isStatic);
            if (property.PublicGacFieldAccessor != null)
            {
                {
                    WriteLine("{0} {1}get_{2}();",
                        GetType(property.PropertyType),
                        property.OwnerUDT.Name.Aggregate("", (a, b) => a + b + "::"),
                        property.Name
                        );
                    Begin("{");
                    WriteLine("throw 0;");
                    End("}");
                }
                if (!property.IsEventField)
                {
                    WriteLine("void {0}set_{1}({2} value);",
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

        protected void GenerateMembers(RgacUDT udt)
        {
            if (!udt.IsAbstract)
            {
                foreach (var m in udt.Constructors)
                {
                    GenerateConstructor(m);
                    WriteLine("");
                }
            }

            foreach (var m in udt.Methods)
            {
                GenerateMethod(m, false);
                WriteLine("");
            }

            foreach (var m in udt.StaticMethods)
            {
                GenerateMethod(m, true);
                WriteLine("");
            }

            foreach (var m in udt.Properties)
            {
                GenerateProperty(m, false);
                WriteLine("");
            }

            foreach (var m in udt.StaticProperties)
            {
                GenerateProperty(m, true);
                WriteLine("");
            }
        }

        #endregion

        protected void GenerateCppImpl()
        {
            foreach (var udt in this.options.Udts)
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
            WriteLine("namespace gacui_cpp");
            Begin("{");

            GenerateCppImpl();

            End("}");
        }

        public Codegen_GacUI_codegen_cpp(CodeGeneratorOptions options)
            : base(options, options.GacuiPath + "GacUI.cpp")
        {
        }
    }
}
