using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using Developer.LanguageProvider;

namespace Developer.LanguageServices.NativeX
{
    public static partial class NativeXCodeParser
    {
        public static Developer.LanguageServices.NativeX.SyntaxTree.NativeXUnit ParseUnit(List<CodeToken> tokens, ref int currentToken, ref bool parseSuccess)
        {
            Developer.LanguageProvider.TextPosition start = Developer.LanguageProvider.CodeTokenizer.GetStartPosition(tokens, currentToken);
            Developer.LanguageServices.NativeX.SyntaxTree.NativeXUnit result = default(Developer.LanguageServices.NativeX.SyntaxTree.NativeXUnit);
            System.String NameMember0 = default(System.String);
            Developer.LanguageProvider.CodeNodeList<Developer.LanguageServices.NativeX.SyntaxTree.NativeXUses> UsesUnitsMember0 = default(Developer.LanguageProvider.CodeNodeList<Developer.LanguageServices.NativeX.SyntaxTree.NativeXUses>);
            Developer.LanguageProvider.CodeNodeList<Developer.LanguageServices.NativeX.SyntaxTree.NativeXDeclaration> DeclarationsMember0 = default(Developer.LanguageProvider.CodeNodeList<Developer.LanguageServices.NativeX.SyntaxTree.NativeXDeclaration>);
            {
                int currentIndexCopy1 = currentToken;
                int currentIndex1 = currentToken;
                {
                    Developer.LanguageProvider.CodeTokenizer.ParseToken(tokens, ref currentIndex1, ref parseSuccess, "unit");
                }
                if (parseSuccess)
                {
                    currentIndexCopy1 = currentIndex1;
                }
                else
                {
                    goto LABEL_0;
                }
                {
                    System.String NameMember1 = default(System.String);
                    {
                        Developer.LanguageProvider.CodeToken result2 = default(Developer.LanguageProvider.CodeToken);
                        int currentIndex2 = currentIndex1;
                        {
                            result2 = Developer.LanguageProvider.CodeTokenizer.ParseToken(tokens, ref currentIndex2, ref parseSuccess, Developer.LanguageServices.NativeX.NativeXTokenizer.IdToken);
                        }
                        if (parseSuccess)
                        {
                            currentIndex1 = currentIndex2;
                            NameMember1 = result2.Value;
                        }
                    }
                    NameMember0 = NameMember1;
                }
                if (parseSuccess)
                {
                    currentIndexCopy1 = currentIndex1;
                }
                else
                {
                    goto LABEL_0;
                }
                {
                    Developer.LanguageProvider.CodeTokenizer.ParseToken(tokens, ref currentIndex1, ref parseSuccess, ";");
                }
                if (parseSuccess)
                {
                    currentIndexCopy1 = currentIndex1;
                }
                else
                {
                    goto LABEL_0;
                }
                {
                    Developer.LanguageProvider.CodeNodeList<Developer.LanguageServices.NativeX.SyntaxTree.NativeXUses> UsesUnitsMember1 = default(Developer.LanguageProvider.CodeNodeList<Developer.LanguageServices.NativeX.SyntaxTree.NativeXUses>);
                    {
                        int currentIndex2 = -1;
                        currentIndex2 = currentIndex1;
                        {
                            Developer.LanguageProvider.CodeNodeList<Developer.LanguageServices.NativeX.SyntaxTree.NativeXUses> UsesUnitsMember2 = default(Developer.LanguageProvider.CodeNodeList<Developer.LanguageServices.NativeX.SyntaxTree.NativeXUses>);
                            {
                                int currentIndexCopy3 = currentIndex2;
                                int currentIndex3 = currentIndex2;
                                {
                                    Developer.LanguageProvider.CodeTokenizer.ParseToken(tokens, ref currentIndex3, ref parseSuccess, "uses");
                                }
                                if (parseSuccess)
                                {
                                    currentIndexCopy3 = currentIndex3;
                                }
                                else
                                {
                                    goto LABEL_1;
                                }
                                {
                                    Developer.LanguageProvider.CodeNodeList<Developer.LanguageServices.NativeX.SyntaxTree.NativeXUses> UsesUnitsMember3 = default(Developer.LanguageProvider.CodeNodeList<Developer.LanguageServices.NativeX.SyntaxTree.NativeXUses>);
                                    {
                                        Developer.LanguageProvider.CodeNodeList<Developer.LanguageServices.NativeX.SyntaxTree.NativeXUses> result4 = default(Developer.LanguageProvider.CodeNodeList<Developer.LanguageServices.NativeX.SyntaxTree.NativeXUses>);
                                        int currentIndex4 = currentIndex3;
                                        {
                                            result4 = new Developer.LanguageProvider.CodeNodeList<Developer.LanguageServices.NativeX.SyntaxTree.NativeXUses>();
                                            {
                                                Developer.LanguageServices.NativeX.SyntaxTree.NativeXUses result5 = default(Developer.LanguageServices.NativeX.SyntaxTree.NativeXUses);
                                                int currentIndex5 = currentIndex4;
                                                {
                                                    parseSuccess = false;
                                                    result5 = ParseUseUnitItem(tokens, ref currentIndex5, ref parseSuccess);
                                                }
                                                if (parseSuccess)
                                                {
                                                    currentIndex4 = currentIndex5;
                                                    result4.Add(result5);
                                                    result4.Start = result5.Start;
                                                    result4.End = result5.End;
                                                }
                                                while (true)
                                                {
                                                    int currentIndexCopy5 = currentIndex4;
                                                    {
                                                        Developer.LanguageProvider.CodeTokenizer.ParseToken(tokens, ref currentIndex5, ref parseSuccess, ",");
                                                    }
                                                    if (parseSuccess)
                                                    {
                                                        currentIndexCopy5 = currentIndex5;
                                                    }
                                                    else
                                                    {
                                                        goto LABEL_2;
                                                    }
                                                    {
                                                        parseSuccess = false;
                                                        result5 = ParseUseUnitItem(tokens, ref currentIndex5, ref parseSuccess);
                                                    }
                                                    if (parseSuccess)
                                                    {
                                                        currentIndexCopy5 = currentIndex5;
                                                    }
                                                    else
                                                    {
                                                        goto LABEL_2;
                                                    }
                                                    currentIndex4 = currentIndexCopy5;
                                                    result4.Add(result5);
                                                    result4.End = result5.End;
                                                }
                                            LABEL_2: ; parseSuccess = true;
                                            }
                                        }
                                        if (parseSuccess)
                                        {
                                            currentIndex3 = currentIndex4;
                                            UsesUnitsMember3 = result4;
                                        }
                                    }
                                    UsesUnitsMember2 = UsesUnitsMember3;
                                }
                                if (parseSuccess)
                                {
                                    currentIndexCopy3 = currentIndex3;
                                }
                                else
                                {
                                    goto LABEL_1;
                                }
                                {
                                    Developer.LanguageProvider.CodeTokenizer.ParseToken(tokens, ref currentIndex3, ref parseSuccess, ";");
                                }
                                if (parseSuccess)
                                {
                                    currentIndexCopy3 = currentIndex3;
                                }
                                else
                                {
                                    goto LABEL_1;
                                }
                                currentIndex2 = currentIndexCopy3;
                            LABEL_1: ;
                            }
                            UsesUnitsMember1 = UsesUnitsMember2;
                        }
                        if (parseSuccess)
                        {
                            currentIndex1 = currentIndex2;
                        }
                        parseSuccess = true;
                    }
                    UsesUnitsMember0 = UsesUnitsMember1;
                }
                if (parseSuccess)
                {
                    currentIndexCopy1 = currentIndex1;
                }
                else
                {
                    goto LABEL_0;
                }
                {
                    Developer.LanguageProvider.CodeNodeList<Developer.LanguageServices.NativeX.SyntaxTree.NativeXDeclaration> DeclarationsMember1 = default(Developer.LanguageProvider.CodeNodeList<Developer.LanguageServices.NativeX.SyntaxTree.NativeXDeclaration>);
                    {
                        Developer.LanguageProvider.CodeNodeList<Developer.LanguageServices.NativeX.SyntaxTree.NativeXDeclaration> result2 = default(Developer.LanguageProvider.CodeNodeList<Developer.LanguageServices.NativeX.SyntaxTree.NativeXDeclaration>);
                        int currentIndex2 = currentIndex1;
                        {
                            result2 = new Developer.LanguageProvider.CodeNodeList<Developer.LanguageServices.NativeX.SyntaxTree.NativeXDeclaration>();
                            {
                                Developer.LanguageServices.NativeX.SyntaxTree.NativeXDeclaration result3 = default(Developer.LanguageServices.NativeX.SyntaxTree.NativeXDeclaration);
                                int currentIndex3 = currentIndex2;
                                {
                                    parseSuccess = false;
                                    result3 = ParseDeclaration(tokens, ref currentIndex3, ref parseSuccess);
                                }
                                if (parseSuccess)
                                {
                                    currentIndex2 = currentIndex3;
                                    result2.Add(result3);
                                    result2.Start = result3.Start;
                                    result2.End = result3.End;
                                }
                                while (true)
                                {
                                    int currentIndexCopy3 = currentIndex2;
                                    {
                                        parseSuccess = false;
                                        result3 = ParseDeclaration(tokens, ref currentIndex3, ref parseSuccess);
                                    }
                                    if (parseSuccess)
                                    {
                                        currentIndexCopy3 = currentIndex3;
                                    }
                                    else
                                    {
                                        goto LABEL_3;
                                    }
                                    currentIndex2 = currentIndexCopy3;
                                    result2.Add(result3);
                                    result2.End = result3.End;
                                }
                            LABEL_3: ; parseSuccess = true;
                            }
                        }
                        if (parseSuccess)
                        {
                            currentIndex1 = currentIndex2;
                            DeclarationsMember1 = result2;
                        }
                    }
                    DeclarationsMember0 = DeclarationsMember1;
                }
                if (parseSuccess)
                {
                    currentIndexCopy1 = currentIndex1;
                }
                else
                {
                    goto LABEL_0;
                }
                currentToken = currentIndexCopy1;
            LABEL_0: ;
            }
            if (parseSuccess)
            {
                if (result == null) result = CodeNode.Create<Developer.LanguageServices.NativeX.SyntaxTree.NativeXUnit>();
                result.Name = NameMember0;
                result.UsesUnits = UsesUnitsMember0;
                result.Declarations = DeclarationsMember0;
                result.Start = start;
                result.End = Developer.LanguageProvider.CodeTokenizer.GetEndPosition(tokens, currentToken);
            }
            return result;
        }

        public static Developer.LanguageServices.NativeX.SyntaxTree.NativeXUses ParseUseUnitItem(List<CodeToken> tokens, ref int currentToken, ref bool parseSuccess)
        {
            Developer.LanguageProvider.TextPosition start = Developer.LanguageProvider.CodeTokenizer.GetStartPosition(tokens, currentToken);
            Developer.LanguageServices.NativeX.SyntaxTree.NativeXUses result = default(Developer.LanguageServices.NativeX.SyntaxTree.NativeXUses);
            System.String UnitNameMember0 = default(System.String);
            {
                Developer.LanguageProvider.CodeToken result1 = default(Developer.LanguageProvider.CodeToken);
                int currentIndex1 = currentToken;
                {
                    result1 = Developer.LanguageProvider.CodeTokenizer.ParseToken(tokens, ref currentIndex1, ref parseSuccess, Developer.LanguageServices.NativeX.NativeXTokenizer.IdToken);
                }
                if (parseSuccess)
                {
                    currentToken = currentIndex1;
                    UnitNameMember0 = result1.Value;
                }
            }
            if (parseSuccess)
            {
                if (result == null) result = CodeNode.Create<Developer.LanguageServices.NativeX.SyntaxTree.NativeXUses>();
                result.UnitName = UnitNameMember0;
                result.Start = start;
                result.End = Developer.LanguageProvider.CodeTokenizer.GetEndPosition(tokens, currentToken);
            }
            return result;
        }

        public static Developer.LanguageServices.NativeX.SyntaxTree.NativeXDeclaration ParseDeclaration(List<CodeToken> tokens, ref int currentToken, ref bool parseSuccess)
        {
            Developer.LanguageProvider.TextPosition start = Developer.LanguageProvider.CodeTokenizer.GetStartPosition(tokens, currentToken);
            Developer.LanguageServices.NativeX.SyntaxTree.NativeXDeclaration result = default(Developer.LanguageServices.NativeX.SyntaxTree.NativeXDeclaration);
            {
                Developer.LanguageServices.NativeX.SyntaxTree.NativeXDeclaration result1 = default(Developer.LanguageServices.NativeX.SyntaxTree.NativeXDeclaration);
                int currentIndex1 = -1;
                currentIndex1 = currentToken;
                {
                    {
                        Developer.LanguageServices.NativeX.SyntaxTree.NativeXDeclaration result2 = default(Developer.LanguageServices.NativeX.SyntaxTree.NativeXDeclaration);
                        int currentIndex2 = currentIndex1;
                        {
                            parseSuccess = false;
                            result2 = ParseGenericDeclaration(tokens, ref currentIndex2, ref parseSuccess);
                        }
                        if (parseSuccess)
                        {
                            currentIndex1 = currentIndex2;
                            result1 = result2;
                        }
                    }
                }
                if (parseSuccess)
                {
                    currentToken = currentIndex1;
                    goto LABEL_SUCCESS_0;
                }
                currentIndex1 = currentToken;
                {
                    {
                        Developer.LanguageServices.NativeX.SyntaxTree.NativeXDeclaration result2 = default(Developer.LanguageServices.NativeX.SyntaxTree.NativeXDeclaration);
                        int currentIndex2 = currentIndex1;
                        {
                            parseSuccess = false;
                            result2 = ParseNonGenericDeclaration(tokens, ref currentIndex2, ref parseSuccess);
                        }
                        if (parseSuccess)
                        {
                            currentIndex1 = currentIndex2;
                            result1 = result2;
                        }
                    }
                }
                if (parseSuccess)
                {
                    currentToken = currentIndex1;
                    goto LABEL_SUCCESS_0;
                }
                goto LABEL_FAIL_1;
            LABEL_SUCCESS_0: ;
                result = result1;
            LABEL_FAIL_1: ;
            }
            if (parseSuccess)
            {
                if (result == null) result = CodeNode.Create<Developer.LanguageServices.NativeX.SyntaxTree.NativeXDeclaration>();
                result.Start = start;
                result.End = Developer.LanguageProvider.CodeTokenizer.GetEndPosition(tokens, currentToken);
            }
            return result;
        }

        public static Developer.LanguageServices.NativeX.SyntaxTree.NativeXDeclaration ParseGenericDeclaration(List<CodeToken> tokens, ref int currentToken, ref bool parseSuccess)
        {
            Developer.LanguageProvider.TextPosition start = Developer.LanguageProvider.CodeTokenizer.GetStartPosition(tokens, currentToken);
            Developer.LanguageServices.NativeX.SyntaxTree.NativeXDeclaration result = default(Developer.LanguageServices.NativeX.SyntaxTree.NativeXDeclaration);
            Developer.LanguageProvider.CodeNodeList<Developer.LanguageServices.NativeX.SyntaxTree.NativeXGenericParameter> GenericParametersMember0 = default(Developer.LanguageProvider.CodeNodeList<Developer.LanguageServices.NativeX.SyntaxTree.NativeXGenericParameter>);
            Developer.LanguageProvider.CodeNodeList<Developer.LanguageServices.NativeX.SyntaxTree.NativeXGenericConstraint> GenericConstraintsMember0 = default(Developer.LanguageProvider.CodeNodeList<Developer.LanguageServices.NativeX.SyntaxTree.NativeXGenericConstraint>);
            {
                Developer.LanguageServices.NativeX.SyntaxTree.NativeXDeclaration result1 = default(Developer.LanguageServices.NativeX.SyntaxTree.NativeXDeclaration);
                int currentIndexCopy1 = currentToken;
                int currentIndex1 = currentToken;
                {
                    Developer.LanguageProvider.CodeTokenizer.ParseToken(tokens, ref currentIndex1, ref parseSuccess, "generic");
                }
                if (parseSuccess)
                {
                    currentIndexCopy1 = currentIndex1;
                }
                else
                {
                    goto LABEL_0;
                }
                {
                    Developer.LanguageProvider.CodeTokenizer.ParseToken(tokens, ref currentIndex1, ref parseSuccess, "<");
                }
                if (parseSuccess)
                {
                    currentIndexCopy1 = currentIndex1;
                }
                else
                {
                    goto LABEL_0;
                }
                {
                    Developer.LanguageProvider.CodeNodeList<Developer.LanguageServices.NativeX.SyntaxTree.NativeXGenericParameter> GenericParametersMember1 = default(Developer.LanguageProvider.CodeNodeList<Developer.LanguageServices.NativeX.SyntaxTree.NativeXGenericParameter>);
                    {
                        Developer.LanguageProvider.CodeNodeList<Developer.LanguageServices.NativeX.SyntaxTree.NativeXGenericParameter> result2 = default(Developer.LanguageProvider.CodeNodeList<Developer.LanguageServices.NativeX.SyntaxTree.NativeXGenericParameter>);
                        int currentIndex2 = currentIndex1;
                        {
                            result2 = new Developer.LanguageProvider.CodeNodeList<Developer.LanguageServices.NativeX.SyntaxTree.NativeXGenericParameter>();
                            {
                                Developer.LanguageServices.NativeX.SyntaxTree.NativeXGenericParameter result3 = default(Developer.LanguageServices.NativeX.SyntaxTree.NativeXGenericParameter);
                                int currentIndex3 = currentIndex2;
                                {
                                    parseSuccess = false;
                                    result3 = ParseGenericParameterItem(tokens, ref currentIndex3, ref parseSuccess);
                                }
                                if (parseSuccess)
                                {
                                    currentIndex2 = currentIndex3;
                                    result2.Add(result3);
                                    result2.Start = result3.Start;
                                    result2.End = result3.End;
                                }
                                while (true)
                                {
                                    int currentIndexCopy3 = currentIndex2;
                                    {
                                        Developer.LanguageProvider.CodeTokenizer.ParseToken(tokens, ref currentIndex3, ref parseSuccess, ",");
                                    }
                                    if (parseSuccess)
                                    {
                                        currentIndexCopy3 = currentIndex3;
                                    }
                                    else
                                    {
                                        goto LABEL_1;
                                    }
                                    {
                                        parseSuccess = false;
                                        result3 = ParseGenericParameterItem(tokens, ref currentIndex3, ref parseSuccess);
                                    }
                                    if (parseSuccess)
                                    {
                                        currentIndexCopy3 = currentIndex3;
                                    }
                                    else
                                    {
                                        goto LABEL_1;
                                    }
                                    currentIndex2 = currentIndexCopy3;
                                    result2.Add(result3);
                                    result2.End = result3.End;
                                }
                            LABEL_1: ; parseSuccess = true;
                            }
                        }
                        if (parseSuccess)
                        {
                            currentIndex1 = currentIndex2;
                            GenericParametersMember1 = result2;
                        }
                    }
                    GenericParametersMember0 = GenericParametersMember1;
                }
                if (parseSuccess)
                {
                    currentIndexCopy1 = currentIndex1;
                }
                else
                {
                    goto LABEL_0;
                }
                {
                    Developer.LanguageProvider.CodeTokenizer.ParseToken(tokens, ref currentIndex1, ref parseSuccess, ">");
                }
                if (parseSuccess)
                {
                    currentIndexCopy1 = currentIndex1;
                }
                else
                {
                    goto LABEL_0;
                }
                {
                    Developer.LanguageProvider.CodeNodeList<Developer.LanguageServices.NativeX.SyntaxTree.NativeXGenericConstraint> GenericConstraintsMember1 = default(Developer.LanguageProvider.CodeNodeList<Developer.LanguageServices.NativeX.SyntaxTree.NativeXGenericConstraint>);
                    {
                        int currentIndex2 = -1;
                        currentIndex2 = currentIndex1;
                        {
                            Developer.LanguageProvider.CodeNodeList<Developer.LanguageServices.NativeX.SyntaxTree.NativeXGenericConstraint> GenericConstraintsMember2 = default(Developer.LanguageProvider.CodeNodeList<Developer.LanguageServices.NativeX.SyntaxTree.NativeXGenericConstraint>);
                            {
                                int currentIndexCopy3 = currentIndex2;
                                int currentIndex3 = currentIndex2;
                                {
                                    Developer.LanguageProvider.CodeTokenizer.ParseToken(tokens, ref currentIndex3, ref parseSuccess, "where");
                                }
                                if (parseSuccess)
                                {
                                    currentIndexCopy3 = currentIndex3;
                                }
                                else
                                {
                                    goto LABEL_2;
                                }
                                {
                                    Developer.LanguageProvider.CodeNodeList<Developer.LanguageServices.NativeX.SyntaxTree.NativeXGenericConstraint> GenericConstraintsMember3 = default(Developer.LanguageProvider.CodeNodeList<Developer.LanguageServices.NativeX.SyntaxTree.NativeXGenericConstraint>);
                                    {
                                        Developer.LanguageProvider.CodeNodeList<Developer.LanguageServices.NativeX.SyntaxTree.NativeXGenericConstraint> result4 = default(Developer.LanguageProvider.CodeNodeList<Developer.LanguageServices.NativeX.SyntaxTree.NativeXGenericConstraint>);
                                        int currentIndex4 = currentIndex3;
                                        {
                                            result4 = new Developer.LanguageProvider.CodeNodeList<Developer.LanguageServices.NativeX.SyntaxTree.NativeXGenericConstraint>();
                                            {
                                                Developer.LanguageServices.NativeX.SyntaxTree.NativeXGenericConstraint result5 = default(Developer.LanguageServices.NativeX.SyntaxTree.NativeXGenericConstraint);
                                                int currentIndex5 = currentIndex4;
                                                {
                                                    parseSuccess = false;
                                                    result5 = ParseGenericConstraintItem(tokens, ref currentIndex5, ref parseSuccess);
                                                }
                                                if (parseSuccess)
                                                {
                                                    currentIndex4 = currentIndex5;
                                                    result4.Add(result5);
                                                    result4.Start = result5.Start;
                                                    result4.End = result5.End;
                                                }
                                                while (true)
                                                {
                                                    int currentIndexCopy5 = currentIndex4;
                                                    {
                                                        Developer.LanguageProvider.CodeTokenizer.ParseToken(tokens, ref currentIndex5, ref parseSuccess, ",");
                                                    }
                                                    if (parseSuccess)
                                                    {
                                                        currentIndexCopy5 = currentIndex5;
                                                    }
                                                    else
                                                    {
                                                        goto LABEL_3;
                                                    }
                                                    {
                                                        parseSuccess = false;
                                                        result5 = ParseGenericConstraintItem(tokens, ref currentIndex5, ref parseSuccess);
                                                    }
                                                    if (parseSuccess)
                                                    {
                                                        currentIndexCopy5 = currentIndex5;
                                                    }
                                                    else
                                                    {
                                                        goto LABEL_3;
                                                    }
                                                    currentIndex4 = currentIndexCopy5;
                                                    result4.Add(result5);
                                                    result4.End = result5.End;
                                                }
                                            LABEL_3: ; parseSuccess = true;
                                            }
                                        }
                                        if (parseSuccess)
                                        {
                                            currentIndex3 = currentIndex4;
                                            GenericConstraintsMember3 = result4;
                                        }
                                    }
                                    GenericConstraintsMember2 = GenericConstraintsMember3;
                                }
                                if (parseSuccess)
                                {
                                    currentIndexCopy3 = currentIndex3;
                                }
                                else
                                {
                                    goto LABEL_2;
                                }
                                currentIndex2 = currentIndexCopy3;
                            LABEL_2: ;
                            }
                            GenericConstraintsMember1 = GenericConstraintsMember2;
                        }
                        if (parseSuccess)
                        {
                            currentIndex1 = currentIndex2;
                        }
                        parseSuccess = true;
                    }
                    GenericConstraintsMember0 = GenericConstraintsMember1;
                }
                if (parseSuccess)
                {
                    currentIndexCopy1 = currentIndex1;
                }
                else
                {
                    goto LABEL_0;
                }
                {
                    {
                        Developer.LanguageServices.NativeX.SyntaxTree.NativeXDeclaration result2 = default(Developer.LanguageServices.NativeX.SyntaxTree.NativeXDeclaration);
                        int currentIndex2 = currentIndex1;
                        {
                            parseSuccess = false;
                            result2 = ParseNonGenericDeclaration(tokens, ref currentIndex2, ref parseSuccess);
                        }
                        if (parseSuccess)
                        {
                            currentIndex1 = currentIndex2;
                            result1 = result2;
                        }
                    }
                }
                if (parseSuccess)
                {
                    currentIndexCopy1 = currentIndex1;
                }
                else
                {
                    goto LABEL_0;
                }
                currentToken = currentIndexCopy1;
                result = result1;
            LABEL_0: ;
            }
            if (parseSuccess)
            {
                if (result == null) result = CodeNode.Create<Developer.LanguageServices.NativeX.SyntaxTree.NativeXDeclaration>();
                result.GenericParameters = GenericParametersMember0;
                result.GenericConstraints = GenericConstraintsMember0;
                result.Start = start;
                result.End = Developer.LanguageProvider.CodeTokenizer.GetEndPosition(tokens, currentToken);
            }
            return result;
        }

        public static Developer.LanguageServices.NativeX.SyntaxTree.NativeXGenericParameter ParseGenericParameterItem(List<CodeToken> tokens, ref int currentToken, ref bool parseSuccess)
        {
            Developer.LanguageProvider.TextPosition start = Developer.LanguageProvider.CodeTokenizer.GetStartPosition(tokens, currentToken);
            Developer.LanguageServices.NativeX.SyntaxTree.NativeXGenericParameter result = default(Developer.LanguageServices.NativeX.SyntaxTree.NativeXGenericParameter);
            System.String ParameterNameMember0 = default(System.String);
            {
                Developer.LanguageProvider.CodeToken result1 = default(Developer.LanguageProvider.CodeToken);
                int currentIndex1 = currentToken;
                {
                    result1 = Developer.LanguageProvider.CodeTokenizer.ParseToken(tokens, ref currentIndex1, ref parseSuccess, Developer.LanguageServices.NativeX.NativeXTokenizer.IdToken);
                }
                if (parseSuccess)
                {
                    currentToken = currentIndex1;
                    ParameterNameMember0 = result1.Value;
                }
            }
            if (parseSuccess)
            {
                if (result == null) result = CodeNode.Create<Developer.LanguageServices.NativeX.SyntaxTree.NativeXGenericParameter>();
                result.ParameterName = ParameterNameMember0;
                result.Start = start;
                result.End = Developer.LanguageProvider.CodeTokenizer.GetEndPosition(tokens, currentToken);
            }
            return result;
        }

        public static Developer.LanguageServices.NativeX.SyntaxTree.NativeXGenericConstraint ParseGenericConstraintItem(List<CodeToken> tokens, ref int currentToken, ref bool parseSuccess)
        {
            Developer.LanguageProvider.TextPosition start = Developer.LanguageProvider.CodeTokenizer.GetStartPosition(tokens, currentToken);
            Developer.LanguageServices.NativeX.SyntaxTree.NativeXGenericConstraint result = default(Developer.LanguageServices.NativeX.SyntaxTree.NativeXGenericConstraint);
            System.String ParameterNameMember0 = default(System.String);
            System.String ConceptNameMember0 = default(System.String);
            {
                int currentIndexCopy1 = currentToken;
                int currentIndex1 = currentToken;
                {
                    System.String ParameterNameMember1 = default(System.String);
                    {
                        Developer.LanguageProvider.CodeToken result2 = default(Developer.LanguageProvider.CodeToken);
                        int currentIndex2 = currentIndex1;
                        {
                            result2 = Developer.LanguageProvider.CodeTokenizer.ParseToken(tokens, ref currentIndex2, ref parseSuccess, Developer.LanguageServices.NativeX.NativeXTokenizer.IdToken);
                        }
                        if (parseSuccess)
                        {
                            currentIndex1 = currentIndex2;
                            ParameterNameMember1 = result2.Value;
                        }
                    }
                    ParameterNameMember0 = ParameterNameMember1;
                }
                if (parseSuccess)
                {
                    currentIndexCopy1 = currentIndex1;
                }
                else
                {
                    goto LABEL_0;
                }
                {
                    Developer.LanguageProvider.CodeTokenizer.ParseToken(tokens, ref currentIndex1, ref parseSuccess, ":");
                }
                if (parseSuccess)
                {
                    currentIndexCopy1 = currentIndex1;
                }
                else
                {
                    goto LABEL_0;
                }
                {
                    System.String ConceptNameMember1 = default(System.String);
                    {
                        Developer.LanguageProvider.CodeToken result2 = default(Developer.LanguageProvider.CodeToken);
                        int currentIndex2 = currentIndex1;
                        {
                            result2 = Developer.LanguageProvider.CodeTokenizer.ParseToken(tokens, ref currentIndex2, ref parseSuccess, Developer.LanguageServices.NativeX.NativeXTokenizer.IdToken);
                        }
                        if (parseSuccess)
                        {
                            currentIndex1 = currentIndex2;
                            ConceptNameMember1 = result2.Value;
                        }
                    }
                    ConceptNameMember0 = ConceptNameMember1;
                }
                if (parseSuccess)
                {
                    currentIndexCopy1 = currentIndex1;
                }
                else
                {
                    goto LABEL_0;
                }
                currentToken = currentIndexCopy1;
            LABEL_0: ;
            }
            if (parseSuccess)
            {
                if (result == null) result = CodeNode.Create<Developer.LanguageServices.NativeX.SyntaxTree.NativeXGenericConstraint>();
                result.ParameterName = ParameterNameMember0;
                result.ConceptName = ConceptNameMember0;
                result.Start = start;
                result.End = Developer.LanguageProvider.CodeTokenizer.GetEndPosition(tokens, currentToken);
            }
            return result;
        }

        public static Developer.LanguageServices.NativeX.SyntaxTree.NativeXDeclaration ParseNonGenericDeclaration(List<CodeToken> tokens, ref int currentToken, ref bool parseSuccess)
        {
            Developer.LanguageProvider.TextPosition start = Developer.LanguageProvider.CodeTokenizer.GetStartPosition(tokens, currentToken);
            Developer.LanguageServices.NativeX.SyntaxTree.NativeXDeclaration result = default(Developer.LanguageServices.NativeX.SyntaxTree.NativeXDeclaration);
            {
                Developer.LanguageServices.NativeX.SyntaxTree.NativeXDeclaration result1 = default(Developer.LanguageServices.NativeX.SyntaxTree.NativeXDeclaration);
                int currentIndex1 = -1;
                currentIndex1 = currentToken;
                {
                    {
                        Developer.LanguageServices.NativeX.SyntaxTree.NativeXFunctionDeclaration result2 = default(Developer.LanguageServices.NativeX.SyntaxTree.NativeXFunctionDeclaration);
                        int currentIndex2 = currentIndex1;
                        {
                            parseSuccess = false;
                            result2 = ParseFunctionDeclaration(tokens, ref currentIndex2, ref parseSuccess);
                        }
                        if (parseSuccess)
                        {
                            currentIndex1 = currentIndex2;
                            result1 = result2;
                        }
                    }
                }
                if (parseSuccess)
                {
                    currentToken = currentIndex1;
                    goto LABEL_SUCCESS_0;
                }
                currentIndex1 = currentToken;
                {
                    {
                        Developer.LanguageServices.NativeX.SyntaxTree.NativeXTypeRenameDeclaration result2 = default(Developer.LanguageServices.NativeX.SyntaxTree.NativeXTypeRenameDeclaration);
                        int currentIndex2 = currentIndex1;
                        {
                            parseSuccess = false;
                            result2 = ParseTypeRenameDeclaration(tokens, ref currentIndex2, ref parseSuccess);
                        }
                        if (parseSuccess)
                        {
                            currentIndex1 = currentIndex2;
                            result1 = result2;
                        }
                    }
                }
                if (parseSuccess)
                {
                    currentToken = currentIndex1;
                    goto LABEL_SUCCESS_0;
                }
                currentIndex1 = currentToken;
                {
                    {
                        Developer.LanguageServices.NativeX.SyntaxTree.NativeXVariableDeclaration result2 = default(Developer.LanguageServices.NativeX.SyntaxTree.NativeXVariableDeclaration);
                        int currentIndex2 = currentIndex1;
                        {
                            parseSuccess = false;
                            result2 = ParseVariableDeclaration(tokens, ref currentIndex2, ref parseSuccess);
                        }
                        if (parseSuccess)
                        {
                            currentIndex1 = currentIndex2;
                            result1 = result2;
                        }
                    }
                }
                if (parseSuccess)
                {
                    currentToken = currentIndex1;
                    goto LABEL_SUCCESS_0;
                }
                currentIndex1 = currentToken;
                {
                    {
                        Developer.LanguageServices.NativeX.SyntaxTree.NativeXStructureDeclaration result2 = default(Developer.LanguageServices.NativeX.SyntaxTree.NativeXStructureDeclaration);
                        int currentIndex2 = currentIndex1;
                        {
                            parseSuccess = false;
                            result2 = ParseStructureDeclaration(tokens, ref currentIndex2, ref parseSuccess);
                        }
                        if (parseSuccess)
                        {
                            currentIndex1 = currentIndex2;
                            result1 = result2;
                        }
                    }
                }
                if (parseSuccess)
                {
                    currentToken = currentIndex1;
                    goto LABEL_SUCCESS_0;
                }
                currentIndex1 = currentToken;
                {
                    {
                        Developer.LanguageServices.NativeX.SyntaxTree.NativeXInstanceDeclaration result2 = default(Developer.LanguageServices.NativeX.SyntaxTree.NativeXInstanceDeclaration);
                        int currentIndex2 = currentIndex1;
                        {
                            parseSuccess = false;
                            result2 = ParseInstanceDeclaration(tokens, ref currentIndex2, ref parseSuccess);
                        }
                        if (parseSuccess)
                        {
                            currentIndex1 = currentIndex2;
                            result1 = result2;
                        }
                    }
                }
                if (parseSuccess)
                {
                    currentToken = currentIndex1;
                    goto LABEL_SUCCESS_0;
                }
                currentIndex1 = currentToken;
                {
                    {
                        Developer.LanguageServices.NativeX.SyntaxTree.NativeXConceptDeclaration result2 = default(Developer.LanguageServices.NativeX.SyntaxTree.NativeXConceptDeclaration);
                        int currentIndex2 = currentIndex1;
                        {
                            parseSuccess = false;
                            result2 = ParseConceptDeclaration(tokens, ref currentIndex2, ref parseSuccess);
                        }
                        if (parseSuccess)
                        {
                            currentIndex1 = currentIndex2;
                            result1 = result2;
                        }
                    }
                }
                if (parseSuccess)
                {
                    currentToken = currentIndex1;
                    goto LABEL_SUCCESS_0;
                }
                goto LABEL_FAIL_1;
            LABEL_SUCCESS_0: ;
                result = result1;
            LABEL_FAIL_1: ;
            }
            if (parseSuccess)
            {
                if (result == null) result = CodeNode.Create<Developer.LanguageServices.NativeX.SyntaxTree.NativeXDeclaration>();
                result.Start = start;
                result.End = Developer.LanguageProvider.CodeTokenizer.GetEndPosition(tokens, currentToken);
            }
            return result;
        }

