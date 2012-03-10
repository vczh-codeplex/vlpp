using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Xml.Linq;

namespace Xml2Doc
{
    class DocFunction
    {
        public string Name { get; set; }
        public GacMethod Method { get; set; }
        public XElement MethodDocItem { get; set; }
    }

    class DocItemContainer
    {
        public Dictionary<string, DocType> Types { get; private set; }
        public Dictionary<string, List<DocFunction>> Functions { get; private set; }

        public DocItemContainer()
        {
            this.Types = new Dictionary<string, DocType>();
            this.Functions = new Dictionary<string, List<DocFunction>>();
        }

        public DocType GetType(string name)
        {
            DocType type = null;
            if (!this.Types.TryGetValue(name, out type))
            {
                type = new DocType() { Name = name };
                this.Types.Add(type.Name, type);
            }
            return type;
        }

        public void AddFunction(DocFunction docFunction)
        {
            List<DocFunction> functions = null;
            if (!this.Functions.TryGetValue(docFunction.Name, out functions))
            {
                functions = new List<DocFunction>();
                this.Functions.Add(docFunction.Name, functions);
            }
            functions.Add(docFunction);
        }
    }

    class DocBaseType
    {
        public DocType Type { get; set; }
        public GacBaseClass BaseClass { get; set; }
    }

    class DocType : DocItemContainer
    {
        public string Name { get; set; }
        public DocBaseType[] BaseTypes { get; set; }
        public GacUDT Udt { get; set; }
        public XElement UdtDocItem { get; set; }
    }

    class DocNamespace : DocItemContainer
    {
        public string Name { get; set; }

        public Dictionary<string, DocNamespace> Namespaces { get; private set; }

        public DocNamespace()
        {
            this.Name = "";
            this.Namespaces = new Dictionary<string, DocNamespace>();
        }

        public DocNamespace GetNamespace(IEnumerable<string> namespaces)
        {
            string first = namespaces.FirstOrDefault();
            if (first == null)
            {
                return this;
            }
            else
            {
                DocNamespace ns = null;
                if (!this.Namespaces.TryGetValue(first, out ns))
                {
                    ns = new DocNamespace() { Name = first };
                    this.Namespaces.Add(ns.Name, ns);
                }
                return ns.GetNamespace(namespaces.Skip(1));
            }
        }
    }

    class DocRootNamespace : DocNamespace
    {
        private Dictionary<GacUDT, DocType> typeCache = new Dictionary<GacUDT, DocType>();

        public DocType GetType(GacUDT udt, Dictionary<string, GacUDT> udts)
        {
            DocType resultType = null;
            if (!this.typeCache.TryGetValue(udt, out resultType))
            {
                CppName cppName = CppName.Parse(udt.Name);
                var cascadeNames = cppName
                    .Cascade()
                    .Select(c => new CppName()
                        {
                            Name = c.Name,
                            Parameters = c.Parameters,
                        }.ToPdbString())
                    .ToArray();

                string currentName = "";
                int namespaceCount = 0;
                for (int i = 0; i < cascadeNames.Length; i++)
                {
                    currentName += (i == 0 ? "" : "::") + cascadeNames[i];
                    if (udts.ContainsKey(currentName)) break;
                    namespaceCount = i + 1;
                }

                DocItemContainer ic = GetNamespace(cascadeNames.Take(namespaceCount));
                for (int i = namespaceCount; i < cascadeNames.Length; i++)
                {
                    ic = ic.GetType(cascadeNames[i]);
                }
                resultType = (DocType)ic;
                resultType.Udt = udt;
            }
            return resultType;
        }
    }

    static class DocItemSerializer
    {
        static void FillFunction(DocFunction docFunction, Tuple<XElement, GacMethod> method)
        {
            docFunction.Name = CppName.Parse(method.Item2.Name).Cascade().Last().Name;
            docFunction.Method = method.Item2;
            docFunction.MethodDocItem = method.Item1;
        }

        static DocFunction BuildFunction(DocItemContainer ic, Tuple<XElement, GacMethod> method)
        {
            DocFunction docFunction = new DocFunction();
            FillFunction(docFunction, method);
            ic.AddFunction(docFunction);
            return docFunction;
        }

        static void FillType(DocType type, KeyValuePair<GacUDT, Tuple<XElement, Tuple<XElement, GacMethod>[]>> info, DocRootNamespace root, Dictionary<string, GacUDT> udts, HashSet<GacUDT> hashUdts)
        {
            type.UdtDocItem = info.Value.Item1;
            type.BaseTypes = info.Key
                .BaseClasses
                .Where(b => b.Access != GacAccess.Private)
                .Where(b => hashUdts.Contains(b.UDT))
                .Select(b => new DocBaseType
                {
                    Type = root.GetType(b.UDT, udts),
                    BaseClass = b,
                })
                .ToArray();
            foreach (var m in info.Value.Item2)
            {
                BuildFunction(type, m);
            }
        }

        static DocRootNamespace Categorize(Dictionary<GacUDT, Tuple<XElement, Tuple<XElement, GacMethod>[]>> typeGroupedItems, Tuple<XElement, GacMethod>[] gfuncItems, Dictionary<string, GacUDT> udts)
        {
            HashSet<GacUDT> hashUdts = new HashSet<GacUDT>(udts.Values);
            DocRootNamespace root = new DocRootNamespace();
            foreach (var p in typeGroupedItems)
            {
                DocType type = root.GetType(p.Key, udts);
                FillType(type, p, root, udts, hashUdts);
            }
            return root;
        }

        public static XElement Serialize(Dictionary<string, Tuple<XElement, GacSymbol>> docItems, Dictionary<string, GacUDT> udts, Dictionary<string, GacMethod[]> funcs)
        {
            var typeItems = docItems.Values
                .Select(v => Tuple.Create(v.Item1, v.Item2 as GacUDT))
                .Where(v => v.Item2 != null)
                .ToDictionary(v => v.Item2, v => v.Item1);
            var funcItems = docItems.Values
                .Select(v => Tuple.Create(v.Item1, v.Item2 as GacMethod))
                .Where(v => v.Item2 != null)
                .ToArray();
            var gfuncItems = funcItems
                .Where(v => v.Item2.OwnerUDT == null)
                .ToArray();
            var mfuncItems = funcItems
                .Where(v => v.Item2.OwnerUDT != null)
                .GroupBy(v => v.Item2.OwnerUDT)
                .ToDictionary(g => g.Key, g => g.ToArray());

            var typeGroupedItems =
                typeItems.Keys.Concat(mfuncItems.Keys).Distinct()
                .ToDictionary(t => t, t =>
                    {
                        XElement typeDocItem = null;
                        typeItems.TryGetValue(t, out typeDocItem);
                        Tuple<XElement, GacMethod>[] mfuncDocItems = null;
                        mfuncItems.TryGetValue(t, out mfuncDocItems);
                        return Tuple.Create(typeDocItem, mfuncDocItems ?? new Tuple<XElement, GacMethod>[0]);
                    });

            var docAssembly = Categorize(typeGroupedItems, gfuncItems, udts);

            XElement root = new XElement("cppdoc");
            return root;
        }
    }
}
