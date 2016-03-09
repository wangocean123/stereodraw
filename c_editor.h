#pragma once
#include "c_geometry.h"
#include "c_geo_map.h"
#include "Variant.h"
#include "GraphicsObject.h"
#include "CommonTask.h"

namespace gis
{
	class c_command;
	class c_workspace;
	class c_selection;

	class GOODDRAW_API c_command_params_file
	{
	public:
		c_command_params_file();
		virtual ~c_command_params_file();

		virtual bool open_file(const char* path);

		virtual void begin_command_params(const char* cmd_name);
		virtual void read_param(const char *param_name, bool& v);
		virtual void read_param(const char *param_name, int& v);
		virtual void read_param(const char *param_name, long& v);
		virtual void read_param(const char *param_name, float& v);
		virtual void read_param(const char *param_name, double& v);
		virtual void read_param(const char *param_name, std::string& v);

		virtual void write_param(const char *param_name, bool& v);
		virtual void write_param(const char *param_name, int& v);
		virtual void write_param(const char *param_name, long& v);
		virtual void write_param(const char *param_name, float& v);
		virtual void write_param(const char *param_name, double& v);
		virtual void write_param(const char *param_name, std::string& v);
		virtual void end_command_params();
	};

	class GOODDRAW_API c_command_params_wnd
	{
	public:
		typedef void (c_command::*msg_callback)(const char *id, gd::Variant& new_value);
		c_command_params_wnd();
		virtual ~c_command_params_wnd();

		virtual void set_title(const char *title);
		virtual void set_msg_callback(c_command *pcmd, msg_callback func);
		virtual void add_button(const char *id, const char *name);
		virtual void add_edit(const char *id, const char *name, const char *initText);
		virtual void add_file_button(const char *id, const char *name, const char *initPath, const char *file_filter);
		virtual void add_files_button(const char *id, const char *name, const char *initPath, const char *file_filter);
		virtual void add_folder_button(const char *id, const char *name, const char *initPath);
		virtual void add_color_button(const char *id, const char *name, ulong color);
		virtual void add_list(const char *id, const char *name, std::vector<std::string>& items, bool is_single_selection);
		virtual void add_layer_button(const char *id, const char *name, const char *initText, bool is_single_selection);
		virtual void add_linestring_type(const char *id, const char *name, int line_type);
		virtual bool get_value(const char *id, gd::Variant& value);

		virtual void clear();
	protected:
		c_command *m_pcmd;
		msg_callback m_msg_callback;

	};

	class GOODDRAW_API c_command_view
	{
	public:
		c_command_view();
		virtual ~c_command_view();

		virtual void set_static_draw(gd::GraphicsObject3D *pobj);
		virtual void set_dynamic_draw(gd::GraphicsObject3D *pobj);
		virtual gd::Point3D get_cur_point();
		virtual gd::matrix4d get_cur_matrix();
	};

	class c_editor;

	class GOODDRAW_API c_edit_operation
	{
		friend class c_editor;
	public:
		class GOODDRAW_API operation_item
		{
		public:
			enum e_type
			{
				type_geometry_objs = 1,
				type_geometry_shape = 2,
				type_geometry_vertex = 3,
				type_geometry_properties = 4,
				type_layer_objs = 5,
				type_layer_properties = 6
			};
			operation_item();
			virtual ~operation_item();

			virtual int get_type()const;

			virtual void undo(c_editor *peditor)=0;
			virtual void redo(c_editor *peditor)=0;
			virtual void release_objs(c_editor *peditor);

			c_geo_map *m_pmap;
		};

		class GOODDRAW_API operation_geometry_objs : public operation_item
		{
		public:
			operation_geometry_objs();
			virtual ~operation_geometry_objs();

			virtual int get_type()const;
			virtual void undo(c_editor *peditor);
			virtual void redo(c_editor *peditor);
			virtual void release_objs(c_editor *peditor);

			geometry_array m_old_objs;
			geometry_array m_new_objs;
		};

		class GOODDRAW_API operation_geometry_shape : public operation_item
		{
		public:
			operation_geometry_shape();
			virtual ~operation_geometry_shape();

