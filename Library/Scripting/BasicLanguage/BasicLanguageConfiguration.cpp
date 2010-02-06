#include "BasicLanguageConfiguration.h"

namespace vl
{
	namespace scripting
	{
		namespace basiclanguage
		{

/***********************************************************************
BasicAlgorithmParameter
***********************************************************************/

			BasicAlgorithmConfiguration::BasicAlgorithmConfiguration()
			{
				integerOperationConversion=FreeConversion;
				treatCharacterAsInteger=false;
				treatCharAsSignedInteger=true;
				treatWCharAsSignedInteger=false;
				treatZeroAsNull=false;
				enablePointerArithmetic=true;
				enableImplicitBooleanToIntegerConversion=false;
				enableImplicitIntegerToBooleanConversion=false;
				enableImplicitPointerToBooleanConversion=false;
				enableImplicitFloatToIntegerConversion=false;
				enableImplicitIntegerToFloatConversion=false;
				enableImplicitHighToLowPrecisionConversion=false;
				enableImplicitSignedToUnsignedConversion=false;
				enableSubscribeOnPointer=true;
			}

			bool BasicAlgorithmConfiguration::DecodeInteger(BasicPrimitiveTypeEnum type, bool& sign, int& bytes)const
			{
				switch(type)
				{
				case s8:
					sign=true;
					bytes=1;
					return true;
				case s16:
					sign=true;
					bytes=2;
					return true;
				case s32:
					sign=true;
					bytes=4;
					return true;
				case s64:
					sign=true;
					bytes=8;
					return true;
				case u8:
					sign=false;
					bytes=1;
					return true;
				case u16:
					sign=false;
					bytes=2;
					return true;
				case u32:
					sign=false;
					bytes=4;
					return true;
				case u64:
					sign=false;
					bytes=8;
					return true;
				case bool_type:
					if(enableImplicitBooleanToIntegerConversion)
					{
						sign=false;
						bytes=1;
						return true;
					}
					else
					{
						return false;
					}
				case char_type:
					if(treatCharacterAsInteger)
					{
						sign=treatCharAsSignedInteger;
						bytes=1;
						return true;
					}
					else
					{
						return false;
					}
				case wchar_type:
					if(treatCharacterAsInteger)
					{
						sign=treatWCharAsSignedInteger;
						bytes=2;
						return true;
					}
					else
					{
						return false;
					}
				default:
					return false;
				}
			}

			bool BasicAlgorithmConfiguration::EncodeInteger(BasicPrimitiveTypeEnum& type, bool sign, int bytes)const
			{
				switch(bytes)
				{
				case 1:
					type=sign?s8:u8;
					return true;
				case 2:
					type=sign?s16:u16;
					return true;
				case 4:
					type=sign?s32:u32;
					return true;
				case 8:
					type=sign?s64:u64;
					return true;
				default:
					return false;
				}
			}

			bool BasicAlgorithmConfiguration::DecodeFloat(BasicPrimitiveTypeEnum type, int& bytes)const
			{
				switch(type)
				{
				case f32:
					bytes=4;
					return true;
				case f64:
					bytes=8;
					return true;
				default:
					return false;
				}
			}

			bool BasicAlgorithmConfiguration::EncodeFloat(BasicPrimitiveTypeEnum& type, int bytes)const
			{
				switch(bytes)
				{
				case 4:
					type=f32;
					return true;
				case 8:
					type=f64;
					return true;
				default:
					return false;
				}
			}

