#include "stdafx.h"
#include "GraphicsObject.h"
#include "geo_algorithom.h"
#include "FontHelper.h"

namespace gd
{

	GraphicsObject2D::GraphicsObject2D()
	{
		originX = 0;
		originY = 0;
	}


	GraphicsObject2D::~GraphicsObject2D()
	{
	}



	void GraphicsObject2D::move(double dx, double dy)
	{
		originX += dx;
		originY += dy;
	}

	void GraphicsObject2D::setOrigin(double ox, double oy)
	{
		originX = ox;
		originY = oy;
	}

	void GraphicsObject2D::zoom(double kx, double ky)
	{
		originX *= kx;
		originY *= ky;

		for (std::list<Item*>::iterator it = m_list.begin(); it != m_list.end(); it++)
		{
			Item *p = *it;
			if (p == NULL)continue;

			if (isPoint(p))
			{
				((Point*)p)->pt.x *= kx;
				((Point*)p)->pt.y *= ky;
			}
			else if (isCell(p))
			{
				((Block*)p)->pt.x *= kx;
				((Block*)p)->pt.y *= ky;
			}
			else if (isPointString(p))
			{
				PointString *p2 = (PointString*)p;
				for (std::vector<Point2F>::iterator it = p2->pts.begin(); it != p2->pts.end(); it++)
				{
					(*it).x *= kx;
					(*it).y *= ky;
				}
			}
			else if (isCellString(p))
			{
				BlockString *p2 = (BlockString*)p;
				for (std::vector<Point2F>::iterator it = p2->pts.begin(); it != p2->pts.end(); it++)
				{
					(*it).x *= kx;
					(*it).y *= ky;
				}
			}
			else if (isLineString(p))
			{
				LineString *p2 = (LineString*)p;
				for (std::vector<Point2F>::iterator it = p2->pts.begin(); it != p2->pts.end(); it++)
				{
					(*it).x *= kx;
					(*it).y *= ky;
				}
			}
			else if (isPolygon(p))
			{
				c_polygon *p2 = (c_polygon*)p;
				for (std::vector<Point2F>::iterator it = p2->pts.begin(); it != p2->pts.end(); it++)
				{
					(*it).x *= kx;
					(*it).y *= ky;
				}
			}
			else if (isText(p))
			{
				((Text*)p)->pt.x *= kx;
				((Text*)p)->pt.y *= ky;
			}
		}

	}


	void GraphicsObject2D::rotate(Point2D pt, double angle)
	{
		matrix4d m = matrix4d_rotate(&Point3D(pt.x,pt.y,0), NULL, angle);
		transform(m);
	}



	void GraphicsObject2D::transform(const matrix4d& m)
	{
		std::vector<Point2F> pts;
		getAllPts(pts);

		transform_pts_by_matrix_2d(pts.data(), pts.size(), m);

		setAllPts(pts);
	}


	void GraphicsObject2D::refreshBound()
	{
		for (std::list<Item*>::iterator it = m_list.begin(); it != m_list.end(); it++)
		{
			Item *p = *it;
			if (p == NULL)continue;

			if (isPointString(p))
			{
				PointString *p2 = (PointString*)p;
				Rect2D rc = create_rect2d_from_points(p2->pts.data(), p2->pts.size());
				rect2d_to_points(rc, p2->range.m_pt0, p2->range.m_pt1);
			}
			else if (isCellString(p))
			{
				BlockString *p2 = (BlockString*)p;
				Rect2D rc = create_rect2d_from_points(p2->pts.data(), p2->pts.size());
				rect2d_to_points(rc, p2->range.m_pt0, p2->range.m_pt1);
			}
			else if (isLineString(p))
			{
				LineString *p2 = (LineString*)p;
				Rect2D rc = create_rect2d_from_points(p2->pts.data(), p2->pts.size());
				rect2d_to_points(rc, p2->range.m_pt0, p2->range.m_pt1);
			}
			else if (isPolygon(p))
			{
				c_polygon *p2 = (c_polygon*)p;
				Rect2D rc = create_rect2d_from_points(p2->pts.data(), p2->pts.size());
				rect2d_to_points(rc, p2->range.m_pt0, p2->range.m_pt1);
			}
			else if (isText(p))
			{
				Text *p2 = (Text*)p;
				FontHelper helper;

				TextStyle style = p2->textStyle;
				style.m_height = 400;
				Rect2D rc = helper.getBound(p2->text.data(), style);
				rc *= (p2->textStyle.m_height / 400);
				rect2d_to_points(rc, p2->range.m_pt0, p2->range.m_pt1);
			}
		}
	}


