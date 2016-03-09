#pragma once

#include "math.h"
#include "GoodDraw.h"
#include "rect_def.h"
#include "matrix.h"
#include <vector>

namespace geo_api
{
	const double PI = 3.14159265358979324;

	double GOODDRAW_API get_toler_xy();
	void GOODDRAW_API set_toler_xy(double toler);

	double GOODDRAW_API get_toler_z();
	void GOODDRAW_API set_toler_z(double toler);

	template<class type>
	inline type gf_abs(type a)
	{
		return (a > 0 ? a : -a);
	}

	template<class point_type>
	inline double get_dis_2d(point_type pt1, point_type pt2)
	{
		return sqrt((pt1.x - pt2.x)*(pt1.x - pt2.x) + (pt1.y - pt2.y)*(pt1.y - pt2.y));
	}

	template<class point_type>
	inline double get_dis_square_2d(point_type pt1, point_type pt2)
	{
		return ((pt1.x - pt2.x)*(pt1.x - pt2.x) + (pt1.y - pt2.y)*(pt1.y - pt2.y));
	}

	template<class point_type>
	inline double get_dis_3d(point_type pt1, point_type pt2)
	{
		return sqrt((pt1.x - pt2.x)*(pt1.x - pt2.x) + (pt1.y - pt2.y)*(pt1.y - pt2.y) + (pt1.z - pt2.z)*(pt1.z - pt2.z));
	}

	template<class point_type>
	inline double get_dis_square_3d(point_type pt1, point_type pt2)
	{
		return ((pt1.x - pt2.x)*(pt1.x - pt2.x) + (pt1.y - pt2.y)*(pt1.y - pt2.y) + (pt1.z - pt2.z)*(pt1.z - pt2.z));
	}

	template<class point_type>
	inline point_type get_mid_2d(point_type pt1, point_type pt2)
	{
		point_type pt = pt1;
		pt.x = (pt1.x + pt2.x)*0.5;
		pt.y = (pt1.y + pt2.y)*0.5;
		return pt;
	}

	template<class point_type>
	inline point_type get_mid_3d(point_type pt1, point_type pt2)
	{
		point_type pt = pt1;
		pt.x = (pt1.x + pt2.x)*0.5;
		pt.y = (pt1.y + pt2.y)*0.5;
		pt.z = (pt1.z + pt2.z)*0.5;
		return pt;
	}

	template<class point_type1, class point_type2>
	inline void copy_xy(point_type1 pt1, point_type2& pt2)
	{
		pt2.x = pt1.x;
		pt2.y = pt1.y;
	}

	template<class point_type1, class point_type2>
	inline void copy_xyz(point_type1 pt1, point_type2& pt2)
	{
		pt2.x = pt1.x;
		pt2.y = pt1.y;
		pt2.z = pt1.z;
	}

	template<class point_type1, class point_type2>
	inline void copy_xy(std::vector<point_type1>& pts1, std::vector<point_type2>& pts2)
	{
		for (std::vector<point_type1>::iterator it = pts1.begin(); it != pts1.end(); it++)
		{
			point_type1 pt = *it;
			point_type2 pt2;
			pt2.x = pt.x;
			pt2.y = pt.y;
			pts2.push_back(pt2);
		}
	}

	template<class point_type1, class point_type2>
	inline void copy_xyz(std::vector<point_type1>& pts1, std::vector<point_type2>& pts2)
	{
		for (std::vector<point_type1>::iterator it = pts1.begin(); it != pts1.end(); it++)
		{
			point_type1 pt = *it;
			point_type2 pt2;
			pt2.x = pt.x;
			pt2.y = pt.y;
			pt2.z = pt.z;
			pts2.push_back(pt2);
		}
	}	

	template<class point_type1, class point_type2>
	inline void copy_xy(const point_type1 *pts1, point_type2 *pts2, int npt)
	{
		for (int i=0; i<npt; it++)
		{
			pts2[i].x = pts1[i].x;
			pts2[i].y = pts1[i].y;
		}
	}

