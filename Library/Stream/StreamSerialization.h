/***********************************************************************
Vczh Library++ 3.0
Developer: 陈梓瀚(vczh)
Stream::Interfaces

Interfaces:
	StreamBinarySerializer			：序列化反序列化
***********************************************************************/

#ifndef VCZH_STREAM_STREAMSERIALIZATION
#define VCZH_STREAM_STREAMSERIALIZATION

#include "Interfaces.h"
#include "..\String.h"
#include "..\Collections\List.h"
#include "..\Collections\Dictionary.h"
#include "..\Collections\OperationCopyFrom.h"

namespace vl
{
	namespace stream
	{

/***********************************************************************
接口
***********************************************************************/

		template<typename T, bool isPOD>
		class StreamBinaryPrimitiveSerializer
		{
		};

		template<typename T>
		class StreamBinaryPrimitiveSerializer<T, true>
		{
		public:
			static void Write(IStream& stream, const T& value)
			{
				stream.Write((void*)&value, sizeof(value));
			}

			static void Read(IStream& stream, T& value)
			{
				stream.Read((void*)&value, sizeof(value));
			}
		};

		template<typename T>
		class StreamBinarySerializer
		{
		public:
			static void Write(IStream& stream, const T& value)
			{
				StreamBinaryPrimitiveSerializer<T, POD<T>::Result>::Write(stream, value);
			}

			static void Read(IStream& stream, T& value)
			{
				StreamBinaryPrimitiveSerializer<T, POD<T>::Result>::Read(stream, value);
			}
		};

		template<typename T>
		void WriteStream(IStream& stream, const T& value)
		{
			StreamBinarySerializer<T>::Write(stream, value);
		}

		template<typename T>
		void ReadStream(IStream& stream, T& value)
		{
			StreamBinarySerializer<T>::Read(stream, value);
		}

		template<typename T>
		T ReadStream(IStream& stream)
		{
			T value;
			StreamBinarySerializer<T>::Read(stream, value);
			return value;
		}

/***********************************************************************
字符串
***********************************************************************/

		template<typename T>
		class StreamBinarySerializer<ObjectString<T>>
		{
		public:
			static void Write(IStream& stream, const ObjectString<T>& value)
			{
				WriteStream<vint>(stream, value.Length());
				stream.Write((void*)value.Buffer(), value.Length()*sizeof(T));
			}

			static void Read(IStream& stream, ObjectString<T>& value)
			{
				int count=ReadStream<vint>(stream);
				collections::Array<T> buffer(count+1);
				stream.Read((void*)&buffer[0], count*sizeof(T));
				buffer[count]=0;
				value=&buffer[0];
			}
		};

/***********************************************************************
对
***********************************************************************/

		template<typename K, typename V>
		class StreamBinaryPrimitiveSerializer<collections::Pair<K, V>, false>
		{
		public:
			static void Write(IStream& stream, const collections::Pair<K, V>& value)
			{
				WriteStream<K>(stream, value.key);
				WriteStream<V>(stream, value.value);
			}

			static void Read(IStream& stream, collections::Pair<K, V>& value)
			{
				ReadStream<K>(stream, value.key);
				ReadStream<V>(stream, value.value);
			}
		};

/***********************************************************************
集合
***********************************************************************/

		template<typename T, typename C, bool isPOD>
		class StreamBinaryCollectionSerializer
		{
		public:
			static void Write(IStream& stream, const C& value)
			{
				WriteStream<vint>(stream, value.Count());
				for(vint i=0;i<value.Count();i++)
				{
					WriteStream<T>(stream, value.Get(i));
				}
			}

			static void ReadList(IStream& stream, C& value)
			{
				int count=ReadStream<vint>(stream);
				value.Clear();
				for(vint i=0;i<count;i++)
				{
					value.Add(ReadStream<T>(stream));
				}
			}

			static void ReadArray(IStream& stream, C& value)
			{
				int count=ReadStream<vint>(stream);
				value.Resize(count);
				for(vint i=0;i<count;i++)
				{
					value[i]=ReadStream<T>(stream);
				}
			}
		};

