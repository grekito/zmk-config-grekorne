#include "battery_status.h"

#include <stdio.h>

#include <zmk/events/battery_state_changed.h>
#include <zmk/events/split_peripheral_status_changed.h>

static lv_obj_t *left_label;
static lv_obj_t *right_label;
static lv_obj_t *dongle_label;

static uint8_t left_level;
static uint8_t right_level;
static uint8_t dongle_level;

static void update_display() {

    char text[32];

    snprintf(text, sizeof(text), "LEFT  %d%%", left_level);
    lv_label_set_text(left_label, text);

    snprintf(text, sizeof(text), "RIGHT %d%%", right_level);
    lv_label_set_text(right_label, text);

    snprintf(text, sizeof(text), "DONGLE %d%%", dongle_level);
    lv_label_set_text(dongle_label, text);
}

static int battery_listener(const zmk_event_t *eh) {

    if (as_zmk_battery_state_changed(eh) != NULL) {

        const struct zmk_battery_state_changed *ev =
            as_zmk_battery_state_changed(eh);

        dongle_level = ev->state_of_charge;
        update_display();
    }

    if (as_zmk_split_peripheral_status_changed(eh) != NULL) {

        const struct zmk_split_peripheral_status_changed *ev =
            as_zmk_split_peripheral_status_changed(eh);

        if (ev->peripheral == 0)
            left_level = ev->battery_level;

        if (ev->peripheral == 1)
            right_level = ev->battery_level;

        update_display();
    }

    return ZMK_EV_EVENT_BUBBLE;
}

ZMK_LISTENER(battery_status, battery_listener);

ZMK_SUBSCRIPTION(battery_status, zmk_battery_state_changed);
ZMK_SUBSCRIPTION(battery_status, zmk_split_peripheral_status_changed);

lv_obj_t *battery_status_widget_create(lv_obj_t *parent) {

    lv_obj_t *container = lv_obj_create(parent);

    lv_obj_set_size(container, 128, 40);
    lv_obj_align(container, LV_ALIGN_TOP_LEFT, 0, 0);

    lv_obj_clear_flag(container, LV_OBJ_FLAG_SCROLLABLE);

    left_label = lv_label_create(container);
    right_label = lv_label_create(container);
    dongle_label = lv_label_create(container);

    lv_obj_align(left_label, LV_ALIGN_TOP_LEFT, 0, 0);
    lv_obj_align(right_label, LV_ALIGN_TOP_RIGHT, 0, 0);
    lv_obj_align(dongle_label, LV_ALIGN_TOP_MID, 0, 18);

    update_display();

    return container;
}