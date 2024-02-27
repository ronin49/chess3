#include <windows.h>
#include <gdiplus.h>
#include <map>

#pragma comment (lib,"Gdiplus.lib")

std::map<const wchar_t*, Gdiplus::Image*> map;

void load(const wchar_t* file)
{
    map[file] = new Gdiplus::Image(file);
}

void draw(HDC dc, const wchar_t* file)
{
    Gdiplus::Rect rect(20, 20, 500, 500);
    Gdiplus::Graphics grpx(dc);
    grpx.DrawImage(map[file], rect);
}

// Window Procedure function declaration
LRESULT CALLBACK WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam);

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow) {
    // Register the window class
    const char CLASS_NAME[] = "Sample Window Class";

    WNDCLASS wc = {};

    wc.lpfnWndProc = WindowProc;
    wc.hInstance = hInstance;
    wc.lpszClassName = CLASS_NAME;

    RegisterClass(&wc);

    // Create the window
    HWND hwnd = CreateWindowEx(
        0,                              // Optional window styles
        CLASS_NAME,                     // Window class
        "Minimal WinAPI Window",        // Window text
        WS_OVERLAPPEDWINDOW,            // Window style

        // Size and position
        CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT,

        NULL,       // Parent window
        NULL,       // Menu
        hInstance,  // Instance handle
        NULL        // Additional application data
    );

    if (hwnd == NULL) {
        return 0;
    }

    // Display the window
    ShowWindow(hwnd, nCmdShow);

    // start up GDI+ -- only need to do this once per process at startup
    Gdiplus::GdiplusStartupInput gdiplusStartupInput;
    ULONG_PTR gdiplusToken;
    GdiplusStartup(&gdiplusToken, &gdiplusStartupInput, NULL);

    // Run the message loop
    MSG msg = {};
    while (GetMessage(&msg, NULL, 0, 0)) {
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }

    // shut down - only once per process
    Gdiplus::GdiplusShutdown(gdiplusToken);
    return 0;
}

// Window Procedure function definition
LRESULT CALLBACK WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam) {
    switch (uMsg) {
    case WM_DESTROY:
        PostQuitMessage(0);
        return 0;
    case WM_PAINT: {
        PAINTSTRUCT ps;
        HDC hdc = BeginPaint(hwnd, &ps);
        FillRect(hdc, &ps.rcPaint, (HBRUSH)(COLOR_WINDOW + 1));
        load(L"test2.png");
        draw(hdc, L"test2.png");
        EndPaint(hwnd, &ps);
        return 0;
    }
    }
    return DefWindowProc(hwnd, uMsg, wParam, lParam);
}
