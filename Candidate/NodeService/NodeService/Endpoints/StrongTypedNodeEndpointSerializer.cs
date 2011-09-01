using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Xml.Linq;
using NodeService.Endpoints.StrongTypedNodeEndpointSerializers;
using System.Reflection;
using System.Collections;

namespace NodeService.Endpoints
{
    public class StrongTypedNodeEndpointSerializer
    {
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
            AddSerializer(new GuidTypeSerializer());
            AddSerializer(new XElementTypeSerializer());
            AddSerializer(new ArrayTypeSerializer(this));
            AddSerializer(new ListTypeSerializer(this));
            AddSerializer(new HashSetTypeSerializer(this));
            AddSerializer(new LinkedListTypeSerializer(this));
            AddSerializer(new QueueTypeSerializer(this));
            AddSerializer(new SortedSetTypeSerializer(this));
            AddSerializer(new StackTypeSerializer(this));
            AddSerializer(new DictionaryTypeSerializer(this));
            AddSerializer(new SortedDictionaryTypeSerializer(this));
            AddSerializer(new SortedListTypeSerializer(this));
        }

        public XNode Serialize(object data)
        {
            Type dataType = data.GetType();
            if (dataType.IsArray)
            {
                return this.typedSerializer[typeof(Array)].Serialize(data);
            }
            else
            {
                Type serializerType = dataType.IsGenericType ? dataType.GetGenericTypeDefinition() : dataType;
                return this.typedSerializer[serializerType].Serialize(data);
            }
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
            Type serializerType = type.IsGenericType ? type.GetGenericTypeDefinition() : type;
            return this.typedSerializer[serializerType].Deserialize(data, type);
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

        private bool IsDataType(Type type)
        {
            return type.GetCustomAttributes(typeof(NodeEndpointDataTypeAttribute), false).Length > 0;
        }

        public void AddDefaultSerializer(Type type)
        {
            if (type.IsArray)
            {
                AddDefaultSerializer(type.GetElementType());
            }
            else if (!this.typedSerializer.ContainsKey(type))
            {
                if (IsDataType(type))
                {
                    new DataTypeSerializer(this, type);
                    foreach (NodeEndpointKnownTypeAttribute att in type.GetCustomAttributes(typeof(NodeEndpointKnownTypeAttribute), false))
                    {
                        AddDefaultSerializer(att.KnownType);
                    }
                    Type currentType = type.BaseType;
                    while (currentType != null && !IsDataType(currentType))
                    {
                        currentType = currentType.BaseType;
                    }
                    if (currentType != null)
                    {
                        AddDefaultSerializer(currentType);
                    }
                }
                else if (type.IsGenericType && this.typedSerializer.ContainsKey(type.GetGenericTypeDefinition()))
                {
                    foreach (var argument in type.GetGenericArguments())
                    {
                        AddDefaultSerializer(argument);
                    }
                }
                else
                {
                    throw new InvalidOperationException("Don't know how to serialize type " + type.FullName + ".");
                }
            }
        }

        public XNode SerializeFromArray(string elementName, IEnumerable data)
        {
            return new XElement(
                elementName,
                data
                    .Cast<object>()
                    .Select(i => new XElement("Item", Serialize(i)))
                    .ToArray()
                );
        }

        public IList DeserializeToArray(XNode data, Type elementType)
        {
            Type arrayType = elementType.MakeArrayType();
            ConstructorInfo arrayConstructor = arrayType.GetConstructor(new Type[] { typeof(int) });

            XElement[] items = ((XElement)data).Elements("Item").ToArray();
            IList array = (IList)arrayConstructor.Invoke(new object[] { items.Length });
            for (int i = 0; i < items.Length; i++)
            {
                array[i] = Deserialize(items[i].FirstNode, elementType);
            }
            return array;
        }
    }

    namespace StrongTypedNodeEndpointSerializers
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
    }
}