	template<class point_type1, class point_type2>
	inline void copy_xyz(const point_type1 *pts1, point_type2 *pts2, int npt)
	{
		for (int i = 0; i<npt; it++)
		{
			pts2[i].x = pts1[i].x;
			pts2[i].y = pts1[i].y;
			pts2[i].z = pts1[i].z;
		}
	}

	template<class point_type>
	inline bool is_equal_point(point_type pt1, point_type pt2)
	{
		return (gf_abs(pt1.x - pt2.x)<get_toler_xy() && gf_abs(pt1.y - pt2.y)<get_toler_xy());
	}

	template<class point_type>
	inline bool is_equal_point_3(point_type pt1, point_type pt2)
	{
		return (gf_abs(pt1.x - pt2.x)<get_toler_xy() && gf_abs(pt1.y - pt2.y)<get_toler_xy() && gf_abs(pt1.z - pt2.z)<get_toler_z());
	}


	template<class point_type>
	double gf_multiply(point_type op, point_type sp, point_type ep)
	{
		return((sp.x - op.x)*(ep.y - op.y) - (ep.x - op.x)*(sp.y - op.y));
	}

	template<class point_type>
	double gf_dot_multiply(point_type p0, point_type p1, point_type p2)
	{
		return ((p1.x - p0.x)*(p2.x - p0.x) + (p1.y - p0.y)*(p2.y - p0.y));
	}

	/* 
	判断点与线段的关系（垂足在线段上的比例点，0,起点，1，终点，<0，起点前，>0终点后，0～1,内部）,
	本函数是根据下面的公式写的，P是点C到线段AB所在直线的垂足

			AC dot AB
	r =     ---------
			||AB||^2
		(Cx-Ax)(Bx-Ax) + (Cy-Ay)(By-Ay)
	  =  -------------------------------
				L^2

	r  has the following meaning:
		r=0      P = A
		r=1      P = B
		r<0		 P is on the backward extension of AB
		r>1      P is on the forward extension of AB
		0<r<1	 P is interior to AB

	*/

	template<class point_type>
	double get_relation(point_type p1, point_type p2, point_type pt_test)
	{
		double dis2 = get_dis_square_2d(p1, p2);
		if (dis2 < get_toler_xy()*get_toler_xy())
			return 0;

		return gf_dot_multiply(p1, p2, pt_test) / dis2;
	}

	template<class point_type>
	point_type get_perp(point_type p1, point_type p2, point_type pt_test)
	{
		double r = get_relation(p1, p2, pt_test);
		point_type tp = p1;
		tp.x = p1.x + r*(p2.x - p1.x);
		tp.y = p1.y + r*(p2.y - p1.y);
		return tp;
	}

	template<class point_type>
	point_type get_perp_3d(point_type p1, point_type p2, point_type pt_test)
	{
		double r = get_relation(p1, p2, pt_test);
		point_type tp = p1;
		tp.x = p1.x + r*(p2.x - p1.x);
		tp.y = p1.y + r*(p2.y - p1.y);
		tp.z = p1.z + r*(p2.z - p1.z);
		return tp;
	}

	/* 
	返回原点在o点，起始边为os，终止边为oe的夹角(单位：弧度)
	可以用于求线段之间的夹角(0~pi)
	原理：
	r = dotmultiply(s,e,o) / (dist(o,s)*dist(o,e))
	r'= multiply(s,e,o)
	r >= 1	angle = 0;
	r <= -1	angle = -PI
	-1<r<1 && r'>0	angle = arccos(r)
	-1<r<1 && r'<=0	angle = -arccos(r)
	*/
	template<class point_type>
	double get_angle(point_type o, point_type s, point_type e)
	{
		double cosfi, fi, norm;
		double dsx = s.x - o.x;
		double dsy = s.y - o.y;
		double dex = e.x - o.x;
		double dey = e.y - o.y;

		cosfi = dsx*dex + dsy*dey;
		norm = sqrt((dsx*dsx + dsy*dsy)*(dex*dex + dey*dey));

		if (norm < get_toler_xy()*get_toler_xy())
			return 0;

		cosfi /= norm;

		if (cosfi >= 1.0) return 0;
		if (cosfi <= -1.0) return PI;

		fi = acos(cosfi);
		if (dsx*dey - dsy*dex>0) return fi;      // 说明矢量os 在矢量 oe的顺时针方向 
		return 2*PI-fi;
	}

