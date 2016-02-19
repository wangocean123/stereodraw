#include "StdAfx.h"
#include "c_variant.h"


namespace StereoDraw
{
	template<class Type>
	Type to_type_simple(const c_variant& var)
	{
		switch (var.type)
		{
		case c_variant::type_null:
		case c_variant::type_int1:
		case c_variant::type_bool:
		case c_variant::type_string:
		case c_variant::type_time:
		case c_variant::type_vertex_array:
			return 0;
		case c_variant::type_int2:
			return (Type)var.iVal;
		case c_variant::type_int4:
			return (Type)var.lVal;
		case c_variant::type_float:
			return (Type)var.fltVal;
		case c_variant::type_double:
			return (Type)var.dblVal;
		case c_variant::type_int8:
			return (Type)var.i64Val;
		}
		return 0;
	}

	c_variant::c_variant()
	{
		memset(this, 0, sizeof(c_variant));
		type = type_null;
	}


	c_variant::~c_variant()
	{
		clear();
	}

	void c_variant::clear()
	{
		if (type == type_string)
		{
			delete text;
		}
		else if (type == type_vertex_array)
		{
			delete pts;
		}
		else if (type == type_blob)
		{
			delete[] blob.buf;
		}
		memset(this, 0, sizeof(c_variant));
		type = type_null;
	}

	c_variant::c_variant(const c_variant& a)
	{
		clear();

		type = a.type;
		memcpy(this, &a, sizeof(c_variant));
		if (type == type_string)
		{
			text = new std::string();
			*text = *(a.text);
		}
		else if (type == type_vertex_array)
		{
			pts = new StereoDraw::vertex_array();
			*pts = *(a.pts);
		}
		else if (type == type_blob)
		{
			blob.buf = new byte[a.blob.len];
			blob.len = a.blob.len;
			memcpy(blob.buf, a.blob.buf, a.blob.len);
		}
	}

	c_variant::c_variant(bool a)
	{
		*this = a;
	}

	c_variant::c_variant(char a)
	{
		*this = a;
	}

	c_variant::c_variant(short a)
	{
		*this = a;
	}

	c_variant::c_variant(int a)
	{
		*this = a;
	}

	c_variant::c_variant(long a)
	{
		*this = a;
	}

	c_variant::c_variant(float a)
	{
		*this = a;
	}

	c_variant::c_variant(double a)
	{
		*this = a;
	}

	c_variant::c_variant(const std::string& a)
	{
		*this = a;
	}

	c_variant::c_variant(const char* a)
	{
		*this = a;
	}

	c_variant::c_variant(const StereoDraw::vertex_array& a)
	{
		*this = a;
	}

	c_variant::c_variant(__int64 a)
	{
		*this = a;
	}

	c_variant::c_variant(const byte *buf, int len)
	{
		set_blob(buf,len);
	}

	void c_variant::set_time(long a)
	{
		clear();

		type = type_time;
		tVal = a;
	}

	bool c_variant::get_time(long& a)const
	{
		if (type == type_time)
		{
			a = tVal;
			return true;
		}
		return false;
	}

	void c_variant::set_blob(const byte *buf, int len)
	{
		clear();

		type = type_blob;
		blob.buf = new byte[len];
		memcpy(blob.buf, buf, len);
		blob.len = len;
	}


	bool c_variant::get_blob(const byte *&buf, int& len)const
	{
		if (type == type_blob)
		{
			buf = blob.buf;
			len = blob.len;
			return true;
		}
		return false;
	}

	c_variant& c_variant::operator=(bool a)
	{
		clear();
		type = type_bool;
		bVal = a;
		return *this;
	}

	c_variant& c_variant::operator=(char a)
	{
		clear();
		type = type_int1;
		chVal = a;
		return *this;
	}

	c_variant& c_variant::operator=(short a)
	{
		clear();
		type = type_int2;
		iVal = a;
		return *this;
	}

	c_variant& c_variant::operator=(int a)
	{
		clear();
		type = type_int4;
		lVal = a;
		return *this;
	}
	c_variant& c_variant::operator=(long a)
	{
		clear();
		type = type_int4;
		lVal = a;
		return *this;
	}

