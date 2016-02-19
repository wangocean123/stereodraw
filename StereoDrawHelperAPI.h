#pragma once
#include <xstring>

std::wstring convert_string(const std::string& str);
std::string convert_string(const std::wstring& str);

const wchar_t *convert_string(const char* str);
const char *convert_string(const wchar_t* wstr);