        public static Developer.LanguageServices.NativeX.SyntaxTree.NativeXFunctionDeclaration ParseFunctionDeclaration(List<CodeToken> tokens, ref int currentToken, ref bool parseSuccess)
        {
            Developer.LanguageProvider.TextPosition start = Developer.LanguageProvider.CodeTokenizer.GetStartPosition(tokens, currentToken);
            Developer.LanguageServices.NativeX.SyntaxTree.NativeXFunctionDeclaration result = default(Developer.LanguageServices.NativeX.SyntaxTree.NativeXFunctionDeclaration);
            System.Boolean ForeignMember0 = default(System.Boolean);
            Developer.LanguageServices.NativeX.SyntaxTree.NativeXType ReturnTypeMember0 = default(Developer.LanguageServices.NativeX.SyntaxTree.NativeXType);
            System.String NameMember0 = default(System.String);
            Developer.LanguageProvider.CodeNodeList<Developer.LanguageServices.NativeX.SyntaxTree.NativeXNameTypePair> ParametersMember0 = default(Developer.LanguageProvider.CodeNodeList<Developer.LanguageServices.NativeX.SyntaxTree.NativeXNameTypePair>);
            Developer.LanguageServices.NativeX.SyntaxTree.NativeXLinking LinkingMember0 = default(Developer.LanguageServices.NativeX.SyntaxTree.NativeXLinking);
            Developer.LanguageServices.NativeX.SyntaxTree.NativeXStatement StatementMember0 = default(Developer.LanguageServices.NativeX.SyntaxTree.NativeXStatement);
            {
                int currentIndexCopy1 = currentToken;
                int currentIndex1 = currentToken;
                {
                    System.Boolean ForeignMember1 = default(System.Boolean);
                    {
                        int currentIndex2 = -1;
                        currentIndex2 = currentIndex1;
                        {
                            System.Boolean ForeignMember2 = default(System.Boolean);
                            {
                                int currentIndex3 = currentIndex2;
                                {
                                    Developer.LanguageProvider.CodeTokenizer.ParseToken(tokens, ref currentIndex3, ref parseSuccess, "foreign");
                                }
                                if (parseSuccess)
                                {
                                    currentIndex2 = currentIndex3;
                                    ForeignMember2 = true;
                                }
                            }
                            ForeignMember1 = ForeignMember2;
                        }
                        if (parseSuccess)
                        {
                            currentIndex1 = currentIndex2;
                        }
                        parseSuccess = true;
                    }
                    ForeignMember0 = ForeignMember1;
                }
                if (parseSuccess)
                {
                    currentIndexCopy1 = currentIndex1;
                }
                else
                {
                    goto LABEL_0;
                }
                {
                    Developer.LanguageProvider.CodeTokenizer.ParseToken(tokens, ref currentIndex1, ref parseSuccess, "function");
                }
                if (parseSuccess)
                {
                    currentIndexCopy1 = currentIndex1;
                }
                else
                {
                    goto LABEL_0;
                }
                {
                    Developer.LanguageServices.NativeX.SyntaxTree.NativeXType ReturnTypeMember1 = default(Developer.LanguageServices.NativeX.SyntaxTree.NativeXType);
                    {
                        Developer.LanguageServices.NativeX.SyntaxTree.NativeXType result2 = default(Developer.LanguageServices.NativeX.SyntaxTree.NativeXType);
                        int currentIndex2 = currentIndex1;
                        {
                            parseSuccess = false;
                            result2 = ParseType(tokens, ref currentIndex2, ref parseSuccess);
                        }
                        if (parseSuccess)
                        {
                            currentIndex1 = currentIndex2;
                            ReturnTypeMember1 = result2;
                        }
                    }
                    ReturnTypeMember0 = ReturnTypeMember1;
                }
                if (parseSuccess)
                {
                    currentIndexCopy1 = currentIndex1;
                }
                else
                {
                    goto LABEL_0;
                }
                {
                    System.String NameMember1 = default(System.String);
                    {
                        Developer.LanguageProvider.CodeToken result2 = default(Developer.LanguageProvider.CodeToken);
                        int currentIndex2 = currentIndex1;
                        {
                            result2 = Developer.LanguageProvider.CodeTokenizer.ParseToken(tokens, ref currentIndex2, ref parseSuccess, Developer.LanguageServices.NativeX.NativeXTokenizer.IdToken);
                        }
                        if (parseSuccess)
                        {
                            currentIndex1 = currentIndex2;
                            NameMember1 = result2.Value;
                        }
                    }
                    NameMember0 = NameMember1;
                }
                if (parseSuccess)
                {
                    currentIndexCopy1 = currentIndex1;
                }
                else
                {
                    goto LABEL_0;
                }
                {
                    Developer.LanguageProvider.CodeTokenizer.ParseToken(tokens, ref currentIndex1, ref parseSuccess, "(");
                }
                if (parseSuccess)
                {
                    currentIndexCopy1 = currentIndex1;
                }
                else
                {
                    goto LABEL_0;
                }
                {
                    Developer.LanguageProvider.CodeNodeList<Developer.LanguageServices.NativeX.SyntaxTree.NativeXNameTypePair> ParametersMember1 = default(Developer.LanguageProvider.CodeNodeList<Developer.LanguageServices.NativeX.SyntaxTree.NativeXNameTypePair>);
                    {
                        Developer.LanguageProvider.CodeNodeList<Developer.LanguageServices.NativeX.SyntaxTree.NativeXNameTypePair> result2 = default(Developer.LanguageProvider.CodeNodeList<Developer.LanguageServices.NativeX.SyntaxTree.NativeXNameTypePair>);
                        int currentIndex2 = currentIndex1;
                        {
                            result2 = new Developer.LanguageProvider.CodeNodeList<Developer.LanguageServices.NativeX.SyntaxTree.NativeXNameTypePair>();
                            {
                                Developer.LanguageServices.NativeX.SyntaxTree.NativeXNameTypePair result3 = default(Developer.LanguageServices.NativeX.SyntaxTree.NativeXNameTypePair);
                                int currentIndex3 = currentIndex2;
                                {
                                    parseSuccess = false;
                                    result3 = ParseStructureMemberItem(tokens, ref currentIndex3, ref parseSuccess);
                                }
                                if (parseSuccess)
                                {
                                    currentIndex2 = currentIndex3;
                                    result2.Add(result3);
                                    result2.Start = result3.Start;
                                    result2.End = result3.End;
                                }
                                while (true)
                                {
                                    int currentIndexCopy3 = currentIndex2;
                                    {
                                        Developer.LanguageProvider.CodeTokenizer.ParseToken(tokens, ref currentIndex3, ref parseSuccess, ",");
                                    }
                                    if (parseSuccess)
                                    {
                                        currentIndexCopy3 = currentIndex3;
                                    }
                                    else
                                    {
                                        goto LABEL_1;
                                    }
                                    {
                                        parseSuccess = false;
                                        result3 = ParseStructureMemberItem(tokens, ref currentIndex3, ref parseSuccess);
                                    }
                                    if (parseSuccess)
                                    {
                                        currentIndexCopy3 = currentIndex3;
                                    }
                                    else
                                    {
                                        goto LABEL_1;
                                    }
                                    currentIndex2 = currentIndexCopy3;
                                    result2.Add(result3);
                                    result2.End = result3.End;
                                }
                            LABEL_1: ; parseSuccess = true;
                            }
                        }
                        if (parseSuccess)
                        {
                            currentIndex1 = currentIndex2;
                            ParametersMember1 = result2;
                        }
                    }
                    ParametersMember0 = ParametersMember1;
                }
                if (parseSuccess)
                {
                    currentIndexCopy1 = currentIndex1;
                }
                else
                {
                    goto LABEL_0;
                }
                {
                    Developer.LanguageProvider.CodeTokenizer.ParseToken(tokens, ref currentIndex1, ref parseSuccess, ")");
                }
                if (parseSuccess)
                {
                    currentIndexCopy1 = currentIndex1;
                }
                else
                {
                    goto LABEL_0;
                }
                {
                    Developer.LanguageServices.NativeX.SyntaxTree.NativeXLinking LinkingMember1 = default(Developer.LanguageServices.NativeX.SyntaxTree.NativeXLinking);
                    {
                        int currentIndex2 = -1;
                        currentIndex2 = currentIndex1;
                        {
                            Developer.LanguageServices.NativeX.SyntaxTree.NativeXLinking LinkingMember2 = default(Developer.LanguageServices.NativeX.SyntaxTree.NativeXLinking);
                            {
                                Developer.LanguageServices.NativeX.SyntaxTree.NativeXLinking result3 = default(Developer.LanguageServices.NativeX.SyntaxTree.NativeXLinking);
                                int currentIndex3 = currentIndex2;
                                {
                                    parseSuccess = false;
                                    result3 = ParseLinking(tokens, ref currentIndex3, ref parseSuccess);
                                }
                                if (parseSuccess)
                                {
                                    currentIndex2 = currentIndex3;
                                    LinkingMember2 = result3;
                                }
                            }
                            LinkingMember1 = LinkingMember2;
                        }
                        if (parseSuccess)
                        {
                            currentIndex1 = currentIndex2;
                        }
                        parseSuccess = true;
                    }
                    LinkingMember0 = LinkingMember1;
                }
                if (parseSuccess)
                {
                    currentIndexCopy1 = currentIndex1;
                }
                else
                {
                    goto LABEL_0;
                }
                {
                    Developer.LanguageServices.NativeX.SyntaxTree.NativeXStatement StatementMember1 = default(Developer.LanguageServices.NativeX.SyntaxTree.NativeXStatement);
                    {
                        int currentIndex2 = -1;
                        currentIndex2 = currentIndex1;
                        {
                            Developer.LanguageProvider.CodeTokenizer.ParseToken(tokens, ref currentIndex2, ref parseSuccess, ";");
                        }
                        if (parseSuccess)
                        {
                            currentIndex1 = currentIndex2;
                            goto LABEL_SUCCESS_2;
                        }
                        currentIndex2 = currentIndex1;
                        {
                            Developer.LanguageServices.NativeX.SyntaxTree.NativeXStatement StatementMember2 = default(Developer.LanguageServices.NativeX.SyntaxTree.NativeXStatement);
                            {
                                Developer.LanguageServices.NativeX.SyntaxTree.NativeXStatement result3 = default(Developer.LanguageServices.NativeX.SyntaxTree.NativeXStatement);
                                int currentIndex3 = currentIndex2;
                                {
                                    parseSuccess = false;
                                    result3 = ParseStatement(tokens, ref currentIndex3, ref parseSuccess);
                                }
                                if (parseSuccess)
                                {
                                    currentIndex2 = currentIndex3;
                                    StatementMember2 = result3;
                                }
                            }
                            StatementMember1 = StatementMember2;
                        }
                        if (parseSuccess)
                        {
                            currentIndex1 = currentIndex2;
                            goto LABEL_SUCCESS_2;
                        }
                        goto LABEL_FAIL_3;
                    LABEL_SUCCESS_2: ;
                    LABEL_FAIL_3: ;
                    }
                    StatementMember0 = StatementMember1;
                }
                if (parseSuccess)
                {
                    currentIndexCopy1 = currentIndex1;
                }
                else
                {
                    goto LABEL_0;
                }
                currentToken = currentIndexCopy1;
            LABEL_0: ;
            }
            if (parseSuccess)
            {
                if (result == null) result = CodeNode.Create<Developer.LanguageServices.NativeX.SyntaxTree.NativeXFunctionDeclaration>();
                result.Foreign = ForeignMember0;
                result.ReturnType = ReturnTypeMember0;
                result.Name = NameMember0;
                result.Parameters = ParametersMember0;
                result.Linking = LinkingMember0;
                result.Statement = StatementMember0;
                result.Start = start;
                result.End = Developer.LanguageProvider.CodeTokenizer.GetEndPosition(tokens, currentToken);
            }
            return result;
        }

        public static Developer.LanguageServices.NativeX.SyntaxTree.NativeXType ParseType(List<CodeToken> tokens, ref int currentToken, ref bool parseSuccess)
        {
            Developer.LanguageProvider.TextPosition start = Developer.LanguageProvider.CodeTokenizer.GetStartPosition(tokens, currentToken);
            Developer.LanguageServices.NativeX.SyntaxTree.NativeXType result = default(Developer.LanguageServices.NativeX.SyntaxTree.NativeXType);
            {
                Developer.LanguageServices.NativeX.SyntaxTree.NativeXType result1 = default(Developer.LanguageServices.NativeX.SyntaxTree.NativeXType);
                int currentIndex1 = currentToken;
                {
                    {
                        int currentIndex2 = currentIndex1;
                        {
                            parseSuccess = false;
                            result1 = ParsePrimitiveType(tokens, ref currentIndex2, ref parseSuccess);
                        }
                        if (parseSuccess)
                        {
                            currentIndex1 = currentIndex2;
                        }
                        else
                        {
                            goto LABEL_0;
                        }
                        while (true)
                        {
                            {
                                {
                                    Developer.LanguageProvider.CodeTokenizer.ParseToken(tokens, ref currentIndex2, ref parseSuccess, "*");
                                }
                                if (parseSuccess)
                                {
                                    currentIndex1 = currentIndex2;
                                    Developer.LanguageServices.NativeX.SyntaxTree.NativeXPointerType result2 = CodeNode.Create<Developer.LanguageServices.NativeX.SyntaxTree.NativeXPointerType>();
                                    result2.ElementType = result1;
                                    result2.Start = result1.Start;
                                    result2.End = Developer.LanguageProvider.CodeTokenizer.GetEndPosition(tokens, currentIndex1);
                                    result1 = result2;
                                    continue;
                                }
                            }
                            {
                                Developer.LanguageServices.NativeX.SyntaxTree.NativeXPrimitiveExpression SizeMember2 = default(Developer.LanguageServices.NativeX.SyntaxTree.NativeXPrimitiveExpression);
                                {
                                    Developer.LanguageServices.NativeX.SyntaxTree.NativeXPrimitiveExpression SizeMember3 = default(Developer.LanguageServices.NativeX.SyntaxTree.NativeXPrimitiveExpression);
                                    {
                                        int currentIndexCopy4 = currentIndex2;
                                        int currentIndex4 = currentIndex2;
                                        {
                                            Developer.LanguageProvider.CodeTokenizer.ParseToken(tokens, ref currentIndex4, ref parseSuccess, "[");
                                        }
                                        if (parseSuccess)
                                        {
                                            currentIndexCopy4 = currentIndex4;
                                        }
                                        else
                                        {
                                            goto LABEL_1;
                                        }
                                        {
                                            Developer.LanguageServices.NativeX.SyntaxTree.NativeXPrimitiveExpression SizeMember4 = default(Developer.LanguageServices.NativeX.SyntaxTree.NativeXPrimitiveExpression);
                                            {
                                                Developer.LanguageServices.NativeX.SyntaxTree.NativeXPrimitiveExpression result5 = default(Developer.LanguageServices.NativeX.SyntaxTree.NativeXPrimitiveExpression);
                                                int currentIndex5 = currentIndex4;
                                                {
                                                    parseSuccess = false;
                                                    result5 = ParsePrimitive(tokens, ref currentIndex5, ref parseSuccess);
                                                }
                                                if (parseSuccess)
                                                {
                                                    currentIndex4 = currentIndex5;
                                                    SizeMember4 = result5;
                                                }
                                            }
                                            SizeMember3 = SizeMember4;
                                        }
                                        if (parseSuccess)
                                        {
                                            currentIndexCopy4 = currentIndex4;
                                        }
                                        else
                                        {
                                            goto LABEL_1;
                                        }
                                        {
                                            Developer.LanguageProvider.CodeTokenizer.ParseToken(tokens, ref currentIndex4, ref parseSuccess, "]");
                                        }
                                        if (parseSuccess)
                                        {
                                            currentIndexCopy4 = currentIndex4;
                                        }
                                        else
                                        {
                                            goto LABEL_1;
                                        }
                                        currentIndex2 = currentIndexCopy4;
                                    LABEL_1: ;
                                    }
                                    SizeMember2 = SizeMember3;
                                }
                                if (parseSuccess)
                                {
                                    currentIndex1 = currentIndex2;
                                    Developer.LanguageServices.NativeX.SyntaxTree.NativeXArrayType result2 = CodeNode.Create<Developer.LanguageServices.NativeX.SyntaxTree.NativeXArrayType>();
                                    result2.Size = SizeMember2;
                                    result2.ElementType = result1;
                                    result2.Start = result1.Start;
                                    result2.End = Developer.LanguageProvider.CodeTokenizer.GetEndPosition(tokens, currentIndex1);
                                    result1 = result2;
                                    continue;
                                }
                            }
                            break;
                        }
                        parseSuccess = true;
                    LABEL_0: ;
                    }
                }
                if (parseSuccess)
                {
                    currentToken = currentIndex1;
                    result = result1;
                }
            }
            if (parseSuccess)
            {
                if (result == null) result = CodeNode.Create<Developer.LanguageServices.NativeX.SyntaxTree.NativeXType>();
                result.Start = start;
                result.End = Developer.LanguageProvider.CodeTokenizer.GetEndPosition(tokens, currentToken);
            }
            return result;
        }

        public static Developer.LanguageServices.NativeX.SyntaxTree.NativeXType ParsePrimitiveType(List<CodeToken> tokens, ref int currentToken, ref bool parseSuccess)
        {
            Developer.LanguageProvider.TextPosition start = Developer.LanguageProvider.CodeTokenizer.GetStartPosition(tokens, currentToken);
            Developer.LanguageServices.NativeX.SyntaxTree.NativeXType result = default(Developer.LanguageServices.NativeX.SyntaxTree.NativeXType);
            {
                Developer.LanguageServices.NativeX.SyntaxTree.NativeXType result1 = default(Developer.LanguageServices.NativeX.SyntaxTree.NativeXType);
                int currentIndex1 = -1;
                currentIndex1 = currentToken;
                {
                    {
                        Developer.LanguageServices.NativeX.SyntaxTree.NativeXFunctionType result2 = default(Developer.LanguageServices.NativeX.SyntaxTree.NativeXFunctionType);
                        int currentIndex2 = currentIndex1;
                        {
                            parseSuccess = false;
                            result2 = ParseFunctionType(tokens, ref currentIndex2, ref parseSuccess);
                        }
                        if (parseSuccess)
                        {
                            currentIndex1 = currentIndex2;
                            result1 = result2;
                        }
                    }
                }
                if (parseSuccess)
                {
                    currentToken = currentIndex1;
                    goto LABEL_SUCCESS_0;
                }
                currentIndex1 = currentToken;
                {
                    {
                        Developer.LanguageServices.NativeX.SyntaxTree.NativeXInstanciatedType result2 = default(Developer.LanguageServices.NativeX.SyntaxTree.NativeXInstanciatedType);
                        int currentIndex2 = currentIndex1;
                        {
                            parseSuccess = false;
                            result2 = ParseInstanciatedType(tokens, ref currentIndex2, ref parseSuccess);
                        }
                        if (parseSuccess)
                        {
                            currentIndex1 = currentIndex2;
                            result1 = result2;
                        }
                    }
                }
                if (parseSuccess)
                {
                    currentToken = currentIndex1;
                    goto LABEL_SUCCESS_0;
                }
                currentIndex1 = currentToken;
                {
                    {
                        Developer.LanguageServices.NativeX.SyntaxTree.NativeXReferenceType result2 = default(Developer.LanguageServices.NativeX.SyntaxTree.NativeXReferenceType);
                        int currentIndex2 = currentIndex1;
                        {
                            parseSuccess = false;
                            result2 = ParseReferenceType(tokens, ref currentIndex2, ref parseSuccess);
                        }
                        if (parseSuccess)
                        {
                            currentIndex1 = currentIndex2;
                            result1 = result2;
                        }
                    }
                }
                if (parseSuccess)
                {
                    currentToken = currentIndex1;
                    goto LABEL_SUCCESS_0;
                }
                goto LABEL_FAIL_1;
            LABEL_SUCCESS_0: ;
                result = result1;
            LABEL_FAIL_1: ;
            }
            if (parseSuccess)
            {
                if (result == null) result = CodeNode.Create<Developer.LanguageServices.NativeX.SyntaxTree.NativeXType>();
                result.Start = start;
                result.End = Developer.LanguageProvider.CodeTokenizer.GetEndPosition(tokens, currentToken);
            }
            return result;
        }

        public static Developer.LanguageServices.NativeX.SyntaxTree.NativeXFunctionType ParseFunctionType(List<CodeToken> tokens, ref int currentToken, ref bool parseSuccess)
        {
            Developer.LanguageProvider.TextPosition start = Developer.LanguageProvider.CodeTokenizer.GetStartPosition(tokens, currentToken);
            Developer.LanguageServices.NativeX.SyntaxTree.NativeXFunctionType result = default(Developer.LanguageServices.NativeX.SyntaxTree.NativeXFunctionType);
            Developer.LanguageServices.NativeX.SyntaxTree.NativeXType ReturnTypeMember0 = default(Developer.LanguageServices.NativeX.SyntaxTree.NativeXType);
            Developer.LanguageProvider.CodeNodeList<Developer.LanguageServices.NativeX.SyntaxTree.NativeXType> ParametersMember0 = default(Developer.LanguageProvider.CodeNodeList<Developer.LanguageServices.NativeX.SyntaxTree.NativeXType>);
            {
                int currentIndexCopy1 = currentToken;
                int currentIndex1 = currentToken;
                {
                    Developer.LanguageProvider.CodeTokenizer.ParseToken(tokens, ref currentIndex1, ref parseSuccess, "function");
                }
                if (parseSuccess)
                {
                    currentIndexCopy1 = currentIndex1;
                }
                else
                {
                    goto LABEL_0;
                }
                {
                    Developer.LanguageServices.NativeX.SyntaxTree.NativeXType ReturnTypeMember1 = default(Developer.LanguageServices.NativeX.SyntaxTree.NativeXType);
                    {
                        Developer.LanguageServices.NativeX.SyntaxTree.NativeXType result2 = default(Developer.LanguageServices.NativeX.SyntaxTree.NativeXType);
                        int currentIndex2 = currentIndex1;
                        {
                            parseSuccess = false;
                            result2 = ParseType(tokens, ref currentIndex2, ref parseSuccess);
                        }
                        if (parseSuccess)
                        {
                            currentIndex1 = currentIndex2;
                            ReturnTypeMember1 = result2;
                        }
                    }
                    ReturnTypeMember0 = ReturnTypeMember1;
                }
                if (parseSuccess)
                {
                    currentIndexCopy1 = currentIndex1;
                }
                else
                {
                    goto LABEL_0;
                }
                {
                    Developer.LanguageProvider.CodeTokenizer.ParseToken(tokens, ref currentIndex1, ref parseSuccess, "(");
                }
                if (parseSuccess)
                {
                    currentIndexCopy1 = currentIndex1;
                }
                else
                {
                    goto LABEL_0;
                }
                {
                    Developer.LanguageProvider.CodeNodeList<Developer.LanguageServices.NativeX.SyntaxTree.NativeXType> ParametersMember1 = default(Developer.LanguageProvider.CodeNodeList<Developer.LanguageServices.NativeX.SyntaxTree.NativeXType>);
                    {
                        Developer.LanguageProvider.CodeNodeList<Developer.LanguageServices.NativeX.SyntaxTree.NativeXType> result2 = default(Developer.LanguageProvider.CodeNodeList<Developer.LanguageServices.NativeX.SyntaxTree.NativeXType>);
                        int currentIndex2 = currentIndex1;
                        {
                            result2 = new Developer.LanguageProvider.CodeNodeList<Developer.LanguageServices.NativeX.SyntaxTree.NativeXType>();
                            {
                                Developer.LanguageServices.NativeX.SyntaxTree.NativeXType result3 = default(Developer.LanguageServices.NativeX.SyntaxTree.NativeXType);
                                int currentIndex3 = currentIndex2;
                                {
                                    parseSuccess = false;
                                    result3 = ParseType(tokens, ref currentIndex3, ref parseSuccess);
                                }
                                if (parseSuccess)
                                {
                                    currentIndex2 = currentIndex3;
                                    result2.Add(result3);
                                    result2.Start = result3.Start;
                                    result2.End = result3.End;
                                }
                                while (true)
                                {
                                    int currentIndexCopy3 = currentIndex2;
                                    {
                                        Developer.LanguageProvider.CodeTokenizer.ParseToken(tokens, ref currentIndex3, ref parseSuccess, ",");
                                    }
                                    if (parseSuccess)
                                    {
                                        currentIndexCopy3 = currentIndex3;
                                    }
                                    else
                                    {
                                        goto LABEL_1;
                                    }
                                    {
                                        parseSuccess = false;
                                        result3 = ParseType(tokens, ref currentIndex3, ref parseSuccess);
                                    }
                                    if (parseSuccess)
                                    {
                                        currentIndexCopy3 = currentIndex3;
                                    }
                                    else
                                    {
                                        goto LABEL_1;
                                    }
                                    currentIndex2 = currentIndexCopy3;
                                    result2.Add(result3);
                                    result2.End = result3.End;
                                }
                            LABEL_1: ; parseSuccess = true;
                            }
                        }
                        if (parseSuccess)
                        {
                            currentIndex1 = currentIndex2;
                            ParametersMember1 = result2;
                        }
                    }
                    ParametersMember0 = ParametersMember1;
                }
                if (parseSuccess)
                {
                    currentIndexCopy1 = currentIndex1;
                }
                else
                {
                    goto LABEL_0;
                }
                {
                    Developer.LanguageProvider.CodeTokenizer.ParseToken(tokens, ref currentIndex1, ref parseSuccess, ")");
                }
                if (parseSuccess)
                {
                    currentIndexCopy1 = currentIndex1;
                }
                else
                {
                    goto LABEL_0;
                }
                currentToken = currentIndexCopy1;
            LABEL_0: ;
            }
            if (parseSuccess)
            {
                if (result == null) result = CodeNode.Create<Developer.LanguageServices.NativeX.SyntaxTree.NativeXFunctionType>();
                result.ReturnType = ReturnTypeMember0;
                result.Parameters = ParametersMember0;
                result.Start = start;
                result.End = Developer.LanguageProvider.CodeTokenizer.GetEndPosition(tokens, currentToken);
            }
            return result;
        }

        public static Developer.LanguageServices.NativeX.SyntaxTree.NativeXInstanciatedType ParseInstanciatedType(List<CodeToken> tokens, ref int currentToken, ref bool parseSuccess)
        {
            Developer.LanguageProvider.TextPosition start = Developer.LanguageProvider.CodeTokenizer.GetStartPosition(tokens, currentToken);
            Developer.LanguageServices.NativeX.SyntaxTree.NativeXInstanciatedType result = default(Developer.LanguageServices.NativeX.SyntaxTree.NativeXInstanciatedType);
            Developer.LanguageServices.NativeX.SyntaxTree.NativeXReferenceType ElementTypeMember0 = default(Developer.LanguageServices.NativeX.SyntaxTree.NativeXReferenceType);
            Developer.LanguageProvider.CodeNodeList<Developer.LanguageServices.NativeX.SyntaxTree.NativeXType> GenericArgumentsMember0 = default(Developer.LanguageProvider.CodeNodeList<Developer.LanguageServices.NativeX.SyntaxTree.NativeXType>);
            {
                int currentIndexCopy1 = currentToken;
                int currentIndex1 = currentToken;
                {
                    Developer.LanguageServices.NativeX.SyntaxTree.NativeXReferenceType ElementTypeMember1 = default(Developer.LanguageServices.NativeX.SyntaxTree.NativeXReferenceType);
                    {
                        Developer.LanguageServices.NativeX.SyntaxTree.NativeXReferenceType result2 = default(Developer.LanguageServices.NativeX.SyntaxTree.NativeXReferenceType);
                        int currentIndex2 = currentIndex1;
                        {
                            parseSuccess = false;
                            result2 = ParseReferenceType(tokens, ref currentIndex2, ref parseSuccess);
                        }
                        if (parseSuccess)
                        {
                            currentIndex1 = currentIndex2;
                            ElementTypeMember1 = result2;
                        }
                    }
                    ElementTypeMember0 = ElementTypeMember1;
                }
                if (parseSuccess)
                {
                    currentIndexCopy1 = currentIndex1;
                }
                else
                {
                    goto LABEL_0;
                }
                {
                    Developer.LanguageProvider.CodeTokenizer.ParseToken(tokens, ref currentIndex1, ref parseSuccess, "<");
                }
                if (parseSuccess)
                {
                    currentIndexCopy1 = currentIndex1;
                }
                else
                {
                    goto LABEL_0;
                }
                {
                    Developer.LanguageProvider.CodeNodeList<Developer.LanguageServices.NativeX.SyntaxTree.NativeXType> GenericArgumentsMember1 = default(Developer.LanguageProvider.CodeNodeList<Developer.LanguageServices.NativeX.SyntaxTree.NativeXType>);
                    {
                        Developer.LanguageProvider.CodeNodeList<Developer.LanguageServices.NativeX.SyntaxTree.NativeXType> result2 = default(Developer.LanguageProvider.CodeNodeList<Developer.LanguageServices.NativeX.SyntaxTree.NativeXType>);
                        int currentIndex2 = currentIndex1;
                        {
                            result2 = new Developer.LanguageProvider.CodeNodeList<Developer.LanguageServices.NativeX.SyntaxTree.NativeXType>();
                            {
                                Developer.LanguageServices.NativeX.SyntaxTree.NativeXType result3 = default(Developer.LanguageServices.NativeX.SyntaxTree.NativeXType);
                                int currentIndex3 = currentIndex2;
                                {
                                    parseSuccess = false;
                                    result3 = ParseType(tokens, ref currentIndex3, ref parseSuccess);
                                }
                                if (parseSuccess)
                                {
                                    currentIndex2 = currentIndex3;
                                    result2.Add(result3);
                                    result2.Start = result3.Start;
                                    result2.End = result3.End;
                                }
                                while (true)
                                {
                                    int currentIndexCopy3 = currentIndex2;
                                    {
                                        Developer.LanguageProvider.CodeTokenizer.ParseToken(tokens, ref currentIndex3, ref parseSuccess, ",");
                                    }
                                    if (parseSuccess)
                                    {
                                        currentIndexCopy3 = currentIndex3;
                                    }
                                    else
                                    {
                                        goto LABEL_1;
                                    }
                                    {
                                        parseSuccess = false;
                                        result3 = ParseType(tokens, ref currentIndex3, ref parseSuccess);
                                    }
                                    if (parseSuccess)
                                    {
                                        currentIndexCopy3 = currentIndex3;
                                    }
                                    else
                                    {
                                        goto LABEL_1;
                                    }
                                    currentIndex2 = currentIndexCopy3;
                                    result2.Add(result3);
                                    result2.End = result3.End;
                                }
                            LABEL_1: ; parseSuccess = true;
                            }
                        }
                        if (parseSuccess)
                        {
                            currentIndex1 = currentIndex2;
                            GenericArgumentsMember1 = result2;
                        }
                    }
                    GenericArgumentsMember0 = GenericArgumentsMember1;
                }
                if (parseSuccess)
                {
                    currentIndexCopy1 = currentIndex1;
                }
                else
                {
                    goto LABEL_0;
                }
                {
                    Developer.LanguageProvider.CodeTokenizer.ParseToken(tokens, ref currentIndex1, ref parseSuccess, ">");
                }
                if (parseSuccess)
                {
                    currentIndexCopy1 = currentIndex1;
                }
                else
                {
                    goto LABEL_0;
                }
                currentToken = currentIndexCopy1;
            LABEL_0: ;
            }
            if (parseSuccess)
            {
                if (result == null) result = CodeNode.Create<Developer.LanguageServices.NativeX.SyntaxTree.NativeXInstanciatedType>();
                result.ElementType = ElementTypeMember0;
                result.GenericArguments = GenericArgumentsMember0;
                result.Start = start;
                result.End = Developer.LanguageProvider.CodeTokenizer.GetEndPosition(tokens, currentToken);
            }
            return result;
        }

        public static Developer.LanguageServices.NativeX.SyntaxTree.NativeXReferenceType ParseReferenceType(List<CodeToken> tokens, ref int currentToken, ref bool parseSuccess)
        {
            Developer.LanguageProvider.TextPosition start = Developer.LanguageProvider.CodeTokenizer.GetStartPosition(tokens, currentToken);
            Developer.LanguageServices.NativeX.SyntaxTree.NativeXReferenceType result = default(Developer.LanguageServices.NativeX.SyntaxTree.NativeXReferenceType);
            System.String ReferencedNameMember0 = default(System.String);
            {
                int currentIndex1 = -1;
                currentIndex1 = currentToken;
                {
                    System.String ReferencedNameMember1 = default(System.String);
                    {
                        Developer.LanguageProvider.CodeToken result2 = default(Developer.LanguageProvider.CodeToken);
                        int currentIndex2 = currentIndex1;
                        {
                            result2 = Developer.LanguageProvider.CodeTokenizer.ParseToken(tokens, ref currentIndex2, ref parseSuccess, Developer.LanguageServices.NativeX.NativeXTokenizer.IdToken);
                        }
                        if (parseSuccess)
                        {
                            currentIndex1 = currentIndex2;
                            ReferencedNameMember1 = result2.Value;
                        }
                    }
                    ReferencedNameMember0 = ReferencedNameMember1;
                }
                if (parseSuccess)
                {
                    currentToken = currentIndex1;
                    goto LABEL_SUCCESS_0;
                }
                currentIndex1 = currentToken;
                {
                    System.String ReferencedNameMember1 = default(System.String);
                    {
                        Developer.LanguageProvider.CodeToken result2 = default(Developer.LanguageProvider.CodeToken);
                        int currentIndex2 = currentIndex1;
                        {
                            result2 = Developer.LanguageProvider.CodeTokenizer.ParseToken(tokens, ref currentIndex2, ref parseSuccess, Developer.LanguageServices.NativeX.NativeXTokenizer.TypeKeywordToken);
                        }
                        if (parseSuccess)
                        {
                            currentIndex1 = currentIndex2;
                            ReferencedNameMember1 = result2.Value;
                        }
                    }
                    ReferencedNameMember0 = ReferencedNameMember1;
                }
                if (parseSuccess)
                {
                    currentToken = currentIndex1;
                    goto LABEL_SUCCESS_0;
                }
                goto LABEL_FAIL_1;
            LABEL_SUCCESS_0: ;
            LABEL_FAIL_1: ;
            }
            if (parseSuccess)
            {
                if (result == null) result = CodeNode.Create<Developer.LanguageServices.NativeX.SyntaxTree.NativeXReferenceType>();
                result.ReferencedName = ReferencedNameMember0;
                result.Start = start;
                result.End = Developer.LanguageProvider.CodeTokenizer.GetEndPosition(tokens, currentToken);
            }
            return result;
        }

        public static Developer.LanguageServices.NativeX.SyntaxTree.NativeXPrimitiveExpression ParsePrimitive(List<CodeToken> tokens, ref int currentToken, ref bool parseSuccess)
        {
            Developer.LanguageProvider.TextPosition start = Developer.LanguageProvider.CodeTokenizer.GetStartPosition(tokens, currentToken);
            Developer.LanguageServices.NativeX.SyntaxTree.NativeXPrimitiveExpression result = default(Developer.LanguageServices.NativeX.SyntaxTree.NativeXPrimitiveExpression);
            System.String CodeMember0 = default(System.String);
            {
                int currentIndex1 = -1;
                currentIndex1 = currentToken;
                {
                    System.String CodeMember1 = default(System.String);
                    {
                        Developer.LanguageProvider.CodeToken result2 = default(Developer.LanguageProvider.CodeToken);
                        int currentIndex2 = currentIndex1;
                        {
                            result2 = Developer.LanguageProvider.CodeTokenizer.ParseToken(tokens, ref currentIndex2, ref parseSuccess, Developer.LanguageServices.NativeX.NativeXTokenizer.StringToken);
                        }
                        if (parseSuccess)
                        {
                            currentIndex1 = currentIndex2;
                            CodeMember1 = result2.Value;
                        }
                    }
                    CodeMember0 = CodeMember1;
                }
                if (parseSuccess)
                {
                    currentToken = currentIndex1;
                    goto LABEL_SUCCESS_0;
                }
                currentIndex1 = currentToken;
                {
                    System.String CodeMember1 = default(System.String);
                    {
                        Developer.LanguageProvider.CodeToken result2 = default(Developer.LanguageProvider.CodeToken);
                        int currentIndex2 = currentIndex1;
                        {
                            result2 = Developer.LanguageProvider.CodeTokenizer.ParseToken(tokens, ref currentIndex2, ref parseSuccess, Developer.LanguageServices.NativeX.NativeXTokenizer.NumberToken);
                        }
                        if (parseSuccess)
                        {
                            currentIndex1 = currentIndex2;
                            CodeMember1 = result2.Value;
                        }
                    }
                    CodeMember0 = CodeMember1;
                }
                if (parseSuccess)
                {
                    currentToken = currentIndex1;
                    goto LABEL_SUCCESS_0;
                }
                currentIndex1 = currentToken;
                {
                    System.String CodeMember1 = default(System.String);
                    {
                        Developer.LanguageProvider.CodeToken result2 = default(Developer.LanguageProvider.CodeToken);
                        int currentIndex2 = currentIndex1;
                        {
                            result2 = Developer.LanguageProvider.CodeTokenizer.ParseToken(tokens, ref currentIndex2, ref parseSuccess, "true", "false", "null");
                        }
                        if (parseSuccess)
                        {
                            currentIndex1 = currentIndex2;
                            CodeMember1 = result2.Value;
                        }
                    }
                    CodeMember0 = CodeMember1;
                }
                if (parseSuccess)
                {
                    currentToken = currentIndex1;
                    goto LABEL_SUCCESS_0;
                }
                goto LABEL_FAIL_1;
            LABEL_SUCCESS_0: ;
            LABEL_FAIL_1: ;
            }
            if (parseSuccess)
            {
                if (result == null) result = CodeNode.Create<Developer.LanguageServices.NativeX.SyntaxTree.NativeXPrimitiveExpression>();
                result.Code = CodeMember0;
                result.Start = start;
                result.End = Developer.LanguageProvider.CodeTokenizer.GetEndPosition(tokens, currentToken);
            }
            return result;
        }

        public static Developer.LanguageServices.NativeX.SyntaxTree.NativeXNameTypePair ParseStructureMemberItem(List<CodeToken> tokens, ref int currentToken, ref bool parseSuccess)
        {
            Developer.LanguageProvider.TextPosition start = Developer.LanguageProvider.CodeTokenizer.GetStartPosition(tokens, currentToken);
            Developer.LanguageServices.NativeX.SyntaxTree.NativeXNameTypePair result = default(Developer.LanguageServices.NativeX.SyntaxTree.NativeXNameTypePair);
            Developer.LanguageServices.NativeX.SyntaxTree.NativeXType TypeMember0 = default(Developer.LanguageServices.NativeX.SyntaxTree.NativeXType);
            System.String NameMember0 = default(System.String);
            {
                int currentIndexCopy1 = currentToken;
                int currentIndex1 = currentToken;
                {
                    Developer.LanguageServices.NativeX.SyntaxTree.NativeXType TypeMember1 = default(Developer.LanguageServices.NativeX.SyntaxTree.NativeXType);
                    {
                        Developer.LanguageServices.NativeX.SyntaxTree.NativeXType result2 = default(Developer.LanguageServices.NativeX.SyntaxTree.NativeXType);
                        int currentIndex2 = currentIndex1;
                        {
                            parseSuccess = false;
                            result2 = ParseType(tokens, ref currentIndex2, ref parseSuccess);
                        }
                        if (parseSuccess)
                        {
                            currentIndex1 = currentIndex2;
                            TypeMember1 = result2;
                        }
                    }
                    TypeMember0 = TypeMember1;
                }
                if (parseSuccess)
                {
                    currentIndexCopy1 = currentIndex1;
                }
                else
                {
                    goto LABEL_0;
                }
                {
                    System.String NameMember1 = default(System.String);
                    {
                        Developer.LanguageProvider.CodeToken result2 = default(Developer.LanguageProvider.CodeToken);
                        int currentIndex2 = currentIndex1;
                        {
                            result2 = Developer.LanguageProvider.CodeTokenizer.ParseToken(tokens, ref currentIndex2, ref parseSuccess, Developer.LanguageServices.NativeX.NativeXTokenizer.IdToken);
                        }
                        if (parseSuccess)
                        {
                            currentIndex1 = currentIndex2;
                            NameMember1 = result2.Value;
                        }
                    }
                    NameMember0 = NameMember1;
                }
                if (parseSuccess)
                {
                    currentIndexCopy1 = currentIndex1;
                }
                else
                {
                    goto LABEL_0;
                }
                currentToken = currentIndexCopy1;
            LABEL_0: ;
            }
            if (parseSuccess)
            {
                if (result == null) result = CodeNode.Create<Developer.LanguageServices.NativeX.SyntaxTree.NativeXNameTypePair>();
                result.Type = TypeMember0;
                result.Name = NameMember0;
                result.Start = start;
                result.End = Developer.LanguageProvider.CodeTokenizer.GetEndPosition(tokens, currentToken);
            }
            return result;
        }

