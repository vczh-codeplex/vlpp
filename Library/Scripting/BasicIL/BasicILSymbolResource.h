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
			vint													address;				//��ַ
			ResourceString											name;					//����
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

		//ģ�庯�����
		struct BasicILGenericFunctionEntryRes
		{
			ResourceString											name;					//ģ�庯������
			vint													genericArgumentCount;	//ģ���������
			vint													startInstruction;		//��ʼָ��λ��
			vint													instructionCount;		//ָ������
			ResourceArrayHandle<BasicILGenericNameRes>				uniqueNameTemplate;		//Ψһ��ʶ�ַ���ģ�壨�������һ�����ַ�����Ψһ��ʾ�Լ���
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
			ResourceArrayHandle<BasicILGenericNameRes>				nameArgument;			//�ַ�������
			ResourceHandle<BasicILGenericLinearRes>					sizeArgument;			//�ߴ����
		};

		//ģ�庯���ػ�
		struct BasicILGenericFunctionTargetRes
		{
			ResourceString											symbolName;				//����
			ResourceString											assemblyName;			//Assembly��
			ResourceArrayHandle<BasicILGenericArgumentRes>			arguments;				//ģ�����
		};

		//ģ����ű�
		struct BasicILGenericRes
		{
			ResourceArrayHandle<BasicILGenericFunctionEntryRes>		functionEntries;		//�������
			ResourceArrayHandle<BasicILGenericFunctionTargetRes>	functionTargets;		//�ػ���
			ResourceArrayHandle<BasicILGenericLinearRes>			linears;				//���Բ�����
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
			ResourceHandle<BasicILGenericRes>						genericSymbols;			//ģ�����
		};
	}
}

#endif