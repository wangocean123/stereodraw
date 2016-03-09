#pragma once
#include "SceneLayer.h"
#include "PaintTexture.h"

namespace gd
{
	class GOODDRAW_API GraphicsLayer : public SceneLayer
	{
	public:
		struct vect_obj
		{
			GraphicsObject2D *pobj;
			LONG_PTR id;
			LONG_PTR user_data;
			LONG_PTR user_param;
			bool visible;
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
		GraphicsLayer();
		virtual ~GraphicsLayer();

		virtual void set_texture_info(int nMB, int texture_type);
		virtual void paint();
		virtual void step_paint(const Rect2D& view_rect);
		virtual bool is_paint_ok(const Rect2D& view_rect);

		void set_bound(Rect3D rc);
		virtual Rect3D getBound();

		virtual void set_view_matrix(const matrix4d& m, bool bmodify_zoom_rate);
		virtual void release_textures();

		GraphicsObject2D *createObj(LONG_PTR id, LONG_PTR user_data);
		void deleteObj(LONG_PTR id);
		void set_obj_visible(LONG_PTR id, bool bvisible);

		void deleteAllObjs();

	protected:
		virtual void get_texture_range(Rect2I& range);
		virtual void texture_item_to_texture_coords(const texture_item& item, quad_points& coord);
		virtual void texture_coords_to_texture_item(const Point3D& pt0, texture_item& item);

		virtual void paint_texture_item(const texture_item& item, int items_index);
		virtual bool find_texture_item_to_paint(const Rect2D& view_rect, texture_item& item_to_fill, int& index_to_fill);

	protected:
		Rect3D get_bound_client();

		Rect3D m_bound;
		std::vector<PaintTexture*> m_ptextures;
		std::list<vect_obj> m_objs;
		std::vector<quad_points> m_cache_global_coords;

		int m_nframe_texture_index;

		bool m_use_cache;
	};
}

