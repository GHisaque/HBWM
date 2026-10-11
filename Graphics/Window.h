#pragma once
#include "Math.h"
#include "../Types.h"
#include "Image.h"
#include <windows.h>
#include <windowsx.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <stdint.h>
#include <float.h>
#include <math.h>

#ifndef GIP_WINDOW_H
#define GIP_WINDOW_H

typedef struct {
    vec2 pos;
    vec2 size;
    float rotation;
    vec2 offset; // Image-space rotation pivot in pixels.
} Rect;

typedef struct Window Window;

struct Window {
    HWND hwnd;
    vec2 size;
    uint* pixels;
    bool keys[256];
    vec2 mouse_pos;
    bool mouse_buttons[5];

    uint destroy_hook_count;
    uint resize_hook_count;
    uint update_hook_count;
    uint render_hook_count;
    uint keydown_hook_count;
    uint keyup_hook_count;
    uint mousemove_hook_count;
    uint mousedown_hook_count;
    uint mouseup_hook_count;

    void (*resize_hooks[256])(Window* win);
    void (*destroy_hooks[256])(Window* win);
    void (*render_hooks[256])(Window* win);
    void (*update_hooks[256])(Window* win);
    void (*keydown_hooks[256])(Window* win, int keycode);
    void (*keyup_hooks[256])(Window* win, int keycode);
    void (*mousemove_hooks[256])(Window* win, int x, int y);
    void (*mousedown_hooks[256])(Window* win, int button);
    void (*mouseup_hooks[256])(Window* win, int button);
};

typedef enum {
    IKC_LEFT = 0x25,
    IKC_UP = 0x26,
    IKC_RIGHT = 0x27,
    IKC_DOWN = 0x28,
    IKC_INSERT = 0x2D,
    IKC_ESCAPE = 0x1B,
    IKC_DELETE = 0x2E,
    IKC_HOME = 0x24,
    IKC_END = 0x23,
    IKC_PRIOR = 0x21,
    IKC_NEXT = 0x22,
    IKC_SHIFT = 0x10,
    IKC_LSHIFT = 0xA0,
    IKC_RSHIFT = 0xA1,
    IKC_CTRL = 0x11,
    IKC_LCTRL = 0xA2,
    IKC_RCTRL = 0xA3,
    IKC_ALT = 0x12,
    IKC_LALT = 0xA4,
    IKC_RALT = 0xA5,
    IKC_LHOME = 0x5B,
    IKC_RHOME = 0x5C,
    IKC_APPS = 0x5D,
    IKC_F1 = 0x70,
    IKC_F2 = 0x71,
    IKC_F3 = 0x72,
    IKC_F4 = 0x73,
    IKC_F5 = 0x74,
    IKC_F6 = 0x75,
    IKC_F7 = 0x76,
    IKC_F8 = 0x77,
    IKC_F9 = 0x78,
    IKC_F10 = 0x79,
    IKC_F11 = 0x7A,
    IKC_F12 = 0x7B,
    IKC_CAPS = 0x14,
    IKC_NUMLOCK = 0x90,
    IKC_SCROLL = 0x91,
    IKC_PRINT = 0x2C,
    IKC_PAUSE = 0x13,
    IKC_VMUTE = 0xAD,
    IKC_VDOWN = 0xAE,
    IKC_VUP = 0xAF,
    IKC_MNEXT = 0xB0,
    IKC_MPREV = 0xB1,
    IKC_MSTOP = 0xB2,
    IKC_MTOGGLE = 0xB3
} InaccessableKeyCode;

typedef enum {
    MOUS_LEFT,
    MOUS_MIDD,
    MOUS_RIGH,
    MOUS_X1,
    MOUS_X2
} MouseButton;

typedef enum {
    NONE,
    BAD_WINDOW,
    BAD_PIXELS,
    BAD_WIN_HANDLE
} WindowErrorReturnCode;

