#include "pch.h"
#include "RenderTextLibrary.h"

#include "RenderTextureLibrary.h"
#include <map>

static const uint8_t letterWidth = 3;
static const uint8_t letterMaxHeight = 6;
static const uint8_t letterIndent = 1;

const std::map<char, std::string> letterArt = {
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
};

void RenderTextLibrary::FillTextureWithScaledText(RCTexture* texture, std::string text, TEX_COLOR_RGB textColor, uint8_t textScale, TEX_COORD startCoord)
{
	/*
		TODO: Text Alignment.
	*/

	const TEX_SIZE texSize = { texture->GetWidth(), texture->GetHeight() };
	const RC_UINT texLen = texture->GetWidth() * texture->GetHeight();

	TEX_PIXEL* texData = new TEX_PIXEL[texLen];
	memset(texData, RenderConstants::ClearPixel, texLen * sizeof(TEX_PIXEL));
	//memset(texData, RenderConstants::AlphaZeroPixel, texLen * sizeof(TEX_PIXEL));

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

		drawCoord.x = startCoord.x + ti * letterWidth * textScale + ti * letterIndent * textScale;
		drawCoord.y = startCoord.y;

		if (drawCoord.x >= texSize.cx) break;

		RC_UINT letterStartX = drawCoord.x;

		if (ch == ' ')
		{
			// Handle spaces
			if (!FILL_TEXDATA_WITH_LETTER(' ', textScale))
				break;
		}
		else if (letterArt.find(ch) != letterArt.end())
		{
			// Print the scaled letter
			std::string letter = letterArt.at(ch);

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
					if (!FILL_TEXDATA_WITH_LETTER(letterCh, textScale)) continue;
				}
			}
		}
		else
		{
			// Handle unknown characters
			if (!FILL_TEXDATA_WITH_LETTER('?', letterWidth * textScale))
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
	RC_SIZE textSize;
	textSize.cx = text.length() * letterWidth * textScale + (text.length() - 1) * letterIndent * textScale;
	textSize.cy = letterMaxHeight;

	return textSize;
}
