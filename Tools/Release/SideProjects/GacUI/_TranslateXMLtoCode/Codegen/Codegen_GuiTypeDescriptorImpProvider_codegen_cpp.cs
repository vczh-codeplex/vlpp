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

        protected override void GenerateCodeInternal()
        {
            WriteLine("#include \"GuiTypeDescriptorImpProvider_codegen.h\"");
            WriteLine("");
            WriteLine("namespace vl");
            Begin("{");
            WriteLine("namespace presentation");
            Begin("{");
            WriteLine("namespace reflection_implementation");
            Begin("{");

            foreach (var udt in this.options.Udts)
            {
            }

            WriteSectionComment("Helper Functions");
            WriteLine("ITypeProvider* CreateDefaultTypeProvider()");
            Begin("{");
            WriteLine("return 0;");
            End("}");

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