        public static Developer.LanguageServices.NativeX.SyntaxTree.NativeXLinking ParseLinking(List<CodeToken> tokens, ref int currentToken, ref bool parseSuccess)
        {
            Developer.LanguageProvider.TextPosition start = Developer.LanguageProvider.CodeTokenizer.GetStartPosition(tokens, currentToken);
            Developer.LanguageServices.NativeX.SyntaxTree.NativeXLinking result = default(Developer.LanguageServices.NativeX.SyntaxTree.NativeXLinking);
            System.String LinkingAssemblyMember0 = default(System.String);
            System.String LinkingSymbolMember0 = default(System.String);
            {
                int currentIndexCopy1 = currentToken;
                int currentIndex1 = currentToken;
                {
                    Developer.LanguageProvider.CodeTokenizer.ParseToken(tokens, ref currentIndex1, ref parseSuccess, "alias");
                }
                if (parseSuccess)
                {
                    currentIndexCopy1 = currentIndex1;
                }
                else
                {
                    goto LABEL_0;
                }
                {
                    System.String LinkingAssemblyMember1 = default(System.String);
                    {
                        Developer.LanguageProvider.CodeToken result2 = default(Developer.LanguageProvider.CodeToken);
                        int currentIndex2 = currentIndex1;
                        {
                            result2 = Developer.LanguageProvider.CodeTokenizer.ParseToken(tokens, ref currentIndex2, ref parseSuccess, Developer.LanguageServices.NativeX.NativeXTokenizer.IdToken);
                        }
                        if (parseSuccess)
                        {
                            currentIndex1 = currentIndex2;
                            LinkingAssemblyMember1 = result2.Value;
                        }
                    }
                    LinkingAssemblyMember0 = LinkingAssemblyMember1;
                }
                if (parseSuccess)
                {
                    currentIndexCopy1 = currentIndex1;
                }
                else
                {
                    goto LABEL_0;
                }
                {
                    Developer.LanguageProvider.CodeTokenizer.ParseToken(tokens, ref currentIndex1, ref parseSuccess, ".");
                }
                if (parseSuccess)
                {
                    currentIndexCopy1 = currentIndex1;
                }
                else
                {
                    goto LABEL_0;
                }
                {
                    System.String LinkingSymbolMember1 = default(System.String);
                    {
                        Developer.LanguageProvider.CodeToken result2 = default(Developer.LanguageProvider.CodeToken);
                        int currentIndex2 = currentIndex1;
                        {
                            result2 = Developer.LanguageProvider.CodeTokenizer.ParseToken(tokens, ref currentIndex2, ref parseSuccess, Developer.LanguageServices.NativeX.NativeXTokenizer.IdToken);
                        }
                        if (parseSuccess)
                        {
                            currentIndex1 = currentIndex2;
                            LinkingSymbolMember1 = result2.Value;
                        }
                    }
                    LinkingSymbolMember0 = LinkingSymbolMember1;
                }
                if (parseSuccess)
                {
                    currentIndexCopy1 = currentIndex1;
                }
                else
                {
                    goto LABEL_0;
                }
                currentToken = currentIndexCopy1;
            LABEL_0: ;
            }
            if (parseSuccess)
            {
                if (result == null) result = CodeNode.Create<Developer.LanguageServices.NativeX.SyntaxTree.NativeXLinking>();
                result.LinkingAssembly = LinkingAssemblyMember0;
                result.LinkingSymbol = LinkingSymbolMember0;
                result.Start = start;
                result.End = Developer.LanguageProvider.CodeTokenizer.GetEndPosition(tokens, currentToken);
            }
            return result;
        }

        public static Developer.LanguageServices.NativeX.SyntaxTree.NativeXStatement ParseStatement(List<CodeToken> tokens, ref int currentToken, ref bool parseSuccess)
        {
            Developer.LanguageProvider.TextPosition start = Developer.LanguageProvider.CodeTokenizer.GetStartPosition(tokens, currentToken);
            Developer.LanguageServices.NativeX.SyntaxTree.NativeXStatement result = default(Developer.LanguageServices.NativeX.SyntaxTree.NativeXStatement);
            {
                Developer.LanguageServices.NativeX.SyntaxTree.NativeXStatement result1 = default(Developer.LanguageServices.NativeX.SyntaxTree.NativeXStatement);
                int currentIndex1 = -1;
                currentIndex1 = currentToken;
                {
                    {
                        Developer.LanguageServices.NativeX.SyntaxTree.NativeXEmptyStatement result2 = default(Developer.LanguageServices.NativeX.SyntaxTree.NativeXEmptyStatement);
                        int currentIndex2 = currentIndex1;
                        {
                            parseSuccess = false;
                            result2 = ParseEmptyStatement(tokens, ref currentIndex2, ref parseSuccess);
                        }
                        if (parseSuccess)
                        {
                            currentIndex1 = currentIndex2;
                            result1 = result2;
                        }
                    }
                }
                if (parseSuccess)
                {
                    currentToken = currentIndex1;
                    goto LABEL_SUCCESS_0;
                }
                currentIndex1 = currentToken;
                {
                    {
                        Developer.LanguageServices.NativeX.SyntaxTree.NativeXVariableStatement result2 = default(Developer.LanguageServices.NativeX.SyntaxTree.NativeXVariableStatement);
                        int currentIndex2 = currentIndex1;
                        {
                            parseSuccess = false;
                            result2 = ParseVariableStatement(tokens, ref currentIndex2, ref parseSuccess);
                        }
                        if (parseSuccess)
                        {
                            currentIndex1 = currentIndex2;
                            result1 = result2;
                        }
                    }
                }
                if (parseSuccess)
                {
                    currentToken = currentIndex1;
                    goto LABEL_SUCCESS_0;
                }
                currentIndex1 = currentToken;
                {
                    {
                        Developer.LanguageServices.NativeX.SyntaxTree.NativeXIfStatement result2 = default(Developer.LanguageServices.NativeX.SyntaxTree.NativeXIfStatement);
                        int currentIndex2 = currentIndex1;
                        {
                            parseSuccess = false;
                            result2 = ParseIfStatement(tokens, ref currentIndex2, ref parseSuccess);
                        }
                        if (parseSuccess)
                        {
                            currentIndex1 = currentIndex2;
                            result1 = result2;
                        }
                    }
                }
                if (parseSuccess)
                {
                    currentToken = currentIndex1;
                    goto LABEL_SUCCESS_0;
                }
                currentIndex1 = currentToken;
                {
                    {
                        Developer.LanguageServices.NativeX.SyntaxTree.NativeXBreakStatement result2 = default(Developer.LanguageServices.NativeX.SyntaxTree.NativeXBreakStatement);
                        int currentIndex2 = currentIndex1;
                        {
                            parseSuccess = false;
                            result2 = ParseBreakStatement(tokens, ref currentIndex2, ref parseSuccess);
                        }
                        if (parseSuccess)
                        {
                            currentIndex1 = currentIndex2;
                            result1 = result2;
                        }
                    }
                }
                if (parseSuccess)
                {
                    currentToken = currentIndex1;
                    goto LABEL_SUCCESS_0;
                }
                currentIndex1 = currentToken;
                {
                    {
                        Developer.LanguageServices.NativeX.SyntaxTree.NativeXContinueStatement result2 = default(Developer.LanguageServices.NativeX.SyntaxTree.NativeXContinueStatement);
                        int currentIndex2 = currentIndex1;
                        {
                            parseSuccess = false;
                            result2 = ParseContinueStatement(tokens, ref currentIndex2, ref parseSuccess);
                        }
                        if (parseSuccess)
                        {
                            currentIndex1 = currentIndex2;
                            result1 = result2;
                        }
                    }
                }
                if (parseSuccess)
                {
                    currentToken = currentIndex1;
                    goto LABEL_SUCCESS_0;
                }
                currentIndex1 = currentToken;
                {
                    {
                        Developer.LanguageServices.NativeX.SyntaxTree.NativeXReturnStatement result2 = default(Developer.LanguageServices.NativeX.SyntaxTree.NativeXReturnStatement);
                        int currentIndex2 = currentIndex1;
                        {
                            parseSuccess = false;
                            result2 = ParseExitStatement(tokens, ref currentIndex2, ref parseSuccess);
                        }
                        if (parseSuccess)
                        {
                            currentIndex1 = currentIndex2;
                            result1 = result2;
                        }
                    }
                }
                if (parseSuccess)
                {
                    currentToken = currentIndex1;
                    goto LABEL_SUCCESS_0;
                }
                currentIndex1 = currentToken;
                {
                    {
                        Developer.LanguageServices.NativeX.SyntaxTree.NativeXCompositeStatement result2 = default(Developer.LanguageServices.NativeX.SyntaxTree.NativeXCompositeStatement);
                        int currentIndex2 = currentIndex1;
                        {
                            parseSuccess = false;
                            result2 = ParseCompositeStatement(tokens, ref currentIndex2, ref parseSuccess);
                        }
                        if (parseSuccess)
                        {
                            currentIndex1 = currentIndex2;
                            result1 = result2;
                        }
                    }
                }
                if (parseSuccess)
                {
                    currentToken = currentIndex1;
                    goto LABEL_SUCCESS_0;
                }
                currentIndex1 = currentToken;
                {
                    {
                        Developer.LanguageServices.NativeX.SyntaxTree.NativeXWhileStatement result2 = default(Developer.LanguageServices.NativeX.SyntaxTree.NativeXWhileStatement);
                        int currentIndex2 = currentIndex1;
                        {
                            parseSuccess = false;
                            result2 = ParseDoWhileStatement(tokens, ref currentIndex2, ref parseSuccess);
                        }
                        if (parseSuccess)
                        {
                            currentIndex1 = currentIndex2;
                            result1 = result2;
                        }
                    }
                }
                if (parseSuccess)
                {
                    currentToken = currentIndex1;
                    goto LABEL_SUCCESS_0;
                }
                currentIndex1 = currentToken;
                {
                    {
                        Developer.LanguageServices.NativeX.SyntaxTree.NativeXWhileStatement result2 = default(Developer.LanguageServices.NativeX.SyntaxTree.NativeXWhileStatement);
                        int currentIndex2 = currentIndex1;
                        {
                            parseSuccess = false;
                            result2 = ParseLoopStatement(tokens, ref currentIndex2, ref parseSuccess);
                        }
                        if (parseSuccess)
                        {
                            currentIndex1 = currentIndex2;
                            result1 = result2;
                        }
                    }
                }
                if (parseSuccess)
                {
                    currentToken = currentIndex1;
                    goto LABEL_SUCCESS_0;
                }
                currentIndex1 = currentToken;
                {
                    {
                        Developer.LanguageServices.NativeX.SyntaxTree.NativeXWhileStatement result2 = default(Developer.LanguageServices.NativeX.SyntaxTree.NativeXWhileStatement);
                        int currentIndex2 = currentIndex1;
                        {
                            parseSuccess = false;
                            result2 = ParseWhileDoStatement(tokens, ref currentIndex2, ref parseSuccess);
                        }
                        if (parseSuccess)
                        {
                            currentIndex1 = currentIndex2;
                            result1 = result2;
                        }
                    }
                }
                if (parseSuccess)
                {
                    currentToken = currentIndex1;
                    goto LABEL_SUCCESS_0;
                }
                currentIndex1 = currentToken;
                {
                    {
                        Developer.LanguageServices.NativeX.SyntaxTree.NativeXForStatement result2 = default(Developer.LanguageServices.NativeX.SyntaxTree.NativeXForStatement);
                        int currentIndex2 = currentIndex1;
                        {
                            parseSuccess = false;
                            result2 = ParseForStatement(tokens, ref currentIndex2, ref parseSuccess);
                        }
                        if (parseSuccess)
                        {
                            currentIndex1 = currentIndex2;
                            result1 = result2;
                        }
                    }
                }
                if (parseSuccess)
                {
                    currentToken = currentIndex1;
                    goto LABEL_SUCCESS_0;
                }
                currentIndex1 = currentToken;
                {
                    {
                        Developer.LanguageServices.NativeX.SyntaxTree.NativeXTryCatchStatement result2 = default(Developer.LanguageServices.NativeX.SyntaxTree.NativeXTryCatchStatement);
                        int currentIndex2 = currentIndex1;
                        {
                            parseSuccess = false;
                            result2 = ParseTryCatchStatement(tokens, ref currentIndex2, ref parseSuccess);
                        }
                        if (parseSuccess)
                        {
                            currentIndex1 = currentIndex2;
                            result1 = result2;
                        }
                    }
                }
                if (parseSuccess)
                {
                    currentToken = currentIndex1;
                    goto LABEL_SUCCESS_0;
                }
                currentIndex1 = currentToken;
                {
                    {
                        Developer.LanguageServices.NativeX.SyntaxTree.NativeXThrowStatement result2 = default(Developer.LanguageServices.NativeX.SyntaxTree.NativeXThrowStatement);
                        int currentIndex2 = currentIndex1;
                        {
                            parseSuccess = false;
                            result2 = ParseThrowStatement(tokens, ref currentIndex2, ref parseSuccess);
                        }
                        if (parseSuccess)
                        {
                            currentIndex1 = currentIndex2;
                            result1 = result2;
                        }
                    }
                }
                if (parseSuccess)
                {
                    currentToken = currentIndex1;
                    goto LABEL_SUCCESS_0;
                }
                currentIndex1 = currentToken;
                {
                    {
                        Developer.LanguageServices.NativeX.SyntaxTree.NativeXExpressionStatement result2 = default(Developer.LanguageServices.NativeX.SyntaxTree.NativeXExpressionStatement);
                        int currentIndex2 = currentIndex1;
                        {
                            parseSuccess = false;
                            result2 = ParseExpressionStatement(tokens, ref currentIndex2, ref parseSuccess);
                        }
                        if (parseSuccess)
                        {
                            currentIndex1 = currentIndex2;
                            result1 = result2;
                        }
                    }
                }
                if (parseSuccess)
                {
                    currentToken = currentIndex1;
                    goto LABEL_SUCCESS_0;
                }
                goto LABEL_FAIL_1;
            LABEL_SUCCESS_0: ;
                result = result1;
            LABEL_FAIL_1: ;
            }
            if (parseSuccess)
            {
                if (result == null) result = CodeNode.Create<Developer.LanguageServices.NativeX.SyntaxTree.NativeXStatement>();
                result.Start = start;
                result.End = Developer.LanguageProvider.CodeTokenizer.GetEndPosition(tokens, currentToken);
            }
            return result;
        }

        public static Developer.LanguageServices.NativeX.SyntaxTree.NativeXEmptyStatement ParseEmptyStatement(List<CodeToken> tokens, ref int currentToken, ref bool parseSuccess)
        {
            Developer.LanguageProvider.TextPosition start = Developer.LanguageProvider.CodeTokenizer.GetStartPosition(tokens, currentToken);
            Developer.LanguageServices.NativeX.SyntaxTree.NativeXEmptyStatement result = default(Developer.LanguageServices.NativeX.SyntaxTree.NativeXEmptyStatement);
            Developer.LanguageProvider.CodeTokenizer.ParseToken(tokens, ref currentToken, ref parseSuccess, ";");
            if (parseSuccess)
            {
                if (result == null) result = CodeNode.Create<Developer.LanguageServices.NativeX.SyntaxTree.NativeXEmptyStatement>();
                result.Start = start;
                result.End = Developer.LanguageProvider.CodeTokenizer.GetEndPosition(tokens, currentToken);
            }
            return result;
        }

        public static Developer.LanguageServices.NativeX.SyntaxTree.NativeXVariableStatement ParseVariableStatement(List<CodeToken> tokens, ref int currentToken, ref bool parseSuccess)
        {
            Developer.LanguageProvider.TextPosition start = Developer.LanguageProvider.CodeTokenizer.GetStartPosition(tokens, currentToken);
            Developer.LanguageServices.NativeX.SyntaxTree.NativeXVariableStatement result = default(Developer.LanguageServices.NativeX.SyntaxTree.NativeXVariableStatement);
            Developer.LanguageServices.NativeX.SyntaxTree.NativeXType TypeMember0 = default(Developer.LanguageServices.NativeX.SyntaxTree.NativeXType);
            System.String NameMember0 = default(System.String);
            Developer.LanguageServices.NativeX.SyntaxTree.NativeXExpression InitializerMember0 = default(Developer.LanguageServices.NativeX.SyntaxTree.NativeXExpression);
            {
                int currentIndexCopy1 = currentToken;
                int currentIndex1 = currentToken;
                {
                    Developer.LanguageProvider.CodeTokenizer.ParseToken(tokens, ref currentIndex1, ref parseSuccess, "variable");
                }
                if (parseSuccess)
                {
                    currentIndexCopy1 = currentIndex1;
                }
                else
                {
                    goto LABEL_0;
                }
                {
                    Developer.LanguageServices.NativeX.SyntaxTree.NativeXType TypeMember1 = default(Developer.LanguageServices.NativeX.SyntaxTree.NativeXType);
                    {
                        Developer.LanguageServices.NativeX.SyntaxTree.NativeXType result2 = default(Developer.LanguageServices.NativeX.SyntaxTree.NativeXType);
                        int currentIndex2 = currentIndex1;
                        {
                            parseSuccess = false;
                            result2 = ParseType(tokens, ref currentIndex2, ref parseSuccess);
                        }
                        if (parseSuccess)
                        {
                            currentIndex1 = currentIndex2;
                            TypeMember1 = result2;
                        }
                    }
                    TypeMember0 = TypeMember1;
                }
                if (parseSuccess)
                {
                    currentIndexCopy1 = currentIndex1;
                }
                else
                {
                    goto LABEL_0;
                }
                {
                    System.String NameMember1 = default(System.String);
                    {
                        Developer.LanguageProvider.CodeToken result2 = default(Developer.LanguageProvider.CodeToken);
                        int currentIndex2 = currentIndex1;
                        {
                            result2 = Developer.LanguageProvider.CodeTokenizer.ParseToken(tokens, ref currentIndex2, ref parseSuccess, Developer.LanguageServices.NativeX.NativeXTokenizer.IdToken);
                        }
                        if (parseSuccess)
                        {
                            currentIndex1 = currentIndex2;
                            NameMember1 = result2.Value;
                        }
                    }
                    NameMember0 = NameMember1;
                }
                if (parseSuccess)
                {
                    currentIndexCopy1 = currentIndex1;
                }
                else
                {
                    goto LABEL_0;
                }
                {
                    Developer.LanguageServices.NativeX.SyntaxTree.NativeXExpression InitializerMember1 = default(Developer.LanguageServices.NativeX.SyntaxTree.NativeXExpression);
                    {
                        int currentIndex2 = -1;
                        currentIndex2 = currentIndex1;
                        {
                            Developer.LanguageServices.NativeX.SyntaxTree.NativeXExpression InitializerMember2 = default(Developer.LanguageServices.NativeX.SyntaxTree.NativeXExpression);
                            {
                                int currentIndexCopy3 = currentIndex2;
                                int currentIndex3 = currentIndex2;
                                {
                                    Developer.LanguageProvider.CodeTokenizer.ParseToken(tokens, ref currentIndex3, ref parseSuccess, "=");
                                }
                                if (parseSuccess)
                                {
                                    currentIndexCopy3 = currentIndex3;
                                }
                                else
                                {
                                    goto LABEL_1;
                                }
                                {
                                    Developer.LanguageServices.NativeX.SyntaxTree.NativeXExpression InitializerMember3 = default(Developer.LanguageServices.NativeX.SyntaxTree.NativeXExpression);
                                    {
                                        Developer.LanguageServices.NativeX.SyntaxTree.NativeXExpression result4 = default(Developer.LanguageServices.NativeX.SyntaxTree.NativeXExpression);
                                        int currentIndex4 = currentIndex3;
                                        {
                                            parseSuccess = false;
                                            result4 = ParseExpression(tokens, ref currentIndex4, ref parseSuccess);
                                        }
                                        if (parseSuccess)
                                        {
                                            currentIndex3 = currentIndex4;
                                            InitializerMember3 = result4;
                                        }
                                    }
                                    InitializerMember2 = InitializerMember3;
                                }
                                if (parseSuccess)
                                {
                                    currentIndexCopy3 = currentIndex3;
                                }
                                else
                                {
                                    goto LABEL_1;
                                }
                                currentIndex2 = currentIndexCopy3;
                            LABEL_1: ;
                            }
                            InitializerMember1 = InitializerMember2;
                        }
                        if (parseSuccess)
                        {
                            currentIndex1 = currentIndex2;
                        }
                        parseSuccess = true;
                    }
                    InitializerMember0 = InitializerMember1;
                }
                if (parseSuccess)
                {
                    currentIndexCopy1 = currentIndex1;
                }
                else
                {
                    goto LABEL_0;
                }
                {
                    Developer.LanguageProvider.CodeTokenizer.ParseToken(tokens, ref currentIndex1, ref parseSuccess, ";");
                }
                if (parseSuccess)
                {
                    currentIndexCopy1 = currentIndex1;
                }
                else
                {
                    goto LABEL_0;
                }
                currentToken = currentIndexCopy1;
            LABEL_0: ;
            }
            if (parseSuccess)
            {
                if (result == null) result = CodeNode.Create<Developer.LanguageServices.NativeX.SyntaxTree.NativeXVariableStatement>();
                result.Type = TypeMember0;
                result.Name = NameMember0;
                result.Initializer = InitializerMember0;
                result.Start = start;
                result.End = Developer.LanguageProvider.CodeTokenizer.GetEndPosition(tokens, currentToken);
            }
            return result;
        }

        public static Developer.LanguageServices.NativeX.SyntaxTree.NativeXExpression ParseExpression(List<CodeToken> tokens, ref int currentToken, ref bool parseSuccess)
        {
            Developer.LanguageProvider.TextPosition start = Developer.LanguageProvider.CodeTokenizer.GetStartPosition(tokens, currentToken);
            Developer.LanguageServices.NativeX.SyntaxTree.NativeXExpression result = default(Developer.LanguageServices.NativeX.SyntaxTree.NativeXExpression);
            {
                Developer.LanguageServices.NativeX.SyntaxTree.NativeXExpression result1 = default(Developer.LanguageServices.NativeX.SyntaxTree.NativeXExpression);
                int currentIndex1 = currentToken;
                {
                    parseSuccess = false;
                    result1 = ParseEXP13(tokens, ref currentIndex1, ref parseSuccess);
                }
                if (parseSuccess)
                {
                    currentToken = currentIndex1;
                    result = result1;
                }
            }
            if (parseSuccess)
            {
                if (result == null) result = CodeNode.Create<Developer.LanguageServices.NativeX.SyntaxTree.NativeXExpression>();
                result.Start = start;
                result.End = Developer.LanguageProvider.CodeTokenizer.GetEndPosition(tokens, currentToken);
            }
            return result;
        }

        public static Developer.LanguageServices.NativeX.SyntaxTree.NativeXExpression ParseEXP13(List<CodeToken> tokens, ref int currentToken, ref bool parseSuccess)
        {
            Developer.LanguageProvider.TextPosition start = Developer.LanguageProvider.CodeTokenizer.GetStartPosition(tokens, currentToken);
            Developer.LanguageServices.NativeX.SyntaxTree.NativeXExpression result = default(Developer.LanguageServices.NativeX.SyntaxTree.NativeXExpression);
            {
                Developer.LanguageServices.NativeX.SyntaxTree.NativeXExpression result1 = default(Developer.LanguageServices.NativeX.SyntaxTree.NativeXExpression);
                int currentIndex1 = currentToken;
                {
                    {
                        int currentIndex2 = currentIndex1;
                        {
                            parseSuccess = false;
                            result1 = ParseEXP12(tokens, ref currentIndex2, ref parseSuccess);
                        }
                        if (parseSuccess)
                        {
                            currentIndex1 = currentIndex2;
                        }
                        else
                        {
                            goto LABEL_0;
                        }
                        while (true)
                        {
                            System.String OperatorMember2 = default(System.String);
                            Developer.LanguageServices.NativeX.SyntaxTree.NativeXExpression RightOperandMember2 = default(Developer.LanguageServices.NativeX.SyntaxTree.NativeXExpression);
                            {
                                System.String OperatorMember3 = default(System.String);
                                Developer.LanguageServices.NativeX.SyntaxTree.NativeXExpression RightOperandMember3 = default(Developer.LanguageServices.NativeX.SyntaxTree.NativeXExpression);
                                {
                                    int currentIndexCopy4 = currentIndex2;
                                    int currentIndex4 = currentIndex2;
                                    {
                                        System.String OperatorMember4 = default(System.String);
                                        {
                                            Developer.LanguageProvider.CodeToken result5 = default(Developer.LanguageProvider.CodeToken);
                                            int currentIndex5 = currentIndex4;
                                            {
                                                result5 = Developer.LanguageProvider.CodeTokenizer.ParseToken(tokens, ref currentIndex5, ref parseSuccess, "+=", "-=", "*=", "/=", "%=", "<<=", ">>=", "&=", "/=", "&&=", "||=", "=");
                                            }
                                            if (parseSuccess)
                                            {
                                                currentIndex4 = currentIndex5;
                                                OperatorMember4 = result5.Value;
                                            }
                                        }
                                        OperatorMember3 = OperatorMember4;
                                    }
                                    if (parseSuccess)
                                    {
                                        currentIndexCopy4 = currentIndex4;
                                    }
                                    else
                                    {
                                        goto LABEL_1;
                                    }
                                    {
                                        Developer.LanguageServices.NativeX.SyntaxTree.NativeXExpression RightOperandMember4 = default(Developer.LanguageServices.NativeX.SyntaxTree.NativeXExpression);
                                        {
                                            Developer.LanguageServices.NativeX.SyntaxTree.NativeXExpression result5 = default(Developer.LanguageServices.NativeX.SyntaxTree.NativeXExpression);
                                            int currentIndex5 = currentIndex4;
                                            {
                                                parseSuccess = false;
                                                result5 = ParseEXP12(tokens, ref currentIndex5, ref parseSuccess);
                                            }
                                            if (parseSuccess)
                                            {
                                                currentIndex4 = currentIndex5;
                                                RightOperandMember4 = result5;
                                            }
                                        }
                                        RightOperandMember3 = RightOperandMember4;
                                    }
                                    if (parseSuccess)
                                    {
                                        currentIndexCopy4 = currentIndex4;
                                    }
                                    else
                                    {
                                        goto LABEL_1;
                                    }
                                    currentIndex2 = currentIndexCopy4;
                                LABEL_1: ;
                                }
                                OperatorMember2 = OperatorMember3;
                                RightOperandMember2 = RightOperandMember3;
                            }
                            if (parseSuccess)
                            {
                                currentIndex1 = currentIndex2;
                                Developer.LanguageServices.NativeX.SyntaxTree.NativeXBinaryExpression result2 = CodeNode.Create<Developer.LanguageServices.NativeX.SyntaxTree.NativeXBinaryExpression>();
                                result2.Operator = OperatorMember2;
                                result2.RightOperand = RightOperandMember2;
                                result2.LeftOperand = result1;
                                result2.Start = result1.Start;
                                result2.End = Developer.LanguageProvider.CodeTokenizer.GetEndPosition(tokens, currentIndex1);
                                result1 = result2;
                            }
                            else
                            {
                                break;
                            }
                        }
                        parseSuccess = true;
                    LABEL_0: ;
                    }
                }
                if (parseSuccess)
                {
                    currentToken = currentIndex1;
                    result = result1;
                }
            }
            if (parseSuccess)
            {
                if (result == null) result = CodeNode.Create<Developer.LanguageServices.NativeX.SyntaxTree.NativeXExpression>();
                result.Start = start;
                result.End = Developer.LanguageProvider.CodeTokenizer.GetEndPosition(tokens, currentToken);
            }
            return result;
        }

        public static Developer.LanguageServices.NativeX.SyntaxTree.NativeXExpression ParseEXP12(List<CodeToken> tokens, ref int currentToken, ref bool parseSuccess)
        {
            Developer.LanguageProvider.TextPosition start = Developer.LanguageProvider.CodeTokenizer.GetStartPosition(tokens, currentToken);
            Developer.LanguageServices.NativeX.SyntaxTree.NativeXExpression result = default(Developer.LanguageServices.NativeX.SyntaxTree.NativeXExpression);
            {
                Developer.LanguageServices.NativeX.SyntaxTree.NativeXExpression result1 = default(Developer.LanguageServices.NativeX.SyntaxTree.NativeXExpression);
                int currentIndex1 = currentToken;
                {
                    {
                        int currentIndex2 = currentIndex1;
                        {
                            parseSuccess = false;
                            result1 = ParseEXP11(tokens, ref currentIndex2, ref parseSuccess);
                        }
                        if (parseSuccess)
                        {
                            currentIndex1 = currentIndex2;
                        }
                        else
                        {
                            goto LABEL_0;
                        }
                        while (true)
                        {
                            System.String OperatorMember2 = default(System.String);
                            Developer.LanguageServices.NativeX.SyntaxTree.NativeXExpression RightOperandMember2 = default(Developer.LanguageServices.NativeX.SyntaxTree.NativeXExpression);
                            {
                                System.String OperatorMember3 = default(System.String);
                                Developer.LanguageServices.NativeX.SyntaxTree.NativeXExpression RightOperandMember3 = default(Developer.LanguageServices.NativeX.SyntaxTree.NativeXExpression);
                                {
                                    int currentIndexCopy4 = currentIndex2;
                                    int currentIndex4 = currentIndex2;
                                    {
                                        System.String OperatorMember4 = default(System.String);
                                        {
                                            Developer.LanguageProvider.CodeToken result5 = default(Developer.LanguageProvider.CodeToken);
                                            int currentIndex5 = currentIndex4;
                                            {
                                                result5 = Developer.LanguageProvider.CodeTokenizer.ParseToken(tokens, ref currentIndex5, ref parseSuccess, "||");
                                            }
                                            if (parseSuccess)
                                            {
                                                currentIndex4 = currentIndex5;
                                                OperatorMember4 = result5.Value;
                                            }
                                        }
                                        OperatorMember3 = OperatorMember4;
                                    }
                                    if (parseSuccess)
                                    {
                                        currentIndexCopy4 = currentIndex4;
                                    }
                                    else
                                    {
                                        goto LABEL_1;
                                    }
                                    {
                                        Developer.LanguageServices.NativeX.SyntaxTree.NativeXExpression RightOperandMember4 = default(Developer.LanguageServices.NativeX.SyntaxTree.NativeXExpression);
                                        {
                                            Developer.LanguageServices.NativeX.SyntaxTree.NativeXExpression result5 = default(Developer.LanguageServices.NativeX.SyntaxTree.NativeXExpression);
                                            int currentIndex5 = currentIndex4;
                                            {
                                                parseSuccess = false;
                                                result5 = ParseEXP11(tokens, ref currentIndex5, ref parseSuccess);
                                            }
                                            if (parseSuccess)
                                            {
                                                currentIndex4 = currentIndex5;
                                                RightOperandMember4 = result5;
                                            }
                                        }
                                        RightOperandMember3 = RightOperandMember4;
                                    }
                                    if (parseSuccess)
                                    {
                                        currentIndexCopy4 = currentIndex4;
                                    }
                                    else
                                    {
                                        goto LABEL_1;
                                    }
                                    currentIndex2 = currentIndexCopy4;
                                LABEL_1: ;
                                }
                                OperatorMember2 = OperatorMember3;
                                RightOperandMember2 = RightOperandMember3;
                            }
                            if (parseSuccess)
                            {
                                currentIndex1 = currentIndex2;
                                Developer.LanguageServices.NativeX.SyntaxTree.NativeXBinaryExpression result2 = CodeNode.Create<Developer.LanguageServices.NativeX.SyntaxTree.NativeXBinaryExpression>();
                                result2.Operator = OperatorMember2;
                                result2.RightOperand = RightOperandMember2;
                                result2.LeftOperand = result1;
                                result2.Start = result1.Start;
                                result2.End = Developer.LanguageProvider.CodeTokenizer.GetEndPosition(tokens, currentIndex1);
                                result1 = result2;
                            }
                            else
                            {
                                break;
                            }
                        }
                        parseSuccess = true;
                    LABEL_0: ;
                    }
                }
                if (parseSuccess)
                {
                    currentToken = currentIndex1;
                    result = result1;
                }
            }
            if (parseSuccess)
            {
                if (result == null) result = CodeNode.Create<Developer.LanguageServices.NativeX.SyntaxTree.NativeXExpression>();
                result.Start = start;
                result.End = Developer.LanguageProvider.CodeTokenizer.GetEndPosition(tokens, currentToken);
            }
            return result;
        }

        public static Developer.LanguageServices.NativeX.SyntaxTree.NativeXExpression ParseEXP11(List<CodeToken> tokens, ref int currentToken, ref bool parseSuccess)
        {
            Developer.LanguageProvider.TextPosition start = Developer.LanguageProvider.CodeTokenizer.GetStartPosition(tokens, currentToken);
            Developer.LanguageServices.NativeX.SyntaxTree.NativeXExpression result = default(Developer.LanguageServices.NativeX.SyntaxTree.NativeXExpression);
            {
                Developer.LanguageServices.NativeX.SyntaxTree.NativeXExpression result1 = default(Developer.LanguageServices.NativeX.SyntaxTree.NativeXExpression);
                int currentIndex1 = currentToken;
                {
                    {
                        int currentIndex2 = currentIndex1;
                        {
                            parseSuccess = false;
                            result1 = ParseEXP10(tokens, ref currentIndex2, ref parseSuccess);
                        }
                        if (parseSuccess)
                        {
                            currentIndex1 = currentIndex2;
                        }
                        else
                        {
                            goto LABEL_0;
                        }
                        while (true)
                        {
                            System.String OperatorMember2 = default(System.String);
                            Developer.LanguageServices.NativeX.SyntaxTree.NativeXExpression RightOperandMember2 = default(Developer.LanguageServices.NativeX.SyntaxTree.NativeXExpression);
                            {
                                System.String OperatorMember3 = default(System.String);
                                Developer.LanguageServices.NativeX.SyntaxTree.NativeXExpression RightOperandMember3 = default(Developer.LanguageServices.NativeX.SyntaxTree.NativeXExpression);
                                {
                                    int currentIndexCopy4 = currentIndex2;
                                    int currentIndex4 = currentIndex2;
                                    {
                                        System.String OperatorMember4 = default(System.String);
                                        {
                                            Developer.LanguageProvider.CodeToken result5 = default(Developer.LanguageProvider.CodeToken);
                                            int currentIndex5 = currentIndex4;
                                            {
                                                result5 = Developer.LanguageProvider.CodeTokenizer.ParseToken(tokens, ref currentIndex5, ref parseSuccess, "&&");
                                            }
                                            if (parseSuccess)
                                            {
                                                currentIndex4 = currentIndex5;
                                                OperatorMember4 = result5.Value;
                                            }
                                        }
                                        OperatorMember3 = OperatorMember4;
                                    }
                                    if (parseSuccess)
                                    {
                                        currentIndexCopy4 = currentIndex4;
                                    }
                                    else
                                    {
                                        goto LABEL_1;
                                    }
                                    {
                                        Developer.LanguageServices.NativeX.SyntaxTree.NativeXExpression RightOperandMember4 = default(Developer.LanguageServices.NativeX.SyntaxTree.NativeXExpression);
                                        {
                                            Developer.LanguageServices.NativeX.SyntaxTree.NativeXExpression result5 = default(Developer.LanguageServices.NativeX.SyntaxTree.NativeXExpression);
                                            int currentIndex5 = currentIndex4;
                                            {
                                                parseSuccess = false;
                                                result5 = ParseEXP10(tokens, ref currentIndex5, ref parseSuccess);
                                            }
                                            if (parseSuccess)
                                            {
                                                currentIndex4 = currentIndex5;
                                                RightOperandMember4 = result5;
                                            }
                                        }
                                        RightOperandMember3 = RightOperandMember4;
                                    }
                                    if (parseSuccess)
                                    {
                                        currentIndexCopy4 = currentIndex4;
                                    }
                                    else
                                    {
                                        goto LABEL_1;
                                    }
                                    currentIndex2 = currentIndexCopy4;
                                LABEL_1: ;
                                }
                                OperatorMember2 = OperatorMember3;
                                RightOperandMember2 = RightOperandMember3;
                            }
                            if (parseSuccess)
                            {
                                currentIndex1 = currentIndex2;
                                Developer.LanguageServices.NativeX.SyntaxTree.NativeXBinaryExpression result2 = CodeNode.Create<Developer.LanguageServices.NativeX.SyntaxTree.NativeXBinaryExpression>();
                                result2.Operator = OperatorMember2;
                                result2.RightOperand = RightOperandMember2;
                                result2.LeftOperand = result1;
                                result2.Start = result1.Start;
                                result2.End = Developer.LanguageProvider.CodeTokenizer.GetEndPosition(tokens, currentIndex1);
                                result1 = result2;
                            }
                            else
                            {
                                break;
                            }
                        }
                        parseSuccess = true;
                    LABEL_0: ;
                    }
                }
                if (parseSuccess)
                {
                    currentToken = currentIndex1;
                    result = result1;
                }
            }
            if (parseSuccess)
            {
                if (result == null) result = CodeNode.Create<Developer.LanguageServices.NativeX.SyntaxTree.NativeXExpression>();
                result.Start = start;
                result.End = Developer.LanguageProvider.CodeTokenizer.GetEndPosition(tokens, currentToken);
            }
            return result;
        }

