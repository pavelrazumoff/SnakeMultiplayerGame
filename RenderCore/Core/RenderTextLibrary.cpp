#include "pch.h"
#include "RenderTextLibrary.h"

#include "RenderTextureLibrary.h"
#include <map>

static const uint8_t letterIndent = 1; // Indent between each letter.

using LetterMap = std::map<char, std::string>;

static const LetterMap letterArt = {
	{'A', " * \n* *\n***\n* *\n* *"},
	{'B', "**  \n* *\n** \n* *\n** "},
	{'C', " * \n*  \n*  \n*  \n * "},
	{'D', "** \n* *\n* *\n* *\n** "},
	{'E', "***\n*  \n***\n*  \n***"},
	{'F', "***\n*  \n***\n*  \n*  "},
	{'G', " * \n*  \n* *\n* *\n * "},
	{'H', "* *\n* *\n***\n* *\n* *"},
	{'I', "***\n * \n * \n * \n***"},
	{'J', " **\n  *\n  *\n* *\n **"},
	{'K', "* *\n* *\n** \n* *\n* *"},
	{'L', "*  \n*  \n*  \n*  \n***"},
	{'M', "* *\n***\n* *\n* *\n* *"},
	{'N', "* *\n**\n* *\n* *\n* *"},
	{'O', " * \n* *\n* *\n* *\n * "},
	{'P', "** \n* *\n** \n*  \n*  "},
	{'Q', " * \n* *\n* *\n* *\n **\n * "},
	{'R', "** \n* *\n** \n* *\n* *"},
	{'S', " **\n*  \n * \n  *\n** "},
	{'T', "***\n * \n * \n * \n * "},
	{'U', "* *\n* *\n* *\n* *\n***"},
	{'V', "* *\n* *\n* *\n* *\n * "},
	{'W', "* *\n* *\n***\n***\n* *"},
	{'X', "* *\n* *\n * \n* *\n* *"},
	{'Y', "* *\n* *\n* *\n * \n * "},
	{'Z', "***\n  *\n * \n*  \n***"},
	{'!', " * \n * \n * \n   \n * "},
	{' ', "   \n   \n   \n   \n   "},

	// Numbers.
	{'0', " * \n* *\n* *\n* *\n * "},
	{'1', " * \n** \n * \n * \n***"},
	{'2', " * \n* *\n  *\n * \n***"},
	{'3', " * \n* *\n **\n* *\n * "},
	{'4', "  *\n **\n* *\n***\n  *"},
	{'5', "***\n*  \n***\n  *\n***"},
	{'6', " * \n*  \n***\n* *\n * "},
	{'7', "***\n  *\n  *\n  *\n  *"},
	{'8', " * \n* *\n * \n* *\n * "},
	{'9', " * \n* *\n **\n  *\n * "},
};

