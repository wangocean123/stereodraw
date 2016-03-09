#include "StdAfx.h"
#include "c_geo_db.h"

namespace gis
{
	c_geo_db::c_geo_db()
	{
	}


	c_geo_db::~c_geo_db()
	{
	}
	
	bool c_geo_db::open(const char *path)
	{
		return false;
	}

	bool c_geo_db::create(const char* path)
	{
		return false;
	}


	int c_geo_db::get_geometry_count()
	{
		return 0;
	}

	gis::c_geometry *c_geo_db::read_first_geometry()
	{
		return NULL;
	}

	gis::c_geometry *c_geo_db::read_next_geometry()
	{
		return NULL;
	}


	gis::c_geo_layer *c_geo_db::read_first_layer()
	{
		return NULL;
	}

	gis::c_geo_layer *c_geo_db::read_next_layer()
	{
		return NULL;
	}


	bool c_geo_db::write_geometry(gis::c_geometry *pobj)
	{
		return NULL;
	}

	bool c_geo_db::write_layer(gis::c_geo_layer *pobj)
	{
		return NULL;
	}


	bool c_geo_db::read_map_info(gis::c_geo_map *map)
	{
		return NULL;
	}

	void c_geo_db::write_map_info(gis::c_geo_map *map)
	{
		return;
	}

}
