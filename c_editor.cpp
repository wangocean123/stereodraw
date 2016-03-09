#include "StdAfx.h"
#include "c_editor.h"
#include "c_command.h"
#include "c_workspace.h"

namespace gis
{
	c_command_params_file::c_command_params_file()
	{

	}

	c_command_params_file::~c_command_params_file()
	{

	}


	bool c_command_params_file::open_file(const char* path)
	{
		return false;
	}


	void c_command_params_file::begin_command_params(const char* cmd_name)
	{

	}

	void c_command_params_file::read_param(const char *param_name, bool& v)
	{

	}

	void c_command_params_file::read_param(const char *param_name, int& v)
	{

	}

	void c_command_params_file::read_param(const char *param_name, long& v)
	{

	}

	void c_command_params_file::read_param(const char *param_name, float& v)
	{

	}

	void c_command_params_file::read_param(const char *param_name, double& v)
	{

	}

	void c_command_params_file::read_param(const char *param_name, std::string& v)
	{

	}


	void c_command_params_file::write_param(const char *param_name, bool& v)
	{

	}

	void c_command_params_file::write_param(const char *param_name, int& v)
	{

	}

	void c_command_params_file::write_param(const char *param_name, long& v)
	{

	}

	void c_command_params_file::write_param(const char *param_name, float& v)
	{

	}

	void c_command_params_file::write_param(const char *param_name, double& v)
	{

	}

	void c_command_params_file::write_param(const char *param_name, std::string& v)
	{

	}

	void c_command_params_file::end_command_params()
	{

	}



	c_command_params_wnd::c_command_params_wnd()
	{
	}


	c_command_params_wnd::~c_command_params_wnd()
	{
	}
	

	void c_command_params_wnd::set_title(const char *title)
	{

	}

	void c_command_params_wnd::set_msg_callback(c_command *pcmd, msg_callback func)
	{

	}

	void c_command_params_wnd::add_button(const char *id, const char *name)
	{

	}

	void c_command_params_wnd::add_edit(const char *id, const char *name, const char *initText)
	{

	}

	void c_command_params_wnd::add_file_button(const char *id, const char *name, const char *initPath, const char *file_filter)
	{

	}

	void c_command_params_wnd::add_files_button(const char *id, const char *name, const char *initPath, const char *file_filter)
	{

	}

	void c_command_params_wnd::add_folder_button(const char *id, const char *name, const char *initPath)
	{

	}

	void c_command_params_wnd::add_color_button(const char *id, const char *name, ulong color)
	{

	}

	void c_command_params_wnd::add_list(const char *id, const char *name, std::vector<std::string>& items, bool is_single_selection)
	{

	}

	void c_command_params_wnd::add_layer_button(const char *id, const char *name, const char *initText, bool is_single_selection)
	{

	}

	void c_command_params_wnd::add_linestring_type(const char *id, const char *name, int line_type)
	{

	}

	bool c_command_params_wnd::get_value(const char *id, gd::Variant& value)
	{
		return false;
	}

	void c_command_params_wnd::clear()
	{

	}

	c_command_view::c_command_view()
	{

	}

	c_command_view::~c_command_view()
	{

	}

	void c_command_view::set_static_draw(gd::GraphicsObject3D *pobj)
	{

	}

	void c_command_view::set_dynamic_draw(gd::GraphicsObject3D *pobj)
	{

	}

	gd::Point3D c_command_view::get_cur_point()
	{
		return gd::Point3D();
	}

	gd::matrix4d c_command_view::get_cur_matrix()
	{
		return gd::matrix4d(NULL);
	}


	c_editor::input_event_callback::input_event_callback()
	{
		pobj = NULL;
		event_type = 0;
		p_point_func = NULL;
	}

	c_editor::input_event_callback::input_event_callback(c_command *p, int e, point_func func)
	{
		pobj = p;
		event_type = e;
		p_point_func = func;
	}

	c_editor::input_event_callback::input_event_callback(c_command *p, int e, key_down_func func)
	{
		pobj = p;
		event_type = e;
		p_key_down_func = func;
	}

	c_editor::input_event_callback::input_event_callback(c_command *p, int e, select_func func)
	{
		pobj = p;
		event_type = e;
		p_select_func = func;
	}

	c_editor::input_event_callback::input_event_callback(c_command *p, int e, draw_lineseg_func func)
	{
		pobj = p;
		event_type = e;
		p_draw_lineseg_func = func;
	}

	c_editor::input_event_callback::input_event_callback(c_command *p, int e, draw_linestring_func func)
	{
		pobj = p;
		event_type = e;
		p_draw_linestring_func = func;
	}


	c_edit_operation::operation_item::operation_item()
	{
		m_pmap = NULL;
	}

