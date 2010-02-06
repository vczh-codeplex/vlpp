/***********************************************************************
Vczh Library++ 3.0
Developer: ≥¬Ë˜Â´(vczh)
Scripting::BasicLanguage

Classes:
	BasicAlgorithmConfiguration					£∫”Ô“Â∑÷Œˆ≈‰÷√
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
				bool		treatZeroAsNull;
				bool		enablePointerArithmetic;
				bool		enableImplicitBooleanToIntegerConversion;
				bool		enableImplicitIntegerToBooleanConversion;
				bool		enableImplicitPointerToBooleanConversion;
				bool		enableImplicitFloatToIntegerConversion;
				bool		enableImplicitIntegerToFloatConversion;
				bool		enableImplicitHighToLowPrecisionConversion;
				bool		enableImplicitSignedToUnsignedConversion;
				bool		enableSubscribeOnPointer;

				BasicAlgorithmConfiguration();

				bool		DecodeInteger(BasicPrimitiveTypeEnum type, bool& sign, int& bytes);
				bool		EncodeInteger(BasicPrimitiveTypeEnum& type, bool sign, int bytes);
				bool		DecodeFloat(BasicPrimitiveTypeEnum type, int& bytes);
				bool		EncodeFloat(BasicPrimitiveTypeEnum& type, int bytes);
				bool		CanImplicitConvertTo(BasicPrimitiveTypeEnum from, BasicPrimitiveTypeEnum to);
				bool		CanExplicitConvertTo(BasicPrimitiveTypeEnum from, BasicPrimitiveTypeEnum to);
				bool		BooleanUnaryOperatorTypeConversion(BasicPrimitiveTypeEnum operand, BasicPrimitiveTypeEnum& result);
				bool		IntegerUnaryOperatorTypeConversion(BasicPrimitiveTypeEnum operand, BasicPrimitiveTypeEnum& result);
				bool		NumberUnaryOperatorTypeConversion(BasicPrimitiveTypeEnum operand, BasicPrimitiveTypeEnum& result);
				bool		BooleanBinaryOperatorTypeConversion(BasicPrimitiveTypeEnum left, BasicPrimitiveTypeEnum right, BasicPrimitiveTypeEnum& result);
				bool		IntegerBinaryOperatorTypeConversion(bool leftSign, int leftBytes, bool rightSign, int rightBytes, BasicPrimitiveTypeEnum& result);
				bool		IntegerBinaryOperatorTypeConversion(BasicPrimitiveTypeEnum left, BasicPrimitiveTypeEnum right, BasicPrimitiveTypeEnum& result);
				bool		NumberBinaryOperatorTypeConversion(BasicPrimitiveTypeEnum left, BasicPrimitiveTypeEnum right, BasicPrimitiveTypeEnum& result);
			};
		}
	}
}

#endif