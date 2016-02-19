#include "stdafx.h"
#include "vector_obj.h"
#include "geo_algorithom.h"
#include "c_font_helper.h"

namespace StereoDraw
{

	c_vector_obj_2d::c_vector_obj_2d()
	{
		m_origin_x = 0;
		m_origin_y = 0;
	}


	c_vector_obj_2d::~c_vector_obj_2d()
	{
	}



	void c_vector_obj_2d::move(double dx, double dy)
	{
		m_origin_x += dx;
		m_origin_y += dy;
	}


	void c_vector_obj_2d::zoom(double kx, double ky)
	{
		m_origin_x *= kx;
		m_origin_y *= ky;

		for (std::list<c_head*>::iterator it = m_list.begin(); it != m_list.end(); it++)
		{
			c_head *p = *it;
			if (p == NULL)continue;

			if (is_point(p))
			{
				((c_point*)p)->m_pt.x *= kx;
				((c_point*)p)->m_pt.y *= ky;
			}
			else if (is_cell(p))
			{
				((c_cell*)p)->m_pt.x *= kx;
				((c_cell*)p)->m_pt.y *= ky;
			}
			else if (is_point_string(p))
			{
				c_point_string *p2 = (c_point_string*)p;
				for (std::vector<point_2f>::iterator it = p2->m_pts.begin(); it != p2->m_pts.end(); it++)
				{
					(*it).x *= kx;
					(*it).y *= ky;
				}
			}
			else if (is_cell_string(p))
			{
				c_cell_string *p2 = (c_cell_string*)p;
				for (std::vector<point_2f>::iterator it = p2->m_pts.begin(); it != p2->m_pts.end(); it++)
				{
					(*it).x *= kx;
					(*it).y *= ky;
				}
			}
			else if (is_line_string(p))
			{
				c_line_string *p2 = (c_line_string*)p;
				for (std::vector<point_2f>::iterator it = p2->m_pts.begin(); it != p2->m_pts.end(); it++)
				{
					(*it).x *= kx;
					(*it).y *= ky;
				}
			}
			else if (is_polygon(p))
			{
				c_polygon *p2 = (c_polygon*)p;
				for (std::vector<point_2f>::iterator it = p2->m_pts.begin(); it != p2->m_pts.end(); it++)
				{
					(*it).x *= kx;
					(*it).y *= ky;
				}
			}
			else if (is_text(p))
			{
				((c_text*)p)->m_pt.x *= kx;
				((c_text*)p)->m_pt.y *= ky;
			}
		}

	}


	void c_vector_obj_2d::rotate(point_2d pt, double angle)
	{
		matrix4d m = matrix4d_rotate(&point_3d(pt.x,pt.y,0), NULL, angle);
		transform(m);
	}



	void c_vector_obj_2d::transform(const matrix4d& m)
	{
		std::vector<point_2f> pts;
		get_all_pts(pts);

		transform_pts_by_matrix_2d(pts.data(), pts.size(), m);

		set_all_pts(pts);
	}


	void c_vector_obj_2d::refresh_bound()
	{
		for (std::list<c_head*>::iterator it = m_list.begin(); it != m_list.end(); it++)
		{
			c_head *p = *it;
			if (p == NULL)continue;

			if (is_point_string(p))
			{
				c_point_string *p2 = (c_point_string*)p;
				rect_2d rc = create_rect2d_from_points(p2->m_pts.data(), p2->m_pts.size());
				rect2d_to_points(rc, p2->m_range.m_pt0, p2->m_range.m_pt1);
			}
			else if (is_cell_string(p))
			{
				c_cell_string *p2 = (c_cell_string*)p;
				rect_2d rc = create_rect2d_from_points(p2->m_pts.data(), p2->m_pts.size());
				rect2d_to_points(rc, p2->m_range.m_pt0, p2->m_range.m_pt1);
			}
			else if (is_line_string(p))
			{
				c_line_string *p2 = (c_line_string*)p;
				rect_2d rc = create_rect2d_from_points(p2->m_pts.data(), p2->m_pts.size());
				rect2d_to_points(rc, p2->m_range.m_pt0, p2->m_range.m_pt1);
			}
			else if (is_polygon(p))
			{
				c_polygon *p2 = (c_polygon*)p;
				rect_2d rc = create_rect2d_from_points(p2->m_pts.data(), p2->m_pts.size());
				rect2d_to_points(rc, p2->m_range.m_pt0, p2->m_range.m_pt1);
			}
			else if (is_text(p))
			{
				c_text *p2 = (c_text*)p;
				c_font_helper helper;

				c_text_style style = p2->m_text_style;
				style.m_height = 400;
				rect_2d rc = helper.get_bound(p2->m_text.data(), style);
				rc *= (p2->m_text_style.m_height / 400);
				rect2d_to_points(rc, p2->m_range.m_pt0, p2->m_range.m_pt1);
			}
		}
	}