static inline LRESULT CALLBACK GIPCustomWndProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam) {
    Window* win = (Window*)GetWindowLongPtrW(hwnd, GWLP_USERDATA);

    if (msg == WM_NCCREATE) {
        CREATESTRUCTW* pCreate = (CREATESTRUCTW*)lParam;
        win = (Window*)pCreate->lpCreateParams;
        SetWindowLongPtrW(hwnd, GWLP_USERDATA, (LONG_PTR)win);
    }

    switch (msg) {
        case WM_ERASEBKGND:
            return 1;

        case WM_MOUSEMOVE: {
            int x = GET_X_LPARAM(lParam);
            int y = GET_Y_LPARAM(lParam);
            if (win) {
                win->mouse_pos = (vec2){
                    roundf(x), roundf(y)
                };
                for (uint i = 0; i < win->mousemove_hook_count; i++) {
                    if (win->mousemove_hooks[i] != NULL) win->mousemove_hooks[i](win, x, y);
                }
            }
        }

        case WM_MBUTTONDOWN: {
            if (win) {
                win->mouse_buttons[1] = true; 

                if (wParam & MK_LBUTTON) win->mouse_buttons[0] = true;
                if (wParam & MK_RBUTTON) win->mouse_buttons[2] = true;
                if (wParam & MK_XBUTTON1) win->mouse_buttons[3] = true;
                if (wParam & MK_XBUTTON2) win->mouse_buttons[4] = true;

                for (uint i = 0; i < win->mousedown_hook_count; i++) {
                    if (win->mousedown_hooks[i] != NULL) win->mousedown_hooks[i](win, (int)wParam);
                }
            }
            break;
        }

        case WM_MBUTTONUP: {
            if (win) {
                win->mouse_buttons[1] = false; 

                if (wParam & MK_LBUTTON) win->mouse_buttons[0] = false;
                if (wParam & MK_RBUTTON) win->mouse_buttons[2] = false;
                if (wParam & MK_XBUTTON1) win->mouse_buttons[3] = false;
                if (wParam & MK_XBUTTON2) win->mouse_buttons[4] = false;

                for (uint i = 0; i < win->mouseup_hook_count; i++) {
                    if (win->mouseup_hooks[i] != NULL) win->mouseup_hooks[i](win, (int)wParam);
                }
            }
            break;
        }

        case WM_DESTROY: {
            if (win) {
                KillTimer(hwnd, 1);
                for (uint i = 0; i < win->destroy_hook_count; i++) {
                    if (win->destroy_hooks[i] != NULL) win->destroy_hooks[i](win);
                }
            }
            PostQuitMessage(0);
            return 0;
        }

        case WM_SIZE: {
            if (win) {
                int new_width  = LOWORD(lParam);
                int new_height = HIWORD(lParam);

                if (new_width > 0 && new_height > 0) {
                    win->size.x = (float)new_width;
                    win->size.y = (float)new_height;

                    if (win->pixels) {
                        free(win->pixels);
                    }

                    size_t total_pixels = (size_t)new_width * (size_t)new_height;
                    win->pixels = (uint*)calloc(total_pixels, sizeof(uint));
                }

                for (uint i = 0; i < win->resize_hook_count; i++) {
                    if (win->resize_hooks[i] != NULL) win->resize_hooks[i](win);
                }

                RedrawWindow(hwnd, NULL, NULL, RDW_INVALIDATE | RDW_UPDATENOW);
            }
            return 0;
        }

        case WM_PAINT: {
            if (win) {
                PAINTSTRUCT ps;
                HDC hdc = BeginPaint(hwnd, &ps);

                for (uint i = 0; i < win->render_hook_count; i++) {
                    if (win->render_hooks[i] != NULL) win->render_hooks[i](win);
                }

                int w = (int)win->size.x;
                int h = (int)win->size.y;

                if (win->pixels && w > 0 && h > 0) {
                    BITMAPINFO bmi = {0};
                    bmi.bmiHeader.biSize        = sizeof(BITMAPINFOHEADER);
                    bmi.bmiHeader.biWidth       = w;
                    bmi.bmiHeader.biHeight      = -h;
                    bmi.bmiHeader.biPlanes      = 1;
                    bmi.bmiHeader.biBitCount    = 32;
                    bmi.bmiHeader.biCompression = BI_RGB;

                    StretchDIBits(
                        hdc,
                        0, 0, w, h,
                        0, 0, w, h,
                        win->pixels,
                        &bmi,
                        DIB_RGB_COLORS,
                        SRCCOPY
                    );
                }

                EndPaint(hwnd, &ps);
            }
            return 0;
        }

        case WM_TIMER: {
            if (win) {
                for (uint i = 0; i < win->update_hook_count; i++) {
                    if (win->update_hooks[i] != NULL) win->update_hooks[i](win);
                }

                InvalidateRect(hwnd, NULL, FALSE);
            }
            return 0;
        }

        case WM_KEYDOWN:
            if (win) {
                if (wParam < 256) {
                    for (uint i = 0; i < win->keydown_hook_count; i++) {
                        if (win->keydown_hooks[i] != NULL) win->keydown_hooks[i](win, wParam);
                    }
                    win->keys[wParam] = true;
                }
            }
            return 0;

        case WM_KEYUP:
            if (win) {
                if (wParam < 256) {
                    for (uint i = 0; i < win->keyup_hook_count; i++) {
                        if (win->keyup_hooks[i] != NULL) win->keyup_hooks[i](win, wParam);
                    }
                    win->keys[wParam] = false;
                }
            }
            return 0;
    }
    return DefWindowProcW(hwnd, msg, wParam, lParam);
}