	//两个线段之间的夹角(0~PI)
	template<class point_type>
	double get_angle(point_type pt1, point_type pt2, point_type pt3, point_type pt4)
	{
		point_type o, s, e;
		o.x = o.y = 0;
		s.x = pt2.x - pt1.x;
		s.y = pt2.y - pt1.y;
		e.x = pt4.x - pt3.x;
		e.y = pt4.y - pt3.y;

		return get_angle(o,s,e);
	}

	//线段与x轴的夹角（0～2pi）
	template<class point_type>
	double get_angle(point_type pt1, point_type pt2)
	{
		double dx = pt2.x - pt1.x, dy = pt2.y - pt1.y;
		double udx = dx<0?-dx:dx, udy = dy<0?-dy:dy;

		double toler = get_toler_xy();
		double ang = 0;
		if (udx < udy)
		{
			if (udy < toler)
				return 0;
			else
				ang = PI*0.5 - atan(dx / dy);

			if (ang < 0)ang += PI;
		}
		else
		{
			if (udx < toler)
				return 0;
			else
				ang = atan(dy / dx);
		}

		if(dy < 0)ang += PI;
		return ang;
	}

	template<class point_type>
	double get_dis_pt_to_line(point_type p1, point_type p2, point_type pt_test)
	{
		return get_dis_2d(pt_test, get_perp(p1, p2, pt_test));
	}

	template<class point_type>
	double get_dis_pt_to_lineseg(point_type p1, point_type p2, point_type pt_test)
	{
		double r = get_relation(p1, p2, pt_test);
		if (r<0)
		{
			return get_dis_2d(pt_test, p1);
		}
		if (r>1)
		{
			return get_dis_2d(pt_test, p2);
		}
		point_type tp = p1;
		tp.x = p1.x + r*(p2.x - p1.x);
		tp.y = p1.y + r*(p2.y - p1.y);
		return get_dis_2d(pt_test, tp);
	}

	template<class point_type>
	bool is_pt_on_line(point_type p1, point_type p2, point_type pt_test)
	{
		return (get_dis_pt_to_line(p1, p2, pt_test)<get_toler_xy());
	}

	//0,outside,1,inside,2,end point
	template<class point_type>
	int is_pt_on_lineseg(point_type p1, point_type p2, point_type pt_test)
	{
		if (get_dis_2d(pt_test, p1) < get_toler_xy() || get_dis_2d(pt_test, p2) < get_toler_xy())
			return 2;

		if (get_dis_pt_to_line(p1, p2, pt_test) < get_toler_xy())
		{
			double r = get_relation(p1, p2, pt_test);
			if (r >= 0 && r <= 1)
				return 1;
		}
		return 0;
	}

	template<class point_type>
	double get_nearest_pt_to_linestring(const point_type *pts, int npt, point_type pt_test, point_type *pt_ret, double *pos)
	{
		int min_index = -1;
		double dis, min_dis = -1;
		for (int i = 0; i<npt - 1; i++)
		{
			dis = get_dis_pt_to_lineseg(pts[i], pts[i + 1], pt_test);
			if (min_index<0 || min_dis>dis)
			{
				min_dis = dis;
				min_index = i;
			}
		}

		if (min_index < 0)
			return -1;

		double r = get_relation(pts[min_index], pts[min_index + 1], pt_test);
		if (r <= 0)
		{
			if (pt_ret)*pt_ret = pts[min_index];
			if (pos)*pos = min_index;
		}
		else if (r >= 1)
		{
			if (pt_ret)*pt_ret = pts[min_index+1];
			if (pos)*pos = min_index + 1;
		}
		else
		{
			if (pt_ret)
			{
				*pt_ret = pts[min_index];
				pt_ret->x = pts[min_index].x + r*(pts[min_index+1].x - pts[min_index].x);
				pt_ret->y = pts[min_index].y + r*(pts[min_index+1].y - pts[min_index].y);
			}
			if (pos)*pos = min_index + r;
		}
		return min_dis;
	}

