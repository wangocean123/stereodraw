#pragma once
#include "c_texture_obj.h"
#include "c_drawer_gdi.h"


namespace StereoDraw
{
	class STEREODRAW_API c_texture_gdi : public c_texture_obj
	{
	public:
		c_texture_gdi();
		virtual ~c_texture_gdi();

		void set_device(c_drawer_device_gdi *p);
		virtual bool create(int wid, int hei);
		virtual bool write_bitmap(HBITMAP hbmp);
		virtual void del();
		virtual void draw(const quad_points& qpt)const;
		virtual bool read_bitmap(HBITMAP hbmp);

		c_drawer_device_gdi *m_pdevice;
		HBITMAP m_hbmp;
		bool m_blinear_zoom;
	};

}

