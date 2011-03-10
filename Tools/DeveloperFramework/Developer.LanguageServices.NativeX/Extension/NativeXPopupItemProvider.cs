using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using Developer.LanguageServices.NativeX.SyntaxTree;
using System.Drawing;
using Developer.WinFormControls;
using Developer.LanguageProvider;
using Developer.WinFormControls.Extension;
using System.Windows.Forms;

namespace Developer.LanguageServices.NativeX.Extension
{
    public class NativeXPopupItemProvider
        : LanguagePopupListExtension
        , ITextEditorBoxEditingObserverExtension
    {
        protected NativeXEditingObserverProvider EditingObserverExtension { get; private set; }
        protected List<SnippetContent> Snippets { get; private set; }

        public NativeXPopupItemProvider(NativeXEditingObserverProvider editingObserverExtension)
        {
            this.EditingObserverExtension = editingObserverExtension;
            this.Snippets = new List<SnippetContent>();
            this.Snippets.Add(new SnippetContent(
                new SnippetContent.Snippet("for",
                        new SnippetContent.Text("for(variable int "),
                        new SnippetContent.EditableItem("VARIABLE", "i"),
                        new SnippetContent.Text(" = 0; when("),
                        new SnippetContent.EditableItem("VARIABLE"),
                        new SnippetContent.Text(" < "),
                        new SnippetContent.EditableItem("COUNT", "count"),
                        new SnippetContent.Text(") with "),
                        new SnippetContent.EditableItem("VARIABLE"),
                        new SnippetContent.Text("++;) do\r\n"),

                        new SnippetContent.Tab(), new SnippetContent.Text("{\r\n"),
                        new SnippetContent.Tab(), new SnippetContent.Text("\t"), new SnippetContent.Stop(), new SnippetContent.Text("\r\n"),
                        new SnippetContent.Tab(), new SnippetContent.Text("}")
                    )
                ));
            this.Snippets.Add(new SnippetContent(
                new SnippetContent.Snippet("forr",
                        new SnippetContent.Text("for(variable int "),
                        new SnippetContent.EditableItem("VARIABLE", "i"),
                        new SnippetContent.Text(" = "),
                        new SnippetContent.EditableItem("COUNT", "count"),
                        new SnippetContent.Text(" - 1; when("),
                        new SnippetContent.EditableItem("VARIABLE"),
                        new SnippetContent.Text(" >= 0) with "),
                        new SnippetContent.EditableItem("VARIABLE"),
                        new SnippetContent.Text("--;) do\r\n"),

                        new SnippetContent.Tab(), new SnippetContent.Text("{\r\n"),
                        new SnippetContent.Tab(), new SnippetContent.Text("\t"), new SnippetContent.Stop(), new SnippetContent.Text("\r\n"),
                        new SnippetContent.Tab(), new SnippetContent.Text("}")
                    )
                ));
        }

        public override bool IsPopupListCharAcceptable(char c)
        {
            return c == '#' || base.IsPopupListCharAcceptable(c);
        }

        public override bool IsPopupListKeyAcceptable(KeyEventArgs e)
        {
            return (!e.Control && !e.Alt && e.Shift && e.KeyCode == Keys.D3) || base.IsPopupListKeyAcceptable(e);
        }

        public override bool IsPopupListCloseChar(char c)
        {
            return c == '"' || base.IsPopupListCloseChar(c);
        }

        #region Popup Item Calculation

        protected IEnumerable<TextEditorPopupItem> CreatePopupStructureMembers(NativeXAbstractStructureType structureType)
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

        protected IEnumerable<TextEditorPopupItem> CreatePopupInstanceFunctions(string conceptName, CodeScope scope)
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

        protected IEnumerable<TextEditorPopupItem> CreatePopupExpressions(CodeScope scope)
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
            string[] keywords = new string[] { "result", "cast", "exception", "true", "false", "null", "sizeof", "offsetof" };
            items.AddRange(
                keywords.Select(k => new TextEditorPopupItem()
                {
                    Text = k,
                    Image = keywordImage
                }));
            return items;
        }