	// P = P0 + ▲P0 * t0, P' = P1 + ▲P1 * t1
	//          (x1-x0)*▲P1y - (y1-y0)▲P1x
	//  t0 = -----------------------------------
	//            ▲P1y*▲P0x - ▲P1x*▲P0y

	template<class point_type>
	inline bool get_intersect_of_lines(point_type pt1, point_type pt2, point_type pt3, point_type pt4, point_type* pt_ret, double *relation)
	{
		double x0 = pt1.x, y0 = pt1.y;
		double x1 = pt3.x, y1 = pt3.y;
		double v0x = pt2.x - pt1.x, v0y = pt2.y - pt1.y;
		double v1x = pt4.x - pt3.x, v1y = pt4.y - pt3.y;
		double divide_num = v1y*v0x - v1x*v0y;
		if (gf_abs(divide_num) < 1e-10)
			return false;

		double t0 = ((x1 - x0)*v1y - (y1 - y0)*v1x)/divide_num;
		if (pt_ret)
		{
			*pt_ret = pt1;
			pt_ret->x = x0 + t0*v0x;
			pt_ret->y = y0 + t0*v0y;
		}
		if (relation)
		{
			*relation = t0;
		}
		return true;
	}

	template<class point_type>
	inline bool get_intersect_of_linesegs(point_type pt1, point_type pt2, point_type pt3, point_type pt4, point_type* pt_ret, double *relation1, double *relation2)
	{
		double x0 = pt1.x, y0 = pt1.y;
		double x1 = pt3.x, y1 = pt3.y;
		double v0x = pt2.x - pt1.x, v0y = pt2.y - pt1.y;
		double v1x = pt4.x - pt3.x, v1y = pt4.y - pt3.y;
		double divide_num = v1y*v0x - v1x*v0y;
		if (gf_abs(divide_num) < 1e-10)
			return false;

		double t0 = ((x1 - x0)*v1y - (y1 - y0)*v1x) / divide_num;
		double t1 = ((x1 - x0)*v0y - (y1 - y0)*v0x) / divide_num;
		if (t0<0 || t0>1 || t1<0 || t1>1)
			return false;

		if (pt_ret)
		{
			*pt_ret = pt1;
			pt_ret->x = x0 + t0*v0x;
			pt_ret->y = y0 + t0*v0y;
		}
		if (relation1)
		{
			*relation1 = t0;
		}
		if (relation2)
		{
			*relation2 = t1;
		}
		return true;
	}

	template<class point_type>
	inline bool get_intersect_of_linesegs_toler(point_type pt1, point_type pt2, point_type pt3, point_type pt4, point_type* pt_ret, double *relation1, double *relation2)
	{
		double x0 = pt1.x, y0 = pt1.y;
		double x1 = pt3.x, y1 = pt3.y;
		double v0x = pt2.x - pt1.x, v0y = pt2.y - pt1.y;
		double v1x = pt4.x - pt3.x, v1y = pt4.y - pt3.y;
		double divide_num = v1y*v0x - v1x*v0y;
		if (gf_abs(divide_num) < 1e-10)
			return false;

		double t0 = ((x1 - x0)*v1y - (y1 - y0)*v1x) / divide_num;
		double t1 = ((x1 - x0)*v0y - (y1 - y0)*v0x) / divide_num;
		point_type ret = pt1;
		ret.x = x0 + t0*v0x;
		ret.y = y0 + t0*v0y;

		bool in_toler = false;
		if (t0 < 0 && get_dis_2d(pt1, ret) < get_toler_xy())
			in_toler = true;
		else if (t0>1 && get_dis_2d(pt2, ret) < get_toler_xy())
			in_toler = true;
		else if (t1 < 0 && get_dis_2d(pt3, ret) < get_toler_xy())
			in_toler = true;
		else if (t1>1 && get_dis_2d(pt4, ret) < get_toler_xy())
			in_toler = true;

		if (t0>=0 && t0<=1 && t1<=0 && t1<=1)
			in_toler = true;

		if (!in_toler)
			return false;

		if (pt_ret)
		{
			*pt_ret = ret;
		}
		if (relation1)
		{
			*relation1 = t0;
		}
		if (relation2)
		{
			*relation2 = t1;
		}
		return true;
	}

