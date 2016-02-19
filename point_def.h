#pragma once

#include "StereoDraw.h"
#include <vector>

namespace StereoDraw
{

template<class type>
struct c_point_2
{
	c_point_2()
	{
		x = y = 0;
	}
	c_point_2(type x1, type y1)
	{
		x = x1;
		y = y1;
	}
	bool operator == (const c_point_2<type>& pt)
	{
		return (x == pt.x && y == pt.y);
	}
	type x, y;
};


template<class type>
struct c_point_3 : public c_point_2<type>
{
	c_point_3()
	{
		x = y = z = 0;
	}
	c_point_3(type x1, type y1, type z1)
	{
		x = x1;
		y = y1;
		z = z1;
	}
	bool operator == (const c_point_3<type>& pt)
	{
		return (x == pt.x && y == pt.y && z==pt.z);
	}
	type z;
};


template<class type>
struct c_point_stereo
{
	c_point_stereo()
	{
		xl = yl = xr = yr = 0;
	}
	c_point_stereo(const c_point_stereo& pt)
	{
		xl = pt.xl;
		yl = pt.yl;
		xr = pt.xr;
		yr = pt.yr;
	}
	c_point_stereo(const c_point_3<type>& pt)
	{
		xl = pt.x;
		yl = pt.y;
		xr = pt.z;
		yr = pt.y;
	}
	c_point_stereo& operator=(const c_point_3<type>& pt)
	{
		xl = pt.x;
		yl = pt.y;
		xr = pt.z;
		yr = pt.y;
		return *this;
	}
	operator c_point_3<type>()const
	{
		return c_point_3<type>(xl, yl, xr);
	}

	type xl, yl, xr, yr;
};


typedef c_point_2<int> point_2i;
typedef c_point_2<float> point_2f;
typedef c_point_2<double> point_2d;
typedef c_point_3<double> point_3d;


struct STEREODRAW_API geo_vertex : public point_3d
{
	enum e_pen_code
	{
		code_none = 0,
		code_start = 1,
		code_line = 2,
		code_arc_tan = 3,
		code_arc_3p = 4,
		code_spline = 5,
		code_stream = 6
	};
	geo_vertex();
	geo_vertex(const point_3d& pt);
	geo_vertex(const point_3d& pt, int pen_code1, float pen_width1);

	float pen_width;
	byte pen_code;
};

struct STEREODRAW_API quad_points
{
	point_3d pts[4];
};

typedef std::vector<geo_vertex> vertex_array;
typedef std::vector<point_3d> point3d_array;

}