using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Xml.Linq;
using System.Collections;
using System.Reflection;

namespace NodeService.Endpoints.StrongTypedNodeEndpointSerializers
{
    class CollectionTypeSerializer<TObjectCollection> : INamedSerializer
        where TObjectCollection : IEnumerable<object>
    {
        protected static readonly Type targetType = typeof(TObjectCollection).GetGenericTypeDefinition();
        protected static readonly string elementName = targetType.Name.Substring(0, targetType.Name.IndexOf('`'));

        protected StrongTypedNodeEndpointSerializer serializer;

        public CollectionTypeSerializer(StrongTypedNodeEndpointSerializer serializer)
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

        public virtual XNode Serialize(object data)
        {
            return this.serializer.SerializeFromArray(elementName, (IEnumerable)data);
        }

        public virtual object Deserialize(XNode data, Type type)
        {
            Type elementType = type.GetGenericArguments()[0];
            Type enumerableType = typeof(IEnumerable<>).MakeGenericType(elementType);
            ConstructorInfo constructor = type.GetConstructor(new Type[] { enumerableType });
            return constructor.Invoke(new object[] { this.serializer.DeserializeToArray(data, elementType) });
        }
    }

    class ListTypeSerializer : CollectionTypeSerializer<List<object>>
    {
        public ListTypeSerializer(StrongTypedNodeEndpointSerializer serializer)
            : base(serializer)
        {
        }
    }

    class HashSetTypeSerializer : CollectionTypeSerializer<HashSet<object>>
    {
        public HashSetTypeSerializer(StrongTypedNodeEndpointSerializer serializer)
            : base(serializer)
        {
        }
    }

    class LinkedListTypeSerializer : CollectionTypeSerializer<LinkedList<object>>
    {
        public LinkedListTypeSerializer(StrongTypedNodeEndpointSerializer serializer)
            : base(serializer)
        {
        }
    }

    class QueueTypeSerializer : CollectionTypeSerializer<Queue<object>>
    {
        public QueueTypeSerializer(StrongTypedNodeEndpointSerializer serializer)
            : base(serializer)
        {
        }
    }

    class SortedSetTypeSerializer : CollectionTypeSerializer<SortedSet<object>>
    {
        public SortedSetTypeSerializer(StrongTypedNodeEndpointSerializer serializer)
            : base(serializer)
        {
        }
    }

    class StackTypeSerializer : CollectionTypeSerializer<Stack<object>>
    {
        public StackTypeSerializer(StrongTypedNodeEndpointSerializer serializer)
            : base(serializer)
        {
        }
    }
}
