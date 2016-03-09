#pragma once

#include "c_geo_map.h"
#include "c_octree.h"
#include "readwrite_lock.h"

namespace gis
{

	class GOODDRAW_API c_workspace
	{
	public:
		c_workspace();
		virtual ~c_workspace();

		void add_geo_map(c_geo_map *pmap);
		void del_geo_map(const c_geo_map *pmap);

		void add_geometry(c_geometry *pobj);
		void del_geometry(const c_geometry *pobj);
		void replace_geometry(const c_geometry *pold, c_geometry *pnew);

		void add_geo_layer(c_geo_layer *player);
		void del_geo_layer(const c_geo_layer *player);
		void modify_layer(const c_geo_layer *pold, c_geo_layer *pnew);

		void query_rect(gd::Rect3D rc, const gd::matrix4d *m, gis::const_geometry_array& arr);
		void query_nearest(gd::Point3D pt, double r, const gd::matrix4d* m, gis::const_geometry_array& arr, std::vector<double>* dis_arr = NULL);
		void query_nearest(gd::Point3D pt, gd::Rect3D rect, const gd::matrix4d* m, gis::const_geometry_array& arr, std::vector<double>* dis_arr = NULL);

		void set_active_map(c_geo_map *pmap);
		c_geo_map *get_active_map();

		int get_map_count();
		c_geo_map *get_geo_map(int index);

		void read_lock();
		void read_unlock();
		bool try_read_lock();
		void write_lock();
		void write_unlock();
	protected:
		void clear();

	protected:
		c_octree m_tree;

		std::vector<c_geo_map*> m_maps;
		int m_ncur_map_index;
		gd::readwrite_lock m_lock;
	};


	class GOODDRAW_API c_temp_active_map
	{
	public:
		c_temp_active_map(c_workspace *pws, c_geo_map *pmap);
		virtual ~c_temp_active_map();

		void restore();

	private:
		c_workspace *m_pws;
		c_geo_map *m_pmap, *m_pmap_old;
	};

}