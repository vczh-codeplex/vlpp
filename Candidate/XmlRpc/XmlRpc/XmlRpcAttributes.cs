using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;

namespace XmlRpc
{
    [AttributeUsage(AttributeTargets.Interface)]
    public class XmlRpcService : Attribute
    {
        public string Url { get; set; }

        public XmlRpcService(string url = "")
        {
            this.Url = url;
        }
    }

    [AttributeUsage(AttributeTargets.Method)]
    public class XmlRpcMethod : Attribute
    {
        public string Name { get; set; }

        public XmlRpcMethod(string name)
        {
            this.Name = name;
        }
    }

    [AttributeUsage(AttributeTargets.Class)]
    public class XmlRpcStruct : Attribute
    {
    }

    [AttributeUsage(AttributeTargets.Property)]
    public class XmlRpcMember : Attribute
    {
    }
}
