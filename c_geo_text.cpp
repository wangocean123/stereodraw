#include "StdAfx.h"
#include "c_geo_text.h"


namespace gis
{

	c_geo_text::c_geo_text()
	{
		m_place_type = type_point_horiz;
	}


	c_geo_text::~c_geo_text()
	{
	}
	
	GeometryType c_geo_text::get_geometry_type()const
	{
		return GEO_TEXT;
	}

	int c_geo_text::get_vertex_num()const
	{
		return 1;
	}

	bool c_geo_text::set_vertexs(const gd::GeoVertex* pts, int npt)
	{
		if (npt > 0)
		{
			m_vertexs.clear();
			gd::vector_push<gd::GeoVertex>(m_vertexs, pts, npt);
			return true;
		}
		else
		{
			return false;
		}
	}

	void c_geo_text::get_vertexs(gd::vertex_array& pts)const
	{
		pts = m_vertexs;
	}

	bool c_geo_text::is_valid()const
	{
		return true;
	}

	gd::Rect3D c_geo_text::getBound(double view_scale)const
	{
		gd::Rect3D rect;
		return rect;
	}


	void c_geo_text::copy_from(const c_geometry *p)
	{
		c_geometry::copy_from(p);

		const c_geo_text *p1 = dynamic_cast<const c_geo_text*>(p);
		if (!p1)return;

		m_text = p1->m_text;
		textStyle = p1->textStyle;
		m_vertexs = p1->m_vertexs;
		m_place_type = p1->m_place_type;
	}


	void c_geo_text::write_to(gd::AttributeSet& tab)const
	{
		c_geometry::write_to(tab);		

		tab.add_attr("text_content", m_text);

		tab.add_attr("text_height", textStyle.m_height);
		tab.add_attr("text_width_scale", textStyle.m_width_scale);
		tab.add_attr("text_weight", textStyle.m_weight);
		tab.add_attr("text_bunderline", textStyle.m_bunderline);
		tab.add_attr("text_bstrikeOut", textStyle.m_bstrikeOut);
		tab.add_attr("text_angle", textStyle.m_text_angle);
		tab.add_attr("text_align_type", textStyle.m_align_type);
		tab.add_attr("text_incline_angle", textStyle.m_incline_angle);
		tab.add_attr("text_char_space_scale", textStyle.m_char_sapce_scale);
		tab.add_attr("text_line_space_scale", textStyle.m_line_space_scale);

		tab.add_attr("place_type", m_place_type);
	}


	void c_geo_text::read_from(gd::AttributeSet& tab)
	{
		c_geometry::read_from(tab);

		tab.read_attr("text_content", m_text);

		tab.read_attr("text_height", textStyle.m_height);
		tab.read_attr("text_width_scale", textStyle.m_width_scale);
		tab.read_attr("text_weight", textStyle.m_weight);
		tab.read_attr("text_bunderline", textStyle.m_bunderline);
		tab.read_attr("text_bstrikeOut", textStyle.m_bstrikeOut);
		tab.read_attr("text_angle", textStyle.m_text_angle);
		tab.read_attr("text_align_type", textStyle.m_align_type);
		tab.read_attr("text_incline_angle", textStyle.m_incline_angle);
		tab.read_attr("text_char_space_scale", textStyle.m_char_sapce_scale);
		tab.read_attr("text_line_space_scale", textStyle.m_line_space_scale);

		tab.read_attr("place_type", m_place_type);
	}


	void c_geo_text::set_text_style(const gd::TextStyle& style)
	{
		textStyle = style;
	}

	void c_geo_text::get_text_style(gd::TextStyle& style)const
	{
		style = textStyle;
	}

	void c_geo_text::set_place_type(int type)
	{
		m_place_type = type;
	}

	int c_geo_text::get_place_type()const
	{
		return m_place_type;
	}

}