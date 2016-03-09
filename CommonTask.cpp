#include "stdafx.h"
#include "CommonTask.h"
#include "c_editor.h"
#include "c_workspace.h"
#include "Curve.h"
#include "geo_algorithom.h"

namespace gis
{
	CommonTask::CommonTask(c_editor *p)
	{
		m_peditor = p;
		m_is_end = false;
	}


	CommonTask::~CommonTask()
	{

	}

	void CommonTask::start()
	{

	}

	void CommonTask::left_click(gd::Point3D pt)
	{

	}

	void CommonTask::right_click(gd::Point3D pt)
	{

	}

	void CommonTask::mouse_move(gd::Point3D pt)
	{

	}

	void CommonTask::key_down(int ch, int flag)
	{

	}

	void CommonTask::cancel()
	{
		m_is_end = true;
	}

	void CommonTask::end()
	{
		m_is_end = true;
	}

	bool CommonTask::is_end()
	{
		return m_is_end;
	}

	SelectTask::SelectTask(c_editor *p) :CommonTask(p)
	{
		m_bdraw_rect = false;
	}


	SelectTask::~SelectTask()
	{

	}

	void SelectTask::start()
	{

	}

	void SelectTask::left_click(gd::Point3D pt)
	{
		if (!m_bdraw_rect)
		{
			float select_wid = 10.0;
			gd::Rect3D rect = gd::rect_from_center_3<double>(pt, select_wid, select_wid, 1e+10);

			const_geometry_array arr;
			m_peditor->get_workspace()->query_nearest(pt, rect,&m_peditor->cmdView()->get_cur_matrix(),arr);
			m_peditor->selectSet(arr);

			if (arr.size() > 0)
			{
				m_geo_objs = arr;
				m_is_end = true;
			}
			else
			{
				m_bdraw_rect = true;
				m_pt1 = pt;
			}
		}
		else
		{
			m_peditor->cmdView()->set_dynamic_draw(NULL);
			gd::Rect3D rect = gd::rect_from_corner_3<double>(pt, m_pt1);

			const_geometry_array arr;
			m_peditor->get_workspace()->query_nearest(pt, rect, &m_peditor->cmdView()->get_cur_matrix(), arr);
			m_peditor->selectSet(arr);

			if (arr.size() > 0)
			{
				m_geo_objs = arr;
				m_is_end = true;
			}

			m_bdraw_rect = false;
		}
	}

	void SelectTask::right_click(gd::Point3D pt)
	{
		if (m_bdraw_rect)
		{
			m_bdraw_rect = false;
			m_peditor->cmdView()->set_dynamic_draw(NULL);
		}
	}

	void SelectTask::mouse_move(gd::Point3D pt)
	{
		if (m_bdraw_rect)
		{
			gd::Rect3D rc = gd::rect_from_corner_3<double>(m_pt1, pt);

			gd::Point3D pts[4];
			gd::get_rect_corners(rc, pts);

			pts[0].z = pts[1].z = m_pt1.z;
			pts[2].z = pts[3].z = pt.z;

			gd::GraphicsObject3D vect;
			vect.begin_line_string(0, 0, 0, false);
			for (int i = 0; i < 4; i++)
			{
				vect.line_string(pts[i]);
			}
			vect.line_string(pts[0]);
			vect.end();
			m_peditor->cmdView()->set_dynamic_draw(&vect);
		}
	}

	void SelectTask::key_down(int ch, int flag)
	{
	}

	void SelectTask::cancel()
	{
		m_bdraw_rect = false;
		end();
	}

	DrawLinestringTask::DrawLinestringTask(c_editor *p) :CommonTask(p)
	{
		m_bclose = false;
		m_brectify = false;
	}


	DrawLinestringTask::~DrawLinestringTask()
	{

	}

	void DrawLinestringTask::start()
	{

	}

	void DrawLinestringTask::left_click(gd::Point3D pt)
	{
		gd::GeoVertex vertex = pt;

		gd::Variant var;
		if (m_peditor->cmdParamsWnd()->get_value("LineType", var))
			vertex.pen_code = (long)var;
		else
			vertex.pen_code = gd::GeoVertex::code_line;
		m_vertexs.push_back(vertex);
	}

	void DrawLinestringTask::right_click(gd::Point3D pt)
	{
		if (m_vertexs.size() < 2)
			cancel();
		else
		{
			end();
		}
	}

	void DrawLinestringTask::mouse_move(gd::Point3D pt)
	{
		if (m_vertexs.size() >= 1)
		{
			gd::GraphicsObject3D vect;

			gd::vertex_array pts0 = m_vertexs;
			gd::vertex_array pts;

			gd::Variant var = (long)gd::GeoVertex::code_line;
			m_peditor->cmdParamsWnd()->get_value("LineType",var);
			pts0.push_back(gd::GeoVertex(pt, (long)var, 0));
			Curve::Curve(pts0.data(), pts0.size(), geo_api::get_toler_xy(), false, pts);

			vect.begin_line_string(0, 0, 0, false);
			for (int i = 0; i < pts.size(); i++)
			{
				vect.line_string(pts[i]);
			}
			vect.end();

			m_peditor->cmdView()->set_dynamic_draw(&vect);
		}
	}

	void DrawLinestringTask::key_down(int ch, int flag)
	{

	}

	void DrawLinestringTask::cancel()
	{
		m_vertexs.clear();
		end();
	}
	
	DrawLinesegTask::DrawLinesegTask(c_editor *p) :CommonTask(p)
	{
		m_nclick = 0;
	}


	DrawLinesegTask::~DrawLinesegTask()
	{

	}

	void DrawLinesegTask::start()
	{
		m_nclick = 0;
	}

	void DrawLinesegTask::left_click(gd::Point3D pt)
	{
		if (m_nclick == 0)
		{
			m_pt1 = pt;
		}
		else if (m_nclick == 1)
		{
			m_pt2 = pt;
			end();
		}
	}

	void DrawLinesegTask::right_click(gd::Point3D pt)
	{
		
	}

	void DrawLinesegTask::mouse_move(gd::Point3D pt)
	{
		if (m_nclick == 1)
		{
			gd::GraphicsObject3D vect;

			vect.begin_line_string(0, 0, 0, false);
			vect.line_string(m_pt1);
			vect.line_string(m_pt2);
			vect.end();

			m_peditor->cmdView()->set_dynamic_draw(&vect);
		}

	}

	void DrawLinesegTask::key_down(int ch, int flag)
	{

	}

	void DrawLinesegTask::cancel()
	{
		m_nclick = 0;
		end();
	}

}