	GraphicsObject3D::GraphicsObject3D()
	{
	}


	GraphicsObject3D::~GraphicsObject3D()
	{
	}



	void GraphicsObject3D::move(double dx, double dy, double dz)
	{
		for (std::list<Item*>::iterator it = m_list.begin(); it != m_list.end(); it++)
		{
			Item *p = *it;
			if (p == NULL)continue;

			if (isPoint(p))
			{
				((Point*)p)->pt.x += dx;
				((Point*)p)->pt.y += dy;
				((Point*)p)->pt.z += dz;
			}
			else if (isCell(p))
			{
				((Block*)p)->pt.x += dx;
				((Block*)p)->pt.y += dy;
				((Block*)p)->pt.z += dz;
			}
			else if (isPointString(p))
			{
				PointString *p2 = (PointString*)p;
				for (std::vector<Point3D>::iterator it = p2->pts.begin(); it != p2->pts.end(); it++)
				{
					(*it).x += dx;
					(*it).y += dy;
					(*it).z += dz;
				}
			}
			else if (isCellString(p))
			{
				BlockString *p2 = (BlockString*)p;
				for (std::vector<Point3D>::iterator it = p2->pts.begin(); it != p2->pts.end(); it++)
				{
					(*it).x += dx;
					(*it).y += dy;
					(*it).z += dz;
				}
			}
			else if (isLineString(p))
			{
				LineString *p2 = (LineString*)p;
				for (std::vector<Point3D>::iterator it = p2->pts.begin(); it != p2->pts.end(); it++)
				{
					(*it).x += dx;
					(*it).y += dy;
					(*it).z += dz;
				}
			}
			else if (isPolygon(p))
			{
				c_polygon *p2 = (c_polygon*)p;
				for (std::vector<Point3D>::iterator it = p2->pts.begin(); it != p2->pts.end(); it++)
				{
					(*it).x += dx;
					(*it).y += dy;
					(*it).z += dz;
				}
			}
			else if (isText(p))
			{
				((Text*)p)->pt.x += dx;
				((Text*)p)->pt.y += dy;
				((Text*)p)->pt.z += dz;
			}
		}
	}


	void GraphicsObject3D::zoom(double kx, double ky, double kz)
	{
		for (std::list<Item*>::iterator it = m_list.begin(); it != m_list.end(); it++)
		{
			Item *p = *it;
			if (p == NULL)continue;

			if (isPoint(p))
			{
				((Point*)p)->pt.x *= kx;
				((Point*)p)->pt.y *= ky;
				((Point*)p)->pt.z *= kz;
			}
			else if (isCell(p))
			{
				((Block*)p)->pt.x *= kx;
				((Block*)p)->pt.y *= ky;
				((Block*)p)->pt.z *= kz;
			}
			else if (isPointString(p))
			{
				PointString *p2 = (PointString*)p;
				for (std::vector<Point3D>::iterator it = p2->pts.begin(); it != p2->pts.end(); it++)
				{
					(*it).x *= kx;
					(*it).y *= ky;
					(*it).z *= kz;
				}
			}
			else if (isCellString(p))
			{
				BlockString *p2 = (BlockString*)p;
				for (std::vector<Point3D>::iterator it = p2->pts.begin(); it != p2->pts.end(); it++)
				{
					(*it).x *= kx;
					(*it).y *= ky;
					(*it).z *= kz;
				}
			}
			else if (isLineString(p))
			{
				LineString *p2 = (LineString*)p;
				for (std::vector<Point3D>::iterator it = p2->pts.begin(); it != p2->pts.end(); it++)
				{
					(*it).x *= kx;
					(*it).y *= ky;
					(*it).z *= kz;
				}
			}
			else if (isPolygon(p))
			{
				c_polygon *p2 = (c_polygon*)p;
				for (std::vector<Point3D>::iterator it = p2->pts.begin(); it != p2->pts.end(); it++)
				{
					(*it).x *= kx;
					(*it).y *= ky;
					(*it).z *= kz;
				}
			}
			else if (isText(p))
			{
				((Text*)p)->pt.x *= kx;
				((Text*)p)->pt.y *= ky;
				((Text*)p)->pt.z *= kz;
			}
		}
	}


