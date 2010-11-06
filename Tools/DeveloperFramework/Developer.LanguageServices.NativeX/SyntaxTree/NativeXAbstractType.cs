using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using Developer.LanguageProvider;

namespace Developer.LanguageServices.NativeX.SyntaxTree
{
    public abstract class NativeXAbstractType
    {
        public override string ToString()
        {
            return GetType().Name;
        }

        public abstract NativeXAbstractType Instanciate(List<Tuple<string, NativeXAbstractType>> arguments);

        public virtual NativeXAbstractType Unwrap()
        {
            return this;
        }
    }

    public class NativeXAbstractReferenceType : NativeXAbstractType
    {
        public CodeScope Scope { get; set; }
        public string ReferenceName { get; set; }

        public override string ToString()
        {
            if (this.ReferenceName == null)
            {
                return base.ToString();
            }
            else
            {
                return "REFERENCE-TYPE: " + this.ReferenceName;
            }
        }

        public override NativeXAbstractType Instanciate(List<Tuple<string, NativeXAbstractType>> arguments)
        {
            NativeXAbstractType type = this.Unwrap();
            if (type == null || type == this)
            {
                return this;
            }
            else
            {
                return type.Instanciate(arguments);
            }
        }

        public override NativeXAbstractType Unwrap()
        {
            List<string> references = new List<string>();
            CodeScope scope = this.Scope;
            string name = this.ReferenceName;
            while (true)
            {
                if (scope == null || name == null || references.Contains(name)) break;
                references.Add(name);
                CodeNode node = this.Scope.Find(name);

                NativeXTypeRenameDeclaration typeRenameDeclaration = node as NativeXTypeRenameDeclaration;
                if (typeRenameDeclaration != null)
                {
                    NativeXAbstractReferenceType reference = typeRenameDeclaration.AbstractType as NativeXAbstractReferenceType;
                    if (reference == null) break;
                    scope = reference.Scope;
                    name = reference.ReferenceName;
                }

                NativeXDeclaration declaration = node as NativeXDeclaration;
                if (declaration != null) return declaration.AbstractType;

                NativeXGenericParameter genericParameter = node as NativeXGenericParameter;
                if (genericParameter != null && genericParameter.ParameterName != null) return new NativeXAbstractGenericParameterType()
                {
                    ParameterName = genericParameter.ParameterName
                };
            }
            return base.Unwrap();
        }
    }

    public class NativeXAbstractPointerType : NativeXAbstractType
    {
        public NativeXAbstractType ElementType { get; set; }

        public override string ToString()
        {
            if (this.ElementType == null)
            {
                return base.ToString();
            }
            else
            {
                return "POINTER-TYPE: " + this.ElementType.ToString();
            }
        }

        public override NativeXAbstractType Instanciate(List<Tuple<string, NativeXAbstractType>> arguments)
        {
            return new NativeXAbstractPointerType()
            {
                ElementType = this.ElementType == null ? null : this.ElementType.Instanciate(arguments)
            };
        }
    }

    public class NativeXAbstractArrayType : NativeXAbstractType
    {
        public NativeXAbstractType ElementType { get; set; }

        public override string ToString()
        {
            if (this.ElementType == null)
            {
                return base.ToString();
            }
            else
            {
                return "ARRAY-TYPE: " + this.ElementType.ToString();
            }
        }

        public override NativeXAbstractType Instanciate(List<Tuple<string, NativeXAbstractType>> arguments)
        {
            return new NativeXAbstractArrayType()
            {
                ElementType = this.ElementType == null ? null : this.ElementType.Instanciate(arguments)
            };
        }
    }

    public class NativeXAbstractStructureType : NativeXAbstractType
    {
        public Dictionary<string, NativeXAbstractType> Members { get; private set; }

        public NativeXAbstractStructureType()
        {
            this.Members = new Dictionary<string, NativeXAbstractType>();
        }

        public override NativeXAbstractType Instanciate(List<Tuple<string, NativeXAbstractType>> arguments)
        {
            NativeXAbstractStructureType structure = new NativeXAbstractStructureType();
            foreach (var pair in this.Members)
            {
                structure.Members.Add(pair.Key, pair.Value == null ? null : pair.Value.Instanciate(arguments));
            }
            return structure;
        }
    }

    public class NativeXAbstractFunctionType : NativeXAbstractType
    {
        public NativeXAbstractType ReturnType { get; set; }
        public List<NativeXAbstractType> Parameters { get; private set; }

        public NativeXAbstractFunctionType()
        {
            this.Parameters = new List<NativeXAbstractType>();
        }

        public override NativeXAbstractType Instanciate(List<Tuple<string, NativeXAbstractType>> arguments)
        {
            NativeXAbstractFunctionType function = new NativeXAbstractFunctionType();
            function.ReturnType = this.ReturnType == null ? null : this.ReturnType.Instanciate(arguments);
            foreach (var type in this.Parameters)
            {
                function.Parameters.Add(type == null ? null : type.Instanciate(arguments));
            }
            return function;
        }
    }

    public class NativeXAbstractGenericParameterType : NativeXAbstractType
    {
        public string ParameterName { get; set; }

        public override string ToString()
        {
            if (this.ParameterName == null)
            {
                return base.ToString();
            }
            else
            {
                return "GENERIC-PARAMETER-TYPE: " + this.ParameterName;
            }
        }

        public override NativeXAbstractType Instanciate(List<Tuple<string, NativeXAbstractType>> arguments)
        {
            return arguments.Where(a => a.Item1 == this.ParameterName).FirstOrDefault().Item2;
        }
    }

    public class NativeXAbstractConceptType : NativeXAbstractType
    {
        public Dictionary<string, NativeXAbstractType> Functions { get; private set; }

        public NativeXAbstractConceptType()
        {
            this.Functions = new Dictionary<string, NativeXAbstractType>();
        }

        public override NativeXAbstractType Instanciate(List<Tuple<string, NativeXAbstractType>> arguments)
        {
            NativeXAbstractStructureType structure = new NativeXAbstractStructureType();
            foreach (var pair in this.Functions)
            {
                structure.Members.Add(pair.Key, pair.Value == null ? null : pair.Value.Instanciate(arguments));
            }
            return structure;
        }
    }

    public class NativeXAbstractGenericType : NativeXAbstractType
    {
        public NativeXAbstractType ElementType { get; set; }
        public List<string> GenericParameters { get; private set; }

        public NativeXAbstractGenericType()
        {
            this.GenericParameters = new List<string>();
        }

        public override NativeXAbstractType Instanciate(List<Tuple<string, NativeXAbstractType>> arguments)
        {
            return this;
        }
    }
}
