/***********************************************************************
Vczh Library++ 3.0
Developer: ³Âè÷å«(vczh)
Scripting::BasicLanguage

Classes:
***********************************************************************/

#ifndef VCZH_SCRIPTING_BASICLANGUAGE_BASICLANGUAGEWRITER
#define VCZH_SCRIPTING_BASICLANGUAGE_BASICLANGUAGEWRITER

#include "BasicLanguageExpression.h"

namespace vl
{
	namespace scripting
	{
		namespace basiclanguage
		{

/***********************************************************************
Type
***********************************************************************/

			class BasicTypeNode : public Object
			{
			protected:
				Ptr<BasicType>							type;
			public:
				class ListNode
				{
				public:
					Ptr<collections::List<Ptr<BasicType>>>			types;

					ListNode&							operator<<(const BasicTypeNode& type);
				};

				BasicTypeNode(Ptr<BasicType> _type);
				Ptr<BasicType>							GetInternalValue()const;

				BasicTypeNode							operator*()const;
				BasicTypeNode							operator[](vint size)const;
				BasicTypeNode							operator[](const ListNode& types)const;
				BasicTypeNode							operator()(const ListNode& types)const;
			};

			extern BasicTypeNode						t_int8();
			extern BasicTypeNode						t_int16();
			extern BasicTypeNode						t_int32();
			extern BasicTypeNode						t_int64();
			extern BasicTypeNode						t_int();
			extern BasicTypeNode						t_uint8();
			extern BasicTypeNode						t_uint16();
			extern BasicTypeNode						t_uint32();
			extern BasicTypeNode						t_uint64();
			extern BasicTypeNode						t_uint();
			extern BasicTypeNode						t_float();
			extern BasicTypeNode						t_double();
			extern BasicTypeNode						t_bool();
			extern BasicTypeNode						t_char();
			extern BasicTypeNode						t_wchar();
			extern BasicTypeNode						t_void();
			extern BasicTypeNode						t_type(const WString& type);
			extern BasicTypeNode::ListNode				t_types();

/***********************************************************************
Expression
***********************************************************************/

			class BasicExpressionNode : public Object
			{
			protected:
				Ptr<BasicExpression>					expression;
			public:
				class ListNode
				{
				public:
					Ptr<collections::List<Ptr<BasicExpression>>>	expressions;

					ListNode&							operator<<(const BasicExpressionNode& expression);
				};

				BasicExpressionNode(Ptr<BasicExpression> _expression);
				Ptr<BasicExpression>					GetInternalValue()const;

				BasicExpressionNode						operator++()const;
				BasicExpressionNode						operator--()const;
				BasicExpressionNode						operator++(int)const;
				BasicExpressionNode						operator--(int)const;
				BasicExpressionNode						Ref()const;
				BasicExpressionNode						operator*()const;
				BasicExpressionNode						operator-()const;
				BasicExpressionNode						operator!()const;
				BasicExpressionNode						operator~()const;

				BasicExpressionNode						operator+(const BasicExpressionNode& node)const;
				BasicExpressionNode						operator-(const BasicExpressionNode& node)const;
				BasicExpressionNode						operator*(const BasicExpressionNode& node)const;
				BasicExpressionNode						operator/(const BasicExpressionNode& node)const;
				BasicExpressionNode						operator%(const BasicExpressionNode& node)const;
				BasicExpressionNode						operator<<(const BasicExpressionNode& node)const;
				BasicExpressionNode						operator>>(const BasicExpressionNode& node)const;
				BasicExpressionNode						operator&(const BasicExpressionNode& node)const;
				BasicExpressionNode						operator|(const BasicExpressionNode& node)const;
				BasicExpressionNode						operator^(const BasicExpressionNode& node)const;
				BasicExpressionNode						operator&&(const BasicExpressionNode& node)const;
				BasicExpressionNode						operator||(const BasicExpressionNode& node)const;

				BasicExpressionNode						operator+=(const BasicExpressionNode& node)const;
				BasicExpressionNode						operator-=(const BasicExpressionNode& node)const;
				BasicExpressionNode						operator*=(const BasicExpressionNode& node)const;
				BasicExpressionNode						operator/=(const BasicExpressionNode& node)const;
				BasicExpressionNode						operator%=(const BasicExpressionNode& node)const;
				BasicExpressionNode						operator<<=(const BasicExpressionNode& node)const;
				BasicExpressionNode						operator>>=(const BasicExpressionNode& node)const;
				BasicExpressionNode						operator&=(const BasicExpressionNode& node)const;
				BasicExpressionNode						operator|=(const BasicExpressionNode& node)const;
				BasicExpressionNode						operator^=(const BasicExpressionNode& node)const;
				BasicExpressionNode						Assign(const BasicExpressionNode& node)const;

