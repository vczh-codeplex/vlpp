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
            var RESULT = rule<NativeXFunctionResultExpression>("Result");
            var EXCEPTION = rule<NativeXExceptionExpression>("Exception");
            var CAST = rule<NativeXCastingExpression>("Casting");
            var EXP0 = rule<NativeXExpression>("EXP0");
            var EXPRESSION = rule<NativeXExpression>("Expression");

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

                RESULT.Infer(
                    tok("result")["Code"]
                    );

                EXCEPTION.Infer(
                    tok("exception")["Code"]
                    );

                CAST.Infer(
                    tok("cast") + tok("<") + TYPE["Type"] + tok(">") + tok("(") + EXPRESSION["Operand"] + tok(")")
                    );

                EXP0.Infer(
                    ret(RESULT) | ret(EXCEPTION) | ret(CAST) | ret(PRIMITIVE) | ret(REFERENCE) | tok("(") + ret(EXPRESSION) + tok(")")
                    );

                EXPRESSION.Infer(
                    ret(EXP0)
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
