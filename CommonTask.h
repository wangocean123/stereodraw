#pragma once
#include "c_geometry.h"

namespace gis
{
	class c_editor;
	class GOODDRAW_API CommonTask
	{
	public:
		CommonTask(c_editor *p);
		virtual ~CommonTask();
		virtual void start();
		virtual void left_click(gd::Point3D pt);
		virtual void right_click(gd::Point3D pt);
		virtual void mouse_move(gd::Point3D pt);
		virtual void key_down(int ch, int flag);
		virtual void cancel();
		bool is_end();
		void end();
	protected:
		bool m_is_end;
		c_editor *m_peditor;
	};

	class GOODDRAW_API SelectTask : public CommonTask
	{
	public:
		SelectTask(c_editor *p);
		virtual ~SelectTask();
		virtual void start();
		virtual void left_click(gd::Point3D pt);
		virtual void right_click(gd::Point3D pt);
		virtual void mouse_move(gd::Point3D pt);
		virtual void key_down(int ch, int flag);
		virtual void cancel();

		gd::Point3D m_pt1;
		bool m_bdraw_rect;

		const_geometry_array m_geo_objs;
		int m_select_mode;
		int m_keys;
	};

	class GOODDRAW_API DrawLinestringTask : public CommonTask
	{
	public:
		DrawLinestringTask(c_editor *p);
		virtual ~DrawLinestringTask();
		virtual void start();
		virtual void left_click(gd::Point3D pt);
		virtual void right_click(gd::Point3D pt);
		virtual void mouse_move(gd::Point3D pt);
		virtual void key_down(int ch, int flag);
		virtual void cancel();

		gd::vertex_array m_vertexs;
		bool m_bclose;
		bool m_brectify;
	};


	class GOODDRAW_API DrawLinesegTask : public CommonTask
	{
	public:
		DrawLinesegTask(c_editor *p);
		virtual ~DrawLinesegTask();
		virtual void start();
		virtual void left_click(gd::Point3D pt);
		virtual void right_click(gd::Point3D pt);
		virtual void mouse_move(gd::Point3D pt);
		virtual void key_down(int ch, int flag);
		virtual void cancel();

		int m_nclick;
		gd::Point3D m_pt1, m_pt2;
	};
}