using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using Developer.WinFormControls;
using Developer.LanguageProvider;
using Developer.LanguageServices.NativeX.SyntaxTree;

namespace Developer.LanguageServices.NativeX.Extension
{
    public class NativeXAnalyzingResult
    {
        public Dictionary<int, List<CodeToken>> IdTokens { get; set; }
        public NativeXUnit Unit { get; set; }
    }

    public interface INativeXPredefinedHeaderReader
    {
        IEnumerable<NativeXUnit> PredifinedHeaders { get; }
    }

    public class NativeXCodeAnalyzer : CalculationNotifier<string, NativeXAnalyzingResult>
    {
        private INativeXPredefinedHeaderReader headers = null;

        public NativeXCodeAnalyzer(INativeXPredefinedHeaderReader headers)
        {
            this.headers = headers;
        }

        protected override NativeXAnalyzingResult Calculate(string input)
        {
            NativeXAnalyzingResult result = new NativeXAnalyzingResult();
            result.IdTokens = new Dictionary<int, List<CodeToken>>();

            List<CodeToken> tokens = NativeXCodeParser.Tokenize(input.ToCharArray());
            int lastRow = -1;
            List<CodeToken> lastLines = null;
            for (int i = 0; i < tokens.Count; i++)
            {
                CodeToken token = tokens[i];
                if (token.Id == NativeXTokenizer.IdToken)
                {
                    int row = token.Start.row;
                    if (row != lastRow)
                    {
                        lastLines = new List<CodeToken>();
                        result.IdTokens.Add(row, lastLines);
                        lastRow = row;
                    }
                    lastLines.Add(token);
                }
            }

            int currentToken = 0;
            bool parseSuccess = false;
            result.Unit = NativeXCodeParser.ParseUnit(tokens, ref currentToken, ref parseSuccess);
            if (result.Unit != null)
            {
                result.Unit.BuildScope(null);
            }
            result.Unit.OwningScope.ExtraScopes = this.headers.PredifinedHeaders.Select(u => u.OwningScope).ToArray();
            return result;
        }
    }
}
