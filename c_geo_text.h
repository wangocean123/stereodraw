#pragma once
#include "c_geometry.h"
#include "TextStyle.h"

namespace gis
{

	class GOODDRAW_API c_geo_text : public c_geometry
	{
	public:
		enum e_place_type
		{
			type_point_horiz = 0,
			type_point_vertic = 1,
			type_line_horiz = 2,
			type_line_vertic = 3
		};
		c_geo_text();
		virtual ~c_geo_text();

		virtual GeometryType get_geometry_type()const;
		virtual int get_vertex_num()const;
		virtual bool set_vertexs(const gd::GeoVertex* pts, int npt);
		virtual void get_vertexs(gd::vertex_array& pts)const;
		virtual bool is_valid()const;
		virtual gd::Rect3D getBound(double view_scale)const;

		virtual void copy_from(const c_geometry *p);
		virtual void write_to(gd::AttributeSet& tab)const;
		virtual void read_from(gd::AttributeSet& tab);

		void set_text_style(const gd::TextStyle& style);
		void get_text_style(gd::TextStyle& style)const;

		void set_place_type(int type);
		int get_place_type()const;
		
	protected:
		gd::vertex_array m_vertexs;
		gd::TextStyle textStyle;
		std::string m_text;
		int m_place_type;
	};

}

