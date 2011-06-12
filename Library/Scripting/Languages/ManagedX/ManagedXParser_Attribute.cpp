#include "ManagedXParser_Common.h"

namespace vl
{
	namespace scripting
	{
		namespace language_managedx
		{
/***********************************************************************
ManagedXParserImpl
***********************************************************************/

			void ManagedXParserImpl::LoadAttributes()
			{
				/*--------ATTRIBUTES--------*/

				setterAccessor			= let(PUBLIC, declatt::Public)
										|	let(PROTECTED+INTERNAL, declatt::ProtectedInternal)
										|	let(PROTECTED, declatt::Protected)
										|	let(PRIVATE, declatt::Private)
										|	let(INTERNAL, declatt::Internal)
										;

				accessor				= def(	setterAccessor
											,	declatt::Private);

				inheritation			= def(	let(SEALED, declatt::Sealed)
											|	let(ABSTRACT, declatt::Abstract)
											|	let(VIRTUAL, declatt::Virtual)
											|	let(OVERRIDE, declatt::Override)
											,	declatt::Normal);

				memberType				= def(let(STATIC, declatt::Static), declatt::Instance);

				dataType				= def(	let(CONST, declatt::Constant)
											|	let(READONLY, declatt::Readonly)
											,	declatt::Variable);

				implicitExplicit		= def(	let(IMPLICIT, true)
											|	let(EXPLICIT, false)
											,	true);

				genericArgconv			= def(	let(IN, ManagedGenericInfo::In)
											|	let(OUT, ManagedGenericInfo::Out)
											,	ManagedGenericInfo::None);

				functionArgconv			= def(	let(PARAMS, ManagedParameter::Params)
											|	let(REF, ManagedParameter::Ref)
											|	let(OUT, ManagedParameter::Out)
											,	ManagedParameter::Normal);

				invokeArgconv			= def(	let(REF, ManagedArgument::Ref)
											|	let(OUT, ManagedArgument::Out)
											,	ManagedArgument::Normal);
			}
		}
	}
}