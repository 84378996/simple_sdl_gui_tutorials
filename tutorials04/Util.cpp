#include "Util.h"
#include <Windows.h>

std::wstring utf8_to_wstring(const std::string& utf8Str)
{
    if (utf8Str.empty()) return std::wstring();

    int sizeNeeded = MultiByteToWideChar(CP_UTF8, 0, utf8Str.c_str(), (int)utf8Str.size(), NULL, 0);
    if (sizeNeeded <= 0) {
        return std::wstring();
    }

    std::wstring wStr(sizeNeeded, 0);
    MultiByteToWideChar(CP_UTF8, 0, utf8Str.c_str(), (int)utf8Str.size(), &wStr[0], sizeNeeded);
    return wStr;
}

std::string wstring_to_utf8(const std::wstring& wStr)
{
    if (wStr.empty()) return std::string();

    int sizeNeeded = WideCharToMultiByte(CP_UTF8, 0, wStr.c_str(), (int)wStr.size(), NULL, 0, NULL, NULL);
    if (sizeNeeded <= 0) {
        return std::string();
    }

    std::string utf8Str(sizeNeeded, 0);
    WideCharToMultiByte(CP_UTF8, 0, wStr.c_str(), (int)wStr.size(), &utf8Str[0], sizeNeeded, NULL, NULL);
    return utf8Str;
}

std::wstring gbk_to_wstring(const std::string& str)
{
    if (str.empty()) return std::wstring();

    int sizeNeeded = MultiByteToWideChar(CP_ACP, 0, str.c_str(), (int)str.size(), NULL, 0);
    if (sizeNeeded <= 0) {
        return std::wstring();
    }

    std::wstring wStr(sizeNeeded, 0);
    MultiByteToWideChar(CP_ACP, 0, str.c_str(), (int)str.size(), &wStr[0], sizeNeeded);
    return wStr;
}

std::string wstring_to_gbk(const std::wstring& wStr)
{
    if (wStr.empty()) return std::string();

    int sizeNeeded = WideCharToMultiByte(CP_ACP, 0, wStr.c_str(), (int)wStr.size(), NULL, 0, NULL, NULL);
    if (sizeNeeded <= 0) {
        return std::string();
    }

    std::string utf8Str(sizeNeeded, 0);
    WideCharToMultiByte(CP_ACP, 0, wStr.c_str(), (int)wStr.size(), &utf8Str[0], sizeNeeded, NULL, NULL);
    return utf8Str;
}

std::string gbk_to_utf8(const std::string& gbk)
{
    std::wstring wstr = gbk_to_wstring(gbk);
    return wstring_to_utf8(wstr);
}

std::string utf8_to_gbk(const std::string& utf8)
{
    std::wstring wstr = utf8_to_wstring(utf8);
    return wstring_to_gbk(wstr);
}