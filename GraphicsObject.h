#pragma once

#include <vector>
#include <list>
#include <string>
#include "GoodDrawBaseType.h"
#include "TextStyle.h"
#include "point_def.h"
#include "rect_def.h"
#include "matrix.h"
#include "GraphicsBase.h"


namespace gd
{	

template<class tc_point>
class GraphicsObject
{
public:
	static const int pt_section_num = 256;
public:
	enum e_drawing_type
	{
		type_point = 1,
		type_cell,
		type_point_string,
		type_cell_string,
		type_line_string,
		type_polygon,
		type_text
	};
	struct Range
	{
		tc_point m_pt0, m_pt1;
	};
	struct Item
	{
		byte type;
	};

	typedef std::list<Item*> section_list;

	struct Point : public Item
	{
		Point()
		{
			type = type_point;
			color = 0xffffff;
			size = 1.0;
			globalSize = true;
			flat = true;
		}
		tc_point pt;
		ulong color;
		float size;
		bool globalSize;
		bool flat;
	};
	struct Block : public Item
	{
		Block()
		{
			type = type_cell;
			color = 0xffffff;
			blockIndex = 0;
			scaleX = 1.0;
			scaleY = 1.0;
			angle = 0.0;
			globalScale = true;
		}
		tc_point pt;
		ulong color;
		int blockIndex;
		float scaleX;
		float scaleY;
		float angle;
		bool globalScale;
	};
	struct PointString : public Item
	{
		PointString()
		{
			type = type_point_string;
			globalSize = true;
			flat = true;
			size = 0;
		}
		PointString *copy_head()
		{
			PointString *p = new PointString();
			p->globalSize = globalSize;
			p->flat = flat;
			p->size = size;
			return p;
		}
		PointString& operator = (const PointString& t)
		{
			pts = t.pts;
			colors = t.colors;
			size = t.size;
			globalSize = t.globalSize;
			flat = t.flat;
			range = t.range;
			size = t.size;
			return *this;
		}
		std::vector<tc_point> pts;
		std::vector<ulong> colors;

		bool globalSize;
		bool flat;
		float size;
		Range range;
	};
	struct BlockString : public Item
	{
		BlockString()
		{
			type = type_cell_string;
			blockIndex = 0;
			scaleX = 1.0;
			scaleY = 1.0;
			globalScale = true;
		}
		BlockString *copy_head()
		{
			BlockString *p = new BlockString();
			p->blockIndex = blockIndex;
			p->scaleX = scaleX;
			p->scaleY = scaleY;
			p->globalScale = globalScale;
			return p;
		}
		BlockString& operator = (const BlockString& t)
		{
			pts = t.pts;
			colors = t.colors;
			angles = t.angles;
			blockIndex = t.blockIndex;
			scaleX = t.scaleX;
			scaleY = t.scaleY;
			globalScale = t.globalScale;
			range = t.range;
			return *this;
		}
		std::vector<tc_point> pts;
		std::vector<ulong> colors;
		std::vector<float> angles;

		int blockIndex;
		float scaleX;
		float scaleY;
		bool globalScale;

		Range range;
	};
	struct LineString : public Item
	{
		LineString()
		{
			type = type_line_string;
			color = 0xffffff;
			lineStyle = 0;
			lineWidth = 1.0;
			globalWidth = true;
		}
		LineString *copy_head()
		{
			LineString *p = new LineString();
			p->color = color;
			p->lineStyle = lineStyle;
			p->lineWidth = lineWidth;
			p->globalWidth = globalWidth;
			return p;
		}
		LineString& operator = (const LineString& t)
		{
			pts = t.pts;
			color = t.color;
			lineStyle = t.lineStyle;
			lineWidth = t.lineWidth;
			globalWidth = t.globalWidth;
			range = t.range;
			return *this;
		}
		std::vector<tc_point> pts;
		ulong color;
		int lineStyle;
		float lineWidth;
		bool globalWidth;

		Range range;
	};
	struct c_polygon : public Item
	{
		c_polygon()
		{
			type = type_polygon;
			color = 0xffffff;
		}
		c_polygon *copy_head()
		{
			c_polygon *p = new c_polygon();
			p->color = color;
			return p;
		}
		c_polygon& operator = (const c_polygon& t)
		{
			pts = t.pts;
			counts = t.counts;
			color = t.color;
			range = t.range;
			return *this;
		}
		std::vector<tc_point> pts;
		std::vector<int> counts;
		ulong color;

