using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using Developer.WinFormControls;
using Developer.LanguageProvider;
using Developer.LanguageServices.NativeX.SyntaxTree;
using Developer.LanguageServices.NativeX;

namespace Test.Host.LanguageForms.NativeX
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
            if (result.Unit != null)
            {
                result.Unit.BuildScope(null);
            }
            return result;
        }
    }
}