			bool BasicAlgorithmConfiguration::CanImplicitConvertTo(BasicPrimitiveTypeEnum from, BasicPrimitiveTypeEnum to)const
			{
				if(from==to)return true;
				switch(from)
				{
				case s8:case s16:case s32:case s64:case u8:case u16:case u32:case u64:case char_type:case wchar_type:
					{
						bool fromSign=false;
						int fromBytes=0;
						if(!DecodeInteger(from, fromSign, fromBytes))
						{
							return false;
						}
						else switch(to)
						{
						case s8:case s16:case s32:case s64:case u8:case u16:case u32:case u64:case char_type:case wchar_type:
							{
								bool toSign=false;
								int toBytes=0;
								if(DecodeInteger(to, toSign, toBytes))
								{
									if(!enableImplicitSignedToUnsignedConversion && fromSign && !toSign)
									{
										return false;
									}
									else if(!enableImplicitHighToLowPrecisionConversion && fromBytes>toBytes)
									{
										return false;
									}
									else
									{
										return true;
									}
								}
								else
								{
									return false;
								}
							}
						case f32:case f64:
							{
								int toBytes=0;
								DecodeFloat(to, toBytes);
								return enableImplicitIntegerToFloatConversion && (fromBytes<toBytes || enableImplicitHighToLowPrecisionConversion);
							}
						case bool_type:
							return enableImplicitIntegerToBooleanConversion;
						default:
							return false;
						}
					}
				case f32:case f64:
					{
						int fromBytes=0;
						DecodeFloat(from, fromBytes);
						switch(to)
						{
						case s8:case s16:case s32:case s64:case u8:case u16:case u32:case u64:case char_type:case wchar_type:
							{
								bool toSign=false;
								int toBytes=0;
								if(DecodeInteger(to, toSign, toBytes))
								{
									return enableImplicitFloatToIntegerConversion && (fromBytes<toBytes || enableImplicitHighToLowPrecisionConversion);
								}
								else
								{
									return false;
								}
							}
						case f32:case f64:
							{
								int toBytes=0;
								DecodeFloat(to, toBytes);
								return fromBytes<=toBytes || enableImplicitHighToLowPrecisionConversion;
							}
						case bool_type:
							return enableImplicitIntegerToBooleanConversion && enableImplicitFloatToIntegerConversion;
						default:
							return false;
						}
					}
				case bool_type:
					{
						switch(to)
						{
						case s8:case s16:case s32:case s64:case u8:case u16:case u32:case u64:case char_type:case wchar_type:
							{
								bool toSign=false;
								int toBytes=0;
								if(DecodeInteger(to, toSign, toBytes))
								{
									return enableImplicitBooleanToIntegerConversion;
								}
								else
								{
									return false;
								}
							}
						case f32:case f64:
							return enableImplicitBooleanToIntegerConversion && enableImplicitIntegerToFloatConversion;
						case bool_type:
							return true;
						default:
							return false;
						}
					}
				default:
					return false;
				}
			}

			bool BasicAlgorithmConfiguration::CanExplicitConvertTo(BasicPrimitiveTypeEnum from, BasicPrimitiveTypeEnum to)const
			{
				switch(from)
				{
				case s8:case s16:case s32:case s64:case u8:case u16:case u32:case u64:
					{
						switch(to)
						{
						case s8:case s16:case s32:case s64:case u8:case u16:case u32:case u64:
							return true;
						case f32:case f64:
							return true;
						case char_type:case wchar_type:
							return true;
						case bool_type:
							return enableImplicitIntegerToBooleanConversion;
						default:
							return false;
						}
					}
				case f32:case f64:
					{
						switch(to)
						{
						case s8:case s16:case s32:case s64:case u8:case u16:case u32:case u64:
							return true;
						case f32:case f64:
							return true;
						case char_type:case wchar_type:
							return treatCharacterAsInteger || enableImplicitFloatToIntegerConversion;
						case bool_type:
							return enableImplicitIntegerToBooleanConversion || enableImplicitFloatToIntegerConversion;
						default:
							return false;
						}
					}
				case char_type:case wchar_type:
					{
						switch(to)
						{
						case s8:case s16:case s32:case s64:case u8:case u16:case u32:case u64:
							return true;
						case f32:case f64:
							return treatCharacterAsInteger || enableImplicitIntegerToFloatConversion;
						case char_type:case wchar_type:
							return treatCharacterAsInteger;
						case bool_type:
							return treatCharacterAsInteger || enableImplicitIntegerToBooleanConversion;
						default:
							return false;
						}
					}
				case bool_type:
					{
						switch(to)
						{
						case s8:case s16:case s32:case s64:case u8:case u16:case u32:case u64:
							return enableImplicitBooleanToIntegerConversion;
						case f32:case f64:
							return enableImplicitBooleanToIntegerConversion;
						case char_type:case wchar_type:
							return enableImplicitBooleanToIntegerConversion;
						case bool_type:
							return enableImplicitBooleanToIntegerConversion && treatCharacterAsInteger;
						default:
							return false;
						}
					}
				default:
					return false;
				}
			}

