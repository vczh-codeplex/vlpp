using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using FvCalculation.OperatorExpressions;
using FvCalculation.PrimitiveExpressions;

namespace FvCalculation
{
    unsafe static class ExpressionParser
    {
        private static void SkipSpaces(char** input)
        {
            while (char.IsWhiteSpace(**input))
            {
                (*input)++;
            }
        }

        private static bool Char(char** input, char c)
        {
            SkipSpaces(input);
            if (**input == c)
            {
                (*input)++;
                return true;
            }
            else
            {
                return false;
            }
        }

        private static double Number(char** input)
        {
            SkipSpaces(input);
            bool dotted = false;
            string s = "";
            while (true)
            {
                if ('0' <= **input && **input <= '9')
                {
                    s += **input;
                    (*input)++;
                }
                else if ('.' == **input && !dotted)
                {
                    dotted = true;
                    s += **input;
                    (*input)++;
                }
                else
                {
                    break;
                }
            }
            if (s == "")
            {
                return double.NaN;
            }
            else
            {
                return double.Parse(s);
            }
        }

        private static string Name(char** input)
        {
            SkipSpaces(input);
            string s = "";
            while (true)
            {
                if (('a' <= **input && **input <= 'z') || ('A' <= **input && **input <= 'Z') || ('_' == **input) || (s != "" && '0' <= **input && **input <= '9'))
                {
                    s += **input;
                    (*input)++;
                }
                else
                {
                    break;
                }
            }
            return s == "" ? null : s;
        }

        private static Expression Exp0(char** input)
        {
            if (Char(input, '('))
            {
                Expression e = Exp3(input);
                if (!Char(input, ')'))
                {
                    throw new ArgumentException("Error encountered, at " + new string(*input));
                }
                return e;
            }
            else if (Char(input, '-'))
            {
                return new NegExpression
                {
                    Op = Exp0(input),
                };
            }
            else
            {
                double number = Number(input);
                if (!double.IsNaN(number))
                {
                    return new NumberExpression
                    {
                        Number = number,
                    };
                }

                string name = Name(input);
                if (name == null)
                {
                    throw new ArgumentException("Error encountered, at " + new string(*input));
                }

                if (!Char(input, '('))
                {
                    return new VariableExpression
                    {
                        Name = name,
                    };
                }

                FunctionExpression f = FunctionExpression.FromName(name);
                f.Op = Exp3(input);
                if (!Char(input, ')'))
                {
                    throw new ArgumentException("Error encountered, at " + new string(*input));
                }
                return f;
            }
        }

        private static Expression Exp1(char** input)
        {
            Expression e = Exp0(input);
            while (true)
            {
                if (Char(input, '^'))
                {
                    e = new PowerExpression
                    {
                        Left = e,
                        Right = Exp0(input),
                    };
                }
                else
                {
                    break;
                }
            }
            return e;
        }

        private static Expression Exp2(char** input)
        {
            Expression e = Exp1(input);
            while (true)
            {
                if (Char(input, '*'))
                {
                    e = new MulExpression
                    {
                        Left = e,
                        Right = Exp1(input),
                    };
                }
                else if (Char(input, '/'))
                {
                    e = new DivExpression
                    {
                        Left = e,
                        Right = Exp1(input),
                    };
                }
                else
                {
                    break;
                }
            }
            return e;
        }

        private static Expression Exp3(char** input)
        {
            Expression e = Exp2(input);
            while (true)
            {
                if (Char(input, '+'))
                {
                    e = new AddExpression
                    {
                        Left = e,
                        Right = Exp2(input),
                    };
                }
                else if (Char(input, '-'))
                {
                    e = new SubExpression
                    {
                        Left = e,
                        Right = Exp2(input),
                    };
                }
                else
                {
                    break;
                }
            }
            return e;
        }

        private static Expression UnsafeParse(char* input)
        {
            Expression result = Exp3(&input);
            if ((int)*input == 0)
            {
                return result;
            }
            else
            {
                throw new ArgumentException("Expression contains unparsed tail, at " + new string(input));
            }
        }

        public static Expression Parse(string s)
        {
            fixed (char* input = s.Trim())
            {
                return UnsafeParse(input);
            }
        }
    }
}
