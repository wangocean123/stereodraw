#pragma once

#include <vector>
#include <list>
#include <string>
#include "StereoDrawBaseType.h"
#include "text_style.h"
#include "point_def.h"
#include "rect_def.h"
#include "matrix.h"
#include "c_drawing_base.h"


namespace StereoDraw
{	

template<class tc_point>
class c_vector_obj
{
public:
	static const int pt_section_num = 256;
public:
	enum e_drawing_type
	{
		type_point = 1,
		type_cell,
		type_point_string,
		type_cell_string,
		type_line_string,
		type_polygon,
		type_text
	};
	struct c_range
	{
		tc_point m_pt0, m_pt1;
	};
	struct c_head
	{
		byte m_type;
	};

	typedef std::list<c_head*> section_list;

	struct c_point : public c_head
	{
		c_point()
		{
			m_type = type_point;
			m_color = 0xffffff;
			m_size = 1.0;
			m_bglobal_size = true;
			m_bflat = true;
		}
		tc_point m_pt;
		ulong m_color;
		float m_size;
		bool m_bglobal_size;
		bool m_bflat;
	};
	struct c_cell : public c_head
	{
		c_cell()
		{
			m_type = type_cell;
			m_color = 0xffffff;
			m_cell_index = 0;
			m_scale_x = 1.0;
			m_scale_y = 1.0;
			m_angle = 0.0;
			m_bglobal_scale = true;
		}
		tc_point m_pt;
		ulong m_color;
		int m_cell_index;
		float m_scale_x;
		float m_scale_y;
		float m_angle;
		bool m_bglobal_scale;
	};
	struct c_point_string : public c_head
	{
		c_point_string()
		{
			m_type = type_point_string;
			m_bglobal_size = true;
			m_bflat = true;
			m_size = 0;
		}
		c_point_string *copy_head()
		{
			c_point_string *p = new c_point_string();
			p->m_bglobal_size = m_bglobal_size;
			p->m_bflat = m_bflat;
			p->m_size = m_size;
			return p;
		}
		c_point_string& operator = (const c_point_string& t)
		{
			m_pts = t.m_pts;
			m_colors = t.m_colors;
			m_size = t.m_size;
			m_bglobal_size = t.m_bglobal_size;
			m_bflat = t.m_bflat;
			m_range = t.m_range;
			m_size = t.m_size;
			return *this;
		}
		std::vector<tc_point> m_pts;
		std::vector<ulong> m_colors;

		bool m_bglobal_size;
		bool m_bflat;
		float m_size;
		c_range m_range;
	};
	struct c_cell_string : public c_head
	{
		c_cell_string()
		{
			m_type = type_cell_string;
			m_cell_index = 0;
			m_scale_x = 1.0;
			m_scale_y = 1.0;
			m_bglobal_scale = true;
		}
		c_cell_string *copy_head()
		{
			c_cell_string *p = new c_cell_string();
			p->m_cell_index = m_cell_index;
			p->m_scale_x = m_scale_x;
			p->m_scale_y = m_scale_y;
			p->m_bglobal_scale = m_bglobal_scale;
			return p;
		}
		c_cell_string& operator = (const c_cell_string& t)
		{
			m_pts = t.m_pts;
			m_colors = t.m_colors;
			m_angles = t.m_angles;
			m_cell_index = t.m_cell_index;
			m_scale_x = t.m_scale_x;
			m_scale_y = t.m_scale_y;
			m_bglobal_scale = t.m_bglobal_scale;
			m_range = t.m_range;
			return *this;
		}
		std::vector<tc_point> m_pts;
		std::vector<ulong> m_colors;
		std::vector<float> m_angles;

		int m_cell_index;
		float m_scale_x;
		float m_scale_y;
		bool m_bglobal_scale;