	void GraphicsObject3D::rotate(Point3D pt, double angle, Point3D *dpt)
	{
		matrix4d m = matrix4d_rotate(&pt, dpt, angle);
		transform(m);
	}


	void GraphicsObject3D::transform(const matrix4d& m)
	{
		std::vector<Point3D> pts;
		getAllPts(pts);

		transform_pts_by_matrix_3d(pts.data(), pts.size(), m);

		setAllPts(pts);
	}

	void GraphicsObject3D::refreshBound()
	{
		for (std::list<Item*>::iterator it = m_list.begin(); it != m_list.end(); it++)
		{
			Item *p = *it;
			if (p == NULL)continue;

			if (isPointString(p))
			{
				PointString *p2 = (PointString*)p;
				Rect3D rc = create_rect3d_from_points(p2->pts.data(), p2->pts.size());
				rect3d_to_points(rc, p2->range.m_pt0, p2->range.m_pt1);
			}
			else if (isCellString(p))
			{
				BlockString *p2 = (BlockString*)p;
				Rect3D rc = create_rect3d_from_points(p2->pts.data(), p2->pts.size());
				rect3d_to_points(rc, p2->range.m_pt0, p2->range.m_pt1);
			}
			else if (isLineString(p))
			{
				LineString *p2 = (LineString*)p;
				Rect3D rc = create_rect3d_from_points(p2->pts.data(), p2->pts.size());
				rect3d_to_points(rc, p2->range.m_pt0, p2->range.m_pt1);
			}
			else if (isPolygon(p))
			{
				c_polygon *p2 = (c_polygon*)p;
				Rect3D rc = create_rect3d_from_points(p2->pts.data(), p2->pts.size());
				rect3d_to_points(rc, p2->range.m_pt0, p2->range.m_pt1);
			}
		}
	}
	