		Range range;
	};
	struct Text : public Item
	{
		Text()
		{
			type = type_text;
			globalSize = true;
			upward = false;
		}
		Text& operator = (const Text& t)
		{
			pt = t.pt;
			color = t.color;
			text = t.text;
			textStyle = t.textStyle;
			globalSize = t.globalSize;
			upward = t.upward;
			range = t.range;
			return *this;
		}
		tc_point pt;
		ulong color;
		std::string text;
		TextStyle textStyle;
		bool globalSize;
		bool upward;

		Range range;
	};

	static bool isPoint(const Item *p){
		return (p != NULL && p->type == type_point);
	}

	static bool isCell(const Item *p){
		return (p != NULL && p->type == type_cell);
	}

	static bool isPointString(const Item *p){
		return (p != NULL && p->type == type_point_string);
	}

	static bool isCellString(const Item *p){
		return (p != NULL && p->type == type_cell_string);
	}

	static bool isLineString(const Item *p){
		return (p != NULL && p->type == type_line_string);
	}

	static bool isPolygon(const Item *p){
		return (p != NULL && p->type == type_polygon);
	}

	static bool isText(const Item *p){
		return (p != NULL && p->type == type_text);
	}

public:
	GraphicsObject()
	{
		beginString = false;
	}

	~GraphicsObject()
	{
		clear();
	}

	void point(tc_point pt, ulong color, float size, bool bglobal_size, bool bflat)
	{
		Point * p = new Point();
		p->pt = pt;
		p->color = color;
		p->size = size;
		p->globalSize = bglobal_size;
		p->flat = bflat;

		m_list.push_back(p);
	}

	void cell(tc_point pt, ulong color, int cell_index, float scale_x, float scale_y, bool bglobal_scale, float angle)
	{
		Block * p = new Block();
		p->pt = pt;
		p->color = color;
		p->blockIndex = cell_index;
		p->scaleX = scale_x;
		p->scaleY = scale_y;
		p->globalScale = bglobal_scale;
		p->angle = angle;

		m_list.push_back(p);
	}

	void begin_point_string(float size, bool bglobal_size, bool bflat)
	{
		PointString * p = new PointString();
		p->globalSize = bglobal_size;
		p->flat = bflat;
		p->size = size;

		m_list.push_back(p);
		beginString = true;
	}

	void point_string(tc_point pt, ulong color)
	{
		Item *p_cur = getStringTail();
		if (p_cur == NULL)
			return;
		if (p_cur->type != type_point_string)
			return;

		PointString *p = (PointString*)p_cur;

		if (p->pts.size() >= pt_section_num)
		{
			PointString *p_new = p->copy_head();
			p_new->pts.push_back(p->pts[p->pts.size() - 1]);
			m_list.push_back(p_new);
			p = p_new;
		}		
		p->pts.push_back(pt);
		p->colors.push_back(color);
	}

	void begin_cell_string(int cell_index, float scale_x, float scale_y, bool bglobal_scale)
	{
		BlockString * p = new BlockString();
		p->blockIndex = cell_index;
		p->scaleX = scale_x;
		p->scaleY = scale_y;
		p->globalScale = bglobal_scale;

		m_list.push_back(p);
		beginString = true;
	}

	void cell_string(tc_point pt, ulong color, float angle)
	{
		Item *p_cur = getStringTail();
		if (p_cur == NULL)
			return;
		if (p_cur->type != type_cell_string)
			return;

		BlockString *p = (BlockString*)p_cur;
		if (p->pts.size() >= pt_section_num)
		{
			BlockString *p_new = p->copy_head();
			p_new->pts.push_back(p->pts[p->pts.size() - 1]);
			m_list.push_back(p_new);
			p = p_new;
		}

		p->pts.push_back(pt);
		p->colors.push_back(color);
		p->angles.push_back(angle);
	}

	void begin_line_string(ulong color, int line_style, float line_width, bool bglobal_width)
	{
		LineString * p = new LineString();
		p->color = color;
		p->lineStyle = line_style;
		p->lineWidth = line_width;
		p->globalWidth = bglobal_width;

		m_list.push_back(p);
		beginString = true;
	}

