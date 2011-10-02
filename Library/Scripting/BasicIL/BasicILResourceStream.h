/***********************************************************************
Vczh Library++ 3.0
Developer: 陈梓瀚(vczh)
Scripting::BasicIL

Classes:
  ResourceHandle<T>						：POD资源记录句柄
  ResourceString<T>						：字符串资源句柄
  ResourceArrayHandle<T>				：数组资源句柄
  ResourceRecord<T>						：POD资源记录引用
  ResourceArrayRecord<T>				：数组资源引用
  ResourceStream						：POD资源流

使用方法：

如果想存放以下类型：
struct Node
{
	vint data;
	WString name;
	Node* next;
};
修改成
struct Node
{
	vint data;
	ResourceString name;
	ResourceHandle<Node> next;
};

创建Node对象：
ResourceRecord<Node> node=resourceStream.CreateRecord<Node>();

修改Node对象：
Node->data=0;

//将ResourceRecord<Node>赋值给ResourceHandle<Node>
Node->next=anotherNode;

//将空句柄赋值给ResourceHandle<Node>
Node->next=ResourceHandle<Node>::Null();

//将字符串赋值给ResourceString<wchar_t>
ResourceString name=resourceStream.CreateString<wchar_t>(L"vczh");
Node->name=name;

//将空字符串赋值给ResourceString<wchar_t>
Node->name=ResourceString<wchar_t>::Null();

读取资源流的第一个对象：
ResourceRecord<Node> node=resourceStream.ReadRootRecord<Node>();

通过句柄获得对象：
ResourceRecord<Node> nextNode=resourceStream.ReadRecord(node->next);
WString name=resourceStream.ReadString(node->name);
***********************************************************************/

#ifndef VCZH_SCRIPTING_BASICIL_BASICILRESOURCESTREAM
#define VCZH_SCRIPTING_BASICIL_BASICILRESOURCESTREAM

#include "..\..\String.h"
#include "..\..\Pointer.h"
#include "..\..\Collections\List.h"
#include "..\..\Stream\Interfaces.h"
#include "..\..\Stream\StreamSerialization.h"

namespace vl
{
	namespace scripting
	{
		class ResourceStream;

/***********************************************************************
句柄
***********************************************************************/

		class ResourceReference
		{
		protected:
			vint								pointer;

			ResourceReference()
				:pointer(-1)
			{
			}
		public:
			operator bool()const
			{
				return pointer!=-1;
			}

			vint Pointer()const
			{
				return pointer;
			}
		};

		template<typename T>
		class ResourceHandle : public ResourceReference
		{
			friend class ResourceStream;
			template<typename T>
			friend class ResourceHandle;
			template<typename T>
			friend class ResourceRecord;
		public:
			typedef T ResourceType;

			template<typename U>
			ResourceHandle<U> Cast()const
			{
				ResourceHandle<U> handle;
				handle.pointer=pointer;
				return handle;
			}

			static ResourceHandle<T> Null()
			{
				return ResourceHandle<T>();
			}
		};

		class ResourceString : public ResourceReference
		{
			friend class ResourceStream;
		public:

			static ResourceString Null()
			{
				return ResourceString();
			}
		};

		template<typename T>
		class ResourceArrayHandle : public ResourceReference
		{
			friend class ResourceStream;
			template<typename T>
			friend class ResourceArrayRecord;
		public:
			typedef T ResourceType;

			static ResourceArrayHandle<T> Null()
			{
				return ResourceArrayHandle<T>();
			}
		};

/***********************************************************************
读写对象
***********************************************************************/

		template<typename T>
		class ResourceRecord
		{
		protected:
			const ResourceStream*			resourceStream;
			vint							pointer;
		public:
			ResourceRecord()
				:resourceStream(0)
				,pointer(-1)
			{
			}

			ResourceRecord(const ResourceRecord<T>& record)
				:resourceStream(record.resourceStream)
				,pointer(record.pointer)
			{
			}

			ResourceRecord(const ResourceStream* _resourceStream, vint _pointer)
				:resourceStream(_pointer==-1?0:_resourceStream)
				,pointer(_pointer)
			{
			}