	c_variant& c_variant::operator=(float a)
	{
		clear();
		type = type_float;
		fltVal = a;
		return *this;
	}

	c_variant& c_variant::operator=(double a)
	{
		clear();
		type = type_double;
		dblVal = a;
		return *this;
	}

	c_variant& c_variant::operator=(const std::string& a)
	{
		clear();
		type = type_string;
		text = new std::string();
		*text = a;
		return *this;
	}

	c_variant& c_variant::operator=(const char* a)
	{
		clear();
		type = type_string;
		text = new std::string();
		*text = a;
		return *this;
	}

	c_variant& c_variant::operator=(const StereoDraw::vertex_array& a)
	{
		clear();
		type = type_vertex_array;
		pts = new StereoDraw::vertex_array();
		*pts = a;
		return *this;
	}

	c_variant& c_variant::operator=(__int64 a)
	{
		clear();
		type = type_int8;
		i64Val = a;
		return *this;
	}


	c_variant::operator bool() const
	{
		switch (type)
		{
		case c_variant::type_bool:
			return bVal;
		case c_variant::type_null:
		case c_variant::type_int1:
		case c_variant::type_string:
		case c_variant::type_time:
		case c_variant::type_vertex_array:
		case c_variant::type_float:
		case c_variant::type_double:
			return false;
		case c_variant::type_int2:
			return iVal!=0;
		case c_variant::type_int4:
			return lVal != 0;
		case c_variant::type_int8:
			return i64Val != 0;
		}
		return false;
	}

	c_variant::operator char() const
	{
		return chVal;
	}

	c_variant::operator short() const
	{
		return to_type_simple<short>(*this);
	}

	c_variant::operator int() const
	{
		return to_type_simple<int>(*this);
	}

	c_variant::operator long() const
	{
		return to_type_simple<long>(*this);
	}

	c_variant::operator float() const
	{
		return to_type_simple<float>(*this);
	}

	c_variant::operator double() const
	{
		return to_type_simple<double>(*this);
	}

	c_variant::operator const char*() const
	{
		if (type == type_string)
			return (*text).c_str();
		else
			return NULL;
	}

	c_variant::operator StereoDraw::vertex_array()const
	{
		if (type == type_vertex_array)
			return (*pts);
		else
			return StereoDraw::vertex_array();
	}

	c_variant::operator __int64() const
	{
		return to_type_simple<__int64>(*this);
	}

	static bool compare_vertex_array(const StereoDraw::vertex_array& pts1, const StereoDraw::vertex_array& pts2)
	{
		int len1 = pts1.size();
		int len2 = pts2.size();
		if (len1 != len2)
			return false;
		return memcmp(pts1.data(), pts2.data(), len1*sizeof(StereoDraw::geo_vertex)*len1);
	}

	bool c_variant::operator==(const c_variant& a)const
	{
		if (type == a.type)
		{
			switch (type)
			{
			case type_null:
				return true;
			case type_int1:
				return chVal == a.chVal;
			case type_int2:
				return iVal == a.iVal;
			case type_int4:
				return lVal == a.lVal;
			case type_bool:
				return bVal == a.bVal;
			case type_float:
				return fltVal == a.fltVal;
			case type_double:
				return dblVal == a.dblVal;
			case type_string:
				return *text == *(a.text);
			case type_time:
				return tVal == a.tVal;
			case type_vertex_array:
				return compare_vertex_array(*pts,*a.pts);
			case type_int8:
				return i64Val == a.i64Val;
			case type_blob:
				if (blob.len == a.blob.len)
				{
					return (memcmp(blob.buf, a.blob.buf, blob.len) == 0);
				}
				else
				{
					return false;
				}
			}
		}
		return false;
	}

	bool c_variant::operator!=(const c_variant& a)const
	{
		return !(*this == a);
	}


	bool c_variant::is_valid()const
	{
		if (type == type_string)
		{
			return text != NULL;
		}
		else if (type == type_vertex_array)
		{
			return pts != NULL;
		}
		return true;
	}


	static std::string format(int v)
	{
		char text[100];
		sprintf_s(text, "%d", v);
		return std::string(text);
	}