	c_edit_operation::operation_item::~operation_item()
	{

	}

	int c_edit_operation::operation_item::get_type()const
	{
		return 0;
	}

	void c_edit_operation::operation_item::release_objs(c_editor *peditor)
	{
	}

	c_edit_operation::operation_geometry_objs::operation_geometry_objs()
	{
	}

	c_edit_operation::operation_geometry_objs::~operation_geometry_objs()
	{

	}

	int c_edit_operation::operation_geometry_objs::get_type()const
	{
		return type_geometry_objs;
	}

	void c_edit_operation::operation_geometry_objs::undo(c_editor *peditor)
	{
		c_temp_active_map am(peditor->get_workspace(),m_pmap);
		for (int i = 0; i < m_old_objs.size(); i++)
		{
			peditor->add_geometry(m_old_objs[i]);
		}
		for (int i = 0; i < m_new_objs.size(); i++)
		{
			peditor->del_geometry(m_new_objs[i]);
		}
	}

	void c_edit_operation::operation_geometry_objs::redo(c_editor *peditor)
	{
		c_temp_active_map am(peditor->get_workspace(),m_pmap);
		for (int i = 0; i < m_new_objs.size(); i++)
		{
			peditor->add_geometry(m_new_objs[i]);
		}
		for (int i = 0; i < m_old_objs.size(); i++)
		{
			peditor->del_geometry(m_old_objs[i]);
		}
	}

	void c_edit_operation::operation_geometry_objs::release_objs(c_editor *peditor)
	{
		for (int i = 0; i < m_old_objs.size(); i++)
		{
			m_pmap->release_geometry(m_new_objs[i]);
		}
	}


	c_edit_operation::operation_geometry_shape::operation_geometry_shape()
	{
		m_pobj = NULL;
	}

	c_edit_operation::operation_geometry_shape::~operation_geometry_shape()
	{

	}

	int c_edit_operation::operation_geometry_shape::get_type()const
	{
		return type_geometry_shape;
	}

	void c_edit_operation::operation_geometry_shape::undo(c_editor *peditor)
	{
		c_temp_active_map am(peditor->get_workspace(),m_pmap);
		c_geometry *pnew = m_pobj->clone();
		pnew->set_vertexs(m_vertexs_old.data(), m_vertexs_old.size());
		peditor->modify_geometry(m_pobj, pnew, c_editor::modify_shape);

		delete pnew;
	}

	void c_edit_operation::operation_geometry_shape::redo(c_editor *peditor)
	{
		c_temp_active_map am(peditor->get_workspace(),m_pmap);
		c_geometry *pnew = m_pobj->clone();
		pnew->set_vertexs(m_vertexs_new.data(), m_vertexs_new.size());
		peditor->modify_geometry(m_pobj, pnew, c_editor::modify_shape);

		delete pnew;
	}

	c_edit_operation::operation_geometry_vertex::operation_geometry_vertex()
	{
		m_pobj = NULL;
		m_old_index = m_new_index = -1;
	}

	c_edit_operation::operation_geometry_vertex::~operation_geometry_vertex()
	{

	}

	int c_edit_operation::operation_geometry_vertex::get_type()const
	{
		return type_geometry_vertex;
	}

	bool c_edit_operation::operation_geometry_vertex::isClosedPt()
	{
		if (m_pobj != NULL && m_old_index == 0)
		{
			const gis::c_geo_linear_obj *pobj = dynamic_cast<const gis::c_geo_linear_obj*>(m_pobj);
			if (pobj == NULL)
				return false;

			return pobj->is_closed();
		}
		return false;
	}

	void c_edit_operation::operation_geometry_vertex::undo(c_editor *peditor)
	{
		c_temp_active_map am(peditor->get_workspace(),m_pmap);
		c_geometry *pnew = m_pobj->clone();

		gd::vertex_array vertexs;
		m_pobj->get_vertexs(vertexs);

		//modify
		if (m_old_index >= 0 && m_new_index >= 0)
		{
			vertexs[m_old_index] = m_old_vertex;
			if (isClosedPt())
			{
				vertexs[vertexs.size() - 1].copyXYZ(m_old_vertex);
			}
		}
		//insert
		else if (m_old_index < 0 && m_new_index >= 0)
		{
			vertexs.erase(vertexs.begin() + m_new_index);
		}
		//delete
		else if (m_new_index < 0 && m_old_index >= 0)
		{
			vertexs.insert(vertexs.begin() + m_old_index, m_old_vertex);
			if (isClosedPt())
			{
				vertexs[vertexs.size() - 1].copyXYZ(m_old_vertex);
			}
		}

		pnew->set_vertexs(vertexs.data(), vertexs.size());
		peditor->modify_geometry(m_pobj, pnew, c_editor::modify_shape);

		delete pnew;
	}

