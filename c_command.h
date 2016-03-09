#pragma once

#include "c_geometry.h"
#include "Variant.h"
#include "c_editor.h"

namespace gis
{
	class GOODDRAW_API c_command
	{
	public:
		enum e_state
		{
			state_initial = 0,
			state_started = 2,
			state_running = 3,
			state_finished = 4,
			state_canceled = 5
		};
	public:
		c_command();
		virtual ~c_command();

		void set_editor(c_editor *p);
		virtual const char* get_id()const;
		virtual const char* get_name()const;
		virtual void start();
		virtual void cancel();
		virtual bool restart_if_end();

		bool is_end();
		bool is_canceled();
		bool is_running();
	protected:
		void set_state(int state);

		int m_state;
		c_editor *m_app;
	};
}