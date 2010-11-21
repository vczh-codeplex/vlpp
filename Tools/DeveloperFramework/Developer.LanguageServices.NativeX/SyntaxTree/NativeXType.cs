using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using Developer.LanguageProvider;

namespace Developer.LanguageServices.NativeX.SyntaxTree
{
    public abstract class NativeXType : NativeXNode
    {
        public virtual NativeXAbstractType AbstractType
        {
            get
            {
                return null;
            }
        }

        public virtual string ToFormattedString(Dictionary<string, string> genericArguments)
        {
            return "{?}";
        }

        public static string GetFormattedString(NativeXType type, Dictionary<string, string> genericArguments)
        {
            return type == null ? "{?}" : type.ToFormattedString(genericArguments);
        }
    }

    public abstract class NativeXPointerType : NativeXType
    {
        public abstract NativeXType ElementType { get; set; }

        public override NativeXAbstractType AbstractType
        {
            get
            {
                return new NativeXAbstractPointerType()
                {
                    ElementType = this.ElementType == null ? null : this.ElementType.AbstractType
                };
            }
        }

        public override string ToFormattedString(Dictionary<string, string> genericArguments)
        {
            return GetFormattedString(this.ElementType, genericArguments) + "*";
        }
    }

    public abstract class NativeXArrayType : NativeXType
    {
        public abstract NativeXType ElementType { get; set; }
        public abstract NativeXPrimitiveExpression Size { get; set; }

        public override NativeXAbstractType AbstractType
        {
            get
            {
                return new NativeXAbstractArrayType()
                {
                    ElementType = this.ElementType == null ? null : this.ElementType.AbstractType
                };
            }
        }

        public override string ToFormattedString(Dictionary<string, string> genericArguments)
        {
            return GetFormattedString(this.ElementType, genericArguments) + "[]";
        }
    }

    public abstract class NativeXReferenceType : NativeXType
    {
        public string ReferencedName { get; set; }

        public override NativeXAbstractType AbstractType
        {
            get
            {
                return new NativeXAbstractReferenceType()
                {
                    Scope = this.Scope,
                    ReferenceName = this.ReferencedName
                };
            }
        }

        public override string ToFormattedString(Dictionary<string, string> genericArguments)
        {
            if (this.ReferencedName != null && genericArguments.ContainsKey(this.ReferencedName))
            {
                return genericArguments[this.ReferencedName];
            }
            else
            {
                return this.ReferencedName;
            }
        }
    }

    public abstract class NativeXFunctionType : NativeXType
    {
        public abstract NativeXType ReturnType { get; set; }
        public abstract CodeNodeList<NativeXType> Parameters { get; set; }

        public override NativeXAbstractType AbstractType
        {
            get
            {
                NativeXAbstractFunctionType function = new NativeXAbstractFunctionType();
                function.ReturnType = this.ReturnType == null ? null : this.ReturnType.AbstractType;
                foreach (var type in this.Parameters)
                {
                    function.Parameters.Add(type == null ? null : type.AbstractType);
                }
                return function;
            }
        }

        public override string ToFormattedString(Dictionary<string, string> genericArguments)
        {
            string result = GetFormattedString(this.ReturnType, genericArguments) + "(";
            if (this.Parameters != null)
            {
                for (int i = 0; i < this.Parameters.Count; i++)
                {
                    if (i > 0) result += ", ";
                    result += GetFormattedString(this.Parameters[i], genericArguments);
                }
            }
            result += ")";
            return result;
        }
    }

    public abstract class NativeXInstanciatedType : NativeXType
    {
        public abstract NativeXReferenceType ElementType { get; set; }
        public abstract CodeNodeList<NativeXType> GenericArguments { get; set; }

        public override NativeXAbstractType AbstractType
        {
            get
            {
                if (this.ElementType == null) return null;
                NativeXAbstractType elementType = this.ElementType.AbstractType;
                if (elementType == null) return null;

                NativeXAbstractGenericType type = elementType.Unwrap() as NativeXAbstractGenericType;
                if (type == null || type.ElementType == null) return null;
                List<NativeXAbstractType> types = this.GenericArguments == null ? new List<NativeXAbstractType>() : this.GenericArguments.Select(t => t == null ? null : t.AbstractType).ToList();
                List<Tuple<string, NativeXAbstractType>> arguments = type.GenericParameters.Zip(types, Tuple.Create).ToList();
                return type.ElementType.Instanciate(arguments);
            }
        }

        public override string ToFormattedString(Dictionary<string, string> genericArguments)
        {
            string result = GetFormattedString(this.ElementType, genericArguments) + "<";
            if (this.GenericArguments != null)
            {
                for (int i = 0; i < this.GenericArguments.Count; i++)
                {
                    if (i > 0) result += ", ";
                    result += GetFormattedString(this.GenericArguments[i], genericArguments);
                }
            }
            result += ">";
            return result;
        }
    }
}
