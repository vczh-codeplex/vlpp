using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using ServiceConfigurations;
using NodeServiceHost.GuardService;
using NodeService.Endpoints;
using System.IO;
using System.Xml.Linq;
using System.Xml;
using ServiceUtility;

namespace DiskStorage
{
    class Program
    {
        static void Main(string[] args)
        {
            string serviceName = DiskStorageServiceConfiguration.EndpointName;

            Console.Title = serviceName;
            GuardServiceStarter<DiskStorageService>.LaunchService(
                typeof(Program).Assembly.Location,
                "",
                serviceName,
                DiskStorageServiceConfiguration.CreateFactory(),
                DiskStorageServiceConfiguration.ServerAddress,
                serviceName
                );
        }
    }

    [NodeEndpoint(DiskStorageServiceConfiguration.EndpointName)]
    public class DiskStorageService : StrongTypedNodeEndpoint, IDiskStorage
    {
        private static readonly string IndexPath = Path.GetDirectoryName(typeof(DiskStorageService).Assembly.Location) + "\\DiskStorageIndex.xml";
        private static readonly string DataPath = Path.GetDirectoryName(typeof(DiskStorageService).Assembly.Location) + "\\DataStorageContents";
        private static XElement indexXml = null;

        private static string GetDataFile(string guid)
        {
            return DataPath + "\\" + guid + ".txt";
        }

        public DiskStorageService()
        {
            this.EnableAsynchronization = true;
            lock (IndexPath)
            {
                if (!Directory.Exists(DataPath))
                {
                    Directory.CreateDirectory(DataPath);
                }

                try
                {
                    if (File.Exists(IndexPath))
                    {
                        indexXml = XElement.Load(IndexPath);
                        return;
                    }
                }
                catch (Exception)
                {
                }
                indexXml = new XElement("DiskStorageIndex");
                indexXml.Save(IndexPath);
            }
        }

        [NodeEndpointMethod]
        public void SetCache(string applicationName, string[] keyPath, string content, bool exceptionIfNotExists)
        {
            string[] keys = new string[] { applicationName }.Concat(keyPath).ToArray();
            string[] hexs = keys.Select(k => KeyPathGenerator.GenerateKey(k)).ToArray();
            lock (indexXml)
            {
                XElement currentElement = indexXml;
                for (int i = 0; i < keys.Length; i++)
                {
                    string hex = hexs[i];
                    XElement keyElement = currentElement.Element(hex);
                    if (keyElement == null)
                    {
                        if (exceptionIfNotExists)
                        {
                            throw new InvalidOperationException(i == 0 ? "Application not exists." : "Key path not exists.");
                        }
                        keyElement = new XElement(hex, new XElement("Key", new XCData(keys[i])));
                        currentElement.Add(keyElement);
                    }
                    currentElement = keyElement;
                }

                XElement contentElement = currentElement.Element("Content");
                if (contentElement == null)
                {
                    if (exceptionIfNotExists)
                    {
                        throw new InvalidOperationException(keyPath.Length == 0 ? "Application not exists." : "Key path not exists.");
                    }
                    contentElement = new XElement("Content", Guid.NewGuid().ToString());
                    currentElement.Add(contentElement);
                }

                string guid = contentElement.Value;
                File.WriteAllText(GetDataFile(guid), content);
                indexXml.Save(IndexPath);
            }
        }

        [NodeEndpointMethod]
        public string GetCache(string applicationName, string[] keyPath, bool exceptionIfNotExists)
        {
            string[] keys = new string[] { applicationName }.Concat(keyPath).ToArray();
            string[] hexs = keys.Select(k => KeyPathGenerator.GenerateKey(k)).ToArray();
            lock (indexXml)
            {
                XElement currentElement = indexXml;
                for (int i = 0; i < keys.Length; i++)
                {
                    string hex = hexs[i];
                    XElement keyElement = currentElement.Element(hex);
                    if (keyElement == null)
                    {
                        if (exceptionIfNotExists)
                        {
                            throw new InvalidOperationException(i == 0 ? "Application not exists." : "Key path not exists.");
                        }
                        return "";
                    }
                    currentElement = keyElement;
                }

                XElement contentElement = currentElement.Element("Content");
                if (contentElement == null)
                {
                    if (exceptionIfNotExists)
                    {
                        throw new InvalidOperationException(keyPath.Length == 0 ? "Application not exists." : "Key path not exists.");
                    }
                    return "";
                }

                string guid = contentElement.Value;
                return File.ReadAllText(GetDataFile(guid));
            }
        }
        [NodeEndpointMethod]

        public bool IsCacheExists(string applicationName, string[] keyPath)
        {
            string[] keys = new string[] { applicationName }.Concat(keyPath).ToArray();
            string[] hexs = keys.Select(k => KeyPathGenerator.GenerateKey(k)).ToArray();
            lock (indexXml)
            {
                XElement currentElement = indexXml;
                for (int i = 0; i < keys.Length; i++)
                {
                    string hex = hexs[i];
                    XElement keyElement = currentElement.Element(hex);
                    if (keyElement == null)
                    {
                        return false;
                    }
                    currentElement = keyElement;
                }

                XElement contentElement = currentElement.Element("Content");
                if (contentElement == null)
                {
                    return false;
                }

                return true;
            }
        }

        private void CollectKeyPaths(XElement currentElement, List<string> currentKeyPath, List<string[]> allKeyPaths)
        {
            if (currentElement.Element("Content") != null)
            {
                allKeyPaths.Add(currentKeyPath.ToArray());
            }
            foreach (var element in currentElement.Elements())
            {
                if (element.Name.LocalName.StartsWith("_"))
                {
                    currentKeyPath.Add(element.Element("Key").Value);
                    CollectKeyPaths(element, currentKeyPath, allKeyPaths);
                    currentKeyPath.RemoveAt(currentKeyPath.Count - 1);
                }
            }
        }

        [NodeEndpointMethod]
        public string[][] GetKeyPaths(string applicationName)
        {
            lock (indexXml)
            {
                string hex = KeyPathGenerator.GenerateKey(applicationName);
                XElement element = indexXml.Element(hex);
                if (element != null)
                {
                    List<string[]> allKeyPaths = new List<string[]>();
                    CollectKeyPaths(element, new List<string>(), allKeyPaths);
                    return allKeyPaths.ToArray();
                }
                else
                {
                    return new string[][] { };
                }
            }
        }
    }
}
