/***********************************************************************
Vczh Library++ 3.0
Developer: �����(vczh)
Framework::Exception

Classes:
	Exception									���쳣
	ArgumentException							��Ԥ�ϵĵĲ�������
***********************************************************************/

#ifndef VCZH_EXCEPTION
#define VCZH_EXCEPTION

#include "String.h"

namespace vl
{
	class Exception : public Object
	{
	protected:
		WString						message;

	public:
		Exception(const WString& _message=WString::Empty);

		const WString&				Message()const;
	};

	class ArgumentException : public Exception
	{
	protected:
		WString						function;
		WString						name;

	public:
		ArgumentException(const WString& _message=WString::Empty, const WString& _function=WString::Empty, const WString& _name=WString::Empty);

		const WString&				GetFunction()const;
		const WString&				GetName()const;
	};

	class ParsingException : public Exception
	{
	protected:
		vint							position;
		WString						expression;

	public:
		ParsingException(const WString& _message, const WString& _expression, vint _position);

		const WString&				GetExpression()const;
		vint							GetPosition()const;
	};
}

#endif