	c_vector_obj_3d::c_vector_obj_3d()
	{
	}


	c_vector_obj_3d::~c_vector_obj_3d()
	{
	}



	void c_vector_obj_3d::move(double dx, double dy, double dz)
	{
		for (std::list<c_head*>::iterator it = m_list.begin(); it != m_list.end(); it++)
		{
			c_head *p = *it;
			if (p == NULL)continue;

			if (is_point(p))
			{
				((c_point*)p)->m_pt.x += dx;
				((c_point*)p)->m_pt.y += dy;
				((c_point*)p)->m_pt.z += dz;
			}
			else if (is_cell(p))
			{
				((c_cell*)p)->m_pt.x += dx;
				((c_cell*)p)->m_pt.y += dy;
				((c_cell*)p)->m_pt.z += dz;
			}
			else if (is_point_string(p))
			{
				c_point_string *p2 = (c_point_string*)p;
				for (std::vector<point_3d>::iterator it = p2->m_pts.begin(); it != p2->m_pts.end(); it++)
				{
					(*it).x += dx;
					(*it).y += dy;
					(*it).z += dz;
				}
			}
			else if (is_cell_string(p))
			{
				c_cell_string *p2 = (c_cell_string*)p;
				for (std::vector<point_3d>::iterator it = p2->m_pts.begin(); it != p2->m_pts.end(); it++)
				{
					(*it).x += dx;
					(*it).y += dy;
					(*it).z += dz;
				}
			}
			else if (is_line_string(p))
			{
				c_line_string *p2 = (c_line_string*)p;
				for (std::vector<point_3d>::iterator it = p2->m_pts.begin(); it != p2->m_pts.end(); it++)
				{
					(*it).x += dx;
					(*it).y += dy;
					(*it).z += dz;
				}
			}
			else if (is_polygon(p))
			{
				c_polygon *p2 = (c_polygon*)p;
				for (std::vector<point_3d>::iterator it = p2->m_pts.begin(); it != p2->m_pts.end(); it++)
				{
					(*it).x += dx;
					(*it).y += dy;
					(*it).z += dz;
				}
			}
			else if (is_text(p))
			{
				((c_text*)p)->m_pt.x += dx;
				((c_text*)p)->m_pt.y += dy;
				((c_text*)p)->m_pt.z += dz;
			}
		}
	}