        public static Developer.LanguageServices.NativeX.SyntaxTree.NativeXExpression ParseEXP10(List<CodeToken> tokens, ref int currentToken, ref bool parseSuccess)
        {
            Developer.LanguageProvider.TextPosition start = Developer.LanguageProvider.CodeTokenizer.GetStartPosition(tokens, currentToken);
            Developer.LanguageServices.NativeX.SyntaxTree.NativeXExpression result = default(Developer.LanguageServices.NativeX.SyntaxTree.NativeXExpression);
            {
                Developer.LanguageServices.NativeX.SyntaxTree.NativeXExpression result1 = default(Developer.LanguageServices.NativeX.SyntaxTree.NativeXExpression);
                int currentIndex1 = currentToken;
                {
                    {
                        int currentIndex2 = currentIndex1;
                        {
                            parseSuccess = false;
                            result1 = ParseEXP9(tokens, ref currentIndex2, ref parseSuccess);
                        }
                        if (parseSuccess)
                        {
                            currentIndex1 = currentIndex2;
                        }
                        else
                        {
                            goto LABEL_0;
                        }
                        while (true)
                        {
                            System.String OperatorMember2 = default(System.String);
                            Developer.LanguageServices.NativeX.SyntaxTree.NativeXExpression RightOperandMember2 = default(Developer.LanguageServices.NativeX.SyntaxTree.NativeXExpression);
                            {
                                System.String OperatorMember3 = default(System.String);
                                Developer.LanguageServices.NativeX.SyntaxTree.NativeXExpression RightOperandMember3 = default(Developer.LanguageServices.NativeX.SyntaxTree.NativeXExpression);
                                {
                                    int currentIndexCopy4 = currentIndex2;
                                    int currentIndex4 = currentIndex2;
                                    {
                                        System.String OperatorMember4 = default(System.String);
                                        {
                                            Developer.LanguageProvider.CodeToken result5 = default(Developer.LanguageProvider.CodeToken);
                                            int currentIndex5 = currentIndex4;
                                            {
                                                result5 = Developer.LanguageProvider.CodeTokenizer.ParseToken(tokens, ref currentIndex5, ref parseSuccess, "|");
                                            }
                                            if (parseSuccess)
                                            {
                                                currentIndex4 = currentIndex5;
                                                OperatorMember4 = result5.Value;
                                            }
                                        }
                                        OperatorMember3 = OperatorMember4;
                                    }
                                    if (parseSuccess)
                                    {
                                        currentIndexCopy4 = currentIndex4;
                                    }
                                    else
                                    {
                                        goto LABEL_1;
                                    }
                                    {
                                        Developer.LanguageServices.NativeX.SyntaxTree.NativeXExpression RightOperandMember4 = default(Developer.LanguageServices.NativeX.SyntaxTree.NativeXExpression);
                                        {
                                            Developer.LanguageServices.NativeX.SyntaxTree.NativeXExpression result5 = default(Developer.LanguageServices.NativeX.SyntaxTree.NativeXExpression);
                                            int currentIndex5 = currentIndex4;
                                            {
                                                parseSuccess = false;
                                                result5 = ParseEXP9(tokens, ref currentIndex5, ref parseSuccess);
                                            }
                                            if (parseSuccess)
                                            {
                                                currentIndex4 = currentIndex5;
                                                RightOperandMember4 = result5;
                                            }
                                        }
                                        RightOperandMember3 = RightOperandMember4;
                                    }
                                    if (parseSuccess)
                                    {
                                        currentIndexCopy4 = currentIndex4;
                                    }
                                    else
                                    {
                                        goto LABEL_1;
                                    }
                                    currentIndex2 = currentIndexCopy4;
                                LABEL_1: ;
                                }
                                OperatorMember2 = OperatorMember3;
                                RightOperandMember2 = RightOperandMember3;
                            }
                            if (parseSuccess)
                            {
                                currentIndex1 = currentIndex2;
                                Developer.LanguageServices.NativeX.SyntaxTree.NativeXBinaryExpression result2 = CodeNode.Create<Developer.LanguageServices.NativeX.SyntaxTree.NativeXBinaryExpression>();
                                result2.Operator = OperatorMember2;
                                result2.RightOperand = RightOperandMember2;
                                result2.LeftOperand = result1;
                                result2.Start = result1.Start;
                                result2.End = Developer.LanguageProvider.CodeTokenizer.GetEndPosition(tokens, currentIndex1);
                                result1 = result2;
                            }
                            else
                            {
                                break;
                            }
                        }
                        parseSuccess = true;
                    LABEL_0: ;
                    }
                }
                if (parseSuccess)
                {
                    currentToken = currentIndex1;
                    result = result1;
                }
            }
            if (parseSuccess)
            {
                if (result == null) result = CodeNode.Create<Developer.LanguageServices.NativeX.SyntaxTree.NativeXExpression>();
                result.Start = start;
                result.End = Developer.LanguageProvider.CodeTokenizer.GetEndPosition(tokens, currentToken);
            }
            return result;
        }

        public static Developer.LanguageServices.NativeX.SyntaxTree.NativeXExpression ParseEXP9(List<CodeToken> tokens, ref int currentToken, ref bool parseSuccess)
        {
            Developer.LanguageProvider.TextPosition start = Developer.LanguageProvider.CodeTokenizer.GetStartPosition(tokens, currentToken);
            Developer.LanguageServices.NativeX.SyntaxTree.NativeXExpression result = default(Developer.LanguageServices.NativeX.SyntaxTree.NativeXExpression);
            {
                Developer.LanguageServices.NativeX.SyntaxTree.NativeXExpression result1 = default(Developer.LanguageServices.NativeX.SyntaxTree.NativeXExpression);
                int currentIndex1 = currentToken;
                {
                    {
                        int currentIndex2 = currentIndex1;
                        {
                            parseSuccess = false;
                            result1 = ParseEXP8(tokens, ref currentIndex2, ref parseSuccess);
                        }
                        if (parseSuccess)
                        {
                            currentIndex1 = currentIndex2;
                        }
                        else
                        {
                            goto LABEL_0;
                        }
                        while (true)
                        {
                            System.String OperatorMember2 = default(System.String);
                            Developer.LanguageServices.NativeX.SyntaxTree.NativeXExpression RightOperandMember2 = default(Developer.LanguageServices.NativeX.SyntaxTree.NativeXExpression);
                            {
                                System.String OperatorMember3 = default(System.String);
                                Developer.LanguageServices.NativeX.SyntaxTree.NativeXExpression RightOperandMember3 = default(Developer.LanguageServices.NativeX.SyntaxTree.NativeXExpression);
                                {
                                    int currentIndexCopy4 = currentIndex2;
                                    int currentIndex4 = currentIndex2;
                                    {
                                        System.String OperatorMember4 = default(System.String);
                                        {
                                            Developer.LanguageProvider.CodeToken result5 = default(Developer.LanguageProvider.CodeToken);
                                            int currentIndex5 = currentIndex4;
                                            {
                                                result5 = Developer.LanguageProvider.CodeTokenizer.ParseToken(tokens, ref currentIndex5, ref parseSuccess, "^");
                                            }
                                            if (parseSuccess)
                                            {
                                                currentIndex4 = currentIndex5;
                                                OperatorMember4 = result5.Value;
                                            }
                                        }
                                        OperatorMember3 = OperatorMember4;
                                    }
                                    if (parseSuccess)
                                    {
                                        currentIndexCopy4 = currentIndex4;
                                    }
                                    else
                                    {
                                        goto LABEL_1;
                                    }
                                    {
                                        Developer.LanguageServices.NativeX.SyntaxTree.NativeXExpression RightOperandMember4 = default(Developer.LanguageServices.NativeX.SyntaxTree.NativeXExpression);
                                        {
                                            Developer.LanguageServices.NativeX.SyntaxTree.NativeXExpression result5 = default(Developer.LanguageServices.NativeX.SyntaxTree.NativeXExpression);
                                            int currentIndex5 = currentIndex4;
                                            {
                                                parseSuccess = false;
                                                result5 = ParseEXP8(tokens, ref currentIndex5, ref parseSuccess);
                                            }
                                            if (parseSuccess)
                                            {
                                                currentIndex4 = currentIndex5;
                                                RightOperandMember4 = result5;
                                            }
                                        }
                                        RightOperandMember3 = RightOperandMember4;
                                    }
                                    if (parseSuccess)
                                    {
                                        currentIndexCopy4 = currentIndex4;
                                    }
                                    else
                                    {
                                        goto LABEL_1;
                                    }
                                    currentIndex2 = currentIndexCopy4;
                                LABEL_1: ;
                                }
                                OperatorMember2 = OperatorMember3;
                                RightOperandMember2 = RightOperandMember3;
                            }
                            if (parseSuccess)
                            {
                                currentIndex1 = currentIndex2;
                                Developer.LanguageServices.NativeX.SyntaxTree.NativeXBinaryExpression result2 = CodeNode.Create<Developer.LanguageServices.NativeX.SyntaxTree.NativeXBinaryExpression>();
                                result2.Operator = OperatorMember2;
                                result2.RightOperand = RightOperandMember2;
                                result2.LeftOperand = result1;
                                result2.Start = result1.Start;
                                result2.End = Developer.LanguageProvider.CodeTokenizer.GetEndPosition(tokens, currentIndex1);
                                result1 = result2;
                            }
                            else
                            {
                                break;
                            }
                        }
                        parseSuccess = true;
                    LABEL_0: ;
                    }
                }
                if (parseSuccess)
                {
                    currentToken = currentIndex1;
                    result = result1;
                }
            }
            if (parseSuccess)
            {
                if (result == null) result = CodeNode.Create<Developer.LanguageServices.NativeX.SyntaxTree.NativeXExpression>();
                result.Start = start;
                result.End = Developer.LanguageProvider.CodeTokenizer.GetEndPosition(tokens, currentToken);
            }
            return result;
        }

        public static Developer.LanguageServices.NativeX.SyntaxTree.NativeXExpression ParseEXP8(List<CodeToken> tokens, ref int currentToken, ref bool parseSuccess)
        {
            Developer.LanguageProvider.TextPosition start = Developer.LanguageProvider.CodeTokenizer.GetStartPosition(tokens, currentToken);
            Developer.LanguageServices.NativeX.SyntaxTree.NativeXExpression result = default(Developer.LanguageServices.NativeX.SyntaxTree.NativeXExpression);
            {
                Developer.LanguageServices.NativeX.SyntaxTree.NativeXExpression result1 = default(Developer.LanguageServices.NativeX.SyntaxTree.NativeXExpression);
                int currentIndex1 = currentToken;
                {
                    {
                        int currentIndex2 = currentIndex1;
                        {
                            parseSuccess = false;
                            result1 = ParseEXP7(tokens, ref currentIndex2, ref parseSuccess);
                        }
                        if (parseSuccess)
                        {
                            currentIndex1 = currentIndex2;
                        }
                        else
                        {
                            goto LABEL_0;
                        }
                        while (true)
                        {
                            System.String OperatorMember2 = default(System.String);
                            Developer.LanguageServices.NativeX.SyntaxTree.NativeXExpression RightOperandMember2 = default(Developer.LanguageServices.NativeX.SyntaxTree.NativeXExpression);
                            {
                                System.String OperatorMember3 = default(System.String);
                                Developer.LanguageServices.NativeX.SyntaxTree.NativeXExpression RightOperandMember3 = default(Developer.LanguageServices.NativeX.SyntaxTree.NativeXExpression);
                                {
                                    int currentIndexCopy4 = currentIndex2;
                                    int currentIndex4 = currentIndex2;
                                    {
                                        System.String OperatorMember4 = default(System.String);
                                        {
                                            Developer.LanguageProvider.CodeToken result5 = default(Developer.LanguageProvider.CodeToken);
                                            int currentIndex5 = currentIndex4;
                                            {
                                                result5 = Developer.LanguageProvider.CodeTokenizer.ParseToken(tokens, ref currentIndex5, ref parseSuccess, "&");
                                            }
                                            if (parseSuccess)
                                            {
                                                currentIndex4 = currentIndex5;
                                                OperatorMember4 = result5.Value;
                                            }
                                        }
                                        OperatorMember3 = OperatorMember4;
                                    }
                                    if (parseSuccess)
                                    {
                                        currentIndexCopy4 = currentIndex4;
                                    }
                                    else
                                    {
                                        goto LABEL_1;
                                    }
                                    {
                                        Developer.LanguageServices.NativeX.SyntaxTree.NativeXExpression RightOperandMember4 = default(Developer.LanguageServices.NativeX.SyntaxTree.NativeXExpression);
                                        {
                                            Developer.LanguageServices.NativeX.SyntaxTree.NativeXExpression result5 = default(Developer.LanguageServices.NativeX.SyntaxTree.NativeXExpression);
                                            int currentIndex5 = currentIndex4;
                                            {
                                                parseSuccess = false;
                                                result5 = ParseEXP7(tokens, ref currentIndex5, ref parseSuccess);
                                            }
                                            if (parseSuccess)
                                            {
                                                currentIndex4 = currentIndex5;
                                                RightOperandMember4 = result5;
                                            }
                                        }
                                        RightOperandMember3 = RightOperandMember4;
                                    }
                                    if (parseSuccess)
                                    {
                                        currentIndexCopy4 = currentIndex4;
                                    }
                                    else
                                    {
                                        goto LABEL_1;
                                    }
                                    currentIndex2 = currentIndexCopy4;
                                LABEL_1: ;
                                }
                                OperatorMember2 = OperatorMember3;
                                RightOperandMember2 = RightOperandMember3;
                            }
                            if (parseSuccess)
                            {
                                currentIndex1 = currentIndex2;
                                Developer.LanguageServices.NativeX.SyntaxTree.NativeXBinaryExpression result2 = CodeNode.Create<Developer.LanguageServices.NativeX.SyntaxTree.NativeXBinaryExpression>();
                                result2.Operator = OperatorMember2;
                                result2.RightOperand = RightOperandMember2;
                                result2.LeftOperand = result1;
                                result2.Start = result1.Start;
                                result2.End = Developer.LanguageProvider.CodeTokenizer.GetEndPosition(tokens, currentIndex1);
                                result1 = result2;
                            }
                            else
                            {
                                break;
                            }
                        }
                        parseSuccess = true;
                    LABEL_0: ;
                    }
                }
                if (parseSuccess)
                {
                    currentToken = currentIndex1;
                    result = result1;
                }
            }
            if (parseSuccess)
            {
                if (result == null) result = CodeNode.Create<Developer.LanguageServices.NativeX.SyntaxTree.NativeXExpression>();
                result.Start = start;
                result.End = Developer.LanguageProvider.CodeTokenizer.GetEndPosition(tokens, currentToken);
            }
            return result;
        }

        public static Developer.LanguageServices.NativeX.SyntaxTree.NativeXExpression ParseEXP7(List<CodeToken> tokens, ref int currentToken, ref bool parseSuccess)
        {
            Developer.LanguageProvider.TextPosition start = Developer.LanguageProvider.CodeTokenizer.GetStartPosition(tokens, currentToken);
            Developer.LanguageServices.NativeX.SyntaxTree.NativeXExpression result = default(Developer.LanguageServices.NativeX.SyntaxTree.NativeXExpression);
            {
                Developer.LanguageServices.NativeX.SyntaxTree.NativeXExpression result1 = default(Developer.LanguageServices.NativeX.SyntaxTree.NativeXExpression);
                int currentIndex1 = currentToken;
                {
                    {
                        int currentIndex2 = currentIndex1;
                        {
                            parseSuccess = false;
                            result1 = ParseEXP6(tokens, ref currentIndex2, ref parseSuccess);
                        }
                        if (parseSuccess)
                        {
                            currentIndex1 = currentIndex2;
                        }
                        else
                        {
                            goto LABEL_0;
                        }
                        while (true)
                        {
                            System.String OperatorMember2 = default(System.String);
                            Developer.LanguageServices.NativeX.SyntaxTree.NativeXExpression RightOperandMember2 = default(Developer.LanguageServices.NativeX.SyntaxTree.NativeXExpression);
                            {
                                System.String OperatorMember3 = default(System.String);
                                Developer.LanguageServices.NativeX.SyntaxTree.NativeXExpression RightOperandMember3 = default(Developer.LanguageServices.NativeX.SyntaxTree.NativeXExpression);
                                {
                                    int currentIndexCopy4 = currentIndex2;
                                    int currentIndex4 = currentIndex2;
                                    {
                                        System.String OperatorMember4 = default(System.String);
                                        {
                                            Developer.LanguageProvider.CodeToken result5 = default(Developer.LanguageProvider.CodeToken);
                                            int currentIndex5 = currentIndex4;
                                            {
                                                result5 = Developer.LanguageProvider.CodeTokenizer.ParseToken(tokens, ref currentIndex5, ref parseSuccess, "==", "!=");
                                            }
                                            if (parseSuccess)
                                            {
                                                currentIndex4 = currentIndex5;
                                                OperatorMember4 = result5.Value;
                                            }
                                        }
                                        OperatorMember3 = OperatorMember4;
                                    }
                                    if (parseSuccess)
                                    {
                                        currentIndexCopy4 = currentIndex4;
                                    }
                                    else
                                    {
                                        goto LABEL_1;
                                    }
                                    {
                                        Developer.LanguageServices.NativeX.SyntaxTree.NativeXExpression RightOperandMember4 = default(Developer.LanguageServices.NativeX.SyntaxTree.NativeXExpression);
                                        {
                                            Developer.LanguageServices.NativeX.SyntaxTree.NativeXExpression result5 = default(Developer.LanguageServices.NativeX.SyntaxTree.NativeXExpression);
                                            int currentIndex5 = currentIndex4;
                                            {
                                                parseSuccess = false;
                                                result5 = ParseEXP6(tokens, ref currentIndex5, ref parseSuccess);
                                            }
                                            if (parseSuccess)
                                            {
                                                currentIndex4 = currentIndex5;
                                                RightOperandMember4 = result5;
                                            }
                                        }
                                        RightOperandMember3 = RightOperandMember4;
                                    }
                                    if (parseSuccess)
                                    {
                                        currentIndexCopy4 = currentIndex4;
                                    }
                                    else
                                    {
                                        goto LABEL_1;
                                    }
                                    currentIndex2 = currentIndexCopy4;
                                LABEL_1: ;
                                }
                                OperatorMember2 = OperatorMember3;
                                RightOperandMember2 = RightOperandMember3;
                            }
                            if (parseSuccess)
                            {
                                currentIndex1 = currentIndex2;
                                Developer.LanguageServices.NativeX.SyntaxTree.NativeXBinaryExpression result2 = CodeNode.Create<Developer.LanguageServices.NativeX.SyntaxTree.NativeXBinaryExpression>();
                                result2.Operator = OperatorMember2;
                                result2.RightOperand = RightOperandMember2;
                                result2.LeftOperand = result1;
                                result2.Start = result1.Start;
                                result2.End = Developer.LanguageProvider.CodeTokenizer.GetEndPosition(tokens, currentIndex1);
                                result1 = result2;
                            }
                            else
                            {
                                break;
                            }
                        }
                        parseSuccess = true;
                    LABEL_0: ;
                    }
                }
                if (parseSuccess)
                {
                    currentToken = currentIndex1;
                    result = result1;
                }
            }
            if (parseSuccess)
            {
                if (result == null) result = CodeNode.Create<Developer.LanguageServices.NativeX.SyntaxTree.NativeXExpression>();
                result.Start = start;
                result.End = Developer.LanguageProvider.CodeTokenizer.GetEndPosition(tokens, currentToken);
            }
            return result;
        }

        public static Developer.LanguageServices.NativeX.SyntaxTree.NativeXExpression ParseEXP6(List<CodeToken> tokens, ref int currentToken, ref bool parseSuccess)
        {
            Developer.LanguageProvider.TextPosition start = Developer.LanguageProvider.CodeTokenizer.GetStartPosition(tokens, currentToken);
            Developer.LanguageServices.NativeX.SyntaxTree.NativeXExpression result = default(Developer.LanguageServices.NativeX.SyntaxTree.NativeXExpression);
            {
                Developer.LanguageServices.NativeX.SyntaxTree.NativeXExpression result1 = default(Developer.LanguageServices.NativeX.SyntaxTree.NativeXExpression);
                int currentIndex1 = currentToken;
                {
                    {
                        int currentIndex2 = currentIndex1;
                        {
                            parseSuccess = false;
                            result1 = ParseEXP5(tokens, ref currentIndex2, ref parseSuccess);
                        }
                        if (parseSuccess)
                        {
                            currentIndex1 = currentIndex2;
                        }
                        else
                        {
                            goto LABEL_0;
                        }
                        while (true)
                        {
                            System.String OperatorMember2 = default(System.String);
                            Developer.LanguageServices.NativeX.SyntaxTree.NativeXExpression RightOperandMember2 = default(Developer.LanguageServices.NativeX.SyntaxTree.NativeXExpression);
                            {
                                System.String OperatorMember3 = default(System.String);
                                Developer.LanguageServices.NativeX.SyntaxTree.NativeXExpression RightOperandMember3 = default(Developer.LanguageServices.NativeX.SyntaxTree.NativeXExpression);
                                {
                                    int currentIndexCopy4 = currentIndex2;
                                    int currentIndex4 = currentIndex2;
                                    {
                                        System.String OperatorMember4 = default(System.String);
                                        {
                                            Developer.LanguageProvider.CodeToken result5 = default(Developer.LanguageProvider.CodeToken);
                                            int currentIndex5 = currentIndex4;
                                            {
                                                result5 = Developer.LanguageProvider.CodeTokenizer.ParseToken(tokens, ref currentIndex5, ref parseSuccess, "<", "<=", ">", ">=");
                                            }
                                            if (parseSuccess)
                                            {
                                                currentIndex4 = currentIndex5;
                                                OperatorMember4 = result5.Value;
                                            }
                                        }
                                        OperatorMember3 = OperatorMember4;
                                    }
                                    if (parseSuccess)
                                    {
                                        currentIndexCopy4 = currentIndex4;
                                    }
                                    else
                                    {
                                        goto LABEL_1;
                                    }
                                    {
                                        Developer.LanguageServices.NativeX.SyntaxTree.NativeXExpression RightOperandMember4 = default(Developer.LanguageServices.NativeX.SyntaxTree.NativeXExpression);
                                        {
                                            Developer.LanguageServices.NativeX.SyntaxTree.NativeXExpression result5 = default(Developer.LanguageServices.NativeX.SyntaxTree.NativeXExpression);
                                            int currentIndex5 = currentIndex4;
                                            {
                                                parseSuccess = false;
                                                result5 = ParseEXP5(tokens, ref currentIndex5, ref parseSuccess);
                                            }
                                            if (parseSuccess)
                                            {
                                                currentIndex4 = currentIndex5;
                                                RightOperandMember4 = result5;
                                            }
                                        }
                                        RightOperandMember3 = RightOperandMember4;
                                    }
                                    if (parseSuccess)
                                    {
                                        currentIndexCopy4 = currentIndex4;
                                    }
                                    else
                                    {
                                        goto LABEL_1;
                                    }
                                    currentIndex2 = currentIndexCopy4;
                                LABEL_1: ;
                                }
                                OperatorMember2 = OperatorMember3;
                                RightOperandMember2 = RightOperandMember3;
                            }
                            if (parseSuccess)
                            {
                                currentIndex1 = currentIndex2;
                                Developer.LanguageServices.NativeX.SyntaxTree.NativeXBinaryExpression result2 = CodeNode.Create<Developer.LanguageServices.NativeX.SyntaxTree.NativeXBinaryExpression>();
                                result2.Operator = OperatorMember2;
                                result2.RightOperand = RightOperandMember2;
                                result2.LeftOperand = result1;
                                result2.Start = result1.Start;
                                result2.End = Developer.LanguageProvider.CodeTokenizer.GetEndPosition(tokens, currentIndex1);
                                result1 = result2;
                            }
                            else
                            {
                                break;
                            }
                        }
                        parseSuccess = true;
                    LABEL_0: ;
                    }
                }
                if (parseSuccess)
                {
                    currentToken = currentIndex1;
                    result = result1;
                }
            }
            if (parseSuccess)
            {
                if (result == null) result = CodeNode.Create<Developer.LanguageServices.NativeX.SyntaxTree.NativeXExpression>();
                result.Start = start;
                result.End = Developer.LanguageProvider.CodeTokenizer.GetEndPosition(tokens, currentToken);
            }
            return result;
        }

        public static Developer.LanguageServices.NativeX.SyntaxTree.NativeXExpression ParseEXP5(List<CodeToken> tokens, ref int currentToken, ref bool parseSuccess)
        {
            Developer.LanguageProvider.TextPosition start = Developer.LanguageProvider.CodeTokenizer.GetStartPosition(tokens, currentToken);
            Developer.LanguageServices.NativeX.SyntaxTree.NativeXExpression result = default(Developer.LanguageServices.NativeX.SyntaxTree.NativeXExpression);
            {
                Developer.LanguageServices.NativeX.SyntaxTree.NativeXExpression result1 = default(Developer.LanguageServices.NativeX.SyntaxTree.NativeXExpression);
                int currentIndex1 = currentToken;
                {
                    {
                        int currentIndex2 = currentIndex1;
                        {
                            parseSuccess = false;
                            result1 = ParseEXP4(tokens, ref currentIndex2, ref parseSuccess);
                        }
                        if (parseSuccess)
                        {
                            currentIndex1 = currentIndex2;
                        }
                        else
                        {
                            goto LABEL_0;
                        }
                        while (true)
                        {
                            System.String OperatorMember2 = default(System.String);
                            Developer.LanguageServices.NativeX.SyntaxTree.NativeXExpression RightOperandMember2 = default(Developer.LanguageServices.NativeX.SyntaxTree.NativeXExpression);
                            {
                                System.String OperatorMember3 = default(System.String);
                                Developer.LanguageServices.NativeX.SyntaxTree.NativeXExpression RightOperandMember3 = default(Developer.LanguageServices.NativeX.SyntaxTree.NativeXExpression);
                                {
                                    int currentIndexCopy4 = currentIndex2;
                                    int currentIndex4 = currentIndex2;
                                    {
                                        System.String OperatorMember4 = default(System.String);
                                        {
                                            int currentIndex5 = -1;
                                            currentIndex5 = currentIndex4;
                                            {
                                                System.String OperatorMember5 = default(System.String);
                                                {
                                                    int currentIndex6 = currentIndex5;
                                                    {
                                                        {
                                                            int currentIndexCopy7 = currentIndex6;
                                                            int currentIndex7 = currentIndex6;
                                                            {
                                                                Developer.LanguageProvider.CodeTokenizer.ParseToken(tokens, ref currentIndex7, ref parseSuccess, "<");
                                                            }
                                                            if (parseSuccess)
                                                            {
                                                                currentIndexCopy7 = currentIndex7;
                                                            }
                                                            else
                                                            {
                                                                goto LABEL_4;
                                                            }
                                                            {
                                                                Developer.LanguageProvider.CodeTokenizer.ParseToken(tokens, ref currentIndex7, ref parseSuccess, "<");
                                                            }
                                                            if (parseSuccess)
                                                            {
                                                                currentIndexCopy7 = currentIndex7;
                                                            }
                                                            else
                                                            {
                                                                goto LABEL_4;
                                                            }
                                                            currentIndex6 = currentIndexCopy7;
                                                        LABEL_4: ;
                                                        }
                                                    }
                                                    if (parseSuccess)
                                                    {
                                                        currentIndex5 = currentIndex6;
                                                        OperatorMember5 = "<<";
                                                    }
                                                }
                                                OperatorMember4 = OperatorMember5;
                                            }
                                            if (parseSuccess)
                                            {
                                                currentIndex4 = currentIndex5;
                                                goto LABEL_SUCCESS_2;
                                            }
                                            currentIndex5 = currentIndex4;
                                            {
                                                System.String OperatorMember5 = default(System.String);
                                                {
                                                    int currentIndex6 = currentIndex5;
                                                    {
                                                        {
                                                            int currentIndexCopy7 = currentIndex6;
                                                            int currentIndex7 = currentIndex6;
                                                            {
                                                                Developer.LanguageProvider.CodeTokenizer.ParseToken(tokens, ref currentIndex7, ref parseSuccess, ">");
                                                            }
                                                            if (parseSuccess)
                                                            {
                                                                currentIndexCopy7 = currentIndex7;
                                                            }
                                                            else
                                                            {
                                                                goto LABEL_5;
                                                            }
                                                            {
                                                                Developer.LanguageProvider.CodeTokenizer.ParseToken(tokens, ref currentIndex7, ref parseSuccess, ">");
                                                            }
                                                            if (parseSuccess)
                                                            {
                                                                currentIndexCopy7 = currentIndex7;
                                                            }
                                                            else
                                                            {
                                                                goto LABEL_5;
                                                            }
                                                            currentIndex6 = currentIndexCopy7;
                                                        LABEL_5: ;
                                                        }
                                                    }
                                                    if (parseSuccess)
                                                    {
                                                        currentIndex5 = currentIndex6;
                                                        OperatorMember5 = ">>";
                                                    }
                                                }
                                                OperatorMember4 = OperatorMember5;
                                            }
                                            if (parseSuccess)
                                            {
                                                currentIndex4 = currentIndex5;
                                                goto LABEL_SUCCESS_2;
                                            }
                                            goto LABEL_FAIL_3;
                                        LABEL_SUCCESS_2: ;
                                        LABEL_FAIL_3: ;
                                        }
                                        OperatorMember3 = OperatorMember4;
                                    }
                                    if (parseSuccess)
                                    {
                                        currentIndexCopy4 = currentIndex4;
                                    }
                                    else
                                    {
                                        goto LABEL_1;
                                    }
                                    {
                                        Developer.LanguageServices.NativeX.SyntaxTree.NativeXExpression RightOperandMember4 = default(Developer.LanguageServices.NativeX.SyntaxTree.NativeXExpression);
                                        {
                                            Developer.LanguageServices.NativeX.SyntaxTree.NativeXExpression result5 = default(Developer.LanguageServices.NativeX.SyntaxTree.NativeXExpression);
                                            int currentIndex5 = currentIndex4;
                                            {
                                                parseSuccess = false;
                                                result5 = ParseEXP4(tokens, ref currentIndex5, ref parseSuccess);
                                            }
                                            if (parseSuccess)
                                            {
                                                currentIndex4 = currentIndex5;
                                                RightOperandMember4 = result5;
                                            }
                                        }
                                        RightOperandMember3 = RightOperandMember4;
                                    }
                                    if (parseSuccess)
                                    {
                                        currentIndexCopy4 = currentIndex4;
                                    }
                                    else
                                    {
                                        goto LABEL_1;
                                    }
                                    currentIndex2 = currentIndexCopy4;
                                LABEL_1: ;
                                }
                                OperatorMember2 = OperatorMember3;
                                RightOperandMember2 = RightOperandMember3;
                            }
                            if (parseSuccess)
                            {
                                currentIndex1 = currentIndex2;
                                Developer.LanguageServices.NativeX.SyntaxTree.NativeXBinaryExpression result2 = CodeNode.Create<Developer.LanguageServices.NativeX.SyntaxTree.NativeXBinaryExpression>();
                                result2.Operator = OperatorMember2;
                                result2.RightOperand = RightOperandMember2;
                                result2.LeftOperand = result1;
                                result2.Start = result1.Start;
                                result2.End = Developer.LanguageProvider.CodeTokenizer.GetEndPosition(tokens, currentIndex1);
                                result1 = result2;
                            }
                            else
                            {
                                break;
                            }
                        }
                        parseSuccess = true;
                    LABEL_0: ;
                    }
                }
                if (parseSuccess)
                {
                    currentToken = currentIndex1;
                    result = result1;
                }
            }
            if (parseSuccess)
            {
                if (result == null) result = CodeNode.Create<Developer.LanguageServices.NativeX.SyntaxTree.NativeXExpression>();
                result.Start = start;
                result.End = Developer.LanguageProvider.CodeTokenizer.GetEndPosition(tokens, currentToken);
            }
            return result;
        }

        public static Developer.LanguageServices.NativeX.SyntaxTree.NativeXExpression ParseEXP4(List<CodeToken> tokens, ref int currentToken, ref bool parseSuccess)
        {
            Developer.LanguageProvider.TextPosition start = Developer.LanguageProvider.CodeTokenizer.GetStartPosition(tokens, currentToken);
            Developer.LanguageServices.NativeX.SyntaxTree.NativeXExpression result = default(Developer.LanguageServices.NativeX.SyntaxTree.NativeXExpression);
            {
                Developer.LanguageServices.NativeX.SyntaxTree.NativeXExpression result1 = default(Developer.LanguageServices.NativeX.SyntaxTree.NativeXExpression);
                int currentIndex1 = currentToken;
                {
                    {
                        int currentIndex2 = currentIndex1;
                        {
                            parseSuccess = false;
                            result1 = ParseEXP3(tokens, ref currentIndex2, ref parseSuccess);
                        }
                        if (parseSuccess)
                        {
                            currentIndex1 = currentIndex2;
                        }
                        else
                        {
                            goto LABEL_0;
                        }
                        while (true)
                        {
                            System.String OperatorMember2 = default(System.String);
                            Developer.LanguageServices.NativeX.SyntaxTree.NativeXExpression RightOperandMember2 = default(Developer.LanguageServices.NativeX.SyntaxTree.NativeXExpression);
                            {
                                System.String OperatorMember3 = default(System.String);
                                Developer.LanguageServices.NativeX.SyntaxTree.NativeXExpression RightOperandMember3 = default(Developer.LanguageServices.NativeX.SyntaxTree.NativeXExpression);
                                {
                                    int currentIndexCopy4 = currentIndex2;
                                    int currentIndex4 = currentIndex2;
                                    {
                                        System.String OperatorMember4 = default(System.String);
                                        {
                                            Developer.LanguageProvider.CodeToken result5 = default(Developer.LanguageProvider.CodeToken);
                                            int currentIndex5 = currentIndex4;
                                            {
                                                result5 = Developer.LanguageProvider.CodeTokenizer.ParseToken(tokens, ref currentIndex5, ref parseSuccess, "+", "-");
                                            }
                                            if (parseSuccess)
                                            {
                                                currentIndex4 = currentIndex5;
                                                OperatorMember4 = result5.Value;
                                            }
                                        }
                                        OperatorMember3 = OperatorMember4;
                                    }
                                    if (parseSuccess)
                                    {
                                        currentIndexCopy4 = currentIndex4;
                                    }
                                    else
                                    {
                                        goto LABEL_1;
                                    }
                                    {
                                        Developer.LanguageServices.NativeX.SyntaxTree.NativeXExpression RightOperandMember4 = default(Developer.LanguageServices.NativeX.SyntaxTree.NativeXExpression);
                                        {
                                            Developer.LanguageServices.NativeX.SyntaxTree.NativeXExpression result5 = default(Developer.LanguageServices.NativeX.SyntaxTree.NativeXExpression);
                                            int currentIndex5 = currentIndex4;
                                            {
                                                parseSuccess = false;
                                                result5 = ParseEXP3(tokens, ref currentIndex5, ref parseSuccess);
                                            }
                                            if (parseSuccess)
                                            {
                                                currentIndex4 = currentIndex5;
                                                RightOperandMember4 = result5;
                                            }
                                        }
                                        RightOperandMember3 = RightOperandMember4;
                                    }
                                    if (parseSuccess)
                                    {
                                        currentIndexCopy4 = currentIndex4;
                                    }
                                    else
                                    {
                                        goto LABEL_1;
                                    }
                                    currentIndex2 = currentIndexCopy4;
                                LABEL_1: ;
                                }
                                OperatorMember2 = OperatorMember3;
                                RightOperandMember2 = RightOperandMember3;
                            }
                            if (parseSuccess)
                            {
                                currentIndex1 = currentIndex2;
                                Developer.LanguageServices.NativeX.SyntaxTree.NativeXBinaryExpression result2 = CodeNode.Create<Developer.LanguageServices.NativeX.SyntaxTree.NativeXBinaryExpression>();
                                result2.Operator = OperatorMember2;
                                result2.RightOperand = RightOperandMember2;
                                result2.LeftOperand = result1;
                                result2.Start = result1.Start;
                                result2.End = Developer.LanguageProvider.CodeTokenizer.GetEndPosition(tokens, currentIndex1);
                                result1 = result2;
                            }
                            else
                            {
                                break;
                            }
                        }
                        parseSuccess = true;
                    LABEL_0: ;
                    }
                }
                if (parseSuccess)
                {
                    currentToken = currentIndex1;
                    result = result1;
                }
            }
            if (parseSuccess)
            {
                if (result == null) result = CodeNode.Create<Developer.LanguageServices.NativeX.SyntaxTree.NativeXExpression>();
                result.Start = start;
                result.End = Developer.LanguageProvider.CodeTokenizer.GetEndPosition(tokens, currentToken);
            }
            return result;
        }

        public static Developer.LanguageServices.NativeX.SyntaxTree.NativeXExpression ParseEXP3(List<CodeToken> tokens, ref int currentToken, ref bool parseSuccess)
        {
            Developer.LanguageProvider.TextPosition start = Developer.LanguageProvider.CodeTokenizer.GetStartPosition(tokens, currentToken);
            Developer.LanguageServices.NativeX.SyntaxTree.NativeXExpression result = default(Developer.LanguageServices.NativeX.SyntaxTree.NativeXExpression);
            {
                Developer.LanguageServices.NativeX.SyntaxTree.NativeXExpression result1 = default(Developer.LanguageServices.NativeX.SyntaxTree.NativeXExpression);
                int currentIndex1 = currentToken;
                {
                    {
                        int currentIndex2 = currentIndex1;
                        {
                            parseSuccess = false;
                            result1 = ParseEXP2(tokens, ref currentIndex2, ref parseSuccess);
                        }
                        if (parseSuccess)
                        {
                            currentIndex1 = currentIndex2;
                        }
                        else
                        {
                            goto LABEL_0;
                        }
                        while (true)
                        {
                            System.String OperatorMember2 = default(System.String);
                            Developer.LanguageServices.NativeX.SyntaxTree.NativeXExpression RightOperandMember2 = default(Developer.LanguageServices.NativeX.SyntaxTree.NativeXExpression);
                            {
                                System.String OperatorMember3 = default(System.String);
                                Developer.LanguageServices.NativeX.SyntaxTree.NativeXExpression RightOperandMember3 = default(Developer.LanguageServices.NativeX.SyntaxTree.NativeXExpression);
                                {
                                    int currentIndexCopy4 = currentIndex2;
                                    int currentIndex4 = currentIndex2;
                                    {
                                        System.String OperatorMember4 = default(System.String);
                                        {
                                            Developer.LanguageProvider.CodeToken result5 = default(Developer.LanguageProvider.CodeToken);
                                            int currentIndex5 = currentIndex4;
                                            {
                                                result5 = Developer.LanguageProvider.CodeTokenizer.ParseToken(tokens, ref currentIndex5, ref parseSuccess, "*", "/", "%");
                                            }
                                            if (parseSuccess)
                                            {
                                                currentIndex4 = currentIndex5;
                                                OperatorMember4 = result5.Value;
                                            }
                                        }
                                        OperatorMember3 = OperatorMember4;
                                    }
                                    if (parseSuccess)
                                    {
                                        currentIndexCopy4 = currentIndex4;
                                    }
                                    else
                                    {
                                        goto LABEL_1;
                                    }
                                    {
                                        Developer.LanguageServices.NativeX.SyntaxTree.NativeXExpression RightOperandMember4 = default(Developer.LanguageServices.NativeX.SyntaxTree.NativeXExpression);
                                        {
                                            Developer.LanguageServices.NativeX.SyntaxTree.NativeXExpression result5 = default(Developer.LanguageServices.NativeX.SyntaxTree.NativeXExpression);
                                            int currentIndex5 = currentIndex4;
                                            {
                                                parseSuccess = false;
                                                result5 = ParseEXP2(tokens, ref currentIndex5, ref parseSuccess);
                                            }
                                            if (parseSuccess)
                                            {
                                                currentIndex4 = currentIndex5;
                                                RightOperandMember4 = result5;
                                            }
                                        }
                                        RightOperandMember3 = RightOperandMember4;
                                    }
                                    if (parseSuccess)
                                    {
                                        currentIndexCopy4 = currentIndex4;
                                    }
                                    else
                                    {
                                        goto LABEL_1;
                                    }
                                    currentIndex2 = currentIndexCopy4;
                                LABEL_1: ;
                                }
                                OperatorMember2 = OperatorMember3;
                                RightOperandMember2 = RightOperandMember3;
                            }
                            if (parseSuccess)
                            {
                                currentIndex1 = currentIndex2;
                                Developer.LanguageServices.NativeX.SyntaxTree.NativeXBinaryExpression result2 = CodeNode.Create<Developer.LanguageServices.NativeX.SyntaxTree.NativeXBinaryExpression>();
                                result2.Operator = OperatorMember2;
                                result2.RightOperand = RightOperandMember2;
                                result2.LeftOperand = result1;
                                result2.Start = result1.Start;
                                result2.End = Developer.LanguageProvider.CodeTokenizer.GetEndPosition(tokens, currentIndex1);
                                result1 = result2;
                            }
                            else
                            {
                                break;
                            }
                        }
                        parseSuccess = true;
                    LABEL_0: ;
                    }
                }
                if (parseSuccess)
                {
                    currentToken = currentIndex1;
                    result = result1;
                }
            }
            if (parseSuccess)
            {
                if (result == null) result = CodeNode.Create<Developer.LanguageServices.NativeX.SyntaxTree.NativeXExpression>();
                result.Start = start;
                result.End = Developer.LanguageProvider.CodeTokenizer.GetEndPosition(tokens, currentToken);
            }
            return result;
        }

