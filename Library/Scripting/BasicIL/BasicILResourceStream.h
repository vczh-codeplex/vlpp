/***********************************************************************
Vczh Library++ 3.0
Developer: 陈梓瀚(vczh)
Scripting::BasicIL

Classes:
  ResourceHandle<T>						：POD资源记录句柄
  ResourceRecord<T>						：POD资源记录引用
  ResourceStream						：POD资源流

警告：
假设
struct Node
{
	ResourceString name;
};
ResourceRecord<node> node=resourceStream.CreateRecord<Node>();
使用node->name=resourceStream.CreateString(L"vczh")会因为operator->比CreateString先执行导致ResourceString对象被写进一个野指针。

使用方法：

如果想存放以下类型：
struct Node
{
	int data;
	WString name;
	Node* next;
};
修改成
struct Node
{
	int data;
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
#include "..\..\Collections\List.h"

namespace vl
{
	namespace scripting
	{
		class ResourceStream;

		class ResourceReference
		{
		protected:
			int								pointer;

			ResourceReference()
				:pointer(-1)
			{
			}
		public:
			operator bool()const
			{
				return pointer!=-1;
			}

			int Pointer()const
			{
				return pointer;
			}
		};

		template<typename T>
		class ResourceHandle : public ResourceReference
		{
			friend class ResourceStream;
			template<typename T>
			friend class ResourceRecord;
		public:
			typedef T ResourceType;

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
		class ResourceRecord
		{
		protected:
			const ResourceStream*			resourceStream;
			int								pointer;
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

			ResourceRecord(const ResourceStream* _resourceStream, int _pointer)
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

			T* operator->()const
			{
				return (T*)resourceStream->GetPointer(pointer);
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

			int Pointer()const
			{
				return pointer;
			}
		};

		class ResourceStream : public Object
		{
			template<typename T>
			friend class ResourceRecord;
		protected:
			collections::Array<char>		resource;
			int								usedSize;

			int								CreateRecord(int size);
			const char*						GetPointer(int pointer)const;
			WString							GetString(int pointer)const;
			WString							GetEmptyString()const;

		public:
			ResourceStream();
			~ResourceStream();

			ResourceString					CreateString(const WString& string);
			WString							ReadString(ResourceString string)const;

			template<typename T>
			ResourceRecord<T> CreateRecord()
			{
				return ResourceRecord<T>(this, CreateRecord(sizeof(T)));
			}

			template<typename T>
			ResourceRecord<T> ReadRecord(int pointer)const
			{
				CHECK_ERROR(pointer==-1 || (pointer>=0 && (pointer+(int)sizeof(T))<=resource.Count()), L"ResourceStream::ReadRecord<T>(int)#参数pointer越界。");
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
		};
	}
}

#endif