	static std::string format(double v)
	{
		char text[100];
		sprintf_s(text, "%.6f", v);
		return std::string(text);
	}

	std::string c_variant::to_string()
	{
		switch (type)
		{
		case type_null:
			return std::string();
		case type_int1:
		{
			std::string text;
			text = chVal;
			return text;
		}
		case type_int2:
			return format((int)iVal);
		case type_int4:
			return format((int)lVal);
		case type_bool:
			return bVal ? "1" : "0";
		case type_float:
			return format((double)fltVal);
		case type_double:
			return format((double)dblVal);
		case type_string:
			return *text;
		case type_time:
			return format((int)tVal);
		case type_vertex_array:
			return std::string();
		case type_int8:
			return format((int)i64Val);
		}

		return std::string();
	}


	void c_variant::from_string(const char* text, int itype)
	{
		switch (itype)
		{
		case type_null:
			return;
		case type_int1:
		{
			type = itype;
			chVal = text[0];
			return;
		}
		case type_int2:
		{
			type = itype;
			int v = 0;
			sscanf(text, "%d", &v);
			iVal = v;
		}
		case type_int4:
		{
			type = itype;
			int v = 0;
			sscanf(text, "%d", &v);
			lVal = v;
		}
		case type_bool:
		{
			type = itype;
			int v = 0;
			sscanf(text, "%d", &v);
			bVal = (v != 0);
		}
		case type_float:
		{
			type = itype;
			sscanf(text, "%f", &fltVal);
		}
		case type_double:
		{
			type = itype;
			sscanf(text, "%f", &dblVal);
		}
		case type_string:
		{
			*this = text;
			return;
		}
		case type_time:
			return;
		case type_vertex_array:
			return;
		case type_int8:
		{
			type = itype;
			int v = 0;
			sscanf(text, "%d", &v);
			i64Val = v;
		}
		}
	}

	double c_variant::to_double()
	{
		switch (type)
		{
		case type_null:
			return 0;
		case type_int1:
		{
			char txt[2] = { chVal, (char)0 };
			double v = 0;
			sscanf(txt, "%lf", &v);
			return v;
		}
		case type_int2:
			return (double)iVal;
		case type_int4:
			return (double)lVal;
		case type_bool:
			return bVal ? 1 : 0;
		case type_float:
			return (double)fltVal;
		case type_double:
			return (double)dblVal;
		case type_string:
		{
			double v = 0;
			sscanf((*text).c_str(), "%lf", &v);
			return v;
		}
		case type_time:
			return (double)tVal;
		case type_vertex_array:
			return 0;
		case type_int8:
			return (double)i64Val;
		}

		return 0;
	}


	int c_variant::to_int()
	{
		switch (type)
		{
		case type_null:
			return 0;
		case type_int1:
		{
			char txt[2] = { chVal, (char)0 };
			int v = 0;
			sscanf(txt, "%d", &v);
			return v;
		}
		case type_int2:
			return (int)iVal;
		case type_int4:
			return (int)lVal;
		case type_bool:
			return bVal ? 1 : 0;
		case type_float:
			return (int)fltVal;
		case type_double:
			return (int)dblVal;
		case type_string:
		{
			int v = 0;
			sscanf((*text).c_str(), "%d", &v);
			return v;
		}
		case type_time:
			return (int)tVal;
		case type_vertex_array:
			return 0;
		case type_int8:
			return (int)i64Val;
		}

		return 0;
	}


	c_attr_table::c_attr_table()
	{
		m_begin_row = false;
		m_ncur_index = -1;
	}


	c_attr_table::~c_attr_table()
	{

	}


	void c_attr_table::begin_row()
	{
		variant_array *parray = new variant_array();
		if (!parray)
			return;

		m_begin_row = true;
		m_rows.push_back(parray);

		if (m_attr_names.size() > 0)
		{
			parray->assign(m_attr_names.size(),c_variant());
		}

		m_ncur_index = m_rows.size() - 1;
	}

	const variant_array *c_attr_table::get_cur_row_data()const
	{
		if (m_ncur_index >= 0 && m_ncur_index < m_rows.size())
			return m_rows[m_ncur_index];

		return NULL;
	}