static inline int Window_Init(Window* win, uint width, uint height) {
    if (!win) return BAD_WINDOW;

    memset(win, 0, sizeof(Window));

    RECT rect = { 0, 0, (LONG)width, (LONG)height };
    DWORD style = WS_OVERLAPPEDWINDOW | WS_VISIBLE;
    AdjustWindowRect(&rect, style, FALSE);

    WNDCLASSW wc = {0};
    wc.lpfnWndProc = GIPCustomWndProc;
    wc.hInstance = GetModuleHandleW(NULL);
    wc.lpszClassName = L"GIP_Engine_Class";
    wc.hCursor = LoadCursorW(NULL, (LPCWSTR)IDC_ARROW);

    RegisterClassW(&wc);

    win->hwnd = CreateWindowExW(
        0, L"GIP_Engine_Class", L"Window",
        style,
        CW_USEDEFAULT, CW_USEDEFAULT,
        rect.right - rect.left, rect.bottom - rect.top,
        NULL, NULL, wc.hInstance, win
    );

    if (!win->hwnd) {
        return BAD_WIN_HANDLE;
    }

    SetTimer(win->hwnd, 1, 16, NULL);

    return NONE;
}

static inline const char* Get_Window_Error_Msg(int code) {
    switch (code) {
        case BAD_WINDOW: return "Bad window\n";
        case BAD_PIXELS: return "Bad pixels\n";
        case BAD_WIN_HANDLE: return "Malformed window handle\n";
        default: return "No valid error code detected\n";
    }
}

static inline void Window_Free(Window* win) {
    if (win) {
        if (win->hwnd) {
            KillTimer(win->hwnd, 1);
            DestroyWindow(win->hwnd);
            win->hwnd = NULL;
        }
        if (win->pixels) {
            free(win->pixels);
            win->pixels = NULL;
        }
    }
}

static inline void Window_Close(Window* win) {
    if (!win || !win->hwnd) return;
    
    DestroyWindow(win->hwnd);
    win->hwnd = NULL;
}

