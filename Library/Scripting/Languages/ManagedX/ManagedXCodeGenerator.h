/***********************************************************************
Vczh Library++ 3.0
Developer: ³Âè÷å«(vczh)
Scripting::Managed X

Classes:
***********************************************************************/

#ifndef VCZH_SCRIPTING_LANGUAGES_MANAGEDX_MANAGEDXCODEGENERATOR
#define VCZH_SCRIPTING_LANGUAGES_MANAGEDX_MANAGEDXCODEGENERATOR

#include "..\..\ManagedLanguage\ManagedLanguageExpression.h"
#include "..\..\Common\AlgorithmDeclaration.h"
#include "..\..\..\Stream\Accessor.h"

namespace vl
{
	namespace scripting
	{
		namespace language_managedx
		{
			using namespace stream;
			using namespace managedlanguage;

			typedef struct ManagedXCodeGeneratorParameter
			{
				TextWriter&					writer;
				vint						indentation;

				ManagedXCodeGeneratorParameter(TextWriter& _writer, vint _indentation)
					:writer(_writer)
					,indentation(_indentation)
				{
				}
			} MXCGP;

			extern void ManagedX_GenerateCode_Program(Ptr<ManagedProgram> program, TextWriter& writer);
			EXTERN_ALGORITHM_PROCEDURE(ManagedX_GenerateCode_Type, ManagedType, MXCGP)
			EXTERN_ALGORITHM_PROCEDURE(ManagedX_GenerateCode_ExtendedType, ManagedExtendedType, MXCGP)
			EXTERN_ALGORITHM_PROCEDURE(ManagedX_GenerateCode_Expression, ManagedExpression, MXCGP)
			EXTERN_ALGORITHM_PROCEDURE(ManagedX_GenerateCode_ExtendedExpression, ManagedExtendedExpression, MXCGP)
			EXTERN_ALGORITHM_PROCEDURE(ManagedX_GenerateCode_Statement, ManagedStatement, MXCGP)
			EXTERN_ALGORITHM_PROCEDURE(ManagedX_GenerateCode_ExtendedStatement, ManagedExtendedStatement, MXCGP)
			EXTERN_ALGORITHM_PROCEDURE(ManagedX_GenerateCode_Member, ManagedMember, MXCGP)
			EXTERN_ALGORITHM_PROCEDURE(ManagedX_GenerateCode_ExtendedMember, ManagedExtendedMember, MXCGP)
			EXTERN_ALGORITHM_PROCEDURE(ManagedX_GenerateCode_Declaration, ManagedDeclaration, MXCGP)
			EXTERN_ALGORITHM_PROCEDURE(ManagedX_GenerateCode_ExtendedDeclaration, ManagedExtendedDeclaration, MXCGP)
		}
	}
}

#endif