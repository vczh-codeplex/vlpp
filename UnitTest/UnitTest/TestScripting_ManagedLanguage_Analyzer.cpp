#include "..\..\Library\UnitTest\UnitTest.h"
#include "..\..\Library\Scripting\ManagedLanguage\ManagedLanguageSymbolManager.h"
#include "..\..\Library\Collections\Operation.h"

using namespace vl;
using namespace vl::collections;
using namespace vl::scripting::managedlanguage;

TEST_CASE(Test_ManagedLanguage_SymbolManager)
{
	ManagedSymbolManager sm;
	{
		TEST_ASSERT(sm.Global()->GetName()==ManagedSymbolGlobal::SymbolName);
		TEST_ASSERT(sm.Global()->GetSymbolType()==ManagedSymbolItem::Global);
	}
	{
		ManagedSymbolItem* si1 = new ManagedSymbolItem(&sm, ManagedSymbolItem::Namespace);
		si1->SetName(L"A");
		sm.Global()->Add(si1);

		ManagedSymbolItem* si2 = new ManagedSymbolItem(&sm, ManagedSymbolItem::Namespace);
		si2->SetName(L"A");
		sm.Global()->Add(si2);

		ManagedSymbolItem* si3 = new ManagedSymbolItem(&sm, ManagedSymbolItem::Namespace);
		si3->SetName(L"B");
		sm.Global()->Add(si3);
	}
	{
		TEST_ASSERT(sm.Global()->ItemGroup(L"A")!=0);
		TEST_ASSERT(sm.Global()->ItemGroup(L"B")!=0);
		TEST_ASSERT(sm.Global()->ItemGroup(L"C")==0);

		FOREACH(ManagedSymbolItemGroup*, group, sm.Global()->ItemGroups().Values())
		{
			TEST_ASSERT(group->GetParentItem()==sm.Global());
			FOREACH(ManagedSymbolItem*, item, group->Items())
			{
				TEST_ASSERT(item->GetSymbolType()==ManagedSymbolItem::Namespace);
				TEST_ASSERT(item->GetParentGroup()==group);
				TEST_ASSERT(item->GetParentItem()==sm.Global());
				TEST_ASSERT(item->GetManager()==&sm);
				TEST_ASSERT(sm.Global()->ItemGroup(item->GetName())==group);
			}
		}
	}

	ManagedSymbolItem* si=sm.Global()->ItemGroup(L"B")->Items()[0];
	ManagedTypeSymbol* ts=sm.GetType(si);
	{
		TEST_ASSERT(sm.GetType(si)==ts);
		TEST_ASSERT(ts->GetManager()==&sm);
		TEST_ASSERT(ts->GetSymbol()==si);
	}

	ManagedTypeSymbol* tsA=sm.GetType(sm.Global()->ItemGroup(L"A")->Items()[0]);
	ManagedTypeSymbol* tsB=sm.GetType(sm.Global()->ItemGroup(L"A")->Items()[1]);
	TEST_ASSERT(ts!=tsA);
	TEST_ASSERT(ts!=tsB);
	TEST_ASSERT(tsA!=tsB);

	ManagedTypeSymbol* ts_A=0;
	ManagedTypeSymbol* ts_AB=0;

	List<ManagedTypeSymbol*> ga;
	{
		ga.Clear();
		ga.Add(tsA);
		ts_A=sm.GetType(si, ga.Wrap());
		TEST_ASSERT(sm.GetType(si, ga.Wrap())==ts_A);
		TEST_ASSERT(ts_A->GetManager()==&sm);
		TEST_ASSERT(ts_A->GetSymbol()==si);
		TEST_ASSERT(CompareEnumerable(ga.Wrap(), ts_A->GetGenericArguments())==0);
	}
	{
		ga.Clear();
		ga.Add(tsA);
		ga.Add(tsB);
		ts_AB=sm.GetType(si, ga.Wrap());
		TEST_ASSERT(sm.GetType(si, ga.Wrap())==ts_AB);
		TEST_ASSERT(ts_AB->GetManager()==&sm);
		TEST_ASSERT(ts_AB->GetSymbol()==si);
		TEST_ASSERT(CompareEnumerable(ga.Wrap(), ts_AB->GetGenericArguments())==0);
	}
	TEST_ASSERT(ts_A!=ts_AB);
}