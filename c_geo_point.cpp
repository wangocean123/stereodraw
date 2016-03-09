#include "stdafx.h"
#include "c_geo_point.h"
#include "geo_algorithom.h"

namespace gis
{
	c_geo_point::c_geo_point()
	{
		m_angle = 0;
		m_size = 5;
		m_xscale = m_yscale = 1;
	}

	c_geo_point::~c_geo_point()
	{

	}

	GeometryType c_geo_point::get_geometry_type()const
	{
		return GEO_POINT;
	}

	int c_geo_point::get_vertex_num()const
	{
		return 1;
	}

	bool c_geo_point::set_vertexs(const gd::GeoVertex* pts, int npt)
	{
		if (npt > 0)
		{
			m_vertex = pts[0];
			return true;
		}
		else
		{
			return false;
		}
	}

	void c_geo_point::get_vertexs(gd::vertex_array& pts)const
	{
		pts.push_back(m_vertex);
	}

	bool c_geo_point::is_valid()const
	{
		return true;
	}

	gd::Rect3D c_geo_point::getBound(double view_scale)const
	{
		gd::Rect3D rect;
		rect = gd::rect_from_center_3<double>(m_vertex, geo_api::get_toler_xy(), geo_api::get_toler_xy(), geo_api::get_toler_xy());

		double size = view_scale*m_size;
		rect.inflate(size, size, 0);
		return rect;
	}

	void c_geo_point::copy_from(const c_geometry *p)
	{
		c_geometry::copy_from(p);

		const c_geo_point *p1 = dynamic_cast<const c_geo_point*>(p);
		if (!p1)return;

		m_vertex = p1->m_vertex;
		m_angle = p1->m_angle;
		m_size = p1->m_size;
		m_xscale = p1->m_xscale;
		m_yscale = p1->m_yscale;
	}

	void c_geo_point::write_to(gd::AttributeSet& tab)const
	{
		c_geometry::write_to(tab);

		tab.add_attr("angle",m_angle);
		tab.add_attr("point_size", m_size);
		tab.add_attr("xscale", m_xscale);
		tab.add_attr("yscale", m_yscale);
	}

	void c_geo_point::read_from(gd::AttributeSet& tab)
	{
		c_geometry::read_from(tab);

		tab.read_attr("angle", m_angle);
		tab.read_attr("point_size", m_size);
		tab.read_attr("xscale", m_xscale);
		tab.read_attr("yscale", m_yscale);
	}

	void c_geo_point::paint(gd::GraphicsObject3D *pobj, double view_scale)const
	{
		pobj->point(m_vertex, m_color, m_size, true, false);
	}

	c_geo_multi_point::c_geo_multi_point()
	{
		m_size = 0;
	}

	c_geo_multi_point::~c_geo_multi_point()
	{

	}

	GeometryType c_geo_multi_point::get_geometry_type()const
	{
		return GEO_MULTIPOINT;
	}

	int c_geo_multi_point::get_vertex_num()const
	{
		return 1;
	}

	bool c_geo_multi_point::set_vertexs(const gd::GeoVertex* pts, int npt)
	{
		if (npt > 0)
		{
			vector_push(m_vertexs, pts, npt);
			c_geo_linear_obj::get_shape_rects(m_vertexs, m_shape_rects);
			return true;
		}
		else
		{
			return false;
		}
	}

	void c_geo_multi_point::get_vertexs(gd::vertex_array& pts)const
	{
		pts = m_vertexs;
	}

	bool c_geo_multi_point::is_valid()const
	{
		return m_vertexs.size()>0;
	}

	gd::Rect3D c_geo_multi_point::getBound(double view_scale)const
	{
		gd::Rect3D rect = create_rect3d_from_points(m_vertexs.data(), m_vertexs.size());

		double size = view_scale*m_size;
		rect.inflate(size, size, 0);

		return rect;
	}

	void c_geo_multi_point::copy_from(const c_geometry *p)
	{
		c_geometry::copy_from(p);

		const c_geo_multi_point *p1 = dynamic_cast<const c_geo_multi_point*>(p);
		if (!p1)return;

		m_size = p1->m_size;
	}

	void c_geo_multi_point::write_to(gd::AttributeSet& tab)const
	{
		c_geometry::write_to(tab);

		tab.add_attr("point_size", m_size);
	}

	void c_geo_multi_point::read_from(gd::AttributeSet& tab)
	{
		c_geometry::read_from(tab);

		tab.read_attr("point_size", m_size);
	}

	void c_geo_multi_point::paint(gd::GraphicsObject3D *pobj, double view_scale)const
	{
		pobj->begin_point_string(m_size, true, false);
		for (int i = 0; i < m_vertexs.size(); i++)
		{
			pobj->point(m_vertexs[i], m_color, m_size, true, false);
		}
		pobj->end();
	}

	void c_geo_multi_point::get_shape_rects(c_geo_linear_obj::shape_rect_array& rects)const
	{
		rects = m_shape_rects;
	}
}