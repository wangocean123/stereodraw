#include "stdafx.h"
#include "matrix.h"
#include "geo_algorithom.h"

namespace StereoDraw
{


matrix3d STEREODRAW_API matrix3d_move(double dx, double dy)
{
	matrix3d m;

	m.data()[2] = dx;
	m.data()[5] = dy;

	return m;
}


matrix3d matrix3d_rotate(const point_2d *pt, double angle)
{
	double cosa = cos(angle), sina = sin(angle);

	double mt[] = { 1.0, 0.0, -pt->x, 0.0, 1.0, -pt->y, 0.0, 0.0, 1.0 };
	double mtr[] = { 1.0, 0.0, pt->x, 0.0, 1.0, pt->y, 0.0, 0.0, 1.0 };
	double ma[] = { cosa, -sina, 0.0, sina, cosa, 0.0, 0.0, 0.0, 1.0 };

	return matrix3d(mt)*matrix3d(ma)*matrix3d(mtr);
}


matrix3d matrix3d_zoom(double zoom_x, double zoom_y)
{
	matrix3d m;
	m.data()[0] = zoom_x;
	m.data()[4] = zoom_y;

	return m;
}



matrix4d matrix4d_move(double dx, double dy, double dz)
{
	matrix4d m;

	m.data()[3] = dx;
	m.data()[7] = dy;
	m.data()[11] = dz;
	return m;
}


matrix4d matrix4d_rotate(const point_3d *pt, const point_3d *dpt, double angle)
{
	double a=0, b=1;
	point_3d dpt0 = point_3d(0, 0, 1);
	if (dpt == NULL)
		dpt = &dpt0;

	a = sqrt(dpt->y*dpt->y + dpt->z*dpt->z);
	b = sqrt(dpt->x*dpt->x + dpt->y*dpt->y + dpt->z*dpt->z);
	if (a<1e-6 && b<1e-6)return matrix4d();

	double cosa=0, sina=0, cosb=0, sinb=0;

	if (a<1e-8)
	{ 
		cosa = 1; 
		sina = 0; 
	}
	else 
	{ 
		cosa = dpt->z / a; 
		sina = dpt->y / a; 
	}

	if (b<1e-8)
	{ 
		cosb = 1; 
		sinb = 0;
	}
	else 
	{ 
		cosb = a / b; 
		sinb = dpt->x / b; 
	}

	double cosx = cos(angle), sinx = sin(angle);

	double mt[16] = { 1.0, 0.0, 0.0, -pt->x, 0.0, 1.0, 0.0, -pt->y, 0.0, 0.0, 1.0, -pt->z, 0.0, 0.0, 0.0, 1.0 };
	double mtr[16] = { 1.0, 0.0, 0.0, pt->x, 0.0, 1.0, 0.0, pt->y, 0.0, 0.0, 1.0, pt->z, 0.0, 0.0, 0.0, 1.0 };

	double mrx[16] = { 1.0, 0.0, 0.0, 0.0, 0.0, cosa, -sina, 0.0, 0.0, sina, cosa, 0.0, 0.0, 0.0, 0.0, 1.0 };
	double mrxr[16] = { 1.0, 0.0, 0.0, 0.0, 0.0, cosa, sina, 0.0, 0.0, -sina, cosa, 0.0, 0.0, 0.0, 0.0, 1.0 };

	double mry[16] = { cosb, 0.0, sinb, 0.0, 0.0, 1.0, 0.0, 0.0, -sinb, 0.0, cosb, 0.0, 0.0, 0.0, 0.0, 1.0 };
	double mryr[16] = { cosb, 0.0, -sinb, 0.0, 0.0, 1.0, 0.0, 0.0, sinb, 0.0, cosb, 0.0, 0.0, 0.0, 0.0, 1.0 };

	double mrz[16] = { cosx, -sinx, 0.0, 0.0, sinx, cosx, 0.0, 0.0, 0.0, 0.0, 1.0, 0.0, 0.0, 0.0, 0.0, 1.0 };

	return matrix4d(mtr)*matrix4d(mrxr)*matrix4d(mryr)*matrix4d(mrz)*matrix4d(mry)*matrix4d(mrx)*matrix4d(mt);
}


matrix4d matrix4d_zoom(double zoom_x, double zoom_y, double zoom_z)
{
	matrix4d m;

	m.data()[0] = zoom_x;
	m.data()[5] = zoom_y;
	m.data()[10] = zoom_z;
	return m;
}


bool matrix4d_is_zrotate(const matrix4d& m)
{
	if (fabs(m(0, 2)) < 1e-10 && fabs(m(1, 2)) < 1e-10 && fabs(m(2, 0)) < 1e-10 && fabs(m(2, 1)) < 1e-10)
		return false;
	else
		return true;
}

}