	variant_array *c_attr_table::get_cur_row_data()
	{
		if (m_ncur_index >= 0 && m_ncur_index < m_rows.size())
			return m_rows[m_ncur_index];

		return NULL;
	}

	void c_attr_table::add_attr(const char *attr_name, const c_variant& value)
	{
		if (!m_begin_row)
			return;

		variant_array *parray = get_cur_row_data();
		if (!parray)
			return;

		for (int i = 0; i < m_attr_names.size(); i++)
		{
			if (_stricmp(m_attr_names[i].c_str(), attr_name) == 0)
			{
				(*parray)[i] = value;
				return;
			}
		}

		if (m_ncur_index == 0)
		{
			m_attr_names.push_back(std::string(attr_name));
			parray->push_back(value);
			m_attr_titles.push_back(std::string());
		}
	}

	void c_attr_table::end_row()
	{
		m_begin_row = false;
	}


	void c_attr_table::del_attr(const char *attr_name)
	{
		int index = -1;
		for (int i = 0; i < m_attr_names.size(); i++)
		{
			if (_stricmp(m_attr_names[i].c_str(), attr_name) == 0)
			{
				m_attr_names.erase(m_attr_names.begin() + i);
				m_attr_titles.erase(m_attr_titles.begin() + i);
				index = i;
				break;
			}
		}

		if (index >= 0)
		{
			for (int i = 0; i < m_rows.size(); i++)
			{
				variant_array *parray = m_rows[i];
				if (parray)
				{
					parray->erase(parray->begin() + index);
				}
			}
		}
	}

	void c_attr_table::del_all()
	{
		m_attr_names.clear();
		m_attr_titles.clear();
		for (int i = 0; i < m_rows.size(); i++)
		{
			variant_array *parray = m_rows[i];
			if (parray)
			{
				delete parray;
			}
		}

		m_rows.clear();
	}


	int  c_attr_table::get_attr_count()const
	{
		return m_attr_names.size();
	}

	int  c_attr_table::get_row_count()const
	{
		return m_rows.size();
	}

	void  c_attr_table::set_cur_row(int index)
	{
		m_ncur_index = index;
	}

	int  c_attr_table::get_cur_row()const
	{
		return m_ncur_index;
	}

	void c_attr_table::get_cur_row_data(variant_array& arr)const
	{
		if (m_ncur_index >= 0 && m_ncur_index < m_rows.size())
		{
			arr = *m_rows[m_ncur_index];
		}
	}


	void c_attr_table::set_cur_row_data(const variant_array& arr)const
	{
		if (m_ncur_index >= 0 && m_ncur_index < m_rows.size())
		{
			if (m_rows[m_ncur_index]->size()==arr.size())
				*m_rows[m_ncur_index] = arr;
		}
	}


	std::string c_attr_table::get_attr_name(int index)const
	{
		if (index >= 0 && index < m_attr_names.size())
		{
			return m_attr_names[index];
		}
		return std::string();
	}

	bool c_attr_table::get_attr_value(const char *attr_name, c_variant& value)const
	{
		const variant_array *parray = get_cur_row_data();
		if (!parray)
			return false;

		for (int i = 0; i < m_attr_names.size(); i++)
		{
			if (_stricmp(m_attr_names[i].c_str(), attr_name) == 0)
			{
				value = (*parray)[i];
				return true;
			}
		}

		return false;
	}

	bool c_attr_table::set_attr_value(const char *attr_name, const c_variant& value)
	{
		variant_array *parray = get_cur_row_data();
		if (!parray)
			return false;

		for (int i = 0; i < m_attr_names.size(); i++)
		{
			if (_stricmp(m_attr_names[i].c_str(), attr_name) == 0)
			{
				(*parray)[i] = value;
				return true;
			}
		}

		return false;
	}

	void c_attr_table::set_attr_title(const char *attr_name, const char *title)
	{
		for (int i = 0; i < m_attr_names.size(); i++)
		{
			if (_stricmp(m_attr_names[i].c_str(), attr_name) == 0)
			{
				m_attr_titles[i] = title;
				return;
			}
		}
	}


