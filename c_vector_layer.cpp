#include "StdAfx.h"
#include "c_vector_layer.h"
#include <algorithm>
#include "c_texture_d3d.h"
#include "c_texture_gl.h"
#include "c_texture_gdi.h"
#include "BitmapFunc.h"
#include "geo_algorithom.h"


namespace StereoDraw
{
	c_vector_layer::vect_obj::vect_obj()
	{
		pobj = NULL;
		id = 0;
		bvisible = true;
		bmarked = false;
	}

	bool c_vector_layer::vect_obj::operator < (const vect_obj& b)
	{
		return (id < b.id);
	}

	c_vector_layer::c_vector_layer()
	{
		m_nframe_texture_index = -1;
	}


	c_vector_layer::~c_vector_layer()
	{
		release_cache();
		delete_obj_all();
	}

	void c_vector_layer::set_cache_size(int nMB, int cache_type)
	{
		m_ptextures.clear();

		c_drawing_layer::set_cache_size(nMB,cache_type);

		m_paint_board->begin_texture_operation(m_sz_cache_block.cx, m_sz_cache_block.cy);
		m_nframe_texture_index = m_paint_board->request_frame_texture();

		for (int i = 0; i < m_cache_items.size(); i++)
		{
			if (cache_type == c_drawing_layer::cache_d3d)
			{
				c_texture_d3d *p = new c_texture_d3d();
				p->set_device(m_paint_board->device_d3d());
				p->create(m_sz_cache_block.cx, m_sz_cache_block.cy);
				m_ptextures.push_back(p);
			}
			else if (cache_type == c_drawing_layer::cache_gl)
			{
				c_texture_gl *p = new c_texture_gl();
				p->set_device(m_paint_board->device_gl());
				p->create(m_sz_cache_block.cx, m_sz_cache_block.cy);
				m_ptextures.push_back(p);
			}
			else if (cache_type == c_drawing_layer::cache_gdi)
			{
				c_texture_gdi *p = new c_texture_gdi();
				p->set_device(m_paint_board->device_gdi());
				p->create(m_sz_cache_block.cx, m_sz_cache_block.cy);
				m_ptextures.push_back(p);
			}
		}

		m_paint_board->end_texture_operation();

		m_cache_ground_coords.assign(m_cache_items.size(), quad_points());
	}

