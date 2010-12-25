/***********************************************************************
Vczh Library++ 3.0
Developer: �����(vczh)
Scripting::BasicIL

Classes:
  BasicILExportRes				������ɼ���������
  BasicILLinkingRes				�������ӷ�������
  BasicILEntryRes				�����
***********************************************************************/

#ifndef VCZH_SCRIPTING_BASICIL_BASICILSYMBOLRESOURCE
#define VCZH_SCRIPTING_BASICIL_BASICILSYMBOLRESOURCE

#include "BasicILResourceStream.h"

namespace vl
{
	namespace scripting
	{

/***********************************************************************
�ɼ�����
***********************************************************************/

		//�ɼ�����
		struct BasicILExportRes
		{
			enum TypeEnum
			{
				Variable,
				Function,
			};

			TypeEnum												type;					//����
			vint													address;				//��ַ
			ResourceString											name;					//����
			vint													instructionCount;		//ָ�����������Ժ�����Ч
		};

/***********************************************************************
�����ӷ���
***********************************************************************/

		//����Ŀ��
		struct BasicILLinkingRes
		{
			ResourceString											symbolName;				//����
			ResourceString											assemblyName;			//Asembly��
		};

/***********************************************************************
ģ�����
***********************************************************************/

		//�ַ�����Դ
		struct BasicILGenericNameRes
		{
			bool													isConstant;				//�Ƿ���
			ResourceString											constantString;			//�����ַ���
			vint													stringArgumentIndex;	//�ַ�����������Դ���ػ���������Ĳ�����
		};

		//���Զ������
		struct BasicILGenericFactorItemRes
		{
			vint													factor;					//����
		};

		//���Զ���ֵ = sum(fi*ti) + constant��
		struct BasicILGenericLinearRes
		{
			ResourceArrayHandle<BasicILGenericFactorItemRes>		factors;				//������
			vint													constant;				//������
		};

		//ģ�����
		struct BasicILGenericArgumentRes
		{
			ResourceHandle<BasicILGenericNameRes>					nameArgument;			//����ID
			ResourceHandle<BasicILGenericLinearRes>					sizeArgument;			//�ߴ����
			ResourceArrayHandle<BasicILGenericArgumentRes>			subArgument;			//�Ӳ���
		};

		//--------------------------------------------------------------------------------------------------------

		//ģ�庯�����
		struct BasicILGenericFunctionEntryRes
		{
			ResourceString											name;					//ģ�庯������
			vint													genericArgumentCount;	//ģ���������
			vint													startInstruction;		//��ʼָ��λ��
			vint													instructionCount;		//ָ������
			ResourceString											uniqueEntryID;			//������ڵ�Ψһ��ʶ���������������һ��Ψһ���ַ�����
		};

		//ģ�庯�����
		struct BasicILGenericVariableEntryRes
		{
			ResourceString											name;					//ģ���������
			vint													genericArgumentCount;	//ģ���������
			ResourceHandle<BasicILGenericLinearRes>					size;					//ģ������ߴ�
			ResourceString											uniqueEntryID;			//������ڵ�Ψһ��ʶ���������������һ��Ψһ���ַ�����
		};

		//ģ�庯���ػ�
		struct BasicILGenericTargetRes
		{
			ResourceString											symbolName;				//����
			ResourceString											assemblyName;			//Assembly��
			ResourceArrayHandle<BasicILGenericArgumentRes>			arguments;				//ģ�����
		};

		//Instance�����ػ�
		struct BasicILGenericInstanceTargetRes
		{
			ResourceString											symbolName;				//Concept����
			ResourceString											assemblyName;			//Assembly��
			ResourceString											functionName;			//������
			ResourceHandle<BasicILGenericArgumentRes>				type;					//ģ�����
		};

		//--------------------------------------------------------------------------------------------------------

		struct BasicILGenericConceptFunctionRes
		{
			ResourceString											name;
		};

		struct BasicILGenericConceptRes
		{
			ResourceString											name;
			ResourceArrayHandle<BasicILGenericConceptFunctionRes>	functions;
		};

		//--------------------------------------------------------------------------------------------------------

		struct BasicILGenericInstanceFunctionRes
		{
			ResourceString											functionName;
			ResourceHandle<BasicILGenericTargetRes>					functionTarget;
		};

		struct BasicILGenericInstanceRes
		{
			ResourceString											conceptSymbolName;
			ResourceString											conceptAssemblyName;
			ResourceString											typeUniqueName;
			vint													genericArgumentCount;
			ResourceArrayHandle<BasicILGenericInstanceFunctionRes>	functions;
		};

		//--------------------------------------------------------------------------------------------------------

		//ģ����ű�
		struct BasicILGenericRes
		{
			ResourceArrayHandle<BasicILGenericFunctionEntryRes>		functionEntries;		//�������
			ResourceArrayHandle<BasicILGenericVariableEntryRes>		variableEntries;		//�����������
			ResourceArrayHandle<BasicILGenericTargetRes>			targets;				//�ػ���
			ResourceArrayHandle<BasicILGenericLinearRes>			linears;				//���Բ�����
			ResourceArrayHandle<BasicILGenericConceptRes>			concepts;				//concept��
			ResourceArrayHandle<BasicILGenericInstanceRes>			instances;				//instance��
			ResourceArrayHandle<BasicILGenericInstanceTargetRes>	instanceTargets;		//instance�����ػ���
		};

/***********************************************************************
���
***********************************************************************/

		 //�ܷ������
		struct BasicILEntryRes
		{
			ResourceString											assemblyName;			//Assembly��
			ResourceArrayHandle<BasicILExportRes>					exports;				//�ɼ�����
			ResourceArrayHandle<BasicILLinkingRes>					linkings;				//�����ӷ���
			ResourceArrayHandle<BasicILLinkingRes>					foreigns;				//��纯������
			ResourceHandle<BasicILGenericRes>						genericSymbols;			//ģ�����
		};
	}
}

#endif