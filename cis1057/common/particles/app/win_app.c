// win_app.c
#include <windows.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include "app.h"

static FrameBuffer g_fb;
static char *classname;

static BITMAPINFO bmi;
static HWND hwnd;
static HINSTANCE hInst;

// raw key state updated by WndProc
static uint8_t g_keys[KEY_COUNT];

static int vk_to_key(WPARAM vk) {
    switch (vk) {
    case VK_LEFT:    return KEY_LEFT;
    case VK_RIGHT:   return KEY_RIGHT;
    case VK_UP:      return KEY_UP;
    case VK_DOWN:    return KEY_DOWN;
    case VK_SPACE:   return KEY_SPACE;
    case VK_SHIFT:   return KEY_SHIFT;
    case 'F':        return KEY_F;
    case 'L':        return KEY_L;
    case VK_ESCAPE:  return KEY_ESC;
    default:         return -1;
    }
}

static LRESULT CALLBACK WndProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam) {
    switch (msg) {
    case WM_KEYDOWN: case WM_SYSKEYDOWN: {
        int k = vk_to_key(wParam);
        if (k >= 0) g_keys[k] = 1;
        return 0;
    }
    case WM_KEYUP: case WM_SYSKEYUP: {
        int k = vk_to_key(wParam);
        if (k >= 0) g_keys[k] = 0;
        return 0;
    }
    case WM_DESTROY: PostQuitMessage(0); return 0;
    default: return DefWindowProc(hwnd, msg, wParam, lParam);
    }
}

FrameBuffer *app_init(const char *title, uint32_t w, uint32_t h) {
    classname = malloc(strlen(title) + 4);
    if (!classname) return NULL;

    g_fb.w = w, g_fb.h = h;
    g_fb.raster = malloc(w * h * sizeof(uint32_t));
    if (!g_fb.raster) { free(classname); return NULL; }

    ZeroMemory(&bmi, sizeof(bmi));
    bmi.bmiHeader.biSize = sizeof(BITMAPINFOHEADER);
    bmi.bmiHeader.biWidth = w;
    bmi.bmiHeader.biHeight = -(LONG)h;
    bmi.bmiHeader.biPlanes = 1;
    bmi.bmiHeader.biBitCount = 32;
    bmi.bmiHeader.biCompression = BI_RGB;

    WNDCLASS wc;
    ZeroMemory(&wc, sizeof(wc));
    wc.lpfnWndProc = WndProc;
    wc.hInstance = hInst;
    wc.lpszClassName = strcat(strcpy(classname, title), "Wnd");
    wc.hCursor = LoadCursor(NULL, IDC_ARROW);
    RegisterClass(&wc);

    // compute correct window size for desired client area
    DWORD style = WS_OVERLAPPEDWINDOW;
    RECT rc = { 0, 0, (LONG)w, (LONG)h };
    AdjustWindowRectEx(&rc, style, FALSE, 0);

    hwnd = CreateWindowEx(
        0, wc.lpszClassName, title,
        style,
        CW_USEDEFAULT, CW_USEDEFAULT,
        rc.right - rc.left, rc.bottom - rc.top,
        NULL, NULL, hInst, NULL
    );
    ShowWindow(hwnd, SW_SHOW);

    memset(g_keys, 0, sizeof(g_keys));

    return &g_fb;
}

void app_shutdown(void) {
    free(classname);
    free(g_fb.raster);
}

int app_pump(Input *in) {
    // snapshot previous frame's key state for edge detection
    uint8_t prev[KEY_COUNT];
    memcpy(prev, in->down, KEY_COUNT);

    MSG msg;
    while (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE)) {
        if (msg.message == WM_QUIT) return 0;
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }

    // copy raw state into input and compute edges
    for (int i = 0; i < KEY_COUNT; i++) {
        in->down[i]     = g_keys[i];
        in->pressed[i]  = g_keys[i] && !prev[i];
        in->released[i] = !g_keys[i] && prev[i];
    }

    return 1;
}

void app_present(const FrameBuffer *fb) {
    HDC dc = GetDC(hwnd);
    StretchDIBits(dc,
        0, 0, fb->w, fb->h,
        0, 0, fb->w, fb->h,
        fb->raster, &bmi, DIB_RGB_COLORS, SRCCOPY
    );
    ReleaseDC(hwnd, dc);
}

double app_time(void) {
    static LARGE_INTEGER freq = {0};
    if (!freq.QuadPart) QueryPerformanceFrequency(&freq);
    LARGE_INTEGER t;
    QueryPerformanceCounter(&t);
    return (double)t.QuadPart / (double)freq.QuadPart;
}

// --- platform entry point ---

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrev, LPSTR cmdLine, int showCmd) {
    (void)hPrev; (void)cmdLine; (void)showCmd;
    hInst = hInstance;
    return main_loop();
}