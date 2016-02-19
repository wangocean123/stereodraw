#pragma once
#include "c_texture_obj.h"
#include "..\GLext\glew.h"
#include <gl\GL.h>
#include "c_drawer_gl.h"

namespace StereoDraw
{
	class c_drawer_device_gl;
	class STEREODRAW_API c_texture_gl : public c_texture_obj
	{
	public:
		c_texture_gl();
		virtual ~c_texture_gl();

		void set_device(HGLRC hglrc);
		void set_device(c_drawer_device_gl *p);
		virtual bool create(int wid, int hei);
		virtual bool write_bitmap(HBITMAP hbmp);
		virtual void del();
		virtual void draw(const quad_points& qpt)const;
		virtual bool read_bitmap(HBITMAP hbmp);

		c_drawer_device_gl *m_pdrawer;
		HGLRC m_hglrc;
		GLuint m_ntexture;
	};

}

