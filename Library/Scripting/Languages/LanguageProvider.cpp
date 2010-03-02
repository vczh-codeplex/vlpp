#include "LanguageProvider.h"

namespace vl
{
	namespace scripting
	{

/***********************************************************************
LanguageException
***********************************************************************/

		LanguageException::LanguageException(const WString& message, int _lineIndex, int _lineStart, int _textStart, int _codeIndex)
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

		int LanguageException::LineIndex()const
		{
			return lineIndex;
		}

		int LanguageException::LineStart()const
		{
			return lineStart;
		}

		int LanguageException::TextStart()const
		{
			return textStart;
		}

		int LanguageException::CodeIndex()const
		{
			return codeIndex;
		}
	}
}