		template<typename T, typename C>
		class StreamBinaryCollectionSerializer<T, C, true>
		{
		public:
			static void Write(IStream& stream, const C& value)
			{
				WriteStream<vint>(stream, value.Count());
				stream.Write((void*)&value.Get(0), value.Count()*sizeof(T)); 
			}

			static void ReadList(IStream& stream, C& value)
			{
				int count=ReadStream<vint>(stream);
				value.Clear();
				for(vint i=0;i<count;i++)
				{
					value.Add(ReadStream<T>(stream));
				}
			}

			static void ReadArray(IStream& stream, C& value)
			{
				int count=ReadStream<vint>(stream);
				value.Resize(count);
				stream.Read((void*)&value.Get(0), count*sizeof(T)); 
			}
		};

		template<typename T>
		class StreamBinarySerializer<collections::List<T>>
		{
		public:
			static void Write(IStream& stream, const collections::List<T>& value)
			{
				StreamBinaryCollectionSerializer<T, collections::List<T>, POD<T>::Result>::Write(stream, value);
			}

			static void Read(IStream& stream, collections::List<T>& value)
			{
				StreamBinaryCollectionSerializer<T, collections::List<T>, POD<T>::Result>::ReadList(stream, value);
			}
		};

		template<typename T>
		class StreamBinarySerializer<collections::SortedList<T>>
		{
		public:
			static void Write(IStream& stream, const collections::SortedList<T>& value)
			{
				StreamBinaryCollectionSerializer<T, collections::SortedList<T>, POD<T>::Result>::Write(stream, value);
			}

			static void Read(IStream& stream, collections::SortedList<T>& value)
			{
				StreamBinaryCollectionSerializer<T, collections::SortedList<T>, POD<T>::Result>::ReadList(stream, value);
			}
		};

		template<typename T>
		class StreamBinarySerializer<collections::Array<T>>
		{
		public:
			static void Write(IStream& stream, const collections::Array<T>& value)
			{
				StreamBinaryCollectionSerializer<T, collections::Array<T>, POD<T>::Result>::Write(stream, value);
			}

			static void Read(IStream& stream, collections::Array<T>& value)
			{
				StreamBinaryCollectionSerializer<T,collections::Array<T>,  POD<T>::Result>::ReadArray(stream, value);
			}
		};

		template<typename K, typename V>
		class StreamBinarySerializer<collections::Dictionary<K, V>>
		{
		public:
			static void Write(IStream& stream, const collections::Dictionary<K, V>& value)
			{
				collections::Array<collections::Pair<K, V>> buffer;
				collections::CopyFrom(buffer.Wrap(), value.Wrap());
				StreamBinaryCollectionSerializer<
					collections::Pair<K, V>,
					collections::Array<collections::Pair<K, V>>,
					POD<collections::Pair<K, V>>::Result>
					::Write(stream, buffer);
			}

			static void Read(IStream& stream, collections::Dictionary<K, V>& value)
			{
				collections::Array<collections::Pair<K, V>> buffer;
				StreamBinaryCollectionSerializer<
					collections::Pair<K, V>,
					collections::Array<collections::Pair<K, V>>,
					POD<collections::Pair<K, V>>::Result>
					::ReadArray(stream, buffer);
				collections::CopyFrom(value.Wrap(), buffer.Wrap());
			}
		};

		template<typename K, typename V>
		class StreamBinarySerializer<collections::Group<K, V>>
		{
		public:
			static void Write(IStream& stream, const collections::Group<K, V>& value)
			{
				vint count=value.Count();
				WriteStream<vint>(stream, count);
				for(vint i=0;i<count;i++)
				{
					WriteStream<K>(stream, value.Keys()[i]);
					StreamBinaryCollectionSerializer<V, collections::IReadonlyList<V>, POD<V>::Result>::Write(stream, value.GetByIndex(i));
				}
			}

			static void Read(IStream& stream, collections::Group<K, V>& value)
			{
				vint count=ReadStream<vint>(stream);
				collections::Array<V> buffer;
				for(vint i=0;i<count;i++)
				{
					K key=ReadStream<K>(stream);
					StreamBinaryCollectionSerializer<V, collections::Array<V>, POD<V>::Result>::ReadArray(stream, buffer);
					for(vint j=0;j<buffer.Count();j++)
					{
						value.Add(key, buffer[j]);
					}
				}
			}
		};
	}
}

#endif