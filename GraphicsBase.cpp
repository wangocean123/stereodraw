#include "StdAfx.h"
#include "GraphicsBase.h"
#include "GraphicsObject.h"

namespace gd
{

	GraphicsBase::cell::cell()
	{
		vect = new GraphicsObject2D();
		radius = 0;
	}
	GraphicsBase::cell::~cell()
	{
		if (vect)
			delete vect;
		vect = NULL;
	}
	void GraphicsBase::cell::refresh_rect()
	{
		rect = vect->getBound(1.0);

		Point2D pts[4];
		double dis[4], dis_max;
		pts[0].x = rect.x0; pts[0].y = rect.y0;
		pts[1].x = rect.x1; pts[1].y = rect.y0;
		pts[2].x = rect.x1; pts[2].y = rect.y1;
		pts[3].x = rect.x0; pts[3].y = rect.y1;
		for (int i = 0; i < 4; i++)
		{
			dis[i] = sqrt(pts[i].x*pts[i].x + pts[i].y*pts[i].y);
			if (i == 0)
				dis_max = dis[i];
			else if (dis_max < dis[i])
				dis_max = dis[i];
		}
		radius = dis_max;
	}
	GraphicsBase::GraphicsBase()
	{
	}


	GraphicsBase::~GraphicsBase()
	{
	}

	void GraphicsBase::add_cell(const char *name, GraphicsObject2D *vect)
	{
		int index = find_cell(name);
		if (index<0)
		{
			cell *pnew = new cell();
			pnew->name = name;
			pnew->vect->copyFrom0((GraphicsObject<Point2F>*)vect);

			m_cells.push_back(pnew);
		}
		else
		{
			m_cells[index]->vect->copyFrom0((GraphicsObject<Point2F>*)vect);
		}
	}

	void GraphicsBase::del_cell(const char *name)
	{
		int index = find_cell(name);
		if (index >= 0)
		{
			m_cells.erase(m_cells.begin() + index);
		}
	}

	void GraphicsBase::del_cell(int index)
	{
		if (index >= 0)
		{
			m_cells.erase(m_cells.begin() + index);
		}
	}

	GraphicsBase::cell *GraphicsBase::get_cell(const char *name)
	{
		int index = find_cell(name);
		if (index >= 0)
		{
			return m_cells[index];
		}
		return NULL;
	}

	GraphicsBase::cell *GraphicsBase::get_cell(int index)
	{
		if (index >= 0 && index<m_cells.size())
		{
			return m_cells[index];
		}
		return NULL;
	}

	int GraphicsBase::find_cell(const char *name)
	{
		for (int i = 0; i < m_cells.size(); i++)
		{
			if (_stricmp(m_cells[i]->name.data(), name) == 0)
			{
				return i;
			}
		}
		return -1;
	}


	void GraphicsBase::add_line_style(const char *name, std::vector<float>& dashs)
	{
		int index = find_line_style(name);
		if (index<0)
		{
			line_style *pnew = new line_style();
			pnew->name = name;
			pnew->dashs = dashs;

			m_line_styles.push_back(pnew);
		}
		else
		{
			m_line_styles[index]->dashs = dashs;
		}
	}

	void GraphicsBase::del_line_style(const char *name)
	{
		int index = find_line_style(name);
		if (index >= 0)
		{
			m_line_styles.erase(m_line_styles.begin() + index);
		}
	}

	void GraphicsBase::del_line_style(int index)
	{
		if (index >= 0 && index<m_line_styles.size())
		{
			m_line_styles.erase(m_line_styles.begin() + index);
		}
	}

	GraphicsBase::line_style *GraphicsBase::get_line_style(const char *name)
	{
		int index = find_line_style(name);
		if (index >= 0)
		{
			return m_line_styles[index];
		}
		return NULL;
	}

	GraphicsBase::line_style *GraphicsBase::get_line_style(int index)
	{
		if (index >= 0 && index<m_line_styles.size())
		{
			return m_line_styles[index];
		}
		return NULL;
	}

	int GraphicsBase::find_line_style(const char *name)
	{
		for (int i = 0; i < m_line_styles.size(); i++)
		{
			if (_stricmp(m_line_styles[i]->name.data(), name) == 0)
			{
				return i;
			}
		}
		return -1;
	}


	void GraphicsBase::clear()
	{
		for (int i = 0; i < m_cells.size(); i++)
		{
			delete m_cells[i];
		}
		m_cells.clear();

		for (int i = 0; i < m_line_styles.size(); i++)
		{
			delete m_line_styles[i];
		}
		m_line_styles.clear();
	}

	bool GraphicsBase::load(const char *path)
	{
		return false;
	}

	bool GraphicsBase::save(const char *path)
	{
		return false;
	}

	GraphicsBase *gp_drawing_base = NULL;

	void GraphicsBase::init()
	{
		if (gp_drawing_base == NULL)
		{
			gp_drawing_base = new GraphicsBase();
		}
	}

	GraphicsBase *GraphicsBase::obj()
	{
		return gp_drawing_base;
	}

	void GraphicsBase::cleanup()
	{
		if (gp_drawing_base)
			delete gp_drawing_base;
		return;
	}

}