		c_range m_range;
	};
	struct c_line_string : public c_head
	{
		c_line_string()
		{
			m_type = type_line_string;
			m_color = 0xffffff;
			m_line_style = 0;
			m_line_width = 1.0;
			m_bglobal_width = true;
		}
		c_line_string *copy_head()
		{
			c_line_string *p = new c_line_string();
			p->m_color = m_color;
			p->m_line_style = m_line_style;
			p->m_line_width = m_line_width;
			p->m_bglobal_width = m_bglobal_width;
			return p;
		}
		c_line_string& operator = (const c_line_string& t)
		{
			m_pts = t.m_pts;
			m_color = t.m_color;
			m_line_style = t.m_line_style;
			m_line_width = t.m_line_width;
			m_bglobal_width = t.m_bglobal_width;
			m_range = t.m_range;
			return *this;
		}
		std::vector<tc_point> m_pts;
		ulong m_color;
		int m_line_style;
		float m_line_width;
		bool m_bglobal_width;

		c_range m_range;
	};
	struct c_polygon : public c_head
	{
		c_polygon()
		{
			m_type = type_polygon;
			m_color = 0xffffff;
		}
		c_polygon *copy_head()
		{
			c_polygon *p = new c_polygon();
			p->m_color = m_color;
			return p;
		}
		c_polygon& operator = (const c_polygon& t)
		{
			m_pts = t.m_pts;
			m_counts = t.m_counts;
			m_color = t.m_color;
			m_range = t.m_range;
			return *this;
		}
		std::vector<tc_point> m_pts;
		std::vector<int> m_counts;
		ulong m_color;

		c_range m_range;
	};
	struct c_text : public c_head
	{
		c_text()
		{
			m_type = type_text;
			m_bglobal_size = true;
			m_bupward = false;
		}
		c_text& operator = (const c_text& t)
		{
			m_pt = t.m_pt;
			m_color = t.m_color;
			m_text = t.m_text;
			m_text_style = t.m_text_style;
			m_bglobal_size = t.m_bglobal_size;
			m_bupward = t.m_bupward;
			m_range = t.m_range;
			return *this;
		}
		tc_point m_pt;
		ulong m_color;
		std::string m_text;
		c_text_style m_text_style;
		bool m_bglobal_size;
		bool m_bupward;

		c_range m_range;
	};

	static bool is_point(const c_head *p){
		return (p != NULL && p->m_type == type_point);
	}

	static bool is_cell(const c_head *p){
		return (p != NULL && p->m_type == type_cell);
	}

	static bool is_point_string(const c_head *p){
		return (p != NULL && p->m_type == type_point_string);
	}

	static bool is_cell_string(const c_head *p){
		return (p != NULL && p->m_type == type_cell_string);
	}

	static bool is_line_string(const c_head *p){
		return (p != NULL && p->m_type == type_line_string);
	}

	static bool is_polygon(const c_head *p){
		return (p != NULL && p->m_type == type_polygon);
	}

	static bool is_text(const c_head *p){
		return (p != NULL && p->m_type == type_text);
	}

public:
	c_vector_obj()
	{
		m_bbegin_string = false;
	}

	~c_vector_obj()
	{
		clear();
	}

	void point(tc_point pt, ulong color, float size, bool bglobal_size, bool bflat)
	{
		c_point * p = new c_point();
		p->m_pt = pt;
		p->m_color = color;
		p->m_size = size;
		p->m_bglobal_size = bglobal_size;
		p->m_bflat = bflat;

		m_list.push_back(p);
	}

	void cell(tc_point pt, ulong color, int cell_index, float scale_x, float scale_y, bool bglobal_scale, float angle)
	{
		c_cell * p = new c_cell();
		p->m_pt = pt;
		p->m_color = color;
		p->m_cell_index = cell_index;
		p->m_scale_x = scale_x;
		p->m_scale_y = scale_y;
		p->m_bglobal_scale = bglobal_scale;
		p->m_angle = angle;

		m_list.push_back(p);
	}

	void begin_point_string(float size, bool bglobal_size, bool bflat)
	{
		c_point_string * p = new c_point_string();
		p->m_bglobal_size = bglobal_size;
		p->m_bflat = bflat;
		p->m_size = size;

		m_list.push_back(p);
		m_bbegin_string = true;
	}