				BasicExpressionNode						operator<(const BasicExpressionNode& node)const;
				BasicExpressionNode						operator<=(const BasicExpressionNode& node)const;
				BasicExpressionNode						operator>(const BasicExpressionNode& node)const;
				BasicExpressionNode						operator>=(const BasicExpressionNode& node)const;
				BasicExpressionNode						operator==(const BasicExpressionNode& node)const;
				BasicExpressionNode						operator!=(const BasicExpressionNode& node)const;

				BasicExpressionNode						Member(const WString& member)const;
				BasicExpressionNode						PMember(const WString& member)const;
				BasicExpressionNode						operator[](const BasicExpressionNode& subscribe)const;
				BasicExpressionNode						operator[](const BasicTypeNode& castType)const;
				BasicExpressionNode						operator()(const ListNode& arguments)const;
			};

			extern BasicExpressionNode					e_prim(unsigned __int8 value);
			extern BasicExpressionNode					e_prim(unsigned __int16 value);
			extern BasicExpressionNode					e_prim(unsigned __int32 value);
			extern BasicExpressionNode					e_prim(unsigned __int64 value);
			extern BasicExpressionNode					e_prim(signed __int8 value);
			extern BasicExpressionNode					e_prim(signed __int16 value);
			extern BasicExpressionNode					e_prim(signed __int32 value);
			extern BasicExpressionNode					e_prim(signed __int64 value);
			extern BasicExpressionNode					e_prim(float value);
			extern BasicExpressionNode					e_prim(double value);
			extern BasicExpressionNode					e_prim(bool value);
			extern BasicExpressionNode					e_prim(char value);
			extern BasicExpressionNode					e_prim(wchar_t value);
			extern BasicExpressionNode					e_prim(const WString& value);
			extern BasicExpressionNode					e_prim(const AString& value);
			extern BasicExpressionNode					e_prim(const wchar_t* value);
			extern BasicExpressionNode					e_prim(const char* value);
			extern BasicExpressionNode					e_null();
			extern BasicExpressionNode					e_exception();
			extern BasicExpressionNode					e_name(const WString& name);
			extern BasicExpressionNode					e_name(const WString& name, const BasicTypeNode::ListNode& types);
			extern BasicExpressionNode					e_name(const WString& conceptName, const BasicTypeNode& type, const WString& functionName);
			extern BasicExpressionNode					e_result();
			extern BasicExpressionNode::ListNode		e_exps();

/***********************************************************************
Statement
***********************************************************************/

			class BasicStatementNode : public Object
			{
			protected:
				Ptr<BasicStatement>						statement;
			public:
				BasicStatementNode(Ptr<BasicStatement> _statement);
				Ptr<BasicStatement>						GetInternalValue()const;

				BasicStatementNode						operator<<(const BasicStatementNode& nextStatement)const;
			};

			extern BasicStatementNode					s_expr(const BasicExpressionNode& expression);
			extern BasicStatementNode					s_var(const BasicTypeNode& type, const WString& name);
			extern BasicStatementNode					s_var(const BasicTypeNode& type, const WString& name, const BasicExpressionNode& initializer);
			extern BasicStatementNode					s_if(const BasicExpressionNode& condition, const BasicStatementNode& trueStatement);
			extern BasicStatementNode					s_if(const BasicExpressionNode& condition, const BasicStatementNode& trueStatement, const BasicStatementNode& falseStatement);
			extern BasicStatementNode					s_while(const BasicExpressionNode& condition, const BasicStatementNode& statement);
			extern BasicStatementNode					s_do_while(const BasicExpressionNode& condition, const BasicStatementNode& statement);
			extern BasicStatementNode					s_loop(const BasicStatementNode& statement);
			extern BasicStatementNode					s_conditional_loop(const BasicExpressionNode& beginCondition, const BasicExpressionNode& endCondition, const BasicStatementNode& statement);
			extern BasicStatementNode					s_break();
			extern BasicStatementNode					s_continue();
			extern BasicStatementNode					s_return();
			extern BasicStatementNode					s_empty();
			extern BasicStatementNode					s_for(const BasicStatementNode& initializer, const BasicExpressionNode& condition, const BasicStatementNode& sideEffect, const BasicStatementNode& statement);
			extern BasicStatementNode					s_try_catch(const BasicStatementNode& tryStatement, const BasicStatementNode& catchStatement);
			extern BasicStatementNode					s_throw();
			extern BasicStatementNode					s_throw(const BasicExpressionNode& expression);

/***********************************************************************
Declaration
***********************************************************************/

			class BasicFunctionDeclarationNode : public Object
			{
			protected:
				Ptr<BasicFunctionDeclaration>			declaration;
			public:
				BasicFunctionDeclarationNode(Ptr<BasicFunctionDeclaration> _declaration);
				Ptr<BasicFunctionDeclaration>			GetInternalValue();

