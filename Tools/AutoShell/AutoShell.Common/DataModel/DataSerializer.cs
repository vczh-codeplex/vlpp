using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Xml.Linq;
using System.Collections;
using System.Reflection;

namespace AutoShell.DataModel
{
    public class DataSerializer
    {
        private string GetName(Type type)
        {
            return type.AssemblyQualifiedName;
        }

        public XElement Serialize(IEnumerable enumerable, bool reverse)
        {
            IEnumerable<XElement> nodes = enumerable.OfType<object>().Select(obj => Serialize(obj));
            if (reverse)
            {
                nodes = nodes.Reverse();
            }
            return new XElement(
                "Enumerable",
                new XAttribute("Type", GetName(enumerable.GetType())),
                nodes
                );
        }

        public XElement Serialize(IDictionary dictionary)
        {
            return new XElement(
                "Dictionary",
                new XAttribute("Type", GetName(dictionary.GetType())),
                dictionary.Keys.OfType<object>().Select(key =>
                    new XElement("Pair",
                        new XElement("Key", Serialize(key)),
                        new XElement("Value", Serialize(dictionary[key]))
                        )
                    )
                );
        }

        public XElement Serialize(object source)
        {
            if (source == null)
            {
                return new XElement("Null");
            }
            else if (source.GetType().IsGenericType)
            {
                Type genericDefinition = source.GetType().GetGenericTypeDefinition();
                if (genericDefinition == typeof(List<>))
                {
                    return Serialize(source as IEnumerable, false);
                }
                else if (genericDefinition == typeof(Queue<>))
                {
                    return Serialize(source as IEnumerable, false);
                }
                else if (genericDefinition == typeof(Stack<>))
                {
                    return Serialize(source as IEnumerable, true);
                }
                else if (genericDefinition == typeof(LinkedList<>))
                {
                    return Serialize(source as IEnumerable, false);
                }
                else if (genericDefinition == typeof(Dictionary<,>))
                {
                    return Serialize(source as IDictionary);
                }
                else if (genericDefinition == typeof(SortedDictionary<,>))
                {
                    return Serialize(source as IDictionary);
                }
                else if (genericDefinition == typeof(SortedList<,>))
                {
                    return Serialize(source as IDictionary);
                }
                else
                {
                    throw new NotSupportedException(string.Format(Strings.ExceptionUnserializableType, GetName(source.GetType())));
                }
            }
            else if (source.GetType().IsArray)
            {
                return new XElement(
                    "Array",
                    new XAttribute("Type", GetName(source.GetType())),
                    (source as IEnumerable).OfType<object>().Select(obj => Serialize(obj))
                    );
            }
            else if (DataConverter.PredifinedConverters.ContainsKey(source.GetType()))
            {
                return new XElement(
                    "Primitive",
                    new XAttribute("Type", GetName(source.GetType())),
                    new XCData(DataConverter.PredifinedConverters[source.GetType()].ConvertToString(source))
                    );
            }
            else
            {
                object[] serializers = source.GetType().GetCustomAttributes(typeof(DataSerializableAttribute), false);
                object[] converters = source.GetType().GetCustomAttributes(typeof(DataSerializerConverterAttribute), false);
                if (serializers.Length > 0)
                {
                    return new XElement(
                        "Class",
                        new XAttribute("Type", GetName(source.GetType())),
                        source.GetType().GetProperties(BindingFlags.Public | BindingFlags.Instance | BindingFlags.FlattenHierarchy)
                            .Where(property => property.GetCustomAttributes(typeof(DataSerializerVisibleAttribute), false).Length > 0)
                            .Select(property =>
                                new XElement("Property",
                                    new XAttribute("Name", property.Name),
                                    Serialize(property.GetValue(source, new object[] { }))
                                    )
                                )
                            );
                }
                else if (converters.Length > 0)
                {
                    DataSerializerConverterAttribute attribute = (DataSerializerConverterAttribute)converters[0];
                    return new XElement(
                        "Converter",
                        new XAttribute("Type", GetName(source.GetType())),
                        new XCData(DataConverter.GetConverter(attribute.ConverterType).ConvertToString(source))
                        );
                }
                else
                {
                    throw new NotSupportedException(string.Format(Strings.ExceptionUnserializableType, GetName(source.GetType())));
                }
            }
        }
    }
}