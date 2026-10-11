# Hook-Based Window Module

_(This is a high level module, requiring `windows.h`, `math.h`, `stdbool.h`, and more...)_

## OS targets
|Windows|Mac|Linux|
|-------|---|-----|
|Yes    |No |No   |

## About
It's a window maker like SDL3, but it has hooks

## API
### defines
* `PI` : `3.141592653589793`
* `TAU` : `PI*2`
* `PHI` : `1.618033988749895`
* `EU_NUMBR` : `2.718281828459045`
* `EU_CONST` : `0.577215664901532`

### types
* `uint` : `unsigned int`
* `uchar` : `unsigned char`
* `ulong` : `unsigned long`
* `ull` : `unsigned long long`

### enums
* `InaccessableKeyCode`:
    ```
    IKC_LEFT = 0x25
    IKC_UP = 0x26
    IKC_RIGHT = 0x27
    IKC_DOWN = 0x28
    IKC_INSERT = 0x2D
    IKC_ESCAPE = 0x1B
    IKC_DELETE = 0x2E
    IKC_HOME = 0x24
    IKC_END = 0x23
    IKC_PRIOR = 0x21
    IKC_NEXT = 0x22
    IKC_SHIFT = 0x10
    IKC_LSHIFT = 0xA0
    IKC_RSHIFT = 0xA1
    IKC_CTRL = 0x11
    IKC_LCTRL = 0xA2
    IKC_RCTRL = 0xA3
    IKC_ALT = 0x12
    IKC_LALT = 0xA4
    IKC_RALT = 0xA5
    IKC_LHOME = 0x5B
    IKC_RHOME = 0x5C
    IKC_APPS = 0x5D
    IKC_F1 = 0x70
    IKC_F2 = 0x71
    IKC_F3 = 0x72
    IKC_F4 = 0x73
    IKC_F5 = 0x74
    IKC_F6 = 0x75
    IKC_F7 = 0x76
    IKC_F8 = 0x77
    IKC_F9 = 0x78
    IKC_F10 = 0x79
    IKC_F11 = 0x7A
    IKC_F12 = 0x7B
    IKC_CAPS = 0x14
    IKC_NUMLOCK = 0x90
    IKC_SCROLL = 0x91
    IKC_PRINT = 0x2C
    IKC_PAUSE = 0x13
    IKC_VMUTE = 0xAD
    IKC_VDOWN = 0xAE
    IKC_VUP = 0xAF
    IKC_MNEXT = 0xB0
    IKC_MPREV = 0xB1
    IKC_MSTOP = 0xB2
    IKC_MTOGGLE = 0xB3
    ```
* `MouseButton`:
    ```
    MOUS_LEFT
    MOUS_MIDD
    MOUS_RIGH
    MOUS_X1
    MOUS_X2
    ```
* `WindowErrorReturnCode` :
    ```
    NONE
    BAD_WINDOW
    BAD_PIXELS
    BAD_WIN_HANDLE
    ```

### structs
`vec2`:
* `x` : `float`
* `y` : `float`

`RGBA`:
* `r` : `uchar`
* `g` : `uchar`
* `b` : `uchar`
* `a` : `uchar`

`String`:
* `text` : `const char*`

`DblStr`:
* `items` : `String[2]`

`Image`:
* `width` : `uint` _(if you replace data in this, the image might corrupt)_
* `height` : `uint` _(if you replace data in this, the image might corrupt)_
* `pixels` : `uint*` _(if you replace data in this, the image might corrupt)_
* `offset` : `vec2`
* `scale` : `vec2`
* `rotation` : `float`

`Rect`:
* `pos` : `vec2`
* `size` : `vec2`
* `rotation` : `float`
* `offset` : `vec2` _image-space rotation pivot in pixels_

