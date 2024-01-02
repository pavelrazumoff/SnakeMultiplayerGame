#pragma once

#include "BrushPrimitive.h"
#include "../Utility/FontStyle.h"

#include <string>

class TextPrimitive : public BrushPrimitive
{
	typedef BrushPrimitive Inherited;

public:
	TextPrimitive();
	virtual ~TextPrimitive();

	virtual void Construct() override;

public:
	void SetFontStyle(const FontStyle& newFont) { Font = newFont; }
	const FontStyle& GetFontStyle() const { return Font; }

	void SetText(const std::string& newText) { Text = newText; }
	const std::string& GetText() const { return Text; }

	RC_SIZE GetTextDrawSize() const;

protected:
	std::string Text;

	FontStyle Font;
};
