#include "..\..\Library\UnitTest\UnitTest.h"
#include "..\..\Library\Function.h"
#include "..\..\Library\Scripting\BasicLanguage\BasicLanguageConfiguration.h"

using namespace vl;
using namespace vl::collections;
using namespace vl::scripting::basiclanguage;

TEST_CASE(TestConfiguration_EncodeDecodeInteger)
{
	BasicAlgorithmConfiguration config;
	bool sign=false;
	vint bytes=0;
	BasicPrimitiveTypeEnum type;

	TEST_ASSERT(config.DecodeInteger(s8, sign, bytes)==true);
	TEST_ASSERT(sign==true && bytes==1);
	TEST_ASSERT(config.DecodeInteger(s16, sign, bytes)==true);
	TEST_ASSERT(sign==true && bytes==2);
	TEST_ASSERT(config.DecodeInteger(s32, sign, bytes)==true);
	TEST_ASSERT(sign==true && bytes==4);
	TEST_ASSERT(config.DecodeInteger(s64, sign, bytes)==true);
	TEST_ASSERT(sign==true && bytes==8);

	TEST_ASSERT(config.DecodeInteger(u8, sign, bytes)==true);
	TEST_ASSERT(sign==false && bytes==1);
	TEST_ASSERT(config.DecodeInteger(u16, sign, bytes)==true);
	TEST_ASSERT(sign==false && bytes==2);
	TEST_ASSERT(config.DecodeInteger(u32, sign, bytes)==true);
	TEST_ASSERT(sign==false && bytes==4);
	TEST_ASSERT(config.DecodeInteger(u64, sign, bytes)==true);
	TEST_ASSERT(sign==false && bytes==8);
	
	TEST_ASSERT(config.DecodeInteger(f32, sign, bytes)==false);
	TEST_ASSERT(config.DecodeInteger(f64, sign, bytes)==false);
	TEST_ASSERT(config.DecodeInteger(bool_type, sign, bytes)==false);
	TEST_ASSERT(config.DecodeInteger(void_type, sign, bytes)==false);
	TEST_ASSERT(config.DecodeInteger(char_type, sign, bytes)==false);
	TEST_ASSERT(config.DecodeInteger(wchar_type, sign, bytes)==false);

	config.treatCharacterAsInteger=true;

	config.treatCharAsSignedInteger=true;
	config.treatWCharAsSignedInteger=true;
	TEST_ASSERT(config.DecodeInteger(char_type, sign, bytes)==true);
	TEST_ASSERT(sign==true && bytes==1);
	TEST_ASSERT(config.DecodeInteger(wchar_type, sign, bytes)==true);
	TEST_ASSERT(sign==true && bytes==2);

	config.treatCharAsSignedInteger=false;
	config.treatWCharAsSignedInteger=true;
	TEST_ASSERT(config.DecodeInteger(char_type, sign, bytes)==true);
	TEST_ASSERT(sign==false && bytes==1);
	TEST_ASSERT(config.DecodeInteger(wchar_type, sign, bytes)==true);
	TEST_ASSERT(sign==true && bytes==2);

	config.treatCharAsSignedInteger=true;
	config.treatWCharAsSignedInteger=false;
	TEST_ASSERT(config.DecodeInteger(char_type, sign, bytes)==true);
	TEST_ASSERT(sign==true && bytes==1);
	TEST_ASSERT(config.DecodeInteger(wchar_type, sign, bytes)==true);
	TEST_ASSERT(sign==false && bytes==2);

	config.treatCharAsSignedInteger=false;
	config.treatWCharAsSignedInteger=false;
	TEST_ASSERT(config.DecodeInteger(char_type, sign, bytes)==true);
	TEST_ASSERT(sign==false && bytes==1);
	TEST_ASSERT(config.DecodeInteger(wchar_type, sign, bytes)==true);
	TEST_ASSERT(sign==false && bytes==2);

	TEST_ASSERT(config.EncodeInteger(type, true, 1)==true);
	TEST_ASSERT(type==s8);
	TEST_ASSERT(config.EncodeInteger(type, true, 2)==true);
	TEST_ASSERT(type==s16);
	TEST_ASSERT(config.EncodeInteger(type, true, 4)==true);
	TEST_ASSERT(type==s32);
	TEST_ASSERT(config.EncodeInteger(type, true, 8)==true);
	TEST_ASSERT(type==s64);

	TEST_ASSERT(config.EncodeInteger(type, false, 1)==true);
	TEST_ASSERT(type==u8);
	TEST_ASSERT(config.EncodeInteger(type, false, 2)==true);
	TEST_ASSERT(type==u16);
	TEST_ASSERT(config.EncodeInteger(type, false, 4)==true);
	TEST_ASSERT(type==u32);
	TEST_ASSERT(config.EncodeInteger(type, false, 8)==true);
	TEST_ASSERT(type==u64);
}

const bool T=true;
const bool F=false;
#define X

