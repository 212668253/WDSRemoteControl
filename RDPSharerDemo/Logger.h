#pragma once
#include <Windows.h>
#include <string>
#include <assert.h>
#include <stdexcept>

inline void CHECK(HRESULT hr) {
    if (FAILED(hr))
        throw std::runtime_error("Operation failed!");
}

void SetLoggerWindow(HWND hwndLogger);
HWND GetLoggerWindow();

void WriteToLog(const std::wstring& text);
