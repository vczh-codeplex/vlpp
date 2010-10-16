using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using CodeBoxControl.CodeProvider.ParserCodeGenerator;
using CodeBoxControlTest.CodeParser;

namespace ParserBuilder
{
    class CodeParserTestParserBuilder : Parser
    {
        public static string Build()
        {
            const int ID = CodeParserTokenizer.IdToken;
            const int Number = CodeParserTokenizer.NumberToken;
            var FUNCTION = rule<FunctionExpression>("Function");
            var NUMBER = rule<NumberExpression>("Number");
            var BRACKET = rule<Expression>("Bracket");
            var FACTOR = rule<Expression>("Factor");
            var TERM = rule<Expression>("Term");
            var EXPRESSION = rule<Expression>("Expression");

            FUNCTION.Infer(
              tok(ID)["Name"] + tok("(") + list<Expression>(tok(","), EXPRESSION)["Parameters"] + tok(")")
            );

            NUMBER.Infer(
              tok(Number)["Number"]
            );

            BRACKET.Infer(
              tok(ID) + ret(EXPRESSION) + tok(ID)
            );

            FACTOR.Infer(
              ret(NUMBER) | ret(FUNCTION) | ret(BRACKET)
            );

            TERM.Infer(
              ret(leftrec<BinaryExpression>(FACTOR["Left"], (tok("*")["Operator"] | tok("/")["Operator"]) + FACTOR["Right"]))
            );

            EXPRESSION.Infer(
              ret(leftrec<BinaryExpression>(TERM["Left"], (tok("+")["Operator"] | tok("-")["Operator"]) + TERM["Right"]))
            );

            return ParserGenerator.GenerateCSharpCode(EXPRESSION, "CodeBoxControlTest.CodeParser", "CodeParserAnalyzer");
        }
    }
}
