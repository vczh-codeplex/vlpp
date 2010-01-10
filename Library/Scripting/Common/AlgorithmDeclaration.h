/***********************************************************************
Vczh Library++ 3.0
Developer: ³Âè÷å«(vczh)
Scripting::BasicLanguage

Classes:
***********************************************************************/

#ifndef VCZH_SCRIPTING_COMMON_ALGORITHMDECLARATION
#define VCZH_SCRIPTING_COMMON_ALGORITHMDECLARATION

#define ALGORITHM_TARGET_ROOT(NAME) typedef NAME __algorithm_interface__; virtual void Accept(__algorithm_interface__* algorithm_object)=0;
#define ALGORITHM_ACCEPT_DECLARATION void Accept(__algorithm_interface__* algorithm_object);
#define ALGORITHM_ACCEPT_IMPLEMENTATION(NODE) void NODE::Accept(__algorithm_interface__* algorithm_object){algorithm_object->Apply(this);}

#define DEFINE_ALGORITHM_INTERFACE_ELEMENT(NODE) virtual void Apply(NODE* node);
#define DEFINE_ALGORITHM_INTERFACE(NAME, ALGORITHM_TARGETS)\
	class NAME : public Object, private NotCopyable\
	{\
	public:\
		ALGORITHM_TARGETS(DEFINE_ALGORITHM_INTERFACE_ELEMENT)\
	};

#define DEFINE_ALGORITHM_ACCEPT_IMPLEMENTATION(ALGORITHM_TARGETS) ALGORITHM_TARGETS(ALGORITHM_ACCEPT_IMPLEMENTATION)

#endif