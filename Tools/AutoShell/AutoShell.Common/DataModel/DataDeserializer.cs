using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Collections;
using System.Xml.Linq;
using System.Reflection;

namespace AutoShell.DataModel
{
    public class DataDeserializer
    {
        private Type GetType(string value)
        {
            return Type.GetType(value);
        }

        public void DeserializeEnumerable(XElement node, Action<object> callback)
        {
            foreach (XElement subNode in node.Elements())
            {
                callback(Deserialize(subNode));
            }
        }

        public void DeserializeDictionary(XElement node, Action<object, object> callback)
        {
            foreach (XElement subNode in node.Elements())
            {
                callback(Deserialize((XElement)subNode.Element("Key").FirstNode), Deserialize((XElement)subNode.Element("Value").FirstNode));
            }
        }

        public object Deserialize(XElement node)
        {
            switch (node.Name.LocalName)
            {
                case "Null":
                    return null;
                case "Enumerable":
                    {
                        Type type = GetType(node.Attribute("Type").Value);
                        Type genericDefinition = type.GetGenericTypeDefinition();
                        object enumerable = null;
                        MethodInfo method = null;
                        if (genericDefinition == typeof(List<>))
                        {
                            enumerable = type.GetConstructor(new Type[] { }).Invoke(new object[] { });
                            method = type.GetMethod("Add", type.GetGenericArguments());
                        }
                        else if (genericDefinition == typeof(Queue<>))
                        {
                            enumerable = type.GetConstructor(new Type[] { }).Invoke(new object[] { });
                            method = type.GetMethod("Enqueue", type.GetGenericArguments());
                        }
                        else if (genericDefinition == typeof(Stack<>))
                        {
                            enumerable = type.GetConstructor(new Type[] { }).Invoke(new object[] { });
                            method = type.GetMethod("Push", type.GetGenericArguments());
                        }
                        else if (genericDefinition == typeof(LinkedList<>))
                        {
                            enumerable = type.GetConstructor(new Type[] { }).Invoke(new object[] { });
                            method = type.GetMethod("AddLast", type.GetGenericArguments());
                        }
                        else
                        {
                            throw new NotSupportedException(string.Format(Strings.ExceptionUnserializableType, type.FullName));
                        }
                        DeserializeEnumerable(node, value => method.Invoke(enumerable, new object[] { value }));
                        return enumerable;
                    }
                case "Dictionary":
                    {
                        Type type = GetType(node.Attribute("Type").Value);
                        Type genericDefinition = type.GetGenericTypeDefinition();
                        object dictionary = null;
                        MethodInfo method = null;
                        if (genericDefinition == typeof(Dictionary<,>))
                        {
                            dictionary = type.GetConstructor(new Type[] { }).Invoke(new object[] { });
                            method = type.GetMethod("Add", type.GetGenericArguments());
                        }
                        else if (genericDefinition == typeof(SortedDictionary<,>))
                        {
                            dictionary = type.GetConstructor(new Type[] { }).Invoke(new object[] { });
                            method = type.GetMethod("Add", type.GetGenericArguments());
                        }
                        else if (genericDefinition == typeof(SortedList<,>))
                        {
                            dictionary = type.GetConstructor(new Type[] { }).Invoke(new object[] { });
                            method = type.GetMethod("Add", type.GetGenericArguments());
                        }
                        else
                        {
                            throw new NotSupportedException(string.Format(Strings.ExceptionUnserializableType, type.FullName));
                        }
                        DeserializeDictionary(node, (key, value) => method.Invoke(dictionary, new object[] { key, value }));
                        return dictionary;
                    }
                case "Array":
                    {
                        Type type = GetType(node.Attribute("Type").Value);
                        object enumerable = typeof(List<>).MakeGenericType(type.GetElementType()).GetConstructor(new Type[] { }).Invoke(new object[] { });
                        MethodInfo method = enumerable.GetType().GetMethod("Add", new Type[] { type.GetElementType() });
                        DeserializeEnumerable(node, value => method.Invoke(enumerable, new object[] { value }));
                        return enumerable.GetType().GetMethod("ToArray").Invoke(enumerable, new object[] { });
                    }
                case "Primitive":
                    {
                        Type type = GetType(node.Attribute("Type").Value);
                        XCData value = (XCData)node.FirstNode;
                        return DataConverter.PredifinedConverters[type].ConvertFromString(value.Value, type);
                    }
                case "Converter":
                    {
                        Type type = GetType(node.Attribute("Type").Value);
                        Type converterType = ((DataSerializerConverterAttribute)type.GetCustomAttributes(typeof(DataSerializerConverterAttribute), false)[0]).ConverterType;
                        DataConverter converter = DataConverter.GetConverter(converterType);
                        XCData value = (XCData)node.FirstNode;
                        return converter.ConvertFromString(value.Value, type);
                    }
                case "Class":
                    {
                        Type type = GetType(node.Attribute("Type").Value);
                        object classObject = type.GetConstructor(new Type[] { }).Invoke(new object[] { });
                        foreach (XElement subNode in node.Elements())
                        {
                            string propertyName = subNode.Attribute("Name").Value;
                            PropertyInfo property = type.GetProperty(propertyName);
                            property.SetValue(classObject, Deserialize((XElement)subNode.FirstNode), new object[] { });
                        }
                        return classObject;
                    }
                default:
                    throw new ArgumentException(string.Format(Strings.ExceptionUnserializableNode, node.Name.LocalName));
            }
        }
    }
}
