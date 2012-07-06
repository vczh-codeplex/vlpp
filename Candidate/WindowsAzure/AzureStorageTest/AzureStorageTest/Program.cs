using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using Microsoft.WindowsAzure;
using Microsoft.WindowsAzure.StorageClient;
using System.Threading;
using System.Data.Services.Common;

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
            for (int i = 0; i < 30; i++)
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

        class SquareRow : TableServiceEntity
        {
            public int Number { get; set; }
            public int Squared { get; set; }
        }

        class DoubleRow : TableServiceEntity
        {
            public int Number { get; set; }
            public int Doubled { get; set; }
        }

        static void DoTables(CloudStorageAccount account)
        {
            Console.WriteLine("=======================TABLE==========================");
            CloudTableClient tableClient = new CloudTableClient(account.TableEndpoint.AbsoluteUri, account.Credentials);
            TableServiceContext context = tableClient.GetDataServiceContext();

            Console.WriteLine("Clearning container...");
            tableClient.DeleteTableIfExist("AzureStorageTestTableFrom");
            tableClient.DeleteTableIfExist("AzureStorageTestTableTo");

            Console.WriteLine("Creating container...");
            while (true)
            {
                try
                {
                    tableClient.CreateTable("AzureStorageTestTableFrom");
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
                    tableClient.CreateTable("AzureStorageTestTableTo");
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
                SquareRow row = new SquareRow()
                {
                    Number = i,
                    Squared = i * i,
                    PartitionKey = "0",
                    RowKey = Guid.NewGuid().ToString(),
                };
                context.AddObject("AzureStorageTestTableFrom", row);
            }
            context.SaveChanges();

            Console.WriteLine("Checking data...");
            var query =
                from row in context.CreateQuery<SquareRow>("AzureStorageTestTableFrom")
                where row.Number < 10
                select row;
            foreach (var square in query)
            {
                DoubleRow row = new DoubleRow()
                {
                    Number = square.Squared,
                    Doubled = square.Squared * 2,
                    PartitionKey = "0",
                    RowKey = Guid.NewGuid().ToString(),
                };
                context.AddObject("AzureStorageTestTableTo", row);
            }
            context.SaveChanges();
        }
    }
}
