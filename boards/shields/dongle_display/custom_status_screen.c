#include "custom_status_screen.h"

#include <lvgl.h>

#include "widgets/battery_status.h"

void zmk_display_status_screen(void) {

    lv_obj_t *screen = lv_scr_act();

    lv_obj_set_flex_flow(screen, LV_FLEX_FLOW_COLUMN);
    lv_obj_set_flex_align(
        screen,
        LV_FLEX_ALIGN_START,
        LV_FLEX_ALIGN_CENTER,
        LV_FLEX_ALIGN_CENTER
    );

    battery_status_widget_create(screen);
}