			ResourceRecord& operator=(const ResourceRecord<T>& record)
			{
				resourceStream=record.resourceStream;
				pointer=record.pointer;
				return *this;
			}

			ResourceStream* GetResourceStream()
			{
				return resourceStream;
			}

			T* operator->()const
			{
				return (T*)resourceStream->GetPointer(pointer);
			}

			template<typename U>
			ResourceRecord<U> Cast()const
			{
				ResourceHandle<T> handle=*this;
				return resourceStream->ReadRecord(handle.Cast<U>());
			}

			operator ResourceHandle<T>()const
			{
				ResourceHandle<T> handle;
				handle.pointer=pointer;
				return handle;
			}

			operator bool()const
			{
				return pointer!=-1;
			}

			vint Pointer()const
			{
				return pointer;
			}
		};

		template<typename T>
		class ResourceArrayRecord
		{
		protected:
			const ResourceStream*			resourceStream;
			vint							pointer;
		public:
			class ElementReference
			{
			protected:
				const ResourceArrayRecord<T>*	record;
				vint							index;
			public:
				ElementReference(const ResourceArrayRecord<T>* _record, vint _index)
					:record(_record)
					,index(_index)
				{
				}

				operator ResourceRecord<T>()const
				{
					return record->Get(index);
				}

				void operator=(const ResourceHandle<T> handle)const
				{
					record->Set(index, handle);
				}
			};

			ResourceArrayRecord()
				:resourceStream(0)
				,pointer(-1)
			{
			}

			ResourceArrayRecord(const ResourceArrayRecord<T>& record)
				:resourceStream(record.resourceStream)
				,pointer(record.pointer)
			{
			}

			ResourceArrayRecord(const ResourceStream* _resourceStream, vint _pointer)
				:resourceStream(_pointer==-1?0:_resourceStream)
				,pointer(_pointer)
			{
			}

			ResourceArrayRecord& operator=(const ResourceArrayRecord<T>& record)
			{
				resourceStream=record.resourceStream;
				pointer=record.pointer;
				return *this;
			}

			ResourceStream* GetResourceStream()
			{
				return resourceStream;
			}

			vint Count()const
			{
				vint* buffer=(vint*)resourceStream->GetPointer(pointer);
				return buffer[0];
			}

			ResourceRecord<T> Get(vint index)const
			{
				vint* buffer=(vint*)resourceStream->GetPointer(pointer);
				CHECK_ERROR(index>=0 && index<buffer[0], L"ResourceArrayRecord<T>::operator[](vint)#参数index越界。");
				CHECK_ERROR(pointer+(vint)(sizeof(vint)+sizeof(T)*(*buffer))<=resourceStream->GetUsedSize(), L"ResourceArrayRecord<T>::operator[](vint)#参数index越界。");
				return ResourceRecord<T>(resourceStream, buffer[index+1]);
			}

			void Set(vint index, const ResourceHandle<T> handle)const
			{
				vint* buffer=(vint*)resourceStream->GetPointer(pointer);
				CHECK_ERROR(index>=0 && index<buffer[0], L"ResourceArrayRecord<T>::operator[](vint)#参数index越界。");
				CHECK_ERROR(pointer+(vint)(sizeof(vint)+sizeof(T)*(*buffer))<=resourceStream->GetUsedSize(), L"ResourceArrayRecord<T>::operator[](vint)#参数index越界。");
				buffer[index+1]=handle.Pointer();
			}

			const ElementReference operator[](vint index)const
			{
				return ElementReference(this, index);
			}

			operator ResourceArrayHandle<T>()const
			{
				ResourceArrayHandle<T> handle;
				handle.pointer=pointer;
				return handle;
			}

			operator bool()const
			{
				return pointer!=-1;
			}

			vint Pointer()const
			{
				return pointer;
			}