	void c_edit_operation::operation_geometry_vertex::redo(c_editor *peditor)
	{
		c_temp_active_map am(peditor->get_workspace(),m_pmap);
		c_geometry *pnew = m_pobj->clone();

		gd::vertex_array vertexs;
		m_pobj->get_vertexs(vertexs);

		//modify
		if (m_old_index >= 0 && m_new_index >= 0)
		{
			vertexs[m_old_index] = m_new_vertex;
			if (isClosedPt())
			{
				vertexs[vertexs.size() - 1].copyXYZ(m_new_vertex);
			}
		}
		//insert
		else if (m_old_index < 0 && m_new_index >= 0)
		{
			vertexs.insert(vertexs.begin() + m_new_index, m_new_vertex);
		}
		//delete
		else if (m_new_index < 0 && m_old_index >= 0)
		{
			vertexs.erase(vertexs.begin() + m_old_index);
			if (isClosedPt())
			{
				vertexs[vertexs.size() - 1].copyXYZ(vertexs[0]);
			}
		}

		pnew->set_vertexs(vertexs.data(), vertexs.size());
		peditor->modify_geometry(m_pobj, pnew, c_editor::modify_shape);

		delete pnew;
	}

	c_edit_operation::operation_geometry_properties::operation_geometry_properties()
	{
		m_pobj = NULL;
	}

	c_edit_operation::operation_geometry_properties::~operation_geometry_properties()
	{

	}

	int c_edit_operation::operation_geometry_properties::get_type()const
	{
		return type_geometry_properties;
	}

	void c_edit_operation::operation_geometry_properties::undo(c_editor *peditor)
	{
		c_temp_active_map am(peditor->get_workspace(),m_pmap);
		c_geometry *pnew = m_pobj->clone();
		gd::AttributeSet tab;
		pnew->write_to(tab);
		tab.set_cur_row_data(m_props_old);
		pnew->read_from(tab);
		peditor->modify_geometry(m_pobj, pnew, c_editor::modify_shape);

		delete pnew;
	}

	void c_edit_operation::operation_geometry_properties::redo(c_editor *peditor)
	{
		c_geometry *pnew = m_pobj->clone();
		gd::AttributeSet tab;
		pnew->write_to(tab);
		tab.set_cur_row_data(m_props_new);
		pnew->read_from(tab);
		peditor->modify_geometry(m_pobj, pnew, c_editor::modify_shape);

		delete pnew;
	}


	c_edit_operation::operation_layer_objs::operation_layer_objs()
	{
		m_pold = NULL;
		m_pnew = NULL;
	}

	c_edit_operation::operation_layer_objs::~operation_layer_objs()
	{

	}

	int c_edit_operation::operation_layer_objs::get_type()const
	{
		return type_layer_objs;
	}

	void c_edit_operation::operation_layer_objs::undo(c_editor *peditor)
	{
		c_temp_active_map am(peditor->get_workspace(),m_pmap);
		peditor->add_geo_layer(m_pold);
		peditor->del_geo_layer(m_pnew);
	}

	void c_edit_operation::operation_layer_objs::redo(c_editor *peditor)
	{
		c_temp_active_map am(peditor->get_workspace(),m_pmap);
		peditor->add_geo_layer(m_pnew);
		peditor->del_geo_layer(m_pold);
	}


	void c_edit_operation::operation_layer_objs::release_objs(c_editor *peditor)
	{
		m_pmap->release_geo_layer(m_pnew);
	}


	c_edit_operation::operation_layer_properties::operation_layer_properties()
	{
		m_pobj = NULL;
	}

	c_edit_operation::operation_layer_properties::~operation_layer_properties()
	{

	}

	int c_edit_operation::operation_layer_properties::get_type()const
	{
		return type_layer_objs;
	}

	void c_edit_operation::operation_layer_properties::undo(c_editor *peditor)
	{
		c_temp_active_map am(peditor->get_workspace(),m_pmap);
		c_geo_layer *pnew = m_pobj->clone();
		gd::AttributeSet tab;
		pnew->write_to(tab);
		tab.set_cur_row_data(m_props_old);
		pnew->read_from(tab);
		peditor->modify_geo_layer(m_pobj, pnew);
	}

	void c_edit_operation::operation_layer_properties::redo(c_editor *peditor)
	{
		c_geo_layer *pnew = m_pobj->clone();
		gd::AttributeSet tab;
		pnew->write_to(tab);
		tab.set_cur_row_data(m_props_new);
		pnew->read_from(tab);
		peditor->modify_geo_layer(m_pobj, pnew);
	}


	c_edit_operation::c_edit_operation()
	{

	}

	c_edit_operation::~c_edit_operation()
	{

	}

