/***********************************************************************
Vczh Library++ 3.0
Developer: ³Âè÷å«(vczh)
Scripting::BasicLanguage

Classes:
***********************************************************************/

#ifndef VCZH_SCRIPTING_BASICLANGUAGE_BASICLANGUAGECODEGENERATION
#define VCZH_SCRIPTING_BASICLANGUAGE_BASICLANGUAGECODEGENERATION

#include "..\..\Stream\MemoryStream.h"
#include "BasicLanguageAnalyzer.h"
#include "BasicLanguageResource.h"
#include "..\BasicIL\BasicILDefinition.h"
#include "..\BasicIL\BasicILSymbolResource.h"

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
				typedef collections::Dictionary<BasicTypeRecord*, Ptr<BasicTypeInfo>>			_TypeInfoTable;
				typedef collections::Dictionary<BasicTypeRecord*, ResourceHandle<BasicTypeRes>>	_TypeResTable;
			protected:
				BasicAnalyzer*												analyzer;
				_TypeInfoTable												typeInfos;
				collections::Dictionary<BasicFunctionDeclaration*, int>		functions;
				collections::Dictionary<BasicVariableDeclaration*, int>		globalVariableOffsets;
				collections::Dictionary<BasicVariableStatement*, int>		localVariableOffsets;

				int															maxVariableSpace;
				int															usedVariableSpace;
				collections::List<int>										variableSpaceStack;
				collections::List<int>										returnInstructions;

				collections::List<int>										breakInsStack;
				collections::List<int>										continueInsStack;
				collections::List<int>										breakInstructions;
				collections::List<int>										continueInstructions;

				_TypeResTable												typeResources;
			public:
				int															localFunctionCount;

				BasicCodegenInfo(BasicAnalyzer* _analyzer);

				BasicTypeInfo*												GetTypeInfo(BasicTypeRecord* type);
				BasicEnv*													GetEnv();
				BasicTypeManager*											GetTypeManager();
				BasicAlgorithmConfiguration&								GetConfiguration();
				collections::IDictionary<BasicFunctionDeclaration*, int>&	GetFunctions();
				collections::IDictionary<BasicVariableDeclaration*, int>&	GetGlobalVariableOffsets();
				collections::IDictionary<BasicVariableStatement*, int>&		GetLocalVariableOffsets();

				void														BeginFunction();
				void														EndFunction(int returnIns, basicil::BasicIL* il);
				void														AssociateReturn(int instruction);
				void														EnterScope();
				void														LeaveScope();
				int															UseVariable(int size);
				void														EnterLoop();
				void														LeaveLoop(int breakIns, int continueIns, basicil::BasicIL* il);
				void														AssociateBreak(int instruction);
				void														AssociateContinue(int instruction);
				int															GetMaxVariableSpace();

				ResourceHandle<BasicTypeRes>								GetTypeResource(BasicTypeRecord* type);
				bool														SetTypeResource(BasicTypeRecord* type, ResourceHandle<BasicTypeRes> resource);
			};

