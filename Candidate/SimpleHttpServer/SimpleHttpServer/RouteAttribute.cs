using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace SimpleHttpServer
{
    [AttributeUsage(AttributeTargets.Method)]
    public class RouteAttribute : Attribute
    {
        public string RoutePath { get; set; }

        public RouteAttribute(string routePath)
        {
            this.RoutePath = routePath;
        }
    }
}
