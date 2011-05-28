/***********************************************************************
Vczh Library++ 3.0
Developer: ³Âè÷å«(vczh)
Scripting::Native X

Classes:
***********************************************************************/

#ifndef VCZH_SCRIPTING_LANGUAGES_NATIVEX_NATIVEXCODEGENERATOR
#define VCZH_SCRIPTING_LANGUAGES_NATIVEX_NATIVEXCODEGENERATOR

#include "..\..\BasicLanguage\BasicLanguageExpression.h"
#include "..\..\..\Stream\Accessor.h"

namespace vl
{
	namespace scripting
	{
		namespace language_nativex
		{
			using namespace basiclanguage;
			using namespace stream;

			extern const wchar_t*			PrimitiveTypeToString(BasicPrimitiveTypeEnum type);
			extern const wchar_t*			UnaryOperatorToString(BasicUnaryExpression::OperatorType type);
			extern const wchar_t*			BinaryOperatorToString(BasicBinaryExpression::OperatorType type);

			typedef struct NativeXCodeGeneratorParameter
			{
				TextWriter&					writer;
				vint						indentation;

				NativeXCodeGeneratorParameter(TextWriter& _writer, vint _indentation)
					:writer(_writer)
					,indentation(_indentation)
				{
				}
			} NXCGP;

			EXTERN_ALGORITHM_PROCEDURE(NativeX_BasicType_GenerateCode, BasicType, NXCGP)
			EXTERN_ALGORITHM_PROCEDURE(NativeX_BasicExpression_GenerateCode, BasicExpression, NXCGP)
			EXTERN_ALGORITHM_PROCEDURE(NativeX_BasicStatement_GenerateCode, BasicStatement, NXCGP)
			EXTERN_ALGORITHM_PROCEDURE(NativeX_BasicDeclaration_GenerateCode, BasicDeclaration, NXCGP)

			extern void NativeX_BasicProgram_GenerateCode(Ptr<BasicProgram> program, TextWriter& writer);
		}
	}
}

#endif