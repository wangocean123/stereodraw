#include "StdAfx.h"
#include "c_octree.h"
#include "c_geometry.h"
#include "c_geo_point.h"
#include "c_geo_line.h"
#include "c_geo_polygon.h"
#include "c_geo_text.h"
#include "geo_algorithom.h"


#define query_distance_null_value		1e+10

namespace gis
{
	box_obj::box_obj()
	{
		type = e_point;
		geo = NULL;
	}

	box_obj::~box_obj()
	{

	}

	gd::Rect3D box_obj::getBound()
	{
		if (type == e_point)
		{
			gd::Point3D pt = ((box_point*)this)->pt;
			return gd::rect_from_center_3<double>(pt, geo_api::get_toler_xy(), geo_api::get_toler_xy(), geo_api::get_toler_z());
		}
		else if (type == e_lines)
		{
			return ((box_lines*)this)->rect.rect;
		}
		else if (type == e_polygon)
		{
			return ((box_polygon*)this)->rect.rect;
		}
		else if (type == e_vector_obj)
		{
			return gd::GraphicsObject3D::getBound(((box_graphics_obj*)this)->vect_section, 1.0);
		}

		return gd::Rect3D();
	}

	box_point::box_point()
	{
		type = e_point;
	}

	box_lines::box_lines()
	{
		type = e_lines;
	}

	box_polygon::box_polygon()
	{
		type = e_polygon;
	}

	c_octree_node::c_octree_node()
	{
		memset(m_nodes, 0, sizeof(m_nodes));
		m_node_bound = gd::Rect3D(-1e+10, -1e+10, -1e+10, 1e+10, 1e+10, 1e+10);
	}

	c_octree_node::~c_octree_node()
	{
		int size = sizeof(m_nodes) / sizeof(m_nodes[0]);
		for (int i = 0; i < size; i++)
		{
			if (m_nodes[i])delete m_nodes[i];
		}
	}

	void c_octree_node::create_sub_nodes()
	{
		gd::Rect3D rc_real;
		if (m_objs.size()>0)
		{
			rc_real = get_real_bound();
		}
		else
		{
			rc_real = m_node_bound;
		}

		for (int i = 0; i < 8; i++)
		{
			if (m_nodes[i])delete m_nodes[i];
			m_nodes[i] = new c_octree_node();

			//由低到高的3位的0/1分别代表xyz的两个半轴
			if ((i & 1) == 0)
			{
				m_nodes[i]->m_node_bound.x0 = m_node_bound.x0;
				m_nodes[i]->m_node_bound.x1 = rc_real.center_x();
			}
			else
			{
				m_nodes[i]->m_node_bound.x0 = rc_real.center_x();
				m_nodes[i]->m_node_bound.x1 = m_node_bound.x1;
			}

			if ((i & 2) == 0)
			{
				m_nodes[i]->m_node_bound.y0 = m_node_bound.y0;
				m_nodes[i]->m_node_bound.y1 = rc_real.center_y();
			}
			else
			{
				m_nodes[i]->m_node_bound.y0 = rc_real.center_y();
				m_nodes[i]->m_node_bound.y1 = m_node_bound.y1;
			}

			if ((i & 4) == 0)
			{
				m_nodes[i]->m_node_bound.z0 = m_node_bound.z0;
				m_nodes[i]->m_node_bound.z1 = rc_real.center_z();
			}
			else
			{
				m_nodes[i]->m_node_bound.z0 = rc_real.center_z();
				m_nodes[i]->m_node_bound.z1 = m_node_bound.z1;
			}
		}
	}

	void c_octree_node::split()
	{
		if (m_nodes[0] == NULL)
			create_sub_nodes();

		std::vector<box_obj*> objs;
		int size = m_objs.size();
		for (int i = 0; i < size; i++)
		{
			gd::Rect3D rc = m_objs[i]->getBound();
			box_obj *p = m_objs[i];

			for (int j = 0; j < 8; j++)
			{
				if (m_nodes[j]->m_node_bound.is_contain(rc))
				{
					m_nodes[j]->m_objs.push_back(p);
					p = NULL;
					break;
				}
			}
			if (p != NULL)
			{
				objs.push_back(p);
			}
		}
		m_objs = objs;
	}

	static void insert_in_order(std::vector<box_obj*>& objs, box_obj *obj)
	{
		int size = objs.size();
		if (size <= 0)
		{
			objs.push_back(obj);
			return;
		}

		//二分法查找
		int start = 0, end = size - 1;
		int mid = (start + end) / 2;

		while (start<end && obj->geo != objs[mid]->geo)
		{
			if (obj->geo < objs[mid]->geo)
			{
				end = mid - 1;
			}
			else if (obj->geo > objs[mid]->geo)
			{
				start = mid + 1;
			}

			mid = (start + end) / 2;
		}

		if (obj->geo < objs[mid]->geo)
		{
			objs.insert(objs.begin() + mid, obj);
		}
		else
		{
			objs.insert(objs.begin() + mid + 1, obj);
		}
	}

	void c_octree_node::insert(box_obj *obj, gd::Rect3D *rc_obj)
	{
		if (m_nodes[0] == NULL)
		{
			if (m_objs.size() < 1000)
			{
				//m_objs.push_back(obj);
				insert_in_order(m_objs, obj);
				return;
			}
			else
			{
				split();
			}
		}		
		for (int i = 0; i < 8; i++)
		{
			if (m_nodes[i]->m_node_bound.is_contain(*rc_obj))
			{
				m_nodes[i]->insert(obj, rc_obj);
				return;
			}
		}

		//m_objs.push_back(obj);
		insert_in_order(m_objs, obj);
	}

	gd::Rect3D c_octree_node::get_real_bound()
	{
		gd::Rect3D rc;
		int size = m_objs.size();
		for (int i = 0; i < size; i++)
		{
			rc.union_rect(m_objs[i]->getBound());
		}
		return rc;
	}

