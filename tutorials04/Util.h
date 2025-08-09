#pragma once
#include <string>

std::wstring utf8_to_wstring(const std::string& str);
std::string wstring_to_utf8(const std::wstring& str);
std::wstring gbk_to_wstring(const std::string& str);
std::string wstring_to_gbk(const std::wstring& str);
std::string gbk_to_utf8(const std::string& gbk);
std::string utf8_to_gbk(const std::string& utf8);