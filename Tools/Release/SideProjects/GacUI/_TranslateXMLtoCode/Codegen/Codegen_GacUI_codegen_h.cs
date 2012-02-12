using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;

namespace _TranslateXMLtoCode.Codegen
{
    class Codegen_GacUI_codegen_h : CodeGenerator
    {
        private CodeGeneratorOptions options;

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
                        Begin("public:");
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
            : base(options.GacuiPath + "GacUI.h")
        {
            this.options = options;
        }
    }
}
