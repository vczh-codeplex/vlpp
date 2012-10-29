using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Xml.Linq;
using System.Net;
using System.Collections;
using System.Threading;

namespace XmlRpc
{
    public class XmlRpcClient
    {
        private XElement Serialize(Type type, object value)
        {
            if (value == null)
            {
                return new XElement("nil");
            }
            else if (type == typeof(string))
            {
                return new XElement("string", (string)value);
            }
            else if (type == typeof(int))
            {
                return new XElement("int", (int)value);
            }
            else if (type == typeof(double))
            {
                return new XElement("double", (double)value);
            }
            else if (type == typeof(bool))
            {
                return new XElement("boolean", ((bool)value) ? 1 : 0);
            }
            else if (type == typeof(DateTime))
            {
                DateTime dateTime = (DateTime)value;
                string dateTimeValue = dateTime.ToString("yyyyMMdd") + "T" + dateTime.ToString("HH:mm:ss");
                return new XElement("dateTime.iso8601", dateTimeValue);
            }
            else if (type.IsArray)
            {
                Type elementType = type.GetElementType();
                return new XElement("array",
                    new XElement("data",
                        ((IEnumerable)value).Cast<object>()
                            .Select(x => new XElement("value", Serialize(elementType, x)))
                            .ToArray()
                        )
                    );
            }
            else if (type.IsClass)
            {
                var props = type.GetProperties();
                if (type.GetCustomAttributes(typeof(XmlRpcStruct), false).Length != 0)
                {
                    props = props.Where(p => p.GetCustomAttributes(typeof(XmlRpcMember), false).Length > 0).ToArray();
                }
                return new XElement("struct",
                    props
                        .Select(p => new XElement("member",
                            new XElement("name", p.Name),
                            new XElement("value", Serialize(p.PropertyType, p.GetValue(value, new object[0])))
                            )
                        )
                    );
            }
            else
            {
                throw new XmlRpcInvalidTypeException();
            }
        }

        private object Deserialize(Type type, XElement xml)
        {
            switch (xml.Name.LocalName)
            {
                case "nil":
                    return null;
                case "string":
                    return xml.Value;
                case "int":
                case "i4":
                    return int.Parse(xml.Value);
                case "double":
                    return double.Parse(xml.Value);
                case "boolean":
                    return xml.Value != "0";
                case "dateTime.iso8601":
                    {
                        string dateTimeValue = xml.Value;
                        int year = int.Parse(dateTimeValue.Substring(0, 4));
                        int month = int.Parse(dateTimeValue.Substring(4, 2));
                        int day = int.Parse(dateTimeValue.Substring(6, 2));
                        int hour = int.Parse(dateTimeValue.Substring(9, 2));
                        int minute = int.Parse(dateTimeValue.Substring(12, 2));
                        int second = int.Parse(dateTimeValue.Substring(15, 2));
                        return new DateTime(year, month, day, hour, minute, second);
                    }
                case "array":
                    {
                        var values = xml.Element("data").Elements("value").Select(x => x.Elements().First()).ToArray();
                        Array arrayValue = Array.CreateInstance(type.GetElementType(), values.Length);
                        for (int i = 0; i < values.Length; i++)
                        {
                            arrayValue.SetValue(Deserialize(type.GetElementType(), values[i]), i);
                        }
                        return arrayValue;
                    }
                case "struct":
                    {
                        object structValue = type.GetConstructor(new Type[0]).Invoke(new object[0]);
                        foreach (var member in xml.Elements("member"))
                        {
                            string name = member.Element("name").Value;
                            XElement value = member.Element("value").Elements().First();
                            var prop = type.GetProperty(name);
                            if (prop != null)
                            {
                                prop.SetValue(structValue, Deserialize(prop.PropertyType, value), new object[0]);
                            }
                        }
                        return structValue;
                    }
                default:
                    throw new XmlRpcInvalidTypeException();
            }
        }

        protected XElement CreateMethodArguments(string method, Type[] types, object[] values)
        {
            var parameters = types
                .Zip(
                    values,
                    (a, b) => Tuple.Create(a, b)
                    )
                .ToArray();
            return new XElement("methodCall",
                new XElement("methodName", method),
                new XElement("params",
                    parameters.Select(p =>
                        new XElement("param",
                            new XElement("value", Serialize(p.Item1, p.Item2))
                            )
                        ).ToArray()
                    )
                );

        }

        protected object CreateMethodResult(Type type, XElement xml)
        {
            XElement value = xml.Element("params").Element("param").Element("value").Elements().First();
            return Deserialize(type, value);
        }