	template<class point_type>
	int del_repeat_points(point_type *pts, int npt)
	{
		double toler = get_toler_xy()*get_toler_xy();
		int cur_pos = 0;
		for (int i = 1; i < npt; i++)
		{
			if (get_dis_square_2d(pts[i], pts[cur_pos]) >= toler)
			{
				if (cur_pos != (i + 1))
				{
					pts[cur_pos + 1] = pts[i];
				}
				cur_pos++;
			}
		}
		return cur_pos + 1;
	}

	//-1，outside, 0, online, 1, inside
	template<class point_type>
	int is_pt_in_polygon(point_type pt, const point_type *pts, int npt)
	{
		double toler = get_toler_xy();

		//采用水平向右的射线进行相交判断
		int cross_count = 0;

		/*
		注意到如果从P作水平向左的射线的话，如果P在多边形内部，那么这条射线与多边形的交点必为奇数，如果P在多边形外部，则交点个数必为偶数（0也在内）。
		所以，我们可以顺序考虑多边形的每条边，求出交点的总个数。还有一些特殊情况要考虑。假如考虑边(P1,P2)，
		1)如果射线正好穿过P1或者P2,那么这个交点会被算作2次，处理办法是如果P的从坐标与P1,P2中较小的纵坐标相同，则直接忽略这种情况
		2)如果射线水平，则射线要么与其无交点，要么有无数个，这种情况也直接忽略。
		3)如果射线竖直，而P0的横坐标小于P1,P2的横坐标，则必然相交。
		4)再判断相交之前，先判断P是否在边(P1,P2)的上面，如果在，则直接得出结论：P在多边形内部。
		*/		
		for (int i = 0; i < npt; i++)
		{
			int j = (i + 1) % npt;
			double y0, y1;
			y0 = min(pts[i].y, pts[j].y);
			y1 = max(pts[i].y, pts[j].y);

			//完全落在边界线段外
			if (pt.y<y0 - toler || pt.y>y1 + toler)
				continue;

			if (pt.x>pts[i].x + toler && pt.x > pts[j].x + toler)
				continue;

			//刚好落在边界线段上
			if (!(pt.x < pts[i].x - toler && pt.x < pts[j].x - toler))
			{
				if (is_pt_on_lineseg(pt, pts[i], pts[j]))
				{
					return 0;
				}
			}			
			
			//射线穿过一个顶点，忽略y值较小的那个点，这样交点的计算就只会有一次
			if (gf_abs(pt.y - y0) < toler && gf_abs(pt.y - y1) >= toler )
				continue;
			else if (gf_abs(pt.y - y1) < toler && gf_abs(pt.y - y0) >= toler)
			{ 
				cross_count++;
			}
			//射线穿过两个顶点(水平线段)
			else if (gf_abs(y1 - y0) < toler)
			{
			}
			//当gf_abs(pt.y - y0) < toler && gf_abs(pt.y - y1) < toler且又不落在边界线段上时，与一般相交等同处理 
			else
			{
				double x = pts[i].x + (pts[j].x - pts[i].x) * (pt.y - pts[i].y) / (pts[j].y - pts[i].y);
				if (pt.x < x)
					cross_count++;
			}
		}

		return (cross_count % 1) == 0 ? -1 : 1;
	}

	template<class point_type>
	inline double get_area_triangle(point_type P0, point_type P1, point_type Pt)
	{
		return gf_abs(((P1.x - P0.x) * (Pt.y - P0.y) - (Pt.x - P0.x) * (P1.y - P0.y))*0.5);
	}

