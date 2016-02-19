#pragma once

#include <string>
#include <vector>
#include <list>
#include "point_def.h"

namespace StereoDraw
{

	class STEREODRAW_API c_variant
	{
	public:
		enum e_type
		{
			type_null = 0,
			type_int1 = 1,
			type_int2 = 2,
			type_int4 = 3,
			type_bool = 4,
			type_float = 5,
			type_double = 6,
			type_string = 7,
			type_time = 8,
			type_vertex_array = 9,
			type_int8 = 10,
			type_blob = 11
		};
		int type;
		union
		{
			char chVal;
			bool bVal;
			short iVal;
			long lVal;
			float fltVal;
			double dblVal;
			__int64 i64Val;
			std::string *text;
			long tVal;
			StereoDraw::vertex_array *pts;
			struct
			{
				byte *buf;
				int len;
			}blob;
		};

	public:
		c_variant();
		~c_variant();
		c_variant(const c_variant& a);

		c_variant(bool a);
		c_variant(char a);
		c_variant(short a);
		c_variant(int a);
		c_variant(long a);
		c_variant(float a);
		c_variant(double a);
		c_variant(const std::string& a);
		c_variant(const char* a);
		c_variant(const StereoDraw::vertex_array& a);
		c_variant(__int64 a);
		c_variant(const byte *buf, int len);

		void clear();

		c_variant& operator=(bool a);
		c_variant& operator=(char a);
		c_variant& operator=(short a);
		c_variant& operator=(int a);
		c_variant& operator=(long a);
		c_variant& operator=(float a);
		c_variant& operator=(double a);
		c_variant& operator=(const std::string& a);
		c_variant& operator=(const char* a);
		c_variant& operator=(const StereoDraw::vertex_array& a);
		c_variant& operator=(__int64 a);

		operator bool() const;
		operator char() const;
		operator short() const;
		operator int() const;
		operator long() const;
		operator float() const;
		operator double() const;
		operator const char*() const;
		operator StereoDraw::vertex_array()const;
		operator __int64()const;
		
		void set_time(long a);
		bool get_time(long& a)const;

		void set_blob(const byte *buf, int len);
		bool get_blob(const byte *&buf, int& len)const;

		bool operator==(const c_variant& a)const;
		bool operator!=(const c_variant& a)const;

		bool is_valid()const;

		std::string to_string();
		void from_string(const char* text, int itype);

		double to_double();
		int to_int();
	};

	typedef std::vector<c_variant> variant_array;

	class STEREODRAW_API c_attr_table
	{
	public:
		c_attr_table();
		~c_attr_table();

		void begin_row();
		void add_attr(const char *attr_name, const c_variant& value);
		void end_row();

		void del_attr(const char *attr_name);
		void del_all();
		
		int  get_attr_count()const;
		int  get_row_count()const;
		void set_cur_row(int index);
		int  get_cur_row()const;

		std::string get_attr_name(int index)const;
		bool get_attr_value(const char *attr_name, c_variant& value)const;
		bool set_attr_value(const char *attr_name, const c_variant& value);
		void set_attr_title(const char *attr_name, const char *title);
		std::string get_attr_title(const char *attr_name)const;
		std::string get_attr_title(int index)const;

		bool read_attr(const char *attr_name, bool& a)const;
		bool read_attr(const char *attr_name, char& a)const;
		bool read_attr(const char *attr_name, short& a)const;
		bool read_attr(const char *attr_name, int& a)const;
		bool read_attr(const char *attr_name, long& a)const;
		bool read_attr(const char *attr_name, float& a)const;
		bool read_attr(const char *attr_name, double& a)const;
		bool read_attr(const char *attr_name, std::string& a)const;
		bool read_attr(const char *attr_name, StereoDraw::vertex_array& a)const;
		bool read_attr(const char *attr_name, __int64& a)const;
		bool read_attr(const char *attr_name, ulong& a)const;

		void get_cur_row_data(variant_array& arr)const;
		void set_cur_row_data(const variant_array& arr)const;

	protected:
		const variant_array *get_cur_row_data()const;
		variant_array *get_cur_row_data();

		std::vector<std::string> m_attr_names;
		std::vector<variant_array*> m_rows;
		std::vector<std::string> m_attr_titles;

		bool m_begin_row;
		int m_ncur_index;
	};

	class c_variant_io
	{
	public:
		c_variant_io();
		~c_variant_io();

		static bool read_variant(FILE *fp, c_variant& var);
		static bool write_variant(FILE *fp, const c_variant& var);

		static bool read_variant(FILE *fp, variant_array& vars);
		static bool write_variant(FILE *fp, const variant_array& vars);

		//write
		void add_ele(const char *ele_name, const c_variant& value);
		void add_ele_group(const char *ele_name);
		void ele_group_end();
		bool write_file(const char *path);

		//read
		bool read_file(const char *path);
		bool find(const char *name, bool bfrom_top);
		void into_group();
		void outof_group();

		bool read_ele(const char *ele_name, bool& a);
		bool read_ele(const char *ele_name, char& a);
		bool read_ele(const char *ele_name, short& a);
		bool read_ele(const char *ele_name, int& a);
		bool read_ele(const char *ele_name, long& a);
		bool read_ele(const char *ele_name, float& a);
		bool read_ele(const char *ele_name, double& a);
		bool read_ele(const char *ele_name, std::string& a);
		bool read_ele(const char *ele_name, __int64& a);
		bool read_ele(const char *ele_name, c_variant& a);
	private:
		void parse_levels();
		bool lookup(const char *name, c_variant& var, bool bfrom_top);

		std::vector<c_variant> m_vars;
		std::vector<int> m_levels;
		int m_ncur_pos;
		int m_ncur_group_start_pos;
		int m_ncur_group_level;
	};

}