	void c_vector_obj_3d::zoom(double kx, double ky, double kz)
	{
		for (std::list<c_head*>::iterator it = m_list.begin(); it != m_list.end(); it++)
		{
			c_head *p = *it;
			if (p == NULL)continue;

			if (is_point(p))
			{
				((c_point*)p)->m_pt.x *= kx;
				((c_point*)p)->m_pt.y *= ky;
				((c_point*)p)->m_pt.z *= kz;
			}
			else if (is_cell(p))
			{
				((c_cell*)p)->m_pt.x *= kx;
				((c_cell*)p)->m_pt.y *= ky;
				((c_cell*)p)->m_pt.z *= kz;
			}
			else if (is_point_string(p))
			{
				c_point_string *p2 = (c_point_string*)p;
				for (std::vector<point_3d>::iterator it = p2->m_pts.begin(); it != p2->m_pts.end(); it++)
				{
					(*it).x *= kx;
					(*it).y *= ky;
					(*it).z *= kz;
				}
			}
			else if (is_cell_string(p))
			{
				c_cell_string *p2 = (c_cell_string*)p;
				for (std::vector<point_3d>::iterator it = p2->m_pts.begin(); it != p2->m_pts.end(); it++)
				{
					(*it).x *= kx;
					(*it).y *= ky;
					(*it).z *= kz;
				}
			}
			else if (is_line_string(p))
			{
				c_line_string *p2 = (c_line_string*)p;
				for (std::vector<point_3d>::iterator it = p2->m_pts.begin(); it != p2->m_pts.end(); it++)
				{
					(*it).x *= kx;
					(*it).y *= ky;
					(*it).z *= kz;
				}
			}
			else if (is_polygon(p))
			{
				c_polygon *p2 = (c_polygon*)p;
				for (std::vector<point_3d>::iterator it = p2->m_pts.begin(); it != p2->m_pts.end(); it++)
				{
					(*it).x *= kx;
					(*it).y *= ky;
					(*it).z *= kz;
				}
			}
			else if (is_text(p))
			{
				((c_text*)p)->m_pt.x *= kx;
				((c_text*)p)->m_pt.y *= ky;
				((c_text*)p)->m_pt.z *= kz;
			}
		}
	}


	void c_vector_obj_3d::rotate(point_3d pt, double angle, point_3d *dpt)
	{
		matrix4d m = matrix4d_rotate(&pt, dpt, angle);
		transform(m);
	}


	void c_vector_obj_3d::transform(const matrix4d& m)
	{
		std::vector<point_3d> pts;
		get_all_pts(pts);

		transform_pts_by_matrix_3d(pts.data(), pts.size(), m);

		set_all_pts(pts);
	}

	void c_vector_obj_3d::refresh_bound()
	{
		for (std::list<c_head*>::iterator it = m_list.begin(); it != m_list.end(); it++)
		{
			c_head *p = *it;
			if (p == NULL)continue;

			if (is_point_string(p))
			{
				c_point_string *p2 = (c_point_string*)p;
				rect_3d rc = create_rect3d_from_points(p2->m_pts.data(), p2->m_pts.size());
				rect3d_to_points(rc, p2->m_range.m_pt0, p2->m_range.m_pt1);
			}
			else if (is_cell_string(p))
			{
				c_cell_string *p2 = (c_cell_string*)p;
				rect_3d rc = create_rect3d_from_points(p2->m_pts.data(), p2->m_pts.size());
				rect3d_to_points(rc, p2->m_range.m_pt0, p2->m_range.m_pt1);
			}
			else if (is_line_string(p))
			{
				c_line_string *p2 = (c_line_string*)p;
				rect_3d rc = create_rect3d_from_points(p2->m_pts.data(), p2->m_pts.size());
				rect3d_to_points(rc, p2->m_range.m_pt0, p2->m_range.m_pt1);
			}
			else if (is_polygon(p))
			{
				c_polygon *p2 = (c_polygon*)p;
				rect_3d rc = create_rect3d_from_points(p2->m_pts.data(), p2->m_pts.size());
				rect3d_to_points(rc, p2->m_range.m_pt0, p2->m_range.m_pt1);
			}
		}
	}
	

