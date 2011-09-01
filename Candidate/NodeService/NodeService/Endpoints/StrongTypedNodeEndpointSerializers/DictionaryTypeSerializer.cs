using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Collections;
using System.Xml.Linq;

namespace NodeService.Endpoints.StrongTypedNodeEndpointSerializers
{
    class MapTypeSerializer<TObjectDictionary> : INamedSerializer
        where TObjectDictionary : IDictionary<object, object>, IDictionary
    {
        protected static readonly Type targetType = typeof(TObjectDictionary).GetGenericTypeDefinition();
        protected static readonly string elementName = targetType.Name.Substring(0, targetType.Name.IndexOf('`'));

        protected StrongTypedNodeEndpointSerializer serializer;

        public MapTypeSerializer(StrongTypedNodeEndpointSerializer serializer)
        {
            this.serializer = serializer;
        }

        public virtual string ElementName
        {
            get
            {
                return elementName;
            }
        }

        public virtual Type TargetType
        {
            get
            {
                return targetType;
            }
        }

        public XNode Serialize(object data)
        {
            XElement result = new XElement(elementName);
            IDictionary dictionary = (IDictionary)data;
            IDictionaryEnumerator enumerator = dictionary.GetEnumerator();
            while (enumerator.MoveNext())
            {
                var e = enumerator.Entry;
                result.Add(
                    new XElement(
                        "Pair",
                        new XElement("Key", this.serializer.Serialize(e.Key)),
                        new XElement("Value", this.serializer.Serialize(e.Value))
                        )
                    );
            }
            return result;
        }

        public object Deserialize(XNode data, Type type)
        {
            IDictionary dictionary = (IDictionary)type.GetConstructor(new Type[] { }).Invoke(new object[] { });
            Type keyType = type.GetGenericArguments()[0];
            Type valueType = type.GetGenericArguments()[1];
            foreach (XElement pair in ((XElement)data).Elements("Pair"))
            {
                object key = this.serializer.Deserialize(pair.Element("Key").FirstNode, keyType);
                object value = this.serializer.Deserialize(pair.Element("Value").FirstNode, valueType);
                dictionary[key] = value;
            }
            return dictionary;
        }
    }

    class DictionaryTypeSerializer : MapTypeSerializer<Dictionary<object, object>>
    {
        public DictionaryTypeSerializer(StrongTypedNodeEndpointSerializer serializer)
            : base(serializer)
        {
        }
    }

    class SortedDictionaryTypeSerializer : MapTypeSerializer<SortedDictionary<object, object>>
    {
        public SortedDictionaryTypeSerializer(StrongTypedNodeEndpointSerializer serializer)
            : base(serializer)
        {
        }
    }

    class SortedListTypeSerializer : MapTypeSerializer<SortedList<object, object>>
    {
        public SortedListTypeSerializer(StrongTypedNodeEndpointSerializer serializer)
            : base(serializer)
        {
        }
    }
}
