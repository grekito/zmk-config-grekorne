#include "custom_status_screen.h"

#include <lvgl.h>

#include "widgets/battery_status.h"
#include "widgets/layer_status.h"
#include "widgets/bt_status.h"

void zmk_display_status_screen(void) {

    lv_obj_t *screen = lv_scr_act();

    battery_status_widget_create(screen);
    layer_status_widget_create(screen);
    bt_status_widget_create(screen);
}