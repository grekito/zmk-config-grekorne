#include "bt_status.h"

#include <stdio.h>

#include <zmk/events/ble_active_profile_changed.h>

static lv_obj_t *bt_label;

static void update_profile(uint8_t profile) {

    char text[16];

    snprintf(text, sizeof(text), "Profile: %d", profile + 1);
    lv_label_set_text(bt_label, text);
}

static int bt_listener(const zmk_event_t *eh) {

    if (as_zmk_ble_active_profile_changed(eh) != NULL) {

        const struct zmk_ble_active_profile_changed *ev =
            as_zmk_ble_active_profile_changed(eh);

        update_profile(ev->index);
    }

    return ZMK_EV_EVENT_BUBBLE;
}

ZMK_LISTENER(bt_status, bt_listener);

ZMK_SUBSCRIPTION(bt_status, zmk_ble_active_profile_changed);

lv_obj_t *bt_status_widget_create(lv_obj_t *parent) {

    bt_label = lv_label_create(parent);

    lv_obj_align(bt_label, LV_ALIGN_BOTTOM_RIGHT, 0, -10);

    update_profile(0);

    return bt_label;
}