	template<class point_type>
	inline double get_area_polygon(const point_type *pts, int n)
	{
		double area = 0;
		int  i, j;   // indices

		if (n < 3) return 0;  // a degenerate polygon

		for (i = 0, j = 1; j<n; i++, j++) 
		{
			area += (pts[i].y + pts[j].y)*(pts[j].x - pts[i].x);
		}
		area += (pts[n - 1].y + pts[0].y)*(pts[0].x - pts[n - 1].x);  // wrap-around term
		return area / 2.0;
	}

	template<class point_type>
	point_type get_rotate_point(point_type o, point_type p, double alpha, double new_length)
	{
		double cosa = cos(alpha), sina = sin(alpha);
		point_type tp = o;
		if (new_length <= 0)
		{
			p.x -= o.x;
			p.y -= o.y;
		}
		else
		{
			double dis = get_dis_2d(p, o);
			if (dis < get_toler_xy())
			{
				p.x = new_length;
				p.y = 0;
			}
			else
			{
				p.x -= o.x;
				p.y -= o.y;
				p.x *= new_length / dis;
				p.y *= new_length / dis;
			}
		}

		tp.x = p.x*cosa - p.y*sina + o.x;
		tp.y = p.y*cosa + p.x*sina + o.y;
		return tp;
	}
	template<class point_type>
	point_type get_right_angle_point(point_type o, point_type p, double new_length)
	{
		point_type tp = o;
		if (new_length <= 0)
		{
			p.x -= o.x;
			p.y -= o.y;
		}
		else
		{
			double dis = get_dis_2d(p, o);
			if (dis < get_toler_xy())
			{
				p.x = new_length;
				p.y = 0;
			}
			else
			{
				p.x -= o.x;
				p.y -= o.y;
				p.x *= new_length / dis;
				p.y *= new_length / dis;
			}
		}
		tp.x = - p.y + o.x;
		tp.y = p.x + o.y;
		return tp;
	}

	/******************************************************************************
	用 途	：求不共线的三点确定一个圆
	输 入	：三个点p1,p2,p3
	返回值	：如果三点共线，返回false；反之，返回true。圆心由q返回，半径由r返回
	*******************************************************************************/
	template<class point_type>
	bool get_circle(point_type p1, point_type p2, point_type p3, point_type &center, double &r)
	{
		double dis1 = get_dis_2d(p1, p2);
		double dis2 = get_dis_2d(p3, p2);
		if (dis1 < get_toler_xy() || dis2 < get_toler_xy())
			return false;

		double x12 = p2.x - p1.x;
		double y12 = p2.y - p1.y;
		double x13 = p3.x - p1.x;
		double y13 = p3.y - p1.y;
		double z2 = x12*(p1.x + p2.x) + y12*(p1.y + p2.y);
		double z3 = x13*(p1.x + p3.x) + y13*(p1.y + p3.y);
		double d = 2.0*(x12*(p3.y - p2.y) - y12*(p3.x - p2.x));
		if (gf_abs(d) / max(dis1, dis2)<get_toler_xy()) //共线，圆不存在 
			return false;

		center = p1;
		center.x = (y13*z2 - y12*z3) / d;
		center.y = (x12*z3 - x13*z2) / d;
		r = get_dis_2d(p1, center);
		return true;
	}


	//1,顺时针，0,逆时针，-1,无效多边形；当为非简单多边形时，返回值无参考性
	template<class point_type>
	int is_polygon_clockwise(const point_type *pts, int npt)
	{
		if (npt <= 2)
			return -1;

		double *ang_buf = new double[npt];
		int ang_count = 0;
		for (int i = 0; i < npt-1; i++)
		{
			if (is_equal_point(pts[i], pts[i + 1]))
				continue;

			ang_buf[ang_count++] = get_angle(pts[i], pts[i + 1]);
		}

		if (!is_equal_point(pts[i - 1], pts[0]))
		{
			ang_buf[ang_count++] = get_angle(pts[i-1], pts[0]);
		}

		double ang = 0, pi2 = PI+PI;
		double limit1 = pi2 + 0.0001;
		double limit2 = -limit1;

		for (int i = 0; i < ang_count; i++)
		{
			double ang1 = (ang_buf[(i+1)%ang_count] - ang_buf[i]);

			if (ang1<-PI)ang1 += pi2;
			else if (ang1>PI)ang1 -= pi2;
			ang += ang1;

			//自相交了
			if (ang>limit1 || ang<limit2)
			{
				delete[] ang_buf;
				return -1;
			}
		}

		delete[] ang_buf;

		if (ang>PI)
			return 1;
		if (ang < -PI)
			return 0;
		return -1;
	}

