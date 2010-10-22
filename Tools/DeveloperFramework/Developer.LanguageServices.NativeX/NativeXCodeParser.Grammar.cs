﻿using System;
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

            var EMPTY_STATEMENT = rule<NativeXEmptyStatement>("EmptyStatement");
            var EXPRESSION_STATEMENT = rule<NativeXExpressionStatement>("ExpressionStatement");
            var VARIABLE_STATEMENT = rule<NativeXVariableStatement>("VariableStatement");
            var IF_STATEMENT = rule<NativeXIfStatement>("IfStatement");
            var BREAK_STATEMENT = rule<NativeXBreakStatement>("BreakStatement");
            var CONTINUE_STATEMENT = rule<NativeXContinueStatement>("ContinueStatement");
            var EXIT_STATEMENT = rule<NativeXReturnStatement>("ExitStatement");
            var COMPOSITE_STATEMENT = rule<NativeXCompositeStatement>("CompositeStatement");
            var DO_WHILE_STATEMENT = rule<NativeXWhileStatement>("DoWhileStatement");
            var LOOP_STATEMENT = rule<NativeXWhileStatement>("LoopStatement");
            var WHILE_DO_STATEMENT = rule<NativeXWhileStatement>("WhileDoStatement");
            var FOR_STATEMENT = rule<NativeXForStatement>("ForStatement");
            var TRY_CATCH_STATEMENT = rule<NativeXTryCatchStatement>("TryCatchStatement");
            var THROW_STATEMENT = rule<NativeXThrowStatement>("ThrowStatement");
            var STATEMENT = rule<NativeXStatement>("Statement");

            var NAME_TYPE_PAIR = rule<NativeXNameTypePair>("NameTypePair");
            var NAME_EXPRESSION_PAIR = rule<NativeXNameExpressionPair>("NameExpressionPair");
            var GENERIC_PARAMETER = rule<NativeXGenericParameter>("GenericParameter");
            var GENERIC_CONSTRAINT = rule<NativeXGenericConstraint>("GenericConstraint");
            var LINKING = rule<NativeXLinking>("Linking");
            var FUNCTION_DECLARATION = rule<NativeXFunctionDeclaration>("FunctionDeclaration");
            var GENERIC_DECLARATION = rule<NativeXDeclaration>("GenericDeclaration");
            var NON_GENERIC_DECLARATION = rule<NativeXDeclaration>("NonGenericDeclaration");
            var DECLARATION = rule<NativeXDeclaration>("Declaration");

            {
                NAME_TYPE_PAIR.Infer(
                    TYPE["Type"] + ID["Name"]
                    );

                NAME_EXPRESSION_PAIR.Infer(
                    ID["Name"] + tok("=") + EXPRESSION["Expression"]
                    );

                GENERIC_PARAMETER.Infer(
                    ID["ParameterName"]
                    );

                GENERIC_CONSTRAINT.Infer(
                    ID["ParameterName"] + tok(":") + ID["ConceptName"]
                    );

                LINKING.Infer(
                    tok("alias") + ID["LinkingAssembly"] + tok(".") + ID["LinkingSymbol"]
                    );
            }
            {
                FUNCTION_DECLARATION.Infer(
                    tok("foreign")["Foreign", "true"]
                    + tok("function") + TYPE["ReturnType"] + ID["Name"] + tok("(") + list<NativeXNameTypePair>(tok(","), NAME_TYPE_PAIR)["Parameters"] + tok(")")
                    + opt(LINKING["Linking"])
                    + (STATEMENT["Statement"] | tok(";"))
                    );
            }
            {
                NON_GENERIC_DECLARATION.Infer(
                    ret(FUNCTION_DECLARATION)
                    );

                GENERIC_DECLARATION.Infer(
                    tok("generic") + tok("<") + list<NativeXGenericParameter>(tok(","), GENERIC_PARAMETER)["GenericParameters"] + tok(">")
                    + opt(tok("where") + list<NativeXGenericConstraint>(tok(","), GENERIC_CONSTRAINT)["GenericConstraints"])
                    + ret(NON_GENERIC_DECLARATION)
                    );

                DECLARATION.Infer(
                    ret(GENERIC_DECLARATION) | ret(NON_GENERIC_DECLARATION)
                    );
            }
            {
                EMPTY_STATEMENT.Infer(
                    tok(";")
                    );

                EXPRESSION_STATEMENT.Infer(
                    EXPRESSION["Expression"] + tok(";")
                    );

                VARIABLE_STATEMENT.Infer(
                    tok("variable") + TYPE["Type"] + ID["Name"] + opt(tok("=") + EXPRESSION["Initializer"]) + tok(";")
                    );

                IF_STATEMENT.Infer(
                    tok("if") + tok("(") + EXPRESSION["Condition"] + tok(")") + STATEMENT["TrueStatement"] + opt(tok("else") + STATEMENT["FalseStatement"])
                    );

                BREAK_STATEMENT.Infer(
                    tok("break") + tok(";")
                    );

                CONTINUE_STATEMENT.Infer(
                    tok("continue") + tok(";")
                    );

                EXIT_STATEMENT.Infer(
                    tok("exit") + tok(";")
                    );

                COMPOSITE_STATEMENT.Infer(
                    tok("{") + list<NativeXStatement>(STATEMENT)["Statements"] + tok("}")
                    );

                DO_WHILE_STATEMENT.Infer(
                    tok("do") + STATEMENT["Statement"] + tok("while") + tok("(") + EXPRESSION["EndCondition"] + tok(")") + tok(";")
                    );

                LOOP_STATEMENT.Infer(
                    tok("loop") + STATEMENT["Statement"]
                    );

                WHILE_DO_STATEMENT.Infer(
                    tok("while") + tok("(") + EXPRESSION["BeginCondition"] + tok(")") + STATEMENT["Statement"] + opt(tok("when") + tok("(") + EXPRESSION["EndCondition"] + tok(")") + tok(";"))
                    );

                FOR_STATEMENT.Infer(
                    tok("for") + list<NativeXStatement>(STATEMENT)["Initializer"]
                    + tok("when") + tok("(") + EXPRESSION["Condition"] + tok(")")
                    + tok("with") + list<NativeXStatement>(STATEMENT)["SideEffect"]
                    + tok("do") + STATEMENT["Statement"]
                    );

                TRY_CATCH_STATEMENT.Infer(
                    tok("try") + STATEMENT["TryStatement"] + tok("catch") + STATEMENT["CatchStatement"]
                    );

                THROW_STATEMENT.Infer(
                    tok("throw") + opt(EXPRESSION["ExceptionExpression"]) + tok(";")
                    );

                STATEMENT.Infer(
                    ret(EMPTY_STATEMENT)
                    | ret(VARIABLE_STATEMENT)
                    | ret(IF_STATEMENT)
                    | ret(BREAK_STATEMENT)
                    | ret(CONTINUE_STATEMENT)
                    | ret(EXIT_STATEMENT)
                    | ret(COMPOSITE_STATEMENT)
                    | ret(DO_WHILE_STATEMENT)
                    | ret(LOOP_STATEMENT)
                    | ret(WHILE_DO_STATEMENT)
                    | ret(FOR_STATEMENT)
                    | ret(TRY_CATCH_STATEMENT)
                    | ret(THROW_STATEMENT)
                    | ret(EXPRESSION_STATEMENT)
                    );
            }
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
                    tok("result")
                    );

                EXCEPTION.Infer(
                    tok("exception")
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

            return ParserGenerator.GenerateCSharpCode(DECLARATION, "Developer.LanguageServices.NativeX", "NativeXCodeParser");
        }
    }
}