			vint CopyFromList(const collections::IReadonlyList<ResourceHandle<T>>& src)const
			{
				vint dstCount=Count();
				vint srcCount=src.Count();
				vint minCount=dstCount<srcCount?dstCount:srcCount;
				for(vint i=0;i<minCount;i++)
				{
					Set(i, src[i]);
				}
				return minCount;
			}
		};

/***********************************************************************
资源流
***********************************************************************/

		class ResourceStream : public Object
		{
			template<typename T>
			friend class ResourceRecord;
			template<typename T>
			friend class ResourceArrayRecord;
		public:
			struct BufferFragment
			{
				typedef collections::List<Ptr<BufferFragment>> List;

				collections::Array<char>	buffer;
				vint						usedSize;

				BufferFragment(vint size);
			};
		protected:

			BufferFragment::List			fragments;
			BufferFragment*					currentFragment;
			vint							usedSize;

			vint							CreateRecord(vint size);
			const char*						GetPointer(vint pointer)const;
			WString							GetString(vint pointer)const;
			WString							GetEmptyString()const;

		public:
			ResourceStream();
			~ResourceStream();

			void							LoadFromStream(stream::IStream& stream);
			void							SaveToStream(stream::IStream& stream);
			vint							GetUsedSize()const;

			ResourceString					CreateString(const WString& string);
			WString							ReadString(ResourceString string)const;

			//------------------------------------------------------------------------------

			template<typename T>
			ResourceRecord<T> CreateRecord()
			{
				return ResourceRecord<T>(this, CreateRecord(sizeof(T)));
			}

			template<typename T>
			ResourceRecord<T> ReadRecord(vint pointer)const
			{
				CHECK_ERROR(pointer==-1 || (pointer>=0 && (pointer+(vint)sizeof(T))<=usedSize), L"ResourceStream::ReadRecord<T>(vint)#参数pointer越界。");
				return ResourceRecord<T>(this, pointer);
			}

			template<typename T>
			ResourceRecord<T> ReadRecord(ResourceHandle<T> handler)const
			{
				return ReadRecord<T>(handler.pointer);
			}

			template<typename T>
			ResourceRecord<T> ReadRootRecord()const
			{
				return ReadRecord<T>(0);
			}

			//------------------------------------------------------------------------------

			template<typename T>
			ResourceArrayRecord<T> CreateArrayRecord(vint count)
			{
				vint pointer=CreateRecord((count+1)*sizeof(T));
				*(vint*)GetPointer(pointer)=count;
				return ResourceArrayRecord<T>(this, pointer);
			}

			template<typename T>
			ResourceArrayRecord<T> CreateArrayRecord(const collections::IReadonlyList<ResourceHandle<T>>& src)
			{
				ResourceArrayRecord<T> result=CreateArrayRecord<T>(src.Count());
				result.CopyFromList(src);
				return result;
			}

			template<typename T>
			ResourceArrayRecord<T> ReadArrayRecord(vint pointer)const
			{
				CHECK_ERROR(pointer==-1 || (pointer>=0 && (pointer+(vint)sizeof(T))<=usedSize), L"ResourceStream::ReadArrayRecord<T>(vint)#参数pointer越界。");
				return ResourceArrayRecord<T>(this, pointer);
			}

			template<typename T>
			ResourceArrayRecord<T> ReadArrayRecord(ResourceArrayHandle<T> handler)const
			{
				return ReadArrayRecord<T>(handler.pointer);
			}

			template<typename T>
			ResourceArrayRecord<T> ReadRootArrayRecord()const
			{
				return ReadArrayRecord<T>(0);
			}
		};
	}
}

/***********************************************************************
二进制序列化
***********************************************************************/

namespace vl
{
	namespace stream
	{
		template<>
		class StreamBinarySerializer<Ptr<scripting::ResourceStream>>
		{
		public:
			static void Write(IStream& stream, const Ptr<scripting::ResourceStream>& value)
			{
				value->SaveToStream(stream);
			}

			static void Read(IStream& stream, Ptr<scripting::ResourceStream>& value)
			{
				if(!value)
				{
					value=new scripting::ResourceStream;
				}
				value->LoadFromStream(stream);
			}
		};
	}
}

#endif