        public static Developer.LanguageServices.NativeX.SyntaxTree.NativeXExpression ParseEXP2(List<CodeToken> tokens, ref int currentToken, ref bool parseSuccess)
        {
            Developer.LanguageProvider.TextPosition start = Developer.LanguageProvider.CodeTokenizer.GetStartPosition(tokens, currentToken);
            Developer.LanguageServices.NativeX.SyntaxTree.NativeXExpression result = default(Developer.LanguageServices.NativeX.SyntaxTree.NativeXExpression);
            {
                Developer.LanguageServices.NativeX.SyntaxTree.NativeXExpression result1 = default(Developer.LanguageServices.NativeX.SyntaxTree.NativeXExpression);
                int currentIndex1 = -1;
                currentIndex1 = currentToken;
                {
                    {
                        Developer.LanguageServices.NativeX.SyntaxTree.NativeXExpression result2 = default(Developer.LanguageServices.NativeX.SyntaxTree.NativeXExpression);
                        int currentIndex2 = currentIndex1;
                        {
                            parseSuccess = false;
                            result2 = ParseEXP1(tokens, ref currentIndex2, ref parseSuccess);
                        }
                        if (parseSuccess)
                        {
                            currentIndex1 = currentIndex2;
                            result1 = result2;
                        }
                    }
                }
                if (parseSuccess)
                {
                    currentToken = currentIndex1;
                    goto LABEL_SUCCESS_0;
                }
                currentIndex1 = currentToken;
                {
                    {
                        Developer.LanguageServices.NativeX.SyntaxTree.NativeXUnaryExpression result2 = default(Developer.LanguageServices.NativeX.SyntaxTree.NativeXUnaryExpression);
                        int currentIndex2 = currentIndex1;
                        {
                            parseSuccess = false;
                            result2 = ParseUnary(tokens, ref currentIndex2, ref parseSuccess);
                        }
                        if (parseSuccess)
                        {
                            currentIndex1 = currentIndex2;
                            result1 = result2;
                        }
                    }
                }
                if (parseSuccess)
                {
                    currentToken = currentIndex1;
                    goto LABEL_SUCCESS_0;
                }
                goto LABEL_FAIL_1;
            LABEL_SUCCESS_0: ;
                result = result1;
            LABEL_FAIL_1: ;
            }
            if (parseSuccess)
            {
                if (result == null) result = CodeNode.Create<Developer.LanguageServices.NativeX.SyntaxTree.NativeXExpression>();
                result.Start = start;
                result.End = Developer.LanguageProvider.CodeTokenizer.GetEndPosition(tokens, currentToken);
            }
            return result;
        }

        public static Developer.LanguageServices.NativeX.SyntaxTree.NativeXExpression ParseEXP1(List<CodeToken> tokens, ref int currentToken, ref bool parseSuccess)
        {
            Developer.LanguageProvider.TextPosition start = Developer.LanguageProvider.CodeTokenizer.GetStartPosition(tokens, currentToken);
            Developer.LanguageServices.NativeX.SyntaxTree.NativeXExpression result = default(Developer.LanguageServices.NativeX.SyntaxTree.NativeXExpression);
            {
                Developer.LanguageServices.NativeX.SyntaxTree.NativeXExpression result1 = default(Developer.LanguageServices.NativeX.SyntaxTree.NativeXExpression);
                int currentIndex1 = currentToken;
                {
                    {
                        int currentIndex2 = currentIndex1;
                        {
                            parseSuccess = false;
                            result1 = ParseEXP0(tokens, ref currentIndex2, ref parseSuccess);
                        }
                        if (parseSuccess)
                        {
                            currentIndex1 = currentIndex2;
                        }
                        else
                        {
                            goto LABEL_0;
                        }
                        while (true)
                        {
                            {
                                Developer.LanguageServices.NativeX.SyntaxTree.NativeXExpression SubscribeMember2 = default(Developer.LanguageServices.NativeX.SyntaxTree.NativeXExpression);
                                {
                                    Developer.LanguageServices.NativeX.SyntaxTree.NativeXExpression SubscribeMember3 = default(Developer.LanguageServices.NativeX.SyntaxTree.NativeXExpression);
                                    {
                                        int currentIndexCopy4 = currentIndex2;
                                        int currentIndex4 = currentIndex2;
                                        {
                                            Developer.LanguageProvider.CodeTokenizer.ParseToken(tokens, ref currentIndex4, ref parseSuccess, "[");
                                        }
                                        if (parseSuccess)
                                        {
                                            currentIndexCopy4 = currentIndex4;
                                        }
                                        else
                                        {
                                            goto LABEL_1;
                                        }
                                        {
                                            Developer.LanguageServices.NativeX.SyntaxTree.NativeXExpression SubscribeMember4 = default(Developer.LanguageServices.NativeX.SyntaxTree.NativeXExpression);
                                            {
                                                Developer.LanguageServices.NativeX.SyntaxTree.NativeXExpression result5 = default(Developer.LanguageServices.NativeX.SyntaxTree.NativeXExpression);
                                                int currentIndex5 = currentIndex4;
                                                {
                                                    parseSuccess = false;
                                                    result5 = ParseExpression(tokens, ref currentIndex5, ref parseSuccess);
                                                }
                                                if (parseSuccess)
                                                {
                                                    currentIndex4 = currentIndex5;
                                                    SubscribeMember4 = result5;
                                                }
                                            }
                                            SubscribeMember3 = SubscribeMember4;
                                        }
                                        if (parseSuccess)
                                        {
                                            currentIndexCopy4 = currentIndex4;
                                        }
                                        else
                                        {
                                            goto LABEL_1;
                                        }
                                        {
                                            Developer.LanguageProvider.CodeTokenizer.ParseToken(tokens, ref currentIndex4, ref parseSuccess, "]");
                                        }
                                        if (parseSuccess)
                                        {
                                            currentIndexCopy4 = currentIndex4;
                                        }
                                        else
                                        {
                                            goto LABEL_1;
                                        }
                                        currentIndex2 = currentIndexCopy4;
                                    LABEL_1: ;
                                    }
                                    SubscribeMember2 = SubscribeMember3;
                                }
                                if (parseSuccess)
                                {
                                    currentIndex1 = currentIndex2;
                                    Developer.LanguageServices.NativeX.SyntaxTree.NativeXSubscribeExpression result2 = CodeNode.Create<Developer.LanguageServices.NativeX.SyntaxTree.NativeXSubscribeExpression>();
                                    result2.Subscribe = SubscribeMember2;
                                    result2.Operand = result1;
                                    result2.Start = result1.Start;
                                    result2.End = Developer.LanguageProvider.CodeTokenizer.GetEndPosition(tokens, currentIndex1);
                                    result1 = result2;
                                    continue;
                                }
                            }
                            {
                                Developer.LanguageProvider.CodeNodeList<Developer.LanguageServices.NativeX.SyntaxTree.NativeXExpression> ArgumentsMember2 = default(Developer.LanguageProvider.CodeNodeList<Developer.LanguageServices.NativeX.SyntaxTree.NativeXExpression>);
                                {
                                    Developer.LanguageProvider.CodeNodeList<Developer.LanguageServices.NativeX.SyntaxTree.NativeXExpression> ArgumentsMember3 = default(Developer.LanguageProvider.CodeNodeList<Developer.LanguageServices.NativeX.SyntaxTree.NativeXExpression>);
                                    {
                                        int currentIndexCopy4 = currentIndex2;
                                        int currentIndex4 = currentIndex2;
                                        {
                                            Developer.LanguageProvider.CodeTokenizer.ParseToken(tokens, ref currentIndex4, ref parseSuccess, "(");
                                        }
                                        if (parseSuccess)
                                        {
                                            currentIndexCopy4 = currentIndex4;
                                        }
                                        else
                                        {
                                            goto LABEL_2;
                                        }
                                        {
                                            Developer.LanguageProvider.CodeNodeList<Developer.LanguageServices.NativeX.SyntaxTree.NativeXExpression> ArgumentsMember4 = default(Developer.LanguageProvider.CodeNodeList<Developer.LanguageServices.NativeX.SyntaxTree.NativeXExpression>);
                                            {
                                                Developer.LanguageProvider.CodeNodeList<Developer.LanguageServices.NativeX.SyntaxTree.NativeXExpression> result5 = default(Developer.LanguageProvider.CodeNodeList<Developer.LanguageServices.NativeX.SyntaxTree.NativeXExpression>);
                                                int currentIndex5 = currentIndex4;
                                                {
                                                    result5 = new Developer.LanguageProvider.CodeNodeList<Developer.LanguageServices.NativeX.SyntaxTree.NativeXExpression>();
                                                    {
                                                        Developer.LanguageServices.NativeX.SyntaxTree.NativeXExpression result6 = default(Developer.LanguageServices.NativeX.SyntaxTree.NativeXExpression);
                                                        int currentIndex6 = currentIndex5;
                                                        {
                                                            parseSuccess = false;
                                                            result6 = ParseExpression(tokens, ref currentIndex6, ref parseSuccess);
                                                        }
                                                        if (parseSuccess)
                                                        {
                                                            currentIndex5 = currentIndex6;
                                                            result5.Add(result6);
                                                            result5.Start = result6.Start;
                                                            result5.End = result6.End;
                                                        }
                                                        while (true)
                                                        {
                                                            int currentIndexCopy6 = currentIndex5;
                                                            {
                                                                Developer.LanguageProvider.CodeTokenizer.ParseToken(tokens, ref currentIndex6, ref parseSuccess, ",");
                                                            }
                                                            if (parseSuccess)
                                                            {
                                                                currentIndexCopy6 = currentIndex6;
                                                            }
                                                            else
                                                            {
                                                                goto LABEL_3;
                                                            }
                                                            {
                                                                parseSuccess = false;
                                                                result6 = ParseExpression(tokens, ref currentIndex6, ref parseSuccess);
                                                            }
                                                            if (parseSuccess)
                                                            {
                                                                currentIndexCopy6 = currentIndex6;
                                                            }
                                                            else
                                                            {
                                                                goto LABEL_3;
                                                            }
                                                            currentIndex5 = currentIndexCopy6;
                                                            result5.Add(result6);
                                                            result5.End = result6.End;
                                                        }
                                                    LABEL_3: ; parseSuccess = true;
                                                    }
                                                }
                                                if (parseSuccess)
                                                {
                                                    currentIndex4 = currentIndex5;
                                                    ArgumentsMember4 = result5;
                                                }
                                            }
                                            ArgumentsMember3 = ArgumentsMember4;
                                        }
                                        if (parseSuccess)
                                        {
                                            currentIndexCopy4 = currentIndex4;
                                        }
                                        else
                                        {
                                            goto LABEL_2;
                                        }
                                        {
                                            Developer.LanguageProvider.CodeTokenizer.ParseToken(tokens, ref currentIndex4, ref parseSuccess, ")");
                                        }
                                        if (parseSuccess)
                                        {
                                            currentIndexCopy4 = currentIndex4;
                                        }
                                        else
                                        {
                                            goto LABEL_2;
                                        }
                                        currentIndex2 = currentIndexCopy4;
                                    LABEL_2: ;
                                    }
                                    ArgumentsMember2 = ArgumentsMember3;
                                }
                                if (parseSuccess)
                                {
                                    currentIndex1 = currentIndex2;
                                    Developer.LanguageServices.NativeX.SyntaxTree.NativeXInvokeExpression result2 = CodeNode.Create<Developer.LanguageServices.NativeX.SyntaxTree.NativeXInvokeExpression>();
                                    result2.Arguments = ArgumentsMember2;
                                    result2.Function = result1;
                                    result2.Start = result1.Start;
                                    result2.End = Developer.LanguageProvider.CodeTokenizer.GetEndPosition(tokens, currentIndex1);
                                    result1 = result2;
                                    continue;
                                }
                            }
                            {
                                System.String MemberNameMember2 = default(System.String);
                                {
                                    System.String MemberNameMember3 = default(System.String);
                                    {
                                        int currentIndexCopy4 = currentIndex2;
                                        int currentIndex4 = currentIndex2;
                                        {
                                            Developer.LanguageProvider.CodeTokenizer.ParseToken(tokens, ref currentIndex4, ref parseSuccess, ".");
                                        }
                                        if (parseSuccess)
                                        {
                                            currentIndexCopy4 = currentIndex4;
                                        }
                                        else
                                        {
                                            goto LABEL_4;
                                        }
                                        {
                                            System.String MemberNameMember4 = default(System.String);
                                            {
                                                Developer.LanguageProvider.CodeToken result5 = default(Developer.LanguageProvider.CodeToken);
                                                int currentIndex5 = currentIndex4;
                                                {
                                                    result5 = Developer.LanguageProvider.CodeTokenizer.ParseToken(tokens, ref currentIndex5, ref parseSuccess, Developer.LanguageServices.NativeX.NativeXTokenizer.IdToken);
                                                }
                                                if (parseSuccess)
                                                {
                                                    currentIndex4 = currentIndex5;
                                                    MemberNameMember4 = result5.Value;
                                                }
                                            }
                                            MemberNameMember3 = MemberNameMember4;
                                        }
                                        if (parseSuccess)
                                        {
                                            currentIndexCopy4 = currentIndex4;
                                        }
                                        else
                                        {
                                            goto LABEL_4;
                                        }
                                        currentIndex2 = currentIndexCopy4;
                                    LABEL_4: ;
                                    }
                                    MemberNameMember2 = MemberNameMember3;
                                }
                                if (parseSuccess)
                                {
                                    currentIndex1 = currentIndex2;
                                    Developer.LanguageServices.NativeX.SyntaxTree.NativeXMemberExpression result2 = CodeNode.Create<Developer.LanguageServices.NativeX.SyntaxTree.NativeXMemberExpression>();
                                    result2.MemberName = MemberNameMember2;
                                    result2.Operand = result1;
                                    result2.Start = result1.Start;
                                    result2.End = Developer.LanguageProvider.CodeTokenizer.GetEndPosition(tokens, currentIndex1);
                                    result1 = result2;
                                    continue;
                                }
                            }
                            {
                                System.String MemberNameMember2 = default(System.String);
                                {
                                    System.String MemberNameMember3 = default(System.String);
                                    {
                                        int currentIndexCopy4 = currentIndex2;
                                        int currentIndex4 = currentIndex2;
                                        {
                                            Developer.LanguageProvider.CodeTokenizer.ParseToken(tokens, ref currentIndex4, ref parseSuccess, "->");
                                        }
                                        if (parseSuccess)
                                        {
                                            currentIndexCopy4 = currentIndex4;
                                        }
                                        else
                                        {
                                            goto LABEL_5;
                                        }
                                        {
                                            System.String MemberNameMember4 = default(System.String);
                                            {
                                                Developer.LanguageProvider.CodeToken result5 = default(Developer.LanguageProvider.CodeToken);
                                                int currentIndex5 = currentIndex4;
                                                {
                                                    result5 = Developer.LanguageProvider.CodeTokenizer.ParseToken(tokens, ref currentIndex5, ref parseSuccess, Developer.LanguageServices.NativeX.NativeXTokenizer.IdToken);
                                                }
                                                if (parseSuccess)
                                                {
                                                    currentIndex4 = currentIndex5;
                                                    MemberNameMember4 = result5.Value;
                                                }
                                            }
                                            MemberNameMember3 = MemberNameMember4;
                                        }
                                        if (parseSuccess)
                                        {
                                            currentIndexCopy4 = currentIndex4;
                                        }
                                        else
                                        {
                                            goto LABEL_5;
                                        }
                                        currentIndex2 = currentIndexCopy4;
                                    LABEL_5: ;
                                    }
                                    MemberNameMember2 = MemberNameMember3;
                                }
                                if (parseSuccess)
                                {
                                    currentIndex1 = currentIndex2;
                                    Developer.LanguageServices.NativeX.SyntaxTree.NativeXPointerMemberExpression result2 = CodeNode.Create<Developer.LanguageServices.NativeX.SyntaxTree.NativeXPointerMemberExpression>();
                                    result2.MemberName = MemberNameMember2;
                                    result2.Operand = result1;
                                    result2.Start = result1.Start;
                                    result2.End = Developer.LanguageProvider.CodeTokenizer.GetEndPosition(tokens, currentIndex1);
                                    result1 = result2;
                                    continue;
                                }
                            }
                            {
                                System.String OperatorMember2 = default(System.String);
                                {
                                    System.String OperatorMember3 = default(System.String);
                                    {
                                        Developer.LanguageProvider.CodeToken result4 = default(Developer.LanguageProvider.CodeToken);
                                        int currentIndex4 = currentIndex2;
                                        {
                                            result4 = Developer.LanguageProvider.CodeTokenizer.ParseToken(tokens, ref currentIndex4, ref parseSuccess, "++", "--");
                                        }
                                        if (parseSuccess)
                                        {
                                            currentIndex2 = currentIndex4;
                                            OperatorMember3 = result4.Value;
                                        }
                                    }
                                    OperatorMember2 = OperatorMember3;
                                }
                                if (parseSuccess)
                                {
                                    currentIndex1 = currentIndex2;
                                    Developer.LanguageServices.NativeX.SyntaxTree.NativeXPostUnaryExpression result2 = CodeNode.Create<Developer.LanguageServices.NativeX.SyntaxTree.NativeXPostUnaryExpression>();
                                    result2.Operator = OperatorMember2;
                                    result2.Operand = result1;
                                    result2.Start = result1.Start;
                                    result2.End = Developer.LanguageProvider.CodeTokenizer.GetEndPosition(tokens, currentIndex1);
                                    result1 = result2;
                                    continue;
                                }
                            }
                            break;
                        }
                        parseSuccess = true;
                    LABEL_0: ;
                    }
                }
                if (parseSuccess)
                {
                    currentToken = currentIndex1;
                    result = result1;
                }
            }
            if (parseSuccess)
            {
                if (result == null) result = CodeNode.Create<Developer.LanguageServices.NativeX.SyntaxTree.NativeXExpression>();
                result.Start = start;
                result.End = Developer.LanguageProvider.CodeTokenizer.GetEndPosition(tokens, currentToken);
            }
            return result;
        }

        public static Developer.LanguageServices.NativeX.SyntaxTree.NativeXExpression ParseEXP0(List<CodeToken> tokens, ref int currentToken, ref bool parseSuccess)
        {
            Developer.LanguageProvider.TextPosition start = Developer.LanguageProvider.CodeTokenizer.GetStartPosition(tokens, currentToken);
            Developer.LanguageServices.NativeX.SyntaxTree.NativeXExpression result = default(Developer.LanguageServices.NativeX.SyntaxTree.NativeXExpression);
            {
                Developer.LanguageServices.NativeX.SyntaxTree.NativeXExpression result1 = default(Developer.LanguageServices.NativeX.SyntaxTree.NativeXExpression);
                int currentIndex1 = -1;
                currentIndex1 = currentToken;
                {
                    {
                        Developer.LanguageServices.NativeX.SyntaxTree.NativeXFunctionResultExpression result2 = default(Developer.LanguageServices.NativeX.SyntaxTree.NativeXFunctionResultExpression);
                        int currentIndex2 = currentIndex1;
                        {
                            parseSuccess = false;
                            result2 = ParseResult(tokens, ref currentIndex2, ref parseSuccess);
                        }
                        if (parseSuccess)
                        {
                            currentIndex1 = currentIndex2;
                            result1 = result2;
                        }
                    }
                }
                if (parseSuccess)
                {
                    currentToken = currentIndex1;
                    goto LABEL_SUCCESS_0;
                }
                currentIndex1 = currentToken;
                {
                    {
                        Developer.LanguageServices.NativeX.SyntaxTree.NativeXExceptionExpression result2 = default(Developer.LanguageServices.NativeX.SyntaxTree.NativeXExceptionExpression);
                        int currentIndex2 = currentIndex1;
                        {
                            parseSuccess = false;
                            result2 = ParseException(tokens, ref currentIndex2, ref parseSuccess);
                        }
                        if (parseSuccess)
                        {
                            currentIndex1 = currentIndex2;
                            result1 = result2;
                        }
                    }
                }
                if (parseSuccess)
                {
                    currentToken = currentIndex1;
                    goto LABEL_SUCCESS_0;
                }
                currentIndex1 = currentToken;
                {
                    {
                        Developer.LanguageServices.NativeX.SyntaxTree.NativeXCastingExpression result2 = default(Developer.LanguageServices.NativeX.SyntaxTree.NativeXCastingExpression);
                        int currentIndex2 = currentIndex1;
                        {
                            parseSuccess = false;
                            result2 = ParseCasting(tokens, ref currentIndex2, ref parseSuccess);
                        }
                        if (parseSuccess)
                        {
                            currentIndex1 = currentIndex2;
                            result1 = result2;
                        }
                    }
                }
                if (parseSuccess)
                {
                    currentToken = currentIndex1;
                    goto LABEL_SUCCESS_0;
                }
                currentIndex1 = currentToken;
                {
                    {
                        Developer.LanguageServices.NativeX.SyntaxTree.NativeXPrimitiveExpression result2 = default(Developer.LanguageServices.NativeX.SyntaxTree.NativeXPrimitiveExpression);
                        int currentIndex2 = currentIndex1;
                        {
                            parseSuccess = false;
                            result2 = ParsePrimitive(tokens, ref currentIndex2, ref parseSuccess);
                        }
                        if (parseSuccess)
                        {
                            currentIndex1 = currentIndex2;
                            result1 = result2;
                        }
                    }
                }
                if (parseSuccess)
                {
                    currentToken = currentIndex1;
                    goto LABEL_SUCCESS_0;
                }
                currentIndex1 = currentToken;
                {
                    {
                        Developer.LanguageServices.NativeX.SyntaxTree.NativeXExpression result2 = default(Developer.LanguageServices.NativeX.SyntaxTree.NativeXExpression);
                        int currentIndex2 = currentIndex1;
                        {
                            parseSuccess = false;
                            result2 = ParseReference(tokens, ref currentIndex2, ref parseSuccess);
                        }
                        if (parseSuccess)
                        {
                            currentIndex1 = currentIndex2;
                            result1 = result2;
                        }
                    }
                }
                if (parseSuccess)
                {
                    currentToken = currentIndex1;
                    goto LABEL_SUCCESS_0;
                }
                currentIndex1 = currentToken;
                {
                    {
                        Developer.LanguageServices.NativeX.SyntaxTree.NativeXExpression result2 = default(Developer.LanguageServices.NativeX.SyntaxTree.NativeXExpression);
                        int currentIndexCopy2 = currentIndex1;
                        int currentIndex2 = currentIndex1;
                        {
                            Developer.LanguageProvider.CodeTokenizer.ParseToken(tokens, ref currentIndex2, ref parseSuccess, "(");
                        }
                        if (parseSuccess)
                        {
                            currentIndexCopy2 = currentIndex2;
                        }
                        else
                        {
                            goto LABEL_2;
                        }
                        {
                            {
                                Developer.LanguageServices.NativeX.SyntaxTree.NativeXExpression result3 = default(Developer.LanguageServices.NativeX.SyntaxTree.NativeXExpression);
                                int currentIndex3 = currentIndex2;
                                {
                                    parseSuccess = false;
                                    result3 = ParseExpression(tokens, ref currentIndex3, ref parseSuccess);
                                }
                                if (parseSuccess)
                                {
                                    currentIndex2 = currentIndex3;
                                    result2 = result3;
                                }
                            }
                        }
                        if (parseSuccess)
                        {
                            currentIndexCopy2 = currentIndex2;
                        }
                        else
                        {
                            goto LABEL_2;
                        }
                        {
                            Developer.LanguageProvider.CodeTokenizer.ParseToken(tokens, ref currentIndex2, ref parseSuccess, ")");
                        }
                        if (parseSuccess)
                        {
                            currentIndexCopy2 = currentIndex2;
                        }
                        else
                        {
                            goto LABEL_2;
                        }
                        currentIndex1 = currentIndexCopy2;
                        result1 = result2;
                    LABEL_2: ;
                    }
                }
                if (parseSuccess)
                {
                    currentToken = currentIndex1;
                    goto LABEL_SUCCESS_0;
                }
                goto LABEL_FAIL_1;
            LABEL_SUCCESS_0: ;
                result = result1;
            LABEL_FAIL_1: ;
            }
            if (parseSuccess)
            {
                if (result == null) result = CodeNode.Create<Developer.LanguageServices.NativeX.SyntaxTree.NativeXExpression>();
                result.Start = start;
                result.End = Developer.LanguageProvider.CodeTokenizer.GetEndPosition(tokens, currentToken);
            }
            return result;
        }

        public static Developer.LanguageServices.NativeX.SyntaxTree.NativeXFunctionResultExpression ParseResult(List<CodeToken> tokens, ref int currentToken, ref bool parseSuccess)
        {
            Developer.LanguageProvider.TextPosition start = Developer.LanguageProvider.CodeTokenizer.GetStartPosition(tokens, currentToken);
            Developer.LanguageServices.NativeX.SyntaxTree.NativeXFunctionResultExpression result = default(Developer.LanguageServices.NativeX.SyntaxTree.NativeXFunctionResultExpression);
            Developer.LanguageProvider.CodeTokenizer.ParseToken(tokens, ref currentToken, ref parseSuccess, "result");
            if (parseSuccess)
            {
                if (result == null) result = CodeNode.Create<Developer.LanguageServices.NativeX.SyntaxTree.NativeXFunctionResultExpression>();
                result.Start = start;
                result.End = Developer.LanguageProvider.CodeTokenizer.GetEndPosition(tokens, currentToken);
            }
            return result;
        }

        public static Developer.LanguageServices.NativeX.SyntaxTree.NativeXExceptionExpression ParseException(List<CodeToken> tokens, ref int currentToken, ref bool parseSuccess)
        {
            Developer.LanguageProvider.TextPosition start = Developer.LanguageProvider.CodeTokenizer.GetStartPosition(tokens, currentToken);
            Developer.LanguageServices.NativeX.SyntaxTree.NativeXExceptionExpression result = default(Developer.LanguageServices.NativeX.SyntaxTree.NativeXExceptionExpression);
            Developer.LanguageProvider.CodeTokenizer.ParseToken(tokens, ref currentToken, ref parseSuccess, "exception");
            if (parseSuccess)
            {
                if (result == null) result = CodeNode.Create<Developer.LanguageServices.NativeX.SyntaxTree.NativeXExceptionExpression>();
                result.Start = start;
                result.End = Developer.LanguageProvider.CodeTokenizer.GetEndPosition(tokens, currentToken);
            }
            return result;
        }

        public static Developer.LanguageServices.NativeX.SyntaxTree.NativeXCastingExpression ParseCasting(List<CodeToken> tokens, ref int currentToken, ref bool parseSuccess)
        {
            Developer.LanguageProvider.TextPosition start = Developer.LanguageProvider.CodeTokenizer.GetStartPosition(tokens, currentToken);
            Developer.LanguageServices.NativeX.SyntaxTree.NativeXCastingExpression result = default(Developer.LanguageServices.NativeX.SyntaxTree.NativeXCastingExpression);
            Developer.LanguageServices.NativeX.SyntaxTree.NativeXType TypeMember0 = default(Developer.LanguageServices.NativeX.SyntaxTree.NativeXType);
            Developer.LanguageServices.NativeX.SyntaxTree.NativeXExpression OperandMember0 = default(Developer.LanguageServices.NativeX.SyntaxTree.NativeXExpression);
            {
                int currentIndexCopy1 = currentToken;
                int currentIndex1 = currentToken;
                {
                    Developer.LanguageProvider.CodeTokenizer.ParseToken(tokens, ref currentIndex1, ref parseSuccess, "cast");
                }
                if (parseSuccess)
                {
                    currentIndexCopy1 = currentIndex1;
                }
                else
                {
                    goto LABEL_0;
                }
                {
                    Developer.LanguageProvider.CodeTokenizer.ParseToken(tokens, ref currentIndex1, ref parseSuccess, "<");
                }
                if (parseSuccess)
                {
                    currentIndexCopy1 = currentIndex1;
                }
                else
                {
                    goto LABEL_0;
                }
                {
                    Developer.LanguageServices.NativeX.SyntaxTree.NativeXType TypeMember1 = default(Developer.LanguageServices.NativeX.SyntaxTree.NativeXType);
                    {
                        Developer.LanguageServices.NativeX.SyntaxTree.NativeXType result2 = default(Developer.LanguageServices.NativeX.SyntaxTree.NativeXType);
                        int currentIndex2 = currentIndex1;
                        {
                            parseSuccess = false;
                            result2 = ParseType(tokens, ref currentIndex2, ref parseSuccess);
                        }
                        if (parseSuccess)
                        {
                            currentIndex1 = currentIndex2;
                            TypeMember1 = result2;
                        }
                    }
                    TypeMember0 = TypeMember1;
                }
                if (parseSuccess)
                {
                    currentIndexCopy1 = currentIndex1;
                }
                else
                {
                    goto LABEL_0;
                }
                {
                    Developer.LanguageProvider.CodeTokenizer.ParseToken(tokens, ref currentIndex1, ref parseSuccess, ">");
                }
                if (parseSuccess)
                {
                    currentIndexCopy1 = currentIndex1;
                }
                else
                {
                    goto LABEL_0;
                }
                {
                    Developer.LanguageProvider.CodeTokenizer.ParseToken(tokens, ref currentIndex1, ref parseSuccess, "(");
                }
                if (parseSuccess)
                {
                    currentIndexCopy1 = currentIndex1;
                }
                else
                {
                    goto LABEL_0;
                }
                {
                    Developer.LanguageServices.NativeX.SyntaxTree.NativeXExpression OperandMember1 = default(Developer.LanguageServices.NativeX.SyntaxTree.NativeXExpression);
                    {
                        Developer.LanguageServices.NativeX.SyntaxTree.NativeXExpression result2 = default(Developer.LanguageServices.NativeX.SyntaxTree.NativeXExpression);
                        int currentIndex2 = currentIndex1;
                        {
                            parseSuccess = false;
                            result2 = ParseExpression(tokens, ref currentIndex2, ref parseSuccess);
                        }
                        if (parseSuccess)
                        {
                            currentIndex1 = currentIndex2;
                            OperandMember1 = result2;
                        }
                    }
                    OperandMember0 = OperandMember1;
                }
                if (parseSuccess)
                {
                    currentIndexCopy1 = currentIndex1;
                }
                else
                {
                    goto LABEL_0;
                }
                {
                    Developer.LanguageProvider.CodeTokenizer.ParseToken(tokens, ref currentIndex1, ref parseSuccess, ")");
                }
                if (parseSuccess)
                {
                    currentIndexCopy1 = currentIndex1;
                }
                else
                {
                    goto LABEL_0;
                }
                currentToken = currentIndexCopy1;
            LABEL_0: ;
            }
            if (parseSuccess)
            {
                if (result == null) result = CodeNode.Create<Developer.LanguageServices.NativeX.SyntaxTree.NativeXCastingExpression>();
                result.Type = TypeMember0;
                result.Operand = OperandMember0;
                result.Start = start;
                result.End = Developer.LanguageProvider.CodeTokenizer.GetEndPosition(tokens, currentToken);
            }
            return result;
        }

        public static Developer.LanguageServices.NativeX.SyntaxTree.NativeXExpression ParseReference(List<CodeToken> tokens, ref int currentToken, ref bool parseSuccess)
        {
            Developer.LanguageProvider.TextPosition start = Developer.LanguageProvider.CodeTokenizer.GetStartPosition(tokens, currentToken);
            Developer.LanguageServices.NativeX.SyntaxTree.NativeXExpression result = default(Developer.LanguageServices.NativeX.SyntaxTree.NativeXExpression);
            {
                Developer.LanguageServices.NativeX.SyntaxTree.NativeXExpression result1 = default(Developer.LanguageServices.NativeX.SyntaxTree.NativeXExpression);
                int currentIndex1 = -1;
                currentIndex1 = currentToken;
                {
                    {
                        Developer.LanguageServices.NativeX.SyntaxTree.NativeXInstanceFunctionExpression result2 = default(Developer.LanguageServices.NativeX.SyntaxTree.NativeXInstanceFunctionExpression);
                        int currentIndex2 = currentIndex1;
                        {
                            parseSuccess = false;
                            result2 = ParseInstanceFunctionReference(tokens, ref currentIndex2, ref parseSuccess);
                        }
                        if (parseSuccess)
                        {
                            currentIndex1 = currentIndex2;
                            result1 = result2;
                        }
                    }
                }
                if (parseSuccess)
                {
                    currentToken = currentIndex1;
                    goto LABEL_SUCCESS_0;
                }
                currentIndex1 = currentToken;
                {
                    {
                        Developer.LanguageServices.NativeX.SyntaxTree.NativeXInstanciatedExpression result2 = default(Developer.LanguageServices.NativeX.SyntaxTree.NativeXInstanciatedExpression);
                        int currentIndex2 = currentIndex1;
                        {
                            parseSuccess = false;
                            result2 = ParseInstanciatedReference(tokens, ref currentIndex2, ref parseSuccess);
                        }
                        if (parseSuccess)
                        {
                            currentIndex1 = currentIndex2;
                            result1 = result2;
                        }
                    }
                }
                if (parseSuccess)
                {
                    currentToken = currentIndex1;
                    goto LABEL_SUCCESS_0;
                }
                currentIndex1 = currentToken;
                {
                    {
                        Developer.LanguageServices.NativeX.SyntaxTree.NativeXReferenceExpression result2 = default(Developer.LanguageServices.NativeX.SyntaxTree.NativeXReferenceExpression);
                        int currentIndex2 = currentIndex1;
                        {
                            parseSuccess = false;
                            result2 = ParseIdentifierReference(tokens, ref currentIndex2, ref parseSuccess);
                        }
                        if (parseSuccess)
                        {
                            currentIndex1 = currentIndex2;
                            result1 = result2;
                        }
                    }
                }
                if (parseSuccess)
                {
                    currentToken = currentIndex1;
                    goto LABEL_SUCCESS_0;
                }
                goto LABEL_FAIL_1;
            LABEL_SUCCESS_0: ;
                result = result1;
            LABEL_FAIL_1: ;
            }
            if (parseSuccess)
            {
                if (result == null) result = CodeNode.Create<Developer.LanguageServices.NativeX.SyntaxTree.NativeXExpression>();
                result.Start = start;
                result.End = Developer.LanguageProvider.CodeTokenizer.GetEndPosition(tokens, currentToken);
            }
            return result;
        }

        public static Developer.LanguageServices.NativeX.SyntaxTree.NativeXInstanceFunctionExpression ParseInstanceFunctionReference(List<CodeToken> tokens, ref int currentToken, ref bool parseSuccess)
        {
            Developer.LanguageProvider.TextPosition start = Developer.LanguageProvider.CodeTokenizer.GetStartPosition(tokens, currentToken);
            Developer.LanguageServices.NativeX.SyntaxTree.NativeXInstanceFunctionExpression result = default(Developer.LanguageServices.NativeX.SyntaxTree.NativeXInstanceFunctionExpression);
            System.String ConceptNameMember0 = default(System.String);
            Developer.LanguageServices.NativeX.SyntaxTree.NativeXType TypeMember0 = default(Developer.LanguageServices.NativeX.SyntaxTree.NativeXType);
            System.String FunctionNameMember0 = default(System.String);
            {
                int currentIndexCopy1 = currentToken;
                int currentIndex1 = currentToken;
                {
                    System.String ConceptNameMember1 = default(System.String);
                    {
                        Developer.LanguageProvider.CodeToken result2 = default(Developer.LanguageProvider.CodeToken);
                        int currentIndex2 = currentIndex1;
                        {
                            result2 = Developer.LanguageProvider.CodeTokenizer.ParseToken(tokens, ref currentIndex2, ref parseSuccess, Developer.LanguageServices.NativeX.NativeXTokenizer.IdToken);
                        }
                        if (parseSuccess)
                        {
                            currentIndex1 = currentIndex2;
                            ConceptNameMember1 = result2.Value;
                        }
                    }
                    ConceptNameMember0 = ConceptNameMember1;
                }
                if (parseSuccess)
                {
                    currentIndexCopy1 = currentIndex1;
                }
                else
                {
                    goto LABEL_0;
                }
                {
                    Developer.LanguageProvider.CodeTokenizer.ParseToken(tokens, ref currentIndex1, ref parseSuccess, "<");
                }
                if (parseSuccess)
                {
                    currentIndexCopy1 = currentIndex1;
                }
                else
                {
                    goto LABEL_0;
                }
                {
                    Developer.LanguageServices.NativeX.SyntaxTree.NativeXType TypeMember1 = default(Developer.LanguageServices.NativeX.SyntaxTree.NativeXType);
                    {
                        Developer.LanguageServices.NativeX.SyntaxTree.NativeXType result2 = default(Developer.LanguageServices.NativeX.SyntaxTree.NativeXType);
                        int currentIndex2 = currentIndex1;
                        {
                            parseSuccess = false;
                            result2 = ParseType(tokens, ref currentIndex2, ref parseSuccess);
                        }
                        if (parseSuccess)
                        {
                            currentIndex1 = currentIndex2;
                            TypeMember1 = result2;
                        }
                    }
                    TypeMember0 = TypeMember1;
                }
                if (parseSuccess)
                {
                    currentIndexCopy1 = currentIndex1;
                }
                else
                {
                    goto LABEL_0;
                }
                {
                    Developer.LanguageProvider.CodeTokenizer.ParseToken(tokens, ref currentIndex1, ref parseSuccess, ">");
                }
                if (parseSuccess)
                {
                    currentIndexCopy1 = currentIndex1;
                }
                else
                {
                    goto LABEL_0;
                }
                {
                    Developer.LanguageProvider.CodeTokenizer.ParseToken(tokens, ref currentIndex1, ref parseSuccess, ":");
                }
                if (parseSuccess)
                {
                    currentIndexCopy1 = currentIndex1;
                }
                else
                {
                    goto LABEL_0;
                }
                {
                    Developer.LanguageProvider.CodeTokenizer.ParseToken(tokens, ref currentIndex1, ref parseSuccess, ":");
                }
                if (parseSuccess)
                {
                    currentIndexCopy1 = currentIndex1;
                }
                else
                {
                    goto LABEL_0;
                }
                {
                    System.String FunctionNameMember1 = default(System.String);
                    {
                        Developer.LanguageProvider.CodeToken result2 = default(Developer.LanguageProvider.CodeToken);
                        int currentIndex2 = currentIndex1;
                        {
                            result2 = Developer.LanguageProvider.CodeTokenizer.ParseToken(tokens, ref currentIndex2, ref parseSuccess, Developer.LanguageServices.NativeX.NativeXTokenizer.IdToken);
                        }
                        if (parseSuccess)
                        {
                            currentIndex1 = currentIndex2;
                            FunctionNameMember1 = result2.Value;
                        }
                    }
                    FunctionNameMember0 = FunctionNameMember1;
                }
                if (parseSuccess)
                {
                    currentIndexCopy1 = currentIndex1;
                }
                else
                {
                    goto LABEL_0;
                }
                currentToken = currentIndexCopy1;
            LABEL_0: ;
            }
            if (parseSuccess)
            {
                if (result == null) result = CodeNode.Create<Developer.LanguageServices.NativeX.SyntaxTree.NativeXInstanceFunctionExpression>();
                result.ConceptName = ConceptNameMember0;
                result.Type = TypeMember0;
                result.FunctionName = FunctionNameMember0;
                result.Start = start;
                result.End = Developer.LanguageProvider.CodeTokenizer.GetEndPosition(tokens, currentToken);
            }
            return result;
        }

