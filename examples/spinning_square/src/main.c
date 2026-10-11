#include "../../../HBWM.h"
#include <stdio.h>

static Rect rect = {
    .pos = {300, 300},
    .size = {100, 100},
    .rotation = 0,
    .offset = {50, 50}
};

void render(Window* win) {
    Window_Clear(win, 0xFF000000);
    rect.rotation += deg_to_rad(1);
    if (rect.rotation >= TAU) rect.rotation -= TAU;
    Window_Draw_Rect(win, rect, 0xFFFF0000);
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