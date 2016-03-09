#include "StdAfx.h"
#include "c_geo_line.h"
#include "geo_algorithom.h"
#include "Curve.h"

namespace gis
{

	c_geo_linear_obj::shape_rect::shape_rect()
	{
		pvertexs = NULL;
		start_index = -1;
		pt_count = 0;
	}

	c_geo_linear_obj::c_geo_linear_obj()
	{

	}

	c_geo_linear_obj::~c_geo_linear_obj()
	{
	}


	GeometryType c_geo_linear_obj::get_geometry_type()const
	{
		return GEO_NULL;
	}

	int c_geo_linear_obj::get_vertex_num()const
	{
		return m_vertex_indexs.size();
	}

	bool c_geo_linear_obj::set_vertexs(const gd::GeoVertex* pts, int npt)
	{
		if (npt >= 2)
		{
			m_shape_pts.clear();
			Curve::Curve(pts, npt, 0.01, is_closed(), m_shape_pts);
		}
		else
		{
			for (int i = 0; i < npt; i++)
			{
				m_shape_pts.push_back(pts[i]);
			}
		}

		m_vertex_indexs.clear();
		Curve::GetVertexIndex(m_shape_pts, m_vertex_indexs);

		m_shape_rects.clear();
		get_shape_rects(m_shape_pts, m_shape_rects);

		m_bound = gd::Rect3D();
		for (int i = 0; i < m_shape_rects.size(); i++)
		{
			if (i == 0)
				m_bound = m_shape_rects[i].rect;
			else
				m_bound.union_rect(m_shape_rects[i].rect);
		}

		return true;
	}

	void c_geo_linear_obj::get_shape_rects(const gd::vertex_array& shape_pts, shape_rect_array& rects)
	{
		int size = shape_pts.size();
		int step = 256;
		for (int i = 0; i < size; i += (step-1) )
		{
			shape_rect rect;

			int num = step;
			if (size < i + step)
				num = size - i;
			rect.rect = gd::create_rect3d_from_points(shape_pts.data() + i, num);
			rect.pvertexs = &shape_pts;
			rect.start_index = i;
			rect.pt_count = num;
			rects.push_back(rect);
		}
	}

	void c_geo_linear_obj::get_vertexs(gd::vertex_array& pts)const
	{
		pts.clear();
		pts.assign(m_vertex_indexs.size(), gd::GeoVertex());
		for (int i = 0; i < m_vertex_indexs.size(); i++)
		{
			pts[i] = m_shape_pts[m_vertex_indexs[i]];
		}
	}

	void c_geo_linear_obj::get_shape_pts(gd::vertex_array& pts)const
	{
		pts = m_shape_pts;
	}

	int c_geo_linear_obj::get_vertex_index(double shape_pos)const
	{
		int i = 0;
		for (i = 0; i < m_vertex_indexs.size(); i++)
		{
			if (shape_pos < m_vertex_indexs[i])
			{
				return i - 1;
			}
		}
		return i;
	}

	bool c_geo_linear_obj::is_valid()const
	{
		return m_vertex_indexs.size()>=2;
	}

	gd::Rect3D c_geo_linear_obj::getBound(double view_scale)const
	{
		return m_bound;
	}

	bool c_geo_linear_obj::is_closed()const
	{
		if (m_shape_pts.size() <= 2)
			return false;

		if (geo_api::is_equal_point(m_shape_pts[0], m_shape_pts[m_shape_pts.size() - 1]))
		{
			return true;
		}
		return false;
	}

	void c_geo_linear_obj::ensure_closed(bool bclosed)
	{
		if (m_shape_pts.size() <= 2)
			return;

		gd::vertex_array pts;
		get_vertexs(pts);
		if (pts.size() <= 2)
			return;

		if (is_closed())
		{
			if (!bclosed)
			{
				pts.erase(pts.end() - 1);
				set_vertexs(pts.data(), pts.size());
			}
		}
		else
		{
			if (bclosed)
			{
				pts.push_back(pts[0]);
				set_vertexs(pts.data(), pts.size());
			}
		}
	}

	void c_geo_linear_obj::get_shape_rects(shape_rect_array& rects)const
	{
		rects = m_shape_rects;
	}

