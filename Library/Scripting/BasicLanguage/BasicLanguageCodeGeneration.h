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
#include "..\..\Entity\Linear.h"

namespace vl
{
	namespace scripting
	{
		namespace basiclanguage
		{
			typedef Linear<BasicTypeRecord*, vint>									BasicOffset;

			class BasicTypeInfo : public Object
			{
			public:
				BasicOffset															size;
				collections::List<BasicOffset>										offsets;
			};

			class BasicCodegenInfo : public Object
			{
				typedef collections::Dictionary<BasicTypeRecord*, Ptr<BasicTypeInfo>>			_TypeInfoTable;
				typedef collections::Dictionary<BasicTypeRecord*, ResourceHandle<BasicTypeRes>>	_TypeResTable;
			public:
				struct FunctionEntry
				{
					BasicFunctionDeclaration*										declaration;
					vint															startInstruction;
					vint															instructionCount;
				};

				struct GenericTarget
				{
					BasicFunctionDeclaration*										ownerFunctionDeclaration;
					BasicDeclaration*												targetDeclaration;
					collections::List<BasicTypeRecord*>								genericParameters;
				};

				struct GenericInstanceTarget
				{
					BasicFunctionDeclaration*										ownerFunctionDeclaration;
					BasicConceptBaseDeclaration*									targetDeclaration;
					WString															functionName;
					BasicTypeRecord*												type;
				};

				typedef Linear<vint, vint>											FunctionLinear;
			protected:
				BasicAnalyzer*														analyzer;
				_TypeInfoTable														typeInfos;
				collections::Dictionary<BasicFunctionDeclaration*, vint>			functions;
				collections::Dictionary<BasicVariableDeclaration*, vint>			globalVariableOffsets;
				collections::Dictionary<BasicVariableStatement*, BasicOffset>		localVariableOffsets;
				BasicOffset															usedVariableSpace;

				collections::List<vint>												returnInstructions;
				collections::List<vint>												breakInsStack;
				collections::List<vint>												continueInsStack;
				collections::List<vint>												breakInstructions;
				collections::List<vint>												continueInstructions;

				_TypeResTable														typeResources;
				BasicFunctionDeclaration*											currentFunctionDeclaration;
				collections::List<BasicTypeRecord*>									currentFunctionGenericParameters;
				collections::List<BasicScope*>										semanticScopes;
			public:
				vint																localFunctionCount;
				collections::List<BasicLinking>										linkings;
				collections::List<BasicLinking>										foreigns;
				collections::List<Ptr<FunctionEntry>>								instanciatedGenericFunctionEntries;
				collections::List<Ptr<GenericTarget>>								instanciatedGenericTargets;
				collections::List<Ptr<GenericInstanceTarget>>						instanciatedGenericInstanceTargets;
				collections::List<FunctionLinear>									instanciatedGenericLinears;

				BasicCodegenInfo(BasicAnalyzer* _analyzer);

				BasicTypeInfo*														GetTypeInfo(BasicTypeRecord* type);
				BasicEnv*															GetEnv();
				BasicTypeManager*													GetTypeManager();
				BasicAlgorithmConfiguration&										GetConfiguration();
				collections::IDictionary<BasicFunctionDeclaration*, vint>&			GetFunctions();
				collections::IDictionary<BasicVariableDeclaration*, vint>&			GetGlobalVariableOffsets();
				collections::IDictionary<BasicVariableStatement*, BasicOffset>&		GetLocalVariableOffsets();

				void																BeginFunction(BasicFunctionDeclaration* declaration, basicil::BasicIL* il, vint existInstructionCount);
				void																EndFunction(vint returnIns, basicil::BasicIL* il, vint remainInstructionCount);
				void																EnterSemanticScope(BasicScope* scope);
				void																LeaveSemanticScope();
				BasicScope*															GetSemanticScope();

				void																AssociateReturn(vint instruction);
				void																EnterScope();
				void																LeaveScope();
				BasicOffset															UseVariable(BasicOffset size);
				void																EnterLoop();
				void																LeaveLoop(vint breakIns, vint continueIns, basicil::BasicIL* il);
				void																AssociateBreak(vint instruction);
				void																AssociateContinue(vint instruction);
				BasicOffset															GetMaxVariableSpace();

				vint																RegisterGenericTarget(Ptr<GenericTarget> target);
				vint																RegisterGenericInstanceTarget(Ptr<GenericInstanceTarget> target);
				vint																RegisterLinear(const BasicOffset& offset);

				ResourceHandle<BasicTypeRes>										GetTypeResource(BasicTypeRecord* type);
				bool																SetTypeResource(BasicTypeRecord* type, ResourceHandle<BasicTypeRes> resource);
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

				BasicCodegenParameter(BasicCodegenInfo* _info, basicil::BasicIL* _il, stream::MemoryStream* _globalData, Ptr<ResourceStream> _resource, Ptr<ResourceStream> _exportResource);
				BasicCodegenParameter(const BasicCodegenParameter& parameter);

				void														Ins(basicil::BasicIns::OpCode opcode)const;
				void														Ins(basicil::BasicIns::OpCode opcode, basicil::BasicIns::Argument argument)const;
				void														Ins(basicil::BasicIns::OpCode opcode, basicil::BasicIns::ValueType type1)const;
				void														Ins(basicil::BasicIns::OpCode opcode, basicil::BasicIns::ValueType type1, basicil::BasicIns::Argument argument)const;
				void														Ins(basicil::BasicIns::OpCode opcode, basicil::BasicIns::ValueType type1, basicil::BasicIns::ValueType type2)const;
				
