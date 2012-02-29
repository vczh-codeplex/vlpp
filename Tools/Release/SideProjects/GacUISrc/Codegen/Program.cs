﻿using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.IO;
using System.Xml.Linq;
using System.Text.RegularExpressions;

namespace Codegen
{
    class Program
    {
        static string[] GetCppFiles(string projectFile)
        {
            string np = @"http://schemas.microsoft.com/developer/msbuild/2003";
            XDocument document = XDocument.Load(projectFile);
            return document
                .Root
                .Elements(XName.Get("ItemGroup", np))
                .SelectMany(e => e.Elements(XName.Get("ClCompile", np)))
                .Select(e => Path.GetFullPath(Path.GetDirectoryName(projectFile) + "\\" + e.Attribute("Include").Value))
                .ToArray();
        }

        static Dictionary<string, string[]> CategorizeCodeFiles(XDocument config, string[] files)
        {
            Dictionary<string, string[]> categorizedFiles = new Dictionary<string, string[]>();
            foreach (var e in config.Root.Element("categories").Elements("category"))
            {
                string name = e.Attribute("name").Value;
                string pattern = e.Attribute("pattern").Value.ToUpper();
                string[] exceptions = e.Elements("except").Select(x => x.Attribute("filename").Value.ToUpper()).ToArray();
                categorizedFiles.Add(
                    name,
                    files
                        .Where(f => f.ToUpper().Contains(pattern))
                        .Where(f => !exceptions.Contains(Path.GetFileName(f).ToUpper()))
                        .ToArray()
                        );
            }
            foreach (var a in categorizedFiles.Keys)
            {
                foreach (var b in categorizedFiles.Keys)
                {
                    if (a != b)
                    {
                        if (categorizedFiles[a].Intersect(categorizedFiles[b]).Count() != 0)
                        {
                            throw new ArgumentException();
                        }
                    }
                }
            }
            return categorizedFiles;
        }

        static Dictionary<string, string[]> ScannedFiles = new Dictionary<string, string[]>();
        static Regex IncludeRegex = new Regex(@"^\s*\#include\s*""(?<path>[^""]+)""\s*$");
        static Regex IncludeSystemRegex = new Regex(@"^\s*\#include\s*\<(?<path>[^""]+)\>\s*$");