	void c_octree_node::remove(const gis::c_geometry *geo_obj)
	{
		int size = m_objs.size();
		if (size <= 0)
		{
			return;
		}

		int index = -1;
		//二分法查找
		if (geo_obj <= m_objs[size - 1]->geo && geo_obj >= m_objs[0]->geo)
		{
			int start = 0, end = size - 1;
			int mid = (start + end) / 2;

			while (start<end && geo_obj != m_objs[mid]->geo)
			{
				if (geo_obj < m_objs[mid]->geo)
				{
					end = mid - 1;
				}
				else if (geo_obj > m_objs[mid]->geo)
				{
					start = mid + 1;
				}

				mid = (start + end) / 2;
			}

			if (geo_obj == m_objs[mid]->geo)
				index = mid;
		}

		if (index != -1)
		{
			int i1 = 0, i2 = size;
			for (int i = index-1; i >= 0; i--)
			{
				if (geo_obj != m_objs[i]->geo)
				{
					i1 = i+1;
					break;
				}
			}
			for (int i = index+1; i<size; i++)
			{
				if (geo_obj != m_objs[i]->geo)
				{
					i2 = i;
					break;
				}
			}

			for (int i = i1; i < i2; i++)
			{
				delete m_objs[i];
			}

			m_objs.erase(m_objs.begin() + i1, m_objs.begin() + i2);
		}

		if (m_nodes[0] != NULL)
		{
			for (int i = 0; i < 8; i++)
			{
				m_nodes[i]->remove(geo_obj);
			}
		}
	}

	void c_octree_node::remove_all()
	{
		int size = m_objs.size();
		for (int i = 0; i < size; i++)
		{
			delete m_objs[i];
		}
		m_objs.clear();

		if (m_nodes[0] != NULL)
		{
			for (int i = 0; i < 8; i++)
			{
				m_nodes[i]->remove_all();
				delete m_nodes[i];
				m_nodes[i] = NULL;
			}
		}
	}

	c_octree::c_octree()
	{
	}


	c_octree::~c_octree()
	{
		remove_all();
	}


	void create_range_obj_from_geometry(const gis::c_geometry *geo, box_obj_array& arr)
	{
		int geo_type = geo->get_geometry_type();
		if (geo_type == gis::GEO_POINT)
		{
			box_point *pobj = new box_point();
			gd::vertex_array pts;
			((gis::c_geo_point*)geo)->get_vertexs(pts);
			pobj->pt = pts[0];
			pobj->geo = geo;
			arr.push_back(pobj);
		}
		else if (geo_type == gis::GEO_MULTIPOINT)
		{
			const gis::c_geo_multi_point *geo2 = dynamic_cast<const gis::c_geo_multi_point*>(geo);
			if (!geo2)return;

			gis::c_geo_linear_obj::shape_rect_array rects;
			geo2->get_shape_rects(rects);

			for (int i = 0; i < rects.size(); i++)
			{
				box_lines *pobj = new box_lines();
				pobj->rect = rects[i];
				pobj->geo = geo;
				arr.push_back(pobj);
			}
		}
		else if (geo_type == gis::GEO_LINESTRING || geo_type==gis::GEO_DOUBLELINE || geo_type==gis::GEO_POLYGON || geo_type==gis::GEO_MULTIPOLYGON )
		{
			const gis::c_geo_linear_obj *geo2 = dynamic_cast<const gis::c_geo_linear_obj*>(geo);
			if (!geo2)return;

			gis::c_geo_linear_obj::shape_rect_array rects;
			geo2->get_shape_rects(rects);

			for (int i = 0; i < rects.size(); i++)
			{
				box_lines *pobj = new box_lines();
				pobj->rect = rects[i];
				pobj->geo = geo;
				arr.push_back(pobj);
			}
		}

		if ((geo_type == gis::GEO_POLYGON || geo_type == gis::GEO_MULTIPOLYGON) && ((const gis::c_geo_polygon*)geo)->is_solid_color())
		{
			box_polygon *pobj = new box_polygon();
			pobj->rect = ((const gis::c_geo_polygon*)geo)->get_whole_shape_rect();
			pobj->geo = geo;
			arr.push_back(pobj);
		}
	}

	void c_octree::insert(const gis::c_geometry* geo)
	{
		box_obj_array arr;
		create_range_obj_from_geometry(geo,arr);
		for (int i = 0; i < arr.size(); i++)
		{
			m_root.insert(arr[i], &arr[i]->getBound());
		}
	}

	void c_octree::insert(const gis::c_geometry* geo, gd::GraphicsObject3D *vect)
	{
		gd::GraphicsObject3D::section_list sections;
		vect->getSections(sections);

		gd::GraphicsObject3D *vect_new = new gd::GraphicsObject3D();
		vect_new->copyFrom0(vect);

		gd::GraphicsObject3D::section_list::iterator it = sections.begin();
		for (int i = 0; i < sections.size(); i++, it++)
		{
			box_graphics_obj *obj = new box_graphics_obj();
			obj->geo = geo;
			obj->vect = vect_new;
			obj->vect_section = *it;
			m_root.insert(obj, &obj->getBound());
		}
	}

	void c_octree::remove_all()
	{
		m_root.remove_all();
	}

	void c_octree::remove(const gis::c_geometry* geo)
	{
		m_root.remove(geo);
	}

	namespace
	{
		struct geo_dis
		{
			const gis::c_geometry *geo;
			double dis;
		};

		static int sort_func_geo(const void *geo1, const void *geo2)
		{
			const gis::c_geometry *p1 = (const gis::c_geometry*)geo1;
			const gis::c_geometry *p2 = (const gis::c_geometry*)geo2;
			if (p1 < p2)
				return -1;
			else if (p1>p2)
				return 1;
			else
				return 0;
		}

		static int sort_func_geo_dis_bygeo(const void *geo1, const void *geo2)
		{
			const geo_dis *p1 = (const geo_dis*)geo1;
			const geo_dis *p2 = (const geo_dis*)geo2;
			if (p1->geo < p2->geo)
				return -1;
			else if (p1->geo>p2->geo)
				return 1;
			else
			{
				if (p1->dis < p2->dis)
					return -1;
				else if (p1->dis>p2->dis)
					return 1;
				return 0;
			}
		}

		static int sort_func_geo_dis_bydis(const void *geo1, const void *geo2)
		{
			const geo_dis *p1 = (const geo_dis*)geo1;
			const geo_dis *p2 = (const geo_dis*)geo2;
			if (p1->dis < p2->dis)
				return -1;
			else if (p1->dis>p2->dis)
				return 1;
			else
				return 0;
		}
	}

