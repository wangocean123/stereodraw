#include "StdAfx.h"
#include "GraphicsLayer.h"
#include <algorithm>
#include "PaintTextureD3D.h"
#include "PaintTextureGL.h"
#include "PaintTextureGDI.h"
#include "BitmapFunc.h"
#include "geo_algorithom.h"


namespace gd
{
	GraphicsLayer::vect_obj::vect_obj()
	{
		pobj = NULL;
		id = 0;
		visible = true;
	}

	bool GraphicsLayer::vect_obj::operator < (const vect_obj& b)
	{
		return (id < b.id);
	}

	GraphicsLayer::GraphicsLayer()
	{
		m_nframe_texture_index = -1;
		m_use_cache = true;
	}


	GraphicsLayer::~GraphicsLayer()
	{
		release_textures();
		deleteAllObjs();
	}

	void GraphicsLayer::set_texture_info(int nMB, int texture_type)
	{
		lock_data();
		release_textures();

		m_use_cache = true;
		if (nMB < 0)
		{
			m_use_cache = false;
			unlock_data();
			return;
		}
		SceneLayer::set_texture_info(nMB,texture_type);

		m_paint_board->begin_texture_operation(m_sz_texture.cx, m_sz_texture.cy);
		m_nframe_texture_index = m_paint_board->request_frame_texture();

		for (int i = 0; i < m_texture_items.size(); i++)
		{
			if (texture_type == SceneLayer::texture_d3d)
			{
				PaintTextureD3D *p = new PaintTextureD3D();
				p->set_device(m_paint_board->device_d3d());
				p->create(m_sz_texture.cx, m_sz_texture.cy);
				m_ptextures.push_back(p);
			}
			else if (texture_type == SceneLayer::texture_gl)
			{
				PaintTextureGL *p = new PaintTextureGL();
				p->set_device(m_paint_board->device_gl());
				p->create(m_sz_texture.cx, m_sz_texture.cy);
				m_ptextures.push_back(p);
			}
			else if (texture_type == SceneLayer::texture_gdi)
			{
				PaintTextureGDI *p = new PaintTextureGDI();
				p->set_device(m_paint_board->device_gdi());
				p->create(m_sz_texture.cx, m_sz_texture.cy);
				m_ptextures.push_back(p);
			}
		}

		m_paint_board->end_texture_operation();

		m_cache_global_coords.assign(m_texture_items.size(), quad_points());
		unlock_data();
	}

	static int compare_zoomrate_index(const void *p1, const void *p2)
	{
		const GraphicsLayer::zoomrate_index *p3 = (const GraphicsLayer::zoomrate_index*)p1;
		const GraphicsLayer::zoomrate_index *p4 = (const GraphicsLayer::zoomrate_index*)p2;
		if (p3->zoom == p4->zoom)
			return 0;
		else if (p3->zoom == 0.0f)
			return -1;
		else if (p4->zoom == 0.0f)
			return 1;
		else if (p3->zoom != p3->cur_zoom && p4->zoom == p4->cur_zoom)
			return -1;
		else if (p3->zoom == p3->cur_zoom && p4->zoom != p4->cur_zoom)
			return 1;
		else
		{
			double k1 = p3->zoom > p3->cur_zoom ? p3->zoom / p3->cur_zoom : p3->cur_zoom / p3->zoom;
			double k2 = p4->zoom > p4->cur_zoom ? p4->zoom / p4->cur_zoom : p4->cur_zoom / p4->zoom;
			if (k1 > k2)
				return -1;
			else if (k1 < k2)
				return 1;
			else if (p3->zoom > p3->cur_zoom)
				return 1;
			else
				return -1;
		}
	}


