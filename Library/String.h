/***********************************************************************
Vczh Library++ 3.0
Developer: 陈梓瀚(vczh)
Data Structure::String

Classes:
	AString										：Mbcs字符串
	WString										：Utf-16字符串
***********************************************************************/

#ifndef VCZH_STRING
#define VCZH_STRING

#include <memory.h>
#include "Basic.h"

namespace vl
{
	template<typename T>
	class ObjectString : public Object
	{
	private:
		static const T	zero=0;

		mutable T*				buffer;
		mutable int*			reference;
		mutable int				start;
		mutable int				length;
		mutable int				realLength;

		static int CalculateLength(const T* buffer)
		{
			int result=0;
			while(*buffer++)result++;
			return result;
		}

		static int Compare(const T* bufA, const ObjectString<T>& strB)
		{
			const T* bufB=strB.buffer+strB.start;
			const T* bufAOld=bufA;
			int length=strB.length;
			while(length-- && *bufA)
			{
				int diff=*bufA++-*bufB++;
				if(diff!=0)
				{
					return diff;
				}
			};
			return CalculateLength(bufAOld)-strB.length;
		}

		static int Compare(const ObjectString<T>& strA, const ObjectString<T>& strB)
		{
			const T* bufA=strA.buffer+strA.start;
			const T* bufB=strB.buffer+strB.start;
			int length=strA.length<strB.length?strA.length:strB.length;
			while(length--)
			{
				int diff=*bufA++-*bufB++;
				if(diff!=0)
				{
					return diff;
				}
			};
			return strA.length-strB.length;
		}

		void Inc()const
		{
			if(reference)
			{
				(*reference)++;
			}
		}

		void Dec()const
		{
			if(reference)
			{
				if(--(*reference)==0)
				{
					delete[] buffer;
					delete reference;
				}
			}
		}

		ObjectString(const ObjectString<T>& string, int _start, int _length)
		{
			if(_length==0)
			{
				buffer=(T*)&zero;
				reference=0;
				start=0;
				length=0;
				realLength=0;
			}
			else
			{
				buffer=string.buffer;
				reference=string.reference;
				start=string.start+_start;
				length=_length;
				realLength=string.realLength;
				Inc();
			}
		}

		ObjectString(const ObjectString<T>& dest, const ObjectString<T>& source, int index, int count)
		{
			if(index==0 && count==dest.length && source.length==0)
			{
				buffer=(T*)&zero;
				reference=0;
				start=0;
				length=0;
				realLength=0;
			}
			else
			{
				reference=new int(1);
				start=0;
				length=dest.length-count+source.length;
				realLength=length;
				buffer=new T[length+1];
				memcpy(buffer, dest.buffer+dest.start, sizeof(T)*index);
				memcpy(buffer+index, source.buffer+source.start, sizeof(T)*source.length);
				memcpy(buffer+index+source.length, (dest.buffer+dest.start+index+count), sizeof(T)*(dest.length-index-count));
				buffer[length]=0;
			}
		}
	public:
		static ObjectString<T>	Empty;

		ObjectString()
		{
			buffer=(T*)&zero;
			reference=0;
			start=0;
			length=0;
			realLength=0;
		}

		ObjectString(const T& _char)
		{
			reference=new int(1);
			start=0;
			length=1;
			buffer=new T[2];
			buffer[0]=_char;
			buffer[1]=0;
			realLength=length;
		}

		ObjectString(const T* _buffer, int _length)
		{
			if(_length==0)
			{
				buffer=(T*)&zero;
				reference=0;
				start=0;
				length=0;
				realLength=0;
			}
			else
			{
				buffer=new T[_length+1];
				memcpy(buffer, _buffer, _length*sizeof(T));
				buffer[_length]=0;
				reference=new int(1);
				start=0;
				length=_length;
				realLength=_length;
			}
		}

		ObjectString(const T* _buffer, bool copy = true)
		{
			CHECK_ERROR(_buffer!=0, L"ObjectString<T>::ObjectString(const T*, bool)#不能用空指针构造字符串。");
			if(copy)
			{
				reference=new int(1);
				start=0;
				length=CalculateLength(_buffer);
				buffer=new T[length+1];
				memcpy(buffer, _buffer, sizeof(T)*(length+1));
				realLength=length;
			}
			else
			{
				buffer=(T*)_buffer;
				reference=0;
				start=0;
				length=CalculateLength(_buffer);
				realLength=length;
			}
		}

		ObjectString(const ObjectString<T>& string)
		{
			buffer=string.buffer;
			reference=string.reference;
			start=string.start;
			length=string.length;
			realLength=string.realLength;
			Inc();
		}

		~ObjectString()
		{
			Dec();
		}

		const T* Buffer()const
		{
			if(start+length!=realLength)
			{
				T* newBuffer=new T[length+1];
				memcpy(newBuffer, buffer+start, sizeof(T)*length);
				newBuffer[length]=0;
				Dec();
				buffer=newBuffer;
				reference=new int(1);
				start=0;
				realLength=length;
			}
			return buffer+start;
		}