	void point_string(tc_point pt, ulong color)
	{
		c_head *p_cur = get_string_tail();
		if (p_cur == NULL)
			return;
		if (p_cur->m_type != type_point_string)
			return;

		c_point_string *p = (c_point_string*)p_cur;

		if (p->m_pts.size() >= pt_section_num)
		{
			c_point_string *p_new = p->copy_head();
			p_new->m_pts.push_back(p->m_pts[p->m_pts.size() - 1]);
			m_list.push_back(p_new);
			p = p_new;
		}		
		p->m_pts.push_back(pt);
		p->m_colors.push_back(color);
	}

	void begin_cell_string(int cell_index, float scale_x, float scale_y, bool bglobal_scale)
	{
		c_cell_string * p = new c_cell_string();
		p->m_cell_index = cell_index;
		p->m_scale_x = scale_x;
		p->m_scale_y = scale_y;
		p->m_bglobal_scale = bglobal_scale;

		m_list.push_back(p);
		m_bbegin_string = true;
	}

	void cell_string(tc_point pt, ulong color, float angle)
	{
		c_head *p_cur = get_string_tail();
		if (p_cur == NULL)
			return;
		if (p_cur->m_type != type_cell_string)
			return;

		c_cell_string *p = (c_cell_string*)p_cur;
		if (p->m_pts.size() >= pt_section_num)
		{
			c_cell_string *p_new = p->copy_head();
			p_new->m_pts.push_back(p->m_pts[p->m_pts.size() - 1]);
			m_list.push_back(p_new);
			p = p_new;
		}

		p->m_pts.push_back(pt);
		p->m_colors.push_back(color);
		p->m_angles.push_back(angle);
	}

	void begin_line_string(ulong color, int line_style, float line_width, bool bglobal_width)
	{
		c_line_string * p = new c_line_string();
		p->m_color = color;
		p->m_line_style = line_style;
		p->m_line_width = line_width;
		p->m_bglobal_width = bglobal_width;

		m_list.push_back(p);
		m_bbegin_string = true;
	}

	void line_string(tc_point pt)
	{
		c_head *p_cur = get_string_tail();
		if (p_cur == NULL)
			return;
		if (p_cur->m_type != type_line_string)
			return;

		c_line_string *p = (c_line_string*)p_cur;
		if (p->m_pts.size() >= pt_section_num)
		{
			c_line_string *p_new = p->copy_head();
			p_new->m_pts.push_back(p->m_pts[p->m_pts.size() - 1]);
			m_list.push_back(p_new);
			p = p_new;
		}

		p->m_pts.push_back(pt);
	}

	void begin_polygon(ulong color)
	{
		c_polygon * p = new c_polygon();
		p->m_color = color;

		m_list.push_back(p);
		m_bbegin_string = true;
	}

	void polygon(tc_point pt)
	{
		c_head *p_cur = get_string_tail();
		if (p_cur == NULL)
			return;
		if (p_cur->m_type != type_polygon)
			return;

		c_polygon *p = (c_polygon*)p_cur;
		p->m_pts.push_back(pt);
	}

	void polygon(std::vector<tc_point>& pts)
	{
		c_head *p_cur = get_string_tail();
		if (p_cur == NULL)
			return;
		if (p_cur->m_type != type_polygon)
			return;

		c_polygon *p = (c_polygon*)p_cur;
		p->m_pts.insert(p->m_pts.end(), pts.begin(), pts.end());
		p->m_counts.push_back(pts.size());
	}

