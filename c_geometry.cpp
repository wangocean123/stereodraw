#include "StdAfx.h"
#include "c_geometry.h"
#include "c_geo_point.h"
#include "c_geo_line.h"
#include "c_geo_polygon.h"
#include "c_geo_text.h"
#include <time.h>

namespace gis
{
	geo_id::geo_id()
	{
		m_id = 0;
	}

	geo_id::geo_id(const geo_id& id)
	{
		m_id = id.m_id;
	}

	geo_id::geo_id(__int64 id)
	{
		m_id = id;
	}

	geo_id::~geo_id()
	{

	}

	geo_id geo_id::New()
	{
		__int64 t = time(NULL);
		static int a = GetTickCount();

		return ((t << 32) + a++);
	}

	geo_id::operator __int64()const
	{
		return m_id;
	}

	c_geometry::user_data::user_data()
	{
		ndata[0] = 0;
		ndata[1] = 0;
		pointer = NULL;
	}

	c_geometry::user_data::user_data(int i1, int i2)
	{
		ndata[0] = i1;
		ndata[1] = i2;
	}

	c_geometry::user_data::user_data(void *p)
	{
		pointer = p;
	}

	c_geometry::c_geometry()
	{
		m_layer_id = 0;
		m_color = 0xff0000ff;
		m_deleted = false;
	}


	c_geometry::~c_geometry()
	{
	}


	GeometryType c_geometry::get_geometry_type()const
	{
		return GEO_NULL;
	}

	int c_geometry::get_vertex_num()const
	{
		return 0;
	}

	bool c_geometry::set_vertexs(const gd::GeoVertex* pts, int npt)
	{
		return false;
	}

	void c_geometry::get_vertexs(gd::vertex_array& pts)const
	{

	}

	bool c_geometry::is_valid()const
	{
		return false;
	}

	gd::Rect3D c_geometry::getBound(double view_scale)const
	{
		return gd::Rect3D();
	}

	ulong c_geometry::get_color()const
	{
		return m_color;
	}

	void c_geometry::set_color(ulong color)
	{
		m_color = color;
	}

	std::string c_geometry::get_symbol_name()const
	{
		return m_symbol_name;
	}

	void c_geometry::set_symbol_name(const std::string& sym_name)
	{
		m_symbol_name = sym_name;
	}


	c_geometry::user_data c_geometry::get_userdata()const
	{
		return m_user_data;
	}

	void c_geometry::set_userdata(c_geometry::user_data data)
	{
		m_user_data = data;
	}

	std::string c_geometry::get_user_flag()const
	{
		return m_str_user_flag;
	}


	void c_geometry::set_user_flag(const std::string& flag)
	{
		m_str_user_flag = flag;
	}

	void c_geometry::write_to(gd::AttributeSet& tab)const
	{
		tab.add_attr("geo_type", (long)get_geometry_type());

		gd::vertex_array pts;
		get_vertexs(pts);
		tab.add_attr("vertexs", pts);

		tab.add_attr("color", (long)m_color);
		tab.add_attr("symbol_name", m_symbol_name);
		tab.add_attr("user_flag", m_str_user_flag);
	}

	void c_geometry::read_from(gd::AttributeSet& tab)
	{
		gd::Variant var;

		gd::vertex_array pts;
		if (tab.read_attr("vertexs", pts))
		{
			set_vertexs(pts.data(), pts.size());
		}

		tab.read_attr("color", m_color);
		tab.read_attr("symbol_name", m_symbol_name);
		tab.read_attr("user_flag", m_str_user_flag);
	}

	void c_geometry::get_shape_pts(gd::vertex_array& pts)const
	{
		get_vertexs(pts);
	}

	c_geometry *c_geometry::clone()const
	{
		c_geometry *pnew = create_geometry(get_geometry_type());
		if (pnew == NULL)
			return NULL;

		pnew->copy_from(this);
		return pnew;
	}

	void c_geometry::copy_from(const c_geometry *p)
	{
		m_color = p->m_color;
		m_symbol_name = p->m_symbol_name;
		m_user_data = p->m_user_data;
		m_str_user_flag = p->m_str_user_flag;
		m_deleted = p->m_deleted;
	}

	void c_geometry::release_geometry(c_geometry *p)
	{
		if(p)delete p;
	}

	c_geometry *c_geometry::create_geometry(GeometryType type)
	{
		switch (type)
		{
		case gis::GEO_NULL:
			return NULL;
			break;
		case gis::GEO_POINT:
			return new c_geo_point();
			break;
		case gis::GEO_LINESTRING:
			return new c_geo_line_string();
			break;
		case gis::GEO_POLYGON:
			return new c_geo_polygon();
			break;
		case gis::GEO_MULTIPOINT:
			return new c_geo_multi_point();
			break;
		case gis::GEO_DOUBLELINE:
			return new c_geo_double_line();
			break;
		case gis::GEO_MULTIPOLYGON:
			return new c_geo_multi_polygon();
			break;
		case gis::GEO_TEXT:
			return new c_geo_text();
			break;
		default:
			break;
		}

		return NULL;
	}

	c_geometry *create_geometry_from_attr_table(gd::AttributeSet& tab)
	{
		gd::Variant var;
		if (!tab.get_attr_value("geo_type", var))
			return NULL;

		c_geometry *pgeo = c_geometry::create_geometry((GeometryType)(long)var);
		if (!pgeo)
			return NULL;

		pgeo->read_from(tab);
		return pgeo;
	}

	void c_geometry::create_new_id()
	{
		m_id = geo_id::New();
	}

	void c_geometry::set_id(geo_id id)
	{
		m_id = id;
	}

	geo_id c_geometry::get_id()
	{
		return m_id;
	}
	
	void c_geometry::paint(gd::GraphicsObject3D *pobj, double view_scale)const
	{

	}

	bool c_geometry::is_deleted()
	{
		return m_deleted;
	}

	void c_geometry::set_deleted(bool bdeleted)
	{
		m_deleted = bdeleted;
	}
}