	void GraphicsObject2D::copyfrom(GraphicsObject3D* obj)
	{
		clear();
		for (std::list<GraphicsObject3D::Item*>::iterator it = obj->m_list.begin(); it != obj->m_list.end(); it++)
		{
			const GraphicsObject3D::Item *p = *it;
			if (p == NULL)continue;

			if (GraphicsObject3D::isPoint(p))
			{
				Point *p_new = new Point();
				GraphicsObject3D::Point* p1 = (GraphicsObject3D::Point*)p;

				p_new->pt.x = p1->pt.x;
				p_new->pt.y = p1->pt.y;
				p_new->color = p1->color;
				p_new->size = p1->size;
				p_new->globalSize = p1->globalSize;
				p_new->flat = p1->flat;

				m_list.push_back(p_new);
			}
			else if (GraphicsObject3D::isCell(p))
			{
				Block *p_new = new Block();
				GraphicsObject3D::Block* p1 = (GraphicsObject3D::Block*)p;
				p_new->pt.x = p1->pt.x;
				p_new->pt.y = p1->pt.y;
				p_new->color = p1->color;
				p_new->angle = p1->angle;
				p_new->globalScale = p1->globalScale;
				p_new->blockIndex = p1->blockIndex;
				p_new->scaleX = p1->scaleX;
				p_new->scaleY = p1->scaleY;

				m_list.push_back(p_new);
			}
			else if (GraphicsObject3D::isPointString(p))
			{
				PointString *p_new = new PointString();
				GraphicsObject3D::PointString* p1 = (GraphicsObject3D::PointString*)p;
				geo_api::copy_xy(p1->pts, p_new->pts);
				p_new->colors = p1->colors;
				p_new->size = p1->size;
				p_new->globalSize = p1->globalSize;
				p_new->flat = p1->flat;
				geo_api::copy_xy(p_new->range.m_pt0, p1->range.m_pt0);
				geo_api::copy_xy(p_new->range.m_pt1, p1->range.m_pt1);

				m_list.push_back(p_new);
			}
			else if (GraphicsObject3D::isCellString(p))
			{
				BlockString *p_new = new BlockString();
				GraphicsObject3D::BlockString* p1 = (GraphicsObject3D::BlockString*)p;
				geo_api::copy_xy(p1->pts, p_new->pts);
				p_new->colors = p1->colors;
				p_new->angles = p1->angles;
				p_new->blockIndex = p1->blockIndex;
				p_new->scaleX = p1->scaleX;
				p_new->scaleY = p1->scaleY;
				p_new->globalScale = p1->globalScale;
				geo_api::copy_xy(p_new->range.m_pt0, p1->range.m_pt0);
				geo_api::copy_xy(p_new->range.m_pt1, p1->range.m_pt1);

				m_list.push_back(p_new);
			}
			else if (GraphicsObject3D::isLineString(p))
			{
				LineString *p_new = new LineString();
				GraphicsObject3D::LineString* p1 = (GraphicsObject3D::LineString*)p;

				geo_api::copy_xy(p1->pts, p_new->pts);
				p_new->color = p1->color;
				p_new->lineStyle = p1->lineStyle;
				p_new->lineWidth = p1->lineWidth;
				p_new->globalWidth = p1->globalWidth;
				geo_api::copy_xy(p_new->range.m_pt0, p1->range.m_pt0);
				geo_api::copy_xy(p_new->range.m_pt1, p1->range.m_pt1);

				m_list.push_back(p_new);
			}
			else if (GraphicsObject3D::isPolygon(p))
			{
				c_polygon *p_new = new c_polygon();
				GraphicsObject3D::c_polygon* p1 = (GraphicsObject3D::c_polygon*)p;

				geo_api::copy_xy(p1->pts, p_new->pts);
				p_new->color = p1->color;
				p_new->counts = p1->counts;
				geo_api::copy_xy(p_new->range.m_pt0, p1->range.m_pt0);
				geo_api::copy_xy(p_new->range.m_pt1, p1->range.m_pt1);

				m_list.push_back(p_new);
			}
			else if (GraphicsObject3D::isText(p))
			{
				Text *p_new = new Text();
				GraphicsObject3D::Text* p1 = (GraphicsObject3D::Text*)p;

				p_new->pt.x = p1->pt.x;
				p_new->pt.y = p1->pt.y;
				p_new->color = p1->color;
				p_new->text = p1->text;
				p_new->textStyle = p1->textStyle;
				p_new->globalSize = p1->globalSize;
				p_new->upward = p1->upward;
				geo_api::copy_xy(p_new->range.m_pt0, p1->range.m_pt0);
				geo_api::copy_xy(p_new->range.m_pt1, p1->range.m_pt1);

				m_list.push_back(p_new);
			}
		}

		std::vector<Point3D> pts;
		std::vector<Point2F> pts2;
		obj->getAllPts(pts);
		if (pts.size() > 0)
		{
			originX = pts[0].x;
			originY = pts[0].y;

			pts2.resize(pts.size());

			int size = pts.size();
			for (int i = 0; i < size; i++)
			{
				pts2[i].x = pts[i].x - originX;
				pts2[i].y = pts[i].y - originY;
			}

			setAllPts(pts2);
		}
	}