	void GraphicsLayer::paint()
	{
		if (!m_use_cache)
		{
			matrix4d matrix_old = m_paint_board->get_matrix();
			SizeI sz_old = m_paint_board->get_view_size();			

			Rect2D view_rect = Rect2D(0, 0, sz_old.cx, sz_old.cy);
			Point2D pts[4];
			get_rect_corners(view_rect, pts);
			transform_pts_by_matrix_2d(pts, 4, m_view_to_global_matrix);
			Rect2D view_rect2 = create_rect2d_from_points(pts, 4);

			lock_data();

			m_paint_board->set_matrix(m_global_to_view_matrix);

			double scale = sqrt(fabs(m_paint_board->get_matrix().Det()));

			for (std::list<vect_obj>::iterator it = m_objs.begin(); it != m_objs.end(); it++)
			{
				if (!it->visible)
					continue;
				if (view_rect2.is_intersect(it->pobj->getBound(scale)))
				{
					m_paint_board->draw_vectors(it->pobj);
				}
			}

			m_paint_board->set_matrix(matrix_old);

			unlock_data();

			return;
		}
		
		float cur_zoom = sqrt(fabs(m_global_to_cache_matrix.Det()));
		zoomrate_index *pbuf = new zoomrate_index[m_texture_items.size()];
		for (int i = 0; i < m_texture_items.size(); i++)
		{
			pbuf[i].index = i;
			pbuf[i].state = m_texture_items[i].state;
			pbuf[i].zoom = m_texture_items[i].zoom_rate;
			pbuf[i].cur_zoom = cur_zoom;
		}

		matrix4d global_to_view_matrix = m_global_to_view_matrix;

		std::qsort(pbuf, m_texture_items.size(), sizeof(zoomrate_index), compare_zoomrate_index);

		std::vector<texture_item> cache_items;

		for (int i = 0; i < m_texture_items.size(); i++)
		{
			if (pbuf[i].state == SceneLayer::state_paint)
			{
				cache_items.push_back(m_texture_items[pbuf[i].index]);
			}
		}

		SizeI sz_view = m_paint_board->get_view_size();
		Rect2I view_rect(0,0,sz_view.cx, sz_view.cy);
		Point3D pts[4], pts1[4], pts2[4];

		pts[0] = Point3D(view_rect.x0, view_rect.y0, 0);
		pts[1] = Point3D(view_rect.x1, view_rect.y0, 0);
		pts[2] = Point3D(view_rect.x1, view_rect.y1, 0);
		pts[3] = Point3D(view_rect.x0, view_rect.y1, 0);

		for (int i = 0; i < 4; i++)
		{
			view_coords_to_cache_coords(pts[i], pts1[i]);
			texture_coords_to_global_coords(pts1[i], pts2[i]);
		}

		Rect2D global_rect = create_rect2d_from_points(pts2, 4);

		if (m_nframe_texture_index >= 0)
			m_paint_board->begin_frame_texture(m_nframe_texture_index);

		int count = 0;

		for (int i = 0; i < m_texture_items.size(); i++)
		{
			if (pbuf[i].state == SceneLayer::state_paint )
			{
				int index = pbuf[i].index;

				if (m_nframe_texture_index < 0)
				{
					if (cur_zoom != m_texture_items[index].zoom_rate)
						continue;
				}

				if (!global_rect.is_intersect(create_rect2d_from_points(m_cache_global_coords[index].pts, 4)))
					continue;

				double k = cur_zoom>m_texture_items[index].zoom_rate ? cur_zoom / m_texture_items[index].zoom_rate : m_texture_items[index].zoom_rate / cur_zoom;
				if (k > 5)
					continue;

				if (fabs(k - 1) > 0.001)
				{
					int a = 1;
				}

				quad_points quad_pts;
				//texture_item_to_view_coords(m_texture_items[i], quad_pts);
				quad_pts = m_cache_global_coords[index];
				transform_pts_by_matrix_2d(quad_pts.pts, 4, global_to_view_matrix);

				m_ptextures[index]->m_blinear_zoom = false;
				m_paint_board->draw_texture(m_ptextures[index], quad_pts);

				count++;
			}
		}

		if ( m_nframe_texture_index >= 0)
		{
			m_paint_board->end_frame_texture();
			m_paint_board->draw_frame_texture(m_nframe_texture_index);
		}

		delete[] pbuf;

		return;
	}


