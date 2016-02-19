#include "StdAfx.h"
#include "c_texture_gdi.h"
#include "BitmapFunc.h"
#include "rect_def.h"
#include "point_def.h"
#include "geo_algorithom.h"
#include "c_drawer.h"

namespace StereoDraw
{

	c_texture_gdi::c_texture_gdi() :c_texture_obj(type_opengl)
	{
		m_hbmp = 0;
		m_pdevice = NULL;
	}

	c_texture_gdi::~c_texture_gdi()
	{
	}

	void c_texture_gdi::set_device(c_drawer_device_gdi *p)
	{
		m_pdevice = p;
	}

	bool c_texture_gdi::create(int wid, int hei)
	{
		m_nwid = wid;
		m_nhei = hei;
		m_hbmp = bmp_api::create_bmp_32(wid, hei);
		bmp_api::erase_bmp(m_hbmp, 0);
		return (m_hbmp!=NULL);
	}
	
	void c_texture_gdi::del()
	{
		if (m_hbmp)
			::DeleteObject(m_hbmp);
		m_hbmp = NULL;
	}

	void c_texture_gdi::draw(const quad_points& qpt)const
	{
		if (m_pdevice && m_pdevice->m_pGraphics)
		{
			Gdiplus::Bitmap *pBmp = new Gdiplus::Bitmap(m_nwid, m_nhei, PixelFormat32bppARGB);

			BITMAP bm;
			GetObject(m_hbmp, sizeof(bm), &bm);

			Gdiplus::BitmapData datas;
			Gdiplus::Rect rc(0, 0, m_nwid, m_nhei);
			pBmp->LockBits(&rc, Gdiplus::ImageLockMode::ImageLockModeWrite, PixelFormat32bppARGB, &datas);

			byte* dest = (byte*)(datas.Scan0);
			for (int y = 0; y < m_nhei; y++)
			{
				memcpy(dest + (y * bm.bmWidthBytes), (byte*)bm.bmBits + ((bm.bmHeight - y - 1) * bm.bmWidthBytes), bm.bmWidthBytes);
			}

			pBmp->UnlockBits(&datas);

			Gdiplus::PointF pts[3];
			pts[0].X = qpt.pts[0].x;
			pts[0].Y = qpt.pts[0].y;
			pts[1].X = qpt.pts[1].x;
			pts[1].Y = qpt.pts[1].y;
			pts[2].X = qpt.pts[3].x;
			pts[2].Y = qpt.pts[3].y;
			//for (int i = 0; i < 4; i++)
			//{
			//	pts[i].X = qpt.pts[i].x;
			//	pts[i].Y = qpt.pts[i].y;
			//}
			m_pdevice->m_pGraphics->DrawImage(pBmp, pts, 3);
			//m_pdevice->m_pGraphics->DrawImage(pBmp, (int)qpt.pts[0].x, (int)qpt.pts[0].y);
			delete pBmp;
			return;
		}

		HDC hdc = bmp_api::memdc::get();
		HBITMAP hbmp_old = (HBITMAP)SelectObject(hdc, m_hbmp);

		BLENDFUNCTION bf;
		bf.BlendOp = AC_SRC_OVER;
		bf.BlendFlags = 0;
		bf.AlphaFormat = AC_SRC_ALPHA;
		bf.SourceConstantAlpha = 0xff;

		rect_2d rc = create_rect2d_from_points(qpt.pts, 4);
		rect_2i rc2;
		copy_rect_2(rc, rc2);

		::AlphaBlend(m_pdevice->m_hdc, rc2.x0, rc2.y0, rc2.width(), rc2.height(), hdc, rc2.x0, rc2.y0, rc2.width(), rc2.height(), bf);
	}

	bool c_texture_gdi::write_bitmap(HBITMAP hbmp)
	{
		if (hbmp == NULL) return false;
		return bmp_api::copy_bmp(hbmp, m_hbmp);
	}

	bool c_texture_gdi::read_bitmap(HBITMAP hbmp)
	{
		if (hbmp == NULL) return false;

		return bmp_api::copy_bmp(m_hbmp, hbmp);
	}

}