        protected IEnumerable<TextEditorPopupItem> CreatePopupTypes(CodeScope scope)
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
            foreach (string key in new string[] { "typeof", "function" })
            {
                items.Add(new TextEditorPopupItem()
                {
                    Text = key,
                    Image = Images.Keyword
                });
            }
            return items;
        }

        protected IEnumerable<TextEditorPopupItem> CreatePopupStatementKeywords()
        {
            Bitmap keywordImage = Images.Keyword;
            string[] keywords = new string[] { "variable", "if", "else", "type", "break", "continue", "exit", "do", "while", "loop", "for", "when", "with", "try", "catch", "throw" };
            return keywords
                .Select(k => new TextEditorPopupItem()
                {
                    Text = k,
                    Image = keywordImage
                });
        }

        protected IEnumerable<TextEditorPopupItem> CreatePopupDeclarationKeywords()
        {
            Bitmap keywordImage = Images.Keyword;
            string[] keywords = new string[] { "generic", "function", "type", "variable", "structure", "concept", "instance", "unit", "uses", "where", "#public", "#assembly_initialization" };
            return keywords
                .Select(k => new TextEditorPopupItem()
                {
                    Text = k,
                    Image = keywordImage
                });
        }

        protected IEnumerable<TextEditorPopupItem> CreatePopupConcepts(CodeScope scope)
        {
            Bitmap templateImage = null;
            List<TextEditorPopupItem> items = new List<TextEditorPopupItem>();
            foreach (CodeNode node in scope.FindAllDistinct())
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
            return items;
        }

        protected IEnumerable<TextEditorPopupItem> PopupGenericParameters(CodeScope scope)
        {
            Bitmap parameterImage = null;
            List<TextEditorPopupItem> items = new List<TextEditorPopupItem>();
            foreach (CodeNode node in scope.FindAllDistinct())
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
            return items;
        }

        #endregion

        #region Popups

        private void PopupStructureMembers(NativeXAbstractStructureType structureType)
        {
            this.Callback.TextEditorBox.UIExtensions.PopupItems(
                CreatePopupStructureMembers(structureType),
                null,
                null,
                forceClosingPrevious: true);
        }

        private void PopupInstanceFunctions(NativeXInstanceFunctionExpression function)
        {
            if (function.ConceptName != null && function.Scope != null)
            {
                this.Callback.TextEditorBox.UIExtensions.PopupItems(
                    CreatePopupInstanceFunctions(function.ConceptName.ReferenceName, function.Scope),
                    null,
                    null,
                    forceClosingPrevious: true);
            }
        }

        private void PopupConcepts(CodeScope scope)
        {
            this.Callback.TextEditorBox.UIExtensions.PopupItems(
                CreatePopupConcepts(scope),
                null,
                null,
                forceClosingPrevious: true);
        }

        private void PopupGenericParameters(string reference, CodeScope scope)
        {
            this.Callback.TextEditorBox.UIExtensions.PopupItems(
                PopupGenericParameters(scope),
                null,
                null,
                searchingKey: reference);
        }

        private void PopupInstanceFunctions(string reference, string conceptName, CodeScope scope)
        {
            this.Callback.TextEditorBox.UIExtensions.PopupItems(
                CreatePopupInstanceFunctions(conceptName, scope),
                null,
                null,
                searchingKey: reference,
                forceClosingPrevious: true);
        }

        private void PopupExpressions(string reference, CodeScope scope)
        {
            this.Callback.TextEditorBox.UIExtensions.PopupItems(
                CreatePopupExpressions(scope),
                null,
                null,
                searchingKey: reference);
        }

        private void PopupExpressionsAndTypes(string reference, CodeScope scope)
        {
            this.Callback.TextEditorBox.UIExtensions.PopupItems(
                CreatePopupExpressions(scope).Concat(CreatePopupTypes(scope)),
                null,
                null,
                searchingKey: reference);
        }

        private void PopupExpressionsAndTypesAndStatements(string reference, CodeScope scope)
        {
            this.Callback.TextEditorBox.UIExtensions.PopupItems(
                CreatePopupExpressions(scope)
                    .Concat(CreatePopupStatementKeywords())
                    .Concat(CreatePopupTypes(scope)),
                this.Snippets,
                Images.Snippet,
                searchingKey: reference);
        }

        private void PopupTypes(string reference, CodeScope scope)
        {
            this.Callback.TextEditorBox.UIExtensions.PopupItems(
                CreatePopupTypes(scope),
                null,
                null,
                searchingKey: reference);
        }

        private void PopupDeclarationKeywords(string reference)
        {
            this.Callback.TextEditorBox.UIExtensions.PopupItems(
                CreatePopupDeclarationKeywords(),
                null,
                null,
                searchingKey: reference);
        }

        #endregion

        #region Popup Starter

        public virtual void OnBeforeEdit(TextPosition start, TextPosition end, ref string[] lines)
        {
        }

        public virtual void OnAfterEdit(TextPosition start, TextPosition oldEnd, TextPosition newEnd)
        {
            if (!this.PreventPopupList)
            {
                string inputText = this.Callback.TextEditorBox.TextProvider.GetString(start, newEnd);
                if (this.EditingObserverExtension.EditingNode == null)
                {
                    if (inputText.Length == 1)
                    {
                        if ('a' <= inputText[0] && inputText[0] <= 'z' || 'A' <= inputText[0] && inputText[0] <= 'Z' || inputText[0] == '#')
                        {
                            PopupDeclarationKeywords(inputText);
                        }
                    }
                }
                else
                {
                    switch (inputText)
                    {
                        case ".":
                            {
                                var node = this.EditingObserverExtension.EditingNode.FindDeepest<NativeXMemberExpression>(this.EditingObserverExtension.NodePositionToEditing(newEnd));
                                if (node != null)
                                {
                                    PopupStructureMembers(node.GetStructureType());
                                    break;
                                }
                            }
                            break;
                        case ">":
                            {
                                var node = this.EditingObserverExtension.EditingNode.FindDeepest<NativeXPointerMemberExpression>(this.EditingObserverExtension.NodePositionToEditing(newEnd));
                                if (node != null)
                                {
                                    PopupStructureMembers(node.GetStructureType());
                                    break;
                                }
                            }
                            break;
                        case ":":
                            {
                                TextPosition end = this.EditingObserverExtension.NodePositionToEditing(newEnd);
                                if (end.col >= 2)
                                {
                                    if (this.EditingObserverExtension.EditingNodeCode.GetString(new TextPosition(end.row, end.col - 2), new TextPosition(end.row, end.col - 1)) == ":")
                                    {
                                        {
                                            var node = this.EditingObserverExtension.EditingNode.FindDeepest<NativeXInstanceFunctionExpression>(end);
                                            if (node != null && node.Scope != null)
                                            {
                                                PopupInstanceFunctions(node);
                                                break;
                                            }
                                        }
                                        {
                                            var node = this.EditingObserverExtension.EditingNode.FindDeepest<NativeXTypeofMemberType>(end);
                                            if (node != null && node.Scope != null && node.Type != null)
                                            {
                                                PopupStructureMembers(node.Type.AbstractType.Unwrap() as NativeXAbstractStructureType);
                                                break;
                                            }
                                        }
                                        {
                                            var node = this.EditingObserverExtension.EditingNode.FindDeepest<NativeXOffsetofMemberExpression>(end);
                                            if (node != null && node.Scope != null && node.Type != null)
                                            {
                                                PopupStructureMembers(node.Type.AbstractType.Unwrap() as NativeXAbstractStructureType);
                                                break;
                                            }
                                        }
                                    }
                                    else
                                    {
                                        {
                                            var node = this.EditingObserverExtension.EditingNode.FindDeepest<NativeXGenericConstraint>(end);
                                            if (node != null && node.Scope != null)
                                            {
                                                PopupConcepts(node.Scope);
                                                break;
                                            }
                                        }
                                        {
                                            var node = this.EditingObserverExtension.EditingNode.FindDeepest<NativeXInstanceDeclaration>(end);
                                            if (node != null && node.Scope != null)
                                            {
                                                PopupConcepts(node.Scope);
                                                break;
                                            }
                                        }
                                    }
                                }
                            }
                            break;
                        default:
                            {
                                var node = this.EditingObserverExtension.EditingNode.FindDeepest<NativeXReferenceExpression>(this.EditingObserverExtension.NodePositionToEditing(newEnd));
                                if (node != null && node.Scope != null && node.ReferencedName != null && node.ReferencedName == inputText)
                                {
                                    if (node.FindParent<NativeXNameExpressionPair>() != null)
                                    {
                                        PopupExpressions(node.ReferencedName, node.Scope);
                                        break;
                                    }

                                    var exprstat = node.FindParent<NativeXExpressionStatement>();
                                    if (exprstat != null && exprstat.Expression == node)
                                    {
                                        PopupExpressionsAndTypesAndStatements(node.ReferencedName, node.Scope);
                                        break;
                                    }

                                    PopupExpressionsAndTypes(node.ReferencedName, node.Scope);
                                    break;
                                }
                            }
                            {
                                var node = this.EditingObserverExtension.EditingNode.FindDeepest<NativeXReferenceType>(this.EditingObserverExtension.NodePositionToEditing(newEnd));
                                if (node != null && node.Scope != null && node.ReferencedName != null && node.ReferencedName == inputText)
                                {
                                    PopupTypes(node.ReferencedName, node.Scope);
                                    break;
                                }
                            }
                            {
                                var node = this.EditingObserverExtension.EditingNode.FindDeepest<NativeXNameExpressionPair>(this.EditingObserverExtension.NodePositionToEditing(newEnd));
                                if (node != null && node.Scope != null && node.Name != null && node.Name == inputText)
                                {
                                    var instdecl = node.FindParent<NativeXInstanceDeclaration>();
                                    if (instdecl != null && instdecl.ConceptName != null)
                                    {
                                        PopupInstanceFunctions(node.Name, instdecl.ConceptName.ReferenceName, node.Scope);
                                        break;
                                    }
                                }
                            }
                            {
                                var node = this.EditingObserverExtension.EditingNode.FindDeepest<NativeXGenericConstraint>(this.EditingObserverExtension.NodePositionToEditing(newEnd));
                                if (node != null && node.Scope != null && node.ParameterName != null && node.ParameterName == inputText)
                                {
                                    PopupGenericParameters(node.ParameterName, node.Scope);
                                    break;
                                }
                            }
                            {
                                var node = this.EditingObserverExtension.EditingNode.FindDeepest<NativeXNode>(this.EditingObserverExtension.NodePositionToEditing(newEnd));
                                if (inputText.Length == 1 && node.GetType() == typeof(NativeXDeclaration))
                                {
                                    if ('a' <= inputText[0] && inputText[0] <= 'z' || 'A' <= inputText[0] && inputText[0] <= 'Z' || inputText[0] == '#')
                                    {
                                        PopupDeclarationKeywords(inputText);
                                        break;
                                    }
                                }
                            }
                            break;
                    }
                }
            }
        }

        #endregion
    }
}
