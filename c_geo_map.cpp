#include "StdAfx.h"
#include "c_geo_map.h"


namespace gis
{
	c_geo_layer::c_geo_layer()
	{

	}


	c_geo_layer::~c_geo_layer()
	{

	}


	const char* c_geo_layer::get_name()const
	{
		return m_name.c_str();
	}

	void c_geo_layer::set_name(const char *name)
	{
		m_name = name;
	}

	const char* c_geo_layer::get_code()const
	{
		return m_code.c_str();
	}

	void c_geo_layer::set_code(const char* code)
	{
		m_code = code;
	}

	long c_geo_layer::get_id()const
	{
		return m_id;
	}

	void c_geo_layer::set_id(long id)
	{
		m_id = id;
	}

	ulong c_geo_layer::get_color()const
	{
		return m_color;
	}

	void c_geo_layer::set_color(ulong color)
	{
		m_color = color;
	}


	bool c_geo_layer::is_visible()const
	{
		return m_visible;
	}

	bool c_geo_layer::is_locked()const
	{
		return m_locked;
	}

	void c_geo_layer::set_visible(bool visible)
	{
		m_visible = visible;
	}

	void c_geo_layer::set_locked(bool locked)
	{
		m_locked = locked;
	}

	void c_geo_layer::read_from(gd::AttributeSet& tab)
	{
		tab.read_attr("name", m_name);
		tab.read_attr("id", m_id);
		tab.read_attr("code", m_code);
		tab.read_attr("color", m_color);
		tab.read_attr("visible", m_visible);
		tab.read_attr("locked", m_locked);
	}

	void c_geo_layer::write_to(gd::AttributeSet& tab)const
	{
		tab.add_attr("name", m_name);
		tab.add_attr("id", m_id);
		tab.add_attr("code", m_code);
		tab.add_attr("color", (long)m_color);
		tab.add_attr("visible", m_visible);
		tab.add_attr("locked", m_locked);
	}

	c_geo_layer *c_geo_layer::clone()const
	{
		c_geo_layer *pnew = new c_geo_layer();
		pnew->copy_from(this);

		return pnew;
	}

	void c_geo_layer::copy_from(const c_geo_layer *player)
	{
		m_name = player->m_name;
		m_code = player->m_code;
		m_id = player->m_id;
		m_color = player->m_color;
		m_visible = player->m_visible;
		m_locked = player->m_locked;
		m_deleted = player->m_deleted;
	}

	bool c_geo_layer::is_deleted()const
	{
		return m_deleted;
	}

	void c_geo_layer::set_deleted(bool bdeleted)
	{
		m_deleted = bdeleted;
	}


	c_geo_map::c_geo_map()
	{
		m_scale = 0;
		memset(m_bound_pts, 0, sizeof(m_bound_pts));
	}


	c_geo_map::~c_geo_map()
	{
	}
	
	long c_geo_map::get_scale()const
	{
		return m_scale;
	}

	void c_geo_map::set_scale(long scale)
	{
		m_scale = scale;
	}


	void c_geo_map::get_bounds(gd::Point3D pts[4])const
	{
		memcpy(pts, m_bound_pts, sizeof(m_bound_pts));
	}

	void c_geo_map::set_bounds(gd::Point3D pts[4])
	{
		memcpy(m_bound_pts, pts, sizeof(m_bound_pts));
	}


	const c_geo_layer* c_geo_map::add_layer(const char *name)
	{
		const c_geo_layer *player = get_layer_by_name(name);
		if (player)
		{
			return player;
		}

		c_geo_layer *pnew_layer = new c_geo_layer();
		pnew_layer->set_name(name);
		m_layers.push_back(pnew_layer);
		return pnew_layer;
	}

	void c_geo_map::add_layer(c_geo_layer *player)
	{
		for (int i = 0; i < m_layers.size(); i++)
		{
			if (m_layers[i] == player)
			{
				player->set_deleted(true);
				return;
			}
		}
		m_layers.push_back(player);
		m_map_layer_ids[player->get_id()] = player;
	}

	c_geo_layer* c_geo_map::add_new_layer(const char *name)
	{
		c_geo_layer *player = get_layer_by_name(name);
		if(player)
			return player;

		player = new gis::c_geo_layer();
		player->set_id(get_new_layer_id());
		player->set_name(name);
		add_layer(player);

		return player;
	}

	void c_geo_map::del_layer(c_geo_layer *player)
	{
		for (int i = 0; i < m_layers.size(); i++)
		{
			if (m_layers[i] == player)
			{
				player->set_deleted(false);
				return;
			}
		}
	}

	int c_geo_map::get_layer_count()const
	{
		return m_layers.size();
	}

	c_geo_layer *c_geo_map::get_layer_by_index(int index)const
	{
		if (index >= 0 && index < m_layers.size())
		{
			return m_layers[index];
		}
		return NULL;
	}

	c_geo_layer *c_geo_map::get_layer_by_id(long id)const
	{
		std::map<long, c_geo_layer*>::const_iterator it = m_map_layer_ids.find(id);
		if (it == m_map_layer_ids.end())
		{
			return NULL;
		}
		else
		{
			return it->second;
		}
	}

	c_geo_layer *c_geo_map::get_layer_by_name(const char *name)const
	{
		for (int i = 0; i < m_layers.size(); i++)
		{
			if (_stricmp(m_layers[i]->get_name(), name) == 0)
			{
				return m_layers[i];
			}
		}
		return NULL;
	}


	long c_geo_map::get_new_layer_id()const
	{
		long id = -1;
		for (int i = 0; i < m_layers.size(); i++)
		{
			int id1 = m_layers[i]->get_id();
			if (id < id1)id = id1;
		}
		return id+1;
	}


	void c_geo_map::add_geometry(c_geometry *pobj)
	{
		if(m_geos.find_sort(pobj,NULL))
		{
			pobj->set_deleted(false);
		}
		else
		{
			m_geos.add_sort_no_repeated(pobj);
		}
	}

	void c_geo_map::del_geometry(c_geometry *pobj)
	{
		if(m_geos.find_sort(pobj,NULL))
		{
			pobj->set_deleted(true);
		}
	}


	int c_geo_map::get_geometry_count()const
	{
		return m_geos.size();
	}

	const c_geometry *c_geo_map::get_geometry(int index)const
	{
		if (index >= 0 && index < m_geos.size())
		{
			return m_geos[index];
		}
		return NULL;
	}

	void c_geo_map::release_geometry(c_geometry *pobj)
	{
		if(pobj && m_geos.find_sort(pobj,NULL))
		{
			m_geos.remove_sort_no_repeated(pobj);
		}
		if(pobj)delete pobj;
	}

	void c_geo_map::release_geo_layer(c_geo_layer *player)
	{
		for(int i=m_layers.size()-1; i>=0; i--)
		{
			if(m_layers[i]==player)
			{
				m_layers.erase(m_layers.begin()+i);
			}
		}

		if(player)delete player;
	}

	void c_geo_map::clear()
	{
		for(int i=0; i<m_geos.size(); i++)
		{
			delete m_geos[i];
		}
		for(int i=0; i<m_layers.size(); i++)
		{
			delete m_layers[i];
		}
		m_geos.clear();
		m_layers.clear();
		m_map_layer_ids.clear();
	}
}