        public static Developer.LanguageServices.NativeX.SyntaxTree.NativeXInstanciatedExpression ParseInstanciatedReference(List<CodeToken> tokens, ref int currentToken, ref bool parseSuccess)
        {
            Developer.LanguageProvider.TextPosition start = Developer.LanguageProvider.CodeTokenizer.GetStartPosition(tokens, currentToken);
            Developer.LanguageServices.NativeX.SyntaxTree.NativeXInstanciatedExpression result = default(Developer.LanguageServices.NativeX.SyntaxTree.NativeXInstanciatedExpression);
            System.String ReferencedNameMember0 = default(System.String);
            Developer.LanguageProvider.CodeNodeList<Developer.LanguageServices.NativeX.SyntaxTree.NativeXType> GenericArgumentsMember0 = default(Developer.LanguageProvider.CodeNodeList<Developer.LanguageServices.NativeX.SyntaxTree.NativeXType>);
            {
                int currentIndexCopy1 = currentToken;
                int currentIndex1 = currentToken;
                {
                    System.String ReferencedNameMember1 = default(System.String);
                    {
                        Developer.LanguageProvider.CodeToken result2 = default(Developer.LanguageProvider.CodeToken);
                        int currentIndex2 = currentIndex1;
                        {
                            result2 = Developer.LanguageProvider.CodeTokenizer.ParseToken(tokens, ref currentIndex2, ref parseSuccess, Developer.LanguageServices.NativeX.NativeXTokenizer.IdToken);
                        }
                        if (parseSuccess)
                        {
                            currentIndex1 = currentIndex2;
                            ReferencedNameMember1 = result2.Value;
                        }
                    }
                    ReferencedNameMember0 = ReferencedNameMember1;
                }
                if (parseSuccess)
                {
                    currentIndexCopy1 = currentIndex1;
                }
                else
                {
                    goto LABEL_0;
                }
                {
                    Developer.LanguageProvider.CodeTokenizer.ParseToken(tokens, ref currentIndex1, ref parseSuccess, "<");
                }
                if (parseSuccess)
                {
                    currentIndexCopy1 = currentIndex1;
                }
                else
                {
                    goto LABEL_0;
                }
                {
                    Developer.LanguageProvider.CodeNodeList<Developer.LanguageServices.NativeX.SyntaxTree.NativeXType> GenericArgumentsMember1 = default(Developer.LanguageProvider.CodeNodeList<Developer.LanguageServices.NativeX.SyntaxTree.NativeXType>);
                    {
                        Developer.LanguageProvider.CodeNodeList<Developer.LanguageServices.NativeX.SyntaxTree.NativeXType> result2 = default(Developer.LanguageProvider.CodeNodeList<Developer.LanguageServices.NativeX.SyntaxTree.NativeXType>);
                        int currentIndex2 = currentIndex1;
                        {
                            result2 = new Developer.LanguageProvider.CodeNodeList<Developer.LanguageServices.NativeX.SyntaxTree.NativeXType>();
                            {
                                Developer.LanguageServices.NativeX.SyntaxTree.NativeXType result3 = default(Developer.LanguageServices.NativeX.SyntaxTree.NativeXType);
                                int currentIndex3 = currentIndex2;
                                {
                                    parseSuccess = false;
                                    result3 = ParseType(tokens, ref currentIndex3, ref parseSuccess);
                                }
                                if (parseSuccess)
                                {
                                    currentIndex2 = currentIndex3;
                                    result2.Add(result3);
                                    result2.Start = result3.Start;
                                    result2.End = result3.End;
                                }
                                while (true)
                                {
                                    int currentIndexCopy3 = currentIndex2;
                                    {
                                        Developer.LanguageProvider.CodeTokenizer.ParseToken(tokens, ref currentIndex3, ref parseSuccess, ",");
                                    }
                                    if (parseSuccess)
                                    {
                                        currentIndexCopy3 = currentIndex3;
                                    }
                                    else
                                    {
                                        goto LABEL_1;
                                    }
                                    {
                                        parseSuccess = false;
                                        result3 = ParseType(tokens, ref currentIndex3, ref parseSuccess);
                                    }
                                    if (parseSuccess)
                                    {
                                        currentIndexCopy3 = currentIndex3;
                                    }
                                    else
                                    {
                                        goto LABEL_1;
                                    }
                                    currentIndex2 = currentIndexCopy3;
                                    result2.Add(result3);
                                    result2.End = result3.End;
                                }
                            LABEL_1: ; parseSuccess = true;
                            }
                        }
                        if (parseSuccess)
                        {
                            currentIndex1 = currentIndex2;
                            GenericArgumentsMember1 = result2;
                        }
                    }
                    GenericArgumentsMember0 = GenericArgumentsMember1;
                }
                if (parseSuccess)
                {
                    currentIndexCopy1 = currentIndex1;
                }
                else
                {
                    goto LABEL_0;
                }
                {
                    Developer.LanguageProvider.CodeTokenizer.ParseToken(tokens, ref currentIndex1, ref parseSuccess, ">");
                }
                if (parseSuccess)
                {
                    currentIndexCopy1 = currentIndex1;
                }
                else
                {
                    goto LABEL_0;
                }
                currentToken = currentIndexCopy1;
            LABEL_0: ;
            }
            if (parseSuccess)
            {
                if (result == null) result = CodeNode.Create<Developer.LanguageServices.NativeX.SyntaxTree.NativeXInstanciatedExpression>();
                result.ReferencedName = ReferencedNameMember0;
                result.GenericArguments = GenericArgumentsMember0;
                result.Start = start;
                result.End = Developer.LanguageProvider.CodeTokenizer.GetEndPosition(tokens, currentToken);
            }
            return result;
        }

        public static Developer.LanguageServices.NativeX.SyntaxTree.NativeXReferenceExpression ParseIdentifierReference(List<CodeToken> tokens, ref int currentToken, ref bool parseSuccess)
        {
            Developer.LanguageProvider.TextPosition start = Developer.LanguageProvider.CodeTokenizer.GetStartPosition(tokens, currentToken);
            Developer.LanguageServices.NativeX.SyntaxTree.NativeXReferenceExpression result = default(Developer.LanguageServices.NativeX.SyntaxTree.NativeXReferenceExpression);
            System.String ReferencedNameMember0 = default(System.String);
            {
                Developer.LanguageProvider.CodeToken result1 = default(Developer.LanguageProvider.CodeToken);
                int currentIndex1 = currentToken;
                {
                    result1 = Developer.LanguageProvider.CodeTokenizer.ParseToken(tokens, ref currentIndex1, ref parseSuccess, Developer.LanguageServices.NativeX.NativeXTokenizer.IdToken);
                }
                if (parseSuccess)
                {
                    currentToken = currentIndex1;
                    ReferencedNameMember0 = result1.Value;
                }
            }
            if (parseSuccess)
            {
                if (result == null) result = CodeNode.Create<Developer.LanguageServices.NativeX.SyntaxTree.NativeXReferenceExpression>();
                result.ReferencedName = ReferencedNameMember0;
                result.Start = start;
                result.End = Developer.LanguageProvider.CodeTokenizer.GetEndPosition(tokens, currentToken);
            }
            return result;
        }

        public static Developer.LanguageServices.NativeX.SyntaxTree.NativeXUnaryExpression ParseUnary(List<CodeToken> tokens, ref int currentToken, ref bool parseSuccess)
        {
            Developer.LanguageProvider.TextPosition start = Developer.LanguageProvider.CodeTokenizer.GetStartPosition(tokens, currentToken);
            Developer.LanguageServices.NativeX.SyntaxTree.NativeXUnaryExpression result = default(Developer.LanguageServices.NativeX.SyntaxTree.NativeXUnaryExpression);
            System.String OperatorMember0 = default(System.String);
            Developer.LanguageServices.NativeX.SyntaxTree.NativeXExpression OperandMember0 = default(Developer.LanguageServices.NativeX.SyntaxTree.NativeXExpression);
            {
                int currentIndexCopy1 = currentToken;
                int currentIndex1 = currentToken;
                {
                    System.String OperatorMember1 = default(System.String);
                    {
                        Developer.LanguageProvider.CodeToken result2 = default(Developer.LanguageProvider.CodeToken);
                        int currentIndex2 = currentIndex1;
                        {
                            result2 = Developer.LanguageProvider.CodeTokenizer.ParseToken(tokens, ref currentIndex2, ref parseSuccess, "++", "--", "&", "*", "-", "!", "~");
                        }
                        if (parseSuccess)
                        {
                            currentIndex1 = currentIndex2;
                            OperatorMember1 = result2.Value;
                        }
                    }
                    OperatorMember0 = OperatorMember1;
                }
                if (parseSuccess)
                {
                    currentIndexCopy1 = currentIndex1;
                }
                else
                {
                    goto LABEL_0;
                }
                {
                    Developer.LanguageServices.NativeX.SyntaxTree.NativeXExpression OperandMember1 = default(Developer.LanguageServices.NativeX.SyntaxTree.NativeXExpression);
                    {
                        Developer.LanguageServices.NativeX.SyntaxTree.NativeXExpression result2 = default(Developer.LanguageServices.NativeX.SyntaxTree.NativeXExpression);
                        int currentIndex2 = currentIndex1;
                        {
                            parseSuccess = false;
                            result2 = ParseEXP2(tokens, ref currentIndex2, ref parseSuccess);
                        }
                        if (parseSuccess)
                        {
                            currentIndex1 = currentIndex2;
                            OperandMember1 = result2;
                        }
                    }
                    OperandMember0 = OperandMember1;
                }
                if (parseSuccess)
                {
                    currentIndexCopy1 = currentIndex1;
                }
                else
                {
                    goto LABEL_0;
                }
                currentToken = currentIndexCopy1;
            LABEL_0: ;
            }
            if (parseSuccess)
            {
                if (result == null) result = CodeNode.Create<Developer.LanguageServices.NativeX.SyntaxTree.NativeXUnaryExpression>();
                result.Operator = OperatorMember0;
                result.Operand = OperandMember0;
                result.Start = start;
                result.End = Developer.LanguageProvider.CodeTokenizer.GetEndPosition(tokens, currentToken);
            }
            return result;
        }

        public static Developer.LanguageServices.NativeX.SyntaxTree.NativeXIfStatement ParseIfStatement(List<CodeToken> tokens, ref int currentToken, ref bool parseSuccess)
        {
            Developer.LanguageProvider.TextPosition start = Developer.LanguageProvider.CodeTokenizer.GetStartPosition(tokens, currentToken);
            Developer.LanguageServices.NativeX.SyntaxTree.NativeXIfStatement result = default(Developer.LanguageServices.NativeX.SyntaxTree.NativeXIfStatement);
            Developer.LanguageServices.NativeX.SyntaxTree.NativeXExpression ConditionMember0 = default(Developer.LanguageServices.NativeX.SyntaxTree.NativeXExpression);
            Developer.LanguageServices.NativeX.SyntaxTree.NativeXStatement TrueStatementMember0 = default(Developer.LanguageServices.NativeX.SyntaxTree.NativeXStatement);
            Developer.LanguageServices.NativeX.SyntaxTree.NativeXStatement FalseStatementMember0 = default(Developer.LanguageServices.NativeX.SyntaxTree.NativeXStatement);
            {
                int currentIndexCopy1 = currentToken;
                int currentIndex1 = currentToken;
                {
                    Developer.LanguageProvider.CodeTokenizer.ParseToken(tokens, ref currentIndex1, ref parseSuccess, "if");
                }
                if (parseSuccess)
                {
                    currentIndexCopy1 = currentIndex1;
                }
                else
                {
                    goto LABEL_0;
                }
                {
                    Developer.LanguageProvider.CodeTokenizer.ParseToken(tokens, ref currentIndex1, ref parseSuccess, "(");
                }
                if (parseSuccess)
                {
                    currentIndexCopy1 = currentIndex1;
                }
                else
                {
                    goto LABEL_0;
                }
                {
                    Developer.LanguageServices.NativeX.SyntaxTree.NativeXExpression ConditionMember1 = default(Developer.LanguageServices.NativeX.SyntaxTree.NativeXExpression);
                    {
                        Developer.LanguageServices.NativeX.SyntaxTree.NativeXExpression result2 = default(Developer.LanguageServices.NativeX.SyntaxTree.NativeXExpression);
                        int currentIndex2 = currentIndex1;
                        {
                            parseSuccess = false;
                            result2 = ParseExpression(tokens, ref currentIndex2, ref parseSuccess);
                        }
                        if (parseSuccess)
                        {
                            currentIndex1 = currentIndex2;
                            ConditionMember1 = result2;
                        }
                    }
                    ConditionMember0 = ConditionMember1;
                }
                if (parseSuccess)
                {
                    currentIndexCopy1 = currentIndex1;
                }
                else
                {
                    goto LABEL_0;
                }
                {
                    Developer.LanguageProvider.CodeTokenizer.ParseToken(tokens, ref currentIndex1, ref parseSuccess, ")");
                }
                if (parseSuccess)
                {
                    currentIndexCopy1 = currentIndex1;
                }
                else
                {
                    goto LABEL_0;
                }
                {
                    Developer.LanguageServices.NativeX.SyntaxTree.NativeXStatement TrueStatementMember1 = default(Developer.LanguageServices.NativeX.SyntaxTree.NativeXStatement);
                    {
                        Developer.LanguageServices.NativeX.SyntaxTree.NativeXStatement result2 = default(Developer.LanguageServices.NativeX.SyntaxTree.NativeXStatement);
                        int currentIndex2 = currentIndex1;
                        {
                            parseSuccess = false;
                            result2 = ParseStatement(tokens, ref currentIndex2, ref parseSuccess);
                        }
                        if (parseSuccess)
                        {
                            currentIndex1 = currentIndex2;
                            TrueStatementMember1 = result2;
                        }
                    }
                    TrueStatementMember0 = TrueStatementMember1;
                }
                if (parseSuccess)
                {
                    currentIndexCopy1 = currentIndex1;
                }
                else
                {
                    goto LABEL_0;
                }
                {
                    Developer.LanguageServices.NativeX.SyntaxTree.NativeXStatement FalseStatementMember1 = default(Developer.LanguageServices.NativeX.SyntaxTree.NativeXStatement);
                    {
                        int currentIndex2 = -1;
                        currentIndex2 = currentIndex1;
                        {
                            Developer.LanguageServices.NativeX.SyntaxTree.NativeXStatement FalseStatementMember2 = default(Developer.LanguageServices.NativeX.SyntaxTree.NativeXStatement);
                            {
                                int currentIndexCopy3 = currentIndex2;
                                int currentIndex3 = currentIndex2;
                                {
                                    Developer.LanguageProvider.CodeTokenizer.ParseToken(tokens, ref currentIndex3, ref parseSuccess, "else");
                                }
                                if (parseSuccess)
                                {
                                    currentIndexCopy3 = currentIndex3;
                                }
                                else
                                {
                                    goto LABEL_1;
                                }
                                {
                                    Developer.LanguageServices.NativeX.SyntaxTree.NativeXStatement FalseStatementMember3 = default(Developer.LanguageServices.NativeX.SyntaxTree.NativeXStatement);
                                    {
                                        Developer.LanguageServices.NativeX.SyntaxTree.NativeXStatement result4 = default(Developer.LanguageServices.NativeX.SyntaxTree.NativeXStatement);
                                        int currentIndex4 = currentIndex3;
                                        {
                                            parseSuccess = false;
                                            result4 = ParseStatement(tokens, ref currentIndex4, ref parseSuccess);
                                        }
                                        if (parseSuccess)
                                        {
                                            currentIndex3 = currentIndex4;
                                            FalseStatementMember3 = result4;
                                        }
                                    }
                                    FalseStatementMember2 = FalseStatementMember3;
                                }
                                if (parseSuccess)
                                {
                                    currentIndexCopy3 = currentIndex3;
                                }
                                else
                                {
                                    goto LABEL_1;
                                }
                                currentIndex2 = currentIndexCopy3;
                            LABEL_1: ;
                            }
                            FalseStatementMember1 = FalseStatementMember2;
                        }
                        if (parseSuccess)
                        {
                            currentIndex1 = currentIndex2;
                        }
                        parseSuccess = true;
                    }
                    FalseStatementMember0 = FalseStatementMember1;
                }
                if (parseSuccess)
                {
                    currentIndexCopy1 = currentIndex1;
                }
                else
                {
                    goto LABEL_0;
                }
                currentToken = currentIndexCopy1;
            LABEL_0: ;
            }
            if (parseSuccess)
            {
                if (result == null) result = CodeNode.Create<Developer.LanguageServices.NativeX.SyntaxTree.NativeXIfStatement>();
                result.Condition = ConditionMember0;
                result.TrueStatement = TrueStatementMember0;
                result.FalseStatement = FalseStatementMember0;
                result.Start = start;
                result.End = Developer.LanguageProvider.CodeTokenizer.GetEndPosition(tokens, currentToken);
            }
            return result;
        }

        public static Developer.LanguageServices.NativeX.SyntaxTree.NativeXBreakStatement ParseBreakStatement(List<CodeToken> tokens, ref int currentToken, ref bool parseSuccess)
        {
            Developer.LanguageProvider.TextPosition start = Developer.LanguageProvider.CodeTokenizer.GetStartPosition(tokens, currentToken);
            Developer.LanguageServices.NativeX.SyntaxTree.NativeXBreakStatement result = default(Developer.LanguageServices.NativeX.SyntaxTree.NativeXBreakStatement);
            {
                int currentIndexCopy1 = currentToken;
                int currentIndex1 = currentToken;
                {
                    Developer.LanguageProvider.CodeTokenizer.ParseToken(tokens, ref currentIndex1, ref parseSuccess, "break");
                }
                if (parseSuccess)
                {
                    currentIndexCopy1 = currentIndex1;
                }
                else
                {
                    goto LABEL_0;
                }
                {
                    Developer.LanguageProvider.CodeTokenizer.ParseToken(tokens, ref currentIndex1, ref parseSuccess, ";");
                }
                if (parseSuccess)
                {
                    currentIndexCopy1 = currentIndex1;
                }
                else
                {
                    goto LABEL_0;
                }
                currentToken = currentIndexCopy1;
            LABEL_0: ;
            }
            if (parseSuccess)
            {
                if (result == null) result = CodeNode.Create<Developer.LanguageServices.NativeX.SyntaxTree.NativeXBreakStatement>();
                result.Start = start;
                result.End = Developer.LanguageProvider.CodeTokenizer.GetEndPosition(tokens, currentToken);
            }
            return result;
        }

        public static Developer.LanguageServices.NativeX.SyntaxTree.NativeXContinueStatement ParseContinueStatement(List<CodeToken> tokens, ref int currentToken, ref bool parseSuccess)
        {
            Developer.LanguageProvider.TextPosition start = Developer.LanguageProvider.CodeTokenizer.GetStartPosition(tokens, currentToken);
            Developer.LanguageServices.NativeX.SyntaxTree.NativeXContinueStatement result = default(Developer.LanguageServices.NativeX.SyntaxTree.NativeXContinueStatement);
            {
                int currentIndexCopy1 = currentToken;
                int currentIndex1 = currentToken;
                {
                    Developer.LanguageProvider.CodeTokenizer.ParseToken(tokens, ref currentIndex1, ref parseSuccess, "continue");
                }
                if (parseSuccess)
                {
                    currentIndexCopy1 = currentIndex1;
                }
                else
                {
                    goto LABEL_0;
                }
                {
                    Developer.LanguageProvider.CodeTokenizer.ParseToken(tokens, ref currentIndex1, ref parseSuccess, ";");
                }
                if (parseSuccess)
                {
                    currentIndexCopy1 = currentIndex1;
                }
                else
                {
                    goto LABEL_0;
                }
                currentToken = currentIndexCopy1;
            LABEL_0: ;
            }
            if (parseSuccess)
            {
                if (result == null) result = CodeNode.Create<Developer.LanguageServices.NativeX.SyntaxTree.NativeXContinueStatement>();
                result.Start = start;
                result.End = Developer.LanguageProvider.CodeTokenizer.GetEndPosition(tokens, currentToken);
            }
            return result;
        }

        public static Developer.LanguageServices.NativeX.SyntaxTree.NativeXReturnStatement ParseExitStatement(List<CodeToken> tokens, ref int currentToken, ref bool parseSuccess)
        {
            Developer.LanguageProvider.TextPosition start = Developer.LanguageProvider.CodeTokenizer.GetStartPosition(tokens, currentToken);
            Developer.LanguageServices.NativeX.SyntaxTree.NativeXReturnStatement result = default(Developer.LanguageServices.NativeX.SyntaxTree.NativeXReturnStatement);
            {
                int currentIndexCopy1 = currentToken;
                int currentIndex1 = currentToken;
                {
                    Developer.LanguageProvider.CodeTokenizer.ParseToken(tokens, ref currentIndex1, ref parseSuccess, "exit");
                }
                if (parseSuccess)
                {
                    currentIndexCopy1 = currentIndex1;
                }
                else
                {
                    goto LABEL_0;
                }
                {
                    Developer.LanguageProvider.CodeTokenizer.ParseToken(tokens, ref currentIndex1, ref parseSuccess, ";");
                }
                if (parseSuccess)
                {
                    currentIndexCopy1 = currentIndex1;
                }
                else
                {
                    goto LABEL_0;
                }
                currentToken = currentIndexCopy1;
            LABEL_0: ;
            }
            if (parseSuccess)
            {
                if (result == null) result = CodeNode.Create<Developer.LanguageServices.NativeX.SyntaxTree.NativeXReturnStatement>();
                result.Start = start;
                result.End = Developer.LanguageProvider.CodeTokenizer.GetEndPosition(tokens, currentToken);
            }
            return result;
        }

        public static Developer.LanguageServices.NativeX.SyntaxTree.NativeXCompositeStatement ParseCompositeStatement(List<CodeToken> tokens, ref int currentToken, ref bool parseSuccess)
        {
            Developer.LanguageProvider.TextPosition start = Developer.LanguageProvider.CodeTokenizer.GetStartPosition(tokens, currentToken);
            Developer.LanguageServices.NativeX.SyntaxTree.NativeXCompositeStatement result = default(Developer.LanguageServices.NativeX.SyntaxTree.NativeXCompositeStatement);
            Developer.LanguageProvider.CodeNodeList<Developer.LanguageServices.NativeX.SyntaxTree.NativeXStatement> StatementsMember0 = default(Developer.LanguageProvider.CodeNodeList<Developer.LanguageServices.NativeX.SyntaxTree.NativeXStatement>);
            {
                int currentIndexCopy1 = currentToken;
                int currentIndex1 = currentToken;
                {
                    Developer.LanguageProvider.CodeTokenizer.ParseToken(tokens, ref currentIndex1, ref parseSuccess, "{");
                }
                if (parseSuccess)
                {
                    currentIndexCopy1 = currentIndex1;
                }
                else
                {
                    goto LABEL_0;
                }
                {
                    Developer.LanguageProvider.CodeNodeList<Developer.LanguageServices.NativeX.SyntaxTree.NativeXStatement> StatementsMember1 = default(Developer.LanguageProvider.CodeNodeList<Developer.LanguageServices.NativeX.SyntaxTree.NativeXStatement>);
                    {
                        Developer.LanguageProvider.CodeNodeList<Developer.LanguageServices.NativeX.SyntaxTree.NativeXStatement> result2 = default(Developer.LanguageProvider.CodeNodeList<Developer.LanguageServices.NativeX.SyntaxTree.NativeXStatement>);
                        int currentIndex2 = currentIndex1;
                        {
                            result2 = new Developer.LanguageProvider.CodeNodeList<Developer.LanguageServices.NativeX.SyntaxTree.NativeXStatement>();
                            {
                                Developer.LanguageServices.NativeX.SyntaxTree.NativeXStatement result3 = default(Developer.LanguageServices.NativeX.SyntaxTree.NativeXStatement);
                                int currentIndex3 = currentIndex2;
                                {
                                    parseSuccess = false;
                                    result3 = ParseStatement(tokens, ref currentIndex3, ref parseSuccess);
                                }
                                if (parseSuccess)
                                {
                                    currentIndex2 = currentIndex3;
                                    result2.Add(result3);
                                    result2.Start = result3.Start;
                                    result2.End = result3.End;
                                }
                                while (true)
                                {
                                    int currentIndexCopy3 = currentIndex2;
                                    {
                                        parseSuccess = false;
                                        result3 = ParseStatement(tokens, ref currentIndex3, ref parseSuccess);
                                    }
                                    if (parseSuccess)
                                    {
                                        currentIndexCopy3 = currentIndex3;
                                    }
                                    else
                                    {
                                        goto LABEL_1;
                                    }
                                    currentIndex2 = currentIndexCopy3;
                                    result2.Add(result3);
                                    result2.End = result3.End;
                                }
                            LABEL_1: ; parseSuccess = true;
                            }
                        }
                        if (parseSuccess)
                        {
                            currentIndex1 = currentIndex2;
                            StatementsMember1 = result2;
                        }
                    }
                    StatementsMember0 = StatementsMember1;
                }
                if (parseSuccess)
                {
                    currentIndexCopy1 = currentIndex1;
                }
                else
                {
                    goto LABEL_0;
                }
                {
                    Developer.LanguageProvider.CodeTokenizer.ParseToken(tokens, ref currentIndex1, ref parseSuccess, "}");
                }
                if (parseSuccess)
                {
                    currentIndexCopy1 = currentIndex1;
                }
                else
                {
                    goto LABEL_0;
                }
                currentToken = currentIndexCopy1;
            LABEL_0: ;
            }
            if (parseSuccess)
            {
                if (result == null) result = CodeNode.Create<Developer.LanguageServices.NativeX.SyntaxTree.NativeXCompositeStatement>();
                result.Statements = StatementsMember0;
                result.Start = start;
                result.End = Developer.LanguageProvider.CodeTokenizer.GetEndPosition(tokens, currentToken);
            }
            return result;
        }

        public static Developer.LanguageServices.NativeX.SyntaxTree.NativeXWhileStatement ParseDoWhileStatement(List<CodeToken> tokens, ref int currentToken, ref bool parseSuccess)
        {
            Developer.LanguageProvider.TextPosition start = Developer.LanguageProvider.CodeTokenizer.GetStartPosition(tokens, currentToken);
            Developer.LanguageServices.NativeX.SyntaxTree.NativeXWhileStatement result = default(Developer.LanguageServices.NativeX.SyntaxTree.NativeXWhileStatement);
            Developer.LanguageServices.NativeX.SyntaxTree.NativeXStatement StatementMember0 = default(Developer.LanguageServices.NativeX.SyntaxTree.NativeXStatement);
            Developer.LanguageServices.NativeX.SyntaxTree.NativeXExpression EndConditionMember0 = default(Developer.LanguageServices.NativeX.SyntaxTree.NativeXExpression);
            {
                int currentIndexCopy1 = currentToken;
                int currentIndex1 = currentToken;
                {
                    Developer.LanguageProvider.CodeTokenizer.ParseToken(tokens, ref currentIndex1, ref parseSuccess, "do");
                }
                if (parseSuccess)
                {
                    currentIndexCopy1 = currentIndex1;
                }
                else
                {
                    goto LABEL_0;
                }
                {
                    Developer.LanguageServices.NativeX.SyntaxTree.NativeXStatement StatementMember1 = default(Developer.LanguageServices.NativeX.SyntaxTree.NativeXStatement);
                    {
                        Developer.LanguageServices.NativeX.SyntaxTree.NativeXStatement result2 = default(Developer.LanguageServices.NativeX.SyntaxTree.NativeXStatement);
                        int currentIndex2 = currentIndex1;
                        {
                            parseSuccess = false;
                            result2 = ParseStatement(tokens, ref currentIndex2, ref parseSuccess);
                        }
                        if (parseSuccess)
                        {
                            currentIndex1 = currentIndex2;
                            StatementMember1 = result2;
                        }
                    }
                    StatementMember0 = StatementMember1;
                }
                if (parseSuccess)
                {
                    currentIndexCopy1 = currentIndex1;
                }
                else
                {
                    goto LABEL_0;
                }
                {
                    Developer.LanguageProvider.CodeTokenizer.ParseToken(tokens, ref currentIndex1, ref parseSuccess, "while");
                }
                if (parseSuccess)
                {
                    currentIndexCopy1 = currentIndex1;
                }
                else
                {
                    goto LABEL_0;
                }
                {
                    Developer.LanguageProvider.CodeTokenizer.ParseToken(tokens, ref currentIndex1, ref parseSuccess, "(");
                }
                if (parseSuccess)
                {
                    currentIndexCopy1 = currentIndex1;
                }
                else
                {
                    goto LABEL_0;
                }
                {
                    Developer.LanguageServices.NativeX.SyntaxTree.NativeXExpression EndConditionMember1 = default(Developer.LanguageServices.NativeX.SyntaxTree.NativeXExpression);
                    {
                        Developer.LanguageServices.NativeX.SyntaxTree.NativeXExpression result2 = default(Developer.LanguageServices.NativeX.SyntaxTree.NativeXExpression);
                        int currentIndex2 = currentIndex1;
                        {
                            parseSuccess = false;
                            result2 = ParseExpression(tokens, ref currentIndex2, ref parseSuccess);
                        }
                        if (parseSuccess)
                        {
                            currentIndex1 = currentIndex2;
                            EndConditionMember1 = result2;
                        }
                    }
                    EndConditionMember0 = EndConditionMember1;
                }
                if (parseSuccess)
                {
                    currentIndexCopy1 = currentIndex1;
                }
                else
                {
                    goto LABEL_0;
                }
                {
                    Developer.LanguageProvider.CodeTokenizer.ParseToken(tokens, ref currentIndex1, ref parseSuccess, ")");
                }
                if (parseSuccess)
                {
                    currentIndexCopy1 = currentIndex1;
                }
                else
                {
                    goto LABEL_0;
                }
                {
                    Developer.LanguageProvider.CodeTokenizer.ParseToken(tokens, ref currentIndex1, ref parseSuccess, ";");
                }
                if (parseSuccess)
                {
                    currentIndexCopy1 = currentIndex1;
                }
                else
                {
                    goto LABEL_0;
                }
                currentToken = currentIndexCopy1;
            LABEL_0: ;
            }
            if (parseSuccess)
            {
                if (result == null) result = CodeNode.Create<Developer.LanguageServices.NativeX.SyntaxTree.NativeXWhileStatement>();
                result.Statement = StatementMember0;
                result.EndCondition = EndConditionMember0;
                result.Start = start;
                result.End = Developer.LanguageProvider.CodeTokenizer.GetEndPosition(tokens, currentToken);
            }
            return result;
        }

        public static Developer.LanguageServices.NativeX.SyntaxTree.NativeXWhileStatement ParseLoopStatement(List<CodeToken> tokens, ref int currentToken, ref bool parseSuccess)
        {
            Developer.LanguageProvider.TextPosition start = Developer.LanguageProvider.CodeTokenizer.GetStartPosition(tokens, currentToken);
            Developer.LanguageServices.NativeX.SyntaxTree.NativeXWhileStatement result = default(Developer.LanguageServices.NativeX.SyntaxTree.NativeXWhileStatement);
            Developer.LanguageServices.NativeX.SyntaxTree.NativeXStatement StatementMember0 = default(Developer.LanguageServices.NativeX.SyntaxTree.NativeXStatement);
            {
                int currentIndexCopy1 = currentToken;
                int currentIndex1 = currentToken;
                {
                    Developer.LanguageProvider.CodeTokenizer.ParseToken(tokens, ref currentIndex1, ref parseSuccess, "loop");
                }
                if (parseSuccess)
                {
                    currentIndexCopy1 = currentIndex1;
                }
                else
                {
                    goto LABEL_0;
                }
                {
                    Developer.LanguageServices.NativeX.SyntaxTree.NativeXStatement StatementMember1 = default(Developer.LanguageServices.NativeX.SyntaxTree.NativeXStatement);
                    {
                        Developer.LanguageServices.NativeX.SyntaxTree.NativeXStatement result2 = default(Developer.LanguageServices.NativeX.SyntaxTree.NativeXStatement);
                        int currentIndex2 = currentIndex1;
                        {
                            parseSuccess = false;
                            result2 = ParseStatement(tokens, ref currentIndex2, ref parseSuccess);
                        }
                        if (parseSuccess)
                        {
                            currentIndex1 = currentIndex2;
                            StatementMember1 = result2;
                        }
                    }
                    StatementMember0 = StatementMember1;
                }
                if (parseSuccess)
                {
                    currentIndexCopy1 = currentIndex1;
                }
                else
                {
                    goto LABEL_0;
                }
                currentToken = currentIndexCopy1;
            LABEL_0: ;
            }
            if (parseSuccess)
            {
                if (result == null) result = CodeNode.Create<Developer.LanguageServices.NativeX.SyntaxTree.NativeXWhileStatement>();
                result.Statement = StatementMember0;
                result.Start = start;
                result.End = Developer.LanguageProvider.CodeTokenizer.GetEndPosition(tokens, currentToken);
            }
            return result;
        }

        public static Developer.LanguageServices.NativeX.SyntaxTree.NativeXWhileStatement ParseWhileDoStatement(List<CodeToken> tokens, ref int currentToken, ref bool parseSuccess)
        {
            Developer.LanguageProvider.TextPosition start = Developer.LanguageProvider.CodeTokenizer.GetStartPosition(tokens, currentToken);
            Developer.LanguageServices.NativeX.SyntaxTree.NativeXWhileStatement result = default(Developer.LanguageServices.NativeX.SyntaxTree.NativeXWhileStatement);
            Developer.LanguageServices.NativeX.SyntaxTree.NativeXExpression BeginConditionMember0 = default(Developer.LanguageServices.NativeX.SyntaxTree.NativeXExpression);
            Developer.LanguageServices.NativeX.SyntaxTree.NativeXStatement StatementMember0 = default(Developer.LanguageServices.NativeX.SyntaxTree.NativeXStatement);
            Developer.LanguageServices.NativeX.SyntaxTree.NativeXExpression EndConditionMember0 = default(Developer.LanguageServices.NativeX.SyntaxTree.NativeXExpression);
            {
                int currentIndexCopy1 = currentToken;
                int currentIndex1 = currentToken;
                {
                    Developer.LanguageProvider.CodeTokenizer.ParseToken(tokens, ref currentIndex1, ref parseSuccess, "while");
                }
                if (parseSuccess)
                {
                    currentIndexCopy1 = currentIndex1;
                }
                else
                {
                    goto LABEL_0;
                }
                {
                    Developer.LanguageProvider.CodeTokenizer.ParseToken(tokens, ref currentIndex1, ref parseSuccess, "(");
                }
                if (parseSuccess)
                {
                    currentIndexCopy1 = currentIndex1;
                }
                else
                {
                    goto LABEL_0;
                }
                {
                    Developer.LanguageServices.NativeX.SyntaxTree.NativeXExpression BeginConditionMember1 = default(Developer.LanguageServices.NativeX.SyntaxTree.NativeXExpression);
                    {
                        Developer.LanguageServices.NativeX.SyntaxTree.NativeXExpression result2 = default(Developer.LanguageServices.NativeX.SyntaxTree.NativeXExpression);
                        int currentIndex2 = currentIndex1;
                        {
                            parseSuccess = false;
                            result2 = ParseExpression(tokens, ref currentIndex2, ref parseSuccess);
                        }
                        if (parseSuccess)
                        {
                            currentIndex1 = currentIndex2;
                            BeginConditionMember1 = result2;
                        }
                    }
                    BeginConditionMember0 = BeginConditionMember1;
                }
                if (parseSuccess)
                {
                    currentIndexCopy1 = currentIndex1;
                }
                else
                {
                    goto LABEL_0;
                }
                {
                    Developer.LanguageProvider.CodeTokenizer.ParseToken(tokens, ref currentIndex1, ref parseSuccess, ")");
                }
                if (parseSuccess)
                {
                    currentIndexCopy1 = currentIndex1;
                }
                else
                {
                    goto LABEL_0;
                }
                {
                    Developer.LanguageServices.NativeX.SyntaxTree.NativeXStatement StatementMember1 = default(Developer.LanguageServices.NativeX.SyntaxTree.NativeXStatement);
                    {
                        Developer.LanguageServices.NativeX.SyntaxTree.NativeXStatement result2 = default(Developer.LanguageServices.NativeX.SyntaxTree.NativeXStatement);
                        int currentIndex2 = currentIndex1;
                        {
                            parseSuccess = false;
                            result2 = ParseStatement(tokens, ref currentIndex2, ref parseSuccess);
                        }
                        if (parseSuccess)
                        {
                            currentIndex1 = currentIndex2;
                            StatementMember1 = result2;
                        }
                    }
                    StatementMember0 = StatementMember1;
                }
                if (parseSuccess)
                {
                    currentIndexCopy1 = currentIndex1;
                }
                else
                {
                    goto LABEL_0;
                }
                {
                    Developer.LanguageServices.NativeX.SyntaxTree.NativeXExpression EndConditionMember1 = default(Developer.LanguageServices.NativeX.SyntaxTree.NativeXExpression);
                    {
                        int currentIndex2 = -1;
                        currentIndex2 = currentIndex1;
                        {
                            Developer.LanguageServices.NativeX.SyntaxTree.NativeXExpression EndConditionMember2 = default(Developer.LanguageServices.NativeX.SyntaxTree.NativeXExpression);
                            {
                                int currentIndexCopy3 = currentIndex2;
                                int currentIndex3 = currentIndex2;
                                {
                                    Developer.LanguageProvider.CodeTokenizer.ParseToken(tokens, ref currentIndex3, ref parseSuccess, "when");
                                }
                                if (parseSuccess)
                                {
                                    currentIndexCopy3 = currentIndex3;
                                }
                                else
                                {
                                    goto LABEL_1;
                                }
                                {
                                    Developer.LanguageProvider.CodeTokenizer.ParseToken(tokens, ref currentIndex3, ref parseSuccess, "(");
                                }
                                if (parseSuccess)
                                {
                                    currentIndexCopy3 = currentIndex3;
                                }
                                else
                                {
                                    goto LABEL_1;
                                }
                                {
                                    Developer.LanguageServices.NativeX.SyntaxTree.NativeXExpression EndConditionMember3 = default(Developer.LanguageServices.NativeX.SyntaxTree.NativeXExpression);
                                    {
                                        Developer.LanguageServices.NativeX.SyntaxTree.NativeXExpression result4 = default(Developer.LanguageServices.NativeX.SyntaxTree.NativeXExpression);
                                        int currentIndex4 = currentIndex3;
                                        {
                                            parseSuccess = false;
                                            result4 = ParseExpression(tokens, ref currentIndex4, ref parseSuccess);
                                        }
                                        if (parseSuccess)
                                        {
                                            currentIndex3 = currentIndex4;
                                            EndConditionMember3 = result4;
                                        }
                                    }
                                    EndConditionMember2 = EndConditionMember3;
                                }
                                if (parseSuccess)
                                {
                                    currentIndexCopy3 = currentIndex3;
                                }
                                else
                                {
                                    goto LABEL_1;
                                }
                                {
                                    Developer.LanguageProvider.CodeTokenizer.ParseToken(tokens, ref currentIndex3, ref parseSuccess, ")");
                                }
                                if (parseSuccess)
                                {
                                    currentIndexCopy3 = currentIndex3;
                                }
                                else
                                {
                                    goto LABEL_1;
                                }
                                {
                                    Developer.LanguageProvider.CodeTokenizer.ParseToken(tokens, ref currentIndex3, ref parseSuccess, ";");
                                }
                                if (parseSuccess)
                                {
                                    currentIndexCopy3 = currentIndex3;
                                }
                                else
                                {
                                    goto LABEL_1;
                                }
                                currentIndex2 = currentIndexCopy3;
                            LABEL_1: ;
                            }
                            EndConditionMember1 = EndConditionMember2;
                        }
                        if (parseSuccess)
                        {
                            currentIndex1 = currentIndex2;
                        }
                        parseSuccess = true;
                    }
                    EndConditionMember0 = EndConditionMember1;
                }
                if (parseSuccess)
                {
                    currentIndexCopy1 = currentIndex1;
                }
                else
                {
                    goto LABEL_0;
                }
                currentToken = currentIndexCopy1;
            LABEL_0: ;
            }
            if (parseSuccess)
            {
                if (result == null) result = CodeNode.Create<Developer.LanguageServices.NativeX.SyntaxTree.NativeXWhileStatement>();
                result.BeginCondition = BeginConditionMember0;
                result.Statement = StatementMember0;
                result.EndCondition = EndConditionMember0;
                result.Start = start;
                result.End = Developer.LanguageProvider.CodeTokenizer.GetEndPosition(tokens, currentToken);
            }
            return result;
        }

