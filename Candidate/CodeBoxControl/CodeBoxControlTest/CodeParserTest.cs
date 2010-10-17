using System;
using System.Text;
using System.Collections.Generic;
using System.Linq;
using Microsoft.VisualStudio.TestTools.UnitTesting;
using CodeBoxControl.CodeProvider;
using CodeBoxControlTest.CodeParser;

namespace CodeBoxControlTest
{
    [TestClass]
    public class CodeParserTest
    {
        private Expression Parse(string input)
        {
            List<CodeToken> tokens = new CodeParserTokenizer().Tokenize(input.ToCharArray());
            int currentToken = 0;
            bool parseSuccess = false;
            Expression result = CodeParserAnalyzer.ParseExpression(tokens, ref currentToken, ref parseSuccess);
            if (parseSuccess && currentToken == tokens.Count && result != null)
            {
                return result;
            }
            else
            {
                throw new ArgumentException("Parse error occured.");
            }
        }

        [TestMethod]
        public void TestExpressions()
        {
            // pow((1+2)*(3-4), 3)
            NumberExpression n1 = CodeNode.Create<NumberExpression>();
            n1.Number = 1;
            NumberExpression n2 = CodeNode.Create<NumberExpression>();
            n1.Number = 2;
            NumberExpression n3 = CodeNode.Create<NumberExpression>();
            n1.Number = 3;
            NumberExpression n4 = CodeNode.Create<NumberExpression>();
            n1.Number = 4;
            NumberExpression n5 = CodeNode.Create<NumberExpression>();
            n1.Number = 3;

            AddExpression addExpr = CodeNode.Create<AddExpression>();
            addExpr.Left = n1;
            addExpr.Right = n2;

            SubExpression subExpr = CodeNode.Create<SubExpression>();
            subExpr.Left = n3;
            subExpr.Right = n4;

            MulExpression mulExpr = CodeNode.Create<MulExpression>();
            mulExpr.Left = addExpr;
            mulExpr.Right = subExpr;

            FunctionExpression powExpr = CodeNode.Create<FunctionExpression>();
            powExpr.Name = "pow";
            powExpr.Parameters = new CodeNodeList<Expression>();
            powExpr.Parameters.Add(mulExpr);
            powExpr.Parameters.Add(n5);

            Assert.AreEqual(n1, addExpr.NamedNodes["Left"]);
            Assert.AreEqual(n2, addExpr.NamedNodes["Right"]);
            Assert.IsTrue(addExpr.Nodes.SequenceEqual(new CodeNode[] { n1, n2 }));
            Assert.AreEqual(n3, subExpr.NamedNodes["Left"]);
            Assert.AreEqual(n4, subExpr.NamedNodes["Right"]);
            Assert.IsTrue(subExpr.Nodes.SequenceEqual(new CodeNode[] { n3, n4 }));
            Assert.AreEqual(addExpr, mulExpr.NamedNodes["Left"]);
            Assert.AreEqual(subExpr, mulExpr.NamedNodes["Right"]);
            Assert.IsTrue(mulExpr.Nodes.SequenceEqual(new CodeNode[] { addExpr, subExpr }));
            Assert.AreEqual(powExpr.Parameters, powExpr.NamedNodes["Parameters"]);
            Assert.IsTrue(powExpr.Nodes.SequenceEqual(new CodeNode[] { powExpr.Parameters }));
            Assert.IsTrue(powExpr.Parameters.Nodes.SequenceEqual(new CodeNode[] { mulExpr, n5 }));
        }

        [TestMethod]
        public void TestParseNumber()
        {
            NumberExpression numberExpression = (NumberExpression)Parse("123456");
            Assert.AreEqual(123456, numberExpression.Number);
        }

        [TestMethod]
        public void TestParseMuldiv()
        {
            BinaryExpression a = (BinaryExpression)Parse("123*456");
            Assert.AreEqual("*", a.Operator);
            NumberExpression b = (NumberExpression)a.Left;
            Assert.AreEqual(123, b.Number);
            NumberExpression c = (NumberExpression)a.Right;
            Assert.AreEqual(456, c.Number);
        }

        [TestMethod]
        public void TestParseMuldiv2()
        {
            BinaryExpression a = (BinaryExpression)Parse("123*456/789");
            Assert.AreEqual("/", a.Operator);
            BinaryExpression b = (BinaryExpression)a.Left;
            Assert.AreEqual("*", b.Operator);
            NumberExpression c = (NumberExpression)b.Left;
            Assert.AreEqual(123, c.Number);
            NumberExpression d = (NumberExpression)b.Right;
            Assert.AreEqual(456, d.Number);
            NumberExpression e = (NumberExpression)a.Right;
            Assert.AreEqual(789, e.Number);
        }

