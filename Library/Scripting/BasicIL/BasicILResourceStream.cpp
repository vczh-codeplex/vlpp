#include "BasicILResourceStream.h"

namespace vl
{
	namespace scripting
	{
		using namespace collections;

/***********************************************************************
ResourceStream
***********************************************************************/

		vint ResourceStream::CreateRecord(vint size)
		{
			vint pointer=usedSize;
			usedSize+=size;
			vint extendedSize=(usedSize/65536+1)*65536;
			if(resource.Count()!=extendedSize)
			{
				resource.Resize(extendedSize);
			}
			return pointer;
		}

		const char* ResourceStream::GetPointer(vint pointer)const
		{
			return &resource[pointer];
		}

		WString ResourceStream::GetString(vint pointer)const
		{
			return WString((const wchar_t*)GetPointer(pointer));
		}

		WString ResourceStream::GetEmptyString()const
		{
			return WString();
		}

		ResourceStream::ResourceStream()
			:usedSize(0)
		{
		}

		ResourceStream::~ResourceStream()
		{
		}

		// Deserialization Begin

		vint ReadInt(stream::IStream& stream)
		{
			vint result=0;
			stream.Read(&result, sizeof(result));
			return result;
		}

		template<typename T>
		void ReadArray(stream::IStream& stream, Array<T>& collection)
		{
			collection.Resize(ReadInt(stream));
			if(collection.Count()>0)
			{
				stream.Read(&collection[0], sizeof(T)*collection.Count());
			}
		}

		void ResourceStream::LoadFromStream(stream::IStream& stream)
		{
			ReadArray(stream, resource);
			usedSize=resource.Count();
		}

		// Deserialization End

		// Serialization Begin

		void WriteInt(stream::IStream& stream, vint i)
		{
			stream.Write(&i, sizeof(i));
		}

		template<typename T>
		void WriteArray(stream::IStream& stream, T& collection, vint count)
		{
			WriteInt(stream, count);
			if(count>0)
			{
				stream.Write(&collection[0], sizeof(collection[0])*count);
			}
		}

		void ResourceStream::SaveToStream(stream::IStream& stream)
		{
			WriteArray(stream, resource, usedSize);
		}

		// Serialization End

		ResourceString ResourceStream::CreateString(const WString& string)
		{
			vint size=sizeof(wchar_t)*(string.Length()+1);
			vint pointer=CreateRecord(size);
			if(string.Length()==0)
			{
				*((wchar_t*)GetPointer(pointer))=0;
			}
			else
			{
				wcscpy_s((wchar_t*)GetPointer(pointer), string.Length()+1, string.Buffer());
			}
			ResourceString result;
			result.pointer=pointer;
			return result;
		}

		WString ResourceStream::ReadString(ResourceString string)const
		{
			return string?GetString(string.pointer):GetEmptyString();
		}
	}
}