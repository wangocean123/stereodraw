#include "StdAfx.h"
#include "c_geo_polygon.h"
#include "geo_algorithom.h"

namespace gis
{
	c_geo_polygon::c_geo_polygon()
	{
		m_cell_scale = 1.0;
		m_cell_angle = 0;
		m_space_scale = 1.0;
		m_bsolid_color = true;
	}


	c_geo_polygon::~c_geo_polygon()
	{
	}

	GeometryType c_geo_polygon::get_geometry_type()const
	{
		return GEO_POLYGON;
	}

	void c_geo_polygon::copy_from(const c_geometry *p)
	{
		c_geo_linear_obj::copy_from(p);

		const c_geo_polygon *p1 = dynamic_cast<const c_geo_polygon*>(p);
		if (!p1)return;

		m_cell_scale = p1->m_cell_scale;
		m_cell_angle = p1->m_cell_angle;
		m_space_scale = p1->m_space_scale;
	}

	void c_geo_polygon::write_to(gd::AttributeSet& tab)const
	{
		c_geo_linear_obj::write_to(tab);

		tab.add_attr("cell_scale", m_cell_scale);
		tab.add_attr("cell_anlge", m_cell_angle);
		tab.add_attr("space_scale", m_space_scale);
	}

	void c_geo_polygon::read_from(gd::AttributeSet& tab)
	{
		c_geo_linear_obj::read_from(tab);

		tab.read_attr("cell_scale", m_cell_scale);
		tab.read_attr("cell_anlge", m_cell_angle);
		tab.read_attr("space_scale", m_space_scale);
	}

	void c_geo_polygon::set_solid_color(bool bsolid_color)
	{
		m_bsolid_color = bsolid_color;
	}


	bool c_geo_polygon::is_solid_color()const
	{
		return m_bsolid_color;
	}

	c_geo_linear_obj::shape_rect c_geo_polygon::get_whole_shape_rect()const
	{
		shape_rect rect;
		rect.rect = getBound(1.0);
		rect.pvertexs = &m_shape_pts;
		rect.start_index = 0;
		rect.pt_count = m_shape_pts.size();
		return rect;
	}

	void c_geo_polygon::paint(gd::GraphicsObject3D *pobj, double view_scale)const
	{
		if (m_bsolid_color)
		{
			pobj->begin_polygon(m_color);
			for (int i = 0; i < m_shape_pts.size(); i++)
			{
				if (m_shape_pts[i].pen_code == gd::GeoVertex::code_start)
				{
					pobj->end();
					pobj->begin_polygon(m_color);
				}
				pobj->polygon(m_shape_pts[i]);
			}
			pobj->end();
		}
		else
		{
			pobj->begin_line_string(m_color,0,0,false);
			for (int i = 0; i < m_shape_pts.size(); i++)
			{
				if (m_shape_pts[i].pen_code == gd::GeoVertex::code_start)
				{
					pobj->end();
					pobj->begin_line_string(m_color, 0, 0, false);
				}
				pobj->polygon(m_shape_pts[i]);
			}
			pobj->end();
		}
	}
	
	c_geo_multi_polygon::c_geo_multi_polygon()
	{
	}


	c_geo_multi_polygon::~c_geo_multi_polygon()
	{
	}

	GeometryType c_geo_multi_polygon::get_geometry_type()const
	{
		return GEO_MULTIPOLYGON;
	}

	void c_geo_multi_polygon::add_polygon(gd::vertex_array& vertexs)
	{
		gd::vertex_array all;
		get_vertexs(all);

		all.insert(all.end(), vertexs.begin(), vertexs.end());
		set_vertexs(all.data(), all.size());
	}

	int c_geo_multi_polygon::get_polygon_count()const
	{
		int count = 1;
		for (int i = 1; i < m_shape_pts.size(); i++)
		{
			if (m_shape_pts[i].pen_code == gd::GeoVertex::code_start)
			{
				count++;
			}
		}
		return count;
	}