	void c_edit_operation::set_name(const char* name)
	{
		m_name = name;
	}

	const char* c_edit_operation::get_name()const
	{
		return m_name.c_str();
	}

	void c_edit_operation::add_geometry_add_operation(c_geo_map *pmap, c_geometry *pobj)
	{
		c_edit_operation::operation_geometry_objs* p = get_last_geometry_objs_operation();
		if (p)
		{
			p->m_new_objs.push_back(pobj);
		}
		else
		{
			operation_geometry_objs *p = new operation_geometry_objs();
			p->m_pmap = pmap;
			p->m_new_objs.push_back(pobj);
			m_operation_items.push_back(p);
		}
	}

	void c_edit_operation::add_geometry_del_operation(c_geo_map *pmap, c_geometry *pobj)
	{
		c_edit_operation::operation_geometry_objs* p = get_last_geometry_objs_operation();
		if (p)
		{
			p->m_old_objs.push_back(pobj);
		}
		else
		{
			operation_geometry_objs *p = new operation_geometry_objs();
			p->m_pmap = pmap;
			p->m_old_objs.push_back(pobj);
			m_operation_items.push_back(p);
		}
	}

	void c_edit_operation::add_geometry_modify_operation(c_geo_map *pmap, c_geometry *pold, c_geometry *pnew, int modify_type)
	{
		if (modify_type == c_editor::modify_shape)
		{
			operation_geometry_shape *p = new operation_geometry_shape();
			p->m_pmap = pmap;
			p->m_pobj = pold;

			pnew->get_vertexs(p->m_vertexs_new);
			pold->get_vertexs(p->m_vertexs_old);
			m_operation_items.push_back(p);
		}
		else if (modify_type == c_editor::modify_properties)
		{
			operation_geometry_properties *p = new operation_geometry_properties();
			p->m_pmap = pmap;
			p->m_pobj = pold;

			gd::AttributeSet tab1, tab2;
			pold->write_to(tab1);
			pnew->write_to(tab2);

			tab1.del_attr("shape");
			tab2.del_attr("shape");

			tab1.get_cur_row_data(p->m_props_old);
			tab1.get_cur_row_data(p->m_props_new);
			m_operation_items.push_back(p);
		}
		else if (modify_type == c_editor::modify_vertex)
		{
			operation_geometry_vertex *p = new operation_geometry_vertex();
			p->m_pmap = pmap;
			p->m_pobj = pold;

			gd::vertex_array pts1, pts2;

			pold->get_vertexs(pts1);
			pnew->get_vertexs(pts2);

			//modify
			if (pts1.size() == pts2.size())
			{
				for (int i = 0; i < pts1.size(); i++)
				{
					if (pts2[i].x != pts1[i].x || pts2[i].y != pts1[i].y || pts2[i].z != pts1[i].z)
					{
						p->m_old_index = p->m_new_index = i;
						p->m_new_vertex = pts2[i];
						p->m_old_vertex = pts1[i];
						break;
					}
				}
			}
			//delete
			else if (pts1.size() == (pts2.size() + 1))
			{
				for (int i = 0; i < pts2.size(); i++)
				{
					if (pts2[i].x != pts1[i].x || pts2[i].y != pts1[i].y || pts2[i].z != pts1[i].z)
					{
						p->m_old_index = i;
						p->m_old_vertex = pts1[i];
						break;
					}
				}
				if (p->m_old_index < 0)
				{
					p->m_old_index = pts1.size() - 1;
					p->m_old_vertex = pts1[pts1.size() - 1];
				}
			}
			//insert
			else if (pts2.size() == (pts1.size() + 1))
			{
				for (int i = 0; i < pts1.size(); i++)
				{
					if (pts2[i].x != pts1[i].x || pts2[i].y != pts1[i].y || pts2[i].z != pts1[i].z)
					{
						p->m_new_index = i;
						p->m_new_vertex = pts2[i];
						break;
					}
				}
				if (p->m_new_index < 0)
				{
					p->m_new_index = pts2.size() - 1;
					p->m_new_vertex = pts2[pts2.size() - 1];
				}
			}

			m_operation_items.push_back(p);
		}
	}

	c_edit_operation::operation_geometry_objs* c_edit_operation::get_last_geometry_objs_operation()
	{
		if (m_operation_items.size() > 0)
		{
			operation_item *p = *(m_operation_items.end() - 1);
			if (p->get_type() == operation_item::type_geometry_objs)
			{
				return (c_edit_operation::operation_geometry_objs*)p;
			}
			else
			{
				c_edit_operation::operation_geometry_objs *p2 = new c_edit_operation::operation_geometry_objs();
				m_operation_items.push_back(p2);
				return p2;
			}
		}
		else
		{
			c_edit_operation::operation_geometry_objs *p = new c_edit_operation::operation_geometry_objs();
			m_operation_items.push_back(p);
			return p;
		}
	}

