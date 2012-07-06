using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using Microsoft.WindowsAzure;

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
        }

        static void DoQueues(CloudStorageAccount account)
        {
            Console.WriteLine("=======================QUEUE==========================");
        }

        static void DoTables(CloudStorageAccount account)
        {
            Console.WriteLine("=======================TABLE==========================");
        }
    }
}
