#pragma once
#include "c_geometry.h"

namespace gis
{

	class GOODDRAW_API c_geo_linear_obj : public c_geometry
	{
	public:
		struct shape_rect
		{
			shape_rect();
			gd::Rect3D rect;
			const gd::vertex_array *pvertexs;
			int start_index;
			int pt_count;
		};

		typedef std::vector<shape_rect> shape_rect_array;

		c_geo_linear_obj();
		virtual ~c_geo_linear_obj();

		virtual GeometryType get_geometry_type()const;
		virtual int get_vertex_num()const;
		virtual bool set_vertexs(const gd::GeoVertex* pts, int npt);
		virtual void get_vertexs(gd::vertex_array& pts)const;
		virtual bool is_valid()const;
		virtual gd::Rect3D getBound(double view_scale)const;

		virtual void copy_from(const c_geometry *p);
		virtual void write_to(gd::AttributeSet& tab)const;
		virtual void read_from(gd::AttributeSet& tab);

		virtual void get_shape_pts(gd::vertex_array& pts)const;
		virtual bool is_closed()const;
		virtual void ensure_closed(bool bclosed);
		virtual void paint(gd::GraphicsObject3D *pobj, double view_scale)const;

		void get_shape_rects(shape_rect_array& rects)const;
		int get_vertex_index(double shape_pos)const;

		static void get_shape_rects(const gd::vertex_array& shape_pts, shape_rect_array& rects);

	protected:
		gd::vertex_array m_shape_pts;
		std::vector<int> m_vertex_indexs;
		shape_rect_array m_shape_rects;
		gd::Rect3D m_bound;
	};

	class GOODDRAW_API c_geo_line_string : public c_geo_linear_obj
	{
	public:
		c_geo_line_string();
		virtual ~c_geo_line_string();

		virtual GeometryType get_geometry_type()const;
		virtual void copy_from(const c_geometry *p);
		virtual void write_to(gd::AttributeSet& tab)const;
		virtual void read_from(gd::AttributeSet& tab);
		virtual void paint(gd::GraphicsObject3D *pobj, double view_scale)const;

	protected:
		float m_line_width;
		float m_line_style_scale;
	};

	class GOODDRAW_API c_geo_double_line : public c_geo_line_string
	{
	public:
		c_geo_double_line();
		virtual ~c_geo_double_line();

		virtual GeometryType get_geometry_type()const;
		virtual void paint(gd::GraphicsObject3D *pobj, double view_scale)const;

		void get_assist_vertexs(gd::vertex_array& pts);
		void get_assist_shape_pts(gd::vertex_array& pts);
	};

}