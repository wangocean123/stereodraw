#include "stdafx.h"
#include "text_style.h"

namespace StereoDraw
{

	c_text_style::c_text_style()
	{
		m_height = 2.0f;
		m_width_scale = 1.0f;
		m_weight = 0;
		m_bunderline = false;
		m_bstrikeOut = false;		

		m_align_type = (align_left | align_mid_y);
		m_text_angle = 0;
		m_incline_angle = 0;
		m_char_sapce_scale = 1.0f;
		m_line_space_scale = 1.0f;
	}

	c_text_style::c_text_style(const c_text_style& a)
	{
		*this = a;
	}
	
	c_text_style::~c_text_style()
	{
	}

	bool c_text_style::is_normal()
	{
		return (m_width_scale == 1.0f && m_text_angle == 0.0f && m_incline_angle == 0.0f && m_char_sapce_scale == 1.0f && m_line_space_scale == 1.0f);
	}

	c_text_style& c_text_style::operator = (const c_text_style& a)
	{
		m_height = a.m_height;
		m_width_scale = a.m_width_scale;
		m_weight = a.m_weight;
		m_bunderline = a.m_bunderline;
		m_bstrikeOut = a.m_bstrikeOut;

		m_align_type = a.m_align_type;
		m_text_angle = a.m_text_angle;
		m_incline_angle = a.m_incline_angle;
		m_char_sapce_scale = a.m_char_sapce_scale;
		m_line_space_scale = a.m_line_space_scale;

		return *this;
	}
}