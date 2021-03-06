﻿using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using Developer.LanguageProvider;

namespace Developer.LanguageServices.NativeX.SyntaxTree
{
    public abstract class NativeXStatement : NativeXNode
    {
    }

    public abstract class NativeXEmptyStatement : NativeXStatement
    {
    }

    public abstract class NativeXCompositeStatement : NativeXStatement
    {
        public abstract CodeNodeList<NativeXStatement> Statements { get; set; }

        protected override bool ContainScope
        {
            get
            {
                return true;
            }
        }
    }

    public abstract class NativeXExpressionStatement : NativeXStatement
    {
        public abstract NativeXExpression Expression { get; set; }
    }

    public abstract class NativeXVariableStatement : NativeXStatement
    {
        public abstract NativeXType Type { get; set; }
        public string Name { get; set; }
        public abstract NativeXExpression Initializer { get; set; }

        protected override void FillScope(CodeNodeCollection nodes)
        {
            base.FillScope(nodes);
            nodes.AddIfNotExists(this.Name, this);
        }
    }

    public abstract class NativeXIfStatement : NativeXStatement
    {
        public abstract NativeXExpression Condition { get; set; }
        public abstract NativeXStatement TrueStatement { get; set; }
        public abstract NativeXStatement FalseStatement { get; set; }
    }

    public abstract class NativeXWhileStatement : NativeXStatement
    {
        public abstract NativeXExpression BeginCondition { get; set; }
        public abstract NativeXExpression EndCondition { get; set; }
        public abstract NativeXStatement Statement { get; set; }
    }

    public abstract class NativeXForStatement : NativeXStatement
    {
        public abstract CodeNodeList<NativeXStatement> Initializer { get; set; }
        public abstract CodeNodeList<NativeXStatement> SideEffect { get; set; }
        public abstract NativeXStatement Statement { get; set; }
        public abstract NativeXExpression Condition { get; set; }

        protected override bool ContainScope
        {
            get
            {
                return true;
            }
        }
    }

    public abstract class NativeXBreakStatement : NativeXStatement
    {
    }

    public abstract class NativeXContinueStatement : NativeXStatement
    {
    }

    public abstract class NativeXReturnStatement : NativeXStatement
    {
    }

    public abstract class NativeXTryCatchStatement : NativeXStatement
    {
        public abstract NativeXStatement TryStatement { get; set; }
        public abstract NativeXStatement CatchStatement { get; set; }
    }

    public abstract class NativeXThrowStatement : NativeXStatement
    {
        public abstract NativeXExpression ExceptionExpression { get; set; }
    }
}