static inline int Window_Is_Open(Window* win) {
    if (!win || !win->hwnd) return 0;
    
    MSG msg;
    while (PeekMessageW(&msg, NULL, 0, 0, PM_REMOVE)) {
        if (msg.message == WM_QUIT) {
            return 0;
        }
        TranslateMessage(&msg);
        DispatchMessageW(&msg);
    }
    return 1;
}

static inline void Window_Clear(Window* win, uint color) {
    if (!win || !win->pixels) return;

    int total_pixels = (int)(win->size.x * win->size.y);

    for (int i = 0; i < total_pixels; i++) {
        win->pixels[i] = color;
    }
}

static inline void Window_Clear_RGBA(Window* win, RGBA color) {
    Window_Clear(win, rgba_to_base10(color));
}

static inline void Window_Draw_Pixel_Index(Window* win, int index, uint color) {
    if (!win || !win->pixels) return;

    if (index > 0 && index <= (int)win->size.x * (int)win->size.y) {
        win->pixels[index] = color;
    }
}

static inline void Window_Draw_Pixel_Index_RGBA(Window* win, int index, RGBA color) {
    Window_Draw_Pixel_Index(win, index, rgba_to_base10(color));
}

static inline void Window_Draw_Pixel(Window* win, vec2 pos, uint color) {
    if (!win || !win->pixels) return;

    int x = (int)pos.x;
    int y = (int)pos.y;
    int w = (int)win->size.x;
    int h = (int)win->size.y;

    if (x >= 0 && y >= 0 && x < w && y < h) {
        Window_Draw_Pixel_Index(
            win,
            x + y * w,
            color
        );
    }
}

static inline void Window_Draw_Pixel_RGBA(Window* win, vec2 pos, RGBA color) {
    Window_Draw_Pixel(win, pos, rgba_to_base10(color));
}

static inline void Window_Draw_Line(Window* win, vec2 start, vec2 end, uint color) {
    if (!win || !win->pixels) return;

    int x0 = (int)start.x;
    int y0 = (int)start.y;
    int x1 = (int)end.x;
    int y1 = (int)end.y;

    int dx = abs(x1 - x0);
    int dy = abs(y1 - y0);

    int sx = (x0 < x1) ? 1 : -1;
    int sy = (y0 < y1) ? 1 : -1;

    int err = dx - dy;

    while (1) {
        vec2 p = { (float)x0, (float)y0 };
        Window_Draw_Pixel(win, p, color);

        if (x0 == x1 && y0 == y1) break;

        int e2 = 2 * err;

        if (e2 > -dy) {
            err -= dy;
            x0 += sx;
        }
        
        if (e2 < dx) {
            err += dx;
            y0 += sy;
        }
    }
}

static inline void Window_Draw_Line_RGBA(Window* win, vec2 start, vec2 end, RGBA color) {
    Window_Draw_Line(win, start, end, rgba_to_base10(color));
}

static inline void Window_Draw_Strict_Rect(Window* win, vec2 pos, vec2 size, uint color) {
    if (!win || !win->pixels) return;

    int x0 = (int)pos.x;
    int y0 = (int)pos.y;
    int x1 = (int)(pos.x + size.x);
    int y1 = (int)(pos.y + size.y);

    for (int y = y0; y < y1; y++) {
        for (int x = x0; x < x1; x++) {
            vec2 p = { (float)x, (float)y };
            Window_Draw_Pixel(win, p, color);
        }
    }
}

static inline void Window_Draw_Strict_Rect_RGBA(Window* win, vec2 pos, vec2 size, RGBA color) {
    Window_Draw_Strict_Rect(win, pos, size, rgba_to_base10(color));
}

