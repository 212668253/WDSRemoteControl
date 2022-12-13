#include "Logger.h"

namespace {
    static HWND g_infoLog = nullptr;
}

void SetLoggerWindow(HWND hwndLogger)
{
    g_infoLog = hwndLogger;
}

HWND GetLoggerWindow()
{
    return g_infoLog;
}

void WriteToLog(const std::wstring& text)
{
    if (!text.empty() && g_infoLog) {
        SYSTEMTIME time;
        GetLocalTime(&time);
        std::wstring str(std::to_wstring(time.wHour) + L':' + std::to_wstring(time.wMinute) + L':' + std::to_wstring(time.wSecond) + L" " + text + L"\r\n");
        int len = GetWindowTextLength(g_infoLog);
        SendMessage(g_infoLog, EM_SETSEL, (WPARAM)len, (LPARAM)len);
        SendMessage(g_infoLog, EM_REPLACESEL, 0, (LPARAM)str.c_str());
    }
}