	void c_edit_operation::undo(c_editor *peditor)
	{
		for (int i = m_operation_items.size() - 1; i >= 0; i--)
		{
			m_operation_items[i]->undo(peditor);
		}
	}

	void c_edit_operation::redo(c_editor *peditor)
	{
		for (int i = 0; i < m_operation_items.size(); i++)
		{
			m_operation_items[i]->redo(peditor);
		}
	}


	void c_edit_operation::release_objs(c_editor *peditor)
	{
		for (int i = 0; i < m_operation_items.size(); i++)
		{
			m_operation_items[i]->release_objs(peditor);
		}
	}

	c_edit_event_handler::c_edit_event_handler()
	{

	}

	c_edit_event_handler::~c_edit_event_handler()
	{

	}

	void c_edit_event_handler::on_add_geo_map(c_geo_map *pobj)
	{
	}

	void c_edit_event_handler::on_del_geo_map(const c_geo_map *pobj)
	{
	}

	void c_edit_event_handler::on_add_geometry(c_geometry *pobj)
	{
	}

	void c_edit_event_handler::on_del_geometry(const c_geometry *pobj)
	{
	}

	void c_edit_event_handler::on_moidfy_geometry(const c_geometry *pold, c_geometry *pnew)
	{
	}

	void c_edit_event_handler::on_add_geo_layer(c_geo_layer *pobj)
	{
	}

	void c_edit_event_handler::on_del_geo_layer(const c_geo_layer *pobj)
	{
	}

	void c_edit_event_handler::on_modify_geo_layer(const c_geo_layer *pold, c_geo_layer *pnew)
	{
	}
	
	void c_edit_event_handler::on_cmd_start(c_command *pcmd)
	{
	}

	void c_edit_event_handler::on_cmd_end(c_command *pcmd)
	{
	}

	void c_edit_event_handler::on_selection_change()
	{
	}

	c_editor::c_editor()
	{
		m_edit_event_handler = NULL;
		m_workspace = NULL;
		m_ncur_operation_index = -1;
		m_editing_operation = NULL;
		m_pcur_cmd = NULL;
		m_pCurCommonTask = NULL;
	}


	c_editor::~c_editor()
	{
	}

	void c_editor::set_workspace(c_workspace *p)
	{
		m_workspace = p;
	}

	c_workspace* c_editor::get_workspace()const
	{
		return m_workspace;
	}

	void c_editor::add_geometry(c_geometry *pobj)
	{
		m_workspace->add_geometry(pobj);

		if (m_editing_operation)
		{
			m_editing_operation->add_geometry_add_operation(m_workspace->get_active_map(),pobj);
		}

		if (m_edit_event_handler)
		{
			m_edit_event_handler->on_add_geometry(pobj);
		}
	}

	void c_editor::del_geometry(const c_geometry *pobj)
	{
		if (m_editing_operation)
		{
			m_editing_operation->add_geometry_del_operation(m_workspace->get_active_map(), (gis::c_geometry*)pobj);
		}

		if (m_edit_event_handler)
		{
			m_edit_event_handler->on_del_geometry(pobj);
		}

		m_workspace->del_geometry(pobj);
	}

	void c_editor::modify_geometry(const c_geometry *pold, c_geometry *pnew, int modify_type)
	{
		if (m_editing_operation)
		{
			m_editing_operation->add_geometry_modify_operation(m_workspace->get_active_map(), (gis::c_geometry*)pold, pnew, modify_type);
		}

		if (m_edit_event_handler)
		{
			m_edit_event_handler->on_moidfy_geometry(pold, pnew);
		}

		m_workspace->replace_geometry(pold, pnew);
	}
	
	void c_editor::set_input_event_callback(input_event_callback item)
	{
		for (int i = 0; i < m_funcs.size(); i++)
		{
			if (item.event_type == m_funcs[i].event_type)
			{
				m_funcs[i] = item;
				return;
			}
		}
		m_funcs.push_back(item);
	}

	void c_editor::clear_input_event_callbacks()
	{
		m_funcs0.clear();
		m_funcs.clear();
		m_cur_input_event_callback = input_event_callback();
	}

	void c_editor::set_edit_event_handler(c_edit_event_handler *p)
	{
		m_edit_event_handler = p;
	}

	void c_editor::add_geo_layer(c_geo_layer * player)
	{
		m_workspace->add_geo_layer(player);

		if (m_edit_event_handler)
		{
			m_edit_event_handler->on_add_geo_layer(player);
		}
	}

	void c_editor::del_geo_layer(const c_geo_layer * player)
	{
		m_workspace->del_geo_layer(player);

		if (m_edit_event_handler)
		{
			m_edit_event_handler->on_del_geo_layer(player);
		}
	}