	void c_vector_obj_2d::copyfrom(c_vector_obj_3d* obj)
	{
		clear();
		for (std::list<c_vector_obj_3d::c_head*>::iterator it = obj->m_list.begin(); it != obj->m_list.end(); it++)
		{
			const c_vector_obj_3d::c_head *p = *it;
			if (p == NULL)continue;

			if (c_vector_obj_3d::is_point(p))
			{
				c_point *p_new = new c_point();
				c_vector_obj_3d::c_point* p1 = (c_vector_obj_3d::c_point*)p;

				p_new->m_pt.x = p1->m_pt.x;
				p_new->m_pt.y = p1->m_pt.y;
				p_new->m_color = p1->m_color;
				p_new->m_size = p1->m_size;
				p_new->m_bglobal_size = p1->m_bglobal_size;
				p_new->m_bflat = p1->m_bflat;

				m_list.push_back(p_new);
			}
			else if (c_vector_obj_3d::is_cell(p))
			{
				c_cell *p_new = new c_cell();
				c_vector_obj_3d::c_cell* p1 = (c_vector_obj_3d::c_cell*)p;
				p_new->m_pt.x = p1->m_pt.x;
				p_new->m_pt.y = p1->m_pt.y;
				p_new->m_color = p1->m_color;
				p_new->m_angle = p1->m_angle;
				p_new->m_bglobal_scale = p1->m_bglobal_scale;
				p_new->m_cell_index = p1->m_cell_index;
				p_new->m_scale_x = p1->m_scale_x;
				p_new->m_scale_y = p1->m_scale_y;

				m_list.push_back(p_new);
			}
			else if (c_vector_obj_3d::is_point_string(p))
			{
				c_point_string *p_new = new c_point_string();
				c_vector_obj_3d::c_point_string* p1 = (c_vector_obj_3d::c_point_string*)p;
				geo_api::copy_xy(p1->m_pts, p_new->m_pts);
				p_new->m_colors = p1->m_colors;
				p_new->m_size = p1->m_size;
				p_new->m_bglobal_size = p1->m_bglobal_size;
				p_new->m_bflat = p1->m_bflat;
				geo_api::copy_xy(p_new->m_range.m_pt0, p1->m_range.m_pt0);
				geo_api::copy_xy(p_new->m_range.m_pt1, p1->m_range.m_pt1);

				m_list.push_back(p_new);
			}
			else if (c_vector_obj_3d::is_cell_string(p))
			{
				c_cell_string *p_new = new c_cell_string();
				c_vector_obj_3d::c_cell_string* p1 = (c_vector_obj_3d::c_cell_string*)p;
				geo_api::copy_xy(p1->m_pts, p_new->m_pts);
				p_new->m_colors = p1->m_colors;
				p_new->m_angles = p1->m_angles;
				p_new->m_cell_index = p1->m_cell_index;
				p_new->m_scale_x = p1->m_scale_x;
				p_new->m_scale_y = p1->m_scale_y;
				p_new->m_bglobal_scale = p1->m_bglobal_scale;
				geo_api::copy_xy(p_new->m_range.m_pt0, p1->m_range.m_pt0);
				geo_api::copy_xy(p_new->m_range.m_pt1, p1->m_range.m_pt1);

				m_list.push_back(p_new);
			}
			else if (c_vector_obj_3d::is_line_string(p))
			{
				c_line_string *p_new = new c_line_string();
				c_vector_obj_3d::c_line_string* p1 = (c_vector_obj_3d::c_line_string*)p;

				geo_api::copy_xy(p1->m_pts, p_new->m_pts);
				p_new->m_color = p1->m_color;
				p_new->m_line_style = p1->m_line_style;
				p_new->m_line_width = p1->m_line_width;
				p_new->m_bglobal_width = p1->m_bglobal_width;
				geo_api::copy_xy(p_new->m_range.m_pt0, p1->m_range.m_pt0);
				geo_api::copy_xy(p_new->m_range.m_pt1, p1->m_range.m_pt1);

				m_list.push_back(p_new);
			}
			else if (c_vector_obj_3d::is_polygon(p))
			{
				c_polygon *p_new = new c_polygon();
				c_vector_obj_3d::c_polygon* p1 = (c_vector_obj_3d::c_polygon*)p;

				geo_api::copy_xy(p1->m_pts, p_new->m_pts);
				p_new->m_color = p1->m_color;
				p_new->m_counts = p1->m_counts;
				geo_api::copy_xy(p_new->m_range.m_pt0, p1->m_range.m_pt0);
				geo_api::copy_xy(p_new->m_range.m_pt1, p1->m_range.m_pt1);

				m_list.push_back(p_new);
			}
			else if (c_vector_obj_3d::is_text(p))
			{
				c_text *p_new = new c_text();
				c_vector_obj_3d::c_text* p1 = (c_vector_obj_3d::c_text*)p;

				p_new->m_pt.x = p1->m_pt.x;
				p_new->m_pt.y = p1->m_pt.y;
				p_new->m_color = p1->m_color;
				p_new->m_text = p1->m_text;
				p_new->m_text_style = p1->m_text_style;
				p_new->m_bglobal_size = p1->m_bglobal_size;
				p_new->m_bupward = p1->m_bupward;
				geo_api::copy_xy(p_new->m_range.m_pt0, p1->m_range.m_pt0);
				geo_api::copy_xy(p_new->m_range.m_pt1, p1->m_range.m_pt1);

				m_list.push_back(p_new);
			}
		}

		std::vector<point_3d> pts;
		std::vector<point_2f> pts2;
		obj->get_all_pts(pts);
		if (pts.size() > 0)
		{
			m_origin_x = pts[0].x;
			m_origin_y = pts[0].y;

			pts2.resize(pts.size());

			int size = pts.size();
			for (int i = 0; i < size; i++)
			{
				pts2[i].x = pts[i].x - m_origin_x;
				pts2[i].y = pts[i].y - m_origin_y;
			}

			set_all_pts(pts2);
		}
	}


