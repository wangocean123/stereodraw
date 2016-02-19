#pragma once

#include "StereoDraw.h"

namespace StereoDraw
{

	class STEREODRAW_API c_text_style
	{
	public:
		static const long align_left = 1;
		static const long align_mid_x = 2;
		static const long align_right = 4;
		static const long align_top = 8;
		static const long align_mid_y = 0x10;
		static const long align_bottom = 0x20;
	public:
		c_text_style();
		c_text_style(const c_text_style& a);
		~c_text_style();

		c_text_style& operator = (const c_text_style& a);
		bool is_normal();

		float m_height;
		float m_width_scale;
		long  m_weight;
		bool m_bunderline;
		bool m_bstrikeOut;
		std::string m_font_name;

		float m_text_angle;
		long  m_align_type;
		float m_incline_angle;
		float m_char_sapce_scale;
		float m_line_space_scale;
	};

}