TEST_CASE(TestConfiguration_StrictConversion)
{
	BasicAlgorithmConfiguration config;

	BasicPrimitiveTypeEnum types[]={s8, s16, s32, s64, u8, u16, u32, u64, f32, f64, char_type, wchar_type, bool_type, void_type};
	bool implicitResult[sizeof(types)/sizeof(*types)][sizeof(types)/sizeof(*types)]={
	//				 s8,   s16,   s32, s64,  u8,   u16,  u32,  u64,  f32,  f64,  char, wchar,bool, void
	/*s8*/			{T,    T,    T,    T,    F,    F,    F,    F,    F,    F,    F,    F,    F,    F,    },
	/*s16*/			{F,    T,    T,    T,    F,    F,    F,    F,    F,    F,    F,    F,    F,    F,    },
	/*s32*/			{F,    F,    T,    T,    F,    F,    F,    F,    F,    F,    F,    F,    F,    F,    },
	/*s64*/			{F,    F,    F,    T,    F,    F,    F,    F,    F,    F,    F,    F,    F,    F,    },
	//-----------------------------------------------------------------------------------------------------
	/*u8*/			{F,    F,    F,    F,    T,    T,    T,    T,    F,    F,    F,    F,    F,    F,    },
	/*u16*/			{F,    F,    F,    F,    F,    T,    T,    T,    F,    F,    F,    F,    F,    F,    },
	/*u32*/			{F,    F,    F,    F,    F,    F,    T,    T,    F,    F,    F,    F,    F,    F,    },
	/*u64*/			{F,    F,    F,    F,    F,    F,    F,    T,    F,    F,    F,    F,    F,    F,    },
	//-----------------------------------------------------------------------------------------------------
	/*f32*/			{F,    F,    F,    F,    F,    F,    F,    F,    T,    T,    F,    F,    F,    F,    },
	/*f64*/			{F,    F,    F,    F,    F,    F,    F,    F,    F,    T,    F,    F,    F,    F,    },
	//-----------------------------------------------------------------------------------------------------
	/*char_type*/	{F,    F,    F,    F,    F,    F,    F,    F,    F,    F,    T,    F,    F,    F,    },
	/*wchar_type*/	{F,    F,    F,    F,    F,    F,    F,    F,    F,    F,    F,    T,    F,    F,    },
	//-----------------------------------------------------------------------------------------------------
	/*bool_type*/	{F,    F,    F,    F,    F,    F,    F,    F,    F,    F,    F,    F,    T,    F,    },
	/*void_type*/	{F,    F,    F,    F,    F,    F,    F,    F,    F,    F,    F,    F,    F,    F,    },
	};
	bool explicitResult[sizeof(types)/sizeof(*types)][sizeof(types)/sizeof(*types)]={
	//				 s8,   s16,   s32, s64,  u8,   u16,  u32,  u64,  f32,  f64,  char, wchar,bool, void
	/*s8*/			{T,    T,    T,    T,    T,    T,    T,    T,    T,    T,    T,    T,    F,    F,    },
	/*s16*/			{T,    T,    T,    T,    T,    T,    T,    T,    T,    T,    T,    T,    F,    F,    },
	/*s32*/			{T,    T,    T,    T,    T,    T,    T,    T,    T,    T,    T,    T,    F,    F,    },
	/*s64*/			{T,    T,    T,    T,    T,    T,    T,    T,    T,    T,    T,    T,    F,    F,    },
	//-----------------------------------------------------------------------------------------------------
	/*u8*/			{T,    T,    T,    T,    T,    T,    T,    T,    T,    T,    T,    T,    F,    F,    },
	/*u16*/			{T,    T,    T,    T,    T,    T,    T,    T,    T,    T,    T,    T,    F,    F,    },
	/*u32*/			{T,    T,    T,    T,    T,    T,    T,    T,    T,    T,    T,    T,    F,    F,    },
	/*u64*/			{T,    T,    T,    T,    T,    T,    T,    T,    T,    T,    T,    T,    F,    F,    },
	//-----------------------------------------------------------------------------------------------------
	/*f32*/			{T,    T,    T,    T,    T,    T,    T,    T,    T,    T,    F,    F,    F,    F,    },
	/*f64*/			{T,    T,    T,    T,    T,    T,    T,    T,    T,    T,    F,    F,    F,    F,    },
	//-----------------------------------------------------------------------------------------------------
	/*char_type*/	{T,    T,    T,    T,    T,    T,    T,    T,    F,    F,    T,    F,    F,    F,    },
	/*wchar_type*/	{T,    T,    T,    T,    T,    T,    T,    T,    F,    F,    F,    T,    F,    F,    },
	//-----------------------------------------------------------------------------------------------------
	/*bool_type*/	{F,    F,    F,    F,    F,    F,    F,    F,    F,    F,    F,    F,    T,    F,    },
	/*void_type*/	{F,    F,    F,    F,    F,    F,    F,    F,    F,    F,    F,    F,    F,    F,    },
	};

	for(vint i=0;i<sizeof(types)/sizeof(*types);i++)
		for(vint j=0;j<sizeof(types)/sizeof(*types);j++)
		{
			TEST_ASSERT(config.CanImplicitConvertTo(types[i], types[j])==implicitResult[i][j]);
			TEST_ASSERT(config.CanExplicitConvertTo(types[i], types[j])==explicitResult[i][j]);
		}
}

