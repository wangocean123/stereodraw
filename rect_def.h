#pragma once

#include "point_def.h"

namespace StereoDraw
{

	template<class type>
	class c_rect_2
	{
	public:
		c_rect_2(){
			x0 = 0; x1 = -1;
			y0 = 0; y1 = -1;
		}
		c_rect_2(type ix0, type iy0, type ix1, type iy1){
			x0 = ix0; x1 = ix1;
			y0 = iy0; y1 = iy1;
		}

		~c_rect_2()
		{
		}

		operator RECT()const
		{
			RECT rect1;
			rect1.left = x0;
			rect1.top = y0;
			rect1.right = x1;
			rect1.bottom = y1;
			return rect1;
		}

		type width()const{
			return x1 - x0;
		}
		type height()const{
			return y1 - y0;
		}

		type center_x()const{
			return (x0 + x1)*0.5;
		}

		type center_y()const{
			return (y0 + y1)*0.5;
		}

		c_point_2<type> center_point_2()const{
			c_point_2<type> pt;
			pt.x = (x0 + x1)*0.5;
			pt.y = (y0 + y1)*0.5;
			return pt;
		}

		void offset(type dx, type dy){
			x0 += dx; x1 += dx;
			y0 += dy; y1 += dy;
		}

		void inflate(type dx, type dy){
			x0 -= dx; x1 += dx;
			y0 -= dy; y1 += dy;
		}

		void deflate(type dx, type dy){
			x0 += dx; x1 -= dx;
			y0 += dy; y1 -= dy;
		}
	
		void intersect(const c_rect_2<type>& rc){
			x0 = (x0 > rc.x0 ? x0 : rc.x0);
			x1 = (x1<rc.x1 ? x1 : rc.x1);

			y0 = (y0>rc.y0 ? y0 : rc.y0);
			y1 = (y1 < rc.y1 ? y1 : rc.y1);
		}

		void union_rect(const c_rect_2<type>& rc){

			if (is_empty())
			{
				*this = rc;
				return;
			}
			x0 = (x0<rc.x0 ? x0 : rc.x0);
			x1 = (x1>rc.x1 ? x1 : rc.x1);

			y0 = (y0<rc.y0 ? y0 : rc.y0);
			y1 = (y1>rc.y1 ? y1 : rc.y1);
		}

		bool is_empty()const{
			return (x0 > x1 || y0 > y1);
		}
		
		bool is_contain(const c_rect_2<type>& rc)
		{
			return x0 <= rc.x0 && x1 >= rc.x1 && y0 <= rc.y0 && y1 >= rc.y1;
		}

		template<class point_type>
		bool is_contain_pt(const point_type& pt)
		{
			return x0 <= pt.x && x1 >= pt.x && y0 <= pt.y && y1 >= pt.y;
		}

		bool is_intersect(const c_rect_2<type>& rc)const{

			if (is_empty() || rc.is_empty())
				return false;

			if (x1<rc.x0 || x0>rc.x1)
				return false;

			if (y1<rc.y0 || y0>rc.y1)
				return false;

			return true;
		}

		template <class point_type>
		bool is_intersect_pts(const point_type& pt1, const point_type& pt2)
		{
			type tx0 = min(pt1.x, pt2.x);
			type tx1 = max(pt1.x, pt2.x);
			type ty0 = min(pt1.y, pt2.y);
			type ty1 = max(pt1.y, pt2.y);

			if (tx1<x0 || x1<tx0)
				return false;

			if (ty1<y0 || y1<ty0)
				return false;

			point_type pt3;
			pt3.x = x0; pt3.y = y0;
			bool b1 = geo_api::is_triangle_clockwise(pt1, pt2, pt3);

			pt3.x = x1; pt3.y = y0;
			bool b2 = geo_api::is_triangle_clockwise(pt1, pt2, pt3);

			if (b1 != b2)return true;

			pt3.x = x1; pt3.y = y1;
			bool b3 = geo_api::is_triangle_clockwise(pt1, pt2, pt3);

			if (b2 != b3)return true;

			pt3.x = x0; pt3.y = y1;
			bool b4 = geo_api::is_triangle_clockwise(pt1, pt2, pt3);

			if (b3 != b4)return true;

			return false;
		}

		c_rect_2<type> operator *(double k)
		{
			x0 *= k;
			y0 *= k;
			x1 *= k;
			y1 *= k;
			return *this;
		}
		c_rect_2<type> operator *=(double k)
		{
			x0 *= k;
			y0 *= k;
			x1 *= k;
			y1 *= k;
			return *this;
		}