	//此算法不准确
	template<class point_type>
	bool is_polygon_self_intersect(const point_type *pts, int npt)
	{
		if (npt <= 2)
			return false;
		return (is_polygon_clockwise(pts)!=-1);
	}

	template<class point_type>
	bool is_triangle_clockwise(point_type p1, point_type p2, point_type p3)
	{
		return (p2.x - p1.x) * (p3.y - p1.y) - (p3.x - p1.x) * (p2.y - p1.y)<0;
	}

	template<class point_type>
	void simplize_points(std::vector<point_type>& pts, double toler)
	{
		int size = pts.size();
		if (size <= 2)
			return;

		std::vector<point_type> result;
		point_type pt1 = pts[0], pt2 = pts[1];

		result.push_back(pt1);

		double dx1 = 0, dx2 = 0, dy1 = 0, dy2 = 0;
		for (int i = 2; i < size; i++)
		{
			double t0 = get_relation(pt1, pt2, pts[i]);

			point_type tp = get_perp(pt1, pts[i], pt2);

			bool bleft = gf_multiply(pt1, pt2, pts[i])>0;

			double dis_y = get_dis_2d(tp, pt2);
			double dis_x = get_dis_2d(tp, pts[i]);

			if (bleft)
			{
				dy1 += dis_y;
				dy2 -= dis_y;
				if (dy2 < 0)dy2 = 0;
			}
			else
			{
				dy2 += dis_y;
				dy1 -= dis_y;
				if (dy1 < 0)dy1 = 0;
			}

			if (t0 >= 0)
			{
				dx2 += dis_x;
				dx1 -= dis_x;
				if (dx1 < 0)dx1 = 0;
			}
			else
			{
				dx1 += dis_x;
				dx2 -= dis_x;
				if (dx2 < 0)dx2 = 0;
			}

			if (dx1 >= toler || dy1 >= toler || dy2 >= toler)
			{
				result.push_back(pt2);
				pt1 = pt2;
				pt2 = pts[i];
				dx1 = dy1 = dy2 = 0;
			}
		}

		result.push_back(pt2);
		pts = result;
	}

	//几何向量
	template<class PointType>
	class vector
	{
	public:
		double dx, dy;
	public:
		vector()
		{
			dx = dy = 0;
		}
		vector(PointType pt1, PointType pt2)
		{
			dx = pt2.x - pt1.x;
			dy = pt2.y - pt1.y;
		}
		~vector()
		{
		}
		void rotate90()
		{
			double dx2 = -dy;
			double dy2 = dx;
			dx = dx2;
			dy = dy2;
		}
		void rotate180()
		{
			dx = -dx;
			dy = -dy;
		}
		void rotate270()
		{
			double dx2 = dy;
			double dy2 = -dx;
			dx = dx2;
			dy = dy2;
		}
		void rotate(double ang)
		{
			double cosa = cos(ang), sina = cos(ang);
			double dx2 = dx*cosa - dy*sina;
			double dy2 = dx*sina + dy*cosa;
			dx = dx2;
			dy = dy2;
		}
		void unit()
		{
			double dis = sqrt(dx*dx + dy*dy);
			if (dis == 0.0)
				return;
			dx /= dis;
			dy /= dis;
		}

		void to_length(double len)
		{
			dx *= len;
			dy *= len;
		}

		void add_to(PointType& pt)
		{
			pt.x += dx;
			pt.y += dy;
		}
	};


	gd::Rect3D transform_rect3d(const gd::Rect3D& rect, const gd::matrix4d& m);

	template<class point_type>
	point_type transform_point3d(const point_type& pt, const gd::matrix4d& m)
	{
		point_type pt2 = pt;
		gd::transform_pts_by_matrix_3d(&pt2, 1, m);
		return pt2;
	}
}