	void c_vector_obj_3d::copyfrom(c_vector_obj_2d* obj)
	{
		clear();
		for (std::list<c_vector_obj_2d::c_head*>::iterator it = obj->m_list.begin(); it != obj->m_list.end(); it++)
		{
			const c_vector_obj_2d::c_head *p = *it;
			if (p == NULL)continue;

			if (c_vector_obj_2d::is_point(p))
			{
				c_point *p_new = new c_point();
				c_vector_obj_2d::c_point* p1 = (c_vector_obj_2d::c_point*)p;

				p_new->m_pt.x = p1->m_pt.x;
				p_new->m_pt.y = p1->m_pt.y;
				p_new->m_color = p1->m_color;
				p_new->m_size = p1->m_size;
				p_new->m_bglobal_size = p1->m_bglobal_size;
				p_new->m_bflat = p1->m_bflat;

				m_list.push_back(p_new);
			}
			else if (c_vector_obj_2d::is_cell(p))
			{
				c_cell *p_new = new c_cell();
				c_vector_obj_2d::c_cell* p1 = (c_vector_obj_2d::c_cell*)p;
				p_new->m_pt.x = p1->m_pt.x;
				p_new->m_pt.y = p1->m_pt.y;
				p_new->m_color = p1->m_color;
				p_new->m_angle = p1->m_angle;
				p_new->m_bglobal_scale = p1->m_bglobal_scale;
				p_new->m_cell_index = p1->m_cell_index;
				p_new->m_scale_x = p1->m_scale_x;
				p_new->m_scale_y = p1->m_scale_y;

				m_list.push_back(p_new);
			}
			else if (c_vector_obj_2d::is_point_string(p))
			{
				c_point_string *p_new = new c_point_string();
				c_vector_obj_2d::c_point_string* p1 = (c_vector_obj_2d::c_point_string*)p;
				geo_api::copy_xy(p1->m_pts, p_new->m_pts);
				p_new->m_colors = p1->m_colors;
				p_new->m_size = p1->m_size;
				p_new->m_bglobal_size = p1->m_bglobal_size;
				p_new->m_bflat = p1->m_bflat;
				geo_api::copy_xy(p_new->m_range.m_pt0, p1->m_range.m_pt0);
				geo_api::copy_xy(p_new->m_range.m_pt1, p1->m_range.m_pt1);

				m_list.push_back(p_new);
			}
			else if (c_vector_obj_2d::is_cell_string(p))
			{
				c_cell_string *p_new = new c_cell_string();
				c_vector_obj_2d::c_cell_string* p1 = (c_vector_obj_2d::c_cell_string*)p;
				geo_api::copy_xy(p1->m_pts, p_new->m_pts);
				p_new->m_colors = p1->m_colors;
				p_new->m_angles = p1->m_angles;
				p_new->m_cell_index = p1->m_cell_index;
				p_new->m_scale_x = p1->m_scale_x;
				p_new->m_scale_y = p1->m_scale_y;
				p_new->m_bglobal_scale = p1->m_bglobal_scale;
				geo_api::copy_xy(p_new->m_range.m_pt0, p1->m_range.m_pt0);
				geo_api::copy_xy(p_new->m_range.m_pt1, p1->m_range.m_pt1);

				m_list.push_back(p_new);
			}
			else if (c_vector_obj_2d::is_line_string(p))
			{
				c_line_string *p_new = new c_line_string();
				c_vector_obj_2d::c_line_string* p1 = (c_vector_obj_2d::c_line_string*)p;

				geo_api::copy_xy(p1->m_pts, p_new->m_pts);
				p_new->m_color = p1->m_color;
				p_new->m_line_style = p1->m_line_style;
				p_new->m_line_width = p1->m_line_width;
				p_new->m_bglobal_width = p1->m_bglobal_width;
				geo_api::copy_xy(p_new->m_range.m_pt0, p1->m_range.m_pt0);
				geo_api::copy_xy(p_new->m_range.m_pt1, p1->m_range.m_pt1);

				m_list.push_back(p_new);
			}
			else if (c_vector_obj_2d::is_polygon(p))
			{
				c_polygon *p_new = new c_polygon();
				c_vector_obj_2d::c_polygon* p1 = (c_vector_obj_2d::c_polygon*)p;

				geo_api::copy_xy(p1->m_pts, p_new->m_pts);
				p_new->m_color = p1->m_color;
				p_new->m_counts = p1->m_counts;
				geo_api::copy_xy(p_new->m_range.m_pt0, p1->m_range.m_pt0);
				geo_api::copy_xy(p_new->m_range.m_pt1, p1->m_range.m_pt1);

				m_list.push_back(p_new);
			}
			else if (c_vector_obj_2d::is_text(p))
			{
				c_text *p_new = new c_text();
				c_vector_obj_2d::c_text* p1 = (c_vector_obj_2d::c_text*)p;

				p_new->m_pt.x = p1->m_pt.x;
				p_new->m_pt.y = p1->m_pt.y;
				p_new->m_color = p1->m_color;
				p_new->m_text = p1->m_text;
				p_new->m_text_style = p1->m_text_style;
				p_new->m_bglobal_size = p1->m_bglobal_size;
				p_new->m_bupward = p1->m_bupward;
				geo_api::copy_xy(p_new->m_range.m_pt0, p1->m_range.m_pt0);
				geo_api::copy_xy(p_new->m_range.m_pt1, p1->m_range.m_pt1);

				m_list.push_back(p_new);
			}
		}

		move(obj->m_origin_x, obj->m_origin_y, 0);
	}

