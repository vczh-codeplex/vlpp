using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using SimpleHttpServer;

namespace ConsoleServer
{
    class Program
    {
        [Route("/hello/{name1},{name2}")]
        public string Index(string name2, string name1)
        {
            return string.Format("<b>Hello {0} - {1}!</b>", name1, name2);
        }

        static void Main(string[] args)
        {
            SimpleHttpServerHost.Run(typeof(Program), host: "localhost", port: 80);
            Console.WriteLine("Press [ENTER] to exit.");
            Console.ReadLine();
        }
    }
}
