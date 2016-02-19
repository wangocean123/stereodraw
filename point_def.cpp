#include "stdafx.h"
#include "point_def.h"


namespace StereoDraw
{

geo_vertex::geo_vertex()
{
	pen_width = 0;
	pen_code = geo_vertex::code_line;
}

geo_vertex::geo_vertex(const point_3d& pt)
{
	x = pt.x;
	y = pt.y;
	z = pt.z;
	pen_code = code_none;
	pen_width = 0;
}


geo_vertex::geo_vertex(const point_3d& pt, int pen_code1, float pen_width1)
{
	x = pt.x;
	y = pt.y;
	z = pt.z;
	pen_code = pen_code1;
	pen_width = pen_width1;
}

}