TEST_CASE(TestConfiguration_CSharpConversion)
{
	BasicAlgorithmConfiguration config;
	config.enableImplicitIntegerToFloatConversion=true;

	BasicPrimitiveTypeEnum types[]={s8, s16, s32, s64, u8, u16, u32, u64, f32, f64, char_type, wchar_type, bool_type, void_type};
	bool implicitResult[sizeof(types)/sizeof(*types)][sizeof(types)/sizeof(*types)]={
	//				 s8,   s16,   s32, s64,  u8,   u16,  u32,  u64,  f32,  f64,  char, wchar,bool, void
	/*s8*/			{T,    T,    T,    T,    F,    F,    F,    F,    T,    T,    F,    F,    F,    F,    },
	/*s16*/			{F,    T,    T,    T,    F,    F,    F,    F,    T,    T,    F,    F,    F,    F,    },
	/*s32*/			{F,    F,    T,    T,    F,    F,    F,    F,    F,    T,    F,    F,    F,    F,    },
	/*s64*/			{F,    F,    F,    T,    F,    F,    F,    F,    F,    F,    F,    F,    F,    F,    },
	//-----------------------------------------------------------------------------------------------------
	/*u8*/			{F,    F,    F,    F,    T,    T,    T,    T,    T,    T,    F,    F,    F,    F,    },
	/*u16*/			{F,    F,    F,    F,    F,    T,    T,    T,    T,    T,    F,    F,    F,    F,    },
	/*u32*/			{F,    F,    F,    F,    F,    F,    T,    T,    F,    T,    F,    F,    F,    F,    },
	/*u64*/			{F,    F,    F,    F,    F,    F,    F,    T,    F,    F,    F,    F,    F,    F,    },
	//-----------------------------------------------------------------------------------------------------
	/*f32*/			{F,    F,    F,    F,    F,    F,    F,    F,    T,    T,    F,    F,    F,    F,    },
	/*f64*/			{F,    F,    F,    F,    F,    F,    F,    F,    F,    T,    F,    F,    F,    F,    },
	//-----------------------------------------------------------------------------------------------------
	/*char_type*/	{F,    F,    F,    F,    F,    F,    F,    F,    F,    F,    T,    F,    F,    F,    },
	/*wchar_type*/	{F,    F,    F,    F,    F,    F,    F,    F,    F,    F,    F,    T,    F,    F,    },
	//-----------------------------------------------------------------------------------------------------
	/*bool_type*/	{F,    F,    F,    F,    F,    F,    F,    F,    F,    F,    F,    F,    T,    F,    },
	/*void_type*/	{F,    F,    F,    F,    F,    F,    F,    F,    F,    F,    F,    F,    F,    F,    },
	};
	bool explicitResult[sizeof(types)/sizeof(*types)][sizeof(types)/sizeof(*types)]={
	//				 s8,   s16,   s32, s64,  u8,   u16,  u32,  u64,  f32,  f64,  char, wchar,bool, void
	/*s8*/			{T,    T,    T,    T,    T,    T,    T,    T,    T,    T,    T,    T,    F,    F,    },
	/*s16*/			{T,    T,    T,    T,    T,    T,    T,    T,    T,    T,    T,    T,    F,    F,    },
	/*s32*/			{T,    T,    T,    T,    T,    T,    T,    T,    T,    T,    T,    T,    F,    F,    },
	/*s64*/			{T,    T,    T,    T,    T,    T,    T,    T,    T,    T,    T,    T,    F,    F,    },
	//-----------------------------------------------------------------------------------------------------
	/*u8*/			{T,    T,    T,    T,    T,    T,    T,    T,    T,    T,    T,    T,    F,    F,    },
	/*u16*/			{T,    T,    T,    T,    T,    T,    T,    T,    T,    T,    T,    T,    F,    F,    },
	/*u32*/			{T,    T,    T,    T,    T,    T,    T,    T,    T,    T,    T,    T,    F,    F,    },
	/*u64*/			{T,    T,    T,    T,    T,    T,    T,    T,    T,    T,    T,    T,    F,    F,    },
	//-----------------------------------------------------------------------------------------------------
	/*f32*/			{T,    T,    T,    T,    T,    T,    T,    T,    T,    T,    F,    F,    F,    F,    },
	/*f64*/			{T,    T,    T,    T,    T,    T,    T,    T,    T,    T,    F,    F,    F,    F,    },
	//-----------------------------------------------------------------------------------------------------
	/*char_type*/	{T,    T,    T,    T,    T,    T,    T,    T,    T,    T,    T,    F,    F,    F,    },
	/*wchar_type*/	{T,    T,    T,    T,    T,    T,    T,    T,    T,    T,    F,    T,    F,    F,    },
	//-----------------------------------------------------------------------------------------------------
	/*bool_type*/	{F,    F,    F,    F,    F,    F,    F,    F,    F,    F,    F,    F,    T,    F,    },
	/*void_type*/	{F,    F,    F,    F,    F,    F,    F,    F,    F,    F,    F,    F,    F,    F,    },
	};

	for(vint i=0;i<sizeof(types)/sizeof(*types);i++)
		for(vint j=0;j<sizeof(types)/sizeof(*types);j++)
		{
			TEST_ASSERT(config.CanImplicitConvertTo(types[i], types[j])==implicitResult[i][j]);
			TEST_ASSERT(config.CanExplicitConvertTo(types[i], types[j])==explicitResult[i][j]);
		}
}

