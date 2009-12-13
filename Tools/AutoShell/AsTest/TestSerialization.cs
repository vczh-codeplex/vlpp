using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using Microsoft.VisualStudio.TestTools.UnitTesting;
using AutoShell.DataModel;

namespace AsTest
{
    [TestClass]
    public class TestSerialization
    {
        private object Do(object source)
        {
            DataSerializer serializer = new DataSerializer();
            DataDeserializer deserializer = new DataDeserializer();
            return deserializer.Deserialize(serializer.Serialize(source));
        }

        [TestMethod]
        public void TestNull()
        {
            Assert.IsNull(Do(null));
        }

        [TestMethod]
        public void TestPrimitive()
        {
            Guid guid = Guid.NewGuid();
            DateTime dt = new DateTime(2000, 1, 2, 3, 4, 5, 6);
            DateTime dt2 = DateTime.MinValue;

            Assert.AreEqual(true, Do(true));
            Assert.AreEqual(false, Do(false));
            Assert.AreEqual('v', Do('v'));
            Assert.AreEqual((UInt16)123, Do((UInt16)123));
            Assert.AreEqual((UInt32)123, Do((UInt32)123));
            Assert.AreEqual((UInt64)123, Do((UInt64)123));
            Assert.AreEqual((Int16)123, Do((Int16)123));
            Assert.AreEqual((Int32)123, Do((Int32)123));
            Assert.AreEqual((Int64)123, Do((Int64)123));
            Assert.AreEqual("vczh", Do("vczh"));
            Assert.AreEqual(guid, Do(guid));

            dt2 = (DateTime)(dt);
            Assert.AreEqual(dt.ToString(), dt2.ToString());
        }

        [TestMethod]
        public void TestConverter()
        {
            MyClass myClass = new MyClass();
            myClass.Value = 5;
            Assert.AreEqual(myClass.Value, ((MyClass)Do(myClass)).Value);
        }

        [TestMethod]
        public void TestList()
        {
            List<int> list = new List<int>(new int[] { 0, 1, 2, 3, 4, 5, 6, 7, 8, 9 });
            Assert.IsTrue(list.SequenceEqual((List<int>)Do(list)));
        }

        [TestMethod]
        public void TestStack()
        {
            Stack<int> list = new Stack<int>(new int[] { 0, 1, 2, 3, 4, 5, 6, 7, 8, 9 });
            Assert.IsTrue(list.SequenceEqual((Stack<int>)Do(list)));
        }

        [TestMethod]
        public void TestQueue()
        {
            Queue<int> list = new Queue<int>(new int[] { 0, 1, 2, 3, 4, 5, 6, 7, 8, 9 });
            Assert.IsTrue(list.SequenceEqual((Queue<int>)Do(list)));
        }

        [TestMethod]
        public void TestLinkedList()
        {
            LinkedList<int> list = new LinkedList<int>(new int[] { 0, 1, 2, 3, 4, 5, 6, 7, 8, 9 });
            Assert.IsTrue(list.SequenceEqual((LinkedList<int>)Do(list)));
        }

        [TestMethod]
        public void TestDictionary()
        {
            Dictionary<int, int> dictionary = new Dictionary<int, int>();
            for (int i = 0; i < 10; i++)
            {
                dictionary.Add(i, i * i);
            }
            Assert.IsTrue(dictionary.SequenceEqual((Dictionary<int, int>)Do(dictionary)));
        }

        [TestMethod]
        public void TestSortedList()
        {
            SortedList<int, int> dictionary = new SortedList<int, int>();
            for (int i = 0; i < 10; i++)
            {
                dictionary.Add(i, i * i);
            }
            Assert.IsTrue(dictionary.SequenceEqual((SortedList<int, int>)Do(dictionary)));
        }

        [TestMethod]
        public void TestSortedDictionary()
        {
            SortedDictionary<int, int> dictionary = new SortedDictionary<int, int>();
            for (int i = 0; i < 10; i++)
            {
                dictionary.Add(i, i * i);
            }
            Assert.IsTrue(dictionary.SequenceEqual((SortedDictionary<int, int>)Do(dictionary)));
        }

        [TestMethod]
        public void TestArray()
        {
            int[] array = new int[] { 0, 1, 2, 3, 4, 5, 6, 7, 8, 9 };
            Assert.IsTrue(array.SequenceEqual((int[])Do(array)));
        }

        [TestMethod]
        public void TestClass()
        {
            MyBase myBase = new MyBase();
            myBase.Value1 = 1;
            myBase.Value2 = "2";
            Assert.AreEqual(myBase, Do(myBase));
            MyDerived myDerived = new MyDerived();
            myDerived.Value1 = 1;
            myDerived.Value2 = "2";
            myDerived.Value3 = Guid.NewGuid();
            myDerived.Value4 = true;
            Assert.AreEqual(myDerived, Do(myDerived));
        }
    }

    [DataSerializerConverter(typeof(MyClassConverter))]
    internal class MyClass
    {
        public int Value { get; set; }
    }

    internal class MyClassConverter : DataConverter
    {
        public override bool IsConvertableToString(Type type)
        {
            return type == typeof(MyClass);
        }

        public override bool IsConvertableFromString(Type type)
        {
            return type == typeof(MyClass);
        }

        public override string ConvertToString(object source)
        {
            return ((MyClass)source).Value.ToString();
        }

        public override object ConvertFromString(string source, Type type)
        {
            MyClass myClass = new MyClass();
            myClass.Value = int.Parse(source);
            return myClass;
        }
    }

    [DataSerializable]
    internal class MyBase
    {
        [DataSerializerVisible]
        public int Value1 { get; set; }

        [DataSerializerVisible]
        public string Value2 { get; set; }

        public override bool Equals(object obj)
        {
            MyBase myBase = obj as MyBase;
            if (myBase != null)
            {
                return this.Value1 == myBase.Value1 && this.Value2 == myBase.Value2;
            }
            else
            {
                return false;
            }
        }

        public override int GetHashCode()
        {
            return Value1.GetHashCode() + Value2.GetHashCode();
        }
    }

    [DataSerializable]
    internal class MyDerived : MyBase
    {
        [DataSerializerVisible]
        public Guid Value3 { get; set; }

        [DataSerializerVisible]
        public bool Value4 { get; set; }

        public override bool Equals(object obj)
        {
            MyDerived myDerived = obj as MyDerived;
            if (myDerived != null)
            {
                return base.Equals(obj) && this.Value3 == myDerived.Value3 && this.Value4 == myDerived.Value4;
            }
            else
            {
                return false;
            }
        }

        public override int GetHashCode()
        {
            return base.GetHashCode() + Value1.GetHashCode() + Value2.GetHashCode();
        }
    }
}
