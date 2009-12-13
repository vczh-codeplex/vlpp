using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Reflection;

namespace AutoShell.DataModel
{
    public abstract class DataConverter
    {
        public static Dictionary<Type, DataConverter> PredifinedConverters { get; private set; }
        public static Dictionary<Type, DataConverter> LoadedConverters { get; private set; }

        static DataConverter()
        {
            DataConverter.PredifinedConverters = new Dictionary<Type, DataConverter>();
            foreach (Type type in new Type[] { typeof(bool), typeof(byte), typeof(char), typeof(UInt16), typeof(Int16), typeof(UInt32), typeof(Int32), typeof(UInt64), typeof(Int64), typeof(DateTime), typeof(Guid) })
            {
                DataConverter.PredifinedConverters.Add(type, new PrimitiveDataConverter(type));
            }
            DataConverter.PredifinedConverters.Add(typeof(string), new StringDataConverter());
            DataConverter.LoadedConverters = new Dictionary<Type, DataConverter>();
        }

        public static DataConverter GetConverter(Type converterType)
        {
            if (!DataConverter.LoadedConverters.ContainsKey(converterType))
            {
                object converter = converterType.GetConstructor(new Type[] { }).Invoke(new object[] { });
                DataConverter.LoadedConverters.Add(converterType, (DataConverter)converter);
            }
            return DataConverter.LoadedConverters[converterType];
        }

        public abstract bool IsConvertableToString(Type type);
        public abstract bool IsConvertableFromString(Type type);
        public abstract string ConvertToString(object source);
        public abstract object ConvertFromString(string source, Type type);
    }

    internal class StringDataConverter : DataConverter
    {
        public override bool IsConvertableToString(Type type)
        {
            return type == typeof(string);
        }

        public override bool IsConvertableFromString(Type type)
        {
            return type == typeof(string);
        }

        public override string ConvertToString(object source)
        {
            return (string)source;
        }

        public override object ConvertFromString(string source, Type type)
        {
            return source;
        }
    }

    internal class PrimitiveDataConverter : DataConverter
    {
        private MethodInfo parse = null;
        private ConstructorInfo ctor = null;
        public Type PrimitiveType { get; private set; }

        public PrimitiveDataConverter(Type primitiveType)
        {
            this.PrimitiveType = primitiveType;
            MethodInfo parseMethod = primitiveType.GetMethod("Parse", BindingFlags.Public | BindingFlags.Static, null, new Type[] { typeof(string) }, null);
            if (parseMethod != null && parseMethod.GetParameters().Length == 1 && parseMethod.GetParameters()[0].ParameterType == typeof(string))
            {
                this.parse = parseMethod;
            }
            this.ctor = primitiveType.GetConstructor(new Type[] { typeof(string) });
            if (this.parse == null && this.ctor == null)
            {
                throw new InvalidOperationException();
            }
        }

        public override bool IsConvertableToString(Type type)
        {
            return type == this.PrimitiveType && (this.parse != null || this.ctor != null);
        }

        public override bool IsConvertableFromString(Type type)
        {
            return type == this.PrimitiveType && (this.parse != null || this.ctor != null);
        }

        public override string ConvertToString(object source)
        {
            if (source == null)
            {
                return null;
            }
            else
            {
                return source.ToString();
            }
        }

        public override object ConvertFromString(string source, Type type)
        {
            if (source == null)
            {
                return null;
            }
            else if (this.parse != null)
            {
                return this.parse.Invoke(null, new object[] { source });
            }
            else
            {
                return this.ctor.Invoke(new object[] { source });
            }
        }
    }
}