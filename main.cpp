#include <windows.h>
#include <gdiplus.h>
#include <map>
#include <iostream>

#pragma comment (lib,"Gdiplus.lib")

const wchar_t* grab;
HWND hWnd;

std::map<const wchar_t*, Gdiplus::Image*> map;

void load(const wchar_t* file)
{
    map[file] = new Gdiplus::Image(file);
}

const int CELL_SIZE = 50;
void draw(HDC dc, const wchar_t* file, int x, int y)
{
    Gdiplus::Rect rect(x, y, CELL_SIZE, CELL_SIZE);
    Gdiplus::Graphics grpx(dc);
    grpx.DrawImage(map[file], rect);
}

const wchar_t* field[8][8] = {};
void drawField(HDC dc) {
    for (int row = 0; row < 8; ++row)
        for (int col = 0; col < 8; ++col) {
            draw(dc, L"cell.png", col * CELL_SIZE, row * CELL_SIZE);
            draw(dc, field[row][col], col * CELL_SIZE, row * CELL_SIZE);
        }
}

void click() {
    POINT cursorPos;
    GetCursorPos(&cursorPos);
    RECT windowRect;
    GetWindowRect(hWnd, &windowRect);
    int x = cursorPos.x - windowRect.left;
    int y = cursorPos.y - windowRect.top;
    grab = field[y / CELL_SIZE][x / CELL_SIZE];
}

void drawGrabbed(HDC dc) {
    std::cout << grab << std::endl;
    POINT cursorPos;
    GetCursorPos(&cursorPos);
    RECT windowRect;
    GetWindowRect(hWnd, &windowRect);
    int x = cursorPos.x - windowRect.left;
    int y = cursorPos.y - windowRect.top;
    draw(dc, grab, x, y);
}

// Window Procedure function declaration
LRESULT CALLBACK WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam);

// Define a timer ID
#define TIMER_ID 1

// Define the timer interval in milliseconds
#define TIMER_INTERVAL_MS 1000 / 1

// Timer callback function
VOID CALLBACK TimerProc(HWND hwnd, UINT uMsg, UINT_PTR idEvent, DWORD dwTime) {
    // This function will be called every TIMER_INTERVAL_MS milliseconds
    // Perform your desired actions here

    InvalidateRect(hwnd, 0, 0);
}

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow) {
    // Register the window class
    const char CLASS_NAME[] = "Sample Window Class";

    WNDCLASS wc = {};

    wc.lpfnWndProc = WindowProc;
    wc.hInstance = hInstance;
    wc.lpszClassName = CLASS_NAME;

    RegisterClass(&wc);

    // Create the window
    hWnd = CreateWindowEx(
        0,                              // Optional window styles
        CLASS_NAME,                     // Window class
        "Minimal WinAPI Window",        // Window text
        WS_OVERLAPPEDWINDOW,            // Window style

        // Size and position
        CW_USEDEFAULT, CW_USEDEFAULT, 9 * CELL_SIZE, 9 * CELL_SIZE,

        NULL,       // Parent window
        NULL,       // Menu
        hInstance,  // Instance handle
        NULL        // Additional application data
    );

    if (hWnd == NULL) {
        return 0;
    }

    // Display the window
    ShowWindow(hWnd, nCmdShow);

    // start up GDI+ -- only need to do this once per process at startup
    Gdiplus::GdiplusStartupInput gdiplusStartupInput;
    ULONG_PTR gdiplusToken;
    GdiplusStartup(&gdiplusToken, &gdiplusStartupInput, NULL);

    load(L"black_king.png");
    load(L"black_queen.png");
    load(L"black_slon.png");
    load(L"black_horse.png");
    load(L"black_tura.png");
    load(L"black_peshka.png");
    load(L"white_king.png");
    load(L"white_queen.png");
    load(L"white_slon.png");
    load(L"white_horse.png");
    load(L"white_tura.png");
    load(L"white_peshka.png");
    load(L"cell.png");

    for (int col = 0; col != 8; ++col) {
        field[1][col] = L"black_peshka.png";
        field[6][col] = L"white_peshka.png";
    }

    field[0][0] = L"black_tura.png";
    field[0][7] = L"black_tura.png";
    field[0][1] = L"black_horse.png";
    field[0][6] = L"black_horse.png";
    field[0][2] = L"black_slon.png";
    field[0][5] = L"black_slon.png";
    field[0][3] = L"black_queen.png";
    field[0][4] = L"black_king.png";

    field[7][0] = L"white_tura.png";
    field[7][7] = L"white_tura.png";
    field[7][1] = L"white_horse.png";
    field[7][6] = L"white_horse.png";
    field[7][2] = L"white_slon.png";
    field[7][5] = L"white_slon.png";
    field[7][3] = L"white_queen.png";
    field[7][4] = L"white_king.png";

    UINT_PTR timerId = SetTimer(hWnd, TIMER_ID, TIMER_INTERVAL_MS, TimerProc);

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

int main() {
    WinMain(0, 0, 0, 1);
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
        FillRect(hdc, &ps.rcPaint, (HBRUSH)(COLOR_WINDOW + 2));
        drawField(hdc);
        drawGrabbed(hdc);
        EndPaint(hwnd, &ps);
        return 0;
    }
    case WM_LBUTTONDOWN: {
        click();
        return 0;
    }
    }
    return DefWindowProc(hwnd, uMsg, wParam, lParam);
}