/***********************************************************************
Extension
***********************************************************************/

			struct BasicCodegenParameter;
			typedef BasicCodegenParameter BCP;

			class BasicCodegenExtension : public Object, private NotCopyable
			{
			public:
				virtual BasicTypeRecord*									PushValue(BasicExtendedExpression* expression, const BCP& argument);
				virtual void												RunSideEffect(BasicExtendedExpression* expression, const BCP& argument);
				virtual void												PushRef(BasicExtendedExpression* expression, const BCP& argument);
				virtual void												PushRefWithoutSideEffect(BasicExtendedExpression* expression, const BCP& argument);
				virtual bool												CanPushRefWithoutSideEffect(BasicExtendedExpression* expression, const BCP& argument);
				virtual void												GenerateCode(BasicExtendedStatement* statement, const BCP& argument);
				virtual void												GenerateCodePass1(BasicExtendedDeclaration* statement, const BCP& argument);
				virtual void												GenerateCodePass2(BasicExtendedDeclaration* statement, const BCP& argument);
				virtual ResourceHandle<BasicDeclarationRes>					GenerateResource(BasicExtendedDeclaration* statement, const BCP& argument);
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
				Ptr<ResourceStream>											resource;
				Ptr<ResourceStream>											exportResource;
				BasicLanguageElement*										currentLanguageElement;
				collections::List<BasicLinking>								linkings;

				BasicCodegenParameter(BasicCodegenInfo* _info, basicil::BasicIL* _il, stream::MemoryStream* _globalData, Ptr<ResourceStream> _resource, Ptr<ResourceStream> _exportResource);
				BasicCodegenParameter(const BasicCodegenParameter& parameter);

				void														Ins(basicil::BasicIns::OpCode opcode)const;
				void														Ins(basicil::BasicIns::OpCode opcode, basicil::BasicIns::Argument argument)const;
				void														Ins(basicil::BasicIns::OpCode opcode, basicil::BasicIns::ValueType type1)const;
				void														Ins(basicil::BasicIns::OpCode opcode, basicil::BasicIns::ValueType type1, basicil::BasicIns::Argument argument)const;
				void														Ins(basicil::BasicIns::OpCode opcode, basicil::BasicIns::ValueType type1, basicil::BasicIns::ValueType type2)const;
			};

/***********************************************************************
Code Generation Helper Functions
***********************************************************************/
			
			extern basicil::BasicIns::ValueType		Convert								(BasicPrimitiveTypeEnum type);
			extern basicil::BasicIns::ValueType		Convert								(BasicTypeRecord* type);
			extern basicil::BasicIns::Argument		Convert								(BasicPrimitiveValueEnum value);
			extern bool								IsExternalFunction					(BasicReferenceExpression* referenceExpression, const BCP& argument);
			extern int								GetFunctionIndex					(BasicReferenceExpression* referenceExpression, const BCP& argument);
			extern void								Code_ScaleAdder						(BasicTypeRecord* addedValueType, const BCP& argument, bool scaleOne);
			extern void								Code_Read							(BasicTypeRecord* type, const BCP& argument);
			extern void								Code_Write							(BasicTypeRecord* type, const BCP& argument);
			extern void								Code_Copy							(BasicTypeRecord* type, const BCP& argument);
			extern void								Code_CopyStack						(BasicTypeRecord* type, const BCP& argument, int offset=0);
			extern void								Code_CopyAddressInStack				(BasicExpression* addressExpression, const BCP& argument, int offset=0);
			extern void								Code_Convert						(BasicTypeRecord* from, BasicTypeRecord* to, const BCP& argument);
			extern BasicTypeRecord*					Code_Binary							(BasicBinaryExpression* node, const BCP& argument, basicil::BasicIns::OpCode opCode);
			extern BasicTypeRecord*					Code_Compare						(BasicBinaryExpression* node, BasicTypeRecord* leftType, BasicTypeRecord* rightType, const BCP& argument, basicil::BasicIns::OpCode opCode);
			extern BasicTypeRecord*					Code_BinaryAssign					(BasicBinaryExpression* node, const BCP& argument, basicil::BasicIns::OpCode opCode);
			extern void								Code_BinaryAssignSideEffect			(BasicBinaryExpression* node, const BCP& argument, basicil::BasicIns::OpCode opCode);
			extern void								Code_BinaryAssignRef				(BasicBinaryExpression* node, const BCP& argument, basicil::BasicIns::OpCode opCode);
			extern BasicTypeRecord*					Code_InvokeFunctionPushParameters	(BasicInvokeExpression* node, const BCP& argument, int& index, int& returnSize, int& parameterSize, bool returnInStack, bool& isExternal);
			extern void								Code_InvokeFunctionCallFunction		(BasicInvokeExpression* node, const BCP& argument, int index, int returnSize, int parameterSize, bool clearReturnInStack, bool isExternal);
			extern BasicTypeRecord*					Code_InvokeFunction					(BasicInvokeExpression* node, const BCP& argument, bool sideEffectOnly);

