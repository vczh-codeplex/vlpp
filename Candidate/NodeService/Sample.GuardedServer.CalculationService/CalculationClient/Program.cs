using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using NodeService;
using NodeService.Protocols;

namespace CalculationClient
{
    class Program
    {
        static void Main(string[] args)
        {
            INodeEndpointProtocolFactory protocolFactory = new NamedPipeProtocolFactory();
            ICalculationService client = protocolFactory.WaitForClient<ICalculationService>("GuardedServiceTest", "CalculationService");
            Console.WriteLine("ICalculationService.Add(1, 2) == " + client.Add(1, 2).ToString());
            client.Dispose();
        }
    }

    public interface ICalculationService : INodeEndpointClient
    {
        int Add(int a, int b);
    }
}
