using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using Developer.LanguageProvider;

namespace Developer.LanguageServices.NativeX.SyntaxTree
{
    public abstract class NativeXDeclaration : NativeXNode
    {
        public string Name { get; set; }
        public abstract CodeNodeList<NativeXGenericParameter> GenericParameters { get; set; }
        public abstract CodeNodeList<NativeXGenericConstraint> GenericConstraints { get; set; }
        public abstract NativeXLinking Linking { get; set; }

        public virtual NativeXAbstractType AbstractType
        {
            get
            {
                return null;
            }
        }

        protected override bool ContainScope
        {
            get
            {
                return true;
            }
        }

        protected override void FillScope(CodeNodeCollection nodes)
        {
            base.FillScope(nodes);
            nodes.AddIfNotExists(this.Name, this);
        }

        protected virtual NativeXAbstractType WrapGeneric(NativeXAbstractType type)
        {
            if (type == null)
            {
                return null;
            }
            else if (this.GenericParameters == null)
            {
                return type;
            }
            else
            {
                NativeXAbstractGenericType generic = new NativeXAbstractGenericType();
                generic.ElementType = type;
                foreach (var p in this.GenericParameters)
                {
                    if (p.ParameterName != null)
                    {
                        generic.GenericParameters.Add(p.ParameterName);
                    }
                }
                return generic;
            }
        }
    }

    public abstract class NativeXNameTypePair : NativeXNode
    {
        public abstract NativeXType Type { get; set; }
        public string Name { get; set; }

        protected override void FillScope(CodeNodeCollection nodes)
        {
            base.FillScope(nodes);
            nodes.AddIfNotExists(this.Name, this);
        }
    }

    public abstract class NativeXNameExpressionPair : NativeXNode
    {
        public abstract NativeXExpression Expression { get; set; }
        public string Name { get; set; }
    }

    public abstract class NativeXGenericParameter : NativeXNode
    {
        public string ParameterName { get; set; }

        protected override void FillScope(CodeNodeCollection nodes)
        {
            base.FillScope(nodes);
            nodes.AddIfNotExists(this.ParameterName, this);
        }
    }

    public abstract class NativeXGenericConstraint : NativeXNode
    {
        public string ParameterName { get; set; }
        public string ConceptName { get; set; }
    }

    public abstract class NativeXLinking : NativeXNode
    {
        public string LinkingAssembly { get; set; }
        public string LinkingSymbol { get; set; }
    }

    public abstract class NativeXFunctionDeclaration : NativeXDeclaration
    {
        public abstract NativeXType ReturnType { get; set; }
        public abstract CodeNodeList<NativeXNameTypePair> Parameters { get; set; }
        public abstract NativeXStatement Statement { get; set; }
        public bool Foreign { get; set; }

        public override NativeXAbstractType AbstractType
        {
            get
            {
                NativeXAbstractFunctionType function = new NativeXAbstractFunctionType();
                function.ReturnType = this.ReturnType == null ? null : this.ReturnType.AbstractType;
                foreach (var type in this.Parameters)
                {
                    function.Parameters.Add(type == null || type.Type == null ? null : type.Type.AbstractType);
                }
                return WrapGeneric(function);
            }
        }
    }

    public abstract class NativeXStructureDeclaration : NativeXDeclaration
    {
        public abstract CodeNodeList<NativeXNameTypePair> Members { get; set; }

        public override NativeXAbstractType AbstractType
        {
            get
            {
                NativeXAbstractStructureType structure = new NativeXAbstractStructureType();
                if (this.Members != null)
                {
                    foreach (var pair in this.Members)
                    {
                        if (pair.Name != null)
                        {
                            structure.Members[pair.Name] = pair.Type == null ? null : pair.Type.AbstractType;
                        }
                    }
                }
                return WrapGeneric(structure);
            }
        }
    }

    public abstract class NativeXVariableDeclaration : NativeXDeclaration
    {
        public abstract NativeXType Type { get; set; }
        public abstract NativeXExpression Initializer { get; set; }

        public override NativeXAbstractType AbstractType
        {
            get
            {
                return this.Type == null ? null : WrapGeneric(this.Type.AbstractType);
            }
        }
    }

    public abstract class NativeXTypeRenameDeclaration : NativeXDeclaration
    {
        public abstract NativeXType Type { get; set; }

        public override NativeXAbstractType AbstractType
        {
            get
            {
                return this.Type == null ? null : WrapGeneric(this.Type.AbstractType);
            }
        }
    }

    public abstract class NativeXConceptDeclaration : NativeXDeclaration
    {
        public abstract NativeXGenericParameter ConceptType { get; set; }
        public abstract CodeNodeList<NativeXNameTypePair> Functions { get; set; }

        public override NativeXAbstractType AbstractType
        {
            get
            {
                NativeXAbstractConceptType concept = new NativeXAbstractConceptType();
                if (this.Functions != null)
                {
                    foreach (var pair in this.Functions)
                    {
                        if (pair.Name != null)
                        {
                            concept.Functions[pair.Name] = pair.Type == null ? null : pair.Type.AbstractType;
                        }
                    }
                }

                NativeXAbstractGenericType generic = new NativeXAbstractGenericType();
                generic.ElementType = concept;
                if (this.ConceptType != null && this.ConceptType.ParameterName != null)
                {
                    generic.GenericParameters.Add(this.ConceptType.ParameterName);
                }
                return generic;
            }
        }
    }

    public abstract class NativeXInstanceDeclaration : NativeXDeclaration
    {
        public abstract NativeXReferenceType Type { get; set; }
        public string ConceptName { get; set; }
        public abstract CodeNodeList<NativeXNameExpressionPair> Functions { get; set; }
    }
}
