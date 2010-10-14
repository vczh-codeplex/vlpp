using System;
using System.Text;
using System.Collections.Generic;
using System.Linq;
using Microsoft.VisualStudio.TestTools.UnitTesting;
using CodeBoxControl.CodeProvider;

namespace CodeBoxControlTest
{
    [TestClass]
    public class CodeNodeTest
    {
        [TestMethod]
        public void TestCreateNode()
        {
            NumberNode a = CodeNode.Create<NumberNode>();
            a.Number = 1;
            Assert.AreEqual(0, a.NamedNodes.Count);
            Assert.IsNull(a.OwningScope);
            Assert.IsNull(a.ParentNode);
            Assert.IsNull(a.Scope);

            NumberNode b = CodeNode.Create<NumberNode>();
            b.Number = 2;
            Assert.AreEqual(0, b.NamedNodes.Count);
            Assert.IsNull(b.OwningScope);
            Assert.IsNull(b.ParentNode);
            Assert.IsNull(b.Scope);

            AddNode add = CodeNode.Create<AddNode>();
            Assert.AreEqual(0, add.NamedNodes.Count);
            Assert.IsNull(add.OwningScope);
            Assert.IsNull(add.ParentNode);
            Assert.IsNull(add.Scope);
            Assert.IsNull(add.Left);
            Assert.IsNull(add.Right);

            add.Left = a;
            add.Right = b;

            Assert.AreEqual(0, a.NamedNodes.Count);
            Assert.IsNull(a.OwningScope);
            Assert.AreEqual(add, a.ParentNode);
            Assert.IsNull(a.Scope);

            Assert.AreEqual(0, b.NamedNodes.Count);
            Assert.IsNull(b.OwningScope);
            Assert.AreEqual(add, b.ParentNode);
            Assert.IsNull(b.Scope);

            Assert.AreEqual(2, add.NamedNodes.Count);
            Assert.IsNull(add.OwningScope);
            Assert.IsNull(add.ParentNode);
            Assert.IsNull(add.Scope);
            Assert.AreEqual(a, add.Left);
            Assert.AreEqual(b, add.Right);
            Assert.AreEqual(a, add.NamedNodes["Left"]);
            Assert.AreEqual(b, add.NamedNodes["Right"]);
        }
    }

    public abstract class ExpressionNode : CodeNode
    {
    }

    public abstract class NumberNode : ExpressionNode
    {
        public int Number { get; set; }
    }

    public abstract class AddNode : ExpressionNode
    {
        public abstract ExpressionNode Left { get; set; }
        public abstract ExpressionNode Right { get; set; }
    }
}
