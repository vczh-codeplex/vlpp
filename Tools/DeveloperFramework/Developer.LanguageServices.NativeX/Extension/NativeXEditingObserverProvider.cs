using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using Developer.LanguageServices.NativeX.SyntaxTree;
using Developer.WinFormControls.Extension;
using Developer.LanguageProvider;

namespace Developer.LanguageServices.NativeX.Extension
{
    public class NativeXEditingObserverProvider : LanguageEditingObserverExtension<NativeXAnalyzingResult, NativeXDeclaration, NativeXEditingDeclarations>
    {
        public NativeXEditingObserverProvider()
            : base(new NativeXProvider())
        {
        }

        public NativeXEditingObserverProvider(NativeXProvider provider)
            : base(provider)
        {
        }

        public class NativeXProvider : ILanguageEditingObserverProvider<NativeXAnalyzingResult, NativeXDeclaration, NativeXEditingDeclarations>
        {
            protected NativeXEditingObserverProvider Extension { get; private set; }

            public virtual void Attach(LanguageEditingObserverExtension<NativeXAnalyzingResult, NativeXDeclaration, NativeXEditingDeclarations> extension)
            {
                this.Extension = (NativeXEditingObserverProvider)extension;
            }

            public virtual CalculationNotifier<string, NativeXAnalyzingResult> CreateAnalyzer()
            {
                return new NativeXCodeAnalyzer();
            }

            public virtual NativeXEditingDeclarations ParseEditingNodeContainer(string code)
            {
                List<CodeToken> tokens = NativeXCodeParser.Tokenize(code.ToCharArray());
                int currentToken = 0;
                bool parseSuccess = false;
                return NativeXCodeParser.ParseEditingDeclarations(tokens, ref currentToken, ref parseSuccess);
            }

            public virtual NativeXDeclaration GetEditingNode(NativeXAnalyzingResult result, TextPosition pos)
            {
                if (result != null && result.Unit != null && result.Unit.Declarations != null)
                {
                    if (pos >= result.Unit.End && result.Unit.Declarations.Count > 0)
                    {
                        return result.Unit.Declarations.Last();
                    }
                    else
                    {
                        return result.Unit.FindDeepest<NativeXDeclaration>(pos);
                    }
                }
                else
                {
                    return null;
                }
            }

            public virtual NativeXDeclaration GetEditingNode(NativeXEditingDeclarations container, TextPosition pos)
            {
                return container.FindDeepest<NativeXDeclaration>(pos);
            }

            public virtual void ResultUpdated()
            {
                TemporaryResultUpdated();
                this.Extension.Callback.TextEditorBox.RedrawContent(true, false);
            }

            public virtual void TemporaryResultUpdated()
            {
            }

            public virtual void AnalyzerResultReceived(NativeXAnalyzingResult result)
            {
            }
        }
    }
}
