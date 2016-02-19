#include "StdAfx.h"
#include "c_texture_gl.h"
#include "BitmapFunc.h"


namespace StereoDraw
{

	c_texture_gl::c_texture_gl() :c_texture_obj(type_opengl)
	{
		m_ntexture = 0;
		m_pdrawer = NULL;
	}


	c_texture_gl::~c_texture_gl()
	{
	}

	void c_texture_gl::set_device(HGLRC hglrc)
	{
		m_hglrc = hglrc;
	}

	void c_texture_gl::set_device(c_drawer_device_gl *p)
	{
		m_pdrawer = p;
		m_hglrc = p->m_hglrc;
	}

	bool c_texture_gl::create(int wid, int hei)
	{
		glGenTextures(1, &m_ntexture);
		if (m_ntexture <= 0)
			return false;

		m_nwid = wid; 
		m_nhei = hei;
		glPixelStorei(GL_UNPACK_ROW_LENGTH, wid);
		byte *pBits = new byte[wid*hei * 4];
		memset(pBits, 0, wid*hei * 4);
		glBindTexture(GL_TEXTURE_2D, m_ntexture);
		glTexImage2D(GL_TEXTURE_2D, 0, 4, wid, hei, 0, GL_BGRA_EXT, GL_UNSIGNED_BYTE, pBits);

		GLenum err = glGetError();
		delete[] pBits;

		return true;
	}

	bool c_texture_gl::write_bitmap(HBITMAP hbmp)
	{
		if (hbmp == NULL) return false;
		if (m_ntexture <= 0)
			return false;

		BITMAP bmp;
		::GetObject(hbmp, sizeof(BITMAP), &bmp);
		BYTE* pBits = (BYTE*)bmp.bmBits;
		if (pBits == NULL)return false;

		int linesize = bmp.bmWidth*bmp.bmBitsPixel / 8;
		linesize = bmp_api::align_4bytes(linesize);
		pBits = pBits + (bmp.bmHeight - m_nhei)*linesize;

		byte *pNew = new byte[linesize*bmp.bmHeight];
		BYTE *p1 = pNew, *p2 = pBits +(bmp.bmHeight - 1)*linesize;
		for (int i = 0; i<bmp.bmHeight; i++)
		{
			memcpy(p1, p2, linesize);
			p1 += linesize;
			p2 -= linesize;
		}

		glPixelStorei(GL_UNPACK_ROW_LENGTH, bmp.bmWidth);
		glPixelStorei(GL_UNPACK_SKIP_PIXELS, 0);
		glPixelStorei(GL_UNPACK_SKIP_ROWS, 0);

		HGLRC htest = wglGetCurrentContext();

		glBindTexture(GL_TEXTURE_2D, m_ntexture);
		GLenum err = glGetError();
		glTexSubImage2D(GL_TEXTURE_2D, 0, 0, 0, m_nwid, m_nhei, GL_BGRA_EXT, GL_UNSIGNED_BYTE, pNew);
		delete[] pNew;
		err = glGetError();
		return true;
	}

	void c_texture_gl::del()
	{
		glDeleteTextures(1, &m_ntexture);
		m_ntexture = 0;
	}

	void c_texture_gl::draw(const quad_points& qpt)const
	{
		glBindTexture(GL_TEXTURE_2D, m_ntexture);
		glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, (float)GL_CLAMP_TO_EDGE);
		glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, (float)GL_CLAMP_TO_EDGE);

		if (m_blinear_zoom)
		{
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		}
		else
		{
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
		}

		glBegin(GL_QUADS);
		glColor4f(1.0, 1.0, 1.0,1.0);
		glTexCoord2d(0.0, 0.0); glVertex3d(qpt.pts[0].x, qpt.pts[0].y, 0);
		glTexCoord2d(1.0, 0.0); glVertex3d(qpt.pts[1].x, qpt.pts[1].y, 0);
		glTexCoord2d(1.0, 1.0); glVertex3d(qpt.pts[2].x, qpt.pts[2].y, 0);
		glTexCoord2d(0.0, 1.0); glVertex3d(qpt.pts[3].x, qpt.pts[3].y, 0);

		glEnd();
	}

	bool c_texture_gl::read_bitmap(HBITMAP hbmp)
	{
		if (hbmp == NULL) return false;
		if (m_ntexture <= 0)
			return false;

		BITMAP bmp;
		::GetObject(hbmp, sizeof(BITMAP), &bmp);
		BYTE* pBits = (BYTE*)bmp.bmBits;
		if (pBits == NULL)return false;

		int linesize = bmp.bmWidth*bmp.bmBitsPixel / 8;
		linesize = bmp_api::align_4bytes(linesize);
		pBits = pBits + (bmp.bmHeight - m_nhei)*linesize;

		byte *pNew = new byte[linesize*bmp.bmHeight];

		glPixelStorei(GL_UNPACK_ROW_LENGTH, bmp.bmWidth);
		glPixelStorei(GL_UNPACK_SKIP_PIXELS, 0);
		glPixelStorei(GL_UNPACK_SKIP_ROWS, 0);

		glBindTexture(GL_TEXTURE_2D, m_ntexture);
		glGetTexImage(GL_TEXTURE_2D, 0, GL_BGRA_EXT, GL_UNSIGNED_BYTE, pNew);

		byte *p2 = pNew, *p1 = pBits + (bmp.bmHeight - 1)*linesize;
		for (int i = 0; i<bmp.bmHeight; i++)
		{
			memcpy(p1, p2, linesize);
			p1 += linesize;
			p2 -= linesize;
		}
		delete[] pNew;

		return true;
	}

}