	void c_octree::query_rect(gd::Rect3D rect, const gd::matrix4d* m, gis::const_geometry_array& arr)
	{
		if (rect.deepth() < 0)
		{
			rect.z0 = -1e+10;
			rect.z1 = 1e+10;
		}

		query_rect_data data;
		data.arr = &arr;
		data.m = NULL;
		data.rect = rect;

		if (m != NULL && matrix4d_is_zrotate(*m))
		{
			data.m = m;
		}

		do_query_loop(&m_root, &c_octree::compare_rect, &data);

		if (arr.size() <= 0)
			return;

		//对结果作排序，去除重复
		std::qsort(arr.data(), arr.size(), sizeof(gis::c_geometry*), &sort_func_geo);

		gis::const_geometry_array arr2;
		arr2.push_back(arr[0]);
		for (int i = 1; i < arr.size(); i++)
		{
			if (arr[i] != arr[i - 1])
			{
				arr2.push_back(arr[i]);
			}
		}

		arr = arr2;
	}

	void c_octree::query_nearest(gd::Point3D pt, double r, const gd::matrix4d* m, gis::const_geometry_array& arr, std::vector<double>* dis_arr)
	{
		query_nearest(pt, gd::rect_from_center_3<double>(pt,r*2,r*2,0), m, arr, dis_arr);
	}

	void c_octree::query_nearest(gd::Point3D pt, gd::Rect3D rect, const gd::matrix4d* m, gis::const_geometry_array& arr, std::vector<double>* dis_arr)
	{
		if (rect.deepth() < 0)
		{
			rect.z0 = -1e+10;
			rect.z1 = 1e+10;
		}

		query_nearest_data data;
		data.arr = &arr;
		data.m = NULL;
		data.rect = rect;
		data.pt = pt;

		if (m != NULL && matrix4d_is_zrotate(*m))
		{
			data.m = m;
		}

		do_query_loop(&m_root, &c_octree::compare_nearest, &data);

		if (arr.size() <= 0)
			return;

		std::vector<geo_dis> arr1;
		arr1.resize(arr.size());
		for (int i = 0; i < arr.size(); i++)
		{
			arr1[i].geo = arr[i];
			arr1[i].dis = data.distances[i];
		}

		//对结果作排序，去除重复geo对象
		std::qsort(arr1.data(), arr1.size(), sizeof(geo_dis), &sort_func_geo_dis_bygeo);

		std::vector<geo_dis> arr2;
		arr2.push_back(arr1[0]);
		for (int i = 1; i < arr1.size(); i++)
		{
			if (arr1[i].geo != arr1[i - 1].geo)
			{
				arr2.push_back(arr1[i]);
			}
		}

		//按dis排序
		std::qsort(arr2.data(), arr2.size(), sizeof(geo_dis), &sort_func_geo_dis_bydis);
		arr.resize(arr2.size());
		for (int i = 0; i < arr2.size(); i++)
		{
			arr[i] = (gis::c_geometry*)arr2[i].geo;
		}

		if (dis_arr != NULL)
		{
			dis_arr->resize(arr2.size());
			for (int i = 0; i < arr2.size(); i++)
			{
				(*dis_arr)[i] = arr2[i].dis;
			}
		}
	}

	void c_octree::do_query_loop(c_octree_node *node, compare_func func, query_data *data)
	{
		if (!node->m_node_bound.is_intersect(data->rect))
		{
			return;
		}

		if (node->m_objs.size() > 0)
			(this->*func)(node, data);

		if (node->m_nodes[0] != NULL)
		{
			for (int i = 0; i < 8; i++)
			{
				do_query_loop(node->m_nodes[i],func,data);
			}
		}
	}

