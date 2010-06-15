#include "LanguageProvider.h"

namespace vl
{
	namespace scripting
	{

/***********************************************************************
LanguageException
***********************************************************************/

		LanguageException::LanguageException(const WString& message, vint _lineIndex, vint _lineStart, vint _textStart, vint _codeIndex)
			:Exception(message)
			,lineIndex(_lineIndex)
			,lineStart(_lineStart)
			,textStart(_textStart)
			,codeIndex(_codeIndex)
		{
		}

		LanguageException::~LanguageException()
		{
		}

		vint LanguageException::LineIndex()const
		{
			return lineIndex;
		}

		vint LanguageException::LineStart()const
		{
			return lineStart;
		}

		vint LanguageException::TextStart()const
		{
			return textStart;
		}

		vint LanguageException::CodeIndex()const
		{
			return codeIndex;
		}
	}
}