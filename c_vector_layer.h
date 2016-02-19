#pragma once
#include "c_drawing_layer.h"
#include "c_texture_obj.h"

namespace StereoDraw
{
	class STEREODRAW_API c_vector_layer : public c_drawing_layer
	{
	public:
		struct vect_obj
		{
			c_vector_obj_2d *pobj;
			long id;
			bool bvisible;
			bool bmarked;

			vect_obj();
			bool operator < (const vect_obj& b);
		};
	public:
		struct zoomrate_index
		{
			int index;
			float zoom;
			float cur_zoom;
			int state;
		};
	public:
		c_vector_layer();
		virtual ~c_vector_layer();

		virtual void set_cache_size(int nMB, int cache_type);
		virtual void draw();

		void set_bound(rect_3d rc);
		virtual rect_3d get_bound();

		virtual void set_view_matrix(const matrix4d& m, bool bmodify_zoom_rate);
		virtual void release_cache();

		c_vector_obj_2d *open_obj(long id);
		void delete_obj(long id);
		void set_obj_visible(long id, bool bvisible);
		void set_obj_marked(long id, bool bmarked);

		void delete_obj_all();

	protected:
		virtual void get_cache_range(rect_2i& range);
		virtual void get_cache_item_cache_coords(const cache_item& item, quad_points& coord);
		virtual void cache_coord_to_cache_item(const point_3d& pt0, cache_item& item);

		virtual void fill_cache_item(const cache_item& item, int items_index);
		virtual bool find_cache_item_to_fill(const rect_2d& view_rect, cache_item& item_to_fill, int& index_to_fill);

	protected:
		rect_3d get_bound_client();

		rect_3d m_bound;
		std::vector<c_texture_obj*> m_ptextures;
		std::list<vect_obj> m_objs;
		std::vector<quad_points> m_cache_ground_coords;

		int m_nframe_texture_index;
	};
}

