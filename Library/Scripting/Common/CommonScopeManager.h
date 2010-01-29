/***********************************************************************
Vczh Library++ 3.0
Developer: ³Âè÷å«(vczh)
Scripting::Common

Classes:
***********************************************************************/

#ifndef VCZH_SCRIPTING_COMMON_COMMONSCOPEMANAGER
#define VCZH_SCRIPTING_COMMON_COMMONSCOPEMANAGER

#include "..\..\Collections\List.h"
#include "..\..\Collections\Dictionary.h"
#include "..\..\Pointer.h"

namespace vl
{
	namespace scripting
	{
		template<typename T>
		class CommonScope : public Object, private NotCopyable
		{
		protected:
			T*									previousScope;
			collections::List<T*>				relatedScopes;
		public:
			CommonScope(T* _previousScope)
				:previousScope(_previousScope)
			{
			}

			T* PreviousScope()
			{
				return previousScope;
			}

			const collections::IReadonlyList<T*>& RelatedScopes()
			{
				return relatedScopes.Wrap();
			}
		};

		template<typename T, typename K, typename V>
		class CommonScopeItems : public Object, private NotCopyable
		{
			friend T;
		private:
			CommonScopeItems<T, K, V> T::*		self;
			T*									owner;
			collections::Dictionary<K, V>		items;
			V									defaultValue;

			CommonScopeItems<T, K, V>* GetItems(T* scope)
			{
				return &(scope->*self);
			}

			
			void Initialize(T* _owner, CommonScopeItems<T, K, V> T::* _self, V _defaultValue)
			{
				owner=_owner;
				self=_self;
				defaultValue=_defaultValue;
			}
		public:
			CommonScopeItems()
			{
			}

			const collections::IDictionary<K, V>& Items()
			{
				return items.Wrap();
			}

			void Add(const K& key, const V& value)
			{
				items.Add(key, value);
			}

			V Find(const K& key, T*& scope)
			{
				scope=0;
				int index=items.Keys().IndexOf(key);
				if(index!=-1)
				{
					scope=owner;
					return items.Values()[index];
				}
				for(int i=0;i<owner->RelatedScopes().Count();i++)
				{
					V result=GetItems(owner->RelatedScopes()[i])->Find(key, scope);
					if(scope)
					{
						return result;
					}
				}
				if(owner->PreviousScope())
				{
					return GetItems(owner->PreviousScope())->Find(key, scope);
				}
				return defaultValue;
			}

			V Find(const K& key)
			{
				T* scope=0;
				return Find(key, scope);
			}
		};
	}
}

#endif