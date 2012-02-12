using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;

namespace _TranslateXMLtoCode.Codegen
{
    class Codegen_GacUI_codegen_cpp : CodeGenerator
    {
        private CodeGeneratorOptions options;

        protected void GenerateCppImpl()
        {
            foreach (var udt in this.options.Udts)
            {
                if (udt.Kind != RgacUDTKind.Enum)
                {
                    WriteSectionComment(udt.ToString());
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
            : base(options.GacuiPath + "GacUI.cpp")
        {
            this.options = options;
        }
    }
}
