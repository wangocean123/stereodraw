#include "StdAfx.h"
#include "BitmapFunc.h"
#include "rect_def.h"

#define GetAValue(rgb)      (LOBYTE((rgb)>>24))

namespace bmp_api
{
	int align_4bytes(int size)
	{
		return ((size + 3)&(~3));
	}

	ulong shift_RGBF(ulong color)
	{
		return ((color & 0xff000000) | RGB(GetBValue(color), GetGValue(color), GetRValue(color)));
	}

	ulong shift_RGB(ulong color)
	{
		return (RGB(GetBValue(color), GetGValue(color), GetRValue(color)));
	}

	HBITMAP create_bmp_24(int wid, int hei)
	{
		BITMAPINFO bmpInfo;
		memset(&bmpInfo, 0, sizeof(bmpInfo));
		bmpInfo.bmiHeader.biSize = sizeof(BITMAPINFOHEADER);
		bmpInfo.bmiHeader.biWidth = wid;
		bmpInfo.bmiHeader.biHeight = hei;
		bmpInfo.bmiHeader.biPlanes = 1;
		bmpInfo.bmiHeader.biBitCount = 24;
		bmpInfo.bmiHeader.biCompression = BI_RGB;

		void *pBits = NULL;
		HBITMAP hbmp = CreateDIBSection(NULL, &bmpInfo, DIB_RGB_COLORS, &pBits, NULL, 0);
		return hbmp;
	}

	HBITMAP create_bmp_32(int wid, int hei)
	{
		BITMAPINFO bmpInfo;
		memset(&bmpInfo, 0, sizeof(bmpInfo));
		bmpInfo.bmiHeader.biSize = sizeof(BITMAPINFOHEADER);
		bmpInfo.bmiHeader.biWidth = wid;
		bmpInfo.bmiHeader.biHeight = hei;
		bmpInfo.bmiHeader.biPlanes = 1;
		bmpInfo.bmiHeader.biBitCount = 32;
		bmpInfo.bmiHeader.biCompression = BI_RGB;

		void *pBits = NULL;
		HBITMAP hbmp = CreateDIBSection(NULL, &bmpInfo, DIB_RGB_COLORS, &pBits, NULL, 0);
		return hbmp;
	}

	bool erase_bmp(HBITMAP hbmp, ulong color)
	{
		BITMAP info;
		if (!GetObject(hbmp, sizeof(info), &info))
			return false;

		int pixel_size = info.bmBitsPixel / 8;
		if (pixel_size != 3 && pixel_size != 4)
			return false;

		if (info.bmBits == NULL)
			return false;

		color = shift_RGB(color);

		byte *pbits = (byte*)info.bmBits;
		int linesize = info.bmWidthBytes;
		for (int i = 0; i < info.bmHeight; i++)
		{
			byte *p = pbits + i*linesize;
			for (int j = 0; j < info.bmWidth; j++, p += pixel_size)
			{
				if (info.bmBitsPixel == 32)
				{
					*(ulong*)p = color;
				}
				else if (info.bmBitsPixel == 24)
				{
					p[0] = GetRValue(color);
					p[1] = GetGValue(color);
					p[2] = GetBValue(color);
				}
			}
		}
		return true;
	}

	HBITMAP copy_bmp(HBITMAP hbmp)
	{
		BITMAP info;
		if (!GetObject(hbmp, sizeof(info), &info))
			return NULL;

		int pixel_size = info.bmBitsPixel / 8;
		if (pixel_size != 3 && pixel_size != 4)
			return NULL;

		HBITMAP hbmp_new = NULL;
		if (pixel_size == 3)
			hbmp_new = create_bmp_24(info.bmWidth,info.bmHeight);
		else
			hbmp_new = create_bmp_32(info.bmWidth, info.bmHeight);

		BITMAP info_new;
		if (!GetObject(hbmp_new, sizeof(info_new), &info_new))
			return NULL;

		memcpy(info_new.bmBits, info.bmBits, info.bmWidthBytes*info.bmHeight);
		return hbmp_new;
	}

	bool copy_bmp(HBITMAP hbmp0, HBITMAP hbmp1)
	{
		BITMAP info;
		if (!GetObject(hbmp0, sizeof(info), &info))
			return false;

		BITMAP info_new;
		if (!GetObject(hbmp1, sizeof(info_new), &info_new))
			return false;

		if (info.bmBits == NULL || info_new.bmBits == NULL)
			return false;

		if (info.bmWidth != info_new.bmWidth || info.bmHeight != info_new.bmHeight)
			return false;

		if ((info.bmBitsPixel != 24 && info.bmBitsPixel != 32) || (info_new.bmBitsPixel != 24 && info_new.bmBitsPixel != 32))
			return false;

		if (info.bmBitsPixel == info_new.bmBitsPixel)
			memcpy(info_new.bmBits, info.bmBits, info.bmWidthBytes*info.bmHeight);
		else
		{
			int pixel_size1 = info.bmBitsPixel / 8;
			int pixel_size2 = info_new.bmBitsPixel / 8;
			byte *pline2 = (byte*)info_new.bmBits;
			byte *pline1 = (byte*)info.bmBits;
			for (int i = 0; i < info.bmHeight; i++)
			{
				byte *p2 = pline2;
				byte *p1 = pline1;
				for (int j = 0; j < info.bmWidth; j++)
				{
					p2[0] = p1[0];
					p2[1] = p1[1];
					p2[2] = p1[2];
					p2 += pixel_size2;
					p1 += pixel_size1;
				}

				pline2 += info_new.bmWidthBytes;
				pline1 += info.bmWidthBytes;
			}
		}
		return true;
	}