`Window`:
* `hwnd` : `HWND` _from windows.h_
* `size` : `vec2`
* `pixels` : `uint*`
* `keys` : `bool[256]`
* `mouse_pos` : `vec2`
* `mouse_buttons` : `bool[5]`
* `destroy_hook_count` : `uint` _(these aren't meant to be changable)_
* `resize_hook_count` : `uint` _(these aren't meant to be changable)_
* `update_hook_count` : `uint` _(these aren't meant to be changable)_
* `render_hook_count` : `uint` _(these aren't meant to be changable)_
* `keydown_hook_count` : `uint` _(these aren't meant to be changable)_
* `keyup_hook_count` : `uint` _(these aren't meant to be changable)_
* `mousemove_hook_count` : `uint` _(these aren't meant to be changable)_
* `mousedown_hook_count` : `uint` _(these aren't meant to be changable)_
* `mouseup_hook_count` : `uint` _(these aren't meant to be changable)_
* `resize_hooks` : `void(*[256])(Window* win)` _(these aren't meant to be changable)_
* `destroy_hooks` : `void(*[256])(Window* win)` _(these aren't meant to be changable)_
* `render_hooks` : `void(*[256])(Window* win)` _(these aren't meant to be changable)_
* `resize_hooks` : `void(*[256])(Window* win)` _(these aren't meant to be changable)_
* `update_hooks` : `void(*[256])(Window* win)` _(these aren't meant to be changable)_
* `keydown_hooks` : `void(*[256])(Window* win, int keycode)` _(these aren't meant to be changable)_
* `keyup_hooks` : `void(*[256])(Window* win, int keycode)` _(these aren't meant to be changable)_
* `mousemove_hooks` : `void(*[256])(Window* win, int x, int y)` _(these aren't meant to be changable)_
* `mousedown_hooks` : `void(*[256])(Window* win, int button)` _(these aren't meant to be changable)_
* `mouseup_hooks` : `void(*[256])(Window* win, int button)` _(these aren't meant to be changable)_
### functions
#### math
* `static inline float clamp(float x, float min_val, float max_val)`
* `static inline float deg_to_rad(float degrees)`
* `static inline float rad_to_deg(float radians)`
* `static inline vec2 vec2_make(float x, float y)`
* `static inline vec2 vec2_add(vec2 a, vec2 b)`
* `static inline vec2 vec2_sub(vec2 a, vec2 b)`
* `static inline vec2 vec2_mul(vec2 a, vec2 b)`
* `static inline vec2 vec2_div(vec2 a, vec2 b)`
* `static inline float vec2_len_squ(vec2 v)`
* `static inline float vec2_len(vec2 v)`
* `static inline vec2 vec2_norm(vec2 v)`
* `static inline vec2 vec2_neg(vec2 v)`
* `static inline RGBA rgba(uchar r, uchar g, uchar b, uchar a)`
* `static inline RGBA rgb(uchar r, uchar g, uchar b)`
* `static inline uint rgba_to_base10(RGBA c)`
* `static inline RGBA rgba_blend(RGBA base, RGBA c)`
#### image
* `Image Image_Load_Png(const char* filename)`
* `void Image_Free(Image* img)`
#### window
* `static inline int Window_Init(Window* win, uint width, uint height)`
* `static inline const char* Get_Window_Error_Msg(int code)`
* `static inline void Window_Free(Window* win)`
* `static inline void Window_Close(Window* win)`
* `static inline int Window_Is_Open(Window* win)`
* `static inline void Window_Clear(Window* win, uint color)`
* `static inline void Window_Clear_RGBA(Window* win, RGBA color)`
* `static inline void Window_Draw_Pixel_Index(Window* win, int index, uint color)`
* `static inline void Window_Draw_Pixel_Index_RGBA(Window* win, int index, RGBA color)`
* `static inline void Window_Draw_Pixel(Window* win, vec2 pos, uint color)`
* `static inline void Window_Draw_Pixel_RGBA(Window* win, vec2 pos, RGBA color)`
* `static inline void Window_Draw_Line(Window* win, vec2 start, vec2 end, uint color)`
* `static inline void Window_Draw_Line_RGBA(Window* win, vec2 start, vec2 end, RGBA color)`
* `static inline void Window_Draw_Strict_Rect(Window* win, vec2 pos, vec2 size, uint color)`
* `static inline void Window_Draw_Strict_Rect_RGBA(Window* win, vec2 pos, vec2 size, RGBA color)`
* `static inline void Window_Draw_Strict_Rect_RGBA(Window* win, vec2 pos, vec2 size, RGBA color)`
* `static inline void Window_Draw_Rect_RGBA(Window* win, Rect rect, RGBA color)`
* `static inline void Window_Draw_Image(Window* win, Image img, vec2 pos)`
* `static inline bool Window_Key_Down(Window* win, char keycode)`
* `static inline bool Window_Any_Key_Down(Window* win)`
* `static inline vec2 Window_Get_Mouse_Pos(Window* win)`
* `static inline bool Window_Mouse_Down(Window* win, int button)`
* `static inline bool Window_Any_Mouse_Down(Window* win)`
#### hooking
* `static inline void Window_Hook_Destroy(Window* win, void (*callback)(Window* win))`
* `static inline void Window_Hook_Resize(Window* win, void (*callback)(Window* win))`
* `static inline void Window_Hook_Update(Window* win, void (*callback)(Window* win))`
* `static inline void Window_Hook_Render(Window* win, void (*callback)(Window* win))`
* `static inline void Window_Hook_KeyDown(Window* win, void (*callback)(Window* win, int keycode))`
* `static inline void Window_Hook_KeyUp(Window* win, void (*callback)(Window* win, int keycode))`
* `static inline void Window_Hook_MouseMove(Window* win, void (*callback)(Window* win, int x, int y))`
* `static inline void Window_Hook_MouseDown(Window* win, void (*callback)(Window* win, int button))`
* `static inline void Window_Hook_MouseUp(Window* win, void (*callback)(Window* win, int button))`
#### unhooking
* `static inline void Window_PopHook_Destroy(Window* win)`
* `static inline void Window_PopHook_Resize(Window* win)`
* `static inline void Window_PopHook_Update(Window* win)`
* `static inline void Window_PopHook_Render(Window* win)`
* `static inline void Window_PopHook_KeyDown(Window* win)`
* `static inline void Window_PopHook_KeyUp(Window* win)`
* `static inline void Window_PopHook_MouseMove(Window* win)`
* `static inline void Window_PopHook_MouseDown(Window* win)`
* `static inline void Window_PopHook_MouseUp(Window* win)`
#### strings
* `static inline const char* ccs_from_char(const char input)`
* `static inline uint String_Get_Length(const char* str)`
* `static inline uint String_Get_Length_String(String str)`
* `static inline String String_Join_String(String str1, String str2)`
* `static inline String String_Join(String str1, const char* str2)`
* `static inline bool String_Contains(const char* target, String src)`
* `static inline bool String_Contains_String(String target, String src)`
* `static inline String* String_Split(const char splitter, String src)`
* `static inline DblStr String_Split_From_Index_Point(const char target, uint offset, String src)`
* `static inline uint String_Get_List_Length(String* list)`
#### audio
* `static inline void Audio_Set_Volume(float volume)` _(This only sets the master volume, not individually)_
* `static inline void Audio_Play(const char* path, float volume, bool loop)`
* `static inline void Audio_Stop(void)` _(This stops all sounds, not individually)_