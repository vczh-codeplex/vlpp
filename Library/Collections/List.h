/***********************************************************************
Vczh Library++ 3.0
Developer: 陈梓瀚(vczh)
Data Structure::List

Classes:
	ListStore<T,PODType>				：列表存储复制算法
	ListBase<T,K>						：列表基类
	Array<T,K>							：数组
	List<T,K>							：列表
	SortedList<T,K>						：有序列表
***********************************************************************/

#ifndef VCZH_COLLECTIONS_LIST
#define VCZH_COLLECTIONS_LIST

#include <string.h>
#include "Interfaces.h"
#include "ListWrappers.h"

namespace vl
{
	namespace collections
	{

/***********************************************************************
储存结构
***********************************************************************/

		template<typename T, bool PODType>
		class ListStore abstract : public Object
		{
		};
		
		template<typename T>
		class ListStore<T,false> abstract : public Object
		{
		protected:
			static void CopyObjects(T* dest, const T* source, int count)
			{
				if(dest<source)
				{
					for(int i=0;i<count;i++)
					{
						dest[i]=source[i];
					}
				}
				else if(dest>source)
				{
					for(int i=count-1;i>=0;i--)
					{
						dest[i]=source[i];
					}
				}
			}
		public:
		};
		
		template<typename T>
		class ListStore<T,true> abstract : public Object
		{
		protected:
			static void CopyObjects(T* dest, const T* source, int count)
			{
				if(count)
				{
					memmove(dest, source, sizeof(T)*count);
				}
			}
		public:
		};

		template<typename T, typename K=typename KeyType<T>::Type>
		class ListBase abstract : public ListStore<T,POD<T>::Result>
		{
		protected:
			int						count;
			int						capacity;
			T*						buffer;

			int CalculateCapacity(int expected)
			{
				int result=capacity;
				while(result<expected)
				{
					result=result*5/4+1;
				}
				return result;
			}

			void MakeRoom(int index, int _count)
			{
				int newCount=count+_count;
				if(newCount>capacity)
				{
					int newCapacity=CalculateCapacity(newCount);
					T* newBuffer=new T[newCapacity];
					CopyObjects(newBuffer, buffer, index);
					CopyObjects(newBuffer+index+_count, buffer+index, count-index);
					delete[] buffer;
					capacity=newCapacity;
					buffer=newBuffer;
				}
				else
				{
					CopyObjects(buffer+index+_count, buffer+index, count-index);
				}
				count=newCount;
			}

			void ReleaseUnnecessaryBuffer()
			{
				if(count<=capacity/2)
				{
					int newCapacity=capacity*5/8;
					if(count<newCapacity)
					{
						T* newBuffer=new T[newCapacity];
						CopyObjects(newBuffer, buffer, count);
						delete[] buffer;
						capacity=newCapacity;
						buffer=newBuffer;
					}
				}
			}
		public:
			ListBase()
			{
				count=0;
				capacity=0;
				buffer=0;
			}

			~ListBase()
			{
				delete[] buffer;
			}

			int Count()const
			{
				return count;
			}

			const T& Get(int index)const
			{
				CHECK_ERROR(index>=0 && index<count, L"ListBase<T, K>::Get(int)#参数越界。");
				return buffer[index];
			}

			const T& operator[](int index)const
			{
				CHECK_ERROR(index>=0 && index<count, L"ListBase<T, K>::operator[](int)#参数index越界。");
				return buffer[index];
			}

			bool RemoveAt(int index)
			{
				CHECK_ERROR(index>=0 && index<count, L"ListBase<T, K>::RemoveAt(int)#参数index越界。");
				CopyObjects(buffer+index,buffer+index+1,count-index-1);
				count--;
				ReleaseUnnecessaryBuffer();
				return true;
			}

			bool RemoveRange(int index, int _count)
			{
				CHECK_ERROR(index>=0 && index<=count, L"ListBase<T, K>::RemoveRange(int, int)#参数index越界。");
				CHECK_ERROR(index+_count>=0 && index+_count<=count, L"ListBase<T,K>::RemoveRange(int, int)#参数_count越界。");
				CopyObjects(buffer+index, buffer+index+_count, count-index-_count);
				count-=_count;
				ReleaseUnnecessaryBuffer();
				return true;
			}

			bool Clear()
			{
				count=0;
				capacity=0;
				delete[] buffer;
				buffer=0;
				return true;
			}
		};

/***********************************************************************
列表对象
***********************************************************************/

		template<typename T, typename K=typename KeyType<T>::Type>
		class Array : public ListStore<T, POD<T>::Result>, private NotCopyable
		{
		protected:
			int								count;
			T*								buffer;
			mutable ArrayWrapper<Array<T, K>, T, K>		wrapper;

			void Create(int size)
			{
				if(size>0)
				{
					count=size;
					buffer=new T[size];
				}
				else
				{
					count=0;
					buffer=0;
				}
			}

			void Destroy()
			{
				count=0;
				delete[] buffer;
				buffer=0;
			}
		public:
			Array(int size=0)
			{
				wrapper.SetContainer(this);
				Create(size);
			}