	void c_geo_linear_obj::copy_from(const c_geometry *p)
	{
		c_geometry::copy_from(p);

		const c_geo_linear_obj *p1 = dynamic_cast<const c_geo_linear_obj*>(p);
		if (!p1)return;

		m_shape_pts = p1->m_shape_pts;
		m_vertex_indexs = p1->m_vertex_indexs;
		m_shape_rects = p1->m_shape_rects;

		for (int i = 0; i < m_shape_rects.size(); i++)
		{
			m_shape_rects[i].pvertexs = &m_shape_pts;
		}
		m_bound = p1->m_bound;
	}

	void c_geo_linear_obj::write_to(gd::AttributeSet& tab)const
	{
		c_geometry::write_to(tab);
	}

	void c_geo_linear_obj::read_from(gd::AttributeSet& tab)
	{
		c_geometry::read_from(tab);
	}

	void c_geo_linear_obj::paint(gd::GraphicsObject3D *pobj, double view_scale)const
	{
		pobj->begin_line_string(m_color, 0, 0, false);
		for (int i = 0; i < m_shape_pts.size(); i++)
		{
			pobj->line_string(m_shape_pts[i]);
		}
		pobj->end();
	}

	c_geo_line_string::c_geo_line_string()
	{
		m_line_width = 0;
		m_line_style_scale = 1;
	}


	c_geo_line_string::~c_geo_line_string()
	{
	}


	GeometryType c_geo_line_string::get_geometry_type()const
	{
		return GEO_LINESTRING;
	}

	void c_geo_line_string::copy_from(const c_geometry *p)
	{
		c_geo_linear_obj::copy_from(p);

		const c_geo_line_string *p1 = dynamic_cast<const c_geo_line_string*>(p);
		if (!p1)return;

		m_line_width = p1->m_line_width;
		m_line_style_scale = p1->m_line_style_scale;
	}

	void c_geo_line_string::write_to(gd::AttributeSet& tab)const
	{
		c_geo_linear_obj::write_to(tab);

		tab.add_attr("line_width", m_line_width);
		tab.add_attr("line_style_scale", m_line_style_scale);
	}

	void c_geo_line_string::read_from(gd::AttributeSet& tab)
	{
		c_geo_linear_obj::read_from(tab);

		tab.read_attr("line_width", m_line_width);
		tab.read_attr("line_style_scale", m_line_style_scale);
	}

	void c_geo_line_string::paint(gd::GraphicsObject3D *pobj, double view_scale)const
	{
		pobj->begin_line_string(m_color, 0, m_line_width, false);
		for (int i = 0; i < m_shape_pts.size(); i++)
		{
			pobj->line_string(m_shape_pts[i]);
		}
		pobj->end();
		pobj->setAllColor(RGBF(0, 0, 255));
	}

	c_geo_double_line::c_geo_double_line()
	{
		
	}


	c_geo_double_line::~c_geo_double_line()
	{
	}


	GeometryType c_geo_double_line::get_geometry_type()const
	{
		return GEO_DOUBLELINE;
	}

	void c_geo_double_line::paint(gd::GraphicsObject3D *pobj, double view_scale)const
	{
		pobj->begin_line_string(m_color, 0, m_line_width, false);
		for (int i = 0; i < m_shape_pts.size(); i++)
		{
			if (m_shape_pts[i].pen_code == gd::GeoVertex::code_start)
			{
				pobj->end();
				pobj->begin_line_string(m_color, 0, m_line_width, false);
			}
			pobj->line_string(m_shape_pts[i]);
		}
		pobj->end();
	}

	void c_geo_double_line::get_assist_vertexs(gd::vertex_array& pts)
	{
		gd::vertex_array pts1;
		get_vertexs(pts1);

		int start_pos = -1;
		for (int i = 0; i < pts1.size(); i++)
		{
			if (i != 0 && start_pos == -1 && pts1[i].pen_code == gd::GeoVertex::code_start)
			{
				start_pos = i;
				pts.push_back(pts1[i]);
			}
			else if (start_pos>=0)
			{
				pts.push_back(pts1[i]);
			}
		}
	}

	void c_geo_double_line::get_assist_shape_pts(gd::vertex_array& pts)
	{
		gd::vertex_array pts1;
		get_vertexs(pts1);

		int start_pos = -1;
		for (int i = 0; i < pts1.size(); i++)
		{
			if (i != 0 && start_pos == -1 && pts1[i].pen_code == gd::GeoVertex::code_start)
			{
				start_pos = i;
				pts.push_back(pts1[i]);
			}
			else if (start_pos >= 0)
			{
				pts.push_back(pts1[i]);
			}
		}
	}
}
