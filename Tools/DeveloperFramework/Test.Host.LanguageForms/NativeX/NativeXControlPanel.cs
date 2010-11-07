﻿using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using Developer.WinFormControls;
using System.Windows.Forms;
using Developer.WinFormControls.Core;
using Developer.LanguageProvider;
using System.Drawing;
using Developer.LanguageServices.NativeX.SyntaxTree;
using Developer.LanguageServices.NativeX;

namespace Test.Host.LanguageForms.NativeX
{
    class NativeXControlPanel : LanguageControlPanel<NativeXAnalyzingResult, NativeXStatement, NativeXEditingStatement>
    {
        private NativeXForm form = null;
        private int counter = 0;

        public NativeXControlPanel(NativeXForm form)
        {
            this.form = form;
        }

        #region LanguageControlPanel Members

        protected override CalculationNotifier<string, NativeXAnalyzingResult> CreateAnalyzer()
        {
            return new NativeXCodeAnalyzer();
        }

        protected override NativeXEditingStatement ParseEditingNodeContainer(string code)
        {
            List<CodeToken> tokens = NativeXCodeParser.Tokenize(code.ToCharArray());
            int currentToken = 0;
            bool parseSuccess = false;
            return NativeXCodeParser.ParseEditingStatement(tokens, ref currentToken, ref parseSuccess);
        }

        protected override NativeXStatement GetEditingNode(NativeXAnalyzingResult result, TextPosition pos)
        {
            if (result != null && result.Unit != null)
            {
                return result.Unit.FindDeepest<NativeXStatement>(pos);
            }
            else
            {
                return null;
            }
        }

        protected override void ResultUpdated()
        {
            TemporaryResultUpdated();
        }

        protected override void TemporaryResultUpdated()
        {
            this.form.ContextText = this.EditingNodeCode.Text
                + "\r\n********************************\r\nSYNTAX TREE\r\n********************************\r\n"
                + (this.EditingNode == null ? "<NULL>" : this.EditingNode.ToString());
        }

        protected override void AnalyzerResultReceived(NativeXAnalyzingResult result)
        {
            string treeText = result.Unit == null ? "<NULL>" : result.Unit.ToString();
            string statusText = (++this.counter).ToString();

            this.Callback.TextEditorBox.Invoke(new MethodInvoker(() =>
            {
                this.form.TreeText = treeText;
                this.form.StatusText = statusText;
            }));
        }

        #endregion

        #region Popups

        private void PopupStructureMembers(NativeXAbstractStructureType structureType)
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
                this.Callback.TextEditorBox.PopupItems(members, forceClosingPrevious: true);
            }
        }

        private void PopupInstanceFunctions(NativeXInstanceFunctionExpression function)
        {
            if (function.ConceptName != null)
            {
                NativeXConceptDeclaration concept = function.Scope.Find(function.ConceptName) as NativeXConceptDeclaration;
                if (concept != null && concept.Functions != null && concept.Functions.Count > 0)
                {
                    Bitmap functionImage = Images.Function;
                    var members = concept.Functions
                        .Select(f => new TextEditorPopupItem()
                        {
                            Image = functionImage,
                            Text = f.Name
                        });
                    this.Callback.TextEditorBox.PopupItems(members, forceClosingPrevious: true);
                }
            }
        }

        private void PopupExpressions(string reference, CodeScope scope)
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
            this.Callback.TextEditorBox.PopupItems(items, searchingKey: reference);
        }

        private void PopupTypes(string reference, CodeScope scope)
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
            this.Callback.TextEditorBox.PopupItems(items, searchingKey: reference);
        }

        #endregion

        #region ITextEditorControlPanel Members

        public override void OnAfterEdit(TextPosition start, TextPosition oldEnd, TextPosition newEnd)
        {
            base.OnAfterEdit(start, oldEnd, newEnd);
            if (this.EditingNode != null && !this.PreventPopupList)
            {
                string inputText = this.Callback.TextEditorBox.TextProvider.GetString(start, newEnd);
                switch (inputText)
                {
                    case ".":
                        {
                            var node = this.EditingNode.FindDeepest<NativeXMemberExpression>(ConvertToEditingPosition(newEnd));
                            if (node != null)
                            {
                                PopupStructureMembers(node.GetStructureType());
                            }
                        }
                        break;
                    case ">":
                        {
                            var node = this.EditingNode.FindDeepest<NativeXPointerMemberExpression>(ConvertToEditingPosition(newEnd));
                            if (node != null)
                            {
                                PopupStructureMembers(node.GetStructureType());
                            }
                        }
                        break;
                    case ":":
                        {
                            TextPosition end = ConvertToEditingPosition(newEnd);
                            if (end.col >= 2)
                            {
                                if (this.EditingNodeCode.GetString(new TextPosition(end.row, end.col - 2), new TextPosition(end.row, end.col - 1)) == ":")
                                {
                                    var node = this.EditingNode.FindDeepest<NativeXInstanceFunctionExpression>(end);
                                    if (node != null && node.Scope != null)
                                    {
                                        PopupInstanceFunctions(node);
                                    }
                                }
                            }
                        }
                        break;
                    default:
                        {
                            var node = this.EditingNode.FindDeepest<NativeXReferenceExpression>(ConvertToEditingPosition(newEnd));
                            if (node != null && node.Scope != null && node.ReferencedName != null && node.ReferencedName == inputText)
                            {
                                PopupExpressions(node.ReferencedName, node.Scope);
                                break;
                            }
                        }
                        {
                            var node = this.EditingNode.FindDeepest<NativeXReferenceType>(ConvertToEditingPosition(newEnd));
                            if (node != null && node.Scope != null && node.ReferencedName != null && node.ReferencedName == inputText)
                            {
                                PopupTypes(node.ReferencedName, node.Scope);
                                break;
                            }
                        }
                        break;
                }
            }
        }

        #endregion
    }
}
