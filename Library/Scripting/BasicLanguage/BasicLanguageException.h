/***********************************************************************
Vczh Library++ 3.0
Developer: ³Âè÷å«(vczh)
Scripting::BasicLanguage

Classes:
	BasicLanguageCodeException					£ºÓïÒå·ÖÎö´íÎó
***********************************************************************/

#ifndef VCZH_SCRIPTING_BASICLANGUAGE_BASICLANGUAGEEXCEPTION
#define VCZH_SCRIPTING_BASICLANGUAGE_BASICLANGUAGEEXCEPTION

#include "..\..\Exception.h"
#include "BasicLanguageExpression.h"

namespace vl
{
	namespace scripting
	{
		namespace basiclanguage
		{
			class BasicLanguageCodeException : public Exception
			{
			public:
				enum ExceptionCode
				{
					TypeNameNotExists,
					FunctionAlreadyExists,
					VariableAlreadyExists,
					TypeAlreadyExists,
					StructureMemberAlreadyExists,
					VariableNotExists,
					FailToCast,
					VoidFunctionDoesNotHaveResult,
				};
			protected:
				BasicLanguageElement*											element;
				ExceptionCode													exceptionCode;
				collections::List<WString>										parameters;
			public:
				BasicLanguageCodeException(BasicLanguageElement* _element, ExceptionCode _exceptionCode, const collections::IReadonlyList<WString>& _parameters);
				BasicLanguageCodeException(const BasicLanguageCodeException& exception);
				~BasicLanguageCodeException();

				BasicLanguageElement*											GetBasicLanguageElement()const;
				ExceptionCode													GetExceptionCode()const;
				const collections::IReadonlyList<WString>&						GetParameters()const;

				static Ptr<BasicLanguageCodeException>							GetTypeNameNotExists(BasicReferenceType* type);
				static Ptr<BasicLanguageCodeException>							GetFunctionAlreadyExists(BasicFunctionDeclaration* function);
				static Ptr<BasicLanguageCodeException>							GetVariableAlreadyExists(BasicVariableDeclaration* variable);
				static Ptr<BasicLanguageCodeException>							GetTypeAlreadyExists(BasicStructureDeclaration* type);
				static Ptr<BasicLanguageCodeException>							GetTypeAlreadyExists(BasicTypeRenameDeclaration* type);
				static Ptr<BasicLanguageCodeException>							GetStructureMemberAlreadyExists(BasicStructureDeclaration* type, int memberIndex);
				static Ptr<BasicLanguageCodeException>							GetVariableNotExists(BasicReferenceExpression* variable);
				static Ptr<BasicLanguageCodeException>							GetFailToCast(BasicCastingExpression* casting);
				static Ptr<BasicLanguageCodeException>							GetVoidFunctionDoesNotHaveResult(BasicFunctionResultExpression* result);
			};
		}
	}
}

#endif