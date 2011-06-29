/***********************************************************************
Vczh Library++ 3.0
Developer: 陈梓瀚(vczh)
Scripting::BasicLanguage

Classes:
***********************************************************************/

#ifndef VCZH_SCRIPTING_COMMON_ALGORITHMDECLARATION
#define VCZH_SCRIPTING_COMMON_ALGORITHMDECLARATION

/***********************************************************************
Algorithm Definition
***********************************************************************/

#define ALGORITHM_TARGET_ROOT(NAME) friend class NAME##Algorithm; virtual void Accept(NAME##Algorithm* algorithm_object)=0;
#define ALGORITHM_ACCEPT_DECLARATION(NAME) void Accept(NAME##Algorithm* algorithm_object);
#define ALGORITHM_ACCEPT_IMPLEMENTATION(NAME, NODE) void NODE::Accept(NAME##Algorithm* algorithm_object){algorithm_object->Apply(this);}

#define ALGORITHM_ACCEPT_DECLARATION_UNNSED(NAME) void Accept(NAME##Algorithm* algorithm_object){CHECK_ERROR(false, L"不可使用。");}

#define DEFINE_ALGORITHM_INTERFACE_ELEMENT(NAME, NODE) virtual void Apply(NODE* node)=0;
#define DEFINE_ALGORITHM_INTERFACE(NAME, ALGORITHM_TARGETS)\
	class NAME##Algorithm : public Object, private NotCopyable\
	{\
	protected:\
		template<typename T>void BeforeCall(void*, const T&){}\
		template<typename T>void AfterCall(void*, const T&){}\
	public:\
		ALGORITHM_TARGETS(NAME, DEFINE_ALGORITHM_INTERFACE_ELEMENT)\
	};

#define DEFINE_ALGORITHM_ACCEPT_IMPLEMENTATION(NAME, ALGORITHM_TARGETS) ALGORITHM_TARGETS(NAME, ALGORITHM_ACCEPT_IMPLEMENTATION)

/***********************************************************************
Algorithm Extern
***********************************************************************/

#define EXTERN_ALGORITHM_FUNCTION(NAME, NODE, INPUT_TYPE, OUTPUT_TYPE)\
	extern OUTPUT_TYPE NAME(NODE* node, INPUT_TYPE _input_value);\
	extern OUTPUT_TYPE NAME(Ptr<NODE> node, INPUT_TYPE _input_value);

#define EXTERN_ALGORITHM_PROCEDURE(NAME, NODE, INPUT_TYPE)\
	extern void NAME(NODE* node, INPUT_TYPE _input_value);\
	extern void NAME(Ptr<NODE> node, INPUT_TYPE _input_value);

/***********************************************************************
Algorithm Begin/End Call
***********************************************************************/

#define ALGORITHM_BEFORE_CALL\
		void BeforeCall(__algorithm_target_type__* node, const __algorithm_input_type__& argument)

#define ALGORITHM_AFTER_CALL\
		void AfterCall(__algorithm_target_type__* node, const __algorithm_input_type__& argument)

/***********************************************************************
Algorithm Function
***********************************************************************/

#define BEGIN_ALGORITHM_FUNCTION(NAME, NODE, INPUT_TYPE, OUTPUT_TYPE)\
	class NAME##Algorithm : public NODE##Algorithm\
	{\
	private:\
		typedef INPUT_TYPE					__algorithm_input_type__;\
		typedef OUTPUT_TYPE					__algorithm_output_type__;\
		typedef NODE						__algorithm_target_type__;\
		friend __algorithm_output_type__	NAME(__algorithm_target_type__*, __algorithm_input_type__);\
		friend __algorithm_output_type__	NAME(Ptr<__algorithm_target_type__>, __algorithm_input_type__);\
		const __algorithm_input_type__*		input_value;\
		__algorithm_output_type__			output_value;\
	public:\
		__algorithm_output_type__ Execute(__algorithm_target_type__* node, const __algorithm_input_type__& _input_value)\
		{\
			input_value=&_input_value;\
			node->Accept(this);\
			return output_value;\
		}\
		__algorithm_output_type__ Execute(Ptr<__algorithm_target_type__> node, const __algorithm_input_type__& _input_value)\
		{\
			input_value=&_input_value;\
			node->Accept(this);\
			return output_value;\
		}

#define ALGORITHM_FUNCTION_MATCH(NODE)\
		void Apply(NODE* node)\
		{\
			BeforeCall(node, *input_value);\
			output_value=Match(node, *input_value);\
			AfterCall(node, *input_value);\
		}\
		__algorithm_output_type__ Match(NODE* node, const __algorithm_input_type__& argument)

#define END_ALGORITHM_FUNCTION(NAME)\
		};\
		NAME##Algorithm::__algorithm_output_type__ NAME(\
			NAME##Algorithm::__algorithm_target_type__* node,\
			NAME##Algorithm::__algorithm_input_type__ _input_value)\
		{\
			return NAME##Algorithm().Execute(node, _input_value);\
		}\
		NAME##Algorithm::__algorithm_output_type__ NAME(\
			Ptr<NAME##Algorithm::__algorithm_target_type__> node,\
			NAME##Algorithm::__algorithm_input_type__ _input_value)\
		{\
			return NAME##Algorithm().Execute(node, _input_value);\
		}

/***********************************************************************
Algorithm Procedure
***********************************************************************/

#define BEGIN_ALGORITHM_PROCEDURE(NAME, NODE, INPUT_TYPE)\
	class NAME##Algorithm : public NODE##Algorithm\
	{\
	private:\
		typedef INPUT_TYPE					__algorithm_input_type__;\
		typedef NODE						__algorithm_target_type__;\
		friend void							NAME(__algorithm_target_type__*, __algorithm_input_type__);\
		friend void							NAME(Ptr<__algorithm_target_type__>, __algorithm_input_type__);\
		const __algorithm_input_type__*		input_value;\
	public:\
		void Execute(__algorithm_target_type__* node, const __algorithm_input_type__& _input_value)\
		{\
			input_value=&_input_value;\
			node->Accept(this);\
		}\
		void Execute(Ptr<__algorithm_target_type__> node, const __algorithm_input_type__& _input_value)\
		{\
			input_value=&_input_value;\
			node->Accept(this);\
		}

#define ALGORITHM_PROCEDURE_MATCH(NODE)\
		void Apply(NODE* node)\
		{\
			BeforeCall(node, *input_value);\
			Match(node, *input_value);\
			AfterCall(node, *input_value);\
		}\
		void Match(NODE* node, const __algorithm_input_type__& argument)

#define END_ALGORITHM_PROCEDURE(NAME)\
		};\
		void NAME(\
			NAME##Algorithm::__algorithm_target_type__* node,\
			NAME##Algorithm::__algorithm_input_type__ _input_value)\
		{\
			NAME##Algorithm().Execute(node, _input_value);\
		}\
		void NAME(\
			Ptr<NAME##Algorithm::__algorithm_target_type__> node,\
			NAME##Algorithm::__algorithm_input_type__ _input_value)\
		{\
			NAME##Algorithm().Execute(node, _input_value);\
		}

#endif