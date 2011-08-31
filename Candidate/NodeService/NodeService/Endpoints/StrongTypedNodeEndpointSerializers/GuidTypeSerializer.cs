using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Xml.Linq;

namespace NodeService.Endpoints.StrongTypedNodeEndpointSerializers
{
    class GuidTypeSerializer : ITypedSerializer
    {
        public Type TargetType
        {
            get
            {
                return typeof(Guid);
            }
        }

        public XNode Serialize(object data)
        {
            return new XText(data.ToString());
        }

        public object Deserialize(XNode data, Type type)
        {
            return Guid.Parse(data.ToString());
        }
    }
}