static inline void Window_Draw_Rect(Window* win, Rect rect, uint color) {
    if (!win || !win->pixels) return;
    if (rect.size.x <= 0 || rect.size.y <= 0) return;

    const double cos_angle = cos((double)rect.rotation);
    const double sin_angle = sin((double)rect.rotation);
    const double size_x = (double)rect.size.x;
    const double size_y = (double)rect.size.y;
    const double pivot_x = (double)rect.offset.x;
    const double pivot_y = (double)rect.offset.y;

    const double corner_x[] = {-pivot_x, size_x - pivot_x};
    const double corner_y[] = {-pivot_y, size_y - pivot_y};
    double min_x = DBL_MAX, min_y = DBL_MAX;
    double max_x = -DBL_MAX, max_y = -DBL_MAX;

    for (int y = 0; y < 2; y++) {
        for (int x = 0; x < 2; x++) {
            const double scaled_x = corner_x[x];
            const double scaled_y = corner_y[y];
            const double rotated_x = scaled_x * cos_angle - scaled_y * sin_angle;
            const double rotated_y = scaled_x * sin_angle + scaled_y * cos_angle;
            if (rotated_x < min_x) min_x = rotated_x;
            if (rotated_x > max_x) max_x = rotated_x;
            if (rotated_y < min_y) min_y = rotated_y;
            if (rotated_y > max_y) max_y = rotated_y;
        }
    }

    const double center_x = (double)rect.pos.x + (double)rect.offset.x;
    const double center_y = (double)rect.pos.y + (double)rect.offset.y;
    const double left = center_x + min_x;
    const double top = center_y + min_y;
    const double right = center_x + max_x;
    const double bottom = center_y + max_y;
    const int start_x = left <= 0.0 ? 0 : (int)floor(left);
    const int start_y = top <= 0.0 ? 0 : (int)floor(top);
    const int end_x = right >= (double)win->size.x ? (int)ceil(right) : (int)ceil(right);
    const int end_y = bottom >= (double)win->size.y ? (int)ceil(bottom) : (int)ceil(bottom);
    if (right <= 0.0 || bottom <= 0.0 ||
        left >= (double)win->size.x || top >= (double)win->size.y) return;

    for (int screen_y = start_y; screen_y < end_y; screen_y++) {
        for (int screen_x = start_x; screen_x < end_x; screen_x++) {
            const double relative_x = (double)screen_x + 0.5 - center_x;
            const double relative_y = (double)screen_y + 0.5 - center_y;
            const double source_scaled_x = relative_x * cos_angle + relative_y * sin_angle;
            const double source_scaled_y = -relative_x * sin_angle + relative_y * cos_angle;
            const double source_x = source_scaled_x + pivot_x;
            const double source_y = source_scaled_y + pivot_y;

            vec2 result = (vec2){screen_x, screen_y};
            result = vec2_sub(result, rect.offset);

            if (source_x < 0.0 || source_x >= (double)rect.size.x ||
                source_y < 0.0 || source_y >= (double)rect.size.y) continue;

            Window_Draw_Pixel(win, result, color);
        }
    }
}

static inline void Window_Draw_Rect_RGBA(Window* win, Rect rect, RGBA color) {
    Window_Draw_Rect(win, rect, rgba_to_base10(color));
}