        public static Developer.LanguageServices.NativeX.SyntaxTree.NativeXForStatement ParseForStatement(List<CodeToken> tokens, ref int currentToken, ref bool parseSuccess)
        {
            Developer.LanguageProvider.TextPosition start = Developer.LanguageProvider.CodeTokenizer.GetStartPosition(tokens, currentToken);
            Developer.LanguageServices.NativeX.SyntaxTree.NativeXForStatement result = default(Developer.LanguageServices.NativeX.SyntaxTree.NativeXForStatement);
            Developer.LanguageProvider.CodeNodeList<Developer.LanguageServices.NativeX.SyntaxTree.NativeXStatement> InitializerMember0 = default(Developer.LanguageProvider.CodeNodeList<Developer.LanguageServices.NativeX.SyntaxTree.NativeXStatement>);
            Developer.LanguageServices.NativeX.SyntaxTree.NativeXExpression ConditionMember0 = default(Developer.LanguageServices.NativeX.SyntaxTree.NativeXExpression);
            Developer.LanguageProvider.CodeNodeList<Developer.LanguageServices.NativeX.SyntaxTree.NativeXStatement> SideEffectMember0 = default(Developer.LanguageProvider.CodeNodeList<Developer.LanguageServices.NativeX.SyntaxTree.NativeXStatement>);
            Developer.LanguageServices.NativeX.SyntaxTree.NativeXStatement StatementMember0 = default(Developer.LanguageServices.NativeX.SyntaxTree.NativeXStatement);
            {
                int currentIndexCopy1 = currentToken;
                int currentIndex1 = currentToken;
                {
                    Developer.LanguageProvider.CodeTokenizer.ParseToken(tokens, ref currentIndex1, ref parseSuccess, "for");
                }
                if (parseSuccess)
                {
                    currentIndexCopy1 = currentIndex1;
                }
                else
                {
                    goto LABEL_0;
                }
                {
                    Developer.LanguageProvider.CodeNodeList<Developer.LanguageServices.NativeX.SyntaxTree.NativeXStatement> InitializerMember1 = default(Developer.LanguageProvider.CodeNodeList<Developer.LanguageServices.NativeX.SyntaxTree.NativeXStatement>);
                    {
                        Developer.LanguageProvider.CodeNodeList<Developer.LanguageServices.NativeX.SyntaxTree.NativeXStatement> result2 = default(Developer.LanguageProvider.CodeNodeList<Developer.LanguageServices.NativeX.SyntaxTree.NativeXStatement>);
                        int currentIndex2 = currentIndex1;
                        {
                            result2 = new Developer.LanguageProvider.CodeNodeList<Developer.LanguageServices.NativeX.SyntaxTree.NativeXStatement>();
                            {
                                Developer.LanguageServices.NativeX.SyntaxTree.NativeXStatement result3 = default(Developer.LanguageServices.NativeX.SyntaxTree.NativeXStatement);
                                int currentIndex3 = currentIndex2;
                                {
                                    parseSuccess = false;
                                    result3 = ParseStatement(tokens, ref currentIndex3, ref parseSuccess);
                                }
                                if (parseSuccess)
                                {
                                    currentIndex2 = currentIndex3;
                                    result2.Add(result3);
                                    result2.Start = result3.Start;
                                    result2.End = result3.End;
                                }
                                while (true)
                                {
                                    int currentIndexCopy3 = currentIndex2;
                                    {
                                        parseSuccess = false;
                                        result3 = ParseStatement(tokens, ref currentIndex3, ref parseSuccess);
                                    }
                                    if (parseSuccess)
                                    {
                                        currentIndexCopy3 = currentIndex3;
                                    }
                                    else
                                    {
                                        goto LABEL_1;
                                    }
                                    currentIndex2 = currentIndexCopy3;
                                    result2.Add(result3);
                                    result2.End = result3.End;
                                }
                            LABEL_1: ; parseSuccess = true;
                            }
                        }
                        if (parseSuccess)
                        {
                            currentIndex1 = currentIndex2;
                            InitializerMember1 = result2;
                        }
                    }
                    InitializerMember0 = InitializerMember1;
                }
                if (parseSuccess)
                {
                    currentIndexCopy1 = currentIndex1;
                }
                else
                {
                    goto LABEL_0;
                }
                {
                    Developer.LanguageProvider.CodeTokenizer.ParseToken(tokens, ref currentIndex1, ref parseSuccess, "when");
                }
                if (parseSuccess)
                {
                    currentIndexCopy1 = currentIndex1;
                }
                else
                {
                    goto LABEL_0;
                }
                {
                    Developer.LanguageProvider.CodeTokenizer.ParseToken(tokens, ref currentIndex1, ref parseSuccess, "(");
                }
                if (parseSuccess)
                {
                    currentIndexCopy1 = currentIndex1;
                }
                else
                {
                    goto LABEL_0;
                }
                {
                    Developer.LanguageServices.NativeX.SyntaxTree.NativeXExpression ConditionMember1 = default(Developer.LanguageServices.NativeX.SyntaxTree.NativeXExpression);
                    {
                        Developer.LanguageServices.NativeX.SyntaxTree.NativeXExpression result2 = default(Developer.LanguageServices.NativeX.SyntaxTree.NativeXExpression);
                        int currentIndex2 = currentIndex1;
                        {
                            parseSuccess = false;
                            result2 = ParseExpression(tokens, ref currentIndex2, ref parseSuccess);
                        }
                        if (parseSuccess)
                        {
                            currentIndex1 = currentIndex2;
                            ConditionMember1 = result2;
                        }
                    }
                    ConditionMember0 = ConditionMember1;
                }
                if (parseSuccess)
                {
                    currentIndexCopy1 = currentIndex1;
                }
                else
                {
                    goto LABEL_0;
                }
                {
                    Developer.LanguageProvider.CodeTokenizer.ParseToken(tokens, ref currentIndex1, ref parseSuccess, ")");
                }
                if (parseSuccess)
                {
                    currentIndexCopy1 = currentIndex1;
                }
                else
                {
                    goto LABEL_0;
                }
                {
                    Developer.LanguageProvider.CodeTokenizer.ParseToken(tokens, ref currentIndex1, ref parseSuccess, "with");
                }
                if (parseSuccess)
                {
                    currentIndexCopy1 = currentIndex1;
                }
                else
                {
                    goto LABEL_0;
                }
                {
                    Developer.LanguageProvider.CodeNodeList<Developer.LanguageServices.NativeX.SyntaxTree.NativeXStatement> SideEffectMember1 = default(Developer.LanguageProvider.CodeNodeList<Developer.LanguageServices.NativeX.SyntaxTree.NativeXStatement>);
                    {
                        Developer.LanguageProvider.CodeNodeList<Developer.LanguageServices.NativeX.SyntaxTree.NativeXStatement> result2 = default(Developer.LanguageProvider.CodeNodeList<Developer.LanguageServices.NativeX.SyntaxTree.NativeXStatement>);
                        int currentIndex2 = currentIndex1;
                        {
                            result2 = new Developer.LanguageProvider.CodeNodeList<Developer.LanguageServices.NativeX.SyntaxTree.NativeXStatement>();
                            {
                                Developer.LanguageServices.NativeX.SyntaxTree.NativeXStatement result3 = default(Developer.LanguageServices.NativeX.SyntaxTree.NativeXStatement);
                                int currentIndex3 = currentIndex2;
                                {
                                    parseSuccess = false;
                                    result3 = ParseStatement(tokens, ref currentIndex3, ref parseSuccess);
                                }
                                if (parseSuccess)
                                {
                                    currentIndex2 = currentIndex3;
                                    result2.Add(result3);
                                    result2.Start = result3.Start;
                                    result2.End = result3.End;
                                }
                                while (true)
                                {
                                    int currentIndexCopy3 = currentIndex2;
                                    {
                                        parseSuccess = false;
                                        result3 = ParseStatement(tokens, ref currentIndex3, ref parseSuccess);
                                    }
                                    if (parseSuccess)
                                    {
                                        currentIndexCopy3 = currentIndex3;
                                    }
                                    else
                                    {
                                        goto LABEL_2;
                                    }
                                    currentIndex2 = currentIndexCopy3;
                                    result2.Add(result3);
                                    result2.End = result3.End;
                                }
                            LABEL_2: ; parseSuccess = true;
                            }
                        }
                        if (parseSuccess)
                        {
                            currentIndex1 = currentIndex2;
                            SideEffectMember1 = result2;
                        }
                    }
                    SideEffectMember0 = SideEffectMember1;
                }
                if (parseSuccess)
                {
                    currentIndexCopy1 = currentIndex1;
                }
                else
                {
                    goto LABEL_0;
                }
                {
                    Developer.LanguageProvider.CodeTokenizer.ParseToken(tokens, ref currentIndex1, ref parseSuccess, "do");
                }
                if (parseSuccess)
                {
                    currentIndexCopy1 = currentIndex1;
                }
                else
                {
                    goto LABEL_0;
                }
                {
                    Developer.LanguageServices.NativeX.SyntaxTree.NativeXStatement StatementMember1 = default(Developer.LanguageServices.NativeX.SyntaxTree.NativeXStatement);
                    {
                        Developer.LanguageServices.NativeX.SyntaxTree.NativeXStatement result2 = default(Developer.LanguageServices.NativeX.SyntaxTree.NativeXStatement);
                        int currentIndex2 = currentIndex1;
                        {
                            parseSuccess = false;
                            result2 = ParseStatement(tokens, ref currentIndex2, ref parseSuccess);
                        }
                        if (parseSuccess)
                        {
                            currentIndex1 = currentIndex2;
                            StatementMember1 = result2;
                        }
                    }
                    StatementMember0 = StatementMember1;
                }
                if (parseSuccess)
                {
                    currentIndexCopy1 = currentIndex1;
                }
                else
                {
                    goto LABEL_0;
                }
                currentToken = currentIndexCopy1;
            LABEL_0: ;
            }
            if (parseSuccess)
            {
                if (result == null) result = CodeNode.Create<Developer.LanguageServices.NativeX.SyntaxTree.NativeXForStatement>();
                result.Initializer = InitializerMember0;
                result.Condition = ConditionMember0;
                result.SideEffect = SideEffectMember0;
                result.Statement = StatementMember0;
                result.Start = start;
                result.End = Developer.LanguageProvider.CodeTokenizer.GetEndPosition(tokens, currentToken);
            }
            return result;
        }

        public static Developer.LanguageServices.NativeX.SyntaxTree.NativeXTryCatchStatement ParseTryCatchStatement(List<CodeToken> tokens, ref int currentToken, ref bool parseSuccess)
        {
            Developer.LanguageProvider.TextPosition start = Developer.LanguageProvider.CodeTokenizer.GetStartPosition(tokens, currentToken);
            Developer.LanguageServices.NativeX.SyntaxTree.NativeXTryCatchStatement result = default(Developer.LanguageServices.NativeX.SyntaxTree.NativeXTryCatchStatement);
            Developer.LanguageServices.NativeX.SyntaxTree.NativeXStatement TryStatementMember0 = default(Developer.LanguageServices.NativeX.SyntaxTree.NativeXStatement);
            Developer.LanguageServices.NativeX.SyntaxTree.NativeXStatement CatchStatementMember0 = default(Developer.LanguageServices.NativeX.SyntaxTree.NativeXStatement);
            {
                int currentIndexCopy1 = currentToken;
                int currentIndex1 = currentToken;
                {
                    Developer.LanguageProvider.CodeTokenizer.ParseToken(tokens, ref currentIndex1, ref parseSuccess, "try");
                }
                if (parseSuccess)
                {
                    currentIndexCopy1 = currentIndex1;
                }
                else
                {
                    goto LABEL_0;
                }
                {
                    Developer.LanguageServices.NativeX.SyntaxTree.NativeXStatement TryStatementMember1 = default(Developer.LanguageServices.NativeX.SyntaxTree.NativeXStatement);
                    {
                        Developer.LanguageServices.NativeX.SyntaxTree.NativeXStatement result2 = default(Developer.LanguageServices.NativeX.SyntaxTree.NativeXStatement);
                        int currentIndex2 = currentIndex1;
                        {
                            parseSuccess = false;
                            result2 = ParseStatement(tokens, ref currentIndex2, ref parseSuccess);
                        }
                        if (parseSuccess)
                        {
                            currentIndex1 = currentIndex2;
                            TryStatementMember1 = result2;
                        }
                    }
                    TryStatementMember0 = TryStatementMember1;
                }
                if (parseSuccess)
                {
                    currentIndexCopy1 = currentIndex1;
                }
                else
                {
                    goto LABEL_0;
                }
                {
                    Developer.LanguageProvider.CodeTokenizer.ParseToken(tokens, ref currentIndex1, ref parseSuccess, "catch");
                }
                if (parseSuccess)
                {
                    currentIndexCopy1 = currentIndex1;
                }
                else
                {
                    goto LABEL_0;
                }
                {
                    Developer.LanguageServices.NativeX.SyntaxTree.NativeXStatement CatchStatementMember1 = default(Developer.LanguageServices.NativeX.SyntaxTree.NativeXStatement);
                    {
                        Developer.LanguageServices.NativeX.SyntaxTree.NativeXStatement result2 = default(Developer.LanguageServices.NativeX.SyntaxTree.NativeXStatement);
                        int currentIndex2 = currentIndex1;
                        {
                            parseSuccess = false;
                            result2 = ParseStatement(tokens, ref currentIndex2, ref parseSuccess);
                        }
                        if (parseSuccess)
                        {
                            currentIndex1 = currentIndex2;
                            CatchStatementMember1 = result2;
                        }
                    }
                    CatchStatementMember0 = CatchStatementMember1;
                }
                if (parseSuccess)
                {
                    currentIndexCopy1 = currentIndex1;
                }
                else
                {
                    goto LABEL_0;
                }
                currentToken = currentIndexCopy1;
            LABEL_0: ;
            }
            if (parseSuccess)
            {
                if (result == null) result = CodeNode.Create<Developer.LanguageServices.NativeX.SyntaxTree.NativeXTryCatchStatement>();
                result.TryStatement = TryStatementMember0;
                result.CatchStatement = CatchStatementMember0;
                result.Start = start;
                result.End = Developer.LanguageProvider.CodeTokenizer.GetEndPosition(tokens, currentToken);
            }
            return result;
        }

        public static Developer.LanguageServices.NativeX.SyntaxTree.NativeXThrowStatement ParseThrowStatement(List<CodeToken> tokens, ref int currentToken, ref bool parseSuccess)
        {
            Developer.LanguageProvider.TextPosition start = Developer.LanguageProvider.CodeTokenizer.GetStartPosition(tokens, currentToken);
            Developer.LanguageServices.NativeX.SyntaxTree.NativeXThrowStatement result = default(Developer.LanguageServices.NativeX.SyntaxTree.NativeXThrowStatement);
            Developer.LanguageServices.NativeX.SyntaxTree.NativeXExpression ExceptionExpressionMember0 = default(Developer.LanguageServices.NativeX.SyntaxTree.NativeXExpression);
            {
                int currentIndexCopy1 = currentToken;
                int currentIndex1 = currentToken;
                {
                    Developer.LanguageProvider.CodeTokenizer.ParseToken(tokens, ref currentIndex1, ref parseSuccess, "throw");
                }
                if (parseSuccess)
                {
                    currentIndexCopy1 = currentIndex1;
                }
                else
                {
                    goto LABEL_0;
                }
                {
                    Developer.LanguageServices.NativeX.SyntaxTree.NativeXExpression ExceptionExpressionMember1 = default(Developer.LanguageServices.NativeX.SyntaxTree.NativeXExpression);
                    {
                        int currentIndex2 = -1;
                        currentIndex2 = currentIndex1;
                        {
                            Developer.LanguageServices.NativeX.SyntaxTree.NativeXExpression ExceptionExpressionMember2 = default(Developer.LanguageServices.NativeX.SyntaxTree.NativeXExpression);
                            {
                                Developer.LanguageServices.NativeX.SyntaxTree.NativeXExpression result3 = default(Developer.LanguageServices.NativeX.SyntaxTree.NativeXExpression);
                                int currentIndex3 = currentIndex2;
                                {
                                    parseSuccess = false;
                                    result3 = ParseExpression(tokens, ref currentIndex3, ref parseSuccess);
                                }
                                if (parseSuccess)
                                {
                                    currentIndex2 = currentIndex3;
                                    ExceptionExpressionMember2 = result3;
                                }
                            }
                            ExceptionExpressionMember1 = ExceptionExpressionMember2;
                        }
                        if (parseSuccess)
                        {
                            currentIndex1 = currentIndex2;
                        }
                        parseSuccess = true;
                    }
                    ExceptionExpressionMember0 = ExceptionExpressionMember1;
                }
                if (parseSuccess)
                {
                    currentIndexCopy1 = currentIndex1;
                }
                else
                {
                    goto LABEL_0;
                }
                {
                    Developer.LanguageProvider.CodeTokenizer.ParseToken(tokens, ref currentIndex1, ref parseSuccess, ";");
                }
                if (parseSuccess)
                {
                    currentIndexCopy1 = currentIndex1;
                }
                else
                {
                    goto LABEL_0;
                }
                currentToken = currentIndexCopy1;
            LABEL_0: ;
            }
            if (parseSuccess)
            {
                if (result == null) result = CodeNode.Create<Developer.LanguageServices.NativeX.SyntaxTree.NativeXThrowStatement>();
                result.ExceptionExpression = ExceptionExpressionMember0;
                result.Start = start;
                result.End = Developer.LanguageProvider.CodeTokenizer.GetEndPosition(tokens, currentToken);
            }
            return result;
        }

        public static Developer.LanguageServices.NativeX.SyntaxTree.NativeXExpressionStatement ParseExpressionStatement(List<CodeToken> tokens, ref int currentToken, ref bool parseSuccess)
        {
            Developer.LanguageProvider.TextPosition start = Developer.LanguageProvider.CodeTokenizer.GetStartPosition(tokens, currentToken);
            Developer.LanguageServices.NativeX.SyntaxTree.NativeXExpressionStatement result = default(Developer.LanguageServices.NativeX.SyntaxTree.NativeXExpressionStatement);
            Developer.LanguageServices.NativeX.SyntaxTree.NativeXExpression ExpressionMember0 = default(Developer.LanguageServices.NativeX.SyntaxTree.NativeXExpression);
            {
                int currentIndexCopy1 = currentToken;
                int currentIndex1 = currentToken;
                {
                    Developer.LanguageServices.NativeX.SyntaxTree.NativeXExpression ExpressionMember1 = default(Developer.LanguageServices.NativeX.SyntaxTree.NativeXExpression);
                    {
                        Developer.LanguageServices.NativeX.SyntaxTree.NativeXExpression result2 = default(Developer.LanguageServices.NativeX.SyntaxTree.NativeXExpression);
                        int currentIndex2 = currentIndex1;
                        {
                            parseSuccess = false;
                            result2 = ParseExpression(tokens, ref currentIndex2, ref parseSuccess);
                        }
                        if (parseSuccess)
                        {
                            currentIndex1 = currentIndex2;
                            ExpressionMember1 = result2;
                        }
                    }
                    ExpressionMember0 = ExpressionMember1;
                }
                if (parseSuccess)
                {
                    currentIndexCopy1 = currentIndex1;
                }
                else
                {
                    goto LABEL_0;
                }
                {
                    Developer.LanguageProvider.CodeTokenizer.ParseToken(tokens, ref currentIndex1, ref parseSuccess, ";");
                }
                if (parseSuccess)
                {
                    currentIndexCopy1 = currentIndex1;
                }
                else
                {
                    goto LABEL_0;
                }
                currentToken = currentIndexCopy1;
            LABEL_0: ;
            }
            if (parseSuccess)
            {
                if (result == null) result = CodeNode.Create<Developer.LanguageServices.NativeX.SyntaxTree.NativeXExpressionStatement>();
                result.Expression = ExpressionMember0;
                result.Start = start;
                result.End = Developer.LanguageProvider.CodeTokenizer.GetEndPosition(tokens, currentToken);
            }
            return result;
        }

        public static Developer.LanguageServices.NativeX.SyntaxTree.NativeXTypeRenameDeclaration ParseTypeRenameDeclaration(List<CodeToken> tokens, ref int currentToken, ref bool parseSuccess)
        {
            Developer.LanguageProvider.TextPosition start = Developer.LanguageProvider.CodeTokenizer.GetStartPosition(tokens, currentToken);
            Developer.LanguageServices.NativeX.SyntaxTree.NativeXTypeRenameDeclaration result = default(Developer.LanguageServices.NativeX.SyntaxTree.NativeXTypeRenameDeclaration);
            System.String NameMember0 = default(System.String);
            Developer.LanguageServices.NativeX.SyntaxTree.NativeXType TypeMember0 = default(Developer.LanguageServices.NativeX.SyntaxTree.NativeXType);
            {
                int currentIndexCopy1 = currentToken;
                int currentIndex1 = currentToken;
                {
                    Developer.LanguageProvider.CodeTokenizer.ParseToken(tokens, ref currentIndex1, ref parseSuccess, "type");
                }
                if (parseSuccess)
                {
                    currentIndexCopy1 = currentIndex1;
                }
                else
                {
                    goto LABEL_0;
                }
                {
                    System.String NameMember1 = default(System.String);
                    {
                        Developer.LanguageProvider.CodeToken result2 = default(Developer.LanguageProvider.CodeToken);
                        int currentIndex2 = currentIndex1;
                        {
                            result2 = Developer.LanguageProvider.CodeTokenizer.ParseToken(tokens, ref currentIndex2, ref parseSuccess, Developer.LanguageServices.NativeX.NativeXTokenizer.IdToken);
                        }
                        if (parseSuccess)
                        {
                            currentIndex1 = currentIndex2;
                            NameMember1 = result2.Value;
                        }
                    }
                    NameMember0 = NameMember1;
                }
                if (parseSuccess)
                {
                    currentIndexCopy1 = currentIndex1;
                }
                else
                {
                    goto LABEL_0;
                }
                {
                    Developer.LanguageProvider.CodeTokenizer.ParseToken(tokens, ref currentIndex1, ref parseSuccess, "=");
                }
                if (parseSuccess)
                {
                    currentIndexCopy1 = currentIndex1;
                }
                else
                {
                    goto LABEL_0;
                }
                {
                    Developer.LanguageServices.NativeX.SyntaxTree.NativeXType TypeMember1 = default(Developer.LanguageServices.NativeX.SyntaxTree.NativeXType);
                    {
                        Developer.LanguageServices.NativeX.SyntaxTree.NativeXType result2 = default(Developer.LanguageServices.NativeX.SyntaxTree.NativeXType);
                        int currentIndex2 = currentIndex1;
                        {
                            parseSuccess = false;
                            result2 = ParseType(tokens, ref currentIndex2, ref parseSuccess);
                        }
                        if (parseSuccess)
                        {
                            currentIndex1 = currentIndex2;
                            TypeMember1 = result2;
                        }
                    }
                    TypeMember0 = TypeMember1;
                }
                if (parseSuccess)
                {
                    currentIndexCopy1 = currentIndex1;
                }
                else
                {
                    goto LABEL_0;
                }
                {
                    Developer.LanguageProvider.CodeTokenizer.ParseToken(tokens, ref currentIndex1, ref parseSuccess, ";");
                }
                if (parseSuccess)
                {
                    currentIndexCopy1 = currentIndex1;
                }
                else
                {
                    goto LABEL_0;
                }
                currentToken = currentIndexCopy1;
            LABEL_0: ;
            }
            if (parseSuccess)
            {
                if (result == null) result = CodeNode.Create<Developer.LanguageServices.NativeX.SyntaxTree.NativeXTypeRenameDeclaration>();
                result.Name = NameMember0;
                result.Type = TypeMember0;
                result.Start = start;
                result.End = Developer.LanguageProvider.CodeTokenizer.GetEndPosition(tokens, currentToken);
            }
            return result;
        }

        public static Developer.LanguageServices.NativeX.SyntaxTree.NativeXVariableDeclaration ParseVariableDeclaration(List<CodeToken> tokens, ref int currentToken, ref bool parseSuccess)
        {
            Developer.LanguageProvider.TextPosition start = Developer.LanguageProvider.CodeTokenizer.GetStartPosition(tokens, currentToken);
            Developer.LanguageServices.NativeX.SyntaxTree.NativeXVariableDeclaration result = default(Developer.LanguageServices.NativeX.SyntaxTree.NativeXVariableDeclaration);
            Developer.LanguageServices.NativeX.SyntaxTree.NativeXType TypeMember0 = default(Developer.LanguageServices.NativeX.SyntaxTree.NativeXType);
            System.String NameMember0 = default(System.String);
            Developer.LanguageServices.NativeX.SyntaxTree.NativeXLinking LinkingMember0 = default(Developer.LanguageServices.NativeX.SyntaxTree.NativeXLinking);
            Developer.LanguageServices.NativeX.SyntaxTree.NativeXExpression InitializerMember0 = default(Developer.LanguageServices.NativeX.SyntaxTree.NativeXExpression);
            {
                int currentIndexCopy1 = currentToken;
                int currentIndex1 = currentToken;
                {
                    Developer.LanguageProvider.CodeTokenizer.ParseToken(tokens, ref currentIndex1, ref parseSuccess, "variable");
                }
                if (parseSuccess)
                {
                    currentIndexCopy1 = currentIndex1;
                }
                else
                {
                    goto LABEL_0;
                }
                {
                    Developer.LanguageServices.NativeX.SyntaxTree.NativeXType TypeMember1 = default(Developer.LanguageServices.NativeX.SyntaxTree.NativeXType);
                    {
                        Developer.LanguageServices.NativeX.SyntaxTree.NativeXType result2 = default(Developer.LanguageServices.NativeX.SyntaxTree.NativeXType);
                        int currentIndex2 = currentIndex1;
                        {
                            parseSuccess = false;
                            result2 = ParseType(tokens, ref currentIndex2, ref parseSuccess);
                        }
                        if (parseSuccess)
                        {
                            currentIndex1 = currentIndex2;
                            TypeMember1 = result2;
                        }
                    }
                    TypeMember0 = TypeMember1;
                }
                if (parseSuccess)
                {
                    currentIndexCopy1 = currentIndex1;
                }
                else
                {
                    goto LABEL_0;
                }
                {
                    System.String NameMember1 = default(System.String);
                    {
                        Developer.LanguageProvider.CodeToken result2 = default(Developer.LanguageProvider.CodeToken);
                        int currentIndex2 = currentIndex1;
                        {
                            result2 = Developer.LanguageProvider.CodeTokenizer.ParseToken(tokens, ref currentIndex2, ref parseSuccess, Developer.LanguageServices.NativeX.NativeXTokenizer.IdToken);
                        }
                        if (parseSuccess)
                        {
                            currentIndex1 = currentIndex2;
                            NameMember1 = result2.Value;
                        }
                    }
                    NameMember0 = NameMember1;
                }
                if (parseSuccess)
                {
                    currentIndexCopy1 = currentIndex1;
                }
                else
                {
                    goto LABEL_0;
                }
                {
                    Developer.LanguageServices.NativeX.SyntaxTree.NativeXLinking LinkingMember1 = default(Developer.LanguageServices.NativeX.SyntaxTree.NativeXLinking);
                    {
                        int currentIndex2 = -1;
                        currentIndex2 = currentIndex1;
                        {
                            Developer.LanguageServices.NativeX.SyntaxTree.NativeXLinking LinkingMember2 = default(Developer.LanguageServices.NativeX.SyntaxTree.NativeXLinking);
                            {
                                Developer.LanguageServices.NativeX.SyntaxTree.NativeXLinking result3 = default(Developer.LanguageServices.NativeX.SyntaxTree.NativeXLinking);
                                int currentIndex3 = currentIndex2;
                                {
                                    parseSuccess = false;
                                    result3 = ParseLinking(tokens, ref currentIndex3, ref parseSuccess);
                                }
                                if (parseSuccess)
                                {
                                    currentIndex2 = currentIndex3;
                                    LinkingMember2 = result3;
                                }
                            }
                            LinkingMember1 = LinkingMember2;
                        }
                        if (parseSuccess)
                        {
                            currentIndex1 = currentIndex2;
                        }
                        parseSuccess = true;
                    }
                    LinkingMember0 = LinkingMember1;
                }
                if (parseSuccess)
                {
                    currentIndexCopy1 = currentIndex1;
                }
                else
                {
                    goto LABEL_0;
                }
                {
                    Developer.LanguageServices.NativeX.SyntaxTree.NativeXExpression InitializerMember1 = default(Developer.LanguageServices.NativeX.SyntaxTree.NativeXExpression);
                    {
                        int currentIndex2 = -1;
                        currentIndex2 = currentIndex1;
                        {
                            Developer.LanguageServices.NativeX.SyntaxTree.NativeXExpression InitializerMember2 = default(Developer.LanguageServices.NativeX.SyntaxTree.NativeXExpression);
                            {
                                int currentIndexCopy3 = currentIndex2;
                                int currentIndex3 = currentIndex2;
                                {
                                    Developer.LanguageProvider.CodeTokenizer.ParseToken(tokens, ref currentIndex3, ref parseSuccess, "=");
                                }
                                if (parseSuccess)
                                {
                                    currentIndexCopy3 = currentIndex3;
                                }
                                else
                                {
                                    goto LABEL_1;
                                }
                                {
                                    Developer.LanguageServices.NativeX.SyntaxTree.NativeXExpression InitializerMember3 = default(Developer.LanguageServices.NativeX.SyntaxTree.NativeXExpression);
                                    {
                                        Developer.LanguageServices.NativeX.SyntaxTree.NativeXExpression result4 = default(Developer.LanguageServices.NativeX.SyntaxTree.NativeXExpression);
                                        int currentIndex4 = currentIndex3;
                                        {
                                            parseSuccess = false;
                                            result4 = ParseExpression(tokens, ref currentIndex4, ref parseSuccess);
                                        }
                                        if (parseSuccess)
                                        {
                                            currentIndex3 = currentIndex4;
                                            InitializerMember3 = result4;
                                        }
                                    }
                                    InitializerMember2 = InitializerMember3;
                                }
                                if (parseSuccess)
                                {
                                    currentIndexCopy3 = currentIndex3;
                                }
                                else
                                {
                                    goto LABEL_1;
                                }
                                currentIndex2 = currentIndexCopy3;
                            LABEL_1: ;
                            }
                            InitializerMember1 = InitializerMember2;
                        }
                        if (parseSuccess)
                        {
                            currentIndex1 = currentIndex2;
                        }
                        parseSuccess = true;
                    }
                    InitializerMember0 = InitializerMember1;
                }
                if (parseSuccess)
                {
                    currentIndexCopy1 = currentIndex1;
                }
                else
                {
                    goto LABEL_0;
                }
                {
                    Developer.LanguageProvider.CodeTokenizer.ParseToken(tokens, ref currentIndex1, ref parseSuccess, ";");
                }
                if (parseSuccess)
                {
                    currentIndexCopy1 = currentIndex1;
                }
                else
                {
                    goto LABEL_0;
                }
                currentToken = currentIndexCopy1;
            LABEL_0: ;
            }
            if (parseSuccess)
            {
                if (result == null) result = CodeNode.Create<Developer.LanguageServices.NativeX.SyntaxTree.NativeXVariableDeclaration>();
                result.Type = TypeMember0;
                result.Name = NameMember0;
                result.Linking = LinkingMember0;
                result.Initializer = InitializerMember0;
                result.Start = start;
                result.End = Developer.LanguageProvider.CodeTokenizer.GetEndPosition(tokens, currentToken);
            }
            return result;
        }