			virtual int get_type()const;
			virtual void undo(c_editor *peditor);
			virtual void redo(c_editor *peditor);

			c_geometry *m_pobj;
			gd::vertex_array m_vertexs_old;
			gd::vertex_array m_vertexs_new;
		};

		class GOODDRAW_API operation_geometry_vertex : public operation_item
		{
		public:
			operation_geometry_vertex();
			virtual ~operation_geometry_vertex();

			virtual int get_type()const;
			virtual void undo(c_editor *peditor);
			virtual void redo(c_editor *peditor);

			bool isClosedPt();

			c_geometry *m_pobj;
			//m_old_index==m_new_index: modify, m_old_index==-1 && m_new_index>=0 : insert, m_old_index>=0 && m_new_index==-1 : delete
			int m_old_index, m_new_index;
			gd::GeoVertex m_old_vertex, m_new_vertex;
		};

		class GOODDRAW_API operation_geometry_properties : public operation_item
		{
		public:
			operation_geometry_properties();
			virtual ~operation_geometry_properties();

			virtual int get_type()const;
			virtual void undo(c_editor *peditor);
			virtual void redo(c_editor *peditor);

			c_geometry *m_pobj;
			gd::variant_array m_props_old, m_props_new;
		};

		class GOODDRAW_API operation_layer_objs : public operation_item
		{
		public:
			operation_layer_objs();
			virtual ~operation_layer_objs();

			virtual int get_type()const;
			virtual void undo(c_editor *peditor);
			virtual void redo(c_editor *peditor);
			virtual void release_objs(c_editor *peditor);

			c_geo_layer *m_pold;
			c_geo_layer *m_pnew;
		};

		class GOODDRAW_API operation_layer_properties : public operation_item
		{
		public:
			operation_layer_properties();
			virtual ~operation_layer_properties();

			virtual int get_type()const;
			virtual void undo(c_editor *peditor);
			virtual void redo(c_editor *peditor);

			c_geo_layer *m_pobj;
			gd::variant_array m_props_old, m_props_new;
		};

		c_edit_operation();
		~c_edit_operation();

		void set_name(const char* name);
		const char* get_name()const;

		void add_geometry_add_operation(c_geo_map *pmap, c_geometry *pobj);
		void add_geometry_del_operation(c_geo_map *pmap, c_geometry *pobj);
		void add_geometry_modify_operation(c_geo_map *pmap, c_geometry *pold, c_geometry *pnew, int modify_type);
		void undo(c_editor *peditor);
		void redo(c_editor *peditor);
		void release_objs(c_editor *peditor);
	protected:
		operation_geometry_objs* get_last_geometry_objs_operation();

	protected:
		std::string m_name;
		std::vector<operation_item*> m_operation_items;
	};

	class GOODDRAW_API c_edit_event_handler
	{
	public:
		c_edit_event_handler();
		virtual ~c_edit_event_handler();
		virtual void on_add_geo_map(c_geo_map *pobj) = 0;
		virtual void on_del_geo_map(const c_geo_map *pobj) = 0;
		virtual void on_add_geometry(c_geometry *pobj) = 0;
		virtual void on_del_geometry(const c_geometry *pobj) = 0;
		virtual void on_moidfy_geometry(const c_geometry *pold, c_geometry *pnew) = 0;
		virtual void on_add_geo_layer(c_geo_layer *pobj) = 0;
		virtual void on_del_geo_layer(const c_geo_layer *pobj) = 0;
		virtual void on_modify_geo_layer(const c_geo_layer *pold, c_geo_layer *pnew) = 0;

		virtual void on_cmd_start(c_command *pcmd);
		virtual void on_cmd_end(c_command *pcmd);
		virtual void on_selection_change();
	};

	class GOODDRAW_API c_editor
	{
	public:
		enum e_input_event
		{
			input_left_click = 1,
			input_right_click = 2,
			input_mouse_move = 3,
			input_key_down = 4,
			input_select = 5,
			input_draw_lineseg = 6,
			input_draw_linestring = 7
		};
		enum e_keydown_flag
		{
			e_ctrl = 1,
			e_shift = 2
		};
		enum e_geometry_modify_type
		{
			modify_shape = 1,
			modify_properties = 2,
			modify_vertex = 3,
		};
		typedef void(c_command::*point_func)(gd::Point3D pt);
		typedef void(c_command::*key_down_func)(int nchar, int flag);
		typedef void(c_command::*select_func)();
		typedef void(c_command::*draw_lineseg_func)(gd::Point3D pt1, gd::Point3D pt2);
		typedef void(c_command::*draw_linestring_func)(const gd::vertex_array& pts);