	void GraphicsLayer::set_bound(Rect3D rc)
	{
		lock_data();
		m_bound = rc;
		unlock_data();
	}

	Rect3D GraphicsLayer::getBound()
	{
		return m_bound;
	}

	Rect3D GraphicsLayer::get_bound_client()
	{
		Point3D bound_pts[4];
		get_rect_corners(m_bound, bound_pts);
		transform_pts_by_matrix_2d(bound_pts, 4, m_global_to_view_matrix);

		Rect3D view_bound = create_rect3d_from_points(bound_pts, 4);
		view_bound.x0 = floor(view_bound.x0);
		view_bound.y0 = floor(view_bound.y0);
		view_bound.x1 = ceil(view_bound.x1);
		view_bound.y1 = ceil(view_bound.y1);
		return view_bound;
	}

	void GraphicsLayer::set_view_matrix(const matrix4d& m, bool bmodify_zoom_rate)
	{
		lock_data();
		m_global_to_view_matrix = m;
		m_view_to_global_matrix = !m;

		Rect3D view_bound = get_bound_client();

		m_view_to_cache_matrix = matrix4d_move(-view_bound.x0, -view_bound.y0, 0);
		m_cache_to_view_matrix = !m_view_to_cache_matrix;

		m_global_to_cache_matrix = m_view_to_cache_matrix*m;
		m_cache_to_global_matrix = !m_global_to_cache_matrix;
		if (bmodify_zoom_rate)
		{
			//erase_all_textures();
		}
		unlock_data();
	}
	
	void GraphicsLayer::release_textures()
	{
		lock_data();
		if (m_paint_board)
		{
			m_paint_board->begin_texture_operation(m_sz_texture.cx, m_sz_texture.cy);
			for (int i = 0; i < m_ptextures.size(); i++)
			{
				m_ptextures[i]->release();
				delete m_ptextures[i];
			}
			m_ptextures.clear();
			m_paint_board->end_texture_operation();
		}

		m_texture_items.clear();
		unlock_data();
	}


	void GraphicsLayer::get_texture_range(Rect2I& range)
	{
		Rect3D view_bound = get_bound_client();

		range.x0 = floor(view_bound.x0 / m_sz_texture.cx);
		range.x1 = ceil(view_bound.x1 / m_sz_texture.cx);
		range.y0 = floor(view_bound.y0 / m_sz_texture.cy);
		range.y1 = ceil(view_bound.y1 / m_sz_texture.cy);

		range.x0 = range.y0 = -100000000;
		range.x1 = range.y1 = 100000000;
	}

	void GraphicsLayer::texture_item_to_texture_coords(const texture_item& item, quad_points& coord)
	{
		coord.pts[0].x = item.pos.x * m_sz_texture.cx;
		coord.pts[0].y = item.pos.y * m_sz_texture.cy;
		coord.pts[1].x = (item.pos.x+1) * m_sz_texture.cx;
		coord.pts[1].y = item.pos.y * m_sz_texture.cy;
		coord.pts[2].x = (item.pos.x+1) * m_sz_texture.cx;
		coord.pts[2].y = (item.pos.y+1) * m_sz_texture.cy;
		coord.pts[3].x = item.pos.x * m_sz_texture.cx;
		coord.pts[3].y = (item.pos.y+1) * m_sz_texture.cy;
	}

	void GraphicsLayer::texture_coords_to_texture_item(const Point3D& pt0, texture_item& item)
	{
		item.pos.x = floor(pt0.x / m_sz_texture.cx + 1e-6);
		item.pos.y = floor(pt0.y / m_sz_texture.cy + 1e-6);
	}

	extern void bmp_api::write_bmp_tofile(HBITMAP);

