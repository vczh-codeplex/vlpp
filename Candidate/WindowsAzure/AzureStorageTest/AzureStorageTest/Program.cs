using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using Microsoft.WindowsAzure;
using Microsoft.WindowsAzure.StorageClient;
using System.Threading;

namespace AzureStorageTest
{
    class Program
    {
        static void Main(string[] args)
        {
            CloudStorageAccount account = CloudStorageAccount.Parse("UseDevelopmentStorage=true");
            DoBlobs(account);
            DoQueues(account);
            DoTables(account);
            Console.WriteLine("=======================FINISHED=======================");
            Console.ReadLine();
        }

        static void DoBlobs(CloudStorageAccount account)
        {
            Console.WriteLine("=======================BLOB===========================");
            CloudBlobClient blobClient = new CloudBlobClient(account.BlobEndpoint.AbsoluteUri, account.Credentials);

            Console.WriteLine("Clearning container...");
            var container = blobClient.GetContainerReference("azure-storage-test-blob");
            try
            {
                container.Delete();
            }
            catch (Exception)
            {
            }

            Console.WriteLine("Creating container...");
            while (true)
            {
                try
                {
                    container.CreateIfNotExist();
                    break;
                }
                catch (Exception)
                {
                    Thread.Sleep(1000);
                }
            }

            Console.WriteLine("Uploading data...");
            for (int i = 0; i < 30; i++)
            {
                string key = "blob-" + i.ToString();
                string value = "value-" + i.ToString();
                CloudBlob blobRef = container.GetBlobReference(key);
                blobRef.UploadText(value);
                blobRef.UploadText(value);
            }

            Console.WriteLine("Checking data...");
            foreach (var blob in container.ListBlobs())
            {
                CloudBlob blobRef = container.GetBlobReference(blob.Uri.ToString());
                Console.WriteLine(blobRef.Name + " = " + blobRef.DownloadText());
            }
        }

        static void DoQueues(CloudStorageAccount account)
        {
            Console.WriteLine("=======================QUEUE==========================");
            CloudQueueClient queueClient = new CloudQueueClient(account.QueueEndpoint.AbsoluteUri, account.Credentials);

            Console.WriteLine("Clearning container...");
            var containerIn = queueClient.GetQueueReference("azure-storage-test-queue-in");
            var containerOut = queueClient.GetQueueReference("azure-storage-test-queue-out");
            try
            {
                containerIn.Delete();
            }
            catch (Exception)
            {
            }
            try
            {
                containerOut.Delete();
            }
            catch (Exception)
            {
            }

            Console.WriteLine("Creating container...");
            while (true)
            {
                try
                {
                    containerIn.CreateIfNotExist();
                    break;
                }
                catch (Exception)
                {
                    Thread.Sleep(1000);
                }
            }
            while (true)
            {
                try
                {
                    containerOut.CreateIfNotExist();
                    break;
                }
                catch (Exception)
                {
                    Thread.Sleep(1000);
                }
            }

            Console.WriteLine("Uploading data...");
            for (int i = 0; i<30; i++)
            {
                containerIn.AddMessage(new CloudQueueMessage("queue message " + i.ToString()));
            }

            for (int i = 0; i < 10; i++)
            {
                CloudQueueMessage message = containerIn.GetMessage();
                containerOut.AddMessage(message);
            }

            Console.WriteLine("Checking data...");
            CloudQueueMessage[] messages = containerIn.PeekMessages(30).ToArray();
            foreach (var message in messages)
            {
                Console.WriteLine("azure-storage-test-queue-in: " + message.AsString);
            }
            messages = containerOut.PeekMessages(30).ToArray();
            foreach (var message in messages)
            {
                Console.WriteLine("azure-storage-test-queue-out: " + message.AsString);
            }
        }

        static void DoTables(CloudStorageAccount account)
        {
            Console.WriteLine("=======================TABLE==========================");
        }
    }
}