	void c_octree::compare_rect(c_octree_node *node, query_data *data)
	{
		query_rect_data *d = (query_rect_data*)data;

		int size = node->m_objs.size();

		if (d->m == NULL)
		{
			for (int i = 0; i < size; i++)
			{
				box_obj *obj = node->m_objs[i];

				//矩形比较：无矩阵，点对象
				if (obj->type == box_obj::e_point)
				{
					if (data->rect.is_contain_pt(((box_point*)obj)->pt))
					{
						data->arr->push_back((gis::c_geometry*)obj->geo);
					}
					continue;
				}

				if (!data->rect.is_intersect_2(obj->getBound()))
					continue;

				//矩形比较：无矩阵，线对象
				if (obj->type == box_obj::e_lines)
				{
					box_lines *obj2 = (box_lines*)obj;
					gis::c_geo_linear_obj::shape_rect rect = obj2->rect;
					const gd::GeoVertex *pts = rect.pvertexs->data();

					for (int i = 0; i < rect.pt_count - 1; i++)
					{
						if (data->rect.is_intersect_pts(pts[rect.start_index+i], pts[rect.start_index+i + 1]))
						{
							data->arr->push_back((gis::c_geometry*)obj->geo);
							break;
						}
					}
				}
				//矩形比较：无矩阵，面对象
				else if (obj->type == box_obj::e_polygon)
				{
					box_polygon *obj2 = (box_polygon*)obj;
					gis::c_geo_linear_obj::shape_rect rect = obj2->rect;
					const gd::GeoVertex *pts = rect.pvertexs->data();

					int i0 = rect.start_index, j = 0;
					for (j = 0; j < rect.pt_count - 1; j++)
					{
						if (pts[i0 + j + 1].pen_code != gd::GeoVertex::code_start &&
							data->rect.is_intersect_pts(pts[i0 + j], pts[i0 + j + 1]))
						{
							data->arr->push_back((gis::c_geometry*)obj->geo);
							break;
						}
					}

					if (j >= rect.pt_count - 1)
					{
						if (rect.rect.is_contain_pt(pts[i0]))
						{
							data->arr->push_back((gis::c_geometry*)obj->geo);
						}
						else if (gis::c_geo_multi_polygon::is_contain_pt(*(rect.pvertexs),gd::Point3D(rect.rect.x0, rect.rect.y0, 0)))
						{
							data->arr->push_back((gis::c_geometry*)obj->geo);
						}
					}
				}
				//矩形比较：无矩阵，图形对象
				else if (obj->type == box_obj::e_vector_obj)
				{
					box_graphics_obj *obj2 = (box_graphics_obj*)obj;
					//矩形比较：无矩阵，图形点对象
					if (gd::GraphicsObject3D::isPoint(obj2->vect_section) || gd::GraphicsObject3D::isCell(obj2->vect_section))
					{
						data->arr->push_back((gis::c_geometry*)obj->geo);
					}
					//矩形比较：无矩阵，图形点串对象
					else if (gd::GraphicsObject3D::isPointString(obj2->vect_section))
					{
						gd::GraphicsObject3D::PointString *section = (gd::GraphicsObject3D::PointString*)obj2->vect_section;
						const gd::Point3D *pts = section->pts.data();
						int npt = section->pts.size();

						for (int j = 0; j < npt; j++)
						{
							if (data->rect.is_contain_pt(pts[j]))
							{
								data->arr->push_back((gis::c_geometry*)obj->geo);
								break;
							}
						}
					}
					//矩形比较：无矩阵，图形单元串对象
					else if (gd::GraphicsObject3D::isCellString(obj2->vect_section))
					{
						gd::GraphicsObject3D::BlockString *section = (gd::GraphicsObject3D::BlockString*)obj2->vect_section;
						const gd::Point3D *pts = section->pts.data();
						int npt = section->pts.size();

						gd::GraphicsObject3D::BlockString s2;
						s2 = *section;
						s2.pts.resize(1);
						gd::Rect3D rc0 = gd::GraphicsObject3D::getBound(&s2, 1.0);
						gd::Point3D pt0 = pts[0];

						for (int j = 0; j < npt; j++)
						{
							gd::Point3D pt1 = pts[j];
							gd::Rect3D rc = rc0;
							rc.offset(pt1.x-pt0.x,pt1.y-pt0.y,0);
							if (data->rect.is_intersect_2(rc))
							{
								data->arr->push_back((gis::c_geometry*)obj->geo);
								break;
							}
						}
					}
					//矩形比较：无矩阵，图形线对象
					else if (gd::GraphicsObject3D::isLineString(obj2->vect_section))
					{
						gd::GraphicsObject3D::LineString *section = (gd::GraphicsObject3D::LineString*)obj2->vect_section;
						const gd::Point3D *pts = section->pts.data();
						int npt = section->pts.size();

						for (int j = 0; j < npt - 1; j++)
						{
							if (data->rect.is_intersect_pts(pts[j], pts[j + 1]))
							{
								data->arr->push_back((gis::c_geometry*)obj->geo);
								break;
							}
						}
					}
					//矩形比较：无矩阵，图形面对象
					else if (gd::GraphicsObject3D::isPolygon(obj2->vect_section))
					{
						gd::GraphicsObject3D::c_polygon *section = (gd::GraphicsObject3D::c_polygon*)obj2->vect_section;
						const gd::Point3D *pts = section->pts.data();
						int npt = section->pts.size(), j;

						for (j = 0; j < npt - 1; j++)
						{
							if (data->rect.is_intersect_pts(pts[j], pts[j + 1]))
							{
								data->arr->push_back((gis::c_geometry*)obj->geo);
								break;
							}
						}

						if (j >= npt - 1)
						{
							if (data->rect.is_contain_pt(pts[0]))
							{
								data->arr->push_back((gis::c_geometry*)obj->geo);
							}
							else if (gis::c_geo_multi_polygon::is_contain_pt(section->pts, section->counts, gd::Point3D(data->rect.x0, data->rect.y0, 0)))
							{
								data->arr->push_back((gis::c_geometry*)obj->geo);
							}
						}
					}
					//矩形比较：无矩阵，图形文字对象
					else if (gd::GraphicsObject3D::isText(obj2->vect_section))
					{
						data->arr->push_back((gis::c_geometry*)obj->geo);
					}
				}
			}
		}
		else
		{
			for (int i = 0; i < size; i++)
			{
				box_obj *obj = node->m_objs[i];
				//矩形比较：有矩阵，点对象
				if (obj->type == box_obj::e_point)
				{
					if (data->rect.is_contain_pt(geo_api::transform_point3d(((box_point*)obj)->pt, *data->m)))
					{
						data->arr->push_back((gis::c_geometry*)obj->geo);
					}
					continue;
				}

				if (!data->rect.is_intersect_2(geo_api::transform_rect3d(obj->getBound(), *data->m)))
					continue;
				
				//矩形比较：有矩阵，线对象
				if (obj->type == box_obj::e_lines)
				{
					box_lines *obj2 = (box_lines*)obj;
					gis::c_geo_linear_obj::shape_rect rect = obj2->rect;
					const gd::GeoVertex *pts = rect.pvertexs->data();

					gd::Point3D pt1, pt2;
					pt1 = pt2 = geo_api::transform_point3d(pts[rect.start_index], *data->m);
					for (int j = 1; j < rect.pt_count; j++)
					{
						pt1 = pt2;
						pt2 = geo_api::transform_point3d(pts[rect.start_index + j], *data->m);
						if (data->rect.is_intersect_pts(pt1, pt2))
						{
							data->arr->push_back((gis::c_geometry*)obj->geo);
							break;
						}
					}
				}
				//矩形比较：有矩阵，面对象
				else if (obj->type == box_obj::e_polygon)
				{
					box_lines *obj2 = (box_lines*)obj;
					gis::c_geo_linear_obj::shape_rect rect = obj2->rect;
					const gd::GeoVertex *pts = rect.pvertexs->data();

					gd::vertex_array pts2;
					gd::GeoVertex pt1, pt2;
					pt1 = pt2 = geo_api::transform_point3d(pts[rect.start_index], *data->m);
					pts2.push_back(pt1);

					int i0 = rect.start_index, j = 0;
					for (j = 1; j < rect.pt_count; j++)
					{
						pt1 = pt2;
						pt2 = geo_api::transform_point3d(pts[i0 + j], *data->m);
						pts2.push_back(pt2);

						if (data->rect.is_intersect_pts(pt1, pt2))
						{
							data->arr->push_back((gis::c_geometry*)obj->geo);
							break;
						}
					}

					if (j >= rect.pt_count - 1)
					{
						if (rect.rect.is_contain_pt(geo_api::transform_point3d(pts[i0], *data->m)))
						{
							data->arr->push_back((gis::c_geometry*)obj->geo);
						}
						else if (gis::c_geo_multi_polygon::is_contain_pt(pts2, gd::Point3D(rect.rect.x0, rect.rect.y0, 0)))
						{
							data->arr->push_back((gis::c_geometry*)obj->geo);
						}
					}
				}
				//矩形比较：有矩阵，图形对象
				else if (obj->type == box_obj::e_vector_obj)
				{
					box_graphics_obj *obj2 = (box_graphics_obj*)obj;
					//矩形比较：有矩阵，图形点对象
					if (gd::GraphicsObject3D::isPoint(obj2->vect_section) || gd::GraphicsObject3D::isCell(obj2->vect_section))
					{
						data->arr->push_back((gis::c_geometry*)obj->geo);
					}
					//矩形比较：有矩阵，图形点串对象
					else if (gd::GraphicsObject3D::isPointString(obj2->vect_section))
					{
						gd::GraphicsObject3D::PointString *section = (gd::GraphicsObject3D::PointString*)obj2->vect_section;
						const gd::Point3D *pts = section->pts.data();
						int npt = section->pts.size();

						for (int j = 0; j < npt; j++)
						{
							if (data->rect.is_contain_pt(geo_api::transform_point3d(pts[j], *data->m)))
							{
								data->arr->push_back((gis::c_geometry*)obj->geo);
								break;
							}
						}
					}
					//矩形比较：有矩阵，图形单元串对象
					else if (gd::GraphicsObject3D::isCellString(obj2->vect_section))
					{
						gd::GraphicsObject3D::BlockString *section = (gd::GraphicsObject3D::BlockString*)obj2->vect_section;
						const gd::Point3D *pts = section->pts.data();
						int npt = section->pts.size();

						gd::GraphicsObject3D::BlockString s2;
						s2 = *section;
						s2.pts.resize(1);
						gd::Rect3D rc0 = gd::GraphicsObject3D::getBound(&s2, 1.0);
						gd::Point3D pt0 = geo_api::transform_point3d(pts[0], *data->m);

						for (int j = 0; j < npt; j++)
						{
							gd::Point3D pt1 = geo_api::transform_point3d(pts[j], *data->m);
							gd::Rect3D rc = rc0;
							rc.offset(pt1.x - pt0.x, pt1.y - pt0.y, 0);
							if (data->rect.is_intersect_2(rc))
							{
								data->arr->push_back((gis::c_geometry*)obj->geo);
								break;
							}
						}
					}
					//矩形比较：有矩阵，图形线对象
					else if (gd::GraphicsObject3D::isLineString(obj2->vect_section))
					{
						gd::GraphicsObject3D::LineString *section = (gd::GraphicsObject3D::LineString*)obj2->vect_section;
						const gd::Point3D *pts = section->pts.data();
						int npt = section->pts.size();

						gd::Point3D pt1, pt2;
						pt1 = pt2 = geo_api::transform_point3d(pts[0], *data->m);
						for (int j = 1; j < npt; j++)
						{
							pt1 = pt2;
							pt2 = geo_api::transform_point3d(pts[j], *data->m);
							if (data->rect.is_intersect_pts(pt1, pt2))
							{
								data->arr->push_back((gis::c_geometry*)obj->geo);
								break;
							}
						}
					}
					//矩形比较：有矩阵，图形面对象
					else if (gd::GraphicsObject3D::isPolygon(obj2->vect_section))
					{
						gd::GraphicsObject3D::c_polygon *section = (gd::GraphicsObject3D::c_polygon*)obj2->vect_section;
						const gd::Point3D *pts = section->pts.data();
						int npt = section->pts.size(), j;

						gd::point3d_array pts2;
						gd::Point3D pt1, pt2;
						pt1 = pt2 = geo_api::transform_point3d(pts[0], *data->m);
						pts2.push_back(pt1);

						for (j = 1; j < npt; j++)
						{
							pt1 = pt2;
							pt2 = geo_api::transform_point3d(pts[j], *data->m);
							pts2.push_back(pt2);

							if (data->rect.is_intersect_pts(pt1, pt2))
							{
								data->arr->push_back((gis::c_geometry*)obj->geo);
								break;
							}
						}

						if (j >= npt)
						{
							if (data->rect.is_contain_pt(geo_api::transform_point3d(pts[0], *data->m)))
							{
								data->arr->push_back((gis::c_geometry*)obj->geo);
							}
							else if (gis::c_geo_multi_polygon::is_contain_pt(pts2, section->counts, gd::Point3D(data->rect.x0, data->rect.y0, 0)))
							{
								data->arr->push_back((gis::c_geometry*)obj->geo);
							}
						}
					}
					//矩形比较：有矩阵，图形文字对象
					else if (gd::GraphicsObject3D::isText(obj2->vect_section))
					{
						data->arr->push_back((gis::c_geometry*)obj->geo);
					}
				}
			}
		}
	}

