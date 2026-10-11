#include "../../../HBWM.h"
#include <stdio.h>

static Image nut;
static Image hook;

void render(Window* win) {
    Window_Clear(win, 0xFF000000);
    nut.rotation += deg_to_rad(1);
    if (nut.rotation >= TAU) nut.rotation -= TAU;
    Window_Draw_Image(win, nut, (vec2){750, 750});
    Window_Draw_Image(win, hook, (vec2){0, 0});
}

int main(void) {
    Window win;

    const int err = Window_Init(&win, 600, 600);
    if (err > 0) {
        printf("%s", Get_Window_Error_Msg(err));
        return err;
    }

    nut = Image_Load_Png("assets/nut.png");
    nut.offset = (vec2){(float)nut.width/2, (float)nut.height/2};
    hook = Image_Load_Png("assets/hook.png");

    Window_Hook_Render(&win, render);

    while (Window_Is_Open(&win));

    Window_Free(&win);
    Image_Free(&hook);
    Image_Free(&nut);

    return 0;
}