	std::string c_attr_table::get_attr_title(const char *attr_name)const
	{
		for (int i = 0; i < m_attr_names.size(); i++)
		{
			if (_stricmp(m_attr_names[i].c_str(), attr_name) == 0)
			{
				return m_attr_titles[i];
			}
		}

		return std::string();
	}

	std::string c_attr_table::get_attr_title(int index)const
	{
		if (index >= 0 && index < m_attr_titles.size())
		{
			return m_attr_titles[index];
		}
		return std::string();
	}

	template<class Type>
	bool templ_read_attr(const c_attr_table& tab, const char *attr_name, Type a)
	{
		c_variant var;
		if (tab.get_attr_value(attr_name, var))
		{
			a = var;
			return true;
		}
		return false;
	}

	bool c_attr_table::read_attr(const char *attr_name, bool& a)const
	{
		return templ_read_attr(*this, attr_name, a);
	}

	bool c_attr_table::read_attr(const char *attr_name, char& a)const
	{
		return templ_read_attr(*this, attr_name, a);
	}

	bool c_attr_table::read_attr(const char *attr_name, short& a)const
	{
		return templ_read_attr(*this, attr_name, a);
	}

	bool c_attr_table::read_attr(const char *attr_name, int& a)const
	{
		return templ_read_attr(*this, attr_name, a);
	}

	bool c_attr_table::read_attr(const char *attr_name, long& a)const
	{
		return templ_read_attr(*this, attr_name, a);
	}

	bool c_attr_table::read_attr(const char *attr_name, ulong& a)const
	{
		long a2 = 0;
		bool bret = templ_read_attr(*this, attr_name, a2);
		a = a2;
		return bret;
	}

	bool c_attr_table::read_attr(const char *attr_name, float& a)const
	{
		return templ_read_attr(*this, attr_name, a);
	}

	bool c_attr_table::read_attr(const char *attr_name, double& a)const
	{
		return templ_read_attr(*this, attr_name, a);
	}

	bool c_attr_table::read_attr(const char *attr_name, std::string& a)const
	{
		c_variant var;
		if (get_attr_value(attr_name, var))
		{
			a = (const char*)var;
			return true;
		}
		return false;
	}

	bool c_attr_table::read_attr(const char *attr_name, StereoDraw::vertex_array& a)const
	{
		return templ_read_attr(*this, attr_name, a);
	}

	bool c_attr_table::read_attr(const char *attr_name, __int64& a)const
	{
		return templ_read_attr(*this, attr_name, a);
	}

	c_variant_io::c_variant_io()
	{
		m_ncur_pos = -2;
		m_ncur_group_start_pos = 0;
		m_ncur_group_level = 0;
	}


	c_variant_io::~c_variant_io()
	{

	}


	bool c_variant_io::read_variant(FILE *fp, c_variant& var)
	{
		int info = 0;
		int nread = fread(&info, sizeof(int), 1, fp);
		if (nread != 1)
			return false;
		int type = info & 0xff;
		int len = (info >> 8);

		switch (type)
		{
		case c_variant::type_bool:
		{
			bool v;
			fread(&v, sizeof(v), 1, fp);
			var = v;
			break;
		}
		case c_variant::type_int1:
		{
			byte v;
			fread(&v, sizeof(v), 1, fp);
			var = v;
			break;
		}
		case c_variant::type_int2:
		{
			short v;
			fread(&v, sizeof(v), 1, fp);
			var = v;
			break;
		}
		case c_variant::type_int4:
		{
			long v;
			fread(&v, sizeof(v), 1, fp);
			var = v;
			break;
		}
		case c_variant::type_time:
		{
			long v;
			fread(&v, sizeof(v), 1, fp);
			var.set_time(v);
			break;
		}
		case c_variant::type_int8:
		{
			long long v;
			fread(&v, sizeof(v), 1, fp);
			var = v;
			break;
		}
		case c_variant::type_float:
		{
			float v;
			fread(&v, sizeof(v), 1, fp);
			var = v;
			break;
		}
		case c_variant::type_double:
		{
			double v;
			fread(&v, sizeof(v), 1, fp);
			var = v;
			break;
		}
		case c_variant::type_string:
		{
			char *buf = new char[len + 1];
			memset(buf, 0, len + 1);
			fread(buf, 1, len, fp);
			var = buf;
			delete[] buf;
			break;
		}
		default:
			return false;
		}

		return true;
	}