/***********************************************************************
Code Generation
***********************************************************************/

			extern BasicTypeRecord* BasicLanguage_PushValue(Ptr<BasicExpression> expression, const BCP& argument);
			extern BasicTypeRecord* BasicLanguage_PushValue(BasicExpression* expression, const BCP& argument);
			extern BasicTypeRecord* BasicLanguage_PushValue(Ptr<BasicExpression> expression, const BCP& argument, BasicTypeRecord* expectedType);
			extern BasicTypeRecord* BasicLanguage_PushValue(BasicExpression* expression, const BCP& argument, BasicTypeRecord* expectedType);

			extern void BasicLanguage_StoreToAddress(BasicExpression* expression, BasicExpression* address, const BCP& argument);
			extern void BasicLanguage_StoreToAddress(BasicExpression* expression, BasicVariableStatement* address, const BCP& argument);
			extern void BasicLanguage_StoreToAddress(BasicExpression* expression, BasicVariableDeclaration* address, const BCP& argument);

			EXTERN_ALGORITHM_PROCEDURE(BasicLanguage_RunSideEffect, BasicExpression, BCP)
			EXTERN_ALGORITHM_PROCEDURE(BasicLanguage_PushRef, BasicExpression, BCP)
			EXTERN_ALGORITHM_PROCEDURE(BasicLanguage_PushRefWithoutSideEffect, BasicExpression, BCP)
			EXTERN_ALGORITHM_FUNCTION(BasicLanguage_CanPushRefWithoutSideEffect, BasicExpression, BCP, bool)

			EXTERN_ALGORITHM_PROCEDURE(BasicLanguage_GenerateCode, BasicStatement, BCP)
			EXTERN_ALGORITHM_PROCEDURE(BasicLanguage_GenerateLinkingSymbolTable, BasicDeclaration, BCP)
			EXTERN_ALGORITHM_PROCEDURE(BasicLanguage_GenerateCodePass1, BasicDeclaration, BCP)
			EXTERN_ALGORITHM_PROCEDURE(BasicLanguage_GenerateCodePass2, BasicDeclaration, BCP)

			extern ResourceHandle<BasicTypeRes> GenerateResource(BasicTypeRecord* type, const BCP& argument);
			EXTERN_ALGORITHM_FUNCTION(BasicLanguage_GenerateResource, BasicDeclaration, BCP, ResourceHandle<BasicDeclarationRes>);
			EXTERN_ALGORITHM_FUNCTION(BasicLanguage_GenerateExport, BasicDeclaration, BCP, ResourceHandle<BasicILExportRes>);

			extern void BasicLanguage_GenerateCode(Ptr<BasicProgram> program, const WString& programName, const BCP& argument);

#define BASIC_LANGUAGE_ALGORITHM_INITIALIZER\
			BasicLanguageElement* lastCurrentLanguageElement;\
			ALGORITHM_BEFORE_CALL\
			{\
				lastCurrentLanguageElement=argument.currentLanguageElement;\
				const_cast<BCP&>(argument).currentLanguageElement=node;\
			}\
			ALGORITHM_AFTER_CALL\
			{\
				const_cast<BCP&>(argument).currentLanguageElement=lastCurrentLanguageElement;\
			}

/***********************************************************************
BasicCodeGenerator
***********************************************************************/

			class BasicCodeGenerator
			{
			protected:
				Ptr<basicil::BasicIL>										il;
				Ptr<stream::MemoryStream>									globalData;
				BasicCodegenExtension*										codegenExtension;
				Ptr<BasicCodegenInfo>										codegenInfo;
				Ptr<BasicProgram>											program;
				WString														programName;
			public:
				BasicCodeGenerator(BasicAnalyzer* analyzer, BasicCodegenExtension* extension, const WString& _programName);
				~BasicCodeGenerator();

				Ptr<basicil::BasicIL>										GetIL();
				void														GenerateCode();
			};
		}
	}
}

#endif