	void GraphicsLayer::paint_texture_item(const texture_item& item, int items_index)
	{
		if (!m_use_cache)
			return;

		matrix4d matrix_old = m_paint_board->get_matrix();
		Rect3D rc_bound = get_bound_client();

		matrix4d m1 = matrix4d_move(-rc_bound.x0 - item.pos.x*m_sz_texture.cx, -rc_bound.y0 - item.pos.y*m_sz_texture.cy, 0);
		matrix4d m_new = m1*m_global_to_view_matrix;

		SizeI sz_old = m_paint_board->get_view_size();

		m_paint_board->begin_texture_operation(m_sz_texture.cx, m_sz_texture.cy);
		m_paint_board->set_view_size(m_sz_texture);
		m_paint_board->set_matrix(m_new);

		GraphicsObject2D obj;
		char text[100] = { 0 };
		sprintf_s(text, "%d,%d", item.pos.x, item.pos.y);
		gd::TextStyle style;
		style.m_font_name = "宋体";
		style.m_height = 20;
		obj.text(Point2F(100, 128), 0xff0000ff, text, style, false, false);
		obj.transform(!m_new);
		//obj.refresh_bound();

		Rect2D view_rect = Rect2D(0, 0, m_sz_texture.cx, m_sz_texture.cy);
		Point2D pts[4];
		get_rect_corners(view_rect, pts);
		transform_pts_by_matrix_2d(pts, 4, !m_new);
		Rect2D view_rect2 = create_rect2d_from_points(pts, 4);

		double scale = sqrt(fabs(m_new.Det()));
		
		for (std::list<vect_obj>::iterator it = m_objs.begin(); it != m_objs.end(); it++)
		{
			if (!it->visible)
				continue;
			else if (view_rect2.is_intersect(it->pobj->getBound(scale)))
			{
				m_paint_board->draw_vectors(it->pobj);
			}
		}

		//m_paint_board->draw_vectors(&obj);

		m_paint_board->fill_texture(m_ptextures[items_index]);
		m_paint_board->set_view_size(sz_old);
		m_paint_board->set_matrix(matrix_old);
		m_paint_board->end_texture_operation();

		SceneLayer::paint_texture_item(item, items_index);

		quad_points quad;
		texture_item_to_view_coords(item, quad);
		transform_pts_by_matrix_2d(quad.pts, 4, m_view_to_global_matrix);
		m_cache_global_coords[items_index] = quad;

		m_texture_items[items_index].zoom_rate = sqrt(fabs(m_global_to_cache_matrix.Det()));
	}

	GraphicsObject2D *GraphicsLayer::createObj(LONG_PTR id, LONG_PTR user_data)
	{
		lock_data();

		vect_obj obj;
		obj.id = id;
		std::list<vect_obj>::iterator it = std::lower_bound(m_objs.begin(), m_objs.end(), obj);
		if (it != m_objs.end() && it->id==id )
		{
			unlock_data();
			return it->pobj;
		}
		else
		{
			obj.pobj = new GraphicsObject2D();
			obj.id = id;
			obj.user_data = user_data;
			m_objs.insert(it, obj);
			unlock_data();
			return obj.pobj;
		}
	}

	void GraphicsLayer::deleteObj(LONG_PTR id)
	{
		lock_data();
		vect_obj obj;
		obj.id = id;
		std::list<vect_obj>::iterator it = std::lower_bound(m_objs.begin(), m_objs.end(), obj);
		if (it != m_objs.end() && it->id == id)
		{
			delete it->pobj;
			m_objs.erase(it);
		}
		unlock_data();
	}

	void GraphicsLayer::deleteAllObjs()
	{
		lock_data();
		std::list<vect_obj>::iterator it = m_objs.begin();
		for (; it != m_objs.end(); it++)
		{
			delete it->pobj;
		}
		m_objs.clear();
		unlock_data();
	}

	void GraphicsLayer::set_obj_visible(LONG_PTR id, bool bvisible)
	{
		lock_data();
		vect_obj obj;
		obj.id = id;
		std::list<vect_obj>::iterator it = std::lower_bound(m_objs.begin(), m_objs.end(), obj);
		if (it != m_objs.end() && it->id == id)
		{
			it->visible = bvisible;
		}
		unlock_data();
	}


