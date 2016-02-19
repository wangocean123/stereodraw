#pragma once

#include "point_def.h"

namespace StereoDraw
{

	class STEREODRAW_API c_texture_obj
	{
	public:
		enum e_type
		{
			type_opengl = 1,
			type_d3d = 2,
			type_gdi = 3
		};
	public:
		c_texture_obj(int type);
		virtual ~c_texture_obj();

		virtual int  get_type();
		virtual bool create(int wid, int hei);
		virtual bool write_bitmap(HBITMAP hbmp);
		virtual void del();
		virtual void draw(const quad_points& pts)const;
		virtual bool read_bitmap(HBITMAP hbmp);

	public:
		int m_type;
		int m_nwid, m_nhei;
		bool m_blinear_zoom;
	};

}

