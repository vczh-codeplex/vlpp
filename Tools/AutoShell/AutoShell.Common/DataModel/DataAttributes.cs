using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;

namespace AutoShell.DataModel
{
    [AttributeUsage(AttributeTargets.Class)]
    public class DataSerializableAttribute : Attribute
    {
    }

    [AttributeUsage(AttributeTargets.Property)]
    public class DataSerializerVisibleAttribute : Attribute
    {
    }

    [AttributeUsage(AttributeTargets.Class)]
    public class DataSerializerConverterAttribute : Attribute
    {
        public DataSerializerConverterAttribute(Type type)
        {
            this.ConverterType = type;
        }

        public Type ConverterType { get; private set; }
    }
}