        [TestMethod]
        public void TestAddMul()
        {
            BinaryExpression a = (BinaryExpression)Parse("1*2+3*4");
            Assert.AreEqual("+", a.Operator);

            BinaryExpression b = (BinaryExpression)a.Left;
            Assert.AreEqual("*", b.Operator);
            Assert.AreEqual(1, ((NumberExpression)b.Left).Number);
            Assert.AreEqual(2, ((NumberExpression)b.Right).Number);

            BinaryExpression c = (BinaryExpression)a.Right;
            Assert.AreEqual("*", c.Operator);
            Assert.AreEqual(3, ((NumberExpression)c.Left).Number);
            Assert.AreEqual(4, ((NumberExpression)c.Right).Number);
        }

        [TestMethod]
        public void TestAddMul2()
        {
            BinaryExpression a = (BinaryExpression)Parse("(1+2)*(3+4)");
            Assert.AreEqual("*", a.Operator);

            BinaryExpression b = (BinaryExpression)a.Left;
            Assert.AreEqual("+", b.Operator);
            Assert.AreEqual(1, ((NumberExpression)b.Left).Number);
            Assert.AreEqual(2, ((NumberExpression)b.Right).Number);

            BinaryExpression c = (BinaryExpression)a.Right;
            Assert.AreEqual("+", c.Operator);
            Assert.AreEqual(3, ((NumberExpression)c.Left).Number);
            Assert.AreEqual(4, ((NumberExpression)c.Right).Number);
        }

        [TestMethod]
        public void TestFunction1()
        {
            FunctionExpression a = (FunctionExpression)Parse("exp()");
            Assert.AreEqual("exp", a.Name);
            Assert.AreEqual(0, a.Parameters.Count);
        }

        [TestMethod]
        public void TestFunction2()
        {
            FunctionExpression a = (FunctionExpression)Parse("exp(1+2)");
            Assert.AreEqual("exp", a.Name);
            Assert.AreEqual(1, a.Parameters.Count);

            BinaryExpression b = (BinaryExpression)a.Parameters[0];
            Assert.AreEqual("+", b.Operator);
            Assert.AreEqual(1, ((NumberExpression)b.Left).Number);
            Assert.AreEqual(2, ((NumberExpression)b.Right).Number);
        }

        [TestMethod]
        public void TestFunction3()
        {
            FunctionExpression a = (FunctionExpression)Parse("exp(1+2,3/4)");
            Assert.AreEqual("exp", a.Name);
            Assert.AreEqual(2, a.Parameters.Count);

            BinaryExpression b = (BinaryExpression)a.Parameters[0];
            Assert.AreEqual("+", b.Operator);
            Assert.AreEqual(1, ((NumberExpression)b.Left).Number);
            Assert.AreEqual(2, ((NumberExpression)b.Right).Number);

            BinaryExpression c = (BinaryExpression)a.Parameters[1];
            Assert.AreEqual("/", c.Operator);
            Assert.AreEqual(3, ((NumberExpression)c.Left).Number);
            Assert.AreEqual(4, ((NumberExpression)c.Right).Number);
        }

        [TestMethod]
        public void TestComplex()
        {
            string input = "sin(1+2)*cos(3/4)-567-890";
            BinaryExpression a = (BinaryExpression)Parse(input);
            Assert.AreEqual("-", a.Operator);
            {
                BinaryExpression b = (BinaryExpression)a.Left;
                Assert.AreEqual("-", b.Operator);
                {
                    BinaryExpression c = (BinaryExpression)b.Left;
                    Assert.AreEqual("*", c.Operator);
                    {
                        FunctionExpression d = (FunctionExpression)c.Left;
                        Assert.AreEqual("sin", d.Name);
                        Assert.AreEqual(1, d.Parameters.Count);
                        {
                            BinaryExpression e = (BinaryExpression)d.Parameters[0];
                            Assert.AreEqual("+", e.Operator);
                            {
                                NumberExpression f = (NumberExpression)e.Left;
                                Assert.AreEqual(1, f.Number);
                            }
                            {
                                NumberExpression f = (NumberExpression)e.Right;
                                Assert.AreEqual(2, f.Number);
                            }
                        }
                    }
                    {
                        FunctionExpression d = (FunctionExpression)c.Right;
                        Assert.AreEqual("cos", d.Name);
                        Assert.AreEqual(1, d.Parameters.Count);
                        {
                            BinaryExpression e = (BinaryExpression)d.Parameters[0];
                            Assert.AreEqual("/", e.Operator);
                            {
                                NumberExpression f = (NumberExpression)e.Left;
                                Assert.AreEqual(3, f.Number);
                            }
                            {
                                NumberExpression f = (NumberExpression)e.Right;
                                Assert.AreEqual(4, f.Number);
                            }
                        }
                    }
                }
                {
                    NumberExpression c = (NumberExpression)b.Right;
                    Assert.AreEqual(567, c.Number);
                }
            }
            {
                NumberExpression b = (NumberExpression)a.Right;
                Assert.AreEqual(890, b.Number);
            }
        }
    }
}
