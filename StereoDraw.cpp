// GoodDraw.cpp : Defines the exported functions for the DLL application.
//

#include "stdafx.h"
#include "StereoDraw.h"
#include <atlbase.h>
#include <atlcom.h>
#include <GdiPlus.h>
#include "BitmapFunc.h"
#include "..\gdal\gdalDatasetEx.h"
#include "c_drawing_base.h"
#include "geo_algorithom.h"
#include "c_drawer_gl.h"
#include "c_drawer_d3d.h"


extern void GDALAllRegisterWrapper();
extern void GDALAllUnregisterWrapper();


namespace StereoDraw
{
	Gdiplus::GdiplusStartupInput gdiplus_si;
	ULONG_PTR gdiplus_t = 0;

	void STEREODRAW_API dll_init()
	{		
		Gdiplus::GdiplusStartup(&gdiplus_t, &gdiplus_si, NULL);
		GDALAllRegisterWrapper();
		c_drawing_base::init();
	}


	void STEREODRAW_API dll_cleanup()
	{
		GDALAllUnregisterWrapper();
		bmp_api::memdc::cleanup();
		c_drawing_base::cleanup();
		c_drawer_gl::clear_tess_obj();
		c_drawer_d3d::clear_tess_obj();
		Gdiplus::GdiplusShutdown(gdiplus_t);
	}


}