	void line_string(tc_point pt)
	{
		Item *p_cur = getStringTail();
		if (p_cur == NULL)
			return;
		if (p_cur->type != type_line_string)
			return;

		LineString *p = (LineString*)p_cur;
		if (p->pts.size() >= pt_section_num)
		{
			LineString *p_new = p->copy_head();
			p_new->pts.push_back(p->pts[p->pts.size() - 1]);
			m_list.push_back(p_new);
			p = p_new;
		}

		p->pts.push_back(pt);
	}

	void begin_polygon(ulong color)
	{
		c_polygon * p = new c_polygon();
		p->color = color;

		m_list.push_back(p);
		beginString = true;
	}

	void polygon(tc_point pt)
	{
		Item *p_cur = getStringTail();
		if (p_cur == NULL)
			return;
		if (p_cur->type != type_polygon)
			return;

		c_polygon *p = (c_polygon*)p_cur;
		p->pts.push_back(pt);
	}

	void polygon(std::vector<tc_point>& pts)
	{
		Item *p_cur = getStringTail();
		if (p_cur == NULL)
			return;
		if (p_cur->type != type_polygon)
			return;

		c_polygon *p = (c_polygon*)p_cur;
		p->pts.insert(p->pts.end(), pts.begin(), pts.end());
		p->counts.push_back(pts.size());
	}

	void end()
	{
		if (!beginString)
			return;

		Item *p_cur = getStringTail();
		if (p_cur == NULL)
			return;
		if (p_cur->type == type_polygon)
		{
			c_polygon *p = (c_polygon*)p_cur;
			if (p->counts.size() == 0 && p->pts.size() > 2)
			{
				p->counts.push_back(p->pts.size());
			}
			else
			{
				delete p;
				m_list.remove(m_list.back());
			}
		}
		else if (p_cur->type == type_line_string)
		{
			LineString *p = (LineString*)p_cur;
			if (p->pts.size() < 2)
			{
				delete p;
				m_list.remove(m_list.back());
			}
		}
		else if (p_cur->type == type_point_string)
		{
			PointString *p = (PointString*)p_cur;
			if (p->pts.size() < 2)
			{
				delete p;
				m_list.remove(m_list.back());
			}
		}
		else if (p_cur->type == type_cell_string)
		{
			BlockString *p = (BlockString*)p_cur;
			if (p->pts.size() < 2)
			{
				delete p;
				m_list.remove(m_list.back());
			}
		}

		beginString = false;
	}

	void text(tc_point pt, ulong color, const char* words, const TextStyle& style, bool bglobal_size, bool bupward)
	{
		Text * p = new Text();

		p->pt = pt;
		p->color = color;
		p->text = words;
		p->textStyle = style;
		p->globalSize = bglobal_size;
		p->upward = bupward;

		m_list.push_back(p);
	}

	void clear()
	{
		std::list<Item*>::iterator& it = m_list.begin();
		while (it != m_list.end())
		{
			Item *p = *it;
			if (isPoint(p))
				delete ((Block*)p);
			else if (isCell(p))
				delete ((Block*)p);
			else if (isPointString(p))
				delete ((PointString*)p);
			else if (isCellString(p))
				delete ((BlockString*)p);
			else if (isLineString(p))
				delete ((LineString*)p);
			else if (isPolygon(p))
				delete ((c_polygon*)p);
			else if (isText(p))
				delete ((Text*)p);

			it++;
		}
		m_list.clear();
	}
	void removeSection(Item* p)
	{
		m_list.remove(p);
	}

