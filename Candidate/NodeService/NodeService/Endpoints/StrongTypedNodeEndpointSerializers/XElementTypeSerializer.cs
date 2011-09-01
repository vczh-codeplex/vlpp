using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Xml.Linq;

namespace NodeService.Endpoints.StrongTypedNodeEndpointSerializers
{
    class XElementTypeSerializer : ITypedSerializer
    {
        public Type TargetType
        {
            get
            {
                return typeof(XElement);
            }
        }

        public XNode Serialize(object data)
        {
            return (XElement)data;
        }

        public object Deserialize(XNode data, Type type)
        {
            return (XElement)data;
        }
    }
}
