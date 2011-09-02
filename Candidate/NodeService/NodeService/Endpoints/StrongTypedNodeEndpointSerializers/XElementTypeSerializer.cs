using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Xml.Linq;

namespace NodeService.Endpoints.StrongTypedNodeEndpointSerializers
{
    class XElementTypeSerializer : INamedSerializer
    {
        public string ElementName
        {
            get
            {
                return "XElement";
            }
        }

        public Type TargetType
        {
            get
            {
                return typeof(XElement);
            }
        }

        public XNode Serialize(object data)
        {
            return new XElement(this.ElementName, (XElement)data);
        }

        public object Deserialize(XNode data, Type type)
        {
            return ((XElement)data).FirstNode;
        }
    }
}