	void getAllPts(std::vector<tc_point>& pts)
	{
		for (std::list<Item*>::iterator& it = m_list.begin(); it != m_list.end(); it++)
		{
			const Item *p = *it;
			if (p == NULL)continue;

			if (isPoint(p))
			{
				pts.push_back(((const Point*)p)->pt);
			}
			else if (isCell(p))
			{
				pts.push_back(((const Block*)p)->pt);
			}
			else if (isPointString(p))
			{
				pts.insert(pts.end(), ((const PointString*)p)->pts.begin(), ((const PointString*)p)->pts.end());
			}
			else if (isCellString(p))
			{
				pts.insert(pts.end(), ((const BlockString*)p)->pts.begin(), ((const BlockString*)p)->pts.end());
			}
			else if (isLineString(p))
			{
				pts.insert(pts.end(), ((const LineString*)p)->pts.begin(), ((const LineString*)p)->pts.end());
			}
			else if (isPolygon(p))
			{
				pts.insert(pts.end(), ((const c_polygon*)p)->pts.begin(), ((const c_polygon*)p)->pts.end());
			}
			else if (isText(p))
			{
				pts.push_back(((const Text*)p)->pt);
			}
		}
	}
	void setAllPts(std::vector<tc_point>& pts)
	{
		std::vector<tc_point>::iterator it0 = pts.begin();

		for (std::list<Item*>::iterator it = m_list.begin(); it != m_list.end(); it++)
		{
			Item *p = *it;
			if (p == NULL)continue;

			if (isPoint(p))
			{
				((Point*)p)->pt = *it0;
				it0++;
			}
			else if (isCell(p))
			{
				((Block*)p)->pt = *it0;
				it0++;
			}
			else if (isPointString(p))
			{
				((PointString*)p)->pts.assign(it0, it0 + ((PointString*)p)->pts.size());
				it0 += ((PointString*)p)->pts.size();
			}
			else if (isCellString(p))
			{
				((BlockString*)p)->pts.assign(it0, it0 + ((BlockString*)p)->pts.size());
				it0 += ((BlockString*)p)->pts.size();
			}
			else if (isLineString(p))
			{
				((LineString*)p)->pts.assign(it0, it0 + ((LineString*)p)->pts.size());
				it0 += ((LineString*)p)->pts.size();
			}
			else if (isPolygon(p))
			{
				((c_polygon*)p)->pts.assign(it0, it0 + ((c_polygon*)p)->pts.size());
				it0 += ((c_polygon*)p)->pts.size();
			}
			else if (isText(p))
			{
				((Text*)p)->pt = *it0;
				it0++;
			}
		}
	}

	void setAllColor(ulong color)
	{
		for (std::list<Item*>::iterator it = m_list.begin(); it != m_list.end(); it++)
		{
			Item *p = *it;
			if (p == NULL)continue;

			if (isPoint(p))
			{
				((Point*)p)->color = color;
			}
			else if (isCell(p))
			{
				((Block*)p)->color = color;
			}
			else if (isPointString(p))
			{
				PointString *p2 = ((PointString*)p);
				for (std::vector<ulong>::iterator it2 = p2->colors.begin(); it2 != p2->colors.end(); it2++)
				{
					*it2 = color;
				}
			}
			else if (isCellString(p))
			{
				BlockString *p2 = ((BlockString*)p);
				for (std::vector<ulong>::iterator it2 = p2->colors.begin(); it2 != p2->colors.end(); it2++)
				{
					*it2 = color;
				}
			}
			else if (isLineString(p))
			{
				((LineString*)p)->color = color;
			}
			else if (isPolygon(p))
			{
				((c_polygon*)p)->color = color;
			}
			else if (isText(p))
			{
				((Text*)p)->color = color;
			}
		}
	}

	void setAllLineWidth(float line_width, bool bglobal_width)
	{
		for (std::list<Item*>::iterator it = m_list.begin(); it != m_list.end(); it++)
		{
			Item *p = *it;
			if (p == NULL)continue;

			if (isLineString(p))
			{
				((LineString*)p)->lineWidth = line_width;
				((LineString*)p)->globalWidth = bglobal_width;
			}
		}
	}

	void setAllPointSize(float point_size)
	{
		for (std::list<Item*>::iterator it = m_list.begin(); it != m_list.end(); it++)
		{
			Item *p = *it;
			if (p == NULL)continue;

			if (isPoint(p))
			{
				((Point*)p)->size = point_size;
			}
			else if (isPointString(p))
			{
				((PointString*)p)->size = point_size;
			}
		}
	}

	void setAllBlockIndex(int cell_index)
	{
		for (std::list<Item*>::iterator it = m_list.begin(); it != m_list.end(); it++)
		{
			Item *p = *it;
			if (p == NULL)continue;

			if (isCell(p))
			{
				((Block*)p)->blockIndex = cell_index;
			}
			else if (isCellString(p))
			{
				BlockString *p2 = ((BlockString*)p);
				p2->blockIndex = cell_index;
			}
		}
	}

	int getSectionCount(){
		return m_list.size();
	}

	void getSections(std::list<Item*>& list){
		list = m_list;
	}