	void end()
	{
		if (!m_bbegin_string)
			return;

		c_head *p_cur = get_string_tail();
		if (p_cur == NULL)
			return;
		if (p_cur->m_type == type_polygon)
		{
			c_polygon *p = (c_polygon*)p_cur;
			if (p->m_counts.size() == 0 && p->m_pts.size() > 2)
			{
				p->m_counts.push_back(p->m_pts.size());
			}
			else
			{
				delete p;
				m_list.remove(m_list.back());
			}
		}
		else if (p_cur->m_type == type_line_string)
		{
			c_line_string *p = (c_line_string*)p_cur;
			if (p->m_pts.size() < 2)
			{
				delete p;
				m_list.remove(m_list.back());
			}
		}
		else if (p_cur->m_type == type_point_string)
		{
			c_point_string *p = (c_point_string*)p_cur;
			if (p->m_pts.size() < 2)
			{
				delete p;
				m_list.remove(m_list.back());
			}
		}
		else if (p_cur->m_type == type_cell_string)
		{
			c_cell_string *p = (c_cell_string*)p_cur;
			if (p->m_pts.size() < 2)
			{
				delete p;
				m_list.remove(m_list.back());
			}
		}

		m_bbegin_string = false;
	}

	void text(tc_point pt, ulong color, const char* words, const c_text_style& style, bool bglobal_size, bool bupward)
	{
		c_text * p = new c_text();

		p->m_pt = pt;
		p->m_color = color;
		p->m_text = words;
		p->m_text_style = style;
		p->m_bglobal_size = bglobal_size;
		p->m_bupward = bupward;

		m_list.push_back(p);
	}

	void clear()
	{
		std::list<c_head*>::iterator& it = m_list.begin();
		while (it != m_list.end())
		{
			c_head *p = *it;
			if (is_point(p))
				delete ((c_cell*)p);
			else if (is_cell(p))
				delete ((c_cell*)p);
			else if (is_point_string(p))
				delete ((c_point_string*)p);
			else if (is_cell_string(p))
				delete ((c_cell_string*)p);
			else if (is_line_string(p))
				delete ((c_line_string*)p);
			else if (is_polygon(p))
				delete ((c_polygon*)p);
			else if (is_text(p))
				delete ((c_text*)p);

			it++;
		}
		m_list.clear();
	}
	void remove_section(c_head* p)
	{
		m_list.remove(p);
	}

	void get_all_pts(std::vector<tc_point>& pts)
	{
		for (std::list<c_head*>::iterator& it = m_list.begin(); it != m_list.end(); it++)
		{
			const c_head *p = *it;
			if (p == NULL)continue;

			if (is_point(p))
			{
				pts.push_back(((const c_point*)p)->m_pt);
			}
			else if (is_cell(p))
			{
				pts.push_back(((const c_cell*)p)->m_pt);
			}
			else if (is_point_string(p))
			{
				pts.insert(pts.end(), ((const c_point_string*)p)->m_pts.begin(), ((const c_point_string*)p)->m_pts.end());
			}
			else if (is_cell_string(p))
			{
				pts.insert(pts.end(), ((const c_cell_string*)p)->m_pts.begin(), ((const c_cell_string*)p)->m_pts.end());
			}
			else if (is_line_string(p))
			{
				pts.insert(pts.end(), ((const c_line_string*)p)->m_pts.begin(), ((const c_line_string*)p)->m_pts.end());
			}
			else if (is_polygon(p))
			{
				pts.insert(pts.end(), ((const c_polygon*)p)->m_pts.begin(), ((const c_polygon*)p)->m_pts.end());
			}
			else if (is_text(p))
			{
				pts.push_back(((const c_text*)p)->m_pt);
			}
		}
	}
	void set_all_pts(std::vector<tc_point>& pts)
	{
		if (m_list.size() != pts.size())
			return;

		std::vector<tc_point>::iterator it0 = pts.begin();

		for (std::list<c_head*>::iterator it = m_list.begin(); it != m_list.end(); it++)
		{
			c_head *p = *it;
			if (p == NULL)continue;

			if (is_point(p))
			{
				((c_point*)p)->m_pt = *it0;
				it0++;
			}
			else if (is_cell(p))
			{
				((c_cell*)p)->m_pt = *it0;
				it0++;
			}
			else if (is_point_string(p))
			{
				((c_point_string*)p)->m_pts.assign(it0, it0 + ((c_point_string*)p)->m_pts.size());
				it0 += ((c_point_string*)p)->m_pts.size();
			}
			else if (is_cell_string(p))
			{
				((c_cell_string*)p)->m_pts.assign(it0, it0 + ((c_cell_string*)p)->m_pts.size());
				it0 += ((c_cell_string*)p)->m_pts.size();
			}
			else if (is_line_string(p))
			{
				((c_line_string*)p)->m_pts.assign(it0, it0 + ((c_line_string*)p)->m_pts.size());
				it0 += ((c_line_string*)p)->m_pts.size();
			}
			else if (is_polygon(p))
			{
				((c_polygon*)p)->m_pts.assign(it0, it0 + ((c_polygon*)p)->m_pts.size());
				it0 += ((c_polygon*)p)->m_pts.size();
			}
			else if (is_text(p))
			{
				((c_text*)p)->m_pt = *it0;
				it0++;
			}
		}
	}

