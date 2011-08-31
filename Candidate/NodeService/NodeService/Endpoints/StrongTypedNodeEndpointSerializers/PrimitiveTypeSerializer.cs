using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Xml.Linq;

namespace NodeService.Endpoints.StrongTypedNodeEndpointSerializers
{
    class PrimitiveTypeSerializer<T> : ITypedSerializer
    {
        public Type TargetType
        {
            get
            {
                return typeof(T);
            }
        }

        public XNode Serialize(object data)
        {
            return new XText((string)Convert.ChangeType(data, typeof(string)));
        }

        public object Deserialize(XNode data, Type type)
        {
            string stringValue = data == null ? "" : (string)((XText)data).Value;
            return Convert.ChangeType(stringValue, typeof(T));
        }
    }
}
