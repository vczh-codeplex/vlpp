/***********************************************************************
Vczh Library++ 3.0
Developer: ³Âè÷å«(vczh)
Scripting::BasicLanguage

Classes:
***********************************************************************/

#ifndef VCZH_SCRIPTING_BASICLANGUAGE_BASICLANGUAGECODEGENERATION
#define VCZH_SCRIPTING_BASICLANGUAGE_BASICLANGUAGECODEGENERATION

#include "BasicLanguageAnalyzer.h"
#include "..\BasicIL\BasicILDefinition.h"
#include "..\..\Stream\MemoryStream.h"

namespace vl
{
	namespace scripting
	{
		namespace basiclanguage
		{
			class BasicTypeInfo : public Object
			{
			public:
				int															size;
				int															alignment;
				collections::List<int>										offsets;
			};

			class BasicCodegenInfo : public Object
			{
				typedef collections::Dictionary<BasicTypeRecord*, Ptr<BasicTypeInfo>> _TypeInfoTable;
			protected:
				BasicAnalyzer*												analyzer;
				_TypeInfoTable												typeInfos;
				collections::List<BasicFunctionDeclaration*>				functions;
				collections::Dictionary<BasicVariableDeclaration*, int>		globalVariableOffsets;
				collections::Dictionary<BasicVariableStatement*, int>		localVariableOffsets;

				int															maxVariableSpace;
				int															usedVariableSpace;
				collections::List<int>										variableSpaceStack;

				collections::List<int>										breakInsStack;
				collections::List<int>										continueInsStack;
				collections::List<basicil::BasicIns*>						breakInstructions;
				collections::List<basicil::BasicIns*>						continueInstructions;
			public:
				BasicCodegenInfo(BasicAnalyzer* _analyzer);

				BasicTypeInfo*												GetTypeInfo(BasicTypeRecord* type);
				BasicEnv*													GetEnv();
				BasicTypeManager*											GetTypeManager();
				BasicAlgorithmConfiguration&								GetConfiguration();
				collections::IList<BasicFunctionDeclaration*>&				GetFunctions();
				collections::IDictionary<BasicVariableDeclaration*, int>&	GetGlobalVariableOffsets();
				collections::IDictionary<BasicVariableStatement*, int>&		GetLocalVariableOffsets();

				void														BeginFunction();
				void														EnterScope();
				void														LeaveScope();
				int															UseVariable(int size);
				void														EnterLoop();
				void														LeaveLoop(int breakIns, int continueIns);
				void														AssociateBreak(basicil::BasicIns* instruction);
				void														AssociateContinue(basicil::BasicIns* instruction);
			};

/***********************************************************************
Algorithms
***********************************************************************/

			struct BasicCodegenParameter;
			typedef BasicCodegenParameter BCP;

			class BasicCodegenExtension : public Object, private NotCopyable
			{
			public:
				virtual BasicTypeRecord*									PushValue(BasicExtendedExpression* expression, const BCP& argument);
				virtual void												PushRef(BasicExtendedExpression* expression, const BCP& argument);
				virtual void												GenerateCode(BasicExtendedStatement* statement, const BCP& argument);
				virtual void												GenerateCodePass1(BasicExtendedDeclaration* statement, const BCP& argument);
				virtual void												GenerateCodePass2(BasicExtendedDeclaration* statement, const BCP& argument);
			};

			struct BasicCodegenParameter
			{
			private:
				BasicCodegenExtension										defaultCodegenExtension;
			public:
				BasicCodegenInfo*											info;
				basicil::BasicIL*											il;
				stream::MemoryStream*										globalData;
				BasicCodegenExtension*										codegenExtension;

				BasicCodegenParameter(BasicCodegenInfo* _info, basicil::BasicIL* _il, stream::MemoryStream* _globalData);
				BasicCodegenParameter(const BasicCodegenParameter& parameter);
			};

			extern BasicTypeRecord* BasicLanguage_PushValue(Ptr<BasicExpression> expression, const BCP& argument);
			extern BasicTypeRecord* BasicLanguage_PushValue(BasicExpression* expression, const BCP& argument);
			extern BasicTypeRecord* BasicLanguage_PushValue(Ptr<BasicExpression> expression, const BCP& argument, BasicTypeRecord* expectedType);
			extern BasicTypeRecord* BasicLanguage_PushValue(BasicExpression* expression, const BCP& argument, BasicTypeRecord* expectedType);
			EXTERN_ALGORITHM_FUNCTION(BasicLanguage_PushValueInternal, BasicExpression, BCP, BasicTypeRecord*)
			EXTERN_ALGORITHM_PROCEDURE(BasicLanguage_PushRef, BasicExpression, BCP)

			EXTERN_ALGORITHM_PROCEDURE(BasicLanguage_GenerateCode, BasicStatement, BCP)
			EXTERN_ALGORITHM_PROCEDURE(BasicLanguage_GenerateCodePass1, BasicDeclaration, BCP)
			EXTERN_ALGORITHM_PROCEDURE(BasicLanguage_GenerateCodePass2, BasicDeclaration, BCP)
			extern void BasicLanguage_GenerateCode(Ptr<BasicProgram> program, const BCP& argument);
		}
	}
}

#endif