static inline void Window_Draw_Image(Window* win, Image img, vec2 pos) {
    if (!win || !win->pixels || !img.pixels || img.width == 0 || img.height == 0) return;
    if (img.scale.x == 0.0f || img.scale.y == 0.0f) return;
    if (!isfinite(pos.x) || !isfinite(pos.y) ||
        !isfinite(img.offset.x) || !isfinite(img.offset.y) ||
        !isfinite(img.scale.x) || !isfinite(img.scale.y) ||
        !isfinite(img.rotation)) return;

    const int window_width = (int)win->size.x;
    const int window_height = (int)win->size.y;
    if (window_width <= 0 || window_height <= 0) return;

    const double cos_angle = cos((double)img.rotation);
    const double sin_angle = sin((double)img.rotation);
    const double scale_x = (double)img.scale.x;
    const double scale_y = (double)img.scale.y;
    const double pivot_x = (double)img.offset.x;
    const double pivot_y = (double)img.offset.y;

    const double corner_x[] = {-pivot_x, (double)img.width - pivot_x};
    const double corner_y[] = {-pivot_y, (double)img.height - pivot_y};
    double min_x = DBL_MAX, min_y = DBL_MAX;
    double max_x = -DBL_MAX, max_y = -DBL_MAX;

    for (int y = 0; y < 2; y++) {
        for (int x = 0; x < 2; x++) {
            const double scaled_x = corner_x[x] * scale_x;
            const double scaled_y = corner_y[y] * scale_y;
            const double rotated_x = scaled_x * cos_angle - scaled_y * sin_angle;
            const double rotated_y = scaled_x * sin_angle + scaled_y * cos_angle;
            if (rotated_x < min_x) min_x = rotated_x;
            if (rotated_x > max_x) max_x = rotated_x;
            if (rotated_y < min_y) min_y = rotated_y;
            if (rotated_y > max_y) max_y = rotated_y;
        }
    }

    const double center_x = (double)pos.x + (double)img.offset.x;
    const double center_y = (double)pos.y + (double)img.offset.y;
    const double left = center_x + min_x;
    const double top = center_y + min_y;
    const double right = center_x + max_x;
    const double bottom = center_y + max_y;
    if (right <= 0.0 || bottom <= 0.0 ||
        left >= (double)window_width || top >= (double)window_height) return;

    const int start_x = left <= 0.0 ? 0 : (int)floor(left);
    const int start_y = top <= 0.0 ? 0 : (int)floor(top);
    const int end_x = right >= (double)window_width ? window_width : (int)ceil(right);
    const int end_y = bottom >= (double)window_height ? window_height : (int)ceil(bottom);
    const double inverse_scale_x = 1.0 / scale_x;
    const double inverse_scale_y = 1.0 / scale_y;

    for (int screen_y = start_y; screen_y < end_y; screen_y++) {
        for (int screen_x = start_x; screen_x < end_x; screen_x++) {
            const double relative_x = (double)screen_x + 0.5 - center_x;
            const double relative_y = (double)screen_y + 0.5 - center_y;
            const double source_scaled_x = relative_x * cos_angle + relative_y * sin_angle;
            const double source_scaled_y = -relative_x * sin_angle + relative_y * cos_angle;
            const double source_x = source_scaled_x * inverse_scale_x + pivot_x;
            const double source_y = source_scaled_y * inverse_scale_y + pivot_y;

            if (source_x < 0.0 || source_x >= (double)img.width ||
                source_y < 0.0 || source_y >= (double)img.height) continue;

            const int image_x = (int)floor(source_x);
            const int image_y = (int)floor(source_y);

            const uint image_index = (uint)image_y * img.width + (uint)image_x;
            
            Window_Draw_Pixel(win, (vec2){screen_x, screen_y}, img.pixels[image_index]);
        }
    }
}

static inline bool Window_Key_Down(Window* win, char keycode) {
    if (!win || keycode < 0 || keycode >= 256) return 0;
    return win->keys[keycode] == true;
}

static inline bool Window_Any_Key_Down(Window* win) {
    if (!win) return 0;
    for (uint i = 0; i < 256; i++) {
        if (win->keys[i]) return true;
    }
    return false;
}

static inline vec2 Window_Get_Mouse_Pos(Window* win) {
    if (!win) return (vec2){0, 0};
    return win->mouse_pos;
}

static inline bool Window_Mouse_Down(Window* win, int button) {
    if (!win || button < 0 || button > 4) return false;
    return win->mouse_buttons[button] == true;
}

static inline bool Window_Any_Mouse_Down(Window* win) {
    if (!win) return false;
    for (uint i = 0; i < 5; i++) {
        if (win->mouse_buttons[i]) return true;
    }
    return false;
}

