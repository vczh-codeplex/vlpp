using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Xml.Linq;
using System.Reflection;

namespace NodeService.Endpoints.StrongTypedNodeEndpointSerializers
{
    class DataTypeSerializer : INamedSerializer
    {
        private StrongTypedNodeEndpointSerializer serializer;
        private Type type;
        private Dictionary<string, FieldInfo> serializableFields = new Dictionary<string, FieldInfo>();
        private Dictionary<string, PropertyInfo> serializableProperties = new Dictionary<string, PropertyInfo>();

        public DataTypeSerializer(StrongTypedNodeEndpointSerializer serializer, Type type)
        {
            this.serializer = serializer;
            this.type = type;
            this.serializer.AddSerializer(this);

            foreach (var fieldInfo in this.type
                .GetFields(BindingFlags.Public | BindingFlags.NonPublic | BindingFlags.Instance)
                .Where(f => f.GetCustomAttributes(typeof(NodeEndpointDataMemberAttribute), false).Length > 0)
                )
            {
                if (!this.serializableFields.ContainsKey(fieldInfo.Name))
                {
                    this.serializableFields.Add(fieldInfo.Name, fieldInfo);
                    this.serializer.AddDefaultSerializer(fieldInfo.FieldType);
                }
            }

            foreach (var propertyInfo in this.type
                .GetProperties(BindingFlags.Public | BindingFlags.NonPublic | BindingFlags.Instance)
                .Where(f => f.GetCustomAttributes(typeof(NodeEndpointDataMemberAttribute), false).Length > 0)
                )
            {
                if (!this.serializableProperties.ContainsKey(propertyInfo.Name))
                {
                    this.serializableProperties.Add(propertyInfo.Name, propertyInfo);
                    this.serializer.AddDefaultSerializer(propertyInfo.PropertyType);
                }
            }
        }

        public string ElementName
        {
            get
            {
                return this.type.Name;
            }
        }

        public Type TargetType
        {
            get
            {
                return this.type;
            }
        }

        public XNode Serialize(object data)
        {
            var fields = this.serializableFields.Values.Select(f =>
                new XElement(
                    f.Name,
                    this.serializer.Serialize(f.GetValue(data))
                    )
                );
            var properties = this.serializableProperties.Values.Select(f =>
                new XElement(
                    f.Name,
                    this.serializer.Serialize(f.GetValue(data, new object[] { }))
                    )
                );
            return new XElement(
                this.ElementName,
                fields.Concat(properties).ToArray()
                );
        }

        public object Deserialize(XNode data, Type type)
        {
            XElement element = (XElement)data;
            object obj = this.type.GetConstructor(new Type[] { }).Invoke(new object[] { });
            foreach (var fieldInfo in this.serializableFields.Values)
            {
                object value = this.serializer.Deserialize(
                    element.Element(fieldInfo.Name).FirstNode,
                    fieldInfo.FieldType
                    );
                fieldInfo.SetValue(obj, value);
            }
            foreach (var propertyInfo in this.serializableProperties.Values)
            {
                object value = this.serializer.Deserialize(
                    element.Element(propertyInfo.Name).FirstNode,
                    propertyInfo.PropertyType
                    );
                propertyInfo.SetValue(obj, value, new object[] { });
            }
            return obj;
        }
    }
}
