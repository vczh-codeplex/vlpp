using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using Developer.WinFormControls;
using Developer.LanguageProvider;
using Developer.LanguageServices.NativeX.SyntaxTree;

namespace Developer.LanguageServices.NativeX
{
    public class NativeXAnalyzingResult
    {
        public List<CodeToken> Tokens { get; set; }
        public NativeXUnit Unit { get; set; }
    }

    public class NativeXCodeAnalyzer : CalculationNotifier<string, NativeXAnalyzingResult>
    {
        protected override NativeXAnalyzingResult Calculate(string input)
        {
            NativeXAnalyzingResult result = new NativeXAnalyzingResult();
            result.Tokens = NativeXCodeParser.Tokenize(input.ToCharArray());
            int currentToken = 0;
            bool parseSuccess = false;
            result.Unit = NativeXCodeParser.ParseUnit(result.Tokens, ref currentToken, ref parseSuccess);
            return result;
        }
    }
}