				void														Ins(basicil::BasicIns::OpCode opcode, const BasicOffset& argument)const;
				void														Ins(basicil::BasicIns::OpCode opcode, basicil::BasicIns::ValueType type1, const BasicOffset& argument)const;
			};

/***********************************************************************
Code Generation Helper Functions
***********************************************************************/
			
			extern basicil::BasicIns::ValueType		Convert								(BasicPrimitiveTypeEnum type);
			extern basicil::BasicIns::ValueType		Convert								(BasicTypeRecord* type);
			extern basicil::BasicIns::Argument		Convert								(BasicPrimitiveValueEnum value);
			extern bool								IsExternalFunction					(BasicReferenceExpression* referenceExpression, const BCP& argument);
			extern bool								IsForeignFunction					(BasicReferenceExpression* referenceExpression, const BCP& argument);
			extern vint								GetFunctionIndex					(BasicReferenceExpression* referenceExpression, const BCP& argument);
			extern vint								GetGenericVariableTargetIndex		(BasicInstanciatedExpression* node, const BCP& argument, BasicTypeRecord*& resultType);
			extern vint								GetGenericFunctionTargetIndex		(BasicInstanciatedExpression* node, const BCP& argument, BasicTypeRecord*& resultType);
			extern vint								GetGenericInstanceTargetIndex		(BasicInstanceFunctionExpression* node, const BCP& argument);
			extern void								Code_ScaleAdder						(BasicTypeRecord* addedValueType, const BCP& argument, bool scaleOne);
			extern void								Code_Read							(BasicTypeRecord* type, const BCP& argument);
			extern void								Code_Write							(BasicTypeRecord* type, const BCP& argument);
			extern void								Code_Copy							(BasicTypeRecord* type, const BCP& argument);
			extern void								Code_CopyStack						(BasicTypeRecord* type, const BCP& argument, const BasicOffset& offset=0);
			extern void								Code_CopyAddressInStack				(BasicExpression* addressExpression, const BCP& argument, const BasicOffset& offset=0);
			extern void								Code_Convert						(BasicTypeRecord* from, BasicTypeRecord* to, const BCP& argument);
			extern BasicTypeRecord*					Code_Binary							(BasicBinaryExpression* node, const BCP& argument, basicil::BasicIns::OpCode opCode);
			extern BasicTypeRecord*					Code_Compare						(BasicBinaryExpression* node, BasicTypeRecord* leftType, BasicTypeRecord* rightType, const BCP& argument, basicil::BasicIns::OpCode opCode);
			extern BasicTypeRecord*					Code_BinaryAssign					(BasicBinaryExpression* node, const BCP& argument, basicil::BasicIns::OpCode opCode);
			extern void								Code_BinaryAssignSideEffect			(BasicBinaryExpression* node, const BCP& argument, basicil::BasicIns::OpCode opCode);
			extern void								Code_BinaryAssignRef				(BasicBinaryExpression* node, const BCP& argument, basicil::BasicIns::OpCode opCode);
			extern BasicTypeRecord*					Code_InvokeFunctionPushParameters	(BasicInvokeExpression* node, const BCP& argument, BasicOffset& returnSize, BasicOffset& parameterSize, bool returnInStack);
			extern void								Code_InvokeFunctionCallFunction		(BasicInvokeExpression* node, const BCP& argument, const BasicOffset& returnSize, const BasicOffset& parameterSize, bool clearReturnInStack);
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
			extern void BasicLanguage_Invoke(BasicExpression* expression, const BCP& argument);

			extern void BasicLanguage_GenerateCode(BasicStatement* node, const BCP& argument);
			extern void BasicLanguage_GenerateCode(Ptr<BasicStatement> node, const BCP& argument);
			EXTERN_ALGORITHM_PROCEDURE(BasicLanguage_GenerateLinkingSymbolTable, BasicDeclaration, BCP)
			EXTERN_ALGORITHM_PROCEDURE(BasicLanguage_GenerateCodePass1, BasicDeclaration, BCP)
			EXTERN_ALGORITHM_PROCEDURE(BasicLanguage_GenerateCodePass2, BasicDeclaration, BCP)

			extern ResourceHandle<BasicTypeRes> GenerateResource(BasicTypeRecord* type, const BCP& argument);
			EXTERN_ALGORITHM_FUNCTION(BasicLanguage_GenerateResource, BasicDeclaration, BCP, ResourceHandle<BasicDeclarationRes>);
			EXTERN_ALGORITHM_FUNCTION(BasicLanguage_GenerateExport, BasicDeclaration, BCP, ResourceHandle<BasicILExportRes>);
			extern ResourceHandle<BasicILGenericRes> BasicLanguage_GenerateGenericResource(const Ptr<BasicProgram> program, const WString& programName, const BCP& argument);
			extern ResourceArrayHandle<BasicDeclarationRes> BasicLanguage_GenerateDeclarationResource(const Ptr<BasicProgram> program, const BCP& argument);
			extern ResourceArrayHandle<BasicILExportRes> BasicLanguage_GenerateExportResource(const Ptr<BasicProgram> program, const BCP& argument);
			extern ResourceArrayHandle<BasicILLinkingRes> BasicLanguage_GenerateLinkingResource(const Ptr<BasicProgram> program, const BCP& argument);
			extern ResourceArrayHandle<BasicILLinkingRes> BasicLanguage_GenerateForeignResource(const Ptr<BasicProgram> program, const BCP& argument);

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

/***********************************************************************
Header File Generator
***********************************************************************/

			extern Ptr<BasicProgram> BasicLanguage_GenerateHeaderFile(
				WString assemblyName,
				Ptr<ResourceStream> resource,
				bool publicOnly,
				bool currentAssemblyOnly,
				const WString& prefix,
				collections::ICollection<WString>& referencedAssemblies);
		}
	}
}

#endif