	void GraphicsObject3D::copyfrom(GraphicsObject2D* obj)
	{
		clear();
		for (std::list<GraphicsObject2D::Item*>::iterator it = obj->m_list.begin(); it != obj->m_list.end(); it++)
		{
			const GraphicsObject2D::Item *p = *it;
			if (p == NULL)continue;

			if (GraphicsObject2D::isPoint(p))
			{
				Point *p_new = new Point();
				GraphicsObject2D::Point* p1 = (GraphicsObject2D::Point*)p;

				p_new->pt.x = p1->pt.x;
				p_new->pt.y = p1->pt.y;
				p_new->color = p1->color;
				p_new->size = p1->size;
				p_new->globalSize = p1->globalSize;
				p_new->flat = p1->flat;

				m_list.push_back(p_new);
			}
			else if (GraphicsObject2D::isCell(p))
			{
				Block *p_new = new Block();
				GraphicsObject2D::Block* p1 = (GraphicsObject2D::Block*)p;
				p_new->pt.x = p1->pt.x;
				p_new->pt.y = p1->pt.y;
				p_new->color = p1->color;
				p_new->angle = p1->angle;
				p_new->globalScale = p1->globalScale;
				p_new->blockIndex = p1->blockIndex;
				p_new->scaleX = p1->scaleX;
				p_new->scaleY = p1->scaleY;

				m_list.push_back(p_new);
			}
			else if (GraphicsObject2D::isPointString(p))
			{
				PointString *p_new = new PointString();
				GraphicsObject2D::PointString* p1 = (GraphicsObject2D::PointString*)p;
				geo_api::copy_xy(p1->pts, p_new->pts);
				p_new->colors = p1->colors;
				p_new->size = p1->size;
				p_new->globalSize = p1->globalSize;
				p_new->flat = p1->flat;
				geo_api::copy_xy(p_new->range.m_pt0, p1->range.m_pt0);
				geo_api::copy_xy(p_new->range.m_pt1, p1->range.m_pt1);

				m_list.push_back(p_new);
			}
			else if (GraphicsObject2D::isCellString(p))
			{
				BlockString *p_new = new BlockString();
				GraphicsObject2D::BlockString* p1 = (GraphicsObject2D::BlockString*)p;
				geo_api::copy_xy(p1->pts, p_new->pts);
				p_new->colors = p1->colors;
				p_new->angles = p1->angles;
				p_new->blockIndex = p1->blockIndex;
				p_new->scaleX = p1->scaleX;
				p_new->scaleY = p1->scaleY;
				p_new->globalScale = p1->globalScale;
				geo_api::copy_xy(p_new->range.m_pt0, p1->range.m_pt0);
				geo_api::copy_xy(p_new->range.m_pt1, p1->range.m_pt1);

				m_list.push_back(p_new);
			}
			else if (GraphicsObject2D::isLineString(p))
			{
				LineString *p_new = new LineString();
				GraphicsObject2D::LineString* p1 = (GraphicsObject2D::LineString*)p;

				geo_api::copy_xy(p1->pts, p_new->pts);
				p_new->color = p1->color;
				p_new->lineStyle = p1->lineStyle;
				p_new->lineWidth = p1->lineWidth;
				p_new->globalWidth = p1->globalWidth;
				geo_api::copy_xy(p_new->range.m_pt0, p1->range.m_pt0);
				geo_api::copy_xy(p_new->range.m_pt1, p1->range.m_pt1);

				m_list.push_back(p_new);
			}
			else if (GraphicsObject2D::isPolygon(p))
			{
				c_polygon *p_new = new c_polygon();
				GraphicsObject2D::c_polygon* p1 = (GraphicsObject2D::c_polygon*)p;

				geo_api::copy_xy(p1->pts, p_new->pts);
				p_new->color = p1->color;
				p_new->counts = p1->counts;
				geo_api::copy_xy(p_new->range.m_pt0, p1->range.m_pt0);
				geo_api::copy_xy(p_new->range.m_pt1, p1->range.m_pt1);

				m_list.push_back(p_new);
			}
			else if (GraphicsObject2D::isText(p))
			{
				Text *p_new = new Text();
				GraphicsObject2D::Text* p1 = (GraphicsObject2D::Text*)p;

				p_new->pt.x = p1->pt.x;
				p_new->pt.y = p1->pt.y;
				p_new->color = p1->color;
				p_new->text = p1->text;
				p_new->textStyle = p1->textStyle;
				p_new->globalSize = p1->globalSize;
				p_new->upward = p1->upward;
				geo_api::copy_xy(p_new->range.m_pt0, p1->range.m_pt0);
				geo_api::copy_xy(p_new->range.m_pt1, p1->range.m_pt1);

				m_list.push_back(p_new);
			}
		}

		move(obj->originX, obj->originY, 0);
	}

	Rect2D GraphicsObject2D::getBound(double view_scale)
	{
		Rect2D rc;
		for (std::list<GraphicsObject2D::Item*>::iterator it = m_list.begin(); it != m_list.end(); it++)
		{
			rc = (rc | getBound(*it, view_scale));
		}
		rc.offset(originX, originY);
		return rc;
	}

	Rect3D GraphicsObject3D::getBound(double view_scale)
	{
		Rect3D rc;
		for (std::list<GraphicsObject3D::Item*>::iterator it = m_list.begin(); it != m_list.end(); it++)
		{
			rc = (rc | getBound(*it, view_scale));
		}
		return rc;
	}
	