        protected XElement Post(XElement methodCall, string url)
        {
            WebRequest req = WebRequest.Create(url);
            {
                req.ContentType = "application/x-www-form-urlencoded";
                req.Method = "POST";
                byte[] inputBytes = System.Text.Encoding.ASCII.GetBytes(methodCall.ToString());
                req.ContentLength = inputBytes.Length;
                System.IO.Stream os = req.GetRequestStream();
                os.Write(inputBytes, 0, inputBytes.Length);
                os.Close();
            }

            List<byte[]> outputBytesList = new List<byte[]>();
            {
                var resp = req.GetResponse();
                var stream = resp.GetResponseStream();
                byte[] buffer = new byte[1048576];
                while (true)
                {
                    int read = stream.Read(buffer, 0, buffer.Length);
                    if (read == 0) break;
                    byte[] readBuffer = new byte[read];
                    Array.Copy(buffer, readBuffer, read);
                    outputBytesList.Add(readBuffer);
                }
                stream.Close();
                resp.Close();
            }
            string tempString = null;
            {
                byte[] outputBytes = new byte[outputBytesList.Select(b => b.Length).Sum()];
                int start = 0;
                foreach (var bytes in outputBytesList)
                {
                    Array.Copy(bytes, 0, outputBytes, start, bytes.Length);
                    start += bytes.Length;
                }
                var encoding = new UTF8Encoding();
                tempString = encoding.GetString(outputBytes);
            }

            XElement tempXml = null;
            try
            {
                tempXml = XElement.Parse(tempString.Trim());
            }
            catch (Exception)
            {
                tempString = new string(
                    tempString
                        .Where(c => !(
                            0x00 <= c && c <= 0x08 ||
                            0x0B <= c && c <= 0x0C ||
                            0x0E <= c && c <= 0x0F ||
                            0x10 <= c && c <= 0x1F ||
                            c == 0x7F)
                            )
                        .ToArray()
                    );
                tempString = tempString
                    .Replace("&#x00;", "")
                    .Replace("&#x01;", "")
                    .Replace("&#x02;", "")
                    .Replace("&#x03;", "")
                    .Replace("&#x04;", "")
                    .Replace("&#x05;", "")
                    .Replace("&#x06;", "")
                    .Replace("&#x07;", "")
                    .Replace("&#x08;", "")
                    .Replace("&#x0B;", "").Replace("&#x0b;", "")
                    .Replace("&#x0C;", "").Replace("&#x0c;", "")
                    .Replace("&#x0E;", "").Replace("&#x0e;", "")
                    .Replace("&#x0F;", "").Replace("&#x0f;", "")
                    .Replace("&#x10;", "")
                    .Replace("&#x11;", "")
                    .Replace("&#x12;", "")
                    .Replace("&#x13;", "")
                    .Replace("&#x14;", "")
                    .Replace("&#x15;", "")
                    .Replace("&#x16;", "")
                    .Replace("&#x17;", "")
                    .Replace("&#x18;", "")
                    .Replace("&#x19;", "")
                    .Replace("&#x1A;", "").Replace("&#x1a;", "")
                    .Replace("&#x1B;", "").Replace("&#x1b;", "")
                    .Replace("&#x1C;", "").Replace("&#x1c;", "")
                    .Replace("&#x1D;", "").Replace("&#x1d;", "")
                    .Replace("&#x1E;", "").Replace("&#x1e;", "")
                    .Replace("&#x1F;", "").Replace("&#x1f;", "")
                    ;
                tempXml = XElement.Parse(tempString.Trim());
            }
            var query = tempXml.Elements("fault").Elements("value").Elements("struct").Elements("member").Elements("name");
            foreach (XElement x in query)
            {
                if (x.Value == "faultString")
                {
                    throw new XmlRpcException(x.Parent.Elements("value").First().Value);
                }
            }

            if (tempXml.Name.ToString() == "methodResponse")
            {
                return tempXml;
            }
            else
            {
                throw new XmlRpcInvalidServerException(url);
            }
        }

        public static T Create<T>(string url = "")
        {
            return XmlRpcClientBuilder.Create<T>(url);
        }
    }

    class XmlRpcException : Exception
    {
        public XmlRpcException(string message)
            : base(message)
        {
        }
    }

    class XmlRpcInvalidServerException : Exception
    {
        public XmlRpcInvalidServerException(string url)
            : base(string.Format("{0} is not a valid XML-RPC server.", url))
        {
        }
    }

    class XmlRpcInvalidTypeException : Exception
    {
        public XmlRpcInvalidTypeException()
            : base("Only string, int, double, boolean, DateTime, array and class are supported.")
        {
        }
    }

    class XmlRpcInvalidContractException : Exception
    {
        public XmlRpcInvalidContractException(string message)
            : base(message)
        {
        }
    }
}
