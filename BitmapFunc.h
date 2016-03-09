#pragma once
#include "GoodDrawBaseType.h"
#include <vector>

namespace bmp_api
{
	int align_4bytes(int size);
	HBITMAP create_bmp_24(int wid, int hei);
	HBITMAP create_bmp_32(int wid, int hei);
	bool erase_bmp(HBITMAP hbmp, ulong color);
	HBITMAP copy_bmp(HBITMAP hbmp);
	bool copy_bmp(HBITMAP hbmp0, HBITMAP hbmp1);
	void set_bmp_alpha(HBITMAP hbmp, byte alpha);
	void write_bmp_tofile(HBITMAP hbmp);
	ulong shift_RGBF(ulong color);
	ulong shift_RGB(ulong color);

	class memdc
	{
	public:
		static HDC get();
		static void release();
		static void cleanup();

		static HDC get_single();
		static void release_single(HDC hdc);

		static std::vector<HDC> m_dcs;

		//m_dcs中的第一个对象为缺省对象，如果缺省对象已经存在，且未被使用，就用缺省对象
		static bool m_default_used;
	};

	class change_bmpofdc
	{
	public:
		change_bmpofdc();
		~change_bmpofdc();
		change_bmpofdc(HDC hdc, HBITMAP hbmp);
		void change(HDC hdc, HBITMAP hbmp);
		void restore();

		HDC m_hdc;
		HBITMAP m_hbmp_old;
	};
}