static const LetterMap letterBigArt = {
	{'A', "  *  \n * * \n*****\n*   *\n*   *"},
	{'B', "**** \n*   *\n**** \n*   *\n**** "},
	{'C', " *** \n*    \n*    \n*    \n *** "},
	{'D', "**** \n*   *\n*   *\n*   *\n**** "},
	{'E', "*****\n*    \n*****\n*    \n*****"},
	{'F', "*****\n*    \n*****\n*    \n*    "},
	{'G', " *** \n*    \n*  **\n*   *\n *** "},
	{'H', "*   *\n*   *\n*****\n*   *\n*   *"},
	{'I', " *** \n  *  \n  *  \n  *  \n *** "},
	{'J', "  ***\n   * \n   * \n*  * \n **  "},
	{'K', "*   *\n*  * \n**  \n*  * \n*   *"},
	{'L', "*    \n*    \n*    \n*    \n*****"},
	{'M', "*   *\n** **\n* * *\n*   *\n*   *"},
	{'N', "*   *\n**  *\n* * *\n*  **\n*   *"},
	{'O', " *** \n*   *\n*   *\n*   *\n *** "},
	{'P', "**** \n*   *\n**** \n*    \n*    "},
	{'Q', " *** \n*   *\n*   *\n*  * \n ** *"},
	{'R', "**** \n*   *\n**** \n*  * \n*   *"},
	{'S', " *** \n*    \n *** \n    *\n***  "},
	{'T', "*****\n  *  \n  *  \n  *  \n  *  "},
	{'U', "*   *\n*   *\n*   *\n*   *\n *** "},
	{'V', "*   *\n*   *\n * * \n * * \n  *  "},
	{'W', "*   *\n*   *\n* * *\n* * *\n * * "},
	{'X', "*   *\n * * \n  *  \n * * \n*   *"},
	{'Y', "*   *\n * * \n  *  \n  *  \n  *  "},
	{'Z', "*****\n   * \n  *  \n *   \n*****"},
	{'!', "  *  \n  *  \n  *  \n     \n  *  "},
	{' ', "     \n     \n     \n     \n     "},

	// Numbers.
	{'0', " *** \n*   *\n*   *\n*   *\n *** "},
	{'1', "  *  \n **  \n  *  \n  *  \n*****"},
	{'2', " *** \n*   *\n   * \n  *  \n*****"},
	{'3', " *** \n*   *\n  ** \n*   *\n *** "},
	{'4', "   * \n  ** \n * * \n*****\n   * "},
	{'5', "*****\n*    \n**** \n    *\n**** "},
	{'6', " *** \n*    \n**** \n*   *\n *** "},
	{'7', "*****\n   * \n  *  \n *   \n*    "},
	{'8', " *** \n*   *\n *** \n*   *\n *** "},
	{'9', " *** \n*   *\n ****\n    *\n *** "},
};

/*
	Scaled text.
*/

static RC_SIZE _CalcLetterSize(char ch, const LetterMap& letterMap)
{
	RC_UINT maxLetterWidth = 0;
	RC_UINT maxLetterHeight = 0;

	ch = toupper(ch);

	auto it = letterMap.find(ch);
	if (it != letterMap.end())
	{
		const char* pch = it->second.c_str();
		const char* pLastSavedPch = pch;
		while (pch = strchr(pch, '\n'))
		{
			maxLetterWidth = max(maxLetterWidth, (RC_UINT)(pch - pLastSavedPch));
			maxLetterHeight++;

			pch++;
			pLastSavedPch = pch;
		}

		maxLetterWidth = max(maxLetterWidth, (RC_UINT)strlen(pLastSavedPch));
		maxLetterHeight++;
	}

	return { maxLetterWidth, maxLetterHeight };
}

static void _FillTextureWithScaledText(RCTexture* texture, std::string text, TEX_COLOR_RGB textColor, uint8_t textScale, TEX_COORD startCoord,
	const LetterMap& letterMap, TEX_PIXEL fillerPixel = 0)
{
	/*
		TODO: Text Alignment.
	*/

	const TEX_SIZE texSize = { texture->GetWidth(), texture->GetHeight() };
	const RC_UINT texLen = texture->GetWidth() * texture->GetHeight();

	TEX_PIXEL* texData = new TEX_PIXEL[texLen];
	//memset(texData, RenderConstants::ClearPixel, texLen * sizeof(TEX_PIXEL));
	memset(texData, RenderConstants::AlphaZeroPixel, texLen * sizeof(TEX_PIXEL));

	TEX_COORD drawCoord = startCoord;
	auto FILL_TEXDATA_WITH_LETTER = [&texData, &texSize, &drawCoord](char _ch, uint8_t nRepeat) -> bool {

		for (uint8_t i = 0; i < nRepeat; i++)
		{
			if (drawCoord.x >= texSize.cx) return false;
			texData[drawCoord.y * texSize.cx + drawCoord.x] = _ch;
			drawCoord.x++;
		}

		return true;
	};

	for (size_t ti = 0; ti < text.length(); ++ti)
	{
		char ch = toupper(text[ti]);

		RC_SIZE letterSize = _CalcLetterSize(ch, letterMap);

		if (ti > 0 && (ch != ' ')) drawCoord.x += letterIndent * textScale;
		drawCoord.y = startCoord.y;

		if (drawCoord.x >= texSize.cx) break;

		RC_UINT letterStartX = drawCoord.x;

		if (letterMap.find(ch) != letterMap.end())
		{
			// Print the scaled letter
			std::string letter = letterMap.at(ch);

			for (char letterCh : letter)
			{
				if (letterCh == '\n')
				{
					drawCoord.y++;
					drawCoord.x = letterStartX;

					if (drawCoord.y >= texSize.cy) break;
				}
				else
				{
					if (fillerPixel != 0 && letterCh == '*') letterCh = fillerPixel;
					if (!FILL_TEXDATA_WITH_LETTER(letterCh, textScale)) continue;
				}
			}
		}
		else if (ch == ' ')
		{
			// Handle spaces
			if (!FILL_TEXDATA_WITH_LETTER(' ', textScale))
				break;
		}
		else
		{
			// Handle unknown characters
			if (!FILL_TEXDATA_WITH_LETTER('?', letterSize.cx * textScale))
				break;
		}
	}

	TEX_RECT rect = { 0, 0, texSize.cx - 1, texSize.cy - 1 };
	TEX_COORD coord = { 0, 0 };
	RenderTextureLibrary::FillTexture(texture, texData, rect, texSize.cx, coord);
	RenderTextureLibrary::FillTextureColor(texture, textColor);

	delete[] texData;
	texData = nullptr;
}