TEST_CASE(TestConfiguration_CppConversion)
{
	BasicAlgorithmConfiguration config;
	config.treatCharacterAsInteger=true;
	config.enableImplicitBooleanToIntegerConversion=true;
	config.enableImplicitIntegerToBooleanConversion=true;
	config.enableImplicitIntegerToFloatConversion=true;
	config.enableImplicitUnsignedToSignedConversion=true;

	BasicPrimitiveTypeEnum types[]={s8, s16, s32, s64, u8, u16, u32, u64, f32, f64, char_type, wchar_type, bool_type, void_type};
	bool implicitResult[sizeof(types)/sizeof(*types)][sizeof(types)/sizeof(*types)]={
	//				 s8,   s16,   s32, s64,  u8,   u16,  u32,  u64,  f32,  f64,  char, wchar,bool, void
	/*s8*/			{T,    T,    T,    T,    F,    F,    F,    F,    T,    T,    T,    F,    T,    F,    },
	/*s16*/			{F,    T,    T,    T,    F,    F,    F,    F,    T,    T,    F,    F,    T,    F,    },
	/*s32*/			{F,    F,    T,    T,    F,    F,    F,    F,    F,    T,    F,    F,    T,    F,    },
	/*s64*/			{F,    F,    F,    T,    F,    F,    F,    F,    F,    F,    F,    F,    T,    F,    },
	//-----------------------------------------------------------------------------------------------------
	/*u8*/			{T,    T,    T,    T,    T,    T,    T,    T,    T,    T,    T,    T,    T,    F,    },
	/*u16*/			{F,    T,    T,    T,    F,    T,    T,    T,    T,    T,    F,    T,    T,    F,    },
	/*u32*/			{F,    F,    T,    T,    F,    F,    T,    T,    F,    T,    F,    F,    T,    F,    },
	/*u64*/			{F,    F,    F,    T,    F,    F,    F,    T,    F,    F,    F,    F,    T,    F,    },
	//-----------------------------------------------------------------------------------------------------
	/*f32*/			{F,    F,    F,    F,    F,    F,    F,    F,    T,    T,    F,    F,    F,    F,    },
	/*f64*/			{F,    F,    F,    F,    F,    F,    F,    F,    F,    T,    F,    F,    F,    F,    },
	//-----------------------------------------------------------------------------------------------------
	/*char_type*/	{T,    T,    T,    T,    F,    F,    F,    F,    T,    T,    T,    F,    T,    F,    },
	/*wchar_type*/	{F,    T,    T,    T,    F,    T,    T,    T,    T,    T,    F,    T,    T,    F,    },
	//-----------------------------------------------------------------------------------------------------
	/*bool_type*/	{T,    T,    T,    T,    T,    T,    T,    T,    T,    T,    T,    T,    T,    F,    },
	/*void_type*/	{F,    F,    F,    F,    F,    F,    F,    F,    F,    F,    F,    F,    F,    F,    },
	};
	bool explicitResult[sizeof(types)/sizeof(*types)][sizeof(types)/sizeof(*types)]={
	//				 s8,   s16,   s32, s64,  u8,   u16,  u32,  u64,  f32,  f64,  char, wchar,bool, void
	/*s8*/			{T,    T,    T,    T,    T,    T,    T,    T,    T,    T,    T,    T,    T,    F,    },
	/*s16*/			{T,    T,    T,    T,    T,    T,    T,    T,    T,    T,    T,    T,    T,    F,    },
	/*s32*/			{T,    T,    T,    T,    T,    T,    T,    T,    T,    T,    T,    T,    T,    F,    },
	/*s64*/			{T,    T,    T,    T,    T,    T,    T,    T,    T,    T,    T,    T,    T,    F,    },
	//-----------------------------------------------------------------------------------------------------
	/*u8*/			{T,    T,    T,    T,    T,    T,    T,    T,    T,    T,    T,    T,    T,    F,    },
	/*u16*/			{T,    T,    T,    T,    T,    T,    T,    T,    T,    T,    T,    T,    T,    F,    },
	/*u32*/			{T,    T,    T,    T,    T,    T,    T,    T,    T,    T,    T,    T,    T,    F,    },
	/*u64*/			{T,    T,    T,    T,    T,    T,    T,    T,    T,    T,    T,    T,    T,    F,    },
	//-----------------------------------------------------------------------------------------------------
	/*f32*/			{T,    T,    T,    T,    T,    T,    T,    T,    T,    T,    T,    T,    T,    F,    },
	/*f64*/			{T,    T,    T,    T,    T,    T,    T,    T,    T,    T,    T,    T,    T,    F,    },
	//-----------------------------------------------------------------------------------------------------
	/*char_type*/	{T,    T,    T,    T,    T,    T,    T,    T,    T,    T,    T,    T,    T,    F,    },
	/*wchar_type*/	{T,    T,    T,    T,    T,    T,    T,    T,    T,    T,    T,    T,    T,    F,    },
	//-----------------------------------------------------------------------------------------------------
	/*bool_type*/	{T,    T,    T,    T,    T,    T,    T,    T,    T,    T,    T,    T,    T,    F,    },
	/*void_type*/	{F,    F,    F,    F,    F,    F,    F,    F,    F,    F,    F,    F,    F,    F,    },
	};

	for(vint i=0;i<sizeof(types)/sizeof(*types);i++)
		for(vint j=0;j<sizeof(types)/sizeof(*types);j++)
		{
			TEST_ASSERT(config.CanImplicitConvertTo(types[i], types[j])==implicitResult[i][j]);
			TEST_ASSERT(config.CanExplicitConvertTo(types[i], types[j])==explicitResult[i][j]);
		}
}

