#include "stdafx.h"
#include "StereoDrawHelperAPI.h"

std::wstring convert_string(const std::string& str)
{
	int len = str.length();
	if (len == 0)
	{
		return std::wstring(L"");
	}

	wchar_t *buf = new wchar_t[len + 1];
	::MultiByteToWideChar(CP_ACP, 0, str.c_str(), -1, buf, len+1);
	std::wstring ret = buf;
	delete[] buf;
	return ret;
}


std::string convert_string(const std::wstring& str)
{
	int len = str.length();
	if (len == 0)
	{
		return std::string("");
	}

	char *buf = new char[len+1];
	::WideCharToMultiByte(CP_ACP, 0, str.c_str(), -1, buf, len + 1, NULL, NULL);
	std::string ret = buf;
	delete[] buf;
	return ret;
}

const wchar_t *convert_string(const char* str)
{
	static std::wstring wstr;
	wstr = convert_string(std::string(str));
	return wstr.c_str();
}


const char *convert_string(const wchar_t* wstr)
{
	static std::string str;
	str = convert_string(std::wstring(wstr));
	return str.c_str();
}