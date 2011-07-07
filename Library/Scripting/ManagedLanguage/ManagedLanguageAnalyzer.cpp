#include "ManagedLanguageAnalyzer.h"
#include "..\..\Collections\OperationForEach.h"

namespace vl
{
	namespace scripting
	{
		namespace managedlanguage
		{
			using namespace collections;

/***********************************************************************
ManagedAnalyzerParameter
***********************************************************************/

			ManagedAnalyzerParameter::ManagedAnalyzerParameter(ManagedSymbolManager* _symbolManager, ErrorList& _errors)
				:symbolManager(_symbolManager)
				,currentSymbol(_symbolManager->Global())
				,errors(_errors)
			{
			}

			ManagedAnalyzerParameter::ManagedAnalyzerParameter(const ManagedAnalyzerParameter& parameter, ManagedSymbolItem* _currentSymbol)
				:symbolManager(parameter.symbolManager)
				,currentSymbol(_currentSymbol)
				,errors(parameter.errors)
			{
			}

/***********************************************************************
ManagedLanguage_AnalyzeProgram
***********************************************************************/

			void ManagedLanguage_AnalyzeProgram(Ptr<ManagedProgram> program, const MAP& argument)
			{
				// build symbol item place holder
				FOREACH(Ptr<ManagedDeclaration>, declaration, program->declarations.Wrap())
				{
					ManagedLanguage_BuildGlobalScope1_Declaration(declaration, argument);
				}
				if(argument.errors.Count()>0) return;
				
				// fill symbol item
				FOREACH(Ptr<ManagedDeclaration>, declaration, program->declarations.Wrap())
				{
					ManagedLanguage_BuildGlobalScope2_Declaration(declaration, argument);
				}
				if(argument.errors.Count()>0) return;
			}

/***********************************************************************
GetSystemType
***********************************************************************/

			void SearchType(List<ManagedSymbolItem*>& symbolItems, vint& start, vint& end, const WString& name, vint genericParameterCount)
			{
				// TODO: consider accessor
				for(vint i=start;i<end;i++)
				{
					ManagedSymbolItemGroup* group=symbolItems[i]->ItemGroup(name);
					if(group)
					{
						FOREACH(ManagedSymbolItem*, item, group->Items())
						{
							switch(item->GetSymbolType())
							{
							case ManagedSymbolItem::Namespace:
								{
									if(genericParameterCount==0)
									{
										symbolItems.Add(item);
									}
								}
								break;
							case ManagedSymbolItem::Class:
							case ManagedSymbolItem::Structure:
							case ManagedSymbolItem::Interface:
								{
									if(genericParameterCount==dynamic_cast<ManagedSymbolDeclaration*>(item)->orderedGenericParameterNames.Count())
									{
										symbolItems.Add(item);
									}
								}
								break;
							case ManagedSymbolItem::TypeRename:
								{
									if(genericParameterCount==dynamic_cast<ManagedSymbolTypeRename*>(item)->orderedGenericParameterNames.Count())
									{
										symbolItems.Add(item);
									}
								}
								break;
							}
						}
					}
				}
				start=end;
				end=symbolItems.Count();
			}

			void FilterType(List<ManagedSymbolItem*>& symbolItems, vint& start, vint& end, vint genericParameterCount)
			{
				for(vint i=start;i<end;i++)
				{
					ManagedSymbolItem* item=symbolItems[i];
					switch(item->GetSymbolType())
					{
					case ManagedSymbolItem::Class:
					case ManagedSymbolItem::Structure:
					case ManagedSymbolItem::Interface:
						{
							ManagedSymbolDeclaration* symbol=dynamic_cast<ManagedSymbolDeclaration*>(item);
							if(symbol->orderedGenericParameterNames.Count()==genericParameterCount)
							{
								symbolItems.Add(item);
							}
						}
						break;
					case ManagedSymbolItem::TypeRename:
						{
							ManagedSymbolTypeRename* symbol=dynamic_cast<ManagedSymbolTypeRename*>(item);
							if(symbol->orderedGenericParameterNames.Count()==genericParameterCount)
							{
								symbolItems.Add(item);
							}
						}
						break;
					}
				}
				start=end;
				end=symbolItems.Count();
			}

			ManagedTypeSymbol* GetSystemType(ManagedLanguageElement* element, const WString& name, const MAP& argument, vint genericParameterCount)
			{
				List<ManagedSymbolItem*> symbolItems;
				symbolItems.Add(argument.symbolManager->Global());
				vint start=0;
				vint end=symbolItems.Count();
				SearchType(symbolItems, start, end, L"System", 0);
				SearchType(symbolItems, start, end, name, 0);
				FilterType(symbolItems, start, end, genericParameterCount);
				if(start==end)
				{
					argument.errors.Add(ManagedLanguageCodeException::GetSystemTypeNotExists(element, name));
					return 0;
				}
				else if(start+1<end)
				{
					argument.errors.Add(ManagedLanguageCodeException::GetSystemTypeDuplicated(element, name));
					return 0;
				}
				else
				{
					ManagedSymbolItem* baseType=symbolItems[start];
					return argument.symbolManager->GetType(baseType);
				}
			}
		}
	}
}