TEST_CASE(TestConfiguration_FreeConversion)
{
	BasicAlgorithmConfiguration config;
	config.treatCharacterAsInteger=true;
	config.enableImplicitBooleanToIntegerConversion=true;
	config.enableImplicitIntegerToBooleanConversion=true;
	config.enableImplicitPointerToBooleanConversion=true;
	config.enableImplicitFloatToIntegerConversion=true;
	config.enableImplicitIntegerToFloatConversion=true;
	config.enableImplicitHighToLowPrecisionConversion=true;
	config.enableImplicitSignedToUnsignedConversion=true;
	config.enableImplicitUnsignedToSignedConversion=true;

	BasicPrimitiveTypeEnum types[]={s8, s16, s32, s64, u8, u16, u32, u64, f32, f64, char_type, wchar_type, bool_type, void_type};
	bool implicitResult[sizeof(types)/sizeof(*types)][sizeof(types)/sizeof(*types)]={
	//				 s8,   s16,   s32, s64,  u8,   u16,  u32,  u64,  f32,  f64,  char, wchar,bool, void
	/*s8*/			{T,    T,    T,    T,    T,    T,    T,    T,    T,    T,    T,    T,    T,    F,    },
	/*s16*/			{T,    T,    T,    T,    T,    T,    T,    T,    T,    T,    T,    T,    T,    F,    },
	/*s32*/			{T,    T,    T,    T,    T,    T,    T,    T,    T,    T,    T,    T,    T,    F,    },
	/*s64*/			{T,    T,    T,    T,    T,    T,    T,    T,    T,    T,    T,    T,    T,    F,    },
	//-----------------------------------------------------------------------------------------------------
	/*u8*/			{T,    T,    T,    T,    T,    T,    T,    T,    T,    T,    T,    T,    T,    F,    },
	/*u16*/			{T,    T,    T,    T,    T,    T,    T,    T,    T,    T,    T,    T,    T,    F,    },
	/*u32*/			{T,    T,    T,    T,    T,    T,    T,    T,    T,    T,    T,    T,    T,    F,    },
	/*u64*/			{T,    T,    T,    T,    T,    T,    T,    T,    T,    T,    T,    T,    T,    F,    },
	//-----------------------------------------------------------------------------------------------------
	/*f32*/			{T,    T,    T,    T,    T,    T,    T,    T,    T,    T,    T,    T,    T,    F,    },
	/*f64*/			{T,    T,    T,    T,    T,    T,    T,    T,    T,    T,    T,    T,    T,    F,    },
	//-----------------------------------------------------------------------------------------------------
	/*char_type*/	{T,    T,    T,    T,    T,    T,    T,    T,    T,    T,    T,    T,    T,    F,    },
	/*wchar_type*/	{T,    T,    T,    T,    T,    T,    T,    T,    T,    T,    T,    T,    T,    F,    },
	//-----------------------------------------------------------------------------------------------------
	/*bool_type*/	{T,    T,    T,    T,    T,    T,    T,    T,    T,    T,    T,    T,    T,    F,    },
	/*void_type*/	{F,    F,    F,    F,    F,    F,    F,    F,    F,    F,    F,    F,    F,    F,    },
	};
	bool explicitResult[sizeof(types)/sizeof(*types)][sizeof(types)/sizeof(*types)]={
	//				 s8,   s16,   s32, s64,  u8,   u16,  u32,  u64,  f32,  f64,  char, wchar,bool, void
	/*s8*/			{T,    T,    T,    T,    T,    T,    T,    T,    T,    T,    T,    T,    T,    F,    },
	/*s16*/			{T,    T,    T,    T,    T,    T,    T,    T,    T,    T,    T,    T,    T,    F,    },
	/*s32*/			{T,    T,    T,    T,    T,    T,    T,    T,    T,    T,    T,    T,    T,    F,    },
	/*s64*/			{T,    T,    T,    T,    T,    T,    T,    T,    T,    T,    T,    T,    T,    F,    },
	//-----------------------------------------------------------------------------------------------------
	/*u8*/			{T,    T,    T,    T,    T,    T,    T,    T,    T,    T,    T,    T,    T,    F,    },
	/*u16*/			{T,    T,    T,    T,    T,    T,    T,    T,    T,    T,    T,    T,    T,    F,    },
	/*u32*/			{T,    T,    T,    T,    T,    T,    T,    T,    T,    T,    T,    T,    T,    F,    },
	/*u64*/			{T,    T,    T,    T,    T,    T,    T,    T,    T,    T,    T,    T,    T,    F,    },
	//-----------------------------------------------------------------------------------------------------
	/*f32*/			{T,    T,    T,    T,    T,    T,    T,    T,    T,    T,    T,    T,    T,    F,    },
	/*f64*/			{T,    T,    T,    T,    T,    T,    T,    T,    T,    T,    T,    T,    T,    F,    },
	//-----------------------------------------------------------------------------------------------------
	/*char_type*/	{T,    T,    T,    T,    T,    T,    T,    T,    T,    T,    T,    T,    T,    F,    },
	/*wchar_type*/	{T,    T,    T,    T,    T,    T,    T,    T,    T,    T,    T,    T,    T,    F,    },
	//-----------------------------------------------------------------------------------------------------
	/*bool_type*/	{T,    T,    T,    T,    T,    T,    T,    T,    T,    T,    T,    T,    T,    F,    },
	/*void_type*/	{F,    F,    F,    F,    F,    F,    F,    F,    F,    F,    F,    F,    F,    F,    },
	};

	for(vint i=0;i<sizeof(types)/sizeof(*types);i++)
		for(vint j=0;j<sizeof(types)/sizeof(*types);j++)
		{
			TEST_ASSERT(config.CanImplicitConvertTo(types[i], types[j])==implicitResult[i][j]);
			TEST_ASSERT(config.CanExplicitConvertTo(types[i], types[j])==explicitResult[i][j]);
		}
}

