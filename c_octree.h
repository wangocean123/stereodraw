#pragma once

#include "rect_def.h"
#include "point_def.h"
#include "c_geo_line.h"

namespace gis
{
	struct GOODDRAW_API box_obj
	{
		enum e_type
		{
			e_point = 0,
			e_lines = 1,
			e_polygon = 2,
			e_vector_obj = 3
		};
		int type;
		const gis::c_geometry *geo;

		box_obj();
		virtual ~box_obj();
		gd::Rect3D getBound();

	};
	struct GOODDRAW_API box_point : public box_obj
	{
		box_point();
		gd::Point3D pt;
	};
	struct GOODDRAW_API box_lines : public box_obj
	{
		box_lines();
		gis::c_geo_linear_obj::shape_rect rect;
	};
	struct GOODDRAW_API box_polygon : public box_obj
	{
		box_polygon();
		gis::c_geo_linear_obj::shape_rect rect;
	};
	struct GOODDRAW_API box_graphics_obj : public box_obj
	{
		gd::GraphicsObject3D *vect;
		gd::GraphicsObject3D::Item *vect_section;
	};

	typedef std::vector<box_obj*> box_obj_array;

	class GOODDRAW_API c_octree_node
	{
	public:
		gd::Rect3D m_node_bound;
		c_octree_node *m_nodes[8];
		std::vector<box_obj*> m_objs; //按geo的值由小到大的排序

		c_octree_node();
		~c_octree_node();
		void create_sub_nodes();
		void insert(box_obj *obj, gd::Rect3D *rc_obj);
		gd::Rect3D get_real_bound();
		void remove(const gis::c_geometry *obj);
		void remove_all();
	private:
		void split();
	};

	class GOODDRAW_API c_octree
	{
	public:
		c_octree();
		~c_octree();

		void insert(const gis::c_geometry* geo);
		void insert(const gis::c_geometry* geo, gd::GraphicsObject3D *vect);
		void remove(const gis::c_geometry* geo);
		void remove_all();

		void query_rect(gd::Rect3D rect, const gd::matrix4d* m, gis::const_geometry_array& arr);
		void query_nearest(gd::Point3D pt, double r, const gd::matrix4d* m, gis::const_geometry_array& arr, std::vector<double>* dis_arr = NULL);
		void query_nearest(gd::Point3D pt, gd::Rect3D rect, const gd::matrix4d* m, gis::const_geometry_array& arr, std::vector<double>* dis_arr = NULL);

	private:
		struct query_data
		{
			gd::Rect3D rect;
			const gd::matrix4d *m;
			gis::const_geometry_array *arr;
		};
		struct query_rect_data : public query_data
		{
		};
		struct query_nearest_data : public query_data
		{
			gd::Point3D pt;
			std::vector<double> distances;
		};
		typedef void (c_octree::*compare_func)(c_octree_node *node, query_data *data);
		void do_query_loop(c_octree_node *node, compare_func func, query_data *data);
		void compare_rect(c_octree_node *node, query_data *data);
		void compare_nearest(c_octree_node *node, query_data *data);

	private:
		c_octree_node m_root;
	};

}