#if USE_SCALED_TEXT()
void RenderTextLibrary::FillTextureWithScaledText(RCTexture* texture, std::string text, TEX_COLOR_RGB textColor, uint8_t textScale, TEX_COORD startCoord)
{
	_FillTextureWithScaledText(texture, text, textColor, textScale, startCoord, letterArt);
}

void RenderTextLibrary::Test_FillTextureWithScaledLetters(RCTexture* texture, char startLetter, TEX_COLOR_RGB textColor, uint8_t textScale, TEX_COORD startCoord)
{
	std::string testText;

	startLetter = toupper(startLetter);
	for (const auto& letter : letterArt)
	{
		if (letter.first >= 'A' && letter.first < startLetter) continue;

		testText += letter.first;
	}

	FillTextureWithScaledText(texture, testText, textColor, textScale, startCoord);
}

RC_SIZE RenderTextLibrary::GetScaledTextSize(std::string text, uint8_t textScale)
{
	RC_UINT maxTextWidth = 0;
	RC_UINT maxTextHeight = 0;
	for (auto letter : text)
	{
		RC_SIZE letterSize = _CalcLetterSize(letter, letterArt);

		maxTextWidth += letterSize.cx * textScale;
		if (letter != ' ') maxTextWidth += letterIndent;

		maxTextHeight = max(maxTextHeight, letterSize.cy);
	}

	return { maxTextWidth, maxTextHeight };
}
#endif // USE_SCALED_TEXT()

/*
	Non-scaled text.
*/

void RenderTextLibrary::FillTextureWithText(RCTexture* texture, std::string text, TEX_COLOR_RGB textColor, TEX_COORD startCoord, TEX_PIXEL fillerPixel)
{
	_FillTextureWithScaledText(texture, text, textColor, 1, startCoord, letterArt, fillerPixel);
}

void RenderTextLibrary::FillTextureWithTextBig(RCTexture* texture, std::string text, TEX_COLOR_RGB textColor, TEX_COORD startCoord, TEX_PIXEL fillerPixel)
{
	_FillTextureWithScaledText(texture, text, textColor, 1, startCoord, letterBigArt, fillerPixel);
}

static RC_SIZE _GetTextSize(std::string text, uint8_t fontSize, const LetterMap& letterMap)
{
	RC_UINT maxTextWidth = 0;
	RC_UINT maxTextHeight = 0;
	for (auto letter : text)
	{
		RC_SIZE letterSize = _CalcLetterSize(letter, letterMap);
		letterSize.cx += fontSize; letterSize.cy += fontSize;

		maxTextWidth += letterSize.cx;
		if (letter != ' ') maxTextWidth += letterIndent;

		maxTextHeight = max(maxTextHeight, letterSize.cy);
	}

	return { maxTextWidth, maxTextHeight };
}

RC_SIZE RenderTextLibrary::GetTextSize(std::string text, uint8_t fontSize)
{
	return _GetTextSize(text, fontSize, letterArt);
}

RC_SIZE RenderTextLibrary::GetTextBigSize(std::string text, uint8_t fontSize)
{
	return _GetTextSize(text, fontSize, letterBigArt);
}