	void c_editor::modify_geo_layer(const c_geo_layer *pold, c_geo_layer *pnew)
	{
		if (m_edit_event_handler)
		{
			m_edit_event_handler->on_modify_geo_layer(pold, pnew);
		}
	}

	void c_editor::start_edit_operation(const char* name)
	{
		if (m_editing_operation)
			delete m_editing_operation;
		m_editing_operation = new c_edit_operation();
	}


	void c_editor::end_edit_operation()
	{
		if (m_ncur_operation_index != (m_operations.size() - 1))
		{
			for (int i = m_ncur_operation_index + 1; i<m_operations.size(); i++)
			{
				m_operations[i]->release_objs(this);
				delete m_operations[i];
			}

			m_operations.erase(m_operations.begin() + m_ncur_operation_index + 1, m_operations.end());
		}
		if (m_editing_operation->m_operation_items.size() > 0)
		{
			m_operations.push_back(m_editing_operation);
		}
		else
		{
			delete m_editing_operation;
		}

		m_editing_operation = NULL;
		m_ncur_operation_index = m_operations.size() - 1;
	}

	void c_editor::undo_edit_operations(int nsteps)
	{
		if (m_editing_operation)
			delete m_editing_operation;
		m_editing_operation = NULL;

		while (nsteps > 0 && m_ncur_operation_index >= 0)
		{
			nsteps--;
			m_operations[m_ncur_operation_index]->undo(this);
			m_ncur_operation_index--;
		}

		m_editing_operation = new c_edit_operation();
	}

	void c_editor::redo_edit_operations(int nsteps)
	{
		if (m_editing_operation)
			delete m_editing_operation;
		m_editing_operation = NULL;
		
		while (nsteps > 0 && m_ncur_operation_index < (int)(m_operations.size() - 1))
		{
			nsteps--;
			m_ncur_operation_index++;
			m_operations[m_ncur_operation_index]->redo(this);
		}

		m_editing_operation = new c_edit_operation();
	}

	c_command_params_file *c_editor::cmdParamsFile()
	{
		return m_cmd_params_file;
	}

	c_command_params_wnd *c_editor::cmdParamsWnd()
	{
		return m_cmd_params_wnd;
	}

	c_command_view *c_editor::cmdView()
	{
		return m_cmd_view;
	}

	c_editor::input_event_callback c_editor::find_input_event_callback(int nevent)
	{
		for (int i = 0; i < m_funcs.size(); i++)
		{
			if (m_funcs[i].event_type == nevent)
			{
				return m_funcs[i];
			}
		}
		return c_editor::input_event_callback(NULL, 0, (point_func)NULL);
	}

	void c_editor::left_click(gd::Point3D pt)
	{
		if (m_pCurCommonTask)
		{
			m_pCurCommonTask->left_click(pt);
			check_and_end_common_task();
			if ((m_pCurCommonTask == NULL || m_pCurCommonTask->is_end()) && m_cur_input_event_callback.pobj != NULL)
			{
				start_input_event_callbacks();
			}
			check_and_reset_cur_command();
		}
		else
		{
			input_event_callback item = find_input_event_callback(input_left_click);
			if (item.event_type != 0)
			{
				(item.pobj->*item.p_point_func)(pt);
				start_input_event_callbacks();
				check_and_reset_cur_command();
			}
		}
	}

	void c_editor::right_click(gd::Point3D pt)
	{
		if (m_pCurCommonTask)
		{
			m_pCurCommonTask->right_click(pt);
			check_and_end_common_task();
			if ((m_pCurCommonTask == NULL || m_pCurCommonTask->is_end()) && m_cur_input_event_callback.pobj != NULL)
			{
				start_input_event_callbacks();
			}
			check_and_reset_cur_command();
		}
		else
		{
			input_event_callback item = find_input_event_callback(input_right_click);
			if (item.event_type != 0)
			{
				(item.pobj->*item.p_point_func)(pt);
				start_input_event_callbacks();
				check_and_reset_cur_command();
			}
			else if (m_pcur_cmd)
			{
				cancelCurCmd();
				doCmd(m_cur_cmd_id.c_str());
			}
		}
	}

	void c_editor::mouse_move(gd::Point3D pt)
	{
		if (m_pCurCommonTask)
		{
			m_pCurCommonTask->mouse_move(pt);
			check_and_end_common_task();
			if ((m_pCurCommonTask == NULL || m_pCurCommonTask->is_end()) && m_cur_input_event_callback.pobj != NULL)
			{
				start_input_event_callbacks();
			}
			check_and_reset_cur_command();
		}
		else
		{
			input_event_callback item = find_input_event_callback(input_mouse_move);
			if (item.event_type != 0)
			{
				(item.pobj->*item.p_point_func)(pt);
				start_input_event_callbacks();
				check_and_reset_cur_command();
			}
		}
	}

