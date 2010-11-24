#include "UnitTestCompilingHelper.h"

/***********************************************************************
Size
***********************************************************************/

TEST_CASE(TestBasicTypeInfo)
{
	BasicProgramNode program;
	program.DefineStructureForward(L"Link");
	program.DefineRename(L"PLink", *t_type(L"Link"));
	program.DefineStructure(L"Link")
		.Member(L"data", t_int())
		.Member(L"prev", t_type(L"PLink"))
		.Member(L"next", t_type(L"PLink"));
	program.DefineStructure(L"Type")
		.Member(L"a", t_char())
		.Member(L"b", t_int32())
		.Member(L"c", t_int8())
		.Member(L"d", t_int16())
		.Member(L"e", t_double())
		.Member(L"f", t_int8());

	BasicAnalyzer analyzer(program.GetInternalValue(), 0, BasicAlgorithmConfiguration());
	analyzer.Analyze();
	BasicCodegenInfo info(&analyzer);

	List<Ptr<BasicLanguageCodeException>> errors;
	SortedList<WString> forwardStructures;
	BP argument(analyzer.GetEnv(), analyzer.GetEnv()->GlobalScope(), analyzer.GetTypeManager(), errors, forwardStructures);

	BasicTypeRecord* typeInt8=BasicLanguage_GetTypeRecord(t_int8().GetInternalValue(), argument, true);
	BasicTypeRecord* typePInt8=BasicLanguage_GetTypeRecord((*t_int8()).GetInternalValue(), argument, true);
	BasicTypeRecord* typeFunc=BasicLanguage_GetTypeRecord(t_int8()(t_types()<<t_bool()).GetInternalValue(), argument, true);
	BasicTypeRecord* typeInt8s=BasicLanguage_GetTypeRecord(t_int8()[10].GetInternalValue(), argument, true);
	BasicTypeRecord* typeLink=BasicLanguage_GetTypeRecord(t_type(L"Link").GetInternalValue(), argument, true);
	BasicTypeRecord* typeType=BasicLanguage_GetTypeRecord(t_type(L"Type").GetInternalValue(), argument, true);

	BasicTypeInfo* infoInt8=info.GetTypeInfo(typeInt8);
	BasicTypeInfo* infoPInt8=info.GetTypeInfo(typePInt8);
	BasicTypeInfo* infoFunc=info.GetTypeInfo(typeFunc);
	BasicTypeInfo* infoInt8s=info.GetTypeInfo(typeInt8s);
	BasicTypeInfo* infoLink=info.GetTypeInfo(typeLink);
	BasicTypeInfo* infoType=info.GetTypeInfo(typeType);

	TEST_ASSERT(infoInt8->size==(vint)1);
	TEST_ASSERT(infoPInt8->size==(vint)sizeof(void*));
	TEST_ASSERT(infoFunc->size==(vint)sizeof(void*));
	TEST_ASSERT(infoInt8s->size==(vint)10);
	TEST_ASSERT(infoLink->size==(vint)sizeof(void*)*3);
	TEST_ASSERT(infoLink->offsets.Count()==(vint)3);
	TEST_ASSERT(infoLink->offsets[0]==(vint)0);
	TEST_ASSERT(infoLink->offsets[1]==(vint)sizeof(void*));
	TEST_ASSERT(infoLink->offsets[2]==(vint)sizeof(void*)*2);
	TEST_ASSERT(infoType->size==(vint)17);
	TEST_ASSERT(infoType->offsets.Count()==(vint)6);
	TEST_ASSERT(infoType->offsets[0]==(vint)0);
	TEST_ASSERT(infoType->offsets[1]==(vint)1);
	TEST_ASSERT(infoType->offsets[2]==(vint)5);
	TEST_ASSERT(infoType->offsets[3]==(vint)6);
	TEST_ASSERT(infoType->offsets[4]==(vint)8);
	TEST_ASSERT(infoType->offsets[5]==(vint)16);
}