			bool BasicAlgorithmConfiguration::BooleanUnaryOperatorTypeConversion(BasicPrimitiveTypeEnum operand, BasicPrimitiveTypeEnum& result)const
			{
				result=bool_type;
				return CanImplicitConvertTo(operand, bool_type);
			}

			bool BasicAlgorithmConfiguration::IntegerUnaryOperatorTypeConversion(BasicPrimitiveTypeEnum operand, BasicPrimitiveTypeEnum& result)const
			{
				bool sign=false;
				int bytes=0;
				if(DecodeInteger(operand, sign, bytes))
				{
					EncodeInteger(result, sign, bytes);
					return true;
				}
				else
				{
					return false;
				}
			}

			bool BasicAlgorithmConfiguration::NumberUnaryOperatorTypeConversion(BasicPrimitiveTypeEnum operand, BasicPrimitiveTypeEnum& result)const
			{
				bool sign=false;
				int bytes=0;
				if(DecodeFloat(operand, bytes))
				{
					EncodeFloat(result, bytes);
					return true;
				}
				else if(DecodeInteger(operand, sign, bytes))
				{
					EncodeInteger(result, sign, bytes);
					return true;
				}
				else
				{
					return false;
				}
			}

			bool BasicAlgorithmConfiguration::BooleanBinaryOperatorTypeConversion(BasicPrimitiveTypeEnum left, BasicPrimitiveTypeEnum right, BasicPrimitiveTypeEnum& result)const
			{
				result=bool_type;
				return CanImplicitConvertTo(left, bool_type) && CanImplicitConvertTo(right, bool_type);
			}

			bool BasicAlgorithmConfiguration::IntegerBinaryOperatorTypeConversion(bool leftSign, int leftBytes, bool rightSign, int rightBytes, BasicPrimitiveTypeEnum& result)const
			{
				switch(integerOperationConversion)
				{
				case SameTypeConversion:
					EncodeInteger(result, leftSign, leftBytes);
					return leftSign==rightSign && leftBytes==rightBytes;
				case SameSignConversion:
					EncodeInteger(result, (leftSign==rightSign?true:leftSign), leftBytes);
					return leftBytes==rightBytes;
				case FreeConversion:
					EncodeInteger(result, (leftSign==rightSign?true:leftSign), (leftBytes>rightBytes?leftBytes:rightBytes));
					return true;
				default:
					return false;
				}
			}

			bool BasicAlgorithmConfiguration::IntegerBinaryOperatorTypeConversion(BasicPrimitiveTypeEnum left, BasicPrimitiveTypeEnum right, BasicPrimitiveTypeEnum& result)const
			{
				bool leftSign=false;
				bool rightSign=false;
				int leftBytes=0;
				int rightBytes=0;
				if(DecodeInteger(left, leftSign, leftBytes) && DecodeInteger(right, rightSign, rightBytes))
				{
					return IntegerBinaryOperatorTypeConversion(leftSign, leftBytes, rightSign, rightBytes, result);
				}
				else
				{
					return false;
				}
			}

			bool BasicAlgorithmConfiguration::NumberBinaryOperatorTypeConversion(BasicPrimitiveTypeEnum left, BasicPrimitiveTypeEnum right, BasicPrimitiveTypeEnum& result)const
			{
				bool leftSign=false;
				bool rightSign=false;
				int leftBytes=0;
				int rightBytes=0;
				if(DecodeInteger(left, leftSign, leftBytes))
				{
					if(DecodeInteger(right, rightSign, rightBytes))
					{
						return IntegerBinaryOperatorTypeConversion(leftSign, leftBytes, rightSign, rightBytes, result);
					}
					else if(DecodeFloat(right, rightBytes))
					{
						result=f64;
						return enableImplicitIntegerToFloatConversion;
					}
				}
				else if(DecodeFloat(left, leftBytes))
				{
					if(DecodeInteger(right, rightSign, rightBytes))
					{
						result=f64;
						return enableImplicitIntegerToFloatConversion;
					}
					else if(DecodeFloat(right, rightBytes))
					{
						EncodeFloat(result, (leftBytes>rightBytes?leftBytes:rightBytes));
						return true;
					}
				}
				return false;
			}
		}
	}
}