        static string[] GetIncludedFiles(string codeFile)
        {
            codeFile = Path.GetFullPath(codeFile).ToUpper();
            string[] result = null;
            if (!ScannedFiles.TryGetValue(codeFile, out result))
            {
                List<string> directIncludeFiles = new List<string>();
                foreach (var line in File.ReadAllLines(codeFile))
                {
                    Match match = IncludeRegex.Match(line);
                    if (match.Success)
                    {
                        string path = match.Groups["path"].Value;
                        path = Path.GetFullPath(Path.GetDirectoryName(codeFile) + @"\" + path).ToUpper();
                        if (!directIncludeFiles.Contains(path))
                        {
                            directIncludeFiles.Add(path);
                        }
                    }
                }

                for (int i = directIncludeFiles.Count - 1; i >= 0; i--)
                {
                    directIncludeFiles.InsertRange(i, GetIncludedFiles(directIncludeFiles[i]));
                }
                result = directIncludeFiles.Distinct().ToArray();
                ScannedFiles.Add(codeFile, result);
            }
            return result;
        }

        static string[] SortDependecies(Dictionary<string, string[]> dependeicies)
        {
            var dep = dependeicies.ToDictionary(p => p.Key, p => new HashSet<string>(p.Value));
            List<string> sorted = new List<string>();
            while (dep.Count > 0)
            {
                bool found = false;
                foreach (var p in dep)
                {
                    if (p.Value.Count == 0)
                    {
                        found = true;
                        sorted.Add(p.Key);
                        foreach (var q in dep.Values)
                        {
                            q.Remove(p.Key);
                        }
                        dep.Remove(p.Key);
                        break;
                    }
                }
                if (!found)
                {
                    throw new ArgumentException();
                }
            }
            return sorted.ToArray();
        }

        static void Combine(string[] files, string outputFilename, HashSet<string> systemIncludes, params string[] externalIncludes)
        {
            try
            {
                using (StreamWriter writer = new StreamWriter(new FileStream(outputFilename, FileMode.Create), Encoding.Default))
                {
                    writer.WriteLine("/***********************************************************************");
                    writer.WriteLine("THIS FILE IS AUTOMATICALLY GENERATED. DO NOT MODIFY");
                    writer.WriteLine("DEVELOPER: 陈梓瀚(vczh)");
                    writer.WriteLine("***********************************************************************/");
                    foreach (var inc in externalIncludes)
                    {
                        writer.WriteLine("#include \"{0}\"", inc);
                    }

                    foreach (var file in files)
                    {
                        writer.WriteLine("");
                        writer.WriteLine("/***********************************************************************");
                        writer.WriteLine(file);
                        writer.WriteLine("***********************************************************************/");
                        foreach (var line in File.ReadAllLines(file, Encoding.Default))
                        {
                            Match match = null;

                            match = IncludeSystemRegex.Match(line);
                            if (match.Success)
                            {
                                if (systemIncludes.Add(match.Groups["path"].Value.ToUpper()))
                                {
                                    writer.WriteLine(line);
                                }
                            }
                            else
                            {
                                match = IncludeRegex.Match(line);
                                if (!match.Success)
                                {
                                    writer.WriteLine(line);
                                }
                            }
                        }
                    }
                }
                Console.WriteLine("Succeeded to write: {0}", outputFilename);
            }
            catch (Exception)
            {
                Console.WriteLine("Failed to write: {0}", outputFilename);
            }
        }

        static void Combine(string inputFilename, string outputFilename, params string[] externalIncludes)
        {
            HashSet<string> systemIncludes = new HashSet<string>();
            string[] files = GetIncludedFiles(inputFilename).Concat(new string[] { inputFilename }).Distinct().ToArray();
            Combine(files, outputFilename, systemIncludes, externalIncludes);
        }

        static void Main(string[] args)
        {
            // load configuration
            XDocument config = XDocument.Load("CodegenConfig.xml");
            string folder = Path.GetDirectoryName(typeof(Program).Assembly.Location) + "\\";

            // collect project files
            string[] projectFiles = config.Root
                .Element("projects")
                .Elements("project")
                .Select(e => Path.GetFullPath(folder + e.Attribute("path").Value))
                .ToArray();

            // collect code files
            string[] unprocessedCppFiles = projectFiles.SelectMany(GetCppFiles).Distinct().ToArray();
            string[] unprocessedHeaderFiles = unprocessedCppFiles.SelectMany(GetIncludedFiles).Distinct().ToArray();

            // categorize code files
            var categorizedCppFiles = CategorizeCodeFiles(config, unprocessedCppFiles);
            var categorizedHeaderFiles = CategorizeCodeFiles(config, unprocessedHeaderFiles);
            var outputFolder = Path.GetFullPath(folder + config.Root.Element("output").Attribute("path").Value);
            var categorizedOutput = config.Root
                .Element("output")
                .Elements("codepair")
                .ToDictionary(
                    e => e.Attribute("category").Value,
                    e => Path.GetFullPath(outputFolder + e.Attribute("filename").Value
                    ));

            // calculate category dependencies
            var categoryDependencies = categorizedCppFiles
                .Keys
                .Select(k =>
                    {
                        var headerFiles = categorizedCppFiles[k]
                            .SelectMany(GetIncludedFiles)
                            .Distinct()
                            .ToArray();
                        var keys = categorizedHeaderFiles
                            .Where(p => p.Value.Any(h => headerFiles.Contains(h)))
                            .Select(p => p.Key)
                            .Except(new string[] { k })
                            .ToArray();
                        return Tuple.Create(k, keys);
                    })
                .ToDictionary(t => t.Item1, t => t.Item2);

            // sort categories by dependencies
            var categoryOrder = SortDependecies(categoryDependencies);
            Dictionary<string, HashSet<string>> categorizedSystemIncludes = new Dictionary<string, HashSet<string>>();

            // generate code pair header files
            foreach (var c in categoryOrder)
            {
                string output = categorizedOutput[c] + ".h";
                List<string> includes = new List<string>();
                foreach (var dep in categoryDependencies[c])
                {
                    includes.AddRange(categorizedSystemIncludes[dep]);
                }
                HashSet<string> systemIncludes = new HashSet<string>(includes.Distinct());
                categorizedSystemIncludes.Add(c, systemIncludes);
                Combine(
                    categorizedHeaderFiles[c],
                    output,
                    systemIncludes,
                    categoryDependencies[c]
                        .Select(d => Path.GetFileName(categorizedOutput[d] + ".h"))
                        .ToArray()
                    );
            }

            // generate code pair cpp files
            foreach (var c in categoryOrder)
            {
                string output = categorizedOutput[c];
                string outputHeader = Path.GetFileName(output + ".h");
                string outputCpp = output + ".cpp";
                HashSet<string> systemIncludes = categorizedSystemIncludes[c];
                Combine(
                    categorizedCppFiles[c],
                    outputCpp,
                    systemIncludes,
                    outputHeader
                    );
            }

            // generate header files
            var headerOutput = config.Root
                .Element("output")
                .Elements("header")
                .ToDictionary(
                    e => Path.GetFullPath(folder + e.Attribute("source").Value),
                    e => Path.GetFullPath(outputFolder + e.Attribute("filename").Value)
                    );
            foreach (var o in headerOutput)
            {
                Combine(o.Key, o.Value + ".h");
            }
        }
    }
}
