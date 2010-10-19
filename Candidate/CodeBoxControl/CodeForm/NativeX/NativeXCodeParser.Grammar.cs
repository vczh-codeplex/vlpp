using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using CodeBoxControl.CodeProvider.ParserCodeGenerator;
using CodeForm.NativeX.SyntaxTree;

namespace CodeForm.NativeX
{
    public class NativeXCodeParserGrammar : Parser
    {
        public static string Build()
        {
            var ID = id("CodeForm.NativeX.NativeXTokenizer.IdToken");
            var STRING = id("CodeForm.NativeX.NativeXTokenizer.StringToken");
            var NUMBER = id("CodeForm.NativeX.NativeXTokenizer.NumberToken");

            var PRIMITIVE = rule<NativeXPrimitiveExpression>("Primitive");
            var ANYNODE = rule<NativeXNode>("NativeXAnyNode");

            PRIMITIVE.Infer(
                STRING["Code"] | NUMBER["Code"] | tok("true")["Code"] | tok("false")["Code"] | tok("null")["Code"] | tok("exception")["Code"]
                );

            ANYNODE.Infer(
                ret(PRIMITIVE)
                );

            return ParserGenerator.GenerateCSharpCode(ANYNODE, "CodeForm.NativeX", "NativeXCodeParser");
        }
    }
}
