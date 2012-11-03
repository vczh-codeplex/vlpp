#include <iostream>
#include <string>
#include <vector>
#include <memory>
#include <Windows.h>
#include <usp10.h>

#pragma comment(lib, "usp10.lib")
#pragma comment(lib, "gdi32.lib")

using namespace std;

const wchar_t* paragraph = L"For performance reasons, your application should not call ScriptRecordDigitSubstitution frequently. The function requires considerable overhead to call it every time ScriptItemize or ScriptStringAnalyse is called. Instead, the application can save the SCRIPT_DIGITSUBSTITUTE structure and update it only when a WM_SETTINGCHANGE message is received. Alternatively, the application can update the structure when a RegNotifyChangeKeyValue call in a dedicated thread indicates a change in the registry under HKCU\\Control Panel\\International.";

struct ItemInfo
{
	const wchar_t*			stringBuffer;
	int						stringLength;
	SCRIPT_ITEM				scriptItem;
};

struct RangeInfo
{
	SCRIPT_CACHE			cache;
	vector<WORD>			glyphs;		// glyphCount
	vector<WORD>			logGlyphs;	// charCount
	vector<SCRIPT_VISATTR>	svas;		// glyphCount
	vector<SCRIPT_LOGATTR>	slas;		// charCount

	RangeInfo():cache(0){}
};

struct PlaceInfo
{
	vector<int>			advancedWidth;
	vector<GOFFSET>		offset;
	ABC					abc;
	SCRIPT_ANALYSIS		sa;

	PlaceInfo()
	{
	}
};

