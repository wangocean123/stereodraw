#include "StdAfx.h"
#include "FontHelper.h"
#include "BitmapFunc.h"
#include "GoodDrawHelperAPI.h"
#include "rect_def.h"
#include "geo_algorithom.h"
#include <atlbase.h>
#include <atlcom.h>
#include <GdiPlus.h>

using namespace Gdiplus;

namespace gd
{
	FontHelper::FontHelper()
	{
		m_pg = NULL;
		m_hdc = NULL;
		m_font = NULL;
	}


	FontHelper::~FontHelper()
	{
	}

	void FontHelper::init()
	{		
		if (m_hdc == NULL)
		{
			m_hdc = bmp_api::memdc::get_single();
		}
		if (m_pg == NULL)
		{
			m_pg = new Graphics(m_hdc);
		}
	}

	void FontHelper::cleanup()
	{
		if (m_pg)
			delete m_pg;
		m_pg = NULL;

		if (m_font)
			delete m_font;
		m_font = NULL;
	}

	void FontHelper::set_font(Font *font)
	{
		if (m_font)
			delete m_font;

		m_font = font->Clone();
	}

	GraphicsPath *FontHelper::get_outline(const char* text, const TextStyle& style)
	{
		init();
		LOGFONT logfont;
		memset(&logfont, 0, sizeof(logfont));
		logfont.lfHeight = style.m_height;
		wcscpy_s(logfont.lfFaceName, convert_string(style.m_font_name.c_str()));
		logfont.lfWeight = style.m_weight;
		logfont.lfUnderline = style.m_bunderline ? 1 : 0;
		logfont.lfStrikeOut = style.m_bstrikeOut ? 1 : 0;
		logfont.lfCharSet = GB2312_CHARSET;
		Font *font = new Font(m_hdc, &logfont);

		if (!font->IsAvailable())
		{
			logfont.lfCharSet = GB2312_CHARSET;
			font = new Font(m_hdc, &logfont);
		}

		FontFamily fontFamily;
		font->GetFamily(&fontFamily);
		StringFormat strformat;

		GraphicsPath path;
		float linespace = style.m_height*fontFamily.GetLineSpacing(font->GetStyle()) / fontFamily.GetEmHeight(FontStyleRegular)*style.m_line_space_scale;

		std::vector<Gdiplus::Rect> sizes;

		std::wstring textw = convert_string(text);
		int nchar = textw.size();
		int pos = 0;
		int nline = 0;
		for (int i = 0; i < nchar; i++)
		{
			if (textw[i] == '\n' || i==(nchar-1))
			{
				if (textw[i] != '\n')
				{
					i++;
				}
				if (i != pos)
				{
					std::wstring textw2;
					textw2.assign(textw.begin() + pos, textw.begin() + i);

					GraphicsPath path2;
					path2.AddString(textw2.data(), textw2.size(), &fontFamily,
						Gdiplus::FontStyleRegular, style.m_height, Gdiplus::Point(0, 0), &strformat);

					Gdiplus::Rect rcText;
					path2.GetBounds(&rcText);
					sizes.push_back(rcText);

					if (style.m_incline_angle != 0.0f)
					{
						double k = tan(style.m_incline_angle*geo_api::PI / 180);
						Matrix matrix;
						matrix.Shear(-k, 0.0);
						path2.Transform(&matrix);
					}

					if (style.m_align_type&TextStyle::align_right)
					{
						Matrix matrix;
						matrix.Translate(-(rcText.X + rcText.Width + rcText.X), nline == 0 ? 0 : -linespace);
						path2.Transform(&matrix);
					}
					else if (style.m_align_type&TextStyle::align_mid_x)
					{
						Matrix matrix;
						matrix.Translate(-(rcText.Width + rcText.X)*0.5, nline == 0 ? 0 : -linespace);
						path2.Transform(&matrix);
					}
					else
					{
						Matrix matrix;
						matrix.Translate(0, nline == 0 ? 0 : -linespace);
						path2.Transform(&matrix);
					}

					path.AddPath(&path2, FALSE);

					pos = i + 1;
					nline++;
				}
			}
		}

		if (style.m_align_type&TextStyle::align_top)
		{
			Matrix matrix;
			matrix.Translate(0, -linespace);
			path.Transform(&matrix);
		}
		else if (style.m_align_type&TextStyle::align_bottom)
		{
			Matrix matrix;
			matrix.Translate(0, linespace*(nline-1));
			path.Transform(&matrix);
		}
		else if (style.m_align_type&TextStyle::align_mid_y)
		{
			Matrix matrix;
			matrix.Translate(0, linespace*nline*0.5-linespace);
			path.Transform(&matrix);
		}

		if (style.m_text_angle != 0.0f)
		{
			Matrix matrix;
			matrix.Rotate(style.m_text_angle);
			path.Transform(&matrix);
		}

		delete font;

		return path.Clone();
	}

	Rect2D FontHelper::getBound(const char* text, const TextStyle& style)
	{
		GraphicsPath *path = get_outline(text,style);
		RectF rcBound;
		path->GetBounds(&rcBound);
		return Rect2D(rcBound.X, rcBound.Y, rcBound.X + rcBound.Width, rcBound.Y + rcBound.Height);
	}
}