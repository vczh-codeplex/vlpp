using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using CodeBoxControl.CodeProvider;

namespace CodeForm.NativeX.SyntaxTree
{
    public abstract class NativeXExpression : NativeXNode
    {
    }

    public abstract class NativeXPrimitiveExpression : NativeXExpression
    {
        public string Code { get; set; }
    }

    public abstract class NativeXUnaryExpression : NativeXExpression
    {
        public string Operator { get; set; }
        public abstract NativeXExpression Operand { get; set; }
    }

    public abstract class NativeXPostUnaryExpression : NativeXExpression
    {
        public string Operator { get; set; }
        public abstract NativeXExpression Operand { get; set; }
    }

    public abstract class NativeXBinaryExpression : NativeXExpression
    {
        public string Operator { get; set; }
        public abstract NativeXExpression LeftOperand { get; set; }
        public abstract NativeXExpression RightOperand { get; set; }
    }

    public abstract class NativeXSubscribeExpression : NativeXExpression
    {
        public abstract NativeXExpression Operand { get; set; }
        public abstract NativeXExpression Subscribe { get; set; }
    }

    public abstract class NativeXMemberExpression : NativeXExpression
    {
        public abstract NativeXExpression Operand { get; set; }
        public string MemberName { get; set; }
    }

    public abstract class NativeXPointerMemberExpression : NativeXExpression
    {
        public abstract NativeXExpression Operand { get; set; }
        public string MemberName { get; set; }
    }

    public abstract class NativeXInvokeExpression : NativeXExpression
    {
        public abstract NativeXExpression Function { get; set; }
        public abstract CodeNodeList<NativeXExpression> Arguments { get; set; }
    }

    public abstract class NativeXFunctionResultExpression : NativeXExpression
    {
        public string Code { get; set; }
    }

    public abstract class NativeXExceptionExpression : NativeXExpression
    {
        public string Code { get; set; }
    }

    public abstract class NativeXCastingExpression : NativeXExpression
    {
        public abstract NativeXType Type { get; set; }
        public abstract NativeXExpression Operand { get; set; }
    }

    public abstract class NativeXReferenceExpression : NativeXExpression
    {
        public string ReferencedName { get; set; }
    }

    public abstract class NativeXInstanciatedExpression : NativeXExpression
    {
        public string ReferencedName { get; set; }
        public abstract CodeNodeList<NativeXType> GenericArguments { get; set; }
    }

    public abstract class NativeXInstanceFunctionExpression : NativeXExpression
    {
        public string ConceptName { get; set; }
        public string FunctionName { get; set; }
        public abstract NativeXType Type { get; set; }
    }
}