	void c_editor::key_down(int nChar, int flag)
	{
		if (m_pCurCommonTask)
		{
			m_pCurCommonTask->key_down(nChar, flag);
			check_and_end_common_task();
			if ((m_pCurCommonTask == NULL || m_pCurCommonTask->is_end()) && m_cur_input_event_callback.pobj != NULL)
			{
				start_input_event_callbacks();
			}
			check_and_reset_cur_command();
		}
		else
		{
			input_event_callback item = find_input_event_callback(input_key_down);
			if (item.event_type != 0)
			{
				(item.pobj->*item.p_key_down_func)(nChar, flag);
				start_input_event_callbacks();
				check_and_reset_cur_command();
			}
		}
	}

	c_command *c_editor::curCmd()
	{
		return m_pcur_cmd;
	}

	void c_editor::cancelCurCmd()
	{
		clear_input_event_callbacks();
		if (m_pCurCommonTask)
		{
			m_pCurCommonTask->cancel();
			delete m_pCurCommonTask;
			m_pCurCommonTask = NULL;
		}

		if (m_pcur_cmd)
		{
			m_pcur_cmd->cancel();
			if (m_edit_event_handler)
			{
				m_edit_event_handler->on_cmd_end(m_pcur_cmd);
			}
			delete m_pcur_cmd;
			m_pcur_cmd = NULL;
		}

		if (cmdView())
		{
			cmdView()->set_static_draw(NULL);
			cmdView()->set_dynamic_draw(NULL);
		}
	}

	void c_editor::add_create_cmd_func(create_cmd_func func)
	{
		int i;
		for (i = 0; i < m_cmd_regs.size(); i++)
		{
			if (m_cmd_regs[i].func == func)
			{
				break;
			}
		}

		if (i >= m_cmd_regs.size())
		{
			c_command *pcmd = (*func)();
			if (pcmd == NULL)
				return;

			cmd_create_func_item item;
			item.func = func;
			strncpy(item.cmd_id, pcmd->get_id(), sizeof(item.cmd_id) / sizeof(item.cmd_id[0]));
			m_cmd_regs.push_back(item);
		}
	}

	void c_editor::doCmd(const char *cmd_id)
	{
		cancelCurCmd();

		m_cur_cmd_id = cmd_id;

		int i = 0;
		for (i = 0; i < m_cmd_regs.size(); i++)
		{
			if (_stricmp(m_cmd_regs[i].cmd_id, cmd_id) == 0)
			{
				break;
			}
		}

		if (i < m_cmd_regs.size())
		{
			m_pcur_cmd = (*m_cmd_regs[i].func)();
			m_pcur_cmd->set_editor(this);
			m_pcur_cmd->start();
			if (m_edit_event_handler)
			{
				m_edit_event_handler->on_cmd_start(m_pcur_cmd);
			}
		}
	}

	void c_editor::check_and_reset_cur_command()
	{
		if (m_pcur_cmd)
		{
			if (m_pcur_cmd->is_end())
			{
				bool brestart = m_pcur_cmd->restart_if_end();

				if (m_edit_event_handler)
				{
					m_edit_event_handler->on_cmd_end(m_pcur_cmd);
				}

				m_funcs.clear();
				if (m_pCurCommonTask)
				{
					m_pCurCommonTask->cancel();
					delete m_pCurCommonTask;
					m_pCurCommonTask = NULL;
				}
				delete m_pcur_cmd;
				m_pcur_cmd = NULL;

				if (cmdView())
				{
					cmdView()->set_static_draw(NULL);
					cmdView()->set_dynamic_draw(NULL);
				}

				if (brestart)
				{
					doCmd(m_cur_cmd_id.c_str());
				}
				else
				{
					doNormalSelect();
				}
			}
		}
	}

	void c_editor::check_and_end_common_task()
	{
		if (m_pCurCommonTask)
		{
			if (m_pCurCommonTask->is_end() && m_cur_input_event_callback.pobj != NULL)
			{
				if (m_cur_input_event_callback.event_type == input_select)
				{
					(m_cur_input_event_callback.pobj->*m_cur_input_event_callback.p_select_func)();
				}
				else if (m_cur_input_event_callback.event_type == input_draw_lineseg)
				{
					DrawLinesegTask *pcmd = dynamic_cast<DrawLinesegTask*>(m_pCurCommonTask);
					(m_cur_input_event_callback.pobj->*m_cur_input_event_callback.p_draw_lineseg_func)(pcmd->m_pt1, pcmd->m_pt2);
				}
				else if (m_cur_input_event_callback.event_type == input_draw_linestring)
				{
					DrawLinestringTask *pcmd = dynamic_cast<DrawLinestringTask*>(m_pCurCommonTask);
					(m_cur_input_event_callback.pobj->*m_cur_input_event_callback.p_draw_linestring_func)(pcmd->m_vertexs);
				}
			}
		}
	}