		bool operator ==(const c_rect_2<type>& rc)const
		{
			return (x0 == rc.x0 && y0 == rc.y0 && x1 == rc.x1 && y1 == rc.y1 );
		}

		type x0, x1;
		type y0, y1;
	};


	template<class type>
	class c_rect_3 : public c_rect_2<type>
	{
	public:
		c_rect_3(){
			x0 = 0; x1 = -1;
			y0 = 0; y1 = -1;
			z0 = 0; z1 = -1;
		}
		c_rect_3(type ix0, type iy0, type iz0, type ix1, type iy1, type iz1){
			x0 = ix0; x1 = ix1;
			y0 = iy0; y1 = iy1;
			z0 = iz0; z1 = iz1;
		}

		c_rect_3(const c_rect_2<type>& rc){
			x0 = rc.x0; x1 = rc.x1;
			y0 = rc.y0; y1 = rc.y1;
			z0 = 0; z1 = -1;
		}

		~c_rect_3()
		{
		}

		type deepth()const{
			return z1 - z0;
		}

		type center_z()const{
			return (z0 + z1)*0.5;
		}
		
		c_point_3<type> center_point_3()const{
			c_point_3<type> pt;
			pt.x = (x0 + x1)*0.5;
			pt.y = (y0 + y1)*0.5;
			pt.z = (z0 + z1)*0.5;
			return pt;
		}

		void offset(type dx, type dy, type dz){
			x0 += dx; x1 += dx;
			y0 += dy; y1 += dy;
			z0 += dz; z1 += dz;
		}

		void inflate(type dx, type dy, type dz){
			x0 -= dx; x1 += dx;
			y0 -= dy; y1 += dy;
			z0 -= dz; z1 += dz;
		}

		void deflate(type dx, type dy, type dz){
			x0 += dx; x1 -= dx;
			y0 += dy; y1 -= dy;
			z0 += dz; z1 -= dz;
		}

		void intersect(const c_rect_3<type>& rc){
			x0 = (x0 > rc.x0 ? x0 : rc.x0);
			x1 = (x1<rc.x1 ? x1 : rc.x1);

			y0 = (y0>rc.y0 ? y0 : rc.y0);
			y1 = (y1<rc.y1 ? y1 : rc.y1);

			z0 = (z0>rc.z0 ? z0 : rc.z0);
			z1 = (z1 < rc.z1 ? z1 : rc.z1);
		}

		void union_point(const point_3d& pt){
			x0 = (x0<pt.x ? x0 : pt.x);
			x1 = (x1>pt.x ? x1 : pt.x);

			y0 = (y0<pt.y ? y0 : pt.y);
			y1 = (y1>pt.y ? y1 : pt.y);

			z0 = (z0<pt.z ? z0 : pt.z);
			z1 = (z1>pt.z ? z1 : pt.z);
		}

		void union_rect(const c_rect_3<type>& rc){
			x0 = (x0<rc.x0 ? x0 : rc.x0);
			x1 = (x1>rc.x1 ? x1 : rc.x1);

			y0 = (y0<rc.y0 ? y0 : rc.y0);
			y1 = (y1>rc.y1 ? y1 : rc.y1);

			z0 = (z0<rc.z0 ? z0 : rc.z0);
			z1 = (z1>rc.z1 ? z1 : rc.z1);
		}

		c_rect_3<type> operator *(double k)
		{
			x0 *= k;
			y0 *= k;
			x1 *= k;
			y1 *= k;
			z0 *= k;
			z1 *= k;
			return *this;
		}

		c_rect_3<type> operator *=(double k)
		{
			x0 *= k;
			y0 *= k;
			x1 *= k;
			y1 *= k;
			z0 *= k;
			z1 *= k;
			return *this;
		}

		bool is_empty()const{
			return (x0 > x1 || y0 > y1 || z0 > z1);
		}

		bool is_empty_2()const{
			return (x0 > x1 || y0 > y1);
		}

		bool is_contain(const c_rect_3<type>& rc)
		{
			return x0 <= rc.x0 && x1 >= rc.x1 && y0 <= rc.y0 && y1 >= rc.y1 && z0 <= rc.z0 && z1 >= rc.z1;
		}

		bool is_contain_2(const c_rect_3<type>& rc)
		{
			return x0 <= rc.x0 && x1 >= rc.x1 && y0 <= rc.y0 && y1 >= rc.y1;
		}

