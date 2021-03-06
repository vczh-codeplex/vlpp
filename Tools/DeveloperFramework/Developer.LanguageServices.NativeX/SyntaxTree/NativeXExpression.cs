﻿using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using Developer.LanguageProvider;

namespace Developer.LanguageServices.NativeX.SyntaxTree
{
    public abstract class NativeXExpression : NativeXNode
    {
        public virtual NativeXAbstractType AbstractType
        {
            get
            {
                return null;
            }
        }

        public T GetUnwrapType<T>()
            where T : NativeXAbstractType
        {
            NativeXAbstractType type = this.AbstractType;
            return type == null ? null : type.Unwrap() as T;
        }

        public virtual Tuple<string, string, string> GetFormattedFunctionQuickInfo(int parameterIndex)
        {
            return Tuple.Create("", "", "");
        }
    }

    public abstract class NativeXPrimitiveExpression : NativeXExpression
    {
        public string Code { get; set; }
    }

    public abstract class NativeXUnaryExpression : NativeXExpression
    {
        public string Operator { get; set; }
        public abstract NativeXExpression Operand { get; set; }

        public override NativeXAbstractType AbstractType
        {
            get
            {
                if (this.Operand == null) return null;
                switch (this.Operator)
                {
                    case "*":
                        {
                            NativeXAbstractPointerType type = this.Operand.GetUnwrapType<NativeXAbstractPointerType>();
                            if (type == null) return null;
                            return type.ElementType;
                        }
                    case "&":
                        {
                            NativeXAbstractType type = this.Operand.AbstractType;
                            if (type == null) return null;
                            return new NativeXAbstractPointerType()
                            {
                                ElementType = type
                            };
                        }
                    case "++":
                    case "--":
                        return this.Operand == null ? null : this.Operand.AbstractType;
                    default:
                        return null;
                }
            }
        }
    }

    public abstract class NativeXPostUnaryExpression : NativeXExpression
    {
        public string Operator { get; set; }
        public abstract NativeXExpression Operand { get; set; }

        public override NativeXAbstractType AbstractType
        {
            get
            {
                return this.Operand == null ? null : this.Operand.AbstractType;
            }
        }
    }

    public abstract class NativeXBinaryExpression : NativeXExpression
    {
        public string Operator { get; set; }
        public abstract NativeXExpression LeftOperand { get; set; }
        public abstract NativeXExpression RightOperand { get; set; }

        public override NativeXAbstractType AbstractType
        {
            get
            {
                if (this.LeftOperand != null && this.RightOperand != null)
                {
                    NativeXAbstractPointerType t1 = this.LeftOperand.AbstractType as NativeXAbstractPointerType;
                    NativeXAbstractPointerType t2 = this.RightOperand.AbstractType as NativeXAbstractPointerType;
                    switch (this.Operator)
                    {
                        case "+":
                        case "-":
                            if (t1 != null && t2 != null)
                            {
                                return null;
                            }
                            else
                            {
                                return t1 ?? t2;
                            }
                    }
                }
                return null;
            }
        }
    }

    public abstract class NativeXSubscribeExpression : NativeXExpression
    {
        public abstract NativeXExpression Operand { get; set; }
        public abstract NativeXExpression Subscribe { get; set; }

        public override NativeXAbstractType AbstractType
        {
            get
            {
                if (this.Operand != null)
                {
                    {
                        NativeXAbstractArrayType type = this.Operand.GetUnwrapType<NativeXAbstractArrayType>();
                        if (type != null)
                        {
                            return type.ElementType;
                        }
                    }
                    {
                        NativeXAbstractPointerType type = this.Operand.GetUnwrapType<NativeXAbstractPointerType>();
                        if (type != null)
                        {
                            return type.ElementType;
                        }
                    }
                }
                return null;
            }
        }
    }

    public abstract class NativeXMemberExpression : NativeXExpression
    {
        public abstract NativeXExpression Operand { get; set; }
        public string MemberName { get; set; }

