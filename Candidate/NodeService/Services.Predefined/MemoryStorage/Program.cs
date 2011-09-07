using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using ServiceConfigurations;
using NodeServiceHost.GuardService;
using NodeService.Endpoints;
using NodeService;
using ServiceUtility;

namespace MemoryStorage
{
    class Program
    {
        static void Main(string[] args)
        {
            string serviceName = MemoryStorageServiceConfiguration.EndpointName;

            Console.Title = serviceName;
            GuardServiceStarter<MemoryStorageServive>.LaunchService(
                typeof(Program).Assembly.Location,
                "",
                serviceName,
                MemoryStorageServiceConfiguration.CreateFactory(),
                MemoryStorageServiceConfiguration.ServerAddress,
                serviceName
                );
        }
    }

    [NodeEndpoint(MemoryStorageServiceConfiguration.EndpointName)]
    public class MemoryStorageServive : StrongTypedNodeEndpoint, IMemoryStorage
    {
        private class StorageContent
        {
            public string[] KeyPath { get; set; }
            public string Content { get; set; }
        }

        private static Dictionary<string, Dictionary<string, StorageContent>> storages = new Dictionary<string, Dictionary<string, StorageContent>>();

        public MemoryStorageServive()
        {
            this.EnableAsynchronization = true;
        }

        [NodeEndpointMethod]
        public void SetCache(string applicationName, string[] keyPath, string content, bool exceptionIfNotExists)
        {
            string key = KeyPathGenerator.GenerateKey(keyPath);
            lock (storages)
            {
                Dictionary<string, StorageContent> storage = null;
                if (!storages.TryGetValue(applicationName, out storage))
                {
                    if (exceptionIfNotExists)
                    {
                        throw new InvalidOperationException("Application not exists.");
                    }
                    storage = new Dictionary<string, StorageContent>();
                    storages.Add(applicationName, storage);
                }

                StorageContent storageContent = null;
                if (!storage.TryGetValue(key, out storageContent))
                {
                    if (exceptionIfNotExists)
                    {
                        throw new InvalidOperationException("Key path not exists.");
                    }
                    storageContent = new StorageContent();
                    storageContent.KeyPath = keyPath;
                    storage.Add(key, storageContent);
                }

                storageContent.Content = content;
            }
        }

        [NodeEndpointMethod]
        public string GetCache(string applicationName, string[] keyPath, bool exceptionIfNotExists)
        {
            string key = KeyPathGenerator.GenerateKey(keyPath);
            lock (storages)
            {
                Dictionary<string, StorageContent> storage = null;
                if (!storages.TryGetValue(applicationName, out storage))
                {
                    if (exceptionIfNotExists)
                    {
                        throw new InvalidOperationException("Application not exists.");
                    }
                    else
                    {
                        return "";
                    }
                }

                StorageContent storageContent = null;
                if (!storage.TryGetValue(key, out storageContent))
                {
                    if (exceptionIfNotExists)
                    {
                        throw new InvalidOperationException("Key path not exists.");
                    }
                    else
                    {
                        return "";
                    }
                }

                return storageContent.Content;
            }
        }
        [NodeEndpointMethod]

        public bool IsCacheExists(string applicationName, string[] keyPath)
        {
            string key = KeyPathGenerator.GenerateKey(keyPath);
            lock (storages)
            {
                Dictionary<string, StorageContent> storage = null;
                if (!storages.TryGetValue(applicationName, out storage))
                {
                    return false;
                }

                return storage.ContainsKey(key);
            }
        }

        [NodeEndpointMethod]
        public string[][] GetKeyPaths(string applicationName)
        {
            lock (storages)
            {
                Dictionary<string, StorageContent> storage = null;
                if (!storages.TryGetValue(applicationName, out storage))
                {
                    return new string[][] { };
                }

                return storage.Values.Select(c => c.KeyPath).ToArray();
            }
        }
    }
}
