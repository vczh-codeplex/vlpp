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
            var EXP1 = rule<NativeXExpression>("EXP1");
            var EXPRESSION = rule<NativeXExpression>("Expression");

            var REFERENCE_TYPE = rule<NativeXReferenceType>("ReferenceType");
            var FUNCTION_TYPE = rule<NativeXFunctionType>("FunctionType");
            var INSTANCIATED_TYPE = rule<NativeXInstanciatedType>("InstanciatedType");
            var PRIMITIVE_TYPE = rule<NativeXType>("PrimitiveType");
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

                EXP1.Infer(
                    ret(leftrecg(
                            EXP0,
                            g<NativeXSubscribeExpression>("Operand", tok("[") + EXPRESSION["Subscribe"] + tok("]")),
                            g<NativeXInvokeExpression>("Function", tok("(") + list<NativeXExpression>(tok(","), EXPRESSION)["Arguments"] + tok(")")),
                            g<NativeXMemberExpression>("Operand", tok(".") + ID["MemberName"]),
                            g<NativeXPointerMemberExpression>("Operand", tok("->") + ID["MemberName"]),
                            g<NativeXUnaryExpression>("Operand", tok("++")["Operator"] | tok("--")["Operator"])
                        ))
                    );

                EXPRESSION.Infer(
                    ret(EXP1)
                    );
            }
            {
                REFERENCE_TYPE.Infer(
                    ID["ReferencedName"]
                    );

                FUNCTION_TYPE.Infer(
                    tok("function") + TYPE["ReturnType"] + tok("(") + list<NativeXType>(tok(","), TYPE)["Parameters"] + tok(")")
                    );

                INSTANCIATED_TYPE.Infer(
                    REFERENCE_TYPE["ElementType"] + tok("<") + list<NativeXType>(tok(","), TYPE)["GenericArguments"] + tok(">")
                    );

                PRIMITIVE_TYPE.Infer(
                    ret(FUNCTION_TYPE) | ret(INSTANCIATED_TYPE) | ret(REFERENCE_TYPE)
                    );

                TYPE.Infer(
                    ret(leftrecg(
                            PRIMITIVE_TYPE,
                            g<NativeXPointerType>("ElementType", tok("*")),
                            g<NativeXArrayType>("ElementType", tok("[") + PRIMITIVE["Size"] + tok("]"))
                        ))
                    );
            }

            return ParserGenerator.GenerateCSharpCode(EXPRESSION, "CodeForm.NativeX", "NativeXCodeParser");
        }
    }
}
