#include "BasicILResourceStream.h"

namespace vl
{
	namespace scripting
	{
		using namespace collections;

/***********************************************************************
ResourceStream
***********************************************************************/

		ResourceStream::BufferFragment::BufferFragment(vint size)
			:buffer(size)
			,usedSize(0)
		{
		}

		vint ResourceStream::CreateRecord(vint size)
		{
			if(!currentFragment || currentFragment->buffer.Count()-currentFragment->usedSize<size)
			{
				vint extendedSize=(usedSize/65536+1)*65536;
				Ptr<BufferFragment> fragment=new BufferFragment(extendedSize);
				fragments.Add(fragment);
				currentFragment=fragment.Obj();
			}
			vint pointer=usedSize;
			currentFragment->usedSize+=size;
			usedSize+=size;
			return pointer;
		}

		const char* ResourceStream::GetPointer(vint pointer)const
		{
			vint offset=0;
			for(vint i=0;i<fragments.Count();i++)
			{
				BufferFragment* fragment=fragments.Get(i).Obj();
				if(offset<=pointer && pointer<offset+fragment->usedSize)
				{
					return &fragment->buffer[pointer-offset];
				}
				offset+=fragment->usedSize;
			}
			return 0;
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
			,currentFragment(0)
		{
		}

		ResourceStream::~ResourceStream()
		{
		}

		void ResourceStream::LoadFromStream(stream::IStream& stream)
		{
			fragments.Clear();
			currentFragment=0;
			stream.Read(&usedSize, sizeof(usedSize));
			if(usedSize>0)
			{
				vint extendedSize=(usedSize/65536+1)*65536;
				Ptr<BufferFragment> fragment=new BufferFragment(extendedSize);
				fragment->usedSize=usedSize;
				Array<char>& buffer=fragment->buffer;
				stream.Read(&buffer[0], sizeof(buffer[0])*usedSize);

				fragments.Add(fragment);
				currentFragment=fragment.Obj();
			}
		}

		void ResourceStream::SaveToStream(stream::IStream& stream)
		{
			stream.Write(&usedSize, sizeof(usedSize));
			for(vint i=0;i<fragments.Count();i++)
			{
				BufferFragment* fragment=fragments[i].Obj();
				Array<char>& buffer=fragment->buffer;
				stream.Write(&buffer[0], sizeof(buffer[0])*fragment->usedSize);
			}
		}
		
		vint ResourceStream::GetUsedSize()const
		{
			return usedSize;
		}

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