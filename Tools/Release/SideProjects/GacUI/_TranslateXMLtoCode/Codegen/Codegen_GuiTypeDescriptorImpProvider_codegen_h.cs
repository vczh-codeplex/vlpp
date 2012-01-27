using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;

namespace _TranslateXMLtoCode.Codegen
{
    class Codegen_GuiTypeDescriptorImpProvider_codegen_h : CodeGenerator
    {
        private CodeGeneratorOptions options;

        protected override void GenerateCodeInternal()
        {
            WriteLine("#include \"GuiTypeDescriptorImpHelper.h\"");
            WriteLine("");
            WriteLine("namespace vl");
            Begin("{");
            WriteLine("namespace presentation");
            Begin("{");
            WriteLine("namespace reflection_implementation");
            Begin("{");

            WriteLine("extern ITypeProvider* CreateDefaultTypeProvider();");

            End("}");
            End("}");
            End("}");
        }

        public Codegen_GuiTypeDescriptorImpProvider_codegen_h(CodeGeneratorOptions options)
            : base(options.ReflectionPath + "GuiTypeDescriptorImpProvider_codegen.h")
        {
            this.options = options;
        }
    }
}