	void c_editor::doNormalSelect()
	{
		cancelCurCmd();
		if (m_pCurCommonTask)
		{
			m_pCurCommonTask->cancel();
			delete m_pCurCommonTask;
		}
		m_pCurCommonTask = new SelectTask(this);
	}

	void c_editor::start_input_event_callbacks()
	{
		//do nothing if the same
		if (m_funcs0.size() == m_funcs.size())
		{
			int i;
			for (i = 0; i < m_funcs.size(); i++)
			{
				if (m_funcs[i].event_type != m_funcs0[i].event_type || m_funcs[i].pobj != m_funcs0[i].pobj ||
					m_funcs[i].p_point_func != m_funcs0[i].p_point_func)
				{
					break;
				}
			}
			if (i >= m_funcs.size())
				return;
		}

		if (m_pCurCommonTask)
		{
			m_pCurCommonTask->cancel();
			delete m_pCurCommonTask;
			m_pCurCommonTask = NULL;
		}

		int inner_cmd_events[] = { input_draw_linestring, input_draw_lineseg, input_select };

		for (int i = 0; i < m_funcs.size(); i++)
		{
			for (int j = 0; j < sizeof(inner_cmd_events) / sizeof(inner_cmd_events[0]); j++)
			{
				if (m_funcs[i].event_type == inner_cmd_events[j])
				{
					start_input_event_callbacks(m_funcs[i]);
					break;
				}
			}
		}
		m_funcs0 = m_funcs;
	}

	void c_editor::start_input_event_callbacks(input_event_callback item)
	{
		m_cur_input_event_callback = item;
		if (m_pCurCommonTask)
		{
			m_pCurCommonTask->cancel();
			delete m_pCurCommonTask;
			m_pCurCommonTask = NULL;
		}
		if (item.event_type == input_select)
		{
			m_pCurCommonTask = new SelectTask(this);
		}
		else if (item.event_type == input_draw_lineseg)
		{
			m_pCurCommonTask = new DrawLinesegTask(this);
		}
		else if (item.event_type == input_draw_linestring)
		{
			m_pCurCommonTask = new DrawLinestringTask(this);
		}
	}

	CommonTask *c_editor::curCommonTask()
	{
		return m_pCurCommonTask;
	}

	void c_editor::selectAdd(const c_geometry *pobj)
	{
		m_selectGeoObjs.push_back(pobj);

		if (m_edit_event_handler)
		{
			m_edit_event_handler->on_selection_change();
		}
	}


	void c_editor::selectRemove(const c_geometry *pobj)
	{
		for (int i = 0; i < m_selectGeoObjs.size(); i++)
		{
			if (m_selectGeoObjs[i] == pobj)
			{
				m_selectGeoObjs.erase(m_selectGeoObjs.begin() + i);
			}
		}
		if (m_edit_event_handler)
		{
			m_edit_event_handler->on_selection_change();
		}
	}


	void c_editor::selectSet(const_geometry_array& arr)
	{
		m_selectGeoObjs = arr;
		if (m_edit_event_handler)
		{
			m_edit_event_handler->on_selection_change();
		}
	}

	int c_editor::getSelectedCount()
	{
		return m_selectGeoObjs.size();
	}


	void c_editor::getSelectedObjs(const_geometry_array& arr)
	{
		arr = m_selectGeoObjs;
	}

	void c_editor::selectAll()
	{
		m_selectGeoObjs.clear();
		c_geo_map *pmap = get_workspace()->get_active_map();
		if (!pmap)return;

		int nobj = pmap->get_geometry_count();
		for (int i = 0; i<nobj; i++)
		{
			m_selectGeoObjs.push_back(pmap->get_geometry(i));
		}
		if (m_edit_event_handler)
		{
			m_edit_event_handler->on_selection_change();
		}
	}


	void c_editor::selectClear()
	{
		m_selectGeoObjs.clear();
		if (m_edit_event_handler)
		{
			m_edit_event_handler->on_selection_change();
		}
	}

	bool c_editor::isSelected(const c_geometry *pobj)
	{
		for (int i = 0; i<m_selectGeoObjs.size(); i++)
		{
			if (pobj == m_selectGeoObjs[i])
				return true;
		}
		return false;
	}

	const c_geometry *c_editor::lastSelected()
	{
		if (m_selectGeoObjs.size() > 0)
			return m_selectGeoObjs[m_selectGeoObjs.size() - 1];

		return NULL;
	}

}