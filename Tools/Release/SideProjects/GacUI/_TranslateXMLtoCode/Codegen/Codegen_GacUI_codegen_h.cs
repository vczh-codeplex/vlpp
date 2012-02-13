﻿using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;

namespace _TranslateXMLtoCode.Codegen
{
    class Codegen_GacUI_codegen_h : Codegen_GacUI_base
    {
        #region GenerateMembers

        protected void GenerateConstructor(RgacMethod method)
        {
            WriteLine("static rptr<{0}> Create({1});",
                GacUdtTypeName(method.OwnerUDT),
                method.ParameterTypes
                    .Zip(method.ParameterNames, Tuple.Create)
                    .Select(t => string.Format("{0} {1}", GetType(t.Item1), t.Item2))
                    .Aggregate("", (a, b) => a == "" ? b : a + ", " + b)
                );

            if (method.OwnerUDT.Kind == RgacUDTKind.Struct)
            {
                WriteLine("{0}({1});",
                    method.OwnerUDT.Name.Last(),
                    method.ParameterTypes
                        .Zip(method.ParameterNames, Tuple.Create)
                        .Select(t => string.Format("{0} {1}", GetType(t.Item1), t.Item2))
                        .Aggregate("", (a, b) => a == "" ? b : a + ", " + b)
                    );
            }
        }

        protected void GenerateMethod(RgacMethod method, bool isStatic)
        {
            WriteLine("{0}{1} {2}({3});",
                (isStatic ? "static " : ""),
                GetType(method.ReturnType),
                method.Name,
                method.ParameterTypes
                    .Zip(method.ParameterNames, Tuple.Create)
                    .Select(t => string.Format("{0} {1}", GetType(t.Item1), t.Item2))
                    .Aggregate("", (a, b) => a == "" ? b : a + ", " + b)
                );
        }

        protected void GenerateProperty(RgacProperty property, bool isStatic)
        {
            if (property.Getter != null)
                GenerateMethod(property.Getter, isStatic);
            if (property.Setter != null)
                GenerateMethod(property.Setter, isStatic);
            if (property.PublicGacFieldAccessor != null)
            {
                WriteLine("{0}{1} get_{2}();",
                    (isStatic ? "static " : ""),
                    GetType(property.PropertyType),
                    property.Name
                    );
                if (!property.IsEventField)
                {
                    WriteLine("{0}void set_{1}({2} value);",
                        (isStatic ? "static " : ""),
                        property.Name,
                        GetType(property.PropertyType)
                        );
                }
            }
        }

        protected void GenerateMembers(RgacUDT udt)
        {
            Begin("protected:");
            End("");
            Begin("public:");

            if (udt.Constructors.Length > 0 && !udt.IsAbstract)
            {
                foreach (var m in udt.Constructors)
                    GenerateConstructor(m);
                WriteLine("");
            }

            if (udt.Methods.Length > 0)
            {
                foreach (var m in udt.Methods)
                    GenerateMethod(m, false);
                WriteLine("");
            }

            if (udt.StaticMethods.Length > 0)
            {
                foreach (var m in udt.StaticMethods)
                    GenerateMethod(m, true);
                WriteLine("");
            }

            if (udt.Properties.Length > 0)
            {
                foreach (var m in udt.Properties)
                    GenerateProperty(m, false);
                WriteLine("");
            }

            if (udt.StaticProperties.Length > 0)
            {
                foreach (var m in udt.StaticProperties)
                    GenerateProperty(m, true);
                WriteLine("");
            }
        }

        #endregion

        protected void GenerateCppHeader()
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
                    WriteLine("");
                    classNames.RemoveAt(i - 1);
                }

                WriteSectionComment(string.Format("{0}", udt.ToString()));
                for (int i = commonClassNames; i < udt.Name.Length; i++)
                {
                    string className = udt.Name[i];
                    classNames.Add(className);

                    if (i == udt.Name.Length - 1 && udt.Kind == RgacUDTKind.Enum)
                    {
                        WriteLine("enum {0}", className);
                        Begin("{");
                        foreach (var item in udt.AssociatedGacType.Constants)
                        {
                            WriteLine("{0} = {1},", item.Name, item.EnumItemValue);
                        }
                    }
                    else
                    {
                        WriteLine("class GACUI_API {0}", className);
                        WriteLine("{");
                        GenerateMembers(udt);
                    }
                }
            }
            for (int i = 0; i < classNames.Count; i++)
            {
                End("};");
            }
        }

        protected override void GenerateCodeInternal()
        {
            WriteLine("#ifndef GACUI_CONTROL_LIBRARY_CPP");
            WriteLine("#define GACUI_CONTROL_LIBRARY_CPP");
            WriteLine("");
            WriteLine("#include \"GacUICommon.h\"");
            WriteLine("");
            WriteLine("namespace gacui_cpp");
            Begin("{");

            GenerateCppHeader();

            End("}");
            WriteLine("");
            WriteLine("namespace g{ using namespace gacui_cpp; }");
            WriteLine("#endif");
        }

        public Codegen_GacUI_codegen_h(CodeGeneratorOptions options)
            : base(options, options.GacuiPath + "GacUI.h")
        {
        }
    }
}