	void copyFrom0(GraphicsObject<tc_point>* obj)
	{
		clear();
		for (std::list<Item*>::iterator it = obj->m_list.begin(); it != obj->m_list.end(); it++)
		{
			const Item *p = *it;
			if (p == NULL)continue;
			
			if (isPoint(p))
			{
				Point *p_new = new Point();
				*p_new = *(Point*)p;
				m_list.push_back(p_new);
			}
			else if (isCell(p))
			{
				Block *p_new = new Block();
				*p_new = *(Block*)p;
				m_list.push_back(p_new);
			}
			else if (isPointString(p))
			{
				PointString *p_new = new PointString();
				*p_new = *(PointString*)p;
				m_list.push_back(p_new);
			}
			else if (isCellString(p))
			{
				BlockString *p_new = new BlockString();
				*p_new = *(BlockString*)p;
				m_list.push_back(p_new);
			}
			else if (isLineString(p))
			{
				LineString *p_new = new LineString();
				*p_new = *(LineString*)p;
				m_list.push_back(p_new);
			}
			else if (isPolygon(p))
			{
				c_polygon *p_new = new c_polygon();
				*p_new = *(c_polygon*)p;
				m_list.push_back(p_new);
			}
			else if (isText(p))
			{
				Text *p_new = new Text();
				*p_new = *(Text*)p;
				m_list.push_back(p_new);
			}
		}
	}

protected:
	Item *getStringTail()
	{
		if (!beginString)
			return NULL;

		if (m_list.empty())
			return NULL;

		std::list<Item*>::iterator it = m_list.end();

		it--;

		return *it;
	}

	bool beginString;
	std::list<Item*> m_list;
};


template<class point_type>
void rect2d_to_points(const Rect2D& rc, point_type& pt1, point_type& pt2)
{
	pt1.x = rc.x0;
	pt1.y = rc.y0;
	pt2.x = rc.x1;
	pt2.y = rc.y1;
}


template<class point_type>
Rect2D points_to_rect2d(const point_type& pt1, const point_type& pt2)
{
	Rect2D rc;
	rc.m_x0 = pt1.x;
	rc.m_y0 = pt1.y;
	rc.m_x1 = pt2.x;
	rc.m_y1 = pt2.y;
	return rc;
}

template<class point_type>
Rect3D points_to_rect3d(point_type& pt1, point_type& pt2)
{
	Rect3D rc;
	rc.x0 = pt1.x;
	rc.y0 = pt1.y;
	rc.z0 = pt1.z;
	rc.x1 = pt2.x;
	rc.y1 = pt2.y;
	rc.z1 = pt2.z;
	return rc;
}

template<class point_type>
void rect3d_to_points(const Rect3D& rc, point_type& pt1, point_type& pt2)
{
	pt1.x = rc.x0;
	pt1.y = rc.y0;
	pt1.z = rc.z0;
	pt2.x = rc.x1;
	pt2.y = rc.y1;
	pt2.z = rc.z1;
}


class GraphicsObject3D;

class GOODDRAW_API GraphicsObject2D : public GraphicsObject<Point2F>
{
	friend class GraphicsObject3D;
public:
	GraphicsObject2D();
	~GraphicsObject2D();

	void move(double dx, double dy);
	void zoom(double kx, double ky);
	void rotate(Point2D pt, double angle);

	void refreshBound();
	Rect2D getBound(double view_scale);
	static Rect2D getBound(const GraphicsObject2D::Item* p, double view_scale);
	void transform(const matrix4d& m);
	void copyfrom(GraphicsObject3D* obj);
	void setOrigin(double ox, double oy);
protected:
public:
	double originX, originY;
};



class GOODDRAW_API GraphicsObject3D : public GraphicsObject<Point3D>
{
	friend class GraphicsObject2D;
public:
	GraphicsObject3D();
	~GraphicsObject3D();

	void move(double dx, double dy, double dz);
	void zoom(double kx, double ky, double dz);
	void rotate(Point3D pt, double angle, Point3D *dpt = NULL);
	void transform(const matrix4d& m);
	void copyfrom(GraphicsObject2D* obj);

	void refreshBound();
	Rect3D getBound(double view_scale);
	static Rect3D getBound(const GraphicsObject3D::Item* p, double view_scale);
};


typedef std::list<GraphicsObject2D*> vector_obj_2d_list;
typedef std::list<GraphicsObject3D*> vector_obj_3d_list;

}
