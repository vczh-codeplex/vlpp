/***********************************************************************
Vczh Library++ 3.0
Developer: 陈梓瀚(vczh)
Scripting::BasicLanguage

Classes:
	BasicScope									：对象定义域
	BasicEnv									：对象全局环境
***********************************************************************/

#ifndef VCZH_SCRIPTING_BASICLANGUAGE_BASICLANGUAGECODEGENERATOR
#define VCZH_SCRIPTING_BASICLANGUAGE_BASICLANGUAGECODEGENERATOR

#include "..\..\Exception.h"
#include "BasicLanguageExpression.h"
#include "BasicLanguageTypeManager.h"
#include "..\Common\CommonScopeManager.h"

namespace vl
{
	namespace scripting
	{
		namespace basiclanguage
		{

/***********************************************************************
Scope Manager
***********************************************************************/

			class BasicScope : public CommonScope<BasicScope>
			{
				friend class BasicEnv;
				friend class Ptr<BasicScope>;
			protected:
				BasicFunctionDeclaration*											functionDeclaration;
				BasicStatement*														statement;

				void																Initialize();

				BasicScope(BasicScope* _previousScope, BasicFunctionDeclaration* _functionDeclaration);
				BasicScope(BasicScope* _previousScope, BasicStatement* _statement);
				BasicScope(BasicScope* _previousScope);
				BasicScope();
				~BasicScope();
			public:

				BasicFunctionDeclaration*											OwnerDeclaration();
				BasicStatement*														OwnerStatement();

				CommonScopeItems<BasicScope, WString, BasicTypeRecord*>				types;
				CommonScopeItems<BasicScope, WString, BasicTypeRecord*>				variables;
				CommonScopeItems<BasicScope, WString, BasicFunctionDeclaration*>	functions;
			};

			class BasicEnv : public Object, private NotCopyable
			{
				typedef collections::Dictionary<BasicFunctionDeclaration*, BasicScope*>			_FunctionScopeTable;
				typedef collections::Dictionary<BasicFunctionDeclaration*, BasicTypeRecord*>	_FunctionTypeTable;
				typedef collections::Dictionary<BasicStatement*, BasicScope*>					_StatementScopeTable;
				typedef collections::Dictionary<BasicExpression*, BasicTypeRecord*>				_ExpressionTypeTable;
			protected:
				collections::List<Ptr<BasicScope>>								allocatedScopes;
				BasicScope*														globalScope;
				_FunctionScopeTable												functionScopes;
				_StatementScopeTable											statementScopes;
				_FunctionTypeTable												functionTypes;
				_ExpressionTypeTable											expressionTypes;
			public:
				BasicEnv();
				~BasicEnv();

				BasicScope*														GlobalScope();
				BasicScope*														CreateScope(BasicScope* previousScope);
				BasicScope*														CreateFunctionScope(BasicScope* previousScope, BasicFunctionDeclaration* functionDeclaration);
				BasicScope*														CreateStatementScope(BasicScope* previousScope, BasicStatement* statement);
				void															RegisterFunctionType(BasicFunctionDeclaration* function, BasicTypeRecord* type);
				void															RegisterExpressionType(BasicExpression* expression, BasicTypeRecord* type);

				BasicScope*														GetFunctionScope(BasicFunctionDeclaration* function);
				BasicScope*														GetStatementScope(BasicStatement* statement);
				BasicTypeRecord*												GetFunctionType(BasicFunctionDeclaration* function);
				BasicTypeRecord*												GetExpressionType(BasicExpression* expression);
			};

/***********************************************************************
Semantic Input/Output
***********************************************************************/

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
			};

			class BasicSemanticExtension;

			struct BasicAlgorithmConfiguration
			{
				enum IntegerConversion
				{
					SameTypeConversion,
					SameSignConversion,
					FreeConversion
				}			integerOperationConversion;
				bool		treatCharacterAsInteger;
				bool		treatCharAsSignedInteger;
				bool		treatWCharAsSignedInteger;
				bool		treatZeroAsNull;
				bool		enablePointerArithmetic;
				bool		enableImplicitBooleanToIntegerConversion;
				bool		enableImplicitIntegerToBooleanConversion;
				bool		enableImplicitPointerToBooleanConversion;
				bool		enableImplicitFloatToIntegerConversion;
				bool		enableImplicitIntegerToFloatConversion;
				bool		enableImplicitHighToLowPrecisionConversion;
				bool		enableImplicitSignedToUnsignedConversion;
				bool		enableSubscribeOnPointer;

				BasicAlgorithmConfiguration();

				bool		DecodeInteger(BasicPrimitiveTypeEnum type, bool& sign, int& bytes);
				bool		EncodeInteger(BasicPrimitiveTypeEnum& type, bool sign, int bytes);
				bool		DecodeFloat(BasicPrimitiveTypeEnum type, int& bytes);
				bool		EncodeFloat(BasicPrimitiveTypeEnum& type, int bytes);
				bool		CanImplicitConvertTo(BasicPrimitiveTypeEnum from, BasicPrimitiveTypeEnum to);
				bool		CanExplicitConvertTo(BasicPrimitiveTypeEnum from, BasicPrimitiveTypeEnum to);
			};

			typedef class BasicAlgorithmParameter
			{
			public:
				BasicEnv*												env;
				BasicScope*												scope;
				BasicTypeManager*										typeManager;
				collections::List<Ptr<BasicLanguageCodeException>>&		errors;
				collections::SortedList<WString>&						forwardStructures;
				BasicSemanticExtension*									semanticExtension;
				BasicAlgorithmConfiguration								configuration;

				BasicAlgorithmParameter(
					BasicEnv* _env,
					BasicScope* _scope,
					BasicTypeManager* _typeManager,
					collections::List<Ptr<BasicLanguageCodeException>>& _errors,
					collections::SortedList<WString>& _forwardStructures
					);
				BasicAlgorithmParameter(const BasicAlgorithmParameter& bp, BasicScope* _scope);
			} BP;

			class BasicSemanticExtension : public Object, private NotCopyable
			{
			public:
				virtual Ptr<BasicExpression>							ExpressionReplacer(Ptr<BasicExpression> originalExpression, BP& argument);
				virtual Ptr<BasicStatement>								StatementReplacer(Ptr<BasicExpression> originalStatement, BP& argument);
				virtual BasicTypeRecord*								GetTypeRecord(BasicExtendedType* type, const BP& argument);
				virtual void											BuildGlobalScopePass1(BasicExtendedDeclaration* declaration, const BP& argument);
				virtual void											BuildGlobalScopePass2(BasicExtendedDeclaration* declaration, const BP& argument);
				virtual BasicTypeRecord*								GetExpressionType(BasicExtendedExpression* expression, const BP& argument);
			};

/***********************************************************************
Algorithms
***********************************************************************/

			EXTERN_ALGORITHM_FUNCTION(BasicLanguage_GetTypeRecord, BasicType, BP, BasicTypeRecord*)
			EXTERN_ALGORITHM_PROCEDURE(BasicLanguage_BuildGlobalScopePass1, BasicDeclaration, BP)
			EXTERN_ALGORITHM_PROCEDURE(BasicLanguage_BuildGlobalScopePass2, BasicDeclaration, BP)
			extern void BasicLanguage_BuildGlobalScope(Ptr<BasicProgram> program, BP& argument);
			extern BasicTypeRecord* BasicLanguage_GetExpressionType(Ptr<BasicExpression>& expression, BP& argument);
			EXTERN_ALGORITHM_FUNCTION(BasicLanguage_GetExpressionTypeInternal, BasicExpression, BP, BasicTypeRecord*)
		}
	}
}

#endif