TEST_CASE(TestConfiguration_UnaryConversion)
{
	BasicAlgorithmConfiguration config;
	config.treatCharacterAsInteger=true;
	config.enableImplicitBooleanToIntegerConversion=true;
	config.enableImplicitIntegerToBooleanConversion=true;
	config.enableImplicitIntegerToFloatConversion=true;
	config.enableImplicitUnsignedToSignedConversion=true;
	
	BasicPrimitiveTypeEnum types[]={s8, s16, s32, s64, u8, u16, u32, u64, f32, f64, char_type, wchar_type, bool_type, void_type};
	BasicPrimitiveTypeEnum unaryResult[sizeof(types)/sizeof(*types)][3]={
	//				bool,          integer,   number,
	/*s8*/			{bool_type,    s8,        s8,       },
	/*s16*/			{bool_type,    s16,       s16,      },
	/*s32*/			{bool_type,    s32,       s32,      },
	/*s64*/			{bool_type,    s64,       s64,      },
	/*u8*/			{bool_type,    u8,        u8,       },
	/*u16*/			{bool_type,    u16,       u16,      },
	/*u32*/			{bool_type,    u32,       u32,      },
	/*u64*/			{bool_type,    u64,       u64,      },
	/*f32*/			{void_type,    void_type, f32,      },
	/*f64*/			{void_type,    void_type, f64,      },
	/*char_type*/	{bool_type,    s8,        s8,       },
	/*wchar_type*/	{bool_type,    u16,       u16,      },
	/*bool_type*/	{bool_type,    u8,        u8,       },
	/*void_type*/	{void_type,    void_type, void_type,},
	};

	for(vint i=0;i<sizeof(types)/sizeof(*types);i++)
	{
		BasicPrimitiveTypeEnum result=void_type;
		TEST_ASSERT(config.BooleanUnaryOperatorTypeConversion(types[i], result)==(unaryResult[i][0]!=void_type));
		if(unaryResult[i][0]!=void_type)
		{
			TEST_ASSERT(result==unaryResult[i][0]);
		}
		TEST_ASSERT(config.IntegerUnaryOperatorTypeConversion(types[i], result)==(unaryResult[i][1]!=void_type));
		if(unaryResult[i][1]!=void_type)
		{
			TEST_ASSERT(result==unaryResult[i][1]);
		}
		TEST_ASSERT(config.NumberUnaryOperatorTypeConversion(types[i], result)==(unaryResult[i][2]!=void_type));
		if(unaryResult[i][2]!=void_type)
		{
			TEST_ASSERT(result==unaryResult[i][2]);
		}
	}
}

