#include "StdAfx.h"
#include "c_command.h"



namespace gis
{
	c_command::c_command()
	{
		m_app = NULL;
		m_state = state_initial;
	}


	c_command::~c_command()
	{
	}

	const char* c_command::get_id()const
	{
		return "";
	}

	const char* c_command::get_name()const
	{
		return "";
	}

	void c_command::set_editor(c_editor *p)
	{
		m_app = p;
	}

	void c_command::start()
	{
		m_state = state_started;
	}

	void c_command::cancel()
	{
		m_state = state_canceled;
	}


	bool c_command::is_end()
	{
		return (m_state == state_finished || m_state == state_canceled);
	}

	bool c_command::is_canceled()
	{
		return m_state == state_canceled;
	}

	bool c_command::is_running()
	{
		return m_state == state_running;
	}

	void c_command::set_state(int state)
	{
		m_state = state;
	}

	bool c_command::restart_if_end()
	{
		return true;
	}
}