		ObjectString<T>& operator=(const ObjectString<T>& string)
		{
			if(this!=&string)
			{
				Dec();
				buffer=string.buffer;
				reference=string.reference;
				start=string.start;
				length=string.length;
				realLength=string.realLength;
				Inc();
			}
			return *this;
		}

		ObjectString<T>& operator+=(const ObjectString<T>& string)
		{
			return *this=*this+string;
		}

		ObjectString<T> operator+(const ObjectString<T>& string)const
		{
			return ObjectString<T>(*this, string, length, 0);
		}

		bool operator==(const ObjectString<T>& string)const
		{
			return Compare(*this, string)==0;
		}

		bool operator!=(const ObjectString<T>& string)const
		{
			return Compare(*this, string)!=0;
		}

		bool operator>(const ObjectString<T>& string)const
		{
			return Compare(*this, string)>0;
		}

		bool operator>=(const ObjectString<T>& string)const
		{
			return Compare(*this, string)>=0;
		}

		bool operator<(const ObjectString<T>& string)const
		{
			return Compare(*this, string)<0;
		}

		bool operator<=(const ObjectString<T>& string)const
		{
			return Compare(*this, string)<=0;
		}

		bool operator==(const T* buffer)const
		{
			return Compare(buffer, *this)==0;
		}

		bool operator!=(const T* buffer)const
		{
			return Compare(buffer, *this)!=0;
		}

		bool operator>(const T* buffer)const
		{
			return Compare(buffer, *this)<0;
		}

		bool operator>=(const T* buffer)const
		{
			return Compare(buffer, *this)<=0;
		}

		bool operator<(const T* buffer)const
		{
			return Compare(buffer, *this)>0;
		}

		bool operator<=(const T* buffer)const
		{
			return Compare(buffer, *this)>=0;
		}

		T operator[](int index)const
		{
			CHECK_ERROR(index>=0 && index<length, L"ObjectString:<T>:operator[](int)#参数index越界。");
			return buffer[start+index];
		}

		int Length()const
		{
			return length;
		}

		ObjectString<T> Left(int count)const
		{
			CHECK_ERROR(count>=0 && count<=length, L"ObjectString<T>::Left(int)#参数count越界。");
			return ObjectString<T>(*this, 0, count);
		}

		ObjectString<T> Right(int count)const
		{
			CHECK_ERROR(count>=0 && count<=length, L"ObjectString<T>::Right(int)#参数count越界。");
			return ObjectString<T>(*this, length-count, count);
		}

		ObjectString<T> Sub(int index, int count)const
		{
			CHECK_ERROR(index>=0 && index<length, L"ObjectString<T>::Sub(int, int)#参数index越界。");
			CHECK_ERROR(index+count>=0 && index+count<=length, L"ObjectString<T>::Sub(int, int)#参数count越界。");
			return ObjectString<T>(*this, index, count);
		}

		ObjectString<T> Remove(int index, int count)const
		{
			CHECK_ERROR(index>=0 && index<length, L"ObjectString<T>::Remove(int, int)#参数index越界。");
			CHECK_ERROR(index+count>=0 && index+count<=length, L"ObjectString<T>::Remove(int, int)#参数count越界。");
			return ObjectString<T>(*this, ObjectString<T>(), index, count);
		}

		ObjectString<T> Insert(int index, const ObjectString<T>& string)const
		{
			CHECK_ERROR(index>=0 && index<=length, L"ObjectString<T>::Insert(int)#参数count越界。");
			return ObjectString<T>(*this, string, index, 0);
		}

		friend bool operator<(const T* left, const ObjectString<T>& right)
		{
			return Compare(left, right)<0;
		}

		friend bool operator<=(const T* left, const ObjectString<T>& right)
		{
			return Compare(left, right)<=0;
		}

		friend bool operator>(const T* left, const ObjectString<T>& right)
		{
			return Compare(left, right)>0;
		}

		friend bool operator>=(const T* left, const ObjectString<T>& right)
		{
			return Compare(left, right)>=0;
		}

		friend bool operator==(const T* left, const ObjectString<T>& right)
		{
			return Compare(left, right)==0;
		}

		friend bool operator!=(const T* left, const ObjectString<T>& right)
		{
			return Compare(left, right)!=0;
		}

		friend ObjectString<T> operator+(const T* left, const ObjectString<T>& right)
		{
			return WString(left, false)+right;
		}
	};

	template<typename T>
	ObjectString<T> ObjectString<T>::Empty=ObjectString<T>();

	typedef ObjectString<char>		AString;
	typedef ObjectString<wchar_t>	WString;

	extern int		atoi(const AString& string);
	extern int		wtoi(const WString& string);
	extern double	atof(const AString& string);
	extern double	wtof(const WString& string);
	extern AString	itoa(int number);
	extern WString	itow(int number);
	extern AString	ftoa(double number);
	extern WString	ftow(double number);
	extern AString	wtoa(const WString& string);
	extern WString	atow(const AString& string);
}

#endif