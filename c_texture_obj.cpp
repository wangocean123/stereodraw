#include "stdafx.h"
#include "c_texture_obj.h"

namespace StereoDraw
{

	c_texture_obj::c_texture_obj(int type)
	{
		m_type = type;
		m_nwid = 0;
		m_nhei = 0;
		m_blinear_zoom = true;
	}


	c_texture_obj::~c_texture_obj()
	{
	}

	bool c_texture_obj::write_bitmap(HBITMAP hbmp)
	{
		return false;
	}

	bool c_texture_obj::create(int wid, int hei)
	{
		return false;
	}

	void c_texture_obj::del()
	{
	}

	int  c_texture_obj::get_type()
	{
		return m_type;
	}

	void c_texture_obj::draw(const quad_points& pts)const
	{

	}

	bool c_texture_obj::read_bitmap(HBITMAP hbmp)
	{
		return false;
	}

}