	rect_2d c_vector_obj_2d::get_bound(double view_scale)
	{
		rect_2d rc;
		for (std::list<c_vector_obj_2d::c_head*>::iterator it = m_list.begin(); it != m_list.end(); it++)
		{
			rc = (rc | get_bound(*it, view_scale));
		}
		return rc;
	}

	rect_3d c_vector_obj_3d::get_bound(double view_scale)
	{
		rect_3d rc;
		for (std::list<c_vector_obj_3d::c_head*>::iterator it = m_list.begin(); it != m_list.end(); it++)
		{
			rc = (rc | get_bound(*it, view_scale));
		}
		return rc;
	}
	
	rect_2d c_vector_obj_2d::get_bound(const c_vector_obj_2d::c_head* p, double view_scale)
	{
		if (is_point(p))
		{
			c_point *p1 = (c_point*)p;
			double size = 0;
			if (p1->m_bglobal_size)
			{
				size = p1->m_size * view_scale*0.5;
			}
			else
			{
				size = p1->m_size * 0.5;
			}

			return rect_2d(p1->m_pt.x - size, p1->m_pt.y - size, p1->m_pt.x + size, p1->m_pt.y + size);
		}
		else if (is_cell(p))
		{
			c_cell *p1 = (c_cell*)p;
			double size = 5e-5;

			c_drawing_base::cell *pcell = c_drawing_base::obj()->get_cell(p1->m_cell_index);
			if (pcell == NULL)
			{
				return rect_2d(p1->m_pt.x - size, p1->m_pt.y - size, p1->m_pt.x + size, p1->m_pt.y + size);
			}

			double scale = 1.0;
			if (p1->m_bglobal_scale)
				scale = view_scale;

			double scale_x = scale*p1->m_scale_x, scale_y = scale*p1->m_scale_y;

			rect_2d rc = pcell->rect;
			if (p1->m_angle == 0.0f)
			{
				rc.x0 *= scale_x;	rc.x1 *= scale_x;
				rc.y0 *= scale_y;	rc.y1 *= scale_y;
				rc.offset(p1->m_pt.x, p1->m_pt.y);
				return rc;
			}
			else
			{
				scale = scale_x < scale_y ? scale_x : scale_y;
				double r = pcell->radius*scale;
				rc = rect_2d(-r, -r, r, r);
				rc.offset(p1->m_pt.x, p1->m_pt.y);

				return rc;
			}
		}
		else if (is_point_string(p))
		{
			c_point_string *p1 = (c_point_string*)p;
			rect_2d rc_pts = rect_from_corner_2<double>(p1->m_range.m_pt0, p1->m_range.m_pt1);
			double size = 0;
			if (p1->m_bglobal_size)
			{
				size = p1->m_size * view_scale*0.5;
			}
			else
			{
				size = p1->m_size * 0.5;
			}

			rc_pts.inflate(size, size);
			return rc_pts;
		}
		else if (is_cell_string(p))
		{
			rect_2d rc_cell;
			double size = 5e-5;

			c_cell_string *p1 = (c_cell_string*)p;
			c_drawing_base::cell *pcell = c_drawing_base::obj()->get_cell(p1->m_cell_index);
			if (pcell == NULL)
			{
				rc_cell = rect_2d(-size, -size, size, size);
			}

			double scale = 1.0;
			if (p1->m_bglobal_scale)
				scale = view_scale;
			
			double scale_x = scale*p1->m_scale_x, scale_y = scale*p1->m_scale_y;
			scale = scale_x < scale_y ? scale_x : scale_y;
			double r = pcell->radius*scale;
			rect_2d rc_pts = rect_from_corner_2<double>(p1->m_range.m_pt0, p1->m_range.m_pt1);
			rc_pts.x0 = rc_pts.x0 - r;
			rc_pts.x1 = rc_pts.x1 + r;
			rc_pts.y0 = rc_pts.y0 - r;
			rc_pts.y1 = rc_pts.y1 + r;
			return rc_pts;
		}
		else if (is_line_string(p))
		{
			c_line_string *p1 = (c_line_string*)p;
			rect_2d rc_pts = rect_from_corner_2<double>(p1->m_range.m_pt0, p1->m_range.m_pt1);

			float line_width = p1->m_line_width;
			if (p1->m_bglobal_width)
				line_width = view_scale * line_width;

			rc_pts.inflate(line_width, line_width);
			return rc_pts;
		}
		else if (is_polygon(p))
		{
			c_polygon *p1 = (c_polygon*)p;
			rect_2d rc_pts = rect_from_corner_2<double>(p1->m_range.m_pt0, p1->m_range.m_pt1);

			return rc_pts;
		}
		else if (is_text(p))
		{
			c_text *p1 = (c_text*)p;
			rect_2d rc = rect_from_corner_2<double>(p1->m_range.m_pt0, p1->m_range.m_pt1);
			rc.offset(p1->m_pt.x, p1->m_pt.y);

			return rc;
		}

		return rect_2d();
	}