		template<class point_type>
		bool is_contain_pt_3(const point_type& pt)
		{
			return x0 <= pt.x && x1 >= pt.x && y0 <= pt.y0 && y1 >= pt.y && z0 <= pt.z && z1 >= pt.z;
		}

		bool is_intersect(const c_rect_3<type>& rc)const{

			if (is_empty() || rc.is_empty())
				return false;

			if (x1<rc.x0 || x0>rc.x1)
				return false;

			if (y1<rc.y0 || y0>rc.y1)
				return false;

			if (z1<rc.z0 || z0>rc.z1)
				return false;

			return true;
		}

		bool is_intersect_2(const c_rect_3<type>& rc)const{

			if (is_empty_2() || rc.is_empty_2())
				return false;

			if (x1<rc.x0 || x0>rc.x1)
				return false;

			if (y1<rc.y0 || y0>rc.y1)
				return false;

			return true;
		}

		bool operator ==(const c_rect_3<type>& rc)const
		{
			return (x0 == rc.x0 && y0 == rc.y0 && x1 == rc.x1 && y1 == rc.y1 && z0 == rc.z0 && z1 == rc.z1);
		}

		type z0, z1;
	};

	template<class type>
	class c_size
	{
	public:
		c_size()
		{
			cx = cy = 0;
		}
		c_size(type cx1, type cy1)
		{
			cx = cx1;
			cy = cy1;
		}

		type cx, cy;
	};

	typedef c_rect_2<double> rect_2d;
	typedef c_rect_2<float>  rect_2f;
	typedef c_rect_2<int>  rect_2i;
	typedef c_rect_3<double> rect_3d;
	typedef c_rect_3<float>  rect_3f;
	typedef c_size<int>		 size_i;
	typedef c_size<float>	 size_f;
	typedef c_size<double>	 size_d;

	template<class point_type>
	rect_2d create_rect2d_from_points(point_type *pts, int npt)
	{
		double x0, x1, y0, y1;
		for (int i = 0; i < npt; i++)
		{
			if (i == 0)
			{
				x0 = x1 = pts[0].x;
				y0 = y1 = pts[0].y;
			}
			else
			{
				x0 = (x0<pts[i].x ? x0 : pts[i].x);
				x1 = (x1>pts[i].x ? x1 : pts[i].x);

				y0 = (y0<pts[i].y ? y0 : pts[i].y);
				y1 = (y1>pts[i].y ? y1 : pts[i].y);
			}
		}

		if (npt <= 0)
			return rect_2d();
		else
			return rect_2d(x0, y0, x1, y1);
	}



	template<class point_type>
	rect_3d create_rect3d_from_points(point_type *pts, int npt)
	{
		double x0, x1, y0, y1, z0, z1;
		for (int i = 0; i < npt; i++)
		{
			if (i == 0)
			{
				x0 = x1 = pts[0].x;
				y0 = y1 = pts[0].y;
				z0 = z1 = pts[0].z;
			}
			else
			{
				x0 = (x0<pts[i].x ? x0 : pts[i].x);
				x1 = (x1>pts[i].x ? x1 : pts[i].x);

				y0 = (y0<pts[i].y ? y0 : pts[i].y);
				y1 = (y1>pts[i].y ? y1 : pts[i].y);

				z0 = (z0<pts[i].z ? z0 : pts[i].z);
				z1 = (z1>pts[i].z ? z1 : pts[i].z);
			}
		}

		if (npt <= 0)
			return rect_3d();
		else
			return rect_3d(x0, y0, z0, x1, y1, z1);
	}

	template<class rect_type, class point_type>
	void get_rect_corners(const rect_type& rc, point_type *pts)
	{
		pts[0].x = rc.x0; pts[0].y = rc.y1;
		pts[1].x = rc.x1; pts[1].y = rc.y1;
		pts[2].x = rc.x1; pts[2].y = rc.y0;
		pts[3].x = rc.x0; pts[3].y = rc.y0;
	}

