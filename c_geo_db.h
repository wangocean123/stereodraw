#pragma once
#include "c_geo_map.h"


namespace gis
{

	class GOODDRAW_API c_geo_db
	{
	public:
		c_geo_db();
		virtual ~c_geo_db();

		bool open(const char *path);
		bool create(const char* path);

		int get_geometry_count();
		gis::c_geometry *read_first_geometry();
		gis::c_geometry *read_next_geometry();

		gis::c_geo_layer *read_first_layer();
		gis::c_geo_layer *read_next_layer();

		bool write_geometry(gis::c_geometry *pobj);
		bool write_layer(gis::c_geo_layer *pobj);

		bool read_map_info(gis::c_geo_map *map);
		void write_map_info(gis::c_geo_map *map);

	protected:

	};

}