TEST_CASE(TestConfiguration_BooleanBinaryConversion)
{
	BasicAlgorithmConfiguration config;
	config.treatCharacterAsInteger=true;
	config.enableImplicitBooleanToIntegerConversion=true;
	config.enableImplicitIntegerToBooleanConversion=true;
	config.enableImplicitIntegerToFloatConversion=true;
	config.enableImplicitUnsignedToSignedConversion=true;

	BasicPrimitiveTypeEnum types[]={s8, s16, s32, s64, u8, u16, u32, u64, f32, f64, char_type, wchar_type, bool_type, void_type};
	BasicPrimitiveTypeEnum conversionResult[sizeof(types)/sizeof(*types)][sizeof(types)/sizeof(*types)]={
	//				 s8,        s16,       s32,       s64,       u8,        u16,       u32,       u64,       f32,       f64,       char,      wchar,     bool,      void
	/*s8*/			{bool_type, bool_type, bool_type, bool_type, bool_type, bool_type, bool_type, bool_type, void_type, void_type, bool_type, bool_type, bool_type, void_type,},
	/*s16*/			{bool_type, bool_type, bool_type, bool_type, bool_type, bool_type, bool_type, bool_type, void_type, void_type, bool_type, bool_type, bool_type, void_type,},
	/*s32*/			{bool_type, bool_type, bool_type, bool_type, bool_type, bool_type, bool_type, bool_type, void_type, void_type, bool_type, bool_type, bool_type, void_type,},
	/*s64*/			{bool_type, bool_type, bool_type, bool_type, bool_type, bool_type, bool_type, bool_type, void_type, void_type, bool_type, bool_type, bool_type, void_type,},
	//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------
	/*u8*/			{bool_type, bool_type, bool_type, bool_type, bool_type, bool_type, bool_type, bool_type, void_type, void_type, bool_type, bool_type, bool_type, void_type,},
	/*u16*/			{bool_type, bool_type, bool_type, bool_type, bool_type, bool_type, bool_type, bool_type, void_type, void_type, bool_type, bool_type, bool_type, void_type,},
	/*u32*/			{bool_type, bool_type, bool_type, bool_type, bool_type, bool_type, bool_type, bool_type, void_type, void_type, bool_type, bool_type, bool_type, void_type,},
	/*u64*/			{bool_type, bool_type, bool_type, bool_type, bool_type, bool_type, bool_type, bool_type, void_type, void_type, bool_type, bool_type, bool_type, void_type,},
	//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------
	/*f32*/			{void_type, void_type, void_type, void_type, void_type, void_type, void_type, void_type, void_type, void_type, void_type, void_type, void_type, void_type,},
	/*f64*/			{void_type, void_type, void_type, void_type, void_type, void_type, void_type, void_type, void_type, void_type, void_type, void_type, void_type, void_type,},
	//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------
	/*char_type*/	{bool_type, bool_type, bool_type, bool_type, bool_type, bool_type, bool_type, bool_type, void_type, void_type, bool_type, bool_type, bool_type, void_type,},
	/*wchar_type*/	{bool_type, bool_type, bool_type, bool_type, bool_type, bool_type, bool_type, bool_type, void_type, void_type, bool_type, bool_type, bool_type, void_type,},
	//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------
	/*bool_type*/	{bool_type, bool_type, bool_type, bool_type, bool_type, bool_type, bool_type, bool_type, void_type, void_type, bool_type, bool_type, bool_type, void_type,},
	/*void_type*/	{void_type, void_type, void_type, void_type, void_type, void_type, void_type, void_type, void_type, void_type, void_type, void_type, void_type, void_type,},
	};

	for(vint i=0;i<sizeof(types)/sizeof(*types);i++)
		for(vint j=0;j<sizeof(types)/sizeof(*types);j++)
		{
			BasicPrimitiveTypeEnum result=void_type;
			TEST_ASSERT(config.BooleanBinaryOperatorTypeConversion(types[i], types[j], result)==(conversionResult[i][j]!=void_type));
			if(conversionResult[i][j]!=void_type)
			{
				TEST_ASSERT(result==conversionResult[i][j]);
			}
		}
}

TEST_CASE(TestConfiguration_IntegerBinaryConversion)
{
	BasicAlgorithmConfiguration config;
	config.treatCharacterAsInteger=true;
	config.enableImplicitBooleanToIntegerConversion=true;
	config.enableImplicitIntegerToBooleanConversion=true;
	config.enableImplicitIntegerToFloatConversion=true;
	config.enableImplicitUnsignedToSignedConversion=true;

	BasicPrimitiveTypeEnum types[]={s8, s16, s32, s64, u8, u16, u32, u64, f32, f64, char_type, wchar_type, bool_type, void_type};
	BasicPrimitiveTypeEnum conversionResult[sizeof(types)/sizeof(*types)][sizeof(types)/sizeof(*types)]={
	//				 s8,        s16,       s32,       s64,       u8,        u16,       u32,       u64,       f32,       f64,       char,      wchar,     bool,      void
	/*s8*/			{s8,        s16      , s32,       s64,       s8,        s16,       s32,       s64,       void_type, void_type, s8,        s16,       s8,        void_type,},
	/*s16*/			{s16,       s16      , s32,       s64,       s16,       s16,       s32,       s64,       void_type, void_type, s16,       s16,       s16,       void_type,},
	/*s32*/			{s32,       s32      , s32,       s64,       s32,       s32,       s32,       s64,       void_type, void_type, s32,       s32,       s32,       void_type,},
	/*s64*/			{s64,       s64      , s64,       s64,       s64,       s64,       s64,       s64,       void_type, void_type, s64,       s64,       s64,       void_type,},
	//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------
	/*u8*/			{s8,        s16      , s32,       s64,       u8,        u16,       u32,       u64,       void_type, void_type, s8,        u16,       u8,        void_type,},
	/*u16*/			{s16,       s16      , s32,       s64,       u16,       u16,       u32,       u64,       void_type, void_type, s16,       u16,       u16,       void_type,},
	/*u32*/			{s32,       s32      , s32,       s64,       u32,       u32,       u32,       u64,       void_type, void_type, s32,       u32,       u32,       void_type,},
	/*u64*/			{s64,       s64      , s64,       s64,       u64,       u64,       u64,       u64,       void_type, void_type, s64,       u64,       u64,       void_type,},
	//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------
	/*f32*/			{void_type, void_type, void_type, void_type, void_type, void_type, void_type, void_type, void_type, void_type, void_type, void_type, void_type, void_type,},
	/*f64*/			{void_type, void_type, void_type, void_type, void_type, void_type, void_type, void_type, void_type, void_type, void_type, void_type, void_type, void_type,},
	//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------
	/*char_type*/	{s8,        s16      , s32,       s64,       s8,        s16,       s32,       s64,       void_type, void_type, s8,        s16,       s8,        void_type,},
	/*wchar_type*/	{s16,       s16      , s32,       s64,       u16,       u16,       u32,       u64,       void_type, void_type, s16,       u16,       u16,       void_type,},
	//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------
	/*bool_type*/	{s8,        s16      , s32,       s64,       u8,        u16,       u32,       u64,       void_type, void_type, s8,        u16,       u8,        void_type,},
	/*void_type*/	{void_type, void_type, void_type, void_type, void_type, void_type, void_type, void_type, void_type, void_type, void_type, void_type, void_type, void_type,},
	};

	for(vint i=0;i<sizeof(types)/sizeof(*types);i++)
		for(vint j=0;j<sizeof(types)/sizeof(*types);j++)
		{
			BasicPrimitiveTypeEnum result=void_type;
			TEST_ASSERT(config.IntegerBinaryOperatorTypeConversion(types[i], types[j], result)==(conversionResult[i][j]!=void_type));
			if(conversionResult[i][j]!=void_type)
			{
				TEST_ASSERT(result==conversionResult[i][j]);
			}
		}
}

