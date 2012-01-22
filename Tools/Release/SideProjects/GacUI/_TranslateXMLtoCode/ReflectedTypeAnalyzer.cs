using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;

namespace _TranslateXMLtoCode
{
    static class ReflectedTypeAnalyzer
    {
        static void FillUDT(ReflectedTypeAnalyzerInput input, Dictionary<string, RgacUDT> udts, RgacUDT udt)
        {
        }

        static IEnumerable<string> GetSuggestedName(CppName cppName)
        {
            while (cppName != null)
            {
                switch (cppName.Name)
                {
                    case "int":
                    case "wchar_t":
                        yield return cppName.Name;
                        break;
                    default:
                        if (char.IsUpper(cppName.Name[0]))
                        {
                            if (cppName.Parameters.Length > 0)
                            {
                                string parameterName = GetSuggestedName(cppName.Parameters[0])
                                        .Select(n => "_" + n)
                                        .Aggregate((a, b) => a + b)
                                        ;
                                if (cppName.Name == "Ptr")
                                {
                                    yield return parameterName.Substring(1);
                                }
                                else
                                {
                                    yield return cppName.Name + "_of" + parameterName;
                                }
                            }
                            else
                            {
                                yield return cppName.Name;
                            }
                        }
                        break;
                }
                cppName = cppName.Member;
            }
        }

        static RgacUDT CreateUDT(ReflectedTypeAnalyzerInput input, GacUDT udt)
        {
            RgacUDT rudt = new RgacUDT();
            rudt.CppName = CppName.Parse(udt.Name);
            rudt.Name = GetSuggestedName(rudt.CppName).ToArray();
            rudt.IsAbstract = udt.IsAbstract;
            rudt.AssociatedGacType = udt;
            rudt.Descriptable = false;
            if (input.ExportableEnums.ContainsKey(udt.Name))
            {
                rudt.Kind = RgacUDTKind.Enum;
            }
            else if (input.ExportableStructs.ContainsKey(udt.Name))
            {
                rudt.Kind = RgacUDTKind.Struct;
            }
            else
            {
                rudt.Descriptable = input.DescriptableUdts.ContainsKey(udt.Name);
                rudt.Kind = rudt.Name.Last()[0] == 'I' ? RgacUDTKind.Interface : RgacUDTKind.Class;
            }
            return rudt;
        }

        public static ReflectedTypeAnalyzerResult Analyze(ReflectedTypeAnalyzerInput input)
        {
            Dictionary<string, RgacUDT> udts =
                input.DescriptableUdts.Values.Concat(
                input.ExportableClasses.Values.Concat(
                input.ExportableEnums.Values.Concat(
                input.ExportableStructs.Values
                ))).Select(t => CreateUDT(input, t)).ToDictionary(t => t.AssociatedGacType.Name);

            foreach (var t in udts.Values)
            {
                FillUDT(input, udts, t);
            }

            var result = new ReflectedTypeAnalyzerResult
            {
                Udts = udts.Values.OrderBy(t => t.ToString()).ToArray(),
            };

            var duplicates = udts.Values.GroupBy(t => t.ToString()).Where(g => g.Count() > 1).ToArray();
            if (duplicates.Length > 0)
            {
                throw new ArgumentException();
            }
            return result;
        }
    }

    class CppName
    {
        public string Name { get; set; }
        public CppName[] Parameters { get; set; }
        public CppName Member { get; set; }

        private static string ParseName(string name, ref int index)
        {
            string result = "";
            while (index < name.Length)
            {
                bool normalChar = true;
                switch (name[index])
                {
                    case '<':
                    case '>':
                    case ',':
                    case ':':
                        normalChar = false;
                        break;
                }
                if (!normalChar) break;
                result += name[index];
                index++;
            }
            if (string.IsNullOrWhiteSpace(result))
            {
                throw new ArgumentException();
            }
            return result.Trim();
        }

        private static bool ParseChar(string name, char c, ref int index)
        {
            while (index < name.Length && char.IsWhiteSpace(name[index]))
            {
                index++;
            }
            if (index < name.Length && name[index] == c)
            {
                index++;
                return true;
            }
            else
            {
                return false;
            }
        }

        private static CppName ParseCppName(string name, ref int index)
        {
            CppName result = new CppName();
            result.Name = ParseName(name, ref index);
            result.Parameters = new CppName[0];
            if (ParseChar(name, '<', ref index))
            {
                List<CppName> parameters = new List<CppName>();
                while (true)
                {
                    parameters.Add(ParseCppName(name, ref index));
                    if (ParseChar(name, '>', ref index))
                    {
                        break;
                    }
                    else if (!ParseChar(name, ',', ref index))
                    {
                        throw new ArgumentException();
                    }
                }
                result.Parameters = parameters.ToArray();
            }
            if (ParseChar(name, ':', ref index))
            {
                if (!ParseChar(name, ':', ref index))
                {
                    throw new ArgumentException();
                }
                result.Member = ParseCppName(name, ref index);
            }
            return result;
        }

        public static CppName Parse(string name)
        {
            int index = 0;
            CppName result = ParseCppName(name.Trim(), ref index);
            if (index != name.Length)
            {
                throw new ArgumentException();
            }
            return result;
        }

        public override string ToString()
        {
            return Name
                + (Parameters.Length == 0 ? "" : "<" + Parameters.Select(n => n.ToString()).Aggregate("", (a, b) => a == "" ? b : a + ", " + b) + ">")
                + (Member == null ? "" : " :: " + Member.ToString())
                ;
        }
    }

    class ReflectedTypeAnalyzerInput
    {
        public Dictionary<string, GacUDT> DescriptableUdts { get; set; }
        public Dictionary<string, GacUDT> ExportableClasses { get; set; }
        public Dictionary<string, GacUDT> ExportableStructs { get; set; }
        public Dictionary<string, GacUDT> ExportableEnums { get; set; }
        public Dictionary<string, GacUDT> AvailableUdts { get; set; }
    }

    class ReflectedTypeAnalyzerResult
    {
        public RgacUDT[] Udts { get; set; }
    }
}
