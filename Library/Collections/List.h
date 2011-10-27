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
			static void CopyObjects(T* dest, const T* source, vint count)
			{
				if(dest<source)
				{
					for(vint i=0;i<count;i++)
					{
						dest[i]=source[i];
					}
				}
				else if(dest>source)
				{
					for(vint i=count-1;i>=0;i--)
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
			static void CopyObjects(T* dest, const T* source, vint count)
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
			vint						count;
			vint						capacity;
			T*						buffer;
			bool					lessMemoryMode;

			vint CalculateCapacity(vint expected)
			{
				vint result=capacity;
				while(result<expected)
				{
					result=result*5/4+1;
				}
				return result;
			}

			void MakeRoom(vint index, vint _count)
			{
				vint newCount=count+_count;
				if(newCount>capacity)
				{
					vint newCapacity=CalculateCapacity(newCount);
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
				if(lessMemoryMode && count<=capacity/2)
				{
					vint newCapacity=capacity*5/8;
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
				lessMemoryMode=true;
			}

			~ListBase()
			{
				delete[] buffer;
			}

			void SetLessMemoryMode(bool mode)
			{
				lessMemoryMode=mode;
			}

			vint Count()const
			{
				return count;
			}

			const T& Get(vint index)const
			{
				CHECK_ERROR(index>=0 && index<count, L"ListBase<T, K>::Get(vint)#参数越界。");
				return buffer[index];
			}

			const T& operator[](vint index)const
			{
				CHECK_ERROR(index>=0 && index<count, L"ListBase<T, K>::operator[](vint)#参数index越界。");
				return buffer[index];
			}

			bool RemoveAt(vint index)
			{
				CHECK_ERROR(index>=0 && index<count, L"ListBase<T, K>::RemoveAt(vint)#参数index越界。");
				CopyObjects(buffer+index,buffer+index+1,count-index-1);
				count--;
				ReleaseUnnecessaryBuffer();
				return true;
			}

			bool RemoveRange(vint index, vint _count)
			{
				CHECK_ERROR(index>=0 && index<=count, L"ListBase<T, K>::RemoveRange(vint, vint)#参数index越界。");
				CHECK_ERROR(index+_count>=0 && index+_count<=count, L"ListBase<T,K>::RemoveRange(vint, vint)#参数_count越界。");
				CopyObjects(buffer+index, buffer+index+_count, count-index-_count);
				count-=_count;
				ReleaseUnnecessaryBuffer();
				return true;
			}

			bool Clear()
			{
				count=0;
				if(lessMemoryMode)
				{
					capacity=0;
					delete[] buffer;
					buffer=0;
				}
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
			vint								count;
			T*								buffer;
			mutable ArrayWrapper<Array<T, K>, T, K>		wrapper;

			void Create(vint size)
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
			Array(vint size=0)
			{
				wrapper.SetContainer(this);
				Create(size);
			}

			Array(const T* _buffer, vint size)
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

			vint Count()const
			{
				return count;
			}

			const T& Get(vint index)const
			{
				CHECK_ERROR(index>=0 && index<count, L"Array<T, K>::Get(vint)#参数越界。");
				return buffer[index];
			}

			const T& operator[](vint index)const
			{
				CHECK_ERROR(index>=0 && index<count, L"Array<T, K>::operator[](vint)#参数index越界。");
				return buffer[index];
			}

			vint IndexOf(const K& item)const
			{
				for(vint i=0;i<count;i++)
				{
					if(buffer[i]==item)
					{
						return i;
					}
				}
				return -1;
			}

			void Set(vint index, const T& item)
			{
				CHECK_ERROR(index>=0 && index<count, L"Array<T, K>::Set(vint)#参数index越界。");
				buffer[index]=item;
			}

			T& operator[](vint index)
			{
				CHECK_ERROR(index>=0 && index<count, L"Array<T, K>::operator[](vint)#参数index越界。");
				return buffer[index];
			}

			void Resize(vint size)
			{
				vint oldCount=count;
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

			vint IndexOf(const K& item)const
			{
				for(vint i=0;i<count;i++)
				{
					if(buffer[i]==item)
					{
						return i;
					}
				}
				return -1;
			}

			vint Add(const T& item)
			{
				MakeRoom(count, 1);
				buffer[count-1]=item;
				return count-1;
			}

			vint Insert(vint index, const T& item)
			{
				CHECK_ERROR(index>=0 && index<=count, L"List<T, K>::Insert(vint, const T&)#参数index越界。");
				MakeRoom(index,1);
				buffer[index]=item;
				return index;
			}

			bool Remove(const K& item)
			{
				vint index=IndexOf(item);
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

			bool Set(vint index, const T& item)
			{
				CHECK_ERROR(index>=0 && index<count, L"List<T, K>::Set(vint)#参数index越界。");
				buffer[index]=item;
				return true;
			}

			T& operator[](vint index)
			{
				CHECK_ERROR(index>=0 && index<count, L"List<T, K>::operator[](vint)#参数index越界。");
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
			vint IndexOf(const Key& item)const
			{
				vint start=0;
				vint end=count-1;
				while(start<=end)
				{
					vint index=(start+end)/2;
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

			vint IndexOf(const K& item)const
			{
				return IndexOf<K>(item);
			}

			vint Add(const T& item)
			{
				if(count==0)
				{
					MakeRoom(0, 1);
					buffer[0]=item;
					return 0;
				}
				else
				{
					vint start=0;
					vint end=count-1;
					vint index=-1;
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
				vint index=IndexOf(item);
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

/***********************************************************************
容器复制模板
***********************************************************************/

		template<typename A, typename B>
		void CopyToCollection(A& dst, const B& src, bool append=false)
		{
			if(!append)dst.Clear();
			vint count=src.Count();
			for(vint i=0;i<count;i++)
			{
				dst.Add(src.Get(i));
			}
		}

		template<typename A, typename B>
		void CopyToArray(A& dst, const B& src, bool append=false)
		{
			vint start=0;
			vint count=src.Count();
			if(append)
			{
				start=dst.Count();
				dst.Resize(start+count);
			}
			else
			{
				dst.Resize(count);
			}
			for(vint i=0;i<count;i++)
			{
				dst[start+i]=src.Get(i);
			}
		}
	}
}

#endif