	template<class rect_type, class point_type>
	void get_rect_corners_3d(const rect_type& rc, point_type *pts)
	{
		pts[0].x = rc.x0; pts[0].y = rc.y1; pts[0].z = rc.z0;
		pts[1].x = rc.x1; pts[1].y = rc.y1; pts[1].z = rc.z0;
		pts[2].x = rc.x1; pts[2].y = rc.y0; pts[2].z = rc.z0;
		pts[3].x = rc.x0; pts[3].y = rc.y0; pts[3].z = rc.z0;

		pts[4].x = rc.x0; pts[4].y = rc.y1;	pts[4].z = rc.z1;
		pts[5].x = rc.x1; pts[5].y = rc.y1;	pts[5].z = rc.z1;
		pts[6].x = rc.x1; pts[6].y = rc.y0;	pts[6].z = rc.z1;
		pts[7].x = rc.x0; pts[7].y = rc.y0;	pts[7].z = rc.z1;
	}

	template<class type, class point_type>
	c_rect_2<type> rect_from_center_2(point_type pt, float wid, float hei)
	{
		c_rect_2<type> rect;
		rect.x0 = pt.x - wid*0.5; 
		rect.x1 = pt.x + wid*0.5;
		rect.y0 = pt.y - hei*0.5;
		rect.y1 = pt.y + hei*0.5;
		return rect;
	}

	template<class type, class point_type>
	c_rect_2<type> rect_from_corner_2(point_type pt0, point_type pt1)
	{
		c_rect_2<type> rect;
		rect.x0 = pt0.x < pt1.x ? pt0.x : pt1.x;
		rect.x1 = pt0.x > pt1.x ? pt0.x : pt1.x;
		rect.y0 = pt0.y < pt1.y ? pt0.y : pt1.y;
		rect.y1 = pt0.y > pt1.y ? pt0.y : pt1.y;
		return rect;
	}

	template<class type, class point_type>
	c_rect_3<type> rect_from_corner_3(point_type pt0, point_type pt1)
	{
		c_rect_3<type> rect;
		rect.x0 = pt0.x < pt1.x ? pt0.x : pt1.x;
		rect.x1 = pt0.x > pt1.x ? pt0.x : pt1.x;
		rect.y0 = pt0.y < pt1.y ? pt0.y : pt1.y;
		rect.y1 = pt0.y > pt1.y ? pt0.y : pt1.y;
		rect.z0 = pt0.z < pt1.z ? pt0.z : pt1.z;
		rect.z1 = pt0.z > pt1.z ? pt0.z : pt1.z;
		return rect;
	}

	template<class type, class point_type>
	c_rect_3<type> rect_from_center_3(point_type pt, float wid, float hei, float deepth)
	{
		c_rect_3<type> rect;
		rect.x0 = pt.x - wid*0.5;
		rect.x1 = pt.x + wid*0.5;
		rect.y0 = pt.y - hei*0.5;
		rect.y1 = pt.y + hei*0.5;
		rect.z0 = pt.z - deepth*0.5;
		rect.z1 = pt.z + deepth*0.5;
		return rect;
	}

	template<class type0, class type1>
	void copy_rect_2(const c_rect_2<type0>& rect0, c_rect_2<type1>& rect1)
	{
		rect1.x0 = rect0.x0;
		rect1.y0 = rect0.y0;
		rect1.x1 = rect0.x1;
		rect1.y1 = rect0.y1;
	}


	template<class type0, class type1>
	void copy_rect_3(const c_rect_3<type0>& rect0, c_rect_3<type1>& rect1)
	{
		rect1.x0 = rect0.x0;
		rect1.y0 = rect0.y0;
		rect1.x1 = rect0.x1;
		rect1.y1 = rect0.y1;
		rect1.z0 = rect0.z0;
		rect1.z1 = rect0.z1;
	}

	template <class T> 
	inline c_rect_2<T> operator & (const c_rect_2<T>& rc, const c_rect_2<T>& rc2)
	{
		c_rect_2<T> rc3 = rc;
		rc3.intersect(rc2);
		return rc3;
	}


	template <class T>
	inline c_rect_2<T>  operator | (const c_rect_2<T>& rc, const c_rect_2<T>& rc2)
	{
		c_rect_2<T> rc3 = rc;
		rc3.union_rect(rc2);
		return rc3;
	}


	template <class T>
	inline c_rect_3<T> operator & (const c_rect_3<T>& rc, const c_rect_3<T>& rc2)
	{
		c_rect_3<T> rc3 = rc;
		rc3.intersect(rc2);
		return rc3;
	}


	template <class T>
	inline c_rect_3<T>  operator | (const c_rect_3<T>& rc, const c_rect_3<T>& rc2)
	{
		c_rect_3<T> rc3 = rc;
		rc3.union_rect(rc2);
		return rc3;
	}

}