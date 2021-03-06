/***********************************************************************
Vczh Library++ 3.0
Developer: �����(vczh)
Scripting::BasicLanguage

Classes:
	BasicAlgorithmConfiguration					�������������
***********************************************************************/

#ifndef VCZH_SCRIPTING_BASICLANGUAGE_BASICLANGUAGECONFIGURATION
#define VCZH_SCRIPTING_BASICLANGUAGE_BASICLANGUAGECONFIGURATION

#include "BasicLanguageExpression.h"

namespace vl
{
	namespace scripting
	{
		namespace basiclanguage
		{
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
				bool		enablePointerArithmetic;
				bool		enableImplicitBooleanToIntegerConversion;
				bool		enableImplicitIntegerToBooleanConversion;
				bool		enableImplicitPointerToBooleanConversion;
				bool		enableImplicitFloatToIntegerConversion;
				bool		enableImplicitIntegerToFloatConversion;
				bool		enableImplicitHighToLowPrecisionConversion;
				bool		enableImplicitSignedToUnsignedConversion;
				bool		enableImplicitUnsignedToSignedConversion;
				bool		enableSubscribeOnPointer;

				BasicAlgorithmConfiguration();

				bool		DecodeInteger(BasicPrimitiveTypeEnum type, bool& sign, vint& bytes)const;
				bool		EncodeInteger(BasicPrimitiveTypeEnum& type, bool sign, vint bytes)const;
				bool		DecodeFloat(BasicPrimitiveTypeEnum type, vint& bytes)const;
				bool		EncodeFloat(BasicPrimitiveTypeEnum& type, vint bytes)const;

				bool		CanImplicitConvertTo(BasicPrimitiveTypeEnum from, BasicPrimitiveTypeEnum to)const;
				bool		CanExplicitConvertTo(BasicPrimitiveTypeEnum from, BasicPrimitiveTypeEnum to)const;

				bool		BooleanUnaryOperatorTypeConversion(BasicPrimitiveTypeEnum operand, BasicPrimitiveTypeEnum& result)const;
				bool		IntegerUnaryOperatorTypeConversion(BasicPrimitiveTypeEnum operand, BasicPrimitiveTypeEnum& result)const;
				bool		NumberUnaryOperatorTypeConversion(BasicPrimitiveTypeEnum operand, BasicPrimitiveTypeEnum& result)const;

				bool		BooleanBinaryOperatorTypeConversion(BasicPrimitiveTypeEnum left, BasicPrimitiveTypeEnum right, BasicPrimitiveTypeEnum& result)const;
				bool		IntegerBinaryOperatorTypeConversion(bool leftSign, vint leftBytes, bool rightSign, vint rightBytes, BasicPrimitiveTypeEnum& result)const;
				bool		IntegerBinaryOperatorTypeConversion(BasicPrimitiveTypeEnum left, BasicPrimitiveTypeEnum right, BasicPrimitiveTypeEnum& result)const;
				bool		NumberBinaryOperatorTypeConversion(BasicPrimitiveTypeEnum left, BasicPrimitiveTypeEnum right, BasicPrimitiveTypeEnum& result)const;
			};
		}
	}
}

#endif