        public static Developer.LanguageServices.NativeX.SyntaxTree.NativeXStructureDeclaration ParseStructureDeclaration(List<CodeToken> tokens, ref int currentToken, ref bool parseSuccess)
        {
            Developer.LanguageProvider.TextPosition start = Developer.LanguageProvider.CodeTokenizer.GetStartPosition(tokens, currentToken);
            Developer.LanguageServices.NativeX.SyntaxTree.NativeXStructureDeclaration result = default(Developer.LanguageServices.NativeX.SyntaxTree.NativeXStructureDeclaration);
            System.String NameMember0 = default(System.String);
            Developer.LanguageServices.NativeX.SyntaxTree.NativeXLinking LinkingMember0 = default(Developer.LanguageServices.NativeX.SyntaxTree.NativeXLinking);
            Developer.LanguageProvider.CodeNodeList<Developer.LanguageServices.NativeX.SyntaxTree.NativeXNameTypePair> MembersMember0 = default(Developer.LanguageProvider.CodeNodeList<Developer.LanguageServices.NativeX.SyntaxTree.NativeXNameTypePair>);
            {
                int currentIndexCopy1 = currentToken;
                int currentIndex1 = currentToken;
                {
                    Developer.LanguageProvider.CodeTokenizer.ParseToken(tokens, ref currentIndex1, ref parseSuccess, "structure");
                }
                if (parseSuccess)
                {
                    currentIndexCopy1 = currentIndex1;
                }
                else
                {
                    goto LABEL_0;
                }
                {
                    System.String NameMember1 = default(System.String);
                    {
                        Developer.LanguageProvider.CodeToken result2 = default(Developer.LanguageProvider.CodeToken);
                        int currentIndex2 = currentIndex1;
                        {
                            result2 = Developer.LanguageProvider.CodeTokenizer.ParseToken(tokens, ref currentIndex2, ref parseSuccess, Developer.LanguageServices.NativeX.NativeXTokenizer.IdToken);
                        }
                        if (parseSuccess)
                        {
                            currentIndex1 = currentIndex2;
                            NameMember1 = result2.Value;
                        }
                    }
                    NameMember0 = NameMember1;
                }
                if (parseSuccess)
                {
                    currentIndexCopy1 = currentIndex1;
                }
                else
                {
                    goto LABEL_0;
                }
                {
                    Developer.LanguageServices.NativeX.SyntaxTree.NativeXLinking LinkingMember1 = default(Developer.LanguageServices.NativeX.SyntaxTree.NativeXLinking);
                    Developer.LanguageProvider.CodeNodeList<Developer.LanguageServices.NativeX.SyntaxTree.NativeXNameTypePair> MembersMember1 = default(Developer.LanguageProvider.CodeNodeList<Developer.LanguageServices.NativeX.SyntaxTree.NativeXNameTypePair>);
                    {
                        int currentIndex2 = -1;
                        currentIndex2 = currentIndex1;
                        {
                            Developer.LanguageProvider.CodeTokenizer.ParseToken(tokens, ref currentIndex2, ref parseSuccess, ";");
                        }
                        if (parseSuccess)
                        {
                            currentIndex1 = currentIndex2;
                            goto LABEL_SUCCESS_1;
                        }
                        currentIndex2 = currentIndex1;
                        {
                            Developer.LanguageServices.NativeX.SyntaxTree.NativeXLinking LinkingMember2 = default(Developer.LanguageServices.NativeX.SyntaxTree.NativeXLinking);
                            Developer.LanguageProvider.CodeNodeList<Developer.LanguageServices.NativeX.SyntaxTree.NativeXNameTypePair> MembersMember2 = default(Developer.LanguageProvider.CodeNodeList<Developer.LanguageServices.NativeX.SyntaxTree.NativeXNameTypePair>);
                            {
                                int currentIndexCopy3 = currentIndex2;
                                int currentIndex3 = currentIndex2;
                                {
                                    Developer.LanguageServices.NativeX.SyntaxTree.NativeXLinking LinkingMember3 = default(Developer.LanguageServices.NativeX.SyntaxTree.NativeXLinking);
                                    {
                                        int currentIndex4 = -1;
                                        currentIndex4 = currentIndex3;
                                        {
                                            Developer.LanguageServices.NativeX.SyntaxTree.NativeXLinking LinkingMember4 = default(Developer.LanguageServices.NativeX.SyntaxTree.NativeXLinking);
                                            {
                                                Developer.LanguageServices.NativeX.SyntaxTree.NativeXLinking result5 = default(Developer.LanguageServices.NativeX.SyntaxTree.NativeXLinking);
                                                int currentIndex5 = currentIndex4;
                                                {
                                                    parseSuccess = false;
                                                    result5 = ParseLinking(tokens, ref currentIndex5, ref parseSuccess);
                                                }
                                                if (parseSuccess)
                                                {
                                                    currentIndex4 = currentIndex5;
                                                    LinkingMember4 = result5;
                                                }
                                            }
                                            LinkingMember3 = LinkingMember4;
                                        }
                                        if (parseSuccess)
                                        {
                                            currentIndex3 = currentIndex4;
                                        }
                                        parseSuccess = true;
                                    }
                                    LinkingMember2 = LinkingMember3;
                                }
                                if (parseSuccess)
                                {
                                    currentIndexCopy3 = currentIndex3;
                                }
                                else
                                {
                                    goto LABEL_3;
                                }
                                {
                                    Developer.LanguageProvider.CodeTokenizer.ParseToken(tokens, ref currentIndex3, ref parseSuccess, "{");
                                }
                                if (parseSuccess)
                                {
                                    currentIndexCopy3 = currentIndex3;
                                }
                                else
                                {
                                    goto LABEL_3;
                                }
                                {
                                    Developer.LanguageProvider.CodeNodeList<Developer.LanguageServices.NativeX.SyntaxTree.NativeXNameTypePair> MembersMember3 = default(Developer.LanguageProvider.CodeNodeList<Developer.LanguageServices.NativeX.SyntaxTree.NativeXNameTypePair>);
                                    {
                                        Developer.LanguageProvider.CodeNodeList<Developer.LanguageServices.NativeX.SyntaxTree.NativeXNameTypePair> result4 = default(Developer.LanguageProvider.CodeNodeList<Developer.LanguageServices.NativeX.SyntaxTree.NativeXNameTypePair>);
                                        int currentIndex4 = currentIndex3;
                                        {
                                            result4 = new Developer.LanguageProvider.CodeNodeList<Developer.LanguageServices.NativeX.SyntaxTree.NativeXNameTypePair>();
                                            {
                                                Developer.LanguageServices.NativeX.SyntaxTree.NativeXNameTypePair result5 = default(Developer.LanguageServices.NativeX.SyntaxTree.NativeXNameTypePair);
                                                int currentIndex5 = currentIndex4;
                                                {
                                                    {
                                                        Developer.LanguageServices.NativeX.SyntaxTree.NativeXNameTypePair result6 = default(Developer.LanguageServices.NativeX.SyntaxTree.NativeXNameTypePair);
                                                        int currentIndexCopy6 = currentIndex5;
                                                        int currentIndex6 = currentIndex5;
                                                        {
                                                            {
                                                                Developer.LanguageServices.NativeX.SyntaxTree.NativeXNameTypePair result7 = default(Developer.LanguageServices.NativeX.SyntaxTree.NativeXNameTypePair);
                                                                int currentIndex7 = currentIndex6;
                                                                {
                                                                    parseSuccess = false;
                                                                    result7 = ParseStructureMemberItem(tokens, ref currentIndex7, ref parseSuccess);
                                                                }
                                                                if (parseSuccess)
                                                                {
                                                                    currentIndex6 = currentIndex7;
                                                                    result6 = result7;
                                                                }
                                                            }
                                                        }
                                                        if (parseSuccess)
                                                        {
                                                            currentIndexCopy6 = currentIndex6;
                                                        }
                                                        else
                                                        {
                                                            goto LABEL_5;
                                                        }
                                                        {
                                                            Developer.LanguageProvider.CodeTokenizer.ParseToken(tokens, ref currentIndex6, ref parseSuccess, ";");
                                                        }
                                                        if (parseSuccess)
                                                        {
                                                            currentIndexCopy6 = currentIndex6;
                                                        }
                                                        else
                                                        {
                                                            goto LABEL_5;
                                                        }
                                                        currentIndex5 = currentIndexCopy6;
                                                        result5 = result6;
                                                    LABEL_5: ;
                                                    }
                                                }
                                                if (parseSuccess)
                                                {
                                                    currentIndex4 = currentIndex5;
                                                    result4.Add(result5);
                                                    result4.Start = result5.Start;
                                                    result4.End = result5.End;
                                                }
                                                while (true)
                                                {
                                                    int currentIndexCopy5 = currentIndex4;
                                                    {
                                                        {
                                                            Developer.LanguageServices.NativeX.SyntaxTree.NativeXNameTypePair result6 = default(Developer.LanguageServices.NativeX.SyntaxTree.NativeXNameTypePair);
                                                            int currentIndexCopy6 = currentIndex5;
                                                            int currentIndex6 = currentIndex5;
                                                            {
                                                                {
                                                                    Developer.LanguageServices.NativeX.SyntaxTree.NativeXNameTypePair result7 = default(Developer.LanguageServices.NativeX.SyntaxTree.NativeXNameTypePair);
                                                                    int currentIndex7 = currentIndex6;
                                                                    {
                                                                        parseSuccess = false;
                                                                        result7 = ParseStructureMemberItem(tokens, ref currentIndex7, ref parseSuccess);
                                                                    }
                                                                    if (parseSuccess)
                                                                    {
                                                                        currentIndex6 = currentIndex7;
                                                                        result6 = result7;
                                                                    }
                                                                }
                                                            }
                                                            if (parseSuccess)
                                                            {
                                                                currentIndexCopy6 = currentIndex6;
                                                            }
                                                            else
                                                            {
                                                                goto LABEL_6;
                                                            }
                                                            {
                                                                Developer.LanguageProvider.CodeTokenizer.ParseToken(tokens, ref currentIndex6, ref parseSuccess, ";");
                                                            }
                                                            if (parseSuccess)
                                                            {
                                                                currentIndexCopy6 = currentIndex6;
                                                            }
                                                            else
                                                            {
                                                                goto LABEL_6;
                                                            }
                                                            currentIndex5 = currentIndexCopy6;
                                                            result5 = result6;
                                                        LABEL_6: ;
                                                        }
                                                    }
                                                    if (parseSuccess)
                                                    {
                                                        currentIndexCopy5 = currentIndex5;
                                                    }
                                                    else
                                                    {
                                                        goto LABEL_4;
                                                    }
                                                    currentIndex4 = currentIndexCopy5;
                                                    result4.Add(result5);
                                                    result4.End = result5.End;
                                                }
                                            LABEL_4: ; parseSuccess = true;
                                            }
                                        }
                                        if (parseSuccess)
                                        {
                                            currentIndex3 = currentIndex4;
                                            MembersMember3 = result4;
                                        }
                                    }
                                    MembersMember2 = MembersMember3;
                                }
                                if (parseSuccess)
                                {
                                    currentIndexCopy3 = currentIndex3;
                                }
                                else
                                {
                                    goto LABEL_3;
                                }
                                {
                                    Developer.LanguageProvider.CodeTokenizer.ParseToken(tokens, ref currentIndex3, ref parseSuccess, "}");
                                }
                                if (parseSuccess)
                                {
                                    currentIndexCopy3 = currentIndex3;
                                }
                                else
                                {
                                    goto LABEL_3;
                                }
                                currentIndex2 = currentIndexCopy3;
                            LABEL_3: ;
                            }
                            LinkingMember1 = LinkingMember2;
                            MembersMember1 = MembersMember2;
                        }
                        if (parseSuccess)
                        {
                            currentIndex1 = currentIndex2;
                            goto LABEL_SUCCESS_1;
                        }
                        goto LABEL_FAIL_2;
                    LABEL_SUCCESS_1: ;
                    LABEL_FAIL_2: ;
                    }
                    LinkingMember0 = LinkingMember1;
                    MembersMember0 = MembersMember1;
                }
                if (parseSuccess)
                {
                    currentIndexCopy1 = currentIndex1;
                }
                else
                {
                    goto LABEL_0;
                }
                currentToken = currentIndexCopy1;
            LABEL_0: ;
            }
            if (parseSuccess)
            {
                if (result == null) result = CodeNode.Create<Developer.LanguageServices.NativeX.SyntaxTree.NativeXStructureDeclaration>();
                result.Name = NameMember0;
                result.Linking = LinkingMember0;
                result.Members = MembersMember0;
                result.Start = start;
                result.End = Developer.LanguageProvider.CodeTokenizer.GetEndPosition(tokens, currentToken);
            }
            return result;
        }

        public static Developer.LanguageServices.NativeX.SyntaxTree.NativeXInstanceDeclaration ParseInstanceDeclaration(List<CodeToken> tokens, ref int currentToken, ref bool parseSuccess)
        {
            Developer.LanguageProvider.TextPosition start = Developer.LanguageProvider.CodeTokenizer.GetStartPosition(tokens, currentToken);
            Developer.LanguageServices.NativeX.SyntaxTree.NativeXInstanceDeclaration result = default(Developer.LanguageServices.NativeX.SyntaxTree.NativeXInstanceDeclaration);
            Developer.LanguageServices.NativeX.SyntaxTree.NativeXReferenceType TypeMember0 = default(Developer.LanguageServices.NativeX.SyntaxTree.NativeXReferenceType);
            System.String ConceptNameMember0 = default(System.String);
            Developer.LanguageProvider.CodeNodeList<Developer.LanguageServices.NativeX.SyntaxTree.NativeXNameExpressionPair> FunctionsMember0 = default(Developer.LanguageProvider.CodeNodeList<Developer.LanguageServices.NativeX.SyntaxTree.NativeXNameExpressionPair>);
            {
                int currentIndexCopy1 = currentToken;
                int currentIndex1 = currentToken;
                {
                    Developer.LanguageProvider.CodeTokenizer.ParseToken(tokens, ref currentIndex1, ref parseSuccess, "instance");
                }
                if (parseSuccess)
                {
                    currentIndexCopy1 = currentIndex1;
                }
                else
                {
                    goto LABEL_0;
                }
                {
                    Developer.LanguageServices.NativeX.SyntaxTree.NativeXReferenceType TypeMember1 = default(Developer.LanguageServices.NativeX.SyntaxTree.NativeXReferenceType);
                    {
                        Developer.LanguageServices.NativeX.SyntaxTree.NativeXReferenceType result2 = default(Developer.LanguageServices.NativeX.SyntaxTree.NativeXReferenceType);
                        int currentIndex2 = currentIndex1;
                        {
                            parseSuccess = false;
                            result2 = ParseReferenceType(tokens, ref currentIndex2, ref parseSuccess);
                        }
                        if (parseSuccess)
                        {
                            currentIndex1 = currentIndex2;
                            TypeMember1 = result2;
                        }
                    }
                    TypeMember0 = TypeMember1;
                }
                if (parseSuccess)
                {
                    currentIndexCopy1 = currentIndex1;
                }
                else
                {
                    goto LABEL_0;
                }
                {
                    Developer.LanguageProvider.CodeTokenizer.ParseToken(tokens, ref currentIndex1, ref parseSuccess, ":");
                }
                if (parseSuccess)
                {
                    currentIndexCopy1 = currentIndex1;
                }
                else
                {
                    goto LABEL_0;
                }
                {
                    System.String ConceptNameMember1 = default(System.String);
                    {
                        Developer.LanguageProvider.CodeToken result2 = default(Developer.LanguageProvider.CodeToken);
                        int currentIndex2 = currentIndex1;
                        {
                            result2 = Developer.LanguageProvider.CodeTokenizer.ParseToken(tokens, ref currentIndex2, ref parseSuccess, Developer.LanguageServices.NativeX.NativeXTokenizer.IdToken);
                        }
                        if (parseSuccess)
                        {
                            currentIndex1 = currentIndex2;
                            ConceptNameMember1 = result2.Value;
                        }
                    }
                    ConceptNameMember0 = ConceptNameMember1;
                }
                if (parseSuccess)
                {
                    currentIndexCopy1 = currentIndex1;
                }
                else
                {
                    goto LABEL_0;
                }
                {
                    Developer.LanguageProvider.CodeNodeList<Developer.LanguageServices.NativeX.SyntaxTree.NativeXNameExpressionPair> FunctionsMember1 = default(Developer.LanguageProvider.CodeNodeList<Developer.LanguageServices.NativeX.SyntaxTree.NativeXNameExpressionPair>);
                    {
                        int currentIndex2 = -1;
                        currentIndex2 = currentIndex1;
                        {
                            Developer.LanguageProvider.CodeTokenizer.ParseToken(tokens, ref currentIndex2, ref parseSuccess, ";");
                        }
                        if (parseSuccess)
                        {
                            currentIndex1 = currentIndex2;
                            goto LABEL_SUCCESS_1;
                        }
                        currentIndex2 = currentIndex1;
                        {
                            Developer.LanguageProvider.CodeNodeList<Developer.LanguageServices.NativeX.SyntaxTree.NativeXNameExpressionPair> FunctionsMember2 = default(Developer.LanguageProvider.CodeNodeList<Developer.LanguageServices.NativeX.SyntaxTree.NativeXNameExpressionPair>);
                            {
                                int currentIndexCopy3 = currentIndex2;
                                int currentIndex3 = currentIndex2;
                                {
                                    Developer.LanguageProvider.CodeTokenizer.ParseToken(tokens, ref currentIndex3, ref parseSuccess, "{");
                                }
                                if (parseSuccess)
                                {
                                    currentIndexCopy3 = currentIndex3;
                                }
                                else
                                {
                                    goto LABEL_3;
                                }
                                {
                                    Developer.LanguageProvider.CodeNodeList<Developer.LanguageServices.NativeX.SyntaxTree.NativeXNameExpressionPair> FunctionsMember3 = default(Developer.LanguageProvider.CodeNodeList<Developer.LanguageServices.NativeX.SyntaxTree.NativeXNameExpressionPair>);
                                    {
                                        Developer.LanguageProvider.CodeNodeList<Developer.LanguageServices.NativeX.SyntaxTree.NativeXNameExpressionPair> result4 = default(Developer.LanguageProvider.CodeNodeList<Developer.LanguageServices.NativeX.SyntaxTree.NativeXNameExpressionPair>);
                                        int currentIndex4 = currentIndex3;
                                        {
                                            result4 = new Developer.LanguageProvider.CodeNodeList<Developer.LanguageServices.NativeX.SyntaxTree.NativeXNameExpressionPair>();
                                            {
                                                Developer.LanguageServices.NativeX.SyntaxTree.NativeXNameExpressionPair result5 = default(Developer.LanguageServices.NativeX.SyntaxTree.NativeXNameExpressionPair);
                                                int currentIndex5 = currentIndex4;
                                                {
                                                    {
                                                        Developer.LanguageServices.NativeX.SyntaxTree.NativeXNameExpressionPair result6 = default(Developer.LanguageServices.NativeX.SyntaxTree.NativeXNameExpressionPair);
                                                        int currentIndexCopy6 = currentIndex5;
                                                        int currentIndex6 = currentIndex5;
                                                        {
                                                            {
                                                                Developer.LanguageServices.NativeX.SyntaxTree.NativeXNameExpressionPair result7 = default(Developer.LanguageServices.NativeX.SyntaxTree.NativeXNameExpressionPair);
                                                                int currentIndex7 = currentIndex6;
                                                                {
                                                                    parseSuccess = false;
                                                                    result7 = ParseInstanceFunctionItem(tokens, ref currentIndex7, ref parseSuccess);
                                                                }
                                                                if (parseSuccess)
                                                                {
                                                                    currentIndex6 = currentIndex7;
                                                                    result6 = result7;
                                                                }
                                                            }
                                                        }
                                                        if (parseSuccess)
                                                        {
                                                            currentIndexCopy6 = currentIndex6;
                                                        }
                                                        else
                                                        {
                                                            goto LABEL_5;
                                                        }
                                                        {
                                                            Developer.LanguageProvider.CodeTokenizer.ParseToken(tokens, ref currentIndex6, ref parseSuccess, ";");
                                                        }
                                                        if (parseSuccess)
                                                        {
                                                            currentIndexCopy6 = currentIndex6;
                                                        }
                                                        else
                                                        {
                                                            goto LABEL_5;
                                                        }
                                                        currentIndex5 = currentIndexCopy6;
                                                        result5 = result6;
                                                    LABEL_5: ;
                                                    }
                                                }
                                                if (parseSuccess)
                                                {
                                                    currentIndex4 = currentIndex5;
                                                    result4.Add(result5);
                                                    result4.Start = result5.Start;
                                                    result4.End = result5.End;
                                                }
                                                while (true)
                                                {
                                                    int currentIndexCopy5 = currentIndex4;
                                                    {
                                                        {
                                                            Developer.LanguageServices.NativeX.SyntaxTree.NativeXNameExpressionPair result6 = default(Developer.LanguageServices.NativeX.SyntaxTree.NativeXNameExpressionPair);
                                                            int currentIndexCopy6 = currentIndex5;
                                                            int currentIndex6 = currentIndex5;
                                                            {
                                                                {
                                                                    Developer.LanguageServices.NativeX.SyntaxTree.NativeXNameExpressionPair result7 = default(Developer.LanguageServices.NativeX.SyntaxTree.NativeXNameExpressionPair);
                                                                    int currentIndex7 = currentIndex6;
                                                                    {
                                                                        parseSuccess = false;
                                                                        result7 = ParseInstanceFunctionItem(tokens, ref currentIndex7, ref parseSuccess);
                                                                    }
                                                                    if (parseSuccess)
                                                                    {
                                                                        currentIndex6 = currentIndex7;
                                                                        result6 = result7;
                                                                    }
                                                                }
                                                            }
                                                            if (parseSuccess)
                                                            {
                                                                currentIndexCopy6 = currentIndex6;
                                                            }
                                                            else
                                                            {
                                                                goto LABEL_6;
                                                            }
                                                            {
                                                                Developer.LanguageProvider.CodeTokenizer.ParseToken(tokens, ref currentIndex6, ref parseSuccess, ";");
                                                            }
                                                            if (parseSuccess)
                                                            {
                                                                currentIndexCopy6 = currentIndex6;
                                                            }
                                                            else
                                                            {
                                                                goto LABEL_6;
                                                            }
                                                            currentIndex5 = currentIndexCopy6;
                                                            result5 = result6;
                                                        LABEL_6: ;
                                                        }
                                                    }
                                                    if (parseSuccess)
                                                    {
                                                        currentIndexCopy5 = currentIndex5;
                                                    }
                                                    else
                                                    {
                                                        goto LABEL_4;
                                                    }
                                                    currentIndex4 = currentIndexCopy5;
                                                    result4.Add(result5);
                                                    result4.End = result5.End;
                                                }
                                            LABEL_4: ; parseSuccess = true;
                                            }
                                        }
                                        if (parseSuccess)
                                        {
                                            currentIndex3 = currentIndex4;
                                            FunctionsMember3 = result4;
                                        }
                                    }
                                    FunctionsMember2 = FunctionsMember3;
                                }
                                if (parseSuccess)
                                {
                                    currentIndexCopy3 = currentIndex3;
                                }
                                else
                                {
                                    goto LABEL_3;
                                }
                                {
                                    Developer.LanguageProvider.CodeTokenizer.ParseToken(tokens, ref currentIndex3, ref parseSuccess, "}");
                                }
                                if (parseSuccess)
                                {
                                    currentIndexCopy3 = currentIndex3;
                                }
                                else
                                {
                                    goto LABEL_3;
                                }
                                currentIndex2 = currentIndexCopy3;
                            LABEL_3: ;
                            }
                            FunctionsMember1 = FunctionsMember2;
                        }
                        if (parseSuccess)
                        {
                            currentIndex1 = currentIndex2;
                            goto LABEL_SUCCESS_1;
                        }
                        goto LABEL_FAIL_2;
                    LABEL_SUCCESS_1: ;
                    LABEL_FAIL_2: ;
                    }
                    FunctionsMember0 = FunctionsMember1;
                }
                if (parseSuccess)
                {
                    currentIndexCopy1 = currentIndex1;
                }
                else
                {
                    goto LABEL_0;
                }
                currentToken = currentIndexCopy1;
            LABEL_0: ;
            }
            if (parseSuccess)
            {
                if (result == null) result = CodeNode.Create<Developer.LanguageServices.NativeX.SyntaxTree.NativeXInstanceDeclaration>();
                result.Type = TypeMember0;
                result.ConceptName = ConceptNameMember0;
                result.Functions = FunctionsMember0;
                result.Start = start;
                result.End = Developer.LanguageProvider.CodeTokenizer.GetEndPosition(tokens, currentToken);
            }
            return result;
        }

        public static Developer.LanguageServices.NativeX.SyntaxTree.NativeXNameExpressionPair ParseInstanceFunctionItem(List<CodeToken> tokens, ref int currentToken, ref bool parseSuccess)
        {
            Developer.LanguageProvider.TextPosition start = Developer.LanguageProvider.CodeTokenizer.GetStartPosition(tokens, currentToken);
            Developer.LanguageServices.NativeX.SyntaxTree.NativeXNameExpressionPair result = default(Developer.LanguageServices.NativeX.SyntaxTree.NativeXNameExpressionPair);
            System.String NameMember0 = default(System.String);
            Developer.LanguageServices.NativeX.SyntaxTree.NativeXExpression ExpressionMember0 = default(Developer.LanguageServices.NativeX.SyntaxTree.NativeXExpression);
            {
                int currentIndexCopy1 = currentToken;
                int currentIndex1 = currentToken;
                {
                    System.String NameMember1 = default(System.String);
                    {
                        Developer.LanguageProvider.CodeToken result2 = default(Developer.LanguageProvider.CodeToken);
                        int currentIndex2 = currentIndex1;
                        {
                            result2 = Developer.LanguageProvider.CodeTokenizer.ParseToken(tokens, ref currentIndex2, ref parseSuccess, Developer.LanguageServices.NativeX.NativeXTokenizer.IdToken);
                        }
                        if (parseSuccess)
                        {
                            currentIndex1 = currentIndex2;
                            NameMember1 = result2.Value;
                        }
                    }
                    NameMember0 = NameMember1;
                }
                if (parseSuccess)
                {
                    currentIndexCopy1 = currentIndex1;
                }
                else
                {
                    goto LABEL_0;
                }
                {
                    Developer.LanguageProvider.CodeTokenizer.ParseToken(tokens, ref currentIndex1, ref parseSuccess, "=");
                }
                if (parseSuccess)
                {
                    currentIndexCopy1 = currentIndex1;
                }
                else
                {
                    goto LABEL_0;
                }
                {
                    Developer.LanguageServices.NativeX.SyntaxTree.NativeXExpression ExpressionMember1 = default(Developer.LanguageServices.NativeX.SyntaxTree.NativeXExpression);
                    {
                        Developer.LanguageServices.NativeX.SyntaxTree.NativeXExpression result2 = default(Developer.LanguageServices.NativeX.SyntaxTree.NativeXExpression);
                        int currentIndex2 = currentIndex1;
                        {
                            parseSuccess = false;
                            result2 = ParseExpression(tokens, ref currentIndex2, ref parseSuccess);
                        }
                        if (parseSuccess)
                        {
                            currentIndex1 = currentIndex2;
                            ExpressionMember1 = result2;
                        }
                    }
                    ExpressionMember0 = ExpressionMember1;
                }
                if (parseSuccess)
                {
                    currentIndexCopy1 = currentIndex1;
                }
                else
                {
                    goto LABEL_0;
                }
                currentToken = currentIndexCopy1;
            LABEL_0: ;
            }
            if (parseSuccess)
            {
                if (result == null) result = CodeNode.Create<Developer.LanguageServices.NativeX.SyntaxTree.NativeXNameExpressionPair>();
                result.Name = NameMember0;
                result.Expression = ExpressionMember0;
                result.Start = start;
                result.End = Developer.LanguageProvider.CodeTokenizer.GetEndPosition(tokens, currentToken);
            }
            return result;
        }

        public static Developer.LanguageServices.NativeX.SyntaxTree.NativeXConceptDeclaration ParseConceptDeclaration(List<CodeToken> tokens, ref int currentToken, ref bool parseSuccess)
        {
            Developer.LanguageProvider.TextPosition start = Developer.LanguageProvider.CodeTokenizer.GetStartPosition(tokens, currentToken);
            Developer.LanguageServices.NativeX.SyntaxTree.NativeXConceptDeclaration result = default(Developer.LanguageServices.NativeX.SyntaxTree.NativeXConceptDeclaration);
            System.String ConceptTypeMember0 = default(System.String);
            System.String NameMember0 = default(System.String);
            Developer.LanguageServices.NativeX.SyntaxTree.NativeXLinking LinkingMember0 = default(Developer.LanguageServices.NativeX.SyntaxTree.NativeXLinking);
            Developer.LanguageProvider.CodeNodeList<Developer.LanguageServices.NativeX.SyntaxTree.NativeXNameTypePair> FunctionsMember0 = default(Developer.LanguageProvider.CodeNodeList<Developer.LanguageServices.NativeX.SyntaxTree.NativeXNameTypePair>);
            {
                int currentIndexCopy1 = currentToken;
                int currentIndex1 = currentToken;
                {
                    Developer.LanguageProvider.CodeTokenizer.ParseToken(tokens, ref currentIndex1, ref parseSuccess, "concept");
                }
                if (parseSuccess)
                {
                    currentIndexCopy1 = currentIndex1;
                }
                else
                {
                    goto LABEL_0;
                }
                {
                    System.String ConceptTypeMember1 = default(System.String);
                    {
                        Developer.LanguageProvider.CodeToken result2 = default(Developer.LanguageProvider.CodeToken);
                        int currentIndex2 = currentIndex1;
                        {
                            result2 = Developer.LanguageProvider.CodeTokenizer.ParseToken(tokens, ref currentIndex2, ref parseSuccess, Developer.LanguageServices.NativeX.NativeXTokenizer.IdToken);
                        }
                        if (parseSuccess)
                        {
                            currentIndex1 = currentIndex2;
                            ConceptTypeMember1 = result2.Value;
                        }
                    }
                    ConceptTypeMember0 = ConceptTypeMember1;
                }
                if (parseSuccess)
                {
                    currentIndexCopy1 = currentIndex1;
                }
                else
                {
                    goto LABEL_0;
                }
                {
                    Developer.LanguageProvider.CodeTokenizer.ParseToken(tokens, ref currentIndex1, ref parseSuccess, ":");
                }
                if (parseSuccess)
                {
                    currentIndexCopy1 = currentIndex1;
                }
                else
                {
                    goto LABEL_0;
                }
                {
                    System.String NameMember1 = default(System.String);
                    {
                        Developer.LanguageProvider.CodeToken result2 = default(Developer.LanguageProvider.CodeToken);
                        int currentIndex2 = currentIndex1;
                        {
                            result2 = Developer.LanguageProvider.CodeTokenizer.ParseToken(tokens, ref currentIndex2, ref parseSuccess, Developer.LanguageServices.NativeX.NativeXTokenizer.IdToken);
                        }
                        if (parseSuccess)
                        {
                            currentIndex1 = currentIndex2;
                            NameMember1 = result2.Value;
                        }
                    }
                    NameMember0 = NameMember1;
                }
                if (parseSuccess)
                {
                    currentIndexCopy1 = currentIndex1;
                }
                else
                {
                    goto LABEL_0;
                }
                {
                    Developer.LanguageServices.NativeX.SyntaxTree.NativeXLinking LinkingMember1 = default(Developer.LanguageServices.NativeX.SyntaxTree.NativeXLinking);
                    {
                        int currentIndex2 = -1;
                        currentIndex2 = currentIndex1;
                        {
                            Developer.LanguageServices.NativeX.SyntaxTree.NativeXLinking LinkingMember2 = default(Developer.LanguageServices.NativeX.SyntaxTree.NativeXLinking);
                            {
                                Developer.LanguageServices.NativeX.SyntaxTree.NativeXLinking result3 = default(Developer.LanguageServices.NativeX.SyntaxTree.NativeXLinking);
                                int currentIndex3 = currentIndex2;
                                {
                                    parseSuccess = false;
                                    result3 = ParseLinking(tokens, ref currentIndex3, ref parseSuccess);
                                }
                                if (parseSuccess)
                                {
                                    currentIndex2 = currentIndex3;
                                    LinkingMember2 = result3;
                                }
                            }
                            LinkingMember1 = LinkingMember2;
                        }
                        if (parseSuccess)
                        {
                            currentIndex1 = currentIndex2;
                        }
                        parseSuccess = true;
                    }
                    LinkingMember0 = LinkingMember1;
                }
                if (parseSuccess)
                {
                    currentIndexCopy1 = currentIndex1;
                }
                else
                {
                    goto LABEL_0;
                }
                {
                    Developer.LanguageProvider.CodeTokenizer.ParseToken(tokens, ref currentIndex1, ref parseSuccess, "{");
                }
                if (parseSuccess)
                {
                    currentIndexCopy1 = currentIndex1;
                }
                else
                {
                    goto LABEL_0;
                }
                {
                    Developer.LanguageProvider.CodeNodeList<Developer.LanguageServices.NativeX.SyntaxTree.NativeXNameTypePair> FunctionsMember1 = default(Developer.LanguageProvider.CodeNodeList<Developer.LanguageServices.NativeX.SyntaxTree.NativeXNameTypePair>);
                    {
                        Developer.LanguageProvider.CodeNodeList<Developer.LanguageServices.NativeX.SyntaxTree.NativeXNameTypePair> result2 = default(Developer.LanguageProvider.CodeNodeList<Developer.LanguageServices.NativeX.SyntaxTree.NativeXNameTypePair>);
                        int currentIndex2 = currentIndex1;
                        {
                            result2 = new Developer.LanguageProvider.CodeNodeList<Developer.LanguageServices.NativeX.SyntaxTree.NativeXNameTypePair>();
                            {
                                Developer.LanguageServices.NativeX.SyntaxTree.NativeXNameTypePair result3 = default(Developer.LanguageServices.NativeX.SyntaxTree.NativeXNameTypePair);
                                int currentIndex3 = currentIndex2;
                                {
                                    {
                                        Developer.LanguageServices.NativeX.SyntaxTree.NativeXNameTypePair result4 = default(Developer.LanguageServices.NativeX.SyntaxTree.NativeXNameTypePair);
                                        int currentIndexCopy4 = currentIndex3;
                                        int currentIndex4 = currentIndex3;
                                        {
                                            {
                                                Developer.LanguageServices.NativeX.SyntaxTree.NativeXNameTypePair result5 = default(Developer.LanguageServices.NativeX.SyntaxTree.NativeXNameTypePair);
                                                int currentIndex5 = currentIndex4;
                                                {
                                                    parseSuccess = false;
                                                    result5 = ParseConceptFunctionItem(tokens, ref currentIndex5, ref parseSuccess);
                                                }
                                                if (parseSuccess)
                                                {
                                                    currentIndex4 = currentIndex5;
                                                    result4 = result5;
                                                }
                                            }
                                        }
                                        if (parseSuccess)
                                        {
                                            currentIndexCopy4 = currentIndex4;
                                        }
                                        else
                                        {
                                            goto LABEL_2;
                                        }
                                        {
                                            Developer.LanguageProvider.CodeTokenizer.ParseToken(tokens, ref currentIndex4, ref parseSuccess, ";");
                                        }
                                        if (parseSuccess)
                                        {
                                            currentIndexCopy4 = currentIndex4;
                                        }
                                        else
                                        {
                                            goto LABEL_2;
                                        }
                                        currentIndex3 = currentIndexCopy4;
                                        result3 = result4;
                                    LABEL_2: ;
                                    }
                                }
                                if (parseSuccess)
                                {
                                    currentIndex2 = currentIndex3;
                                    result2.Add(result3);
                                    result2.Start = result3.Start;
                                    result2.End = result3.End;
                                }
                                while (true)
                                {
                                    int currentIndexCopy3 = currentIndex2;
                                    {
                                        {
                                            Developer.LanguageServices.NativeX.SyntaxTree.NativeXNameTypePair result4 = default(Developer.LanguageServices.NativeX.SyntaxTree.NativeXNameTypePair);
                                            int currentIndexCopy4 = currentIndex3;
                                            int currentIndex4 = currentIndex3;
                                            {
                                                {
                                                    Developer.LanguageServices.NativeX.SyntaxTree.NativeXNameTypePair result5 = default(Developer.LanguageServices.NativeX.SyntaxTree.NativeXNameTypePair);
                                                    int currentIndex5 = currentIndex4;
                                                    {
                                                        parseSuccess = false;
                                                        result5 = ParseConceptFunctionItem(tokens, ref currentIndex5, ref parseSuccess);
                                                    }
                                                    if (parseSuccess)
                                                    {
                                                        currentIndex4 = currentIndex5;
                                                        result4 = result5;
                                                    }
                                                }
                                            }
                                            if (parseSuccess)
                                            {
                                                currentIndexCopy4 = currentIndex4;
                                            }
                                            else
                                            {
                                                goto LABEL_3;
                                            }
                                            {
                                                Developer.LanguageProvider.CodeTokenizer.ParseToken(tokens, ref currentIndex4, ref parseSuccess, ";");
                                            }
                                            if (parseSuccess)
                                            {
                                                currentIndexCopy4 = currentIndex4;
                                            }
                                            else
                                            {
                                                goto LABEL_3;
                                            }
                                            currentIndex3 = currentIndexCopy4;
                                            result3 = result4;
                                        LABEL_3: ;
                                        }
                                    }
                                    if (parseSuccess)
                                    {
                                        currentIndexCopy3 = currentIndex3;
                                    }
                                    else
                                    {
                                        goto LABEL_1;
                                    }
                                    currentIndex2 = currentIndexCopy3;
                                    result2.Add(result3);
                                    result2.End = result3.End;
                                }
                            LABEL_1: ; parseSuccess = true;
                            }
                        }
                        if (parseSuccess)
                        {
                            currentIndex1 = currentIndex2;
                            FunctionsMember1 = result2;
                        }
                    }
                    FunctionsMember0 = FunctionsMember1;
                }
                if (parseSuccess)
                {
                    currentIndexCopy1 = currentIndex1;
                }
                else
                {
                    goto LABEL_0;
                }
                {
                    Developer.LanguageProvider.CodeTokenizer.ParseToken(tokens, ref currentIndex1, ref parseSuccess, "}");
                }
                if (parseSuccess)
                {
                    currentIndexCopy1 = currentIndex1;
                }
                else
                {
                    goto LABEL_0;
                }
                currentToken = currentIndexCopy1;
            LABEL_0: ;
            }
            if (parseSuccess)
            {
                if (result == null) result = CodeNode.Create<Developer.LanguageServices.NativeX.SyntaxTree.NativeXConceptDeclaration>();
                result.ConceptType = ConceptTypeMember0;
                result.Name = NameMember0;
                result.Linking = LinkingMember0;
                result.Functions = FunctionsMember0;
                result.Start = start;
                result.End = Developer.LanguageProvider.CodeTokenizer.GetEndPosition(tokens, currentToken);
            }
            return result;
        }

        public static Developer.LanguageServices.NativeX.SyntaxTree.NativeXNameTypePair ParseConceptFunctionItem(List<CodeToken> tokens, ref int currentToken, ref bool parseSuccess)
        {
            Developer.LanguageProvider.TextPosition start = Developer.LanguageProvider.CodeTokenizer.GetStartPosition(tokens, currentToken);
            Developer.LanguageServices.NativeX.SyntaxTree.NativeXNameTypePair result = default(Developer.LanguageServices.NativeX.SyntaxTree.NativeXNameTypePair);
            System.String NameMember0 = default(System.String);
            Developer.LanguageServices.NativeX.SyntaxTree.NativeXType TypeMember0 = default(Developer.LanguageServices.NativeX.SyntaxTree.NativeXType);
            {
                int currentIndexCopy1 = currentToken;
                int currentIndex1 = currentToken;
                {
                    System.String NameMember1 = default(System.String);
                    {
                        Developer.LanguageProvider.CodeToken result2 = default(Developer.LanguageProvider.CodeToken);
                        int currentIndex2 = currentIndex1;
                        {
                            result2 = Developer.LanguageProvider.CodeTokenizer.ParseToken(tokens, ref currentIndex2, ref parseSuccess, Developer.LanguageServices.NativeX.NativeXTokenizer.IdToken);
                        }
                        if (parseSuccess)
                        {
                            currentIndex1 = currentIndex2;
                            NameMember1 = result2.Value;
                        }
                    }
                    NameMember0 = NameMember1;
                }
                if (parseSuccess)
                {
                    currentIndexCopy1 = currentIndex1;
                }
                else
                {
                    goto LABEL_0;
                }
                {
                    Developer.LanguageProvider.CodeTokenizer.ParseToken(tokens, ref currentIndex1, ref parseSuccess, "=");
                }
                if (parseSuccess)
                {
                    currentIndexCopy1 = currentIndex1;
                }
                else
                {
                    goto LABEL_0;
                }
                {
                    Developer.LanguageServices.NativeX.SyntaxTree.NativeXType TypeMember1 = default(Developer.LanguageServices.NativeX.SyntaxTree.NativeXType);
                    {
                        Developer.LanguageServices.NativeX.SyntaxTree.NativeXType result2 = default(Developer.LanguageServices.NativeX.SyntaxTree.NativeXType);
                        int currentIndex2 = currentIndex1;
                        {
                            parseSuccess = false;
                            result2 = ParseType(tokens, ref currentIndex2, ref parseSuccess);
                        }
                        if (parseSuccess)
                        {
                            currentIndex1 = currentIndex2;
                            TypeMember1 = result2;
                        }
                    }
                    TypeMember0 = TypeMember1;
                }
                if (parseSuccess)
                {
                    currentIndexCopy1 = currentIndex1;
                }
                else
                {
                    goto LABEL_0;
                }
                currentToken = currentIndexCopy1;
            LABEL_0: ;
            }
            if (parseSuccess)
            {
                if (result == null) result = CodeNode.Create<Developer.LanguageServices.NativeX.SyntaxTree.NativeXNameTypePair>();
                result.Name = NameMember0;
                result.Type = TypeMember0;
                result.Start = start;
                result.End = Developer.LanguageProvider.CodeTokenizer.GetEndPosition(tokens, currentToken);
            }
            return result;
        }

    }
}
