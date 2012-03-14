using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Xml.Linq;

namespace Xml2Doc
{
    static class DocItemProvider
    {
        static string ConvertDocNameToCppName(string name)
        {
            return name.Replace(".", "::");
        }

        static GacMethod TryPickFunction(GacMethod[] funcs, string methodParameters)
        {
            if (funcs.Length != 1)
            {
                return null;
            }
            return funcs[0];
        }

        static GacMethod PickFunction(GacMethod[] funcs, string methodParameters)
        {
            GacMethod method = TryPickFunction(funcs, methodParameters);
            if (method == null)
            {
                throw new ArgumentException();
            }
            return method;
        }

        static GacMethod TryPickFunction(GacUDT udt, GacMethod[] methods, string methodLocalName, string methodParameters)
        {
            var candidates1 = methods.Where(m => m.Name == udt.Name + "::" + methodLocalName).ToArray();
            var candidates2 = methods.Where(m => m.Name == methodLocalName).ToArray();
            var m1 = TryPickFunction(candidates1, methodParameters);
            var m2 = TryPickFunction(candidates2, methodParameters);
            return m1 ?? m2;
        }

        static GacMethod PickFunction(GacUDT udt, string methodLocalName, string methodParameters)
        {
            if (methodLocalName == "#ctor")
            {
                CppName cppName = CppName.Parse(udt.Name);
                methodLocalName = cppName.Cascade().Last().Name;
            }
            var m1 = TryPickFunction(udt, udt.Methods, methodLocalName, methodParameters);
            var m2 = TryPickFunction(udt, udt.StaticMethods, methodLocalName, methodParameters);
            if ((m1 == null) ^ (m2 == null) != true)
            {
                throw new ArgumentException();
            }
            return m1 ?? m2;
        }

        static GacField PickField(GacUDT udt, string fieldLocalName)
        {
            var fields =
                udt.Constants.Concat(udt.Fields).Concat(udt.StaticFields)
                .Where(f => f.Name == fieldLocalName)
                .ToArray();
            if (fields.Length != 1)
            {
                throw new ArgumentException();
            }
            return fields[0];
        }

        public static GacSymbol FindSymbol(XElement docItem, Dictionary<string, GacUDT> udts, Dictionary<string, GacMethod[]> funcs)
        {
            string name = docItem.Attribute("name").Value;
            string head = name.Substring(0, 2);
            name = name.Substring(2);
            switch (head)
            {
                case "T:":
                    {
                        string className = ConvertDocNameToCppName(name);
                        return udts[className];
                    }
                case "F:":
                    {
                        string typeName = "";
                        string fieldLocalName = "";
                        {
                            int pointIndex = name.LastIndexOf('.');
                            typeName = ConvertDocNameToCppName(name.Substring(0, pointIndex));
                            fieldLocalName = name.Substring(pointIndex + 1);
                        }

                        GacUDT udt = udts[typeName];
                        return PickField(udt, fieldLocalName);
                    }
                case "M:":
                    {
                        string methodName = name;
                        string methodParameters = "";
                        {
                            int bracketIndex = methodName.IndexOf('(');
                            if (bracketIndex != -1)
                            {
                                methodName = name.Substring(0, bracketIndex);
                                methodParameters = name.Substring(bracketIndex);
                            }
                        }

                        string containerName = "";
                        string methodLocalName = methodName;
                        {
                            int pointIndex = methodName.LastIndexOf('.');
                            if (pointIndex != -1)
                            {
                                containerName = methodName.Substring(0, pointIndex);
                                methodLocalName = methodName.Substring(pointIndex + 1);
                            }
                        }

                        if (containerName != "")
                        {
                            string className = ConvertDocNameToCppName(containerName);
                            GacUDT udt = null;
                            if (udts.TryGetValue(className, out udt))
                            {
                                return PickFunction(udt, methodLocalName, methodParameters);
                            }
                        }
                        string methodFullName = ConvertDocNameToCppName(methodName);
                        return PickFunction(funcs[methodFullName], methodParameters);
                    }
                default:
                    throw new ArgumentException();
            }
        }
    }
}
