/***********************************************************************
Vczh Library++ 3.0
Developer: �����(vczh)
Scripting::BasicLanguage

Classes:
***********************************************************************/

#ifndef VCZH_SCRIPTING_COMMON_ALGORITHMDECLARATION
#define VCZH_SCRIPTING_COMMON_ALGORITHMDECLARATION

/***********************************************************************
Algorithm Definition
***********************************************************************/

#define ALGORITHM_TARGET_ROOT(NAME) friend class NAME##Algorithm; typedef NAME##Algorithm __algorithm_interface__; virtual void Accept(__algorithm_interface__* algorithm_object)=0;
#define ALGORITHM_ACCEPT_DECLARATION void Accept(__algorithm_interface__* algorithm_object);
#define ALGORITHM_ACCEPT_IMPLEMENTATION(NODE) void NODE::Accept(__algorithm_interface__* algorithm_object){algorithm_object->Apply(this);}

#define DEFINE_ALGORITHM_INTERFACE_ELEMENT(NODE) virtual void Apply(NODE* node)=0;
#define DEFINE_ALGORITHM_INTERFACE(NAME, ALGORITHM_TARGETS)\
	class NAME##Algorithm : public Object, private NotCopyable\
	{\
	public:\
		ALGORITHM_TARGETS(DEFINE_ALGORITHM_INTERFACE_ELEMENT)\
	};

#define DEFINE_ALGORITHM_ACCEPT_IMPLEMENTATION(ALGORITHM_TARGETS) ALGORITHM_TARGETS(ALGORITHM_ACCEPT_IMPLEMENTATION)

/***********************************************************************
Algorithm Function
***********************************************************************/

#define BEGIN_ALGORITHM_FUNCTION(NAME, NODE, INPUT_TYPE, OUTPUT_TYPE)\
	class NAME : public NODE##Algorithm\
	{\
	private:\
		typedef INPUT_TYPE					__algorithm_input_type__;\
		typedef OUTPUT_TYPE					__algorithm_output_type__;\
		const __algorithm_input_type__*		input_value;\
		__algorithm_output_type__			output_value;\
	public:\
		__algorithm_output_type__ Execute(NODE* node, const __algorithm_input_type__& _input_value)\
		{\
			input_value=&_input_value;\
			node->Accept(this);\
			return output_value;\
		}\
		__algorithm_output_type__ Execute(Ptr<NODE> node, const __algorithm_input_type__& _input_value)\
		{\
			input_value=&_input_value;\
			node->Accept(this);\
			return output_value;\
		}

#define ALGORITHM_FUNCTION_MATCH(NODE)\
		void Apply(NODE* node)\
		{\
			output_value=Match(node, *input_value);\
		}\
		__algorithm_output_type__ Match(NODE* node, const __algorithm_input_type__& argument)

#define END_ALGORITHM_FUNCTION\
		};

/***********************************************************************
Algorithm Procedure
***********************************************************************/

#define BEGIN_ALGORITHM_PROCEDURE(NAME, NODE, INPUT_TYPE)\
	class NAME : public NODE##Algorithm\
	{\
	private:\
		typedef INPUT_TYPE					__algorithm_input_type__;\
		const __algorithm_input_type__*		input_value;\
	public:\
		void Execute(NODE* node, const __algorithm_input_type__& _input_value)\
		{\
			input_value=&_input_value;\
			node->Accept(this);\
		}\
		void Execute(Ptr<NODE> node, const __algorithm_input_type__& _input_value)\
		{\
			input_value=&_input_value;\
			node->Accept(this);\
		}

#define ALGORITHM_PROCEDURE_MATCH(NODE)\
		void Apply(NODE* node)\
		{\
			Match(node, *input_value);\
		}\
		void Match(NODE* node, const __algorithm_input_type__& argument)

#define END_ALGORITHM_PROCEDURE\
		};

#endif