	void c_octree::compare_nearest(c_octree_node *node, query_data *data)
	{
		query_nearest_data *d = (query_nearest_data*)data;

		int size = node->m_objs.size();

		if (d->m == NULL)
		{
			for (int i = 0; i < size; i++)
			{
				box_obj *obj = node->m_objs[i];
				//最近点比较：无矩阵，点对象
				if (obj->type == box_obj::e_point)
				{
					if (data->rect.is_contain_pt(((box_point*)obj)->pt))
					{
						data->arr->push_back((gis::c_geometry*)obj->geo);
						d->distances.push_back(geo_api::get_dis_2d(d->pt, ((box_point*)obj)->pt));
					}
					continue;
				}

				if (!data->rect.is_intersect_2(obj->getBound()))
					continue;

				//最近点比较：无矩阵，线对象
				if (obj->type == box_obj::e_lines)
				{
					box_lines *obj2 = (box_lines*)obj;
					gis::c_geo_linear_obj::shape_rect rect = obj2->rect;
					const gd::GeoVertex *pts = rect.pvertexs->data();
					int i0 = rect.start_index;

					bool bintersect = false;
					double dis, dis_min = -1;
					for (int j = 0; j < rect.pt_count - 1; j++)
					{
						if (data->rect.is_intersect_pts(pts[i0 + j], pts[i0 + j + 1]))
						{
							if (!bintersect)
								data->arr->push_back((gis::c_geometry*)obj->geo);
							bintersect = true;
							dis = geo_api::get_dis_pt_to_lineseg((gd::Point3D)pts[i0 + j], (gd::Point3D)pts[i0 + j + 1], d->pt);
							if (dis_min<0 || dis_min>dis)
							{
								dis = dis_min;
							}
						}
					}
					if (bintersect)
					{
						d->distances.push_back(dis_min);
					}
				}
				//最近点比较：无矩阵，面对象
				else if (obj->type == box_obj::e_polygon)
				{
					box_polygon *obj2 = (box_polygon*)obj;
					gis::c_geo_linear_obj::shape_rect rect = obj2->rect;
					const gd::GeoVertex *pts = rect.pvertexs->data();

					bool bintersect = false;
					double dis, dis_min = -1;
					int i0 = rect.start_index, j = 0;
					for (j = 0; j < rect.pt_count - 1; j++)
					{
						if (pts[i0 + j + 1].pen_code != gd::GeoVertex::code_start &&
							data->rect.is_intersect_pts(pts[i0 + j], pts[i0 + j + 1]))
						{
							if (!bintersect)
								data->arr->push_back((gis::c_geometry*)obj->geo);

							bintersect = true;
							dis = geo_api::get_dis_pt_to_lineseg((gd::Point3D)pts[i0 + j], (gd::Point3D)pts[i0 + j + 1], d->pt);
							if (dis_min<0 || dis_min>dis)
							{
								dis = dis_min;
							}
						}
					}

					if (bintersect)
					{
						d->distances.push_back(dis_min);
					}
					else
					{
						if (rect.rect.is_contain_pt(pts[i0]))
						{
							data->arr->push_back((gis::c_geometry*)obj->geo);
							d->distances.push_back(geo_api::get_toler_xy());
						}
						else if (gis::c_geo_multi_polygon::is_contain_pt(*(rect.pvertexs), gd::Point3D(rect.rect.x0, rect.rect.y0, 0)))
						{
							data->arr->push_back((gis::c_geometry*)obj->geo);
							d->distances.push_back(query_distance_null_value);
						}
					}
				}
				//最近点比较：无矩阵，图形对象
				else if (obj->type == box_obj::e_vector_obj)
				{
					box_graphics_obj *obj2 = (box_graphics_obj*)obj;
					//最近点比较：无矩阵，图形点对象
					if (gd::GraphicsObject3D::isPoint(obj2->vect_section))
					{
						data->arr->push_back((gis::c_geometry*)obj->geo);
						d->distances.push_back(geo_api::get_dis_2d(d->pt, ((gd::GraphicsObject3D::Point*)obj2->vect_section)->pt));
					}
					//最近点比较：无矩阵，图形点对象
					if (gd::GraphicsObject3D::isCell(obj2->vect_section))
					{
						data->arr->push_back((gis::c_geometry*)obj->geo);
						d->distances.push_back(geo_api::get_dis_2d(d->pt, ((gd::GraphicsObject3D::Block*)obj2->vect_section)->pt));
					}					
					//最近点比较：无矩阵，图形点串对象
					else if (gd::GraphicsObject3D::isPointString(obj2->vect_section))
					{
						gd::GraphicsObject3D::PointString *section = (gd::GraphicsObject3D::PointString*)obj2->vect_section;
						const gd::Point3D *pts = section->pts.data();
						int npt = section->pts.size();

						bool bintersect = false;
						double dis, dis_min = -1;
						for (int j = 0; j < npt; j++)
						{
							if (data->rect.is_contain_pt(pts[j]))
							{
								if (!bintersect)
									data->arr->push_back((gis::c_geometry*)obj->geo);
								bintersect = true;

								dis = geo_api::get_dis_2d(d->pt, pts[j]);
								if (dis_min<0 || dis_min>dis)
								{
									dis = dis_min;
								}
							}
						}
						if (bintersect)
						{
							d->distances.push_back(dis_min);
						}
					}
					//最近点比较：无矩阵，单元串对象
					else if (gd::GraphicsObject3D::isCellString(obj2->vect_section))
					{
						gd::GraphicsObject3D::BlockString *section = (gd::GraphicsObject3D::BlockString*)obj2->vect_section;
						const gd::Point3D *pts = section->pts.data();
						int npt = section->pts.size();

						gd::GraphicsObject3D::BlockString s2;
						s2 = *section;
						s2.pts.resize(1);
						gd::Rect3D rc0 = gd::GraphicsObject3D::getBound(&s2, 1.0);
						gd::Point3D pt0 = pts[0];

						bool bintersect = false;
						double dis, dis_min = -1;

						for (int j = 0; j < npt; j++)
						{
							gd::Point3D pt1 = pts[j];
							gd::Rect3D rc = rc0;
							rc.offset(pt1.x - pt0.x, pt1.y - pt0.y, 0);
							if (data->rect.is_intersect_2(rc))
							{
								if (!bintersect)
									data->arr->push_back((gis::c_geometry*)obj->geo);
								bintersect = true;

								dis = geo_api::get_dis_2d(d->pt, pts[j]);
								if (dis_min<0 || dis_min>dis)
								{
									dis = dis_min;
								}
							}
						}
						if (bintersect)
						{
							d->distances.push_back(dis_min);
						}
					}
					//最近点比较：无矩阵，图形线串对象
					else if (gd::GraphicsObject3D::isLineString(obj2->vect_section))
					{
						gd::GraphicsObject3D::LineString *section = (gd::GraphicsObject3D::LineString*)obj2->vect_section;
						const gd::Point3D *pts = section->pts.data();
						int npt = section->pts.size();

						bool bintersect = false;
						double dis, dis_min = -1;

						for (int j = 0; j < npt - 1; j++)
						{
							if (data->rect.is_intersect_pts(pts[j], pts[j + 1]))
							{
								if (!bintersect)
									data->arr->push_back((gis::c_geometry*)obj->geo);
								bintersect = true;

								dis = geo_api::get_dis_pt_to_lineseg(pts[j], pts[j + 1], d->pt);
								if (dis_min<0 || dis_min>dis)
								{
									dis = dis_min;
								}
							}
						}

						if (bintersect)
						{
							d->distances.push_back(dis_min);
						}
					}
					//最近点比较：无矩阵，图形面对象
					else if (gd::GraphicsObject3D::isPolygon(obj2->vect_section))
					{
						gd::GraphicsObject3D::c_polygon *section = (gd::GraphicsObject3D::c_polygon*)obj2->vect_section;
						const gd::Point3D *pts = section->pts.data();
						int npt = section->pts.size(), j;

						bool bintersect = false;
						double dis, dis_min = -1;
						for (j = 0; j < npt - 1; j++)
						{
							if (data->rect.is_intersect_pts(pts[j], pts[j + 1]))
							{
								if (!bintersect)
									data->arr->push_back((gis::c_geometry*)obj->geo);
								bintersect = true;

								dis = geo_api::get_dis_pt_to_lineseg(pts[j], pts[j + 1], d->pt);
								if (dis_min<0 || dis_min>dis)
								{
									dis = dis_min;
								}
							}
						}

						if (bintersect)
						{
							d->distances.push_back(dis_min);
						}
						else
						{
							if (data->rect.is_contain_pt(pts[0]))
							{
								data->arr->push_back((gis::c_geometry*)obj->geo);
								d->distances.push_back(geo_api::get_toler_xy());
							}
							else if (gis::c_geo_multi_polygon::is_contain_pt(section->pts, section->counts, gd::Point3D(data->rect.x0, data->rect.y0, 0)))
							{
								data->arr->push_back((gis::c_geometry*)obj->geo);
								d->distances.push_back(query_distance_null_value);
							}
						}
					}
					//最近点比较：无矩阵，图形文字对象
					else if (gd::GraphicsObject3D::isText(obj2->vect_section))
					{
						data->arr->push_back((gis::c_geometry*)obj->geo);
						d->distances.push_back(query_distance_null_value*0.5);
					}
				}
			}
		}
		else
		{
			gd::Point3D pt0 = geo_api::transform_point3d(d->pt, *data->m);
			for (int i = 0; i < size; i++)
			{
				box_obj *obj = node->m_objs[i];

				//最近点比较：有矩阵，点对象
				if (obj->type == box_obj::e_point)
				{
					gd::Point3D pt1 = geo_api::transform_point3d(((box_point*)obj)->pt, *data->m);
					if (data->rect.is_contain_pt(pt1))
					{
						data->arr->push_back((gis::c_geometry*)obj->geo);
						d->distances.push_back(geo_api::get_dis_2d(pt0, pt1));
					}
					continue;
				}

				if (!data->rect.is_intersect_2(geo_api::transform_rect3d(obj->getBound(), *data->m)))
					continue;

				//最近点比较：有矩阵，线对象
				if (obj->type == box_obj::e_lines)
				{
					box_lines *obj2 = (box_lines*)obj;
					gis::c_geo_linear_obj::shape_rect rect = obj2->rect;
					const gd::GeoVertex *pts = rect.pvertexs->data();

					bool bintersect = false;
					double dis, dis_min = -1;
					int i0 = rect.start_index;

					gd::Point3D pt1, pt2;
					pt1 = pt2 = geo_api::transform_point3d(pts[i0], *data->m);
					for (int j = 1; j < rect.pt_count; j++)
					{
						pt1 = pt2;
						pt2 = geo_api::transform_point3d(pts[i0 + j], *data->m);
						if (data->rect.is_intersect_pts(pt1, pt2))
						{
							if (!bintersect)
								data->arr->push_back((gis::c_geometry*)obj->geo);

							bintersect = true;
							dis = geo_api::get_dis_pt_to_lineseg(pt1, pt2, pt0);
							if (dis_min<0 || dis_min>dis)
							{
								dis = dis_min;
							}
						}
					}

					if (bintersect)
					{
						d->distances.push_back(dis_min);
					}
				}

				//最近点比较：有矩阵，面对象
				else if (obj->type == box_obj::e_polygon)
				{
					box_lines *obj2 = (box_lines*)obj;
					gis::c_geo_linear_obj::shape_rect rect = obj2->rect;
					const gd::GeoVertex *pts = rect.pvertexs->data();

					bool bintersect = false;
					double dis, dis_min = -1;
					int i0 = rect.start_index, j;

					gd::vertex_array pts2;
					gd::GeoVertex pt1, pt2;
					pt1 = pt2 = geo_api::transform_point3d(pts[i0], *data->m);
					pts2.push_back(pt1);

					for (j = 1; j < rect.pt_count; j++)
					{
						pt1 = pt2;
						pt2 = geo_api::transform_point3d(pts[i0 + j], *data->m);
						pts2.push_back(pt2);

						if (data->rect.is_intersect_pts(pt1, pt2))
						{
							if (!bintersect)
								data->arr->push_back((gis::c_geometry*)obj->geo);

							bintersect = true;
							dis = geo_api::get_dis_pt_to_lineseg<gd::Point3D>(pt1, pt2, pt0);
							if (dis_min<0 || dis_min>dis)
							{
								dis = dis_min;
							}
						}
					}

					if (bintersect)
					{
						d->distances.push_back(dis_min);
					}
					else
					{
						if (rect.rect.is_contain_pt(geo_api::transform_point3d(pts[i0], *data->m)))
						{
							data->arr->push_back((gis::c_geometry*)obj->geo);
							d->distances.push_back(geo_api::get_toler_xy());
						}
						else if (gis::c_geo_multi_polygon::is_contain_pt(pts2, gd::Point3D(rect.rect.x0, rect.rect.y0, 0)))
						{
							data->arr->push_back((gis::c_geometry*)obj->geo);
							d->distances.push_back(query_distance_null_value);
						}						
					}
				}
				//最近点比较：有矩阵，图形对象
				else if (obj->type == box_obj::e_vector_obj)
				{
					box_graphics_obj *obj2 = (box_graphics_obj*)obj;
					//最近点比较：有矩阵，图形点对象
					if (gd::GraphicsObject3D::isPoint(obj2->vect_section))
					{
						data->arr->push_back((gis::c_geometry*)obj->geo);

						gd::Point3D pt1 = geo_api::transform_point3d(((gd::GraphicsObject3D::Point*)obj2->vect_section)->pt, *data->m);
						d->distances.push_back(geo_api::get_dis_2d(pt0, pt1));
					}
					//最近点比较：无矩阵，图形点对象
					if (gd::GraphicsObject3D::isCell(obj2->vect_section))
					{
						data->arr->push_back((gis::c_geometry*)obj->geo);

						gd::Point3D pt1 = geo_api::transform_point3d(((gd::GraphicsObject3D::Block*)obj2->vect_section)->pt, *data->m);
						d->distances.push_back(geo_api::get_dis_2d(pt0, pt1));
					}
					//最近点比较：有矩阵，图形点串对象
					else if (gd::GraphicsObject3D::isPointString(obj2->vect_section))
					{
						gd::GraphicsObject3D::PointString *section = (gd::GraphicsObject3D::PointString*)obj2->vect_section;
						const gd::Point3D *pts = section->pts.data();
						int npt = section->pts.size();

						bool bintersect = false;
						double dis, dis_min = -1;
						for (int j = 0; j < npt; j++)
						{
							gd::Point3D pt1 = geo_api::transform_point3d(pts[j], *data->m);
							if (data->rect.is_contain_pt(pt1))
							{
								if (!bintersect)
									data->arr->push_back((gis::c_geometry*)obj->geo);

								bintersect = true;

								dis = geo_api::get_dis_2d(pt1, pt0);
								if (dis_min<0 || dis_min>dis)
								{
									dis = dis_min;
								}
							}
						}
						if (bintersect)
						{
							d->distances.push_back(dis_min);
						}
					}
					//最近点比较：有矩阵，图形单元串对象
					else if (gd::GraphicsObject3D::isCellString(obj2->vect_section))
					{
						gd::GraphicsObject3D::BlockString *section = (gd::GraphicsObject3D::BlockString*)obj2->vect_section;
						const gd::Point3D *pts = section->pts.data();
						int npt = section->pts.size();

						gd::GraphicsObject3D::BlockString s2;
						s2 = *section;
						s2.pts.resize(1);
						gd::Rect3D rc0 = gd::GraphicsObject3D::getBound(&s2, 1.0);
						gd::Point3D pt0 = geo_api::transform_point3d(pts[0], *data->m);

						bool bintersect = false;
						double dis, dis_min = -1;

						for (int j = 0; j < npt; j++)
						{
							gd::Point3D pt1 = geo_api::transform_point3d(pts[j], *data->m);
							gd::Rect3D rc = rc0;
							rc.offset(pt1.x - pt0.x, pt1.y - pt0.y, 0);
							if (data->rect.is_intersect_2(rc))
							{
								if (!bintersect)
									data->arr->push_back((gis::c_geometry*)obj->geo);

								bintersect = true;

								dis = geo_api::get_dis_2d(pt1, pt0);
								if (dis_min<0 || dis_min>dis)
								{
									dis = dis_min;
								}
							}
						}
						if (bintersect)
						{
							d->distances.push_back(dis_min);
						}
					}
					//最近点比较：有矩阵，图形线对象
					else if (gd::GraphicsObject3D::isLineString(obj2->vect_section))
					{
						gd::GraphicsObject3D::LineString *section = (gd::GraphicsObject3D::LineString*)obj2->vect_section;
						const gd::Point3D *pts = section->pts.data();
						int npt = section->pts.size();

						bool bintersect = false;
						double dis, dis_min = -1;

						gd::Point3D pt1, pt2;
						pt1 = pt2 = geo_api::transform_point3d(pts[0], *data->m);
						for (int j = 1; j < npt; j++)
						{
							pt1 = pt2;
							pt2 = geo_api::transform_point3d(pts[j], *data->m);
							if (data->rect.is_intersect_pts(pt1, pt2))
							{
								if (!bintersect)
									data->arr->push_back((gis::c_geometry*)obj->geo);

								bintersect = true;
								dis = geo_api::get_dis_pt_to_lineseg(pt1, pt2, pt0);
								if (dis_min<0 || dis_min>dis)
								{
									dis = dis_min;
								}
							}
						}
						if (bintersect)
						{
							d->distances.push_back(dis_min);
						}
					}
					//最近点比较：有矩阵，图形面对象
					else if (gd::GraphicsObject3D::isPolygon(obj2->vect_section))
					{
						gd::GraphicsObject3D::c_polygon *section = (gd::GraphicsObject3D::c_polygon*)obj2->vect_section;
						const gd::Point3D *pts = section->pts.data();
						int npt = section->pts.size(), j;

						bool bintersect = false;
						double dis, dis_min = -1;

						gd::point3d_array pts2;
						gd::Point3D pt1, pt2;
						pt1 = pt2 = geo_api::transform_point3d(pts[0], *data->m);
						pts2.push_back(pt1);

						for (j = 1; j < npt; j++)
						{
							pt1 = pt2;
							pt2 = geo_api::transform_point3d(pts[j], *data->m);
							pts2.push_back(pt2);

							if (data->rect.is_intersect_pts(pt1, pt2))
							{
								if (!bintersect)
									data->arr->push_back((gis::c_geometry*)obj->geo);

								bintersect = true;
								dis = geo_api::get_dis_pt_to_lineseg(pt1, pt2, pt0);
								if (dis_min<0 || dis_min>dis)
								{
									dis = dis_min;
								}
							}
						}

						if (bintersect)
						{
							d->distances.push_back(dis_min);
						}
						else
						{
							if (data->rect.is_contain_pt(geo_api::transform_point3d(pts[0], *data->m)))
							{
								data->arr->push_back((gis::c_geometry*)obj->geo);
								d->distances.push_back(query_distance_null_value);
							}
							else if (gis::c_geo_multi_polygon::is_contain_pt(pts2, section->counts, gd::Point3D(data->rect.x0, data->rect.y0, 0)))
							{
								data->arr->push_back((gis::c_geometry*)obj->geo);
								d->distances.push_back(query_distance_null_value);
							}							
						}
					}
					//最近点比较：有矩阵，图形文字对象
					else if (gd::GraphicsObject3D::isText(obj2->vect_section))
					{
						data->arr->push_back((gis::c_geometry*)obj->geo);
						d->distances.push_back(query_distance_null_value*0.5);
					}
				}
			}
		}
	}

}