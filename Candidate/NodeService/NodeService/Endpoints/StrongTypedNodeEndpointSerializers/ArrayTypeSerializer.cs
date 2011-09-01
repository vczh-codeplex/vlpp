using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Xml.Linq;
using System.Collections;

namespace NodeService.Endpoints.StrongTypedNodeEndpointSerializers
{
    class ArrayTypeSerializer : INamedSerializer
    {
        protected StrongTypedNodeEndpointSerializer serializer;

        public ArrayTypeSerializer(StrongTypedNodeEndpointSerializer serializer)
        {
            this.serializer = serializer;
        }

        public string ElementName
        {
            get
            {
                return "Array";
            }
        }

        public Type TargetType
        {
            get
            {
                return typeof(Array);
            }
        }

        public XNode Serialize(object data)
        {
            return this.serializer.SerializeFromArray(this.ElementName, (IEnumerable)data);
        }

        public object Deserialize(XNode data, Type type)
        {
            return this.serializer.DeserializeToArray(data, type.GetElementType());
        }
    }
}
