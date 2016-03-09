#pragma once

#include "c_geometry.h"
#include "c_geo_line.h"

namespace gis
{
	class GOODDRAW_API c_geo_point : public c_geometry
	{
	public:
		c_geo_point();
		virtual ~c_geo_point();

		virtual GeometryType get_geometry_type()const;
		virtual int get_vertex_num()const;
		virtual bool set_vertexs(const gd::GeoVertex* pts, int npt);
		virtual void get_vertexs(gd::vertex_array& pts)const;
		virtual bool is_valid()const;
		virtual gd::Rect3D getBound(double view_scale)const;

		virtual void copy_from(const c_geometry *p);
		virtual void write_to(gd::AttributeSet& tab)const;
		virtual void read_from(gd::AttributeSet& tab);
		virtual void paint(gd::GraphicsObject3D *pobj, double view_scale)const;

	protected:
		gd::GeoVertex m_vertex;
		float m_angle;
		float m_size;
		float m_xscale, m_yscale;
	};


	class GOODDRAW_API c_geo_multi_point : public c_geometry
	{
	public:
		c_geo_multi_point();
		virtual ~c_geo_multi_point();

		virtual GeometryType get_geometry_type()const;
		virtual int get_vertex_num()const;
		virtual bool set_vertexs(const gd::GeoVertex* pts, int npt);
		virtual void get_vertexs(gd::vertex_array& pts)const;
		virtual bool is_valid()const;
		virtual gd::Rect3D getBound(double view_scale)const;

		virtual void copy_from(const c_geometry *p);
		virtual void write_to(gd::AttributeSet& tab)const;
		virtual void read_from(gd::AttributeSet& tab);
		virtual void paint(gd::GraphicsObject3D *pobj, double view_scale)const;

		void get_shape_rects(c_geo_linear_obj::shape_rect_array& rects)const;

	protected:
		gd::vertex_array m_vertexs;
		c_geo_linear_obj::shape_rect_array m_shape_rects;
		float m_size;
		gd::Rect3D m_bound;
	};
}