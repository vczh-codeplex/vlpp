using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using CodeBoxControl.CodeProvider;

namespace CodeForm.NativeX.SyntaxTree
{
    public abstract class NativeXDeclaration : NativeXNode
    {
        public string Name { get; set; }
        public abstract CodeNodeList<NativeXGenericParameter> GenericParameters { get; set; }
        public abstract CodeNodeList<NativeXGenericConstraint> GenericConstraints { get; set; }
        public string LinkingAssembly { get; set; }
        public string LinkingSymbol { get; set; }
    }

    public abstract class NativeXNameTypePair : NativeXNode
    {
        public abstract NativeXType Type { get; set; }
        public string Name { get; set; }
    }

    public abstract class NativeXNameExpressionPair : NativeXNode
    {
        public abstract NativeXExpression Expression { get; set; }
        public string Name { get; set; }
    }

    public abstract class NativeXGenericParameter : NativeXNode
    {
        public string ParameterName { get; set; }
    }

    public abstract class NativeXGenericConstraint : NativeXNode
    {
        public string ParameterName { get; set; }
        public string ConceptName { get; set; }
    }

    public abstract class NativeXFunctionDeclaration : NativeXDeclaration
    {
        public abstract NativeXType ReturnType { get; set; }
        public abstract CodeNodeList<NativeXNameTypePair> Paramteres { get; set; }
        public abstract NativeXStatement Statement { get; set; }
    }

    public abstract class NativeXStructureDeclaration : NativeXDeclaration
    {
        public abstract CodeNodeList<NativeXNameTypePair> Members { get; set; }
    }

    public abstract class NativeXVariableDeclaration : NativeXDeclaration
    {
        public abstract NativeXType Type { get; set; }
        public abstract NativeXExpression Initializer { get; set; }
    }

    public abstract class NativeXTypeRenameDeclaration : NativeXDeclaration
    {
        public abstract NativeXType Type { get; set; }
    }

    public abstract class NativeXConceptDeclaration : NativeXDeclaration
    {
        public string ConceptType { get; set; }
        public abstract CodeNodeList<NativeXNameTypePair> Functions { get; set; }
    }

    public abstract class NativeXInstanceDeclaration : NativeXDeclaration
    {
        public NativeXType Type { get; set; }
        public abstract CodeNodeList<NativeXNameExpressionPair> Functions { get; set; }
    }
}
