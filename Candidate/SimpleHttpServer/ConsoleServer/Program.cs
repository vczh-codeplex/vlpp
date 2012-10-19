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
        [Route("/index.htm")]
        public string Index()
        {
            return @"
<html>
<head><title>Vczh's Website</title></head>
<body>
<p><strong>Welcome to Vczh's Website.</strong></p>
<p><a href=""./hello/vczh.htm"">vczh</a></p>
<p><a href=""./hello/JJLee.htm"">JJLee</a></p>
</body>
</html>
";
        }

        [Route("/hello/{name}.htm")]
        public string Hello(string name)
        {
            string html = @"
<html>
<head><title>Vczh's Website</title></head>
<body>
Hello, <strong>{0}</strong>.
<a href=""../index.htm"">Home page</a>
</body>
</html>
";
            return string.Format(html, name);
        }

        static void Main(string[] args)
        {
            SimpleHttpServerHost.Run(typeof(Program), host: "localhost", port: 80);
            Console.WriteLine("Press [ENTER] to exit.");
            Console.ReadLine();
        }
    }
}
