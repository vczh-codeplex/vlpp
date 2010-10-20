using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using Developer.WinFormControls;
using Developer.LanguageProvider;

namespace Developer.LanguageServices.NativeX
{
    public class NativeXAnalyzingResult
    {
        public List<CodeToken> Tokens { get; set; }
    }

    public interface INativeXAnalyzingResultReceiver
    {
        void Receive(NativeXAnalyzingResult result);
    }

    public class NativeXCodeAnalyzer : CalculationNotifier<string, NativeXAnalyzingResult>
    {
        private NativeXTokenizer tokenizer = new NativeXTokenizer();
        private INativeXAnalyzingResultReceiver receiver = null;

        public NativeXCodeAnalyzer(INativeXAnalyzingResultReceiver receiver)
        {
            this.receiver = receiver;
        }

        protected override NativeXAnalyzingResult Calculate(string input)
        {
            NativeXAnalyzingResult result = new NativeXAnalyzingResult();
            result.Tokens = this.tokenizer.Tokenize(input.ToCharArray());
            return result;
        }

        protected override void Receive(NativeXAnalyzingResult output)
        {
            this.receiver.Receive(output);
        }
    }
}