        public NativeXAbstractStructureType GetStructureType()
        {
            if (this.Operand == null) return null;
            return this.Operand.GetUnwrapType<NativeXAbstractStructureType>();
        }

        public override NativeXAbstractType AbstractType
        {
            get
            {
                if (this.MemberName == null) return null;
                NativeXAbstractStructureType type = GetStructureType();
                if (type == null) return null;
                if (!type.Members.ContainsKey(this.MemberName)) return null;
                return type.Members[this.MemberName];
            }
        }
    }

    public abstract class NativeXPointerMemberExpression : NativeXExpression
    {
        public abstract NativeXExpression Operand { get; set; }
        public string MemberName { get; set; }

        public NativeXAbstractStructureType GetStructureType()
        {
            if (this.Operand == null) return null;
            NativeXAbstractPointerType pointerType = this.Operand.GetUnwrapType<NativeXAbstractPointerType>();
            if (pointerType == null || pointerType.ElementType == null) return null;
            return pointerType.ElementType.Unwrap() as NativeXAbstractStructureType;
        }

        public override NativeXAbstractType AbstractType
        {
            get
            {
                if (this.MemberName == null) return null;
                NativeXAbstractStructureType type = GetStructureType();
                if (type == null) return null;
                if (!type.Members.ContainsKey(this.MemberName)) return null;
                return type.Members[this.MemberName];
            }
        }
    }

    public abstract class NativeXInvokeExpression : NativeXExpression
    {
        public abstract NativeXExpression Function { get; set; }
        public abstract CodeNodeList<NativeXExpression> Arguments { get; set; }

        public override NativeXAbstractType AbstractType
        {
            get
            {
                if (this.Function == null) return null;
                NativeXAbstractFunctionType type = this.Function.GetUnwrapType<NativeXAbstractFunctionType>();
                if (type == null) return null;
                return type.ReturnType;
            }
        }
    }

    public abstract class NativeXFunctionResultExpression : NativeXExpression
    {
        public override NativeXAbstractType AbstractType
        {
            get
            {
                NativeXFunctionDeclaration declaration = FindParent<NativeXFunctionDeclaration>();
                if (declaration == null || declaration.ReturnType == null) return null;
                return declaration.ReturnType.AbstractType;
            }
        }
    }

    public abstract class NativeXExceptionExpression : NativeXExpression
    {
    }

    public abstract class NativeXCastingExpression : NativeXExpression
    {
        public abstract NativeXType Type { get; set; }
        public abstract NativeXExpression Operand { get; set; }

        public override NativeXAbstractType AbstractType
        {
            get
            {
                if (this.Type == null) return null;
                return this.Type.AbstractType;
            }
        }
    }

    public abstract class NativeXReferenceExpression : NativeXExpression
    {
        public string ReferencedName { get; set; }

        public override NativeXAbstractType AbstractType
        {
            get
            {
                if (this.Scope == null || this.ReferencedName == null) return null;
                CodeNode node = this.Scope.Find(this.ReferencedName);

                NativeXDeclaration declaration = node as NativeXDeclaration;
                if (declaration != null) return declaration.AbstractType;

                NativeXNameTypePair nameTypePair = node as NativeXNameTypePair;
                if (nameTypePair != null && nameTypePair.Type != null) return nameTypePair.Type.AbstractType;

                NativeXGenericParameter genericParameter = node as NativeXGenericParameter;
                if (genericParameter != null && genericParameter.ParameterName != null) return new NativeXAbstractGenericParameterType()
                    {
                        ParameterName = genericParameter.ParameterName
                    };

                NativeXVariableStatement variableStatement = node as NativeXVariableStatement;
                if (variableStatement != null && variableStatement.Type != null) return variableStatement.Type.AbstractType;

                return null;
            }
        }