	void set_all_color(ulong color)
	{
		for (std::list<c_head*>::iterator it = m_list.begin(); it != m_list.end(); it++)
		{
			c_head *p = *it;
			if (p == NULL)continue;

			if (is_point(p))
			{
				((c_point*)p)->m_color = color;
			}
			else if (is_cell(p))
			{
				((c_cell*)p)->m_color = color;
			}
			else if (is_point_string(p))
			{
				c_point_string *p2 = ((c_point_string*)p);
				for (std::vector<ulong>::iterator it2 = p2->m_colors.begin(); it2 != p2->m_colors.end(); it2++)
				{
					*it2 = color;
				}
			}
			else if (is_cell_string(p))
			{
				c_cell_string *p2 = ((c_cell_string*)p);
				for (std::vector<ulong>::iterator it2 = p2->m_colors.begin(); it2 != p2->m_colors.end(); it2++)
				{
					*it2 = color;
				}
			}
			else if (is_line_string(p))
			{
				((c_line_string*)p)->m_color = color;
			}
			else if (is_polygon(p))
			{
				((c_polygon*)p)->m_color = color;
			}
			else if (is_text(p))
			{
				((c_text*)p)->m_color = color;
			}
		}
	}

	void set_all_line_width(float line_width, bool bglobal_width)
	{
		for (std::list<c_head*>::iterator it = m_list.begin(); it != m_list.end(); it++)
		{
			c_head *p = *it;
			if (p == NULL)continue;

			if (is_line_string(p))
			{
				((c_line_string*)p)->m_line_width = line_width;
				((c_line_string*)p)->m_bglobal_width = bglobal_width;
			}
		}
	}

	void set_all_point_size(float point_size)
	{
		for (std::list<c_head*>::iterator it = m_list.begin(); it != m_list.end(); it++)
		{
			c_head *p = *it;
			if (p == NULL)continue;

			if (is_point(p))
			{
				((c_point*)p)->m_size = point_size;
			}
			else if (is_point_string(p))
			{
				((c_point_string*)p)->m_size = point_size;
			}
		}
	}

	void set_all_cell_index(int cell_index)
	{
		for (std::list<c_head*>::iterator it = m_list.begin(); it != m_list.end(); it++)
		{
			c_head *p = *it;
			if (p == NULL)continue;

			if (is_cell(p))
			{
				((c_cell*)p)->m_cell_index = cell_index;
			}
			else if (is_cell_string(p))
			{
				c_cell_string *p2 = ((c_cell_string*)p);
				p2->m_cell_index = cell_index;
			}
		}
	}

	int get_section_count(){
		return m_list.size();
	}

	void get_sections(std::list<c_head*>& list){
		list = m_list;
	}