	rect_3d c_vector_obj_3d::get_bound(const c_vector_obj_3d::c_head* p, double view_scale)
	{
		if (is_point(p))
		{
			c_point *p1 = (c_point*)p;
			double size = 0;
			if (p1->m_bglobal_size)
			{
				size = p1->m_size * view_scale*0.5;
			}
			else
			{
				size = p1->m_size * 0.5;
			}

			return rect_3d(p1->m_pt.x - size, p1->m_pt.y - size, p1->m_pt.z - size, p1->m_pt.x + size, p1->m_pt.y + size, p1->m_pt.z + size);
		}
		else if (is_cell(p))
		{
			c_cell *p1 = (c_cell*)p;
			double size = 5e-5;

			c_drawing_base::cell *pcell = c_drawing_base::obj()->get_cell(p1->m_cell_index);
			if (pcell == NULL)
			{
				return rect_3d(p1->m_pt.x - size, p1->m_pt.y - size, p1->m_pt.z - size, p1->m_pt.x + size, p1->m_pt.y + size, p1->m_pt.z + size);
			}

			double scale = 1.0;
			if (p1->m_bglobal_scale)
				scale = view_scale;

			double scale_x = scale*p1->m_scale_x, scale_y = scale*p1->m_scale_y;

			rect_2d rc = pcell->rect;
			if (p1->m_angle == 0.0f)
			{
				rc.x0 *= scale_x;	rc.x1 *= scale_x;
				rc.y0 *= scale_y;	rc.y1 *= scale_y;
				rc.offset(p1->m_pt.x, p1->m_pt.y);
				return rect_3d(rc.x0, rc.y0, p1->m_pt.z - size, rc.x1, rc.y1, p1->m_pt.z + size);
			}
			else
			{
				scale = scale_x < scale_y ? scale_x : scale_y;
				double r = pcell->radius*scale;
				rect_3d rc = rect_3d(-r, -r, -r, r, r, r);
				rc.offset(p1->m_pt.x, p1->m_pt.y, p1->m_pt.z);

				return rc;
			}
		}
		else if (is_point_string(p))
		{
			c_point_string *p1 = (c_point_string*)p;
			rect_3d rc_pts = rect_from_corner_3<double>(p1->m_range.m_pt0, p1->m_range.m_pt1);
			double size = 0;
			if (p1->m_bglobal_size)
			{
				size = p1->m_size * view_scale*0.5;
			}
			else
			{
				size = p1->m_size * 0.5;
			}

			rc_pts.inflate(size, size, size);
			return rc_pts;
		}
		else if (is_cell_string(p))
		{
			rect_3d rc_cell;
			double size = 5e-5;

			c_cell_string *p1 = (c_cell_string*)p;
			c_drawing_base::cell *pcell = c_drawing_base::obj()->get_cell(p1->m_cell_index);
			if (pcell == NULL)
			{
				rc_cell = rect_3d(-size, -size, -size, size, size, size);
			}

			double scale = 1.0;
			if (p1->m_bglobal_scale)
				scale = view_scale;

			double scale_x = scale*p1->m_scale_x, scale_y = scale*p1->m_scale_y;
			scale = scale_x < scale_y ? scale_x : scale_y;
			double r = pcell->radius*scale;
			rect_3d rc_pts = rect_from_corner_3<double>(p1->m_range.m_pt0, p1->m_range.m_pt1);
			rc_pts.x0 = rc_pts.x0 - r;
			rc_pts.x1 = rc_pts.x1 + r;
			rc_pts.y0 = rc_pts.y0 - r;
			rc_pts.y1 = rc_pts.y1 + r;
			rc_pts.z0 = rc_pts.z0 - size;
			rc_pts.z1 = rc_pts.z1 + size;
			return rc_pts;
		}
		else if (is_line_string(p))
		{
			c_line_string *p1 = (c_line_string*)p;
			rect_3d rc_pts = create_rect3d_from_points(p1->m_pts.data(), p1->m_pts.size());

			float line_width = p1->m_line_width;
			if (p1->m_bglobal_width)
				line_width = view_scale * line_width;

			rc_pts.inflate(line_width, line_width, line_width);
			return rc_pts;
		}
		else if (is_polygon(p))
		{
			c_polygon *p1 = (c_polygon*)p;
			rect_3d rc_pts = create_rect3d_from_points(p1->m_pts.data(), p1->m_pts.size());

			return rc_pts;
		}
		else if (is_text(p))
		{
			c_text *p1 = (c_text*)p;
			rect_3d rc = rect_from_corner_3<double>(p1->m_range.m_pt0, p1->m_range.m_pt1);
			rc.offset(p1->m_pt.x, p1->m_pt.y,0);

			return rc;
		}

		return rect_3d();
	}

}