	bool GraphicsLayer::find_texture_item_to_paint(const Rect2D& view_rect, texture_item& item_to_fill, int& index_to_fill)
	{
		if (!m_use_cache)
			return false;

		//find an empty item
		int empty_item = -1;
		for (std::vector<texture_item>::iterator it = m_texture_items.begin(); it != m_texture_items.end(); it++)
		{
			texture_item item = *it;
			if (item.state == state_invalid)
			{
				empty_item = it - m_texture_items.begin();
				break;
			}
		}

		float cur_zoom = sqrt(fabs(m_global_to_cache_matrix.Det()));

		//find a cache pos to fill
		Rect2I range;
		get_texture_range(range);

		Point3D pts[4], pts1[4];
		texture_item items[4];
		Point2I item_pos[4];

		pts[0] = Point3D(view_rect.x0, view_rect.y0, 0);
		pts[1] = Point3D(view_rect.x1, view_rect.y0, 0);
		pts[2] = Point3D(view_rect.x1, view_rect.y1, 0);
		pts[3] = Point3D(view_rect.x0, view_rect.y1, 0);

		for (int i = 0; i < 4; i++)
		{
			view_coords_to_cache_coords(pts[i], pts1[i]);
			texture_coords_to_texture_item(pts1[i], items[i]);
			item_pos[i] = items[i].pos;
		}
		Rect2I range2;
		Rect2D range3 = create_rect2d_from_points(item_pos, 4);

		copy_rect_2(range3, range2);

		range2.intersect(range);

		//find a nearest cache item to the view center
		texture_item nearest_item;
		double dis, min_dis = -1, max_dis = -1;
		int farest_index = -1;

		for (int i = range2.x0; i <= range2.x1; i++)
		{
			for (int j = range2.y0; j <= range2.y1; j++)
			{
				texture_item item(i, j);
				item.zoom_rate = cur_zoom;
				int cache_index = find_texture_item(item);
				if (cache_index<0 || item.state == state_invalid)
				{
					quad_points coord;
					texture_item_to_view_coords(item, coord);

					Rect2D rc_item = create_rect2d_from_points(coord.pts, 4);
					if (rc_item.is_intersect(view_rect))
					{
						dis = geo_api::get_dis_2d(rc_item.center_point_2(), view_rect.center_point_2());

						if (min_dis < 0 || min_dis>dis)
						{
							min_dis = dis;
							nearest_item = item;
						}
					}
				}
			}
		}

		if (min_dis < 0)
			return false;

		if (empty_item == -1)
		{
			for (std::vector<texture_item>::iterator it = m_texture_items.begin(); it != m_texture_items.end(); it++)
			{
				texture_item item = *it;
				if (item.state == state_paint)
				{
					quad_points coord;
					texture_item_to_view_coords(item, coord);

					Rect2D rc_item = create_rect2d_from_points(coord.pts, 4);
					//if (!rc_item.is_intersect(view_rect))
					{
						dis = geo_api::get_dis_2d(rc_item.center_point_2(), view_rect.center_point_2());

						//优先将倍率不一样的缓存块换掉
						double k = item.zoom_rate > cur_zoom ? (1 + item.zoom_rate / cur_zoom) : (2 + cur_zoom / item.zoom_rate);
						if (fabs(cur_zoom/item.zoom_rate-1)<0.01)
							k = 1.0;
						dis *= k;

						if (max_dis < 0 || max_dis<dis)
						{
							max_dis = dis;
							farest_index = it - m_texture_items.begin();
						}
					}
				}
			}

			if (farest_index < 0)
				return false;
		}

		item_to_fill = nearest_item;
		index_to_fill = (empty_item != -1 ? empty_item : farest_index);

		return true;
	}


	void GraphicsLayer::step_paint(const Rect2D& view_rect)
	{
		if (!m_use_cache)
			return;
		else
		{
			lock_data();
			SceneLayer::step_paint(view_rect);
			unlock_data();
		}
	}
	 
	bool GraphicsLayer::is_paint_ok(const Rect2D& view_rect)
	{
		if (!m_use_cache)
			return true;
		else
			return SceneLayer::is_paint_ok(view_rect);
	}

}