	Rect2D GraphicsObject2D::getBound(const GraphicsObject2D::Item* p, double view_scale)
	{
		if (isPoint(p))
		{
			Point *p1 = (Point*)p;
			double size = 0;
			if (p1->globalSize)
			{
				size = p1->size * view_scale*0.5;
			}
			else
			{
				size = p1->size * 0.5;
			}

			return Rect2D(p1->pt.x - size, p1->pt.y - size, p1->pt.x + size, p1->pt.y + size);
		}
		else if (isCell(p))
		{
			Block *p1 = (Block*)p;
			double size = 5e-5;

			GraphicsBase::cell *pcell = GraphicsBase::obj()->get_cell(p1->blockIndex);
			if (pcell == NULL)
			{
				return Rect2D(p1->pt.x - size, p1->pt.y - size, p1->pt.x + size, p1->pt.y + size);
			}

			double scale = 1.0;
			if (p1->globalScale)
				scale = view_scale;

			double scale_x = scale*p1->scaleX, scale_y = scale*p1->scaleY;

			Rect2D rc = pcell->rect;
			if (p1->angle == 0.0f)
			{
				rc.x0 *= scale_x;	rc.x1 *= scale_x;
				rc.y0 *= scale_y;	rc.y1 *= scale_y;
				rc.offset(p1->pt.x, p1->pt.y);
				return rc;
			}
			else
			{
				scale = scale_x < scale_y ? scale_x : scale_y;
				double r = pcell->radius*scale;
				rc = Rect2D(-r, -r, r, r);
				rc.offset(p1->pt.x, p1->pt.y);

				return rc;
			}
		}
		else if (isPointString(p))
		{
			PointString *p1 = (PointString*)p;
			Rect2D rc_pts = rect_from_corner_2<double>(p1->range.m_pt0, p1->range.m_pt1);
			double size = 0;
			if (p1->globalSize)
			{
				size = p1->size * view_scale*0.5;
			}
			else
			{
				size = p1->size * 0.5;
			}

			rc_pts.inflate(size, size);
			return rc_pts;
		}
		else if (isCellString(p))
		{
			Rect2D rc_cell;
			double size = 5e-5;

			BlockString *p1 = (BlockString*)p;
			GraphicsBase::cell *pcell = GraphicsBase::obj()->get_cell(p1->blockIndex);
			if (pcell == NULL)
			{
				rc_cell = Rect2D(-size, -size, size, size);
			}

			double scale = 1.0;
			if (p1->globalScale)
				scale = view_scale;
			
			double scale_x = scale*p1->scaleX, scale_y = scale*p1->scaleY;
			scale = scale_x < scale_y ? scale_x : scale_y;
			double r = pcell->radius*scale;
			Rect2D rc_pts = rect_from_corner_2<double>(p1->range.m_pt0, p1->range.m_pt1);
			rc_pts.x0 = rc_pts.x0 - r;
			rc_pts.x1 = rc_pts.x1 + r;
			rc_pts.y0 = rc_pts.y0 - r;
			rc_pts.y1 = rc_pts.y1 + r;
			return rc_pts;
		}
		else if (isLineString(p))
		{
			LineString *p1 = (LineString*)p;
			Rect2D rc_pts = rect_from_corner_2<double>(p1->range.m_pt0, p1->range.m_pt1);

			float line_width = p1->lineWidth;
			if (p1->globalWidth)
				line_width = view_scale * line_width;

			rc_pts.inflate(line_width, line_width);
			return rc_pts;
		}
		else if (isPolygon(p))
		{
			c_polygon *p1 = (c_polygon*)p;
			Rect2D rc_pts = rect_from_corner_2<double>(p1->range.m_pt0, p1->range.m_pt1);

			return rc_pts;
		}
		else if (isText(p))
		{
			Text *p1 = (Text*)p;
			Rect2D rc = rect_from_corner_2<double>(p1->range.m_pt0, p1->range.m_pt1);
			rc.offset(p1->pt.x, p1->pt.y);

			return rc;
		}

		return Rect2D();
	}

