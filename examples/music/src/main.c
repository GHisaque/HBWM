#include "../../../HBWM.h"
#include <stdio.h>

int main(void) {
    Window win;

    const int err = Window_Init(&win, 600, 600);
    if (err > 0) {
        printf("%s", Get_Window_Error_Msg(err));
        return err;
    }

    Audio_Set_Volume(1);
    Audio_Play("assets/chip.wav", true);

    while (Window_Is_Open(&win));

    Window_Free(&win);

    return 0;
}