/***********************************************************************
Vczh Library++ 3.0
Developer: �����(vczh)
Scripting::BasicIL

Classes:
  ResourceHandle<T>						��POD��Դ��¼���
  ResourceRecord<T>						��POD��Դ��¼����
  ResourceStream						��POD��Դ��

���棺
����
struct Node
{
	ResourceString name;
};
ResourceRecord<node> node=resourceStream.CreateRecord<Node>();
ʹ��node->name=resourceStream.CreateString(L"vczh")����Ϊoperator->��CreateString��ִ�е���ResourceString����д��һ��Ұָ�롣

ʹ�÷�����

��������������ͣ�
struct Node
{
	vint data;
	WString name;
	Node* next;
};
�޸ĳ�
struct Node
{
	vint data;
	ResourceString name;
	ResourceHandle<Node> next;
};

����Node����
ResourceRecord<Node> node=resourceStream.CreateRecord<Node>();

�޸�Node����
Node->data=0;

//��ResourceRecord<Node>��ֵ��ResourceHandle<Node>
Node->next=anotherNode;

//���վ����ֵ��ResourceHandle<Node>
Node->next=ResourceHandle<Node>::Null();

//���ַ�����ֵ��ResourceString<wchar_t>
ResourceString name=resourceStream.CreateString<wchar_t>(L"vczh");
Node->name=name;

//�����ַ�����ֵ��ResourceString<wchar_t>
Node->name=ResourceString<wchar_t>::Null();

��ȡ��Դ���ĵ�һ������
ResourceRecord<Node> node=resourceStream.ReadRootRecord<Node>();

ͨ�������ö���
ResourceRecord<Node> nextNode=resourceStream.ReadRecord(node->next);
WString name=resourceStream.ReadString(node->name);
***********************************************************************/

#ifndef VCZH_SCRIPTING_BASICIL_BASICILRESOURCESTREAM
#define VCZH_SCRIPTING_BASICIL_BASICILRESOURCESTREAM

#include "..\..\String.h"
#include "..\..\Collections\List.h"
#include "..\..\Stream\Interfaces.h"

namespace vl
{
	namespace scripting
	{
		class ResourceStream;

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
			vint								pointer;
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

		class ResourceStream : public Object
		{
			template<typename T>
			friend class ResourceRecord;
		protected:
			collections::Array<char>		resource;
			vint								usedSize;

			vint								CreateRecord(vint size);
			const char*						GetPointer(vint pointer)const;
			WString							GetString(vint pointer)const;
			WString							GetEmptyString()const;

		public:
			ResourceStream();
			~ResourceStream();

			void							LoadFromStream(stream::IStream& stream);
			void							SaveToStream(stream::IStream& stream);

			ResourceString					CreateString(const WString& string);
			WString							ReadString(ResourceString string)const;

			template<typename T>
			ResourceRecord<T> CreateRecord()
			{
				return ResourceRecord<T>(this, CreateRecord(sizeof(T)));
			}

			template<typename T>
			ResourceRecord<T> ReadRecord(vint pointer)const
			{
				CHECK_ERROR(pointer==-1 || (pointer>=0 && (pointer+(vint)sizeof(T))<=resource.Count()), L"ResourceStream::ReadRecord<T>(vint)#����pointerԽ�硣");
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