	Rect3D GraphicsObject3D::getBound(const GraphicsObject3D::Item* p, double view_scale)
	{
		if (isPoint(p))
		{
			Point *p1 = (Point*)p;
			double size = 0;
			if (p1->globalSize)
			{
				size = p1->size * view_scale*0.5;
			}
			else
			{
				size = p1->size * 0.5;
			}

			return Rect3D(p1->pt.x - size, p1->pt.y - size, p1->pt.z - size, p1->pt.x + size, p1->pt.y + size, p1->pt.z + size);
		}
		else if (isCell(p))
		{
			Block *p1 = (Block*)p;
			double size = 5e-5;

			GraphicsBase::cell *pcell = GraphicsBase::obj()->get_cell(p1->blockIndex);
			if (pcell == NULL)
			{
				return Rect3D(p1->pt.x - size, p1->pt.y - size, p1->pt.z - size, p1->pt.x + size, p1->pt.y + size, p1->pt.z + size);
			}

			double scale = 1.0;
			if (p1->globalScale)
				scale = view_scale;

			double scale_x = scale*p1->scaleX, scale_y = scale*p1->scaleY;

			Rect2D rc = pcell->rect;
			if (p1->angle == 0.0f)
			{
				rc.x0 *= scale_x;	rc.x1 *= scale_x;
				rc.y0 *= scale_y;	rc.y1 *= scale_y;
				rc.offset(p1->pt.x, p1->pt.y);
				return Rect3D(rc.x0, rc.y0, p1->pt.z - size, rc.x1, rc.y1, p1->pt.z + size);
			}
			else
			{
				scale = scale_x < scale_y ? scale_x : scale_y;
				double r = pcell->radius*scale;
				Rect3D rc = Rect3D(-r, -r, -r, r, r, r);
				rc.offset(p1->pt.x, p1->pt.y, p1->pt.z);

				return rc;
			}
		}
		else if (isPointString(p))
		{
			PointString *p1 = (PointString*)p;
			Rect3D rc_pts = rect_from_corner_3<double>(p1->range.m_pt0, p1->range.m_pt1);
			double size = 0;
			if (p1->globalSize)
			{
				size = p1->size * view_scale*0.5;
			}
			else
			{
				size = p1->size * 0.5;
			}

			rc_pts.inflate(size, size, size);
			return rc_pts;
		}
		else if (isCellString(p))
		{
			Rect3D rc_cell;
			double size = 5e-5;

			BlockString *p1 = (BlockString*)p;
			GraphicsBase::cell *pcell = GraphicsBase::obj()->get_cell(p1->blockIndex);
			if (pcell == NULL)
			{
				rc_cell = Rect3D(-size, -size, -size, size, size, size);
			}

			double scale = 1.0;
			if (p1->globalScale)
				scale = view_scale;

			double scale_x = scale*p1->scaleX, scale_y = scale*p1->scaleY;
			scale = scale_x < scale_y ? scale_x : scale_y;
			double r = pcell->radius*scale;
			Rect3D rc_pts = rect_from_corner_3<double>(p1->range.m_pt0, p1->range.m_pt1);
			rc_pts.x0 = rc_pts.x0 - r;
			rc_pts.x1 = rc_pts.x1 + r;
			rc_pts.y0 = rc_pts.y0 - r;
			rc_pts.y1 = rc_pts.y1 + r;
			rc_pts.z0 = rc_pts.z0 - size;
			rc_pts.z1 = rc_pts.z1 + size;
			return rc_pts;
		}
		else if (isLineString(p))
		{
			LineString *p1 = (LineString*)p;
			Rect3D rc_pts = create_rect3d_from_points(p1->pts.data(), p1->pts.size());

			float line_width = p1->lineWidth;
			if (p1->globalWidth)
				line_width = view_scale * line_width;

			rc_pts.inflate(line_width, line_width, line_width);
			return rc_pts;
		}
		else if (isPolygon(p))
		{
			c_polygon *p1 = (c_polygon*)p;
			Rect3D rc_pts = create_rect3d_from_points(p1->pts.data(), p1->pts.size());

			return rc_pts;
		}
		else if (isText(p))
		{
			Text *p1 = (Text*)p;
			Rect3D rc = rect_from_corner_3<double>(p1->range.m_pt0, p1->range.m_pt1);
			rc.offset(p1->pt.x, p1->pt.y,0);

			return rc;
		}

		return Rect3D();
	}

}
