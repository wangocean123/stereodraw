#include "stdafx.h"
#include "geo_algorithom.h"

namespace geo_api
{

	double g_toler_xy = 0.0001;
	double g_toler_z = 0.0001;

	double get_toler_xy()
	{
		return g_toler_xy;
	}

	void set_toler_xy(double toler)
	{
		g_toler_xy = toler;
	}

	double get_toler_z()
	{
		return g_toler_z;
	}

	void set_toler_z(double toler)
	{
		g_toler_z = toler;
	}

	gd::Rect3D transform_rect3d(const gd::Rect3D& rect, const gd::matrix4d& m)
	{
		gd::Point3D corners[8];
		gd::get_rect_corners_3d(rect, corners);

		gd::transform_pts_by_matrix_3d(corners, 8, m);
		return gd::create_rect3d_from_points(corners, 8);
	}
}