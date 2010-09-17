using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Reflection;
using System.CodeDom;
using Microsoft.CSharp;
using System.CodeDom.Compiler;
using System.IO;

namespace CodeBoxControl.CodeProvider
{
    static class CodeDomHelper
    {
        public static Assembly Compile(CodeCompileUnit unit, params string[] references)
        {
            CodeDomProvider provider = new CSharpCodeProvider();
            CompilerResults results = null;
            {
                CompilerParameters options = new CompilerParameters();
                options.GenerateExecutable = false;
                options.GenerateInMemory = true;
                options.IncludeDebugInformation = false;
                options.ReferencedAssemblies.AddRange(references);
                results = provider.CompileAssemblyFromDom(options, unit);
            }
            if (results.Errors.HasErrors)
            {
                string message = "";
                for (int i = 0; i < results.Errors.Count; i++)
                {
                    CompilerError error = results.Errors[i];
                    message += "Line:" + error.Line.ToString() + "; Column:" + error.Column.ToString() + "; Message=" + error.ErrorText + "\r\n";
                }
                using (MemoryStream stream = new MemoryStream())
                using (StreamReader reader = new StreamReader(stream))
                using (StreamWriter writer = new StreamWriter(stream))
                {
                    CodeGeneratorOptions options = new CodeGeneratorOptions();
                    options.BlankLinesBetweenMembers = true;
                    options.BracingStyle = "C";
                    options.ElseOnClosing = false;
                    options.IndentString = "    ";
                    options.VerbatimOrder = true;
                    provider.GenerateCodeFromCompileUnit(unit, writer, options);
                    stream.Seek(0, SeekOrigin.Begin);
                    message = writer.ToString();
                }
                throw new InvalidOperationException(message);
            }
            else
            {
                return results.CompiledAssembly;
            }
        }
    }
}