int main()
{
	SCRIPT_DIGITSUBSTITUTE sds = {0};
	SCRIPT_CONTROL sc = {0};
	SCRIPT_STATE   ss = {0};
	ScriptRecordDigitSubstitution(LOCALE_USER_DEFAULT, &sds);
	ScriptApplyDigitSubstitution(&sds, &sc, &ss);

	int paragraphLength=wcslen(paragraph);
	vector<shared_ptr<ItemInfo>> scriptItems;
	{
		int scriptItemLength=paragraphLength;
		while(true)
		{
			int itemCount=0;
			SCRIPT_ITEM* itemBuffer=new SCRIPT_ITEM[scriptItemLength+1];
			HRESULT hr=ScriptItemize(
				paragraph,
				paragraphLength,
				scriptItemLength,
				&sc,
				&ss,
				&itemBuffer[0],
				&itemCount);
			if(hr==E_OUTOFMEMORY)
			{
				delete[] itemBuffer;
				scriptItemLength+=paragraphLength;
			}
			else
			{
				for(int i=0;i<itemCount;i++)
				{
					shared_ptr<ItemInfo> itemInfo(new ItemInfo);
					itemInfo->stringBuffer=paragraph+itemBuffer[i].iCharPos;
					itemInfo->stringLength=itemBuffer[i+1].iCharPos-itemBuffer[i].iCharPos;
					itemInfo->scriptItem=itemBuffer[i];
					scriptItems.push_back(itemInfo);
				}
				delete[] itemBuffer;
				break;
			}
		}
	}

	// cut each item using char styles into ranges

	HDC hdc=CreateCompatibleDC(NULL);
	{
		LOGFONT logFont = {0};
		wcscpy(logFont.lfFaceName, L"Î¢ÈíÑÅºÚ");
		logFont.lfHeight=20;
		logFont.lfWeight=FW_NORMAL;
		HFONT font=CreateFontIndirect(&logFont);
		SelectObject(hdc, font);
	}
	vector<shared_ptr<RangeInfo>> scriptRanges;
	for(int i=0;i<(int)scriptItems.size();i++)
	{
		ItemInfo* itemInfo=scriptItems[i].get();

		int glyphCount=(int)(1.5*itemInfo->stringLength)+16;
		vector<SCRIPT_VISATTR> svas;
		vector<WORD> glyphs;
		vector<WORD> logGlyphs;
		vector<SCRIPT_LOGATTR> slas;
		svas.resize(glyphCount);
		glyphs.resize(glyphCount);
		logGlyphs.resize(itemInfo->stringLength);
		slas.resize(itemInfo->stringLength);
		HDC hdcArgument=NULL;
		SCRIPT_CACHE cacheArgument=NULL;
		while(true)
		{
			HRESULT hr=ScriptShape(
				hdcArgument,
				&cacheArgument,
				itemInfo->stringBuffer,
				itemInfo->stringLength,
				glyphCount,
				&itemInfo->scriptItem.a,
				&glyphs[0],
				&logGlyphs[0],
				&svas[0],
				&glyphCount);
			switch(hr)
			{
			case E_OUTOFMEMORY:
				glyphCount=svas.size()+itemInfo->stringLength;
				svas.resize(glyphCount);
				glyphs.resize(glyphCount);
				break;
			case E_PENDING:
				hdcArgument=hdc;
				break;
			case USP_E_SCRIPT_NOT_IN_FONT:
				break;
			default:
				goto FINISH_SCRIPT_SHAPE;
			}
		}
FINISH_SCRIPT_SHAPE:
		ScriptBreak(itemInfo->stringBuffer, itemInfo->stringLength, &itemInfo->scriptItem.a, &slas[0]);
		shared_ptr<RangeInfo> rangeInfo(new RangeInfo);
		rangeInfo->cache=cacheArgument;
		rangeInfo->svas.insert(rangeInfo->svas.begin(), &svas[0], &svas[0]+glyphCount);
		rangeInfo->glyphs.insert(rangeInfo->glyphs.begin(), &glyphs[0], &glyphs[0]+glyphCount);
		rangeInfo->logGlyphs.insert(rangeInfo->logGlyphs.begin(), &logGlyphs[0], &logGlyphs[0]+itemInfo->stringLength);
		rangeInfo->slas.insert(rangeInfo->slas.begin(), &slas[0], &slas[0]+itemInfo->stringLength);
		scriptRanges.push_back(rangeInfo);
	}

	vector<shared_ptr<PlaceInfo>> placeInfos;
	for(int i=0;i<(int)scriptRanges.size();i++)
	{
		RangeInfo* rangeInfo=scriptRanges[i].get();
		shared_ptr<PlaceInfo> placeInfo(new PlaceInfo);
		placeInfo->advancedWidth.resize(rangeInfo->glyphs.size());
		placeInfo->offset.resize(rangeInfo->glyphs.size());
		placeInfo->sa=scriptItems[i]->scriptItem.a;
		HRESULT hr=ScriptPlace(
			NULL,
			&rangeInfo->cache,
			&rangeInfo->glyphs[0],
			(int)rangeInfo->glyphs.size(),
			&rangeInfo->svas[0],
			&placeInfo->sa,
			&placeInfo->advancedWidth[0],
			&placeInfo->offset[0],
			&placeInfo->abc);
		if(hr==E_PENDING)
		{
			hr=ScriptPlace(
				hdc,
				&rangeInfo->cache,
				&rangeInfo->glyphs[0],
				(int)rangeInfo->glyphs.size(),
				&rangeInfo->svas[0],
				&placeInfo->sa,
				&placeInfo->advancedWidth[0],
				&placeInfo->offset[0],
				&placeInfo->abc);
		}
		placeInfos.push_back(placeInfo);
	}

	vector<BYTE> bidiLevels;
	vector<int> v2ls;
	vector<int> l2vs;
	bidiLevels.resize(placeInfos.size());
	v2ls.resize(placeInfos.size());
	l2vs.resize(placeInfos.size());
	for(int i=0;i<(int)placeInfos.size();i++)
	{
		bidiLevels[i]==placeInfos[i]->sa.s.uBidiLevel;
	}
	ScriptLayout((int)bidiLevels.size(), &bidiLevels[0], &v2ls[0], &l2vs[0]);

	for(int i=0;i<(int)scriptRanges.size();i++)
	{
		ScriptFreeCache(&scriptRanges[0]->cache);
	}
	DeleteDC(hdc);
	return 0;
}