				BasicFunctionDeclarationNode&			ReturnType(const BasicTypeNode& type);
				BasicFunctionDeclarationNode&			Parameter(const WString& name, const BasicTypeNode& type);
				BasicFunctionDeclarationNode&			Linking(const WString& assemblyName, const WString& symbolName);
				BasicFunctionDeclarationNode&			Foreign();
				BasicFunctionDeclarationNode&			Statement(const BasicStatementNode& statement);
			};

			class BasicStructureDeclarationNode : public Object
			{
			protected:
				Ptr<BasicStructureDeclaration>			declaration;
			public:
				BasicStructureDeclarationNode(Ptr<BasicStructureDeclaration> _declaration);
				Ptr<BasicStructureDeclaration>			GetInternalValue();
				
				BasicStructureDeclarationNode&			Member(const WString& name, const BasicTypeNode& type);
				BasicStructureDeclarationNode&			Linking(const WString& assemblyName, const WString& symbolName);
			};

			class BasicConceptBaseDeclarationNode : public Object
			{
			protected:
				Ptr<BasicConceptBaseDeclaration>		declaration;
			public:
				BasicConceptBaseDeclarationNode(Ptr<BasicConceptBaseDeclaration> _declaration);
				Ptr<BasicConceptBaseDeclaration>		GetInternalValue();
				
				BasicConceptBaseDeclarationNode&		Member(const WString& name, const BasicTypeNode& type);
				BasicConceptBaseDeclarationNode&		Linking(const WString& assemblyName, const WString& symbolName);
			};

			class BasicConceptInstanceDeclarationNode : public Object
			{
			protected:
				Ptr<BasicConceptInstanceDeclaration>	declaration;
			public:
				BasicConceptInstanceDeclarationNode(Ptr<BasicConceptInstanceDeclaration> _declaration);
				Ptr<BasicConceptInstanceDeclaration>	GetInternalValue();

				BasicConceptInstanceDeclarationNode&	Member(const WString& function, const WString& target);
				BasicConceptInstanceDeclarationNode&	Member(const WString& function, const WString& target, const BasicTypeNode::ListNode& types);
			};

/***********************************************************************
Program
***********************************************************************/

			class BasicGenericNode : public Object
			{
			protected:
				Ptr<BasicProgram>						program;
				Ptr<BasicGeneric>						genericDeclaration;

				void									CopyGenericDeclaration(Ptr<BasicDeclaration> declaration);
			public:
				BasicGenericNode(Ptr<BasicProgram> _program);

				BasicGenericNode&						GenericArgument(const WString& name);
				BasicGenericNode&						Where(const WString& genericArgument, const WString& conceptName);
				void									DefineVariable(const WString& name, const BasicTypeNode& type);
				void									DefineVariable(const WString& name, const BasicTypeNode& type, const WString& assemblyName, const WString& symbolName);
				void									DefineVariable(const WString& name, const BasicTypeNode& type, const BasicExpressionNode& initializer);
				void									DefineConstant(const WString& name, const BasicTypeNode& type, const BasicExpressionNode& initializer);
				void									DefineRename(const WString& name, const BasicTypeNode& type);
				BasicFunctionDeclarationNode			DefineFunction(const WString& name);
				void									DefineStructureForward(const WString& name);
				BasicStructureDeclarationNode			DefineStructure(const WString& name);
				void									DefineInstanceForward(const BasicTypeNode& type, const WString& conceptName);
				BasicConceptInstanceDeclarationNode		DefineInstance(const BasicTypeNode& type, const WString& conceptName);
			};

			class BasicProgramNode : public Object
			{
			protected:
				Ptr<BasicProgram>						program;
			public:
				BasicProgramNode();
				Ptr<BasicProgram>						GetInternalValue();

				BasicGenericNode						Generic();
				void									DefineVariable(const WString& name, const BasicTypeNode& type);
				void									DefineVariable(const WString& name, const BasicTypeNode& type, const WString& assemblyName, const WString& symbolName);
				void									DefineVariable(const WString& name, const BasicTypeNode& type, const BasicExpressionNode& initializer);
				void									DefineRename(const WString& name, const BasicTypeNode& type);
				BasicFunctionDeclarationNode			DefineFunction(const WString& name);
				void									DefineStructureForward(const WString& name);
				BasicStructureDeclarationNode			DefineStructure(const WString& name);
				BasicConceptBaseDeclarationNode			DefineConcept(const WString& name, const WString& conceptType);
				void									DefineInstanceForward(const BasicTypeNode& type, const WString& conceptName);
				BasicConceptInstanceDeclarationNode		DefineInstance(const BasicTypeNode& type, const WString& conceptName);
			};
		}
	}
}

#endif