	bool c_variant_io::write_variant(FILE *fp, const c_variant& var)
	{
		int info = 0;

		switch (var.type)
		{
		case c_variant::type_int1:
		{
			info = var.type;
			fwrite(&info, sizeof(info), 1, fp);
			byte v = (char)var;
			fwrite(&v, sizeof(v), 1, fp);
			break;
		}
		case c_variant::type_int2:
		{
			info = var.type;
			fwrite(&info, sizeof(info), 1, fp);
			short v = var;
			fwrite(&v, sizeof(v), 1, fp);
			break;
		}
		case c_variant::type_int4:
		{
			info = var.type;
			fwrite(&info, sizeof(info), 1, fp);
			long v = var;
			fwrite(&v, sizeof(v), 1, fp);
			break;
		}
		case c_variant::type_time:
		{
			info = var.type;
			fwrite(&info, sizeof(info), 1, fp);
			long v = var;
			fwrite(&v, sizeof(v), 1, fp);
			break;
		}		
		case c_variant::type_int8:
		{
			info = var.type;
			fwrite(&info, sizeof(info), 1, fp);
			long long v = var;
			fwrite(&v, sizeof(v), 1, fp);
			break;
		}
		case c_variant::type_float:
		{
			info = var.type;
			fwrite(&info, sizeof(info), 1, fp);
			float v = var;
			fwrite(&v, sizeof(v), 1, fp);
			break;
		}
		case c_variant::type_double:
		{
			info = var.type;
			fwrite(&info, sizeof(info), 1, fp);
			double v = var;
			fwrite(&v, sizeof(v), 1, fp);
			break;
		}
		case c_variant::type_string:
		{
			std::string *str = var.text;
			int len = str->size() + 1;
			char *buf = new char[len + 1];
			memset(buf, 0, len + 1);
			strcpy(buf, (const char*)str);

			info = var.type + (len << 8);
			fwrite(&info, sizeof(info), 1, fp);
			fwrite(buf, 1, len, fp);
			delete[] buf;
			break;
		}
		default:
			return false;
		}
		return true;
	}

	bool c_variant_io::read_variant(FILE *fp, variant_array& vars)
	{
		c_variant var;
		while (read_variant(fp,var))
		{
			vars.push_back(var);
		}
		return true;
	}

	bool c_variant_io::write_variant(FILE *fp, const variant_array& vars)
	{
		for (int i = 0; i < vars.size(); i++)
		{
			write_variant(fp, vars[i]);
		}
		return true;
	}


	//write
	void c_variant_io::add_ele(const char *ele_name, const c_variant& value)
	{
		m_vars.push_back(ele_name);
		m_vars.push_back(value);
	}

	void c_variant_io::add_ele_group(const char *ele_name)
	{
		m_vars.push_back("{");
		m_vars.push_back(ele_name);
	}

	void c_variant_io::ele_group_end()
	{
		m_vars.push_back("}\n");
		m_vars.push_back("");
	}

	bool c_variant_io::write_file(const char *path)
	{
		FILE *fp = fopen(path, "wb");
		if (fp)
		{
			write_variant(fp, m_vars);
			fclose(fp);
			return true;
		}
		return false;
	}

	//read
	bool c_variant_io::read_file(const char *path)
	{
		m_vars.clear();
		FILE *fp = fopen(path, "rb");
		if (fp)
		{
			read_variant(fp, m_vars);
			fclose(fp);

			parse_levels();
			m_ncur_group_start_pos = 0;
			m_ncur_pos = -2;
			m_ncur_group_level = 0;

			return true;
		}
		return false;
	}

