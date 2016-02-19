#include "StdAfx.h"
#include "c_workspace.h"

namespace gis
{
	c_workspace::c_workspace()
	{
		m_ncur_map_index = 0;
	}


	c_workspace::~c_workspace()
	{
	}

	void c_workspace::add_geo_map(c_geo_map *pmap)
	{
		int i = 0;
		for (i = 0; i < m_maps.size(); i++)
		{
			if (m_maps[i] == pmap)
			{
				break;
			}
		}

		if (i >= m_maps.size())
		{
			m_maps.push_back(pmap);
		}
	}

	void c_workspace::del_geo_map(const c_geo_map *pmap)
	{
		int i = 0;
		for (i = 0; i < m_maps.size(); i++)
		{
			if (m_maps[i] == pmap)
			{
				m_maps.erase(m_maps.begin() + i);
				break;
			}
		}
	}


	void c_workspace::add_geometry(c_geometry *pobj)
	{
		c_geo_map *pmap = get_active_map();
		if (pmap)
		{
			pmap->add_geometry(pobj);
			m_tree.insert(pobj);
		}
	}

	void c_workspace::del_geometry(const c_geometry *pobj)
	{
		c_geo_map *pmap = get_active_map();
		if (pmap)
		{
			pmap->del_geometry((c_geometry*)pobj);
			m_tree.remove(pobj);
		}
	}

	void c_workspace::replace_geometry(const c_geometry *pold, c_geometry *pnew)
	{
		c_geo_map *pmap = get_active_map();
		if (pmap)
		{
			m_tree.remove(pold);

			c_geometry *p2 = (c_geometry*)pold;
			p2->copy_from(pnew);
			m_tree.insert(p2);
		}
	}

	void c_workspace::release_geometry(c_geometry *pobj)
	{
		delete pobj;
	}


	void c_workspace::add_geo_layer(c_geo_layer *player)
	{
		c_geo_map *pmap = get_active_map();
		if (pmap)
		{
			pmap->add_layer(player);
		}
	}

	void c_workspace::del_geo_layer(const c_geo_layer *player)
	{
		c_geo_map *pmap = get_active_map();
		if (pmap)
		{
			pmap->del_layer((c_geo_layer*)player);
		}
	}

	void c_workspace::modify_layer(const c_geo_layer *pold, c_geo_layer *pnew)
	{
		c_geo_map *pmap = get_active_map();
		if (pmap)
		{
			c_geo_layer *p2 = (c_geo_layer*)pold;
			p2->copy_from(pnew);		
		}
	}

	void c_workspace::release_geo_layer(c_geo_layer *player)
	{
		if (player)
		{
			delete player;
		}
	}


	void c_workspace::query_rect(StereoDraw::rect_3d rc, const StereoDraw::matrix4d *m, gis::const_geometry_array& arr)
	{
		m_tree.query_rect(rc, m, arr);
	}

	void c_workspace::query_nearest(StereoDraw::point_3d pt, double r, const StereoDraw::matrix4d* m, gis::const_geometry_array& arr, std::vector<double>* dis_arr)
	{
		m_tree.query_nearest(pt, r, m, arr, dis_arr);
	}

	void c_workspace::query_nearest(StereoDraw::point_3d pt, StereoDraw::rect_3d rect, const StereoDraw::matrix4d* m, gis::const_geometry_array& arr, std::vector<double>* dis_arr)
	{
		m_tree.query_nearest(pt, rect, m, arr, dis_arr);
	}


	void c_workspace::set_active_map(c_geo_map *pmap)
	{
		for (int i = 0; i < m_maps.size(); i++)
		{
			if (pmap == m_maps[i])
			{
				m_ncur_map_index = i;
			}
		}
	}

	c_geo_map *c_workspace::get_active_map()
	{
		if (m_ncur_map_index >= 0 && m_ncur_map_index < m_maps.size())
		{
			return m_maps[m_ncur_map_index];
		}
		return NULL;
	}

	int c_workspace::get_map_count()
	{
		return m_maps.size();
	}

	c_geo_map *c_workspace::get_geo_map(int index)
	{
		if (index >= 0 && index < m_maps.size())
		{
			return m_maps[index];
		}
		return NULL;
	}

	
	void c_workspace::read_lock()
	{
		m_lock.readLock();
	}

	void c_workspace::read_unlock()
	{
		m_lock.readUnlock();
	}

	bool c_workspace::try_read_lock()
	{
		return m_lock.tryReadLock();
	}

	void c_workspace::write_lock()
	{
		m_lock.writeLock();
	}

	void c_workspace::write_unlock()
	{
		m_lock.writeUnlock();
	}
}