			Array(const T* _buffer, int size)
			{
				wrapper.SetContainer(this);
				Create(size);
				CopyObjects(buffer, _buffer, size);
			}

			~Array()
			{
				Destroy();
			}

			bool Contains(const K& item)const
			{
				return IndexOf(item)!=-1;
			}

			int Count()const
			{
				return count;
			}

			const T& Get(int index)const
			{
				CHECK_ERROR(index>=0 && index<count, L"Array<T, K>::Get(int)#参数越界。");
				return buffer[index];
			}

			const T& operator[](int index)const
			{
				CHECK_ERROR(index>=0 && index<count, L"Array<T, K>::operator[](int)#参数index越界。");
				return buffer[index];
			}

			int IndexOf(const K& item)const
			{
				for(int i=0;i<count;i++)
				{
					if(buffer[i]==item)
					{
						return i;
					}
				}
				return -1;
			}

			void Set(int index, const K& item)
			{
				CHECK_ERROR(index>=0 && index<count, L"Array<T, K>::Set(int)#参数index越界。");
				buffer[index]=item;
			}

			T& operator[](int index)
			{
				CHECK_ERROR(index>=0 && index<count, L"Array<T, K>::operator[](int)#参数index越界。");
				return buffer[index];
			}

			void Resize(int size)
			{
				int oldCount=count;
				T* oldBuffer=buffer;
				Create(size);
				CopyObjects(buffer, oldBuffer, (count<oldCount?count:oldCount));
				delete[] oldBuffer;
			}

			IArray<T, K>& Wrap()const
			{
				return wrapper;
			}
		};

		template<typename T, typename K=typename KeyType<T>::Type>
		class List : public ListBase<T, K>, private NotCopyable
		{
		protected:
			mutable ListWrapper<List<T, K>, T, K>	wrapper;
		public:
			List()
			{
				wrapper.SetContainer(this);
			}

			bool Contains(const K& item)const
			{
				return IndexOf(item)!=-1;
			}

			int IndexOf(const K& item)const
			{
				for(int i=0;i<count;i++)
				{
					if(buffer[i]==item)
					{
						return i;
					}
				}
				return -1;
			}

			int Add(const T& item)
			{
				MakeRoom(count, 1);
				buffer[count-1]=item;
				return count-1;
			}

			int Insert(int index, const T& item)
			{
				CHECK_ERROR(index>=0 && index<=count, L"List<T, K>::Insert(int, const T&)#参数index越界。");
				MakeRoom(index,1);
				buffer[index]=item;
				return index;
			}

			bool Remove(const K& item)
			{
				int index=IndexOf(item);
				if(index>=0 && index<count)
				{
					RemoveAt(index);
					return true;
				}
				else
				{
					return false;
				}
			}

			bool Set(int index, const K& item)
			{
				CHECK_ERROR(index>=0 && index<count, L"List<T, K>::Set(int)#参数index越界。");
				buffer[index]=item;
				return true;
			}

			T& operator[](int index)
			{
				CHECK_ERROR(index>=0 && index<count, L"List<T, K>::operator[](int)#参数index越界。");
				return buffer[index];
			}

			IList<T, K>& Wrap()const
			{
				return wrapper;
			}
		};

		template<typename T, typename K=typename KeyType<T>::Type>
		class SortedList : public ListBase<T, K>, private NotCopyable
		{
		protected:
			mutable CollectionWrapper<SortedList<T, K>, T, K>	wrapper;
		public:
			SortedList()
			{
				wrapper.SetContainer(this);
			}

			bool Contains(const K& item)const
			{
				return IndexOf(item)!=-1;
			}

			template<typename Key>
			int IndexOf(const Key& item)const
			{
				int start=0;
				int end=count-1;
				while(start<=end)
				{
					int index=(start+end)/2;
					if(buffer[index]==item)
					{
						return index;
					}
					else if(buffer[index]>item)
					{
						end=index-1;
					}
					else
					{
						start=index+1;
					}
				}
				return -1;
			}

			int IndexOf(const K& item)const
			{
				return IndexOf<K>(item);
			}

			int Add(const T& item)
			{
				if(count==0)
				{
					MakeRoom(0, 1);
					buffer[0]=item;
					return 0;
				}
				else
				{
					int start=0;
					int end=count-1;
					int index=-1;
					while(start<=end)
					{
						index=(start+end)/2;
						if(buffer[index]==item)
						{
							goto SORTED_LIST_INSERT;
						}
						else if(buffer[index]>item)
						{
							end=index-1;
						}
						else
						{
							start=index+1;
						}
					}
					CHECK_ERROR(index>=0 && index<count, L"SortedList<T, K>::Add(const T&)#内部错误，变量index越界");
					if(buffer[index]<item)
					{
						index++;
					}
SORTED_LIST_INSERT:
					MakeRoom(index, 1);
					buffer[index]=item;
					return index;
				}
			}

			bool Remove(const K& item)
			{
				int index=IndexOf(item);
				if(index>=0 && index<count)
				{
					RemoveAt(index);
					return true;
				}
				else
				{
					return false;
				}
			}

			ICollection<T, K>& Wrap()const
			{
				return wrapper;
			}
		};
	}
}

#endif