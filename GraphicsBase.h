#pragma once

#include <vector>
#include <list>
#include <string>
#include "GoodDraw.h"
#include "rect_def.h"


namespace gd
{
	class GraphicsObject2D;
	class GOODDRAW_API GraphicsBase
	{
	public:
		struct cell
		{
			cell();
			~cell();
			void refresh_rect();
			std::string name;
			GraphicsObject2D *vect;
			Rect2D rect;
			double radius;
		};
		struct line_style
		{
			std::string name;
			std::vector<float> dashs;
		};
	public:
		GraphicsBase();
		virtual ~GraphicsBase();

		void add_cell(const char *name, GraphicsObject2D *vect);
		void del_cell(const char *name);
		void del_cell(int index);
		cell *get_cell(const char *name);
		cell *get_cell(int index);
		int find_cell(const char *name);

		void add_line_style(const char *name, std::vector<float>& dashs);
		void del_line_style(const char *name);
		void del_line_style(int index);
		line_style *get_line_style(const char *name);
		line_style *get_line_style(int index);
		int find_line_style(const char *name);

		void clear();
		bool load(const char *path);
		bool save(const char *path);

		static void init();
		static GraphicsBase *obj();
		static void cleanup();

		std::vector<cell*> m_cells;
		std::vector<line_style*> m_line_styles;
	};

}