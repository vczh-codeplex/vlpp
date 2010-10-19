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
            var INSTANCE_FUNCTION_REFERENCE = rule<NativeXInstanceFunctionExpression>("InstanceFunctionReference");
            var INSTANCIATED_REFERENCE = rule<NativeXInstanciatedExpression>("InstanciatedReference");
            var IDENTIFIER_REFERENCE = rule<NativeXReferenceExpression>("IdentifierReference");
            var REFERENCE = rule<NativeXExpression>("Reference");
            var EXPRESSION = rule<NativeXNode>("Expression");

            var REFERENCE_TYPE = rule<NativeXReferenceType>("ReferenceType");
            var TYPE = rule<NativeXType>("Type");

            {
                PRIMITIVE.Infer(
                    STRING["Code"] | NUMBER["Code"] | tok("true")["Code"] | tok("false")["Code"] | tok("null")["Code"] | tok("exception")["Code"]
                    );

                INSTANCE_FUNCTION_REFERENCE.Infer(
                    ID["ConceptName"] + tok("<") + TYPE["Type"] + tok(">") + tok(":") + tok(":") + ID["FunctionName"]
                    );

                INSTANCIATED_REFERENCE.Infer(
                    ID["ReferencedName"] + tok("<") + list<NativeXType>(tok(","), TYPE)["GenericArguments"] + tok(">")
                    );

                IDENTIFIER_REFERENCE.Infer(
                    ID["ReferencedName"]
                    );

                REFERENCE.Infer(
                    ret(INSTANCE_FUNCTION_REFERENCE) | ret(INSTANCIATED_REFERENCE) | ret(IDENTIFIER_REFERENCE)
                    );

                EXPRESSION.Infer(
                    ret(PRIMITIVE) | ret(REFERENCE)
                    );
            }
            {
                REFERENCE_TYPE.Infer(
                    ID["ReferencedName"]
                    );

                TYPE.Infer(
                    ret(REFERENCE_TYPE)
                    );
            }

            return ParserGenerator.GenerateCSharpCode(EXPRESSION, "CodeForm.NativeX", "NativeXCodeParser");
        }
    }
}
