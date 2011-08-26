using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Xml.Linq;

namespace NodeService
{
    public class StrongTypedNodeEndpointSerializer
    {
        public interface ITypedSerializer
        {
            Type TargetType { get; }

            XNode Serialize(object data);
            object Deserialize(XNode data, Type type);
        }

        public interface INamedSerializer : ITypedSerializer
        {
            string ElementName { get; }
        }

        private class PrimitiveTypeSerializer<T> : ITypedSerializer
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
                return Convert.ChangeType((string)((XText)data).Value, typeof(T));
            }
        }

        private Dictionary<Type, ITypedSerializer> typedSerializer = new Dictionary<Type, ITypedSerializer>();
        private Dictionary<string, INamedSerializer> namedSerializer = new Dictionary<string, INamedSerializer>();

        public StrongTypedNodeEndpointSerializer()
        {
            AddSerializer(new PrimitiveTypeSerializer<SByte>());
            AddSerializer(new PrimitiveTypeSerializer<Byte>());
            AddSerializer(new PrimitiveTypeSerializer<Int16>());
            AddSerializer(new PrimitiveTypeSerializer<UInt16>());
            AddSerializer(new PrimitiveTypeSerializer<Int32>());
            AddSerializer(new PrimitiveTypeSerializer<UInt32>());
            AddSerializer(new PrimitiveTypeSerializer<Int64>());
            AddSerializer(new PrimitiveTypeSerializer<UInt64>());
            AddSerializer(new PrimitiveTypeSerializer<Single>());
            AddSerializer(new PrimitiveTypeSerializer<Double>());
            AddSerializer(new PrimitiveTypeSerializer<Decimal>());
            AddSerializer(new PrimitiveTypeSerializer<String>());
            AddSerializer(new PrimitiveTypeSerializer<Char>());
            AddSerializer(new PrimitiveTypeSerializer<Boolean>());
        }

        public XNode Serialize(object data)
        {
            return this.typedSerializer[data.GetType()].Serialize(data);
        }

        public object Deserialize(XNode data, Type type)
        {
            XElement element = data as XElement;
            if (element != null)
            {
                INamedSerializer serializer = null;
                if (this.namedSerializer.TryGetValue(element.Name.LocalName, out serializer))
                {
                    return serializer.Deserialize(data, type);
                }
            }
            return this.typedSerializer[type].Deserialize(data, type);
        }

        public void AddSerializer(ITypedSerializer serializer)
        {
            this.typedSerializer[serializer.TargetType] = serializer;
        }

        public void AddSerializer(INamedSerializer serializer)
        {
            this.namedSerializer[serializer.ElementName] = serializer;
            this.typedSerializer[serializer.TargetType] = serializer;
        }

        public void AddDefaultSerializer(Type type)
        {
        }
    }
}
