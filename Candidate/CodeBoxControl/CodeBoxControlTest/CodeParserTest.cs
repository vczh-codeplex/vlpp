using System;
using System.Text;
using System.Collections.Generic;
using System.Linq;
using Microsoft.VisualStudio.TestTools.UnitTesting;
using CodeBoxControl.CodeProvider;

namespace CodeBoxControlTest
{
    [TestClass]
    public class CodeParserTest
    {
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
    }

    public abstract class Expression : CodeNode
    {
    }

    public abstract class NumberExpression : Expression
    {
        public double Number { get; set; }
    }

    public abstract class BinaryExpression : Expression
    {
        public abstract Expression Left { get; set; }
        public abstract Expression Right { get; set; }
    }

    public abstract class AddExpression : BinaryExpression
    {
    }

    public abstract class SubExpression : BinaryExpression
    {
    }

    public abstract class MulExpression : BinaryExpression
    {
    }

    public abstract class DivExpression : BinaryExpression
    {
    }

    public abstract class FunctionExpression : Expression
    {
        public string Name { get; set; }
        public abstract CodeNodeList<Expression> Parameters { get; set; }
    }
}
