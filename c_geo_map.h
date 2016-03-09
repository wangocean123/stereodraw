#pragma once

#include "c_geometry.h"
#include "Variant.h"
#include "point_def.h"
#include <map>
#include "sorted_array.h"

namespace gis
{
	class GOODDRAW_API c_geo_layer
	{
	public:
		c_geo_layer();
		virtual ~c_geo_layer();

		const char* get_name()const;
		void set_name(const char *name);

		const char* get_code()const;
		void set_code(const char* code);

		long get_id()const;
		void set_id(long id);

		ulong get_color()const;
		void set_color(ulong color);

		bool is_visible()const;
		bool is_locked()const;
		void set_visible(bool visible);
		void set_locked(bool locked);

		void read_from(gd::AttributeSet& tab);
		void write_to(gd::AttributeSet& tab)const;

		c_geo_layer *clone()const;
		void copy_from(const c_geo_layer *player);

		bool is_deleted()const;
		void set_deleted(bool bdeleted);

	protected:
		std::string m_name;
		std::string m_code;
		long m_id;
		ulong m_color;
		bool m_visible;
		bool m_locked;
		bool m_deleted;
	};

	typedef std::vector<c_geo_layer*> layer_array;

	class GOODDRAW_API c_geo_map
	{
	public:
		c_geo_map();
		virtual ~c_geo_map();

		long get_scale()const;
		void set_scale(long scale);

		void get_bounds(gd::Point3D pts[4])const;
		void set_bounds(gd::Point3D pts[4]);

		const c_geo_layer* add_layer(const char *name);
		void add_layer(c_geo_layer *player);
		void del_layer(c_geo_layer *player);
		c_geo_layer* add_new_layer(const char *name);

		int get_layer_count()const;
		c_geo_layer *get_layer_by_index(int index)const;
		c_geo_layer *get_layer_by_id(long id)const;
		c_geo_layer *get_layer_by_name(const char *name)const;

		long get_new_layer_id()const;

		void add_geometry(c_geometry *pobj);
		void del_geometry(c_geometry *pobj);

		int get_geometry_count()const;
		const c_geometry *get_geometry(int index)const;

		void release_geometry(c_geometry *pobj);
		void release_geo_layer(c_geo_layer *player);
	protected:
		void clear();

	protected:
		long m_scale;
		gd::Point3D m_bound_pts[4];

		layer_array m_layers;
		std::map<long,c_geo_layer*> m_map_layer_ids;
		sorted_array<c_geometry*> m_geos;
	};

}