	void c_geo_multi_polygon::get_polygon_vertexs(int index, gd::vertex_array& pts)const
	{
		int start_pos = -1;
		if (index == 0)
		{
			start_pos = 0;
		}
		else
		{
			int count = 0;
			for (int i = 1; i < m_shape_pts.size(); i++)
			{
				if (m_shape_pts[i].pen_code == gd::GeoVertex::code_start)
				{
					count++;
					start_pos = i;
					if (count == index)break;
				}
			}
		}

		if (start_pos < 0)return;

		for (int i = start_pos; i < m_shape_pts.size(); i++)
		{
			if (i != start_pos && m_shape_pts[i].pen_code == gd::GeoVertex::code_start)
			{
				break;
			}
			else if (m_shape_pts[i].pen_code != gd::GeoVertex::code_none)
			{
				pts.push_back(m_shape_pts[i]);
			}
		}
	}


	void c_geo_multi_polygon::get_polygon_shape_pts(int index, gd::vertex_array& pts)const
	{
		int start_pos = -1;
		if (index == 0)
		{
			start_pos = 0;
		}
		else
		{
			int count = 0;
			for (int i = 1; i < m_shape_pts.size(); i++)
			{
				if (m_shape_pts[i].pen_code == gd::GeoVertex::code_start)
				{
					count++;
					start_pos = i;
					if (count == index)break;
				}
			}
		}

		if (start_pos < 0)return;

		for (int i = start_pos; i < m_shape_pts.size(); i++)
		{
			if (i!=start_pos && m_shape_pts[i].pen_code == gd::GeoVertex::code_start)
			{
				break;
			}
			else
			{
				pts.push_back(m_shape_pts[i]);
			}
		}
	}

	void c_geo_multi_polygon::paint(gd::GraphicsObject3D *pobj, double view_scale)const
	{
		if (m_bsolid_color)
		{
			pobj->begin_polygon(m_color);
			for (int i = 0; i < m_shape_pts.size(); i++)
			{
				if (m_shape_pts[i].pen_code == gd::GeoVertex::code_start)
				{
					pobj->end();
					pobj->begin_polygon(m_color);
				}
				pobj->line_string(m_shape_pts[i]);
			}
			pobj->end();
		}
		else
		{
			pobj->begin_line_string(m_color, 0, 0, false);
			for (int i = 0; i < m_shape_pts.size(); i++)
			{
				if (m_shape_pts[i].pen_code == gd::GeoVertex::code_start)
				{
					pobj->end();
					pobj->begin_line_string(m_color, 0, 0, false);
				}
				pobj->line_string(m_shape_pts[i]);
			}
			pobj->end();
		}
	}

	bool c_geo_multi_polygon::is_contain_pt(gd::Point3D pt)const
	{
		return is_contain_pt(m_shape_pts, pt);
	}
	
	bool c_geo_multi_polygon::is_contain_pt(const gd::vertex_array& pts, gd::Point3D pt)
	{
		int count = 0, npt = pts.size(), start = 0;
		for (int i = 1; i < npt; i++)
		{
			if (i==(npt-1) || pts[i].pen_code == gd::GeoVertex::code_start)
			{
				if (i - start >= 3 && geo_api::is_pt_in_polygon((gd::GeoVertex)pt, pts.data() + start, i - start) == 1)
				{
					count++;
				}
			}
		}
		return (count%2)!=0;
	}

	bool c_geo_multi_polygon::is_contain_pt(const gd::point3d_array& pts, const std::vector<int>& counts, gd::Point3D pt)
	{
		int count = 0, start = 0;
		for (int i = 0; i < counts.size(); i++)
		{
			if (counts[i] >= 3 && geo_api::is_pt_in_polygon(pt, pts.data() + start, counts[i]) == 1)
			{
				count++;
			}
			start += counts[i];
		}
		return (count % 2) != 0;
	}
}