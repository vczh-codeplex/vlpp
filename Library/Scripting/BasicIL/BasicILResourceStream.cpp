#include "BasicILResourceStream.h"

namespace vl
{
	namespace scripting
	{

/***********************************************************************
ResourceStream
***********************************************************************/

		int ResourceStream::CreateRecord(int size)
		{
			int pointer=usedSize;
			usedSize+=size;
			int extendedSize=(usedSize/65536+1)*65536;
			if(resource.Count()!=extendedSize)
			{
				resource.Resize(extendedSize);
			}
			return pointer;
		}

		const char* ResourceStream::GetPointer(int pointer)const
		{
			return &resource[pointer];
		}

		WString ResourceStream::GetString(int pointer)const
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

		ResourceString ResourceStream::CreateString(const WString& string)
		{
			int size=sizeof(wchar_t)*(string.Length()+1);
			int pointer=CreateRecord(size);
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