	void set_bmp_alpha(HBITMAP hbmp, byte alpha)
	{
		BITMAP info;
		if (!GetObject(hbmp, sizeof(info), &info))
			return;

		if (info.bmBitsPixel != 32)
			return;

		byte *pbits = (byte*)info.bmBits;
		int linesize = info.bmWidthBytes;
		for (int i = 0; i < info.bmHeight; i++)
		{
			byte *p = pbits + 3;
			for (int j = 0; j < info.bmWidth; j++, p += 4)
			{
				p[0] = alpha;
			}
			pbits += linesize;
		}
	}

	std::vector<HDC> memdc::m_dcs;
	bool memdc::m_default_used = false;

	HDC memdc::get()
	{
		if (m_dcs.size()==0)
		{
			HDC hdc = CreateCompatibleDC(NULL);
			m_dcs.push_back(hdc);
			m_default_used = true;
			return hdc;
		}
		else if (m_dcs.size() > 1 || (m_dcs.size() == 1 && m_default_used) )
		{
			HDC hdc = CreateCompatibleDC(NULL);
			m_dcs.push_back(hdc);
			return hdc;
		}
		else
		{
			m_default_used = true;
			return m_dcs[0];
		}
	}

	void memdc::release()
	{
		int size = m_dcs.size();
		if (m_default_used)
		{
			if (size == 1)
			{
				m_default_used = false;
			}
			else if (size > 1)
			{
				m_dcs.erase(m_dcs.end() - 1);
			}
		}
	}

	HDC memdc::get_single()
	{
		return CreateCompatibleDC(NULL);
	}

	void memdc::release_single(HDC hdc)
	{
		if (hdc != NULL)
		{
			DeleteDC(hdc);
		}
	}

	void memdc::cleanup()
	{
		for (int i = 0; i < m_dcs.size(); i++)
		{
			::DeleteDC(m_dcs[i]);
		}
		m_dcs.clear();
	}

	change_bmpofdc::change_bmpofdc()
	{
		m_hdc = NULL;
		m_hbmp_old = NULL;
	}

	change_bmpofdc::~change_bmpofdc()
	{
		if (m_hbmp_old)
		{
			::SelectObject(m_hdc, m_hbmp_old);
		}
	}

	change_bmpofdc::change_bmpofdc(HDC hdc, HBITMAP hbmp)
	{		
		if (hdc != NULL && hbmp != NULL)
		{
			m_hbmp_old = (HBITMAP)::SelectObject(hdc, hbmp);
			m_hdc = hdc;
		}
	}

	void change_bmpofdc::change(HDC hdc, HBITMAP hbmp)
	{
		if (hdc != NULL && hbmp != NULL)
		{
			m_hbmp_old = (HBITMAP)::SelectObject(hdc, hbmp);
			m_hdc = hdc;
		}
	}

	void change_bmpofdc::restore()
	{
		if (m_hbmp_old)
		{
			::SelectObject(m_hdc, m_hbmp_old);
		}
	}


	void write_bmp_tofile(HBITMAP hbmp)
	{
		DIBSECTION dibInfo;
		::GetObject(hbmp, sizeof(dibInfo), &dibInfo);
		BITMAPFILEHEADER fileInfo;
		BITMAPINFO		 bmpInfo;

		fileInfo.bfType = 0x4d42;
		fileInfo.bfSize = sizeof(fileInfo)+sizeof(bmpInfo)+dibInfo.dsBmih.biSizeImage;
		fileInfo.bfReserved1 = 0;
		fileInfo.bfReserved2 = 0;
		fileInfo.bfOffBits = sizeof(fileInfo)+sizeof(bmpInfo);

		memset(&bmpInfo, 0, sizeof(bmpInfo));
		memcpy(&bmpInfo.bmiHeader, &dibInfo.dsBmih, sizeof(dibInfo.dsBmih));

		char fileName[256];
		static int num = 0;
		sprintf(fileName, "e:\\test%d.bmp", num++);
		FILE *fp = fopen(fileName, "wb");
		if (!fp)return;
		fwrite(&fileInfo, sizeof(fileInfo), 1, fp);
		fwrite(&bmpInfo, sizeof(bmpInfo), 1, fp);
		fwrite(dibInfo.dsBm.bmBits, 1, dibInfo.dsBmih.biSizeImage, fp);
		fclose(fp);
	}



}