	void c_variant_io::parse_levels()
	{
		m_levels.clear();
		int size = m_vars.size();
		int level = 0;
		for (int i = 0; i < size; i += 2)
		{
			std::string str = (const char*)m_vars[i];
			if (strcmp(str.c_str(), "{") == 0)
			{
				m_levels.push_back(level);
				m_levels.push_back(0);
				level++;
			}
			else if (strcmp(str.c_str(), "}\n") == 0)
			{
				level--;
				m_levels.push_back(level);
				m_levels.push_back(0);
			}
			else
			{
				m_levels.push_back(level);
				m_levels.push_back(0);
			}
		}
	}
	bool c_variant_io::lookup(const char *name, c_variant& var, bool bfrom_top)
	{
		int size = m_levels.size();
		for (int i = bfrom_top ? m_ncur_group_start_pos : (m_ncur_pos + 2); i < size; i += 2)
		{
			if (m_levels[i] == (m_ncur_group_level - 1))
			{
				break;
			}
			if (m_levels[i] == m_ncur_group_level)
			{
				std::string str = (const char*)m_vars[i];
				if (strcmp(str.c_str(), "{") == 0)
				{
					str = (const char*)m_vars[i + 1];
					if (strcmp(str.c_str(), name) == 0)
					{
						var = m_vars[i + 1];
						m_ncur_pos = i;
						return true;
					}
				}
				else if (strcmp(str.c_str(), name) == 0)
				{
					var = m_vars[i + 1];
					m_ncur_pos = i;
					return true;
				}
			}
		}
		return false;
	}


	bool c_variant_io::find(const char *name, bool bfrom_top)
	{
		c_variant var;
		return lookup(name, var, bfrom_top);
	}

	void c_variant_io::into_group()
	{
		if (m_ncur_pos<m_levels.size() - 1 && m_levels[m_ncur_pos + 2] == (m_ncur_group_level + 1))
		{
			m_ncur_group_start_pos = m_ncur_pos + 2;
			m_ncur_group_level++;
		}
	}

	void c_variant_io::outof_group()
	{
		int size = m_levels.size();
		for (int i = (m_ncur_pos + 2); i < size; i += 2)
		{
			if (m_levels[i] == (m_ncur_group_level - 1))
			{
				m_ncur_pos = i - 2;
				break;
			}
		}
		m_ncur_group_level--;
		for (int i = m_ncur_group_start_pos; i >= 0; i -= 2)
		{
			if (m_levels[i] == (m_ncur_group_level - 1))
			{
				m_ncur_group_start_pos = i + 2;
				break;
			}
		}
	}

	bool c_variant_io::read_ele(const char *ele_name, bool& a)
	{
		c_variant var;
		if (lookup(ele_name, var, true))
		{
			a = var;
			return true;
		}
		return false;
	}

	bool c_variant_io::read_ele(const char *ele_name, char& a)
	{
		c_variant var;
		if (lookup(ele_name, var, true))
		{
			a = var;
			return true;
		}
		return false;
	}

	bool c_variant_io::read_ele(const char *ele_name, short& a)
	{
		c_variant var;
		if (lookup(ele_name, var, true))
		{
			a = var;
			return true;
		}
		return false;
	}

	bool c_variant_io::read_ele(const char *ele_name, int& a)
	{
		c_variant var;
		if (lookup(ele_name, var, true))
		{
			a = var;
			return true;
		}
		return false;
	}

	bool c_variant_io::read_ele(const char *ele_name, long& a)
	{
		c_variant var;
		if (lookup(ele_name, var, true))
		{
			a = var;
			return true;
		}
		return false;
	}

	bool c_variant_io::read_ele(const char *ele_name, float& a)
	{
		c_variant var;
		if (lookup(ele_name, var, true))
		{
			a = var;
			return true;
		}
		return false;
	}

	bool c_variant_io::read_ele(const char *ele_name, double& a)
	{
		c_variant var;
		if (lookup(ele_name, var, true))
		{
			a = var;
			return true;
		}
		return false;
	}

	bool c_variant_io::read_ele(const char *ele_name, std::string& a)
	{
		c_variant var;
		if (lookup(ele_name, var, true))
		{
			a = (const char*)var;
			return true;
		}
		return false;
	}

	bool c_variant_io::read_ele(const char *ele_name, __int64& a)
	{
		c_variant var;
		if (lookup(ele_name, var, true))
		{
			a = var;
			return true;
		}
		return false;
	}

	bool c_variant_io::read_ele(const char *ele_name, c_variant& a)
	{
		c_variant var;
		if (lookup(ele_name, var, true))
		{
			a = var;
			return true;
		}
		return false;
	}

}