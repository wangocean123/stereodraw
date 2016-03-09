#pragma once
#include "c_geometry.h"
#include "c_geo_line.h"

namespace gis
{
	class GOODDRAW_API c_geo_polygon : public c_geo_linear_obj
	{
	public:
		c_geo_polygon();
		virtual ~c_geo_polygon();

		virtual GeometryType get_geometry_type()const;

		virtual void copy_from(const c_geometry *p);
		virtual void write_to(gd::AttributeSet& tab)const;
		virtual void read_from(gd::AttributeSet& tab);
		virtual void paint(gd::GraphicsObject3D *pobj, double view_scale)const;

		void set_solid_color(bool bsolid_color);
		bool is_solid_color()const;
		c_geo_linear_obj::shape_rect get_whole_shape_rect()const;

	protected:
		float m_cell_scale;
		float m_cell_angle;
		float m_space_scale;
		bool m_bsolid_color;
	};

	class GOODDRAW_API c_geo_multi_polygon : public c_geo_polygon
	{
	public:
		c_geo_multi_polygon();
		virtual ~c_geo_multi_polygon();

		virtual GeometryType get_geometry_type()const;	
		virtual void paint(gd::GraphicsObject3D *pobj, double view_scale)const;

		void add_polygon(gd::vertex_array& vertexs);
		int get_polygon_count()const;
		void get_polygon_vertexs(int index, gd::vertex_array& pts)const;
		void get_polygon_shape_pts(int index, gd::vertex_array& pts)const;
		bool is_contain_pt(gd::Point3D pt)const;

		static bool is_contain_pt(const gd::vertex_array& pts, gd::Point3D pt);
		static bool is_contain_pt(const gd::point3d_array& pts, const std::vector<int>& counts, gd::Point3D pt);
	};
}