static inline void Window_Hook_Destroy(Window* win, void (*callback)(Window* win)) {
    if (!win) return;
    if (win->destroy_hook_count >= 256) return;
    win->destroy_hooks[win->destroy_hook_count++] = callback;
}

static inline void Window_Hook_Resize(Window* win, void (*callback)(Window* win)) {
    if (!win) return;
    if (win->resize_hook_count >= 256) return;
    win->resize_hooks[win->resize_hook_count++] = callback;
}

static inline void Window_Hook_Update(Window* win, void (*callback)(Window* win)) {
    if (!win) return;
    if (win->update_hook_count >= 256) return;
    win->update_hooks[win->update_hook_count++] = callback;
}

static inline void Window_Hook_Render(Window* win, void (*callback)(Window* win)) {
    if (!win) return;
    if (win->render_hook_count >= 256) return;
    win->render_hooks[win->render_hook_count++] = callback;
}

static inline void Window_Hook_KeyDown(Window* win, void (*callback)(Window* win, int keycode)) {
    if (!win) return;
    if (win->keydown_hook_count >= 256) return;
    win->keydown_hooks[win->keydown_hook_count++] = callback;
}

static inline void Window_Hook_KeyUp(Window* win, void (*callback)(Window* win, int keycode)) {
    if (!win) return;
    if (win->keyup_hook_count >= 256) return;
    win->keyup_hooks[win->keyup_hook_count++] = callback;
}

static inline void Window_Hook_MouseMove(Window* win, void (*callback)(Window* win, int x, int y)) {
    if (!win) return;
    if (win->mousemove_hook_count >= 256) return;
    win->mousemove_hooks[win->mousemove_hook_count++] = callback;
}

static inline void Window_Hook_MouseDown(Window* win, void (*callback)(Window* win, int button)) {
    if (!win) return;
    if (win->mousedown_hook_count >= 256) return;
    win->mousedown_hooks[win->mousedown_hook_count++] = callback;
}

static inline void Window_Hook_MouseUp(Window* win, void (*callback)(Window* win, int button)) {
    if (!win) return;
    if (win->mouseup_hook_count >= 256) return;
    win->mouseup_hooks[win->mouseup_hook_count++] = callback;
}

static inline void Window_PopHook_Destroy(Window* win) {
    if (!win || win->destroy_hook_count == 0) return;
    win->destroy_hooks[--win->destroy_hook_count] = NULL;
}

static inline void Window_PopHook_Resize(Window* win) {
    if (!win || win->resize_hook_count == 0) return;
    win->resize_hooks[--win->resize_hook_count] = NULL;
}

static inline void Window_PopHook_Update(Window* win) {
    if (!win || win->update_hook_count == 0) return;
    win->update_hooks[--win->update_hook_count] = NULL;
}

static inline void Window_PopHook_Render(Window* win) {
    if (!win || win->render_hook_count == 0) return;
    win->render_hooks[--win->render_hook_count] = NULL;
}

static inline void Window_PopHook_KeyDown(Window* win) {
    if (!win || win->keydown_hook_count == 0) return;
    win->keydown_hooks[--win->keydown_hook_count] = NULL;
}

static inline void Window_PopHook_KeyUp(Window* win) {
    if (!win || win->keyup_hook_count == 0) return;
    win->keyup_hooks[--win->keyup_hook_count] = NULL;
}

static inline void Window_PopHook_MouseMove(Window* win) {
    if (!win || win->mousemove_hook_count == 0) return;
    win->mousemove_hooks[--win->mousemove_hook_count] = NULL;
}

static inline void Window_PopHook_MouseDown(Window* win) {
    if (!win || win->mousedown_hook_count == 0) return;
    win->mousedown_hooks[--win->mousedown_hook_count] = NULL;
}

static inline void Window_PopHook_MouseUp(Window* win) {
    if (!win || win->mouseup_hook_count == 0) return;
    win->mouseup_hooks[--win->mouseup_hook_count] = NULL;
}

#endif