        public override Tuple<string, string, string> GetFormattedFunctionQuickInfo(int parameterIndex)
        {
            if (this.Scope != null)
            {
                CodeNode node = this.Scope.Find(this.ReferencedName);
                NativeXFunctionDeclaration function = node as NativeXFunctionDeclaration;
                if (function != null)
                {
                    return function.GetFormattedFunctionQuickInfo(parameterIndex, new Dictionary<string, string>());
                }
            }
            return base.GetFormattedFunctionQuickInfo(parameterIndex);
        }
    }

    public abstract class NativeXInstanciatedExpression : NativeXExpression
    {
        public string ReferencedName { get; set; }
        public abstract CodeNodeList<NativeXType> GenericArguments { get; set; }

        public override NativeXAbstractType AbstractType
        {
            get
            {
                if (this.Scope == null || this.ReferencedName == null) return null;
                NativeXReferenceExpression reference = CodeNode.Create<NativeXReferenceExpression>();
                reference.ReferencedName = this.ReferencedName;
                reference.BuildScope(this.Scope);

                NativeXAbstractGenericType type = reference.GetUnwrapType<NativeXAbstractGenericType>();
                if (type == null || type.ElementType == null) return null;
                List<NativeXAbstractType> types = this.GenericArguments == null ? new List<NativeXAbstractType>() : this.GenericArguments.Select(t => t == null ? null : t.AbstractType).ToList();
                List<Tuple<string, NativeXAbstractType>> arguments = type.GenericParameters.Zip(types, Tuple.Create).ToList();
                return type.ElementType.Instanciate(arguments);
            }
        }

        public override Tuple<string, string, string> GetFormattedFunctionQuickInfo(int parameterIndex)
        {
            if (this.Scope != null)
            {
                CodeNode node = this.Scope.Find(this.ReferencedName);
                NativeXFunctionDeclaration function = node as NativeXFunctionDeclaration;
                if (function != null)
                {
                    Dictionary<string, string> genericArguments = new Dictionary<string, string>();
                    if (function.GenericParameters != null && this.GenericArguments != null)
                    {
                        for (int i = 0; i < Math.Min(this.GenericArguments.Count, function.GenericParameters.Count); i++)
                        {
                            var gp = function.GenericParameters[i];
                            if (gp.ParameterName != null)
                            {
                                genericArguments[gp.ParameterName] = NativeXType.GetFormattedString(this.GenericArguments[i], new Dictionary<string, string>());
                            }
                        }
                    }
                    return function.GetFormattedFunctionQuickInfo(parameterIndex, genericArguments);
                }
            }
            return base.GetFormattedFunctionQuickInfo(parameterIndex);
        }
    }

    public abstract class NativeXInstanceFunctionExpression : NativeXExpression
    {
        public abstract NativeXConceptReference ConceptName { get; set; }
        public string FunctionName { get; set; }
        public abstract NativeXType Type { get; set; }

        public override NativeXAbstractType AbstractType
        {
            get
            {
                if (this.Scope == null || this.ConceptName == null || this.FunctionName == null || this.Type == null) return null;
                NativeXConceptDeclaration declaration = this.Scope.Find(this.ConceptName.ReferenceName) as NativeXConceptDeclaration;
                if (declaration == null) return null;
                NativeXAbstractType conceptType = declaration.AbstractType;
                if (conceptType == null) return null;
                NativeXAbstractConceptType realConceptType = conceptType.Instanciate(
                    new List<Tuple<string, NativeXAbstractType>>() { Tuple.Create(this.ConceptName.ReferenceName, this.Type.AbstractType) }
                    ) as NativeXAbstractConceptType;
                if (realConceptType == null || !realConceptType.Functions.ContainsKey(this.FunctionName)) return null;
                return realConceptType.Functions[this.FunctionName];
            }
        }
    }

    public abstract class NativeXSizeofTypeExpression : NativeXExpression
    {
        public abstract NativeXType Type { get; set; }
    }

    public abstract class NativeXOffsetofMemberExpression : NativeXExpression
    {
        public abstract NativeXType Type { get; set; }
        public string MemberName { get; set; }
    }
}
