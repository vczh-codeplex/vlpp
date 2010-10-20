using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using Developer.LanguageServices.NativeX.SyntaxTree;
using Developer.LanguageProvider.ParserCodeGenerator;

namespace Developer.LanguageServices.NativeX
{
    public class NativeXCodeParserGrammar : Parser
    {
        public static string Build()
        {
            var ID = id("Developer.LanguageServices.NativeX.NativeXTokenizer.IdToken");
            var STRING = id("Developer.LanguageServices.NativeX.NativeXTokenizer.StringToken");
            var NUMBER = id("Developer.LanguageServices.NativeX.NativeXTokenizer.NumberToken");

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
            var UNARY = rule<NativeXUnaryExpression>("Unary");
            var EXP2 = rule<NativeXExpression>("EXP2");
            var EXP_BINS = Enumerable.Range(3, 11).Select(i => rule<NativeXExpression>("EXP" + i.ToString())).ToArray();
            var EXPRESSION = rule<NativeXExpression>("Expression");

            var REFERENCE_TYPE = rule<NativeXReferenceType>("ReferenceType");
            var FUNCTION_TYPE = rule<NativeXFunctionType>("FunctionType");
            var INSTANCIATED_TYPE = rule<NativeXInstanciatedType>("InstanciatedType");
            var PRIMITIVE_TYPE = rule<NativeXType>("PrimitiveType");
            var TYPE = rule<NativeXType>("Type");

            {
                PRIMITIVE.Infer(
                    STRING["Code"] | NUMBER["Code"] | toks("true", "false", "null")["Code"]
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
                            g<NativeXPostUnaryExpression>("Operand", toks("++", "--")["Operator"])
                        ))
                    );

                UNARY.Infer(
                    ((toks("++", "--", "&", "*", "-", "!", "~")["Operator"])) + EXP2["Operand"]
                    );

                EXP2.Infer(
                    ret(EXP1) | ret(UNARY)
                    );

                {
                    string[][] binaryOperators = new string[][]{
                        new string[]{"*", "/", "%"},
                        new string[]{"+", "-"},
                        new string[]{"<<", ">>"},
                        new string[]{"<", "<=", ">", ">="},
                        new string[]{"==", "!="},
                        new string[]{"&"},
                        new string[]{"^"},
                        new string[]{"|"},
                        new string[]{"&&"},
                        new string[]{"||"},
                        new string[]{"+=", "-=", "*=", "/=", "%=", "<<=", ">>=", "&=", "/=", "&&=", "||=", "="}
                    };
                    ParserNode[] operatorNodes = binaryOperators
                        .Select(ops => toks(ops)["Operator"])
                        .ToArray();
                    ParserNode[] previousNode = new ParserNode[] { EXP2 }
                        .Concat(EXP_BINS.Take(EXP_BINS.Length - 1))
                        .ToArray();
                    for (int i = 0; i < EXP_BINS.Length; i++)
                    {
                        EXP_BINS[i].Infer(
                            ret(leftrec<NativeXBinaryExpression>(previousNode[i]["LeftOperand"], operatorNodes[i] + previousNode[i]["RightOperand"]))
                            );
                    }
                }

                EXPRESSION.Infer(
                    ret(EXP_BINS.Last())
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

            return ParserGenerator.GenerateCSharpCode(EXPRESSION, "Developer.LanguageServices.NativeX", "NativeXCodeParser");
        }
    }
}