	static int compare_zoomrate_index(const void *p1, const void *p2)
	{
		const c_vector_layer::zoomrate_index *p3 = (const c_vector_layer::zoomrate_index*)p1;
		const c_vector_layer::zoomrate_index *p4 = (const c_vector_layer::zoomrate_index*)p2;
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


	void c_vector_layer::draw()
	{
		float cur_zoom = sqrt(fabs(m_ground_to_cache_matrix.Det()));
		zoomrate_index *pbuf = new zoomrate_index[m_cache_items.size()];
		for (int i = 0; i < m_cache_items.size(); i++)
		{
			pbuf[i].index = i;
			pbuf[i].state = m_cache_items[i].state;
			pbuf[i].zoom = m_cache_items[i].zoom_rate;
			pbuf[i].cur_zoom = cur_zoom;
		}

		std::qsort(pbuf, m_cache_items.size(), sizeof(zoomrate_index), compare_zoomrate_index);

		std::vector<cache_item> cache_items;

		for (int i = 0; i < m_cache_items.size(); i++)
		{
			if (pbuf[i].state == c_drawing_layer::state_filled)
			{
				cache_items.push_back(m_cache_items[pbuf[i].index]);
			}
		}

		size_i sz_view = m_paint_board->get_view_size();
		rect_2i view_rect(0,0,sz_view.cx, sz_view.cy);
		point_3d pts[4], pts1[4], pts2[4];

		pts[0] = point_3d(view_rect.x0, view_rect.y0, 0);
		pts[1] = point_3d(view_rect.x1, view_rect.y0, 0);
		pts[2] = point_3d(view_rect.x1, view_rect.y1, 0);
		pts[3] = point_3d(view_rect.x0, view_rect.y1, 0);

		for (int i = 0; i < 4; i++)
		{
			view_coord_to_cache_coord(pts[i], pts1[i]);
			cache_coord_to_ground_coord(pts1[i], pts2[i]);
		}

		rect_2d ground_rect = create_rect2d_from_points(pts2, 4);

		if (m_nframe_texture_index >= 0)
			m_paint_board->begin_frame_texture(m_nframe_texture_index);

		int count = 0;

		for (int i = 0; i < m_cache_items.size(); i++)
		{
			if (pbuf[i].state == c_drawing_layer::state_filled )
			{
				int index = pbuf[i].index;

				if (!ground_rect.is_intersect(create_rect2d_from_points(m_cache_ground_coords[index].pts, 4)))
					continue;

				double k = cur_zoom>m_cache_items[index].zoom_rate ? cur_zoom / m_cache_items[index].zoom_rate : m_cache_items[index].zoom_rate / cur_zoom;
				if (k > 5)
					continue;

				quad_points quad_pts;
				//get_cache_item_view_coords(m_cache_items[i], quad_pts);
				quad_pts = m_cache_ground_coords[index];
				transform_pts_by_matrix_2d(quad_pts.pts, 4, m_ground_to_view_matrix);
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

		for (int i = 0; i < m_ptextures.size(); i++)
		{
			if (m_cache_items[i].state==c_drawing_layer::state_filled)
			{
				quad_points quad_pts;
				//get_cache_item_view_coords(m_cache_items[i], quad_pts);
				quad_pts = m_cache_ground_coords[i];
				transform_pts_by_matrix_2d(quad_pts.pts, 4, m_ground_to_view_matrix);
				m_paint_board->draw_texture(m_ptextures[i], quad_pts);
			}
		}
	}


	void c_vector_layer::set_bound(rect_3d rc)
	{
		m_bound = rc;
	}

	rect_3d c_vector_layer::get_bound()
	{
		return m_bound;
	}

	rect_3d c_vector_layer::get_bound_client()
	{
		point_3d bound_pts[4];
		get_rect_corners(m_bound, bound_pts);
		transform_pts_by_matrix_2d(bound_pts, 4, m_ground_to_view_matrix);

		rect_3d view_bound = create_rect3d_from_points(bound_pts, 4);
		view_bound.x0 = floor(view_bound.x0);
		view_bound.y0 = floor(view_bound.y0);
		view_bound.x1 = ceil(view_bound.x1);
		view_bound.y1 = ceil(view_bound.y1);
		return view_bound;
	}

	void c_vector_layer::set_view_matrix(const matrix4d& m, bool bmodify_zoom_rate)
	{
		m_ground_to_view_matrix = m;
		m_view_to_ground_matrix = !m;

		rect_3d view_bound = get_bound_client();

		m_view_to_cache_matrix = matrix4d_move(-view_bound.x0, -view_bound.y0, 0);
		m_cache_to_view_matrix = !m_view_to_cache_matrix;

		m_ground_to_cache_matrix = m_view_to_cache_matrix*m;
		m_cache_to_ground_matrix = !m_ground_to_cache_matrix;
		if (bmodify_zoom_rate)
		{
			//clear_all_cache();
		}
	}
	
	void c_vector_layer::release_cache()
	{
		if (m_paint_board)
		{
			m_paint_board->begin_texture_operation(m_sz_cache_block.cx, m_sz_cache_block.cy);
			for (int i = 0; i < m_ptextures.size(); i++)
			{
				m_ptextures[i]->del();
				delete m_ptextures[i];
			}
			m_ptextures.clear();
			m_paint_board->end_texture_operation();
		}

		m_cache_items.clear();
	}


	void c_vector_layer::get_cache_range(rect_2i& range)
	{
		rect_3d view_bound = get_bound_client();

		range.x0 = floor(view_bound.x0 / m_sz_cache_block.cx);
		range.x1 = ceil(view_bound.x1 / m_sz_cache_block.cx);
		range.y0 = floor(view_bound.y0 / m_sz_cache_block.cy);
		range.y1 = ceil(view_bound.y1 / m_sz_cache_block.cy);

		range.x0 = range.y0 = -100000000;
		range.x1 = range.y1 = 100000000;
	}

	void c_vector_layer::get_cache_item_cache_coords(const cache_item& item, quad_points& coord)
	{
		coord.pts[0].x = item.pos.x * m_sz_cache_block.cx;
		coord.pts[0].y = item.pos.y * m_sz_cache_block.cy;
		coord.pts[1].x = (item.pos.x+1) * m_sz_cache_block.cx;
		coord.pts[1].y = item.pos.y * m_sz_cache_block.cy;
		coord.pts[2].x = (item.pos.x+1) * m_sz_cache_block.cx;
		coord.pts[2].y = (item.pos.y+1) * m_sz_cache_block.cy;
		coord.pts[3].x = item.pos.x * m_sz_cache_block.cx;
		coord.pts[3].y = (item.pos.y+1) * m_sz_cache_block.cy;
	}

	void c_vector_layer::cache_coord_to_cache_item(const point_3d& pt0, cache_item& item)
	{
		item.pos.x = floor(pt0.x / m_sz_cache_block.cx + 1e-6);
		item.pos.y = floor(pt0.y / m_sz_cache_block.cy + 1e-6);
	}

	extern void bmp_api::write_bmp_tofile(HBITMAP);

	void c_vector_layer::fill_cache_item(const cache_item& item, int items_index)
	{		
		matrix4d matrix_old = m_paint_board->get_matrix();
		rect_3d rc_bound = get_bound_client();

		matrix4d m1 = matrix4d_move(-rc_bound.x0 - item.pos.x*m_sz_cache_block.cx, -rc_bound.y0 - item.pos.y*m_sz_cache_block.cy, 0);
		matrix4d m_new = m1*m_ground_to_view_matrix;

		size_i sz_old = m_paint_board->get_view_size();

		m_paint_board->begin_texture_operation(m_sz_cache_block.cx, m_sz_cache_block.cy);
		m_paint_board->set_view_size(m_sz_cache_block);
		m_paint_board->set_matrix(m_new);

		c_vector_obj_2d obj;
		char text[100] = { 0 };
		sprintf_s(text, "%d,%d", item.pos.x, item.pos.y);
		StereoDraw::c_text_style style;
		style.m_font_name = "宋体";
		style.m_height = 20;
		obj.text(point_2f(100, 128), 0xff0000ff, text, style, false, false);
		obj.transform(!m_new);
		//obj.refresh_bound();

		rect_2d view_rect = rect_2d(0, 0, m_sz_cache_block.cx, m_sz_cache_block.cy);
		point_2d pts[4];
		get_rect_corners(view_rect, pts);
		transform_pts_by_matrix_2d(pts, 4, !m_new);
		rect_2d view_rect2 = create_rect2d_from_points(pts, 4);

		double scale = sqrt(fabs(m_new.Det()));
		
		for (std::list<vect_obj>::iterator it = m_objs.begin(); it != m_objs.end(); it++)
		{
			if (!it->bvisible)
				continue;
			if (it->bmarked);
			else if (view_rect2.is_intersect(it->pobj->get_bound(scale)))
			{
				m_paint_board->draw_vectors(it->pobj, c_paint_board::mode_draw);
			}
		}

		//m_paint_board->draw_vectors(&obj, c_paint_board::mode_draw);

		m_paint_board->fill_texture(m_ptextures[items_index]);
		m_paint_board->set_view_size(sz_old);
		m_paint_board->set_matrix(matrix_old);
		m_paint_board->end_texture_operation();

		c_drawing_layer::fill_cache_item(item, items_index);

		quad_points quad;
		get_cache_item_view_coords(item, quad);
		transform_pts_by_matrix_2d(quad.pts, 4, m_view_to_ground_matrix);
		m_cache_ground_coords[items_index] = quad;

		m_cache_items[items_index].zoom_rate = sqrt(fabs(m_ground_to_cache_matrix.Det()));
	}

	c_vector_obj_2d *c_vector_layer::open_obj(long id)
	{
		vect_obj obj;
		obj.id = id;
		std::list<vect_obj>::iterator it = std::lower_bound(m_objs.begin(), m_objs.end(), obj);
		if (it != m_objs.end() && it->id==id )
		{
			return it->pobj;
		}
		else
		{
			obj.pobj = new c_vector_obj_2d();
			m_objs.insert(it, obj);
			return obj.pobj;
		}
	}

	void c_vector_layer::delete_obj(long id)
	{
		vect_obj obj;
		obj.id = id;
		std::list<vect_obj>::iterator it = std::lower_bound(m_objs.begin(), m_objs.end(), obj);
		if (it != m_objs.end() && it->id == id)
		{
			delete it->pobj;
			m_objs.erase(it);
		}
	}

	void c_vector_layer::delete_obj_all()
	{
		std::list<vect_obj>::iterator it = m_objs.begin();
		for (; it != m_objs.end(); it++)
		{
			delete it->pobj;
		}
		m_objs.clear();
	}

	void c_vector_layer::set_obj_visible(long id, bool bvisible)
	{
		vect_obj obj;
		obj.id = id;
		std::list<vect_obj>::iterator it = std::lower_bound(m_objs.begin(), m_objs.end(), obj);
		if (it != m_objs.end() && it->id == id)
		{
			it->bvisible = bvisible;
		}
	}

	void c_vector_layer::set_obj_marked(long id, bool bmarked)
	{
		vect_obj obj;
		obj.id = id;
		std::list<vect_obj>::iterator it = std::lower_bound(m_objs.begin(), m_objs.end(), obj);
		if (it != m_objs.end() && it->id == id)
		{
			it->bmarked = bmarked;
		}
	}


	bool c_vector_layer::find_cache_item_to_fill(const rect_2d& view_rect, cache_item& item_to_fill, int& index_to_fill)
	{
		//find an empty item
		int empty_item = -1;
		for (std::vector<cache_item>::iterator it = m_cache_items.begin(); it != m_cache_items.end(); it++)
		{
			cache_item item = *it;
			if (item.state == state_invalid)
			{
				empty_item = it - m_cache_items.begin();
				break;
			}
		}

		float cur_zoom = sqrt(fabs(m_ground_to_cache_matrix.Det()));

		//find a cache pos to fill
		rect_2i range;
		get_cache_range(range);

		point_3d pts[4], pts1[4];
		cache_item items[4];
		point_2i item_pos[4];

		pts[0] = point_3d(view_rect.x0, view_rect.y0, 0);
		pts[1] = point_3d(view_rect.x1, view_rect.y0, 0);
		pts[2] = point_3d(view_rect.x1, view_rect.y1, 0);
		pts[3] = point_3d(view_rect.x0, view_rect.y1, 0);

		for (int i = 0; i < 4; i++)
		{
			view_coord_to_cache_coord(pts[i], pts1[i]);
			cache_coord_to_cache_item(pts1[i], items[i]);
			item_pos[i] = items[i].pos;
		}
		rect_2i range2;
		rect_2d range3 = create_rect2d_from_points(item_pos, 4);

		copy_rect_2(range3, range2);

		range2.intersect(range);

		//find a nearest cache item to the view center
		cache_item nearest_item;
		double dis, min_dis = -1, max_dis = -1;
		int farest_index = -1;

		for (int i = range2.x0; i <= range2.x1; i++)
		{
			for (int j = range2.y0; j <= range2.y1; j++)
			{
				cache_item item(i, j);
				item.zoom_rate = cur_zoom;
				int cache_index = find_cache_item(item);
				if (cache_index<0 || item.state == state_invalid)
				{
					quad_points coord;
					get_cache_item_view_coords(item, coord);

					rect_2d rc_item = create_rect2d_from_points(coord.pts, 4);
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
			for (std::vector<cache_item>::iterator it = m_cache_items.begin(); it != m_cache_items.end(); it++)
			{
				cache_item item = *it;
				if (item.state == state_filled)
				{
					quad_points coord;
					get_cache_item_view_coords(item, coord);

					rect_2d rc_item = create_rect2d_from_points(coord.pts, 4);
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
							farest_index = it - m_cache_items.begin();
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

}