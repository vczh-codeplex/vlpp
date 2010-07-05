#include "..\..\Library\UnitTest\UnitTest.h"
#include "..\..\Library\Scripting\BasicIL\BasicILDefinition.h"
#include "..\..\Library\Scripting\BasicIL\BasicILInterpretor.h"
#include "..\..\Library\Scripting\BasicIL\BasicILSymbolResource.h"

using namespace vl;
using namespace vl::scripting;
using namespace vl::scripting::basicil;

TEST_CASE(TestBasicILInstruction_Generic_Function)
{
	BasicIL il;
	{
		Ptr<ResourceStream> symbolResource=new ResourceStream;
		il.resources.Add(BasicILResourceNames::ExportedSymbols, symbolResource);

		ResourceRecord<BasicILEntryRes> entryRes=symbolResource->CreateRecord<BasicILEntryRes>();
		entryRes->assemblyName=symbolResource->CreateString(L"Map");
		entryRes->exports=ResourceArrayHandle<BasicILExportRes>::Null();
		entryRes->linkings=ResourceArrayHandle<BasicILLinkingRes>::Null();
		ResourceRecord<BasicILGenericRes> genericRes=symbolResource->CreateRecord<BasicILGenericRes>();
		entryRes->genericSymbols=genericRes;

		ResourceRecord<BasicILGenericFunctionEntryRes> entry0=symbolResource->CreateRecord<BasicILGenericFunctionEntryRes>();
		{
			entry0->genericArgumentCount=1;
			entry0->startInstruction=0;
			entry0->instructionCount=0;//TODO
			entry0->name=symbolResource->CreateString(L"FindFirst");
			ResourceRecord<BasicILGenericNameRes> name0=symbolResource->CreateRecord<BasicILGenericNameRes>();
			name0->isConstant=true;
			name0->constantString=symbolResource->CreateString(L"FindFirst<");
			name0->stringArgumentIndex=-1;
			ResourceRecord<BasicILGenericNameRes> name1=symbolResource->CreateRecord<BasicILGenericNameRes>();
			name1->isConstant=false;
			name1->constantString=ResourceString::Null();
			name1->stringArgumentIndex=0;
			ResourceRecord<BasicILGenericNameRes> name2=symbolResource->CreateRecord<BasicILGenericNameRes>();
			name2->isConstant=true;
			name2->constantString=symbolResource->CreateString(L">");
			name2->stringArgumentIndex=-1;
			ResourceArrayRecord<BasicILGenericNameRes> names=symbolResource->CreateArrayRecord<BasicILGenericNameRes>(3);
			names.Set(0, name0);
			names.Set(1, name1);
			names.Set(2, name2);
			entry0->uniqueNameTemplate=names;
		}
		ResourceRecord<BasicILGenericFunctionTargetRes> target0=symbolResource->CreateRecord<BasicILGenericFunctionTargetRes>();
		{
			target0->assemblyName=symbolResource->CreateString(L"Map");
			target0->symbolName=symbolResource->CreateString(L"FindFirst");

			ResourceRecord<BasicILGenericArgumentRes> argument0=symbolResource->CreateRecord<BasicILGenericArgumentRes>();
			{
				argument0->sizeArgument=sizeof(double);
				ResourceRecord<BasicILGenericNameRes> name0=symbolResource->CreateRecord<BasicILGenericNameRes>();
				name0->isConstant=true;
				name0->constantString=symbolResource->CreateString(L"PRIMITIVE:DOUBLE");
				name0->stringArgumentIndex=-1;
				ResourceArrayRecord<BasicILGenericNameRes> names=symbolResource->CreateArrayRecord<BasicILGenericNameRes>(1);
				names.Set(0, name0);
				argument0->nameArgument=names;
			}

			ResourceArrayRecord<BasicILGenericArgumentRes> arguments=symbolResource->CreateArrayRecord<BasicILGenericArgumentRes>(1);
			arguments.Set(0, argument0);
			target0->arguments=arguments;
		}
		ResourceRecord<BasicILGenericFunctionTargetRes> target1=symbolResource->CreateRecord<BasicILGenericFunctionTargetRes>();
		{
			target1->assemblyName=symbolResource->CreateString(L"Map");
			target1->symbolName=symbolResource->CreateString(L"FindFirst");

			ResourceRecord<BasicILGenericArgumentRes> argument0=symbolResource->CreateRecord<BasicILGenericArgumentRes>();
			{
				argument0->sizeArgument=sizeof(vint);
				ResourceRecord<BasicILGenericNameRes> name0=symbolResource->CreateRecord<BasicILGenericNameRes>();
				name0->isConstant=true;
				name0->constantString=symbolResource->CreateString(L"PRIMITIVE:INTEGER");
				name0->stringArgumentIndex=-1;
				ResourceArrayRecord<BasicILGenericNameRes> names=symbolResource->CreateArrayRecord<BasicILGenericNameRes>(1);
				names.Set(0, name0);
				argument0->nameArgument=names;
			}

			ResourceArrayRecord<BasicILGenericArgumentRes> arguments=symbolResource->CreateArrayRecord<BasicILGenericArgumentRes>(1);
			arguments.Set(0, argument0);
			target1->arguments=arguments;
		}
		ResourceRecord<BasicILGenericLinearRes> linear0=symbolResource->CreateRecord<BasicILGenericLinearRes>();
		{
			linear0->constant=sizeof(vint);
			ResourceRecord<BasicILGenericFactorItemRes> factor0=symbolResource->CreateRecord<BasicILGenericFactorItemRes>();
			factor0->factor=1;
			ResourceArrayRecord<BasicILGenericFactorItemRes> factors=symbolResource->CreateArrayRecord<BasicILGenericFactorItemRes>(1);
			factors.Set(0, factor0);
			linear0->factors=factors;
		}

		ResourceArrayRecord<BasicILGenericFunctionEntryRes> entries=symbolResource->CreateArrayRecord<BasicILGenericFunctionEntryRes>(1);
		entries.Set(0, entry0);
		ResourceArrayRecord<BasicILGenericFunctionTargetRes> targets=symbolResource->CreateArrayRecord<BasicILGenericFunctionTargetRes>(2);
		targets.Set(0, target0);
		targets.Set(1, target1);
		ResourceArrayRecord<BasicILGenericLinearRes> linears=symbolResource->CreateArrayRecord<BasicILGenericLinearRes>(1);
		linears.Set(0, linear0);
		genericRes->functionEntries=entries;
		genericRes->functionTargets=targets;
		genericRes->linears=linears;
	}

	BasicILInterpretor interpretor(1024);
	vint key=interpretor.LoadIL(&il);
}

TEST_CASE(TestBasicILInstruction_Generic_FunctionPointer)
{
}