	void copyfrom0(c_vector_obj<tc_point>* obj)
	{
		clear();
		for (std::list<c_head*>::iterator it = obj->m_list.begin(); it != obj->m_list.end(); it++)
		{
			const c_head *p = *it;
			if (p == NULL)continue;
			
			if (is_point(p))
			{
				c_point *p_new = new c_point();
				*p_new = *(c_point*)p;
				m_list.push_back(p_new);
			}
			else if (is_cell(p))
			{
				c_cell *p_new = new c_cell();
				*p_new = *(c_cell*)p;
				m_list.push_back(p_new);
			}
			else if (is_point_string(p))
			{
				c_point_string *p_new = new c_point_string();
				*p_new = *(c_point_string*)p;
				m_list.push_back(p_new);
			}
			else if (is_cell_string(p))
			{
				c_cell_string *p_new = new c_cell_string();
				*p_new = *(c_cell_string*)p;
				m_list.push_back(p_new);
			}
			else if (is_line_string(p))
			{
				c_line_string *p_new = new c_line_string();
				*p_new = *(c_line_string*)p;
				m_list.push_back(p_new);
			}
			else if (is_polygon(p))
			{
				c_polygon *p_new = new c_polygon();
				*p_new = *(c_polygon*)p;
				m_list.push_back(p_new);
			}
			else if (is_text(p))
			{
				c_text *p_new = new c_text();
				*p_new = *(c_text*)p;
				m_list.push_back(p_new);
			}
		}
	}

protected:
	c_head *get_string_tail()
	{
		if (!m_bbegin_string)
			return NULL;

		if (m_list.empty())
			return NULL;

		std::list<c_head*>::iterator it = m_list.end();

		it--;

		return *it;
	}

	bool m_bbegin_string;
	std::list<c_head*> m_list;
};


template<class point_type>
void rect2d_to_points(const rect_2d& rc, point_type& pt1, point_type& pt2)
{
	pt1.x = rc.x0;
	pt1.y = rc.y0;
	pt2.x = rc.x1;
	pt2.y = rc.y1;
}


template<class point_type>
rect_2d points_to_rect2d(const point_type& pt1, const point_type& pt2)
{
	rect_2d rc;
	rc.m_x0 = pt1.x;
	rc.m_y0 = pt1.y;
	rc.m_x1 = pt2.x;
	rc.m_y1 = pt2.y;
	return rc;
}

template<class point_type>
rect_3d points_to_rect3d(point_type& pt1, point_type& pt2)
{
	rect_3d rc;
	rc.x0 = pt1.x;
	rc.y0 = pt1.y;
	rc.z0 = pt1.z;
	rc.x1 = pt2.x;
	rc.y1 = pt2.y;
	rc.z1 = pt2.z;
	return rc;
}

template<class point_type>
void rect3d_to_points(const rect_3d& rc, point_type& pt1, point_type& pt2)
{
	pt1.x = rc.x0;
	pt1.y = rc.y0;
	pt1.z = rc.z0;
	pt2.x = rc.x1;
	pt2.y = rc.y1;
	pt2.z = rc.z1;
}


class c_vector_obj_3d;

class STEREODRAW_API c_vector_obj_2d : public c_vector_obj<point_2f>
{
	friend class c_vector_obj_3d;
public:
	c_vector_obj_2d();
	~c_vector_obj_2d();

	void move(double dx, double dy);
	void zoom(double kx, double ky);
	void rotate(point_2d pt, double angle);

	void refresh_bound();
	rect_2d get_bound(double view_scale);
	static rect_2d get_bound(const c_vector_obj_2d::c_head* p, double view_scale);
	void transform(const matrix4d& m);
	void copyfrom(c_vector_obj_3d* obj);
protected:
public:
	double m_origin_x, m_origin_y;
};



class STEREODRAW_API c_vector_obj_3d : public c_vector_obj<point_3d>
{
	friend class c_vector_obj_2d;
public:
	c_vector_obj_3d();
	~c_vector_obj_3d();

	void move(double dx, double dy, double dz);
	void zoom(double kx, double ky, double dz);
	void rotate(point_3d pt, double angle, point_3d *dpt = NULL);
	void transform(const matrix4d& m);
	void copyfrom(c_vector_obj_2d* obj);

	void refresh_bound();
	rect_3d get_bound(double view_scale);
	static rect_3d get_bound(const c_vector_obj_3d::c_head* p, double view_scale);
};


typedef std::list<c_vector_obj_2d*> vector_obj_2d_list;
typedef std::list<c_vector_obj_3d*> vector_obj_3d_list;

}
