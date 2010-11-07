using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using Developer.LanguageServices.NativeX.SyntaxTree;
using System.Drawing;
using Developer.WinFormControls;
using Developer.LanguageProvider;
using Developer.LanguageServices.NativeX;

namespace Test.Host.LanguageForms.NativeX
{
    public static class NativeXPopupItemProvider
    {
        public static IEnumerable<TextEditorPopupItem> PopupStructureMembers(NativeXAbstractStructureType structureType)
        {
            if (structureType != null && structureType.Members.Count > 0)
            {
                Bitmap memberImage = Images.Member;
                var members = structureType.Members
                    .Select(s => new TextEditorPopupItem()
                    {
                        Image = memberImage,
                        Text = s.Key
                    });
                return members;
            }
            return new TextEditorPopupItem[] { };
        }

        public static IEnumerable<TextEditorPopupItem> PopupInstanceFunctions(string conceptName, CodeScope scope)
        {
            NativeXConceptDeclaration concept = scope.Find(conceptName) as NativeXConceptDeclaration;
            if (concept != null && concept.Functions != null && concept.Functions.Count > 0)
            {
                Bitmap functionImage = Images.Function;
                var members = concept.Functions
                    .Select(f => new TextEditorPopupItem()
                    {
                        Image = functionImage,
                        Text = f.Name
                    });
                return members;
            }
            return new TextEditorPopupItem[] { };
        }

        public static IEnumerable<TextEditorPopupItem> PopupExpressions(CodeScope scope)
        {
            Bitmap functionImage = null;
            Bitmap memberImage = null;
            Bitmap templateImage = null;
            Bitmap parameterImage = null;
            List<TextEditorPopupItem> items = new List<TextEditorPopupItem>();
            foreach (CodeNode node in scope.FindAllDistinct())
            {
                {
                    NativeXNameTypePair parameter = node as NativeXNameTypePair;
                    if (parameter != null && parameter.Name != null)
                    {
                        items.Add(new TextEditorPopupItem()
                        {
                            Text = parameter.Name,
                            Image = (parameterImage ?? (parameterImage = Images.Parameter))
                        });
                    }
                }
                {
                    NativeXVariableStatement varstat = node as NativeXVariableStatement;
                    if (varstat != null && varstat.Name != null)
                    {
                        items.Add(new TextEditorPopupItem()
                        {
                            Text = varstat.Name,
                            Image = (memberImage ?? (memberImage = Images.Member))
                        });
                    }
                }
                {
                    NativeXVariableDeclaration vardecl = node as NativeXVariableDeclaration;
                    if (vardecl != null && vardecl.Name != null)
                    {
                        items.Add(new TextEditorPopupItem()
                        {
                            Text = vardecl.Name,
                            Image = (memberImage ?? (memberImage = Images.Member))
                        });
                    }
                }
                {
                    NativeXFunctionDeclaration funcdecl = node as NativeXFunctionDeclaration;
                    if (funcdecl != null && funcdecl.Name != null)
                    {
                        items.Add(new TextEditorPopupItem()
                        {
                            Text = funcdecl.Name,
                            Image = (functionImage ?? (functionImage = Images.Function))
                        });
                    }
                }
                {
                    NativeXConceptDeclaration conceptdecl = node as NativeXConceptDeclaration;
                    if (conceptdecl != null && conceptdecl.Name != null)
                    {
                        items.Add(new TextEditorPopupItem()
                        {
                            Text = conceptdecl.Name,
                            Image = (templateImage ?? (templateImage = Images.Template))
                        });
                    }
                }
            }

            Bitmap keywordImage = Images.Keyword;
            string[] keywords = new string[] { "result", "cast", "exception", "true", "false", "null" };
            items.AddRange(
                keywords.Select(k => new TextEditorPopupItem()
                {
                    Text = k,
                    Image = keywordImage
                }));
            return items;
        }

        public static IEnumerable<TextEditorPopupItem> PopupTypes(CodeScope scope)
        {
            Bitmap typeImage = null;
            Bitmap templateImage = null;
            Bitmap parameterImage = null;
            List<TextEditorPopupItem> items = new List<TextEditorPopupItem>();
            foreach (CodeNode node in scope.FindAllDistinct())
            {
                {
                    NativeXStructureDeclaration structdecl = node as NativeXStructureDeclaration;
                    if (structdecl != null && structdecl.Name != null)
                    {
                        items.Add(new TextEditorPopupItem()
                        {
                            Text = structdecl.Name,
                            Image = structdecl.GenericParameters == null
                                ? (typeImage ?? (typeImage = Images.Type))
                                : (templateImage ?? (templateImage = Images.Template))
                        });
                    }
                }
                {
                    NativeXTypeRenameDeclaration typedecl = node as NativeXTypeRenameDeclaration;
                    if (typedecl != null && typedecl.Name != null)
                    {
                        items.Add(new TextEditorPopupItem()
                        {
                            Text = typedecl.Name,
                            Image = typedecl.GenericParameters == null
                                ? (typeImage ?? (typeImage = Images.Type))
                                : (templateImage ?? (templateImage = Images.Template))
                        });
                    }
                }
                {
                    NativeXGenericParameter genparam = node as NativeXGenericParameter;
                    if (genparam != null && genparam.ParameterName != null)
                    {
                        items.Add(new TextEditorPopupItem()
                        {
                            Text = genparam.ParameterName,
                            Image = (parameterImage ?? (parameterImage = Images.Parameter))
                        });
                    }
                }
            }
            foreach (string key in NativeXTokenizer.TypedKeywords)
            {
                items.Add(new TextEditorPopupItem()
                {
                    Text = key,
                    Image = (typeImage ?? (typeImage = Images.Type))
                });
            }
            items.Add(new TextEditorPopupItem()
            {
                Text = "function",
                Image = Images.Keyword
            });
            return items;
        }

        public static IEnumerable<TextEditorPopupItem> PopupStatementKeywords()
        {
            Bitmap keywordImage = Images.Keyword;
            string[] keywords = new string[] { "variable", "if", "type", "break", "continue", "exit", "do", "while", "loop", "for", "when", "with", "try", "catch", "throw" };
            return keywords
                .Select(k => new TextEditorPopupItem()
                {
                    Text = k,
                    Image = keywordImage
                });
        }

        public static IEnumerable<TextEditorPopupItem> PopupDeclarationKeywords()
        {
            Bitmap keywordImage = Images.Keyword;
            string[] keywords = new string[] { "generic", "function", "type", "variable", "structure", "concept", "instance" };
            return keywords
                .Select(k => new TextEditorPopupItem()
                {
                    Text = k,
                    Image = keywordImage
                });
        }
    }
}