		struct GOODDRAW_API input_event_callback
		{
		public:
			input_event_callback();
			input_event_callback(c_command *p, int e, point_func func);
			input_event_callback(c_command *p, int e, key_down_func func);
			input_event_callback(c_command *p, int e, select_func func);
			input_event_callback(c_command *p, int e, draw_lineseg_func func);
			input_event_callback(c_command *p, int e, draw_linestring_func func);

			c_command *pobj;
			int event_type;
			union
			{
				point_func p_point_func;
				key_down_func p_key_down_func;
				select_func p_select_func;
				draw_lineseg_func p_draw_lineseg_func;
				draw_linestring_func p_draw_linestring_func;
			};
		};

		c_editor();
		virtual ~c_editor();

		virtual void add_geometry(c_geometry *pobj);
		virtual void del_geometry(const c_geometry *pobj);

		//pnew需要在modify_geometry函数外释放
		virtual void modify_geometry(const c_geometry *pold, c_geometry *pnew, int modify_type);

		virtual void add_geo_layer(c_geo_layer *player);
		virtual void del_geo_layer(const c_geo_layer *player);
		virtual void modify_geo_layer(const c_geo_layer *pold, c_geo_layer *pnew);

		virtual void set_workspace(c_workspace *p);
		virtual c_workspace *get_workspace()const;

		void set_input_event_callback(input_event_callback item);
		void clear_input_event_callbacks();
		void set_edit_event_handler(c_edit_event_handler *p);

		void start_edit_operation(const char* name);
		void end_edit_operation();

		void undo_edit_operations(int nsteps);
		void redo_edit_operations(int nsteps);

		c_command_params_file *cmdParamsFile();
		c_command_params_wnd *cmdParamsWnd();
		c_command_view *cmdView();

		virtual void left_click(gd::Point3D pt);
		virtual void right_click(gd::Point3D pt);
		virtual void mouse_move(gd::Point3D pt);
		virtual void key_down(int nChar, int flag);
		
		c_command *curCmd();
		void cancelCurCmd();

		typedef c_command* (*create_cmd_func)();
		struct cmd_create_func_item
		{
			char cmd_id[100];
			create_cmd_func func;
		};

		void add_create_cmd_func(create_cmd_func func);
		void doCmd(const char *cmd_id);

		void doNormalSelect();
		void start_input_event_callbacks();
		CommonTask *curCommonTask();

	public:
		//selection operations
		void selectAdd(const c_geometry *pobj);
		void selectRemove(const c_geometry *pobj);
		void selectSet(const_geometry_array& arr);

		int getSelectedCount();
		void getSelectedObjs(const_geometry_array& arr);

		void selectAll();
		void selectClear();

		bool isSelected(const c_geometry *pobj);
		const c_geometry *lastSelected();

	protected:
		input_event_callback find_input_event_callback(int nevent);
		void start_input_event_callbacks(input_event_callback item);
		void check_and_reset_cur_command();
		void check_and_end_common_task();

	protected:
		std::vector<input_event_callback> m_funcs, m_funcs0;
		c_edit_event_handler *m_edit_event_handler;
		c_workspace *m_workspace;

		std::vector<c_edit_operation*> m_operations;
		int m_ncur_operation_index;
		c_edit_operation *m_editing_operation;

		c_command_params_file *m_cmd_params_file;
		c_command_params_wnd *m_cmd_params_wnd;
		c_command_view *m_cmd_view;

		std::vector<cmd_create_func_item> m_cmd_regs;

		std::string m_cur_cmd_id, m_last_cmd_id;
		c_command *m_pcur_cmd;
		CommonTask *m_pCurCommonTask;
		input_event_callback m_cur_input_event_callback;

		const_geometry_array m_selectGeoObjs;
	};

}