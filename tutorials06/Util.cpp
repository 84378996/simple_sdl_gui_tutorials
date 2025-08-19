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

#pragma comment(lib, "imm32.lib")

std::vector<std::wstring> GetCandidateList(void* hwnd) {
	std::vector<std::wstring> candidates;
	HIMC hIMC = ImmGetContext((HWND)hwnd);
	if (hIMC) {
		DWORD size = ImmGetCandidateListW(hIMC, 0, NULL, 0);
		if (size > 0) {
			LPCANDIDATELIST candList = (LPCANDIDATELIST)new char[size];
			ImmGetCandidateListW(hIMC, 0, candList, size);
			for (DWORD i = 0; i < candList->dwCount; i++) {
				candidates.push_back(std::wstring((WCHAR*)((LPBYTE)candList + candList->dwOffset[i])));
			}
			delete[] candList;
		}
		ImmReleaseContext((HWND)hwnd, hIMC);
	}
	return candidates;
}

// 选择候选项
void SelectCandidate(void* hwnd, int index) {
	HIMC hIMC = ImmGetContext((HWND)hwnd);
	if (hIMC) {
		DWORD dwSize = ImmGetCandidateListW(hIMC, 0, NULL, 0);
		LPCANDIDATELIST lpCandList = (LPCANDIDATELIST)new BYTE[dwSize];
		if (ImmGetCandidateListW(hIMC, 0, lpCandList, dwSize)) {
			if (index >= 0 && index < (int)lpCandList->dwCount) {
				// 设置选择的候选项
				ImmNotifyIME(hIMC, NI_SELECTCANDIDATESTR, 0, index);
			}
		}
		delete[] lpCandList;
		ImmReleaseContext((HWND)hwnd, hIMC);
	}
}

// 获取当前选择的候选项索引
int GetCurrentCandidateIndex(void* hwnd) {
	int index = -1;

	HIMC hIMC = ImmGetContext((HWND)hwnd);
	if (hIMC) {
		DWORD dwSize = ImmGetCandidateListW(hIMC, 0, NULL, 0);
		LPCANDIDATELIST lpCandList = (LPCANDIDATELIST)new BYTE[dwSize];
		if (ImmGetCandidateListW(hIMC, 0, lpCandList, dwSize)) {
			index = lpCandList->dwSelection;
		}
		delete[] lpCandList;
		ImmReleaseContext((HWND)hwnd, hIMC);
	}

	return index;
}