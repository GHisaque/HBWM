#include "../../../HBWM.h"
#include <stdio.h>

void render(Window* win) {
    Window_Clear(win, 0xFF000000);

    Window_Draw_Line(win, (vec2){0, 0}, win->size, 0xFF0000FF);
    Window_Draw_Pixel(win, (vec2){300, 300}, 0xFFFF0000);
    Window_Draw_Pixel(win, vec2_div(win->size, (vec2){2, 2}), 0xFF00FF00);
}

int main(void) {
    Window win;

    const int err = Window_Init(&win, 600, 600);
    if (err > 0) {
        printf("%s", Get_Window_Error_Msg(err));
        return err;
    }

    Window_Hook_Render(&win, render);

    while (Window_Is_Open(&win));

    Window_Free(&win);

    return 0;
}