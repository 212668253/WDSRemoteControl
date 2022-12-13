#include <Windows.h>
#include "WinSharer.h"

namespace
{
    HWND startSharing = nullptr, stopSharing = nullptr;
    HINSTANCE hInstance = nullptr;
    std::unique_ptr<WinSharer> ws;

    HWND CreateButton(const wchar_t* lpButtonName, HWND hWnd, int x, int y, HMENU hmenu) {
        return CreateWindow(L"button", lpButtonName, WS_CHILD | WS_VISIBLE, x, y, 100, 30, hWnd, hmenu, (HINSTANCE)hWnd, 0);
    }
}

LRESULT CALLBACK ControlsWndProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam) {
    HDC hdc;
    PAINTSTRUCT ps;

    switch (msg) {
    case WM_PAINT:
        hdc = BeginPaint(hwnd, &ps);
        EndPaint(hwnd, &ps);
        break;
    case WM_DRAWITEM:
    {
        LPDRAWITEMSTRUCT pDIS;
        pDIS = (LPDRAWITEMSTRUCT)lParam;

        CHAR staticText[99];
        LRESULT len = SendMessage(pDIS->hwndItem, WM_GETTEXT, ARRAYSIZE(staticText), (LPARAM)staticText);

        if (pDIS->hwndItem == GetLoggerWindow()) {
            DrawTextA(pDIS->hDC, staticText, static_cast<int>(strlen(staticText)), &pDIS->rcItem, DT_CENTER | DT_SINGLELINE | DT_VCENTER);
        }
        if (pDIS->hwndItem == startSharing || pDIS->hwndItem == stopSharing) {
            SetBkMode(pDIS->hDC, TRANSPARENT);
            FillRect(pDIS->hDC, &pDIS->rcItem, CreateSolidBrush(RGB(0, 100, 200)));
            SetTextColor(pDIS->hDC, RGB(255, 255, 255));
            DrawTextA(pDIS->hDC, staticText, static_cast<int>(strlen(staticText)), &pDIS->rcItem, DT_CENTER | DT_SINGLELINE | DT_VCENTER);
            SetTextColor(pDIS->hDC, RGB(0, 0, 0));
            SelectObject(pDIS->hDC, GetStockObject(NULL_BRUSH));
            SelectObject(pDIS->hDC, CreatePen(PS_DOT, 1, RGB(255, 255, 255)));

            if (pDIS->itemAction & ODA_SELECT)
                SelectObject(pDIS->hDC, CreatePen(PS_DOT, 1, RGB(255, 255, 255)));
            else
                SelectObject(pDIS->hDC, CreatePen(PS_SOLID, 1, RGB(255, 255, 255)));
            Rectangle(
                pDIS->hDC,
                pDIS->rcItem.left,
                pDIS->rcItem.top,
                pDIS->rcItem.right,
                pDIS->rcItem.bottom
            );
        }
    }
    break;
    case WM_COMMAND:
        if (LOWORD(wParam) == 1001) {
            ws->open();
            ws->writeInvitation();
        }
        if (LOWORD(wParam) == 1002) {
            ws->close();
        }
        break;
    case WM_CLOSE:
        DestroyWindow(hwnd);
        break;
    case WM_DESTROY:
        ws->close();
        PostQuitMessage(0);
        break;
    default:
        return DefWindowProc(hwnd, msg, wParam, lParam);
    }
    return 0;
}

int WINAPI WinMain(HINSTANCE hInstance,
    HINSTANCE hPrevInstance,
    LPSTR lpCmdLine,
    int nCmdShow)
{
    CoInitialize(nullptr);
    ::hInstance = hInstance;
    WNDCLASSEX wc;

    wc.cbSize = sizeof(WNDCLASSEX);
    wc.style = 0;
    wc.lpfnWndProc = ControlsWndProc;
    wc.cbClsExtra = 0;
    wc.cbWndExtra = 0;
    wc.hInstance = hInstance;
    wc.hIcon = nullptr;
    wc.hCursor = LoadCursor(nullptr, IDC_ARROW);
    wc.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
    wc.lpszMenuName = nullptr;
    wc.lpszClassName = L"ControlPanel";
    wc.hIconSm = LoadIcon(nullptr, IDI_APPLICATION);

    RegisterClassEx(&wc);

    auto hwnd = CreateWindowEx(
        0,
        L"ControlPanel",
        L"REMOTE DESKTOP SHARING - INVITER",
        WS_OVERLAPPEDWINDOW,
        0, 0,
        700, 400,
        nullptr, nullptr,
        hInstance,
        nullptr);

    auto startSharing = CreateButton(L"Start Sharing", hwnd, 20, 20, (HMENU)1001);
    auto stopSharing = CreateButton(L"Stop Sharing", hwnd, 530, 20, (HMENU)1002);
    const HWND loggerWindow = CreateWindow(L"edit", 0, WS_CHILD | WS_VISIBLE | ES_MULTILINE | WS_VSCROLL | ES_AUTOVSCROLL | WS_BORDER, 20, 150, 650, 150, hwnd, 0, hInstance, 0);
    SetLoggerWindow(loggerWindow);
    
    ws = std::make_unique<WinSharer>();

    ShowWindow(hwnd, 1);
    UpdateWindow(hwnd);

    SendMessage(loggerWindow, EM_SETREADONLY, 1, 0);
    SetWindowLong(hwnd, GWL_STYLE, GetWindowLong(hwnd, GWL_STYLE) & ~WS_MAXIMIZEBOX);

    MSG Msg;
    while (GetMessage(&Msg, nullptr, 0, 0)) {
        TranslateMessage(&Msg);
        DispatchMessage(&Msg);
    }
    CoUninitialize();
    return 0;
}