TEST_CASE(TestConfiguration_NumberBinaryConversion)
{
	BasicAlgorithmConfiguration config;
	config.treatCharacterAsInteger=true;
	config.enableImplicitBooleanToIntegerConversion=true;
	config.enableImplicitIntegerToBooleanConversion=true;
	config.enableImplicitIntegerToFloatConversion=true;
	config.enableImplicitUnsignedToSignedConversion=true;

	BasicPrimitiveTypeEnum types[]={s8, s16, s32, s64, u8, u16, u32, u64, f32, f64, char_type, wchar_type, bool_type, void_type};
	BasicPrimitiveTypeEnum conversionResult[sizeof(types)/sizeof(*types)][sizeof(types)/sizeof(*types)]={
	//				 s8,        s16,       s32,       s64,       u8,        u16,       u32,       u64,       f32,       f64,       char,      wchar,     bool,      void
	/*s8*/			{s8,        s16      , s32,       s64,       s8,        s16,       s32,       s64,       f32,       f64,       s8,        s16,       s8,        void_type,},
	/*s16*/			{s16,       s16      , s32,       s64,       s16,       s16,       s32,       s64,       f32,       f64,       s16,       s16,       s16,       void_type,},
	/*s32*/			{s32,       s32      , s32,       s64,       s32,       s32,       s32,       s64,       f32,       f64,       s32,       s32,       s32,       void_type,},
	/*s64*/			{s64,       s64      , s64,       s64,       s64,       s64,       s64,       s64,       f64,       f64,       s64,       s64,       s64,       void_type,},
	//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------
	/*u8*/			{s8,        s16      , s32,       s64,       u8,        u16,       u32,       u64,       f32,       f64,       s8,        u16,       u8,        void_type,},
	/*u16*/			{s16,       s16      , s32,       s64,       u16,       u16,       u32,       u64,       f32,       f64,       s16,       u16,       u16,       void_type,},
	/*u32*/			{s32,       s32      , s32,       s64,       u32,       u32,       u32,       u64,       f32,       f64,       s32,       u32,       u32,       void_type,},
	/*u64*/			{s64,       s64      , s64,       s64,       u64,       u64,       u64,       u64,       f64,       f64,       s64,       u64,       u64,       void_type,},
	//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------
	/*f32*/			{f32,       f32,       f32,       f64,       f32,       f32,       f32,       f64,       f32,       f64,       f32,       f32,       f32,       void_type,},
	/*f64*/			{f64,       f64,       f64,       f64,       f64,       f64,       f64,       f64,       f64,       f64,       f64,       f64,       f64,       void_type,},
	//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------
	/*char_type*/	{s8,        s16      , s32,       s64,       s8,        s16,       s32,       s64,       f32,       f64,       s8,        s16,       s8,        void_type,},
	/*wchar_type*/	{s16,       s16      , s32,       s64,       u16,       u16,       u32,       u64,       f32,       f64,       s16,       u16,       u16,       void_type,},
	//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------
	/*bool_type*/	{s8,        s16      , s32,       s64,       u8,        u16,       u32,       u64,       f32,       f64,       s8,        u16,       u8,        void_type,},
	/*void_type*/	{void_type, void_type, void_type, void_type, void_type, void_type, void_type, void_type, void_type, void_type, void_type, void_type, void_type, void_type,},
	};

	for(vint i=0;i<sizeof(types)/sizeof(*types);i++)
		for(vint j=0;j<sizeof(types)/sizeof(*types);j++)
		{
			BasicPrimitiveTypeEnum result=void_type;
			TEST_ASSERT(config.NumberBinaryOperatorTypeConversion(types[i], types[j], result)==(conversionResult[i][j]!=void_type));
			if(conversionResult[i][j]!=void_type)
			{
				TEST_ASSERT(result==conversionResult[i][j]);
			}
		}
}