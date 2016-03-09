#pragma once

#include "rect_def.h"
#include "point_def.h"
#include <vector>
#include "Variant.h"
#include "GraphicsObject.h"

namespace gis
{

	class GOODDRAW_API geo_id
	{
	public:
		geo_id();
		geo_id(const geo_id& id);
		geo_id(__int64 id);
		~geo_id();
		static geo_id New();
		operator __int64()const;
		__int64 m_id;
	};

	enum GeometryType {
		GEO_NULL = 0,
		/// a point
		GEO_POINT = 1,
		/// a linestring
		GEO_LINESTRING = 2,
		/// a polygon
		GEO_POLYGON = 3,
		/// a collection of points
		GEO_MULTIPOINT = 4,
		/// double line
		GEO_DOUBLELINE = 5,
		/// a collection of polygons
		GEO_MULTIPOLYGON = 6,
		/// text
		GEO_TEXT = 7
	};

	class GOODDRAW_API c_geometry
	{
	public:
		struct user_data
		{
			user_data();
			user_data(int i1, int i2=0);
			user_data(void *p);
			union
			{
				int ndata[2];
				void *pointer;
			};
		};

		c_geometry();
		virtual ~c_geometry();

		virtual GeometryType get_geometry_type()const;
		virtual int get_vertex_num()const;
		virtual bool set_vertexs(const gd::GeoVertex* pts, int npt);
		virtual void get_vertexs(gd::vertex_array& pts)const;
		virtual bool is_valid()const;
		virtual gd::Rect3D getBound(double view_scale)const;

		virtual ulong get_color()const;
		virtual void set_color(ulong color);
		virtual std::string get_symbol_name()const;
		virtual void set_symbol_name(const std::string& sym_name);

		//not saved to file
		virtual c_geometry::user_data get_userdata()const;
		virtual void set_userdata(c_geometry::user_data data);

		virtual std::string get_user_flag()const;
		virtual void set_user_flag(const std::string& flag);

		virtual void copy_from(const c_geometry *p);
		virtual void write_to(gd::AttributeSet& tab)const;
		virtual void read_from(gd::AttributeSet& tab);
		virtual void get_shape_pts(gd::vertex_array& pts)const;
		virtual void paint(gd::GraphicsObject3D *pobj, double view_scale)const;

		c_geometry *clone()const;
		static c_geometry *create_geometry(GeometryType type);
		static c_geometry *create_geometry_from_attr_table(gd::AttributeSet& tab);
		static void release_geometry(c_geometry *p);

		void create_new_id();
		void set_id(geo_id id);
		geo_id get_id();

		long get_layer_id();
		void set_layer_id(long id);

		bool is_deleted();
		void set_deleted(bool bdeleted);

	protected:
		geo_id m_id;
		long m_layer_id;
		ulong m_color;
		std::string m_symbol_name;
		c_geometry::user_data m_user_data;
		std::string m_str_user_flag;
		bool m_deleted;
	};

	typedef std::vector<c_geometry*> geometry_array;
	typedef std::vector<const c_geometry*> const_geometry_array;
}