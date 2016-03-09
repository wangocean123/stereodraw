#pragma once
#include "GoodDraw.h"
#include "textstyle.h"
#include "rect_def.h"

namespace Gdiplus
{
	class Graphics;
	class GraphicsPath;
	class Font;
}

namespace gd
{
	class FontHelper
	{
	public:
		FontHelper();
		virtual ~FontHelper();

		void init();
		void cleanup();		
		Gdiplus::GraphicsPath *get_outline(const char* text, const TextStyle& style);
		Rect2D getBound(const char* text, const TextStyle& style);

	protected:
		void set_font(Gdiplus::Font *font);

		Gdiplus::Graphics *m_pg;
		Gdiplus::Font *m_font;
		HDC m_hdc;
	};

}