#include <lvgl.h>
#include <zmk/display.h>
#include <zmk/event_manager.h>

#include <zmk/events/battery_state_changed.h>
#include <zmk/events/split_peripheral_status_changed.h>

static lv_obj_t *dongle_label;
static lv_obj_t *left_label;
static lv_obj_t *right_label;

static uint8_t dongle_battery = 0;
static uint8_t left_battery = 0;
static uint8_t right_battery = 0;

static void update_labels() {
    char text[32];

    snprintf(text, sizeof(text), "D:%d%%", dongle_battery);
    lv_label_set_text(dongle_label, text);

    snprintf(text, sizeof(text), "L:%d%%", left_battery);
    lv_label_set_text(left_label, text);

    snprintf(text, sizeof(text), "R:%d%%", right_battery);
    lv_label_set_text(right_label, text);
}

static int battery_listener(const zmk_event_t *eh) {

    if (is_zmk_event(eh, battery_state_changed)) {

        const struct zmk_battery_state_changed *ev =
            as_zmk_battery_state_changed(eh);

        dongle_battery = ev->state_of_charge;
        update_labels();
    }

    if (is_zmk_event(eh, split_peripheral_status_changed)) {

        const struct zmk_split_peripheral_status_changed *ev =
            as_zmk_split_peripheral_status_changed(eh);

        if (ev->peripheral == 0) {
            left_battery = ev->battery_level;
        } else if (ev->peripheral == 1) {
            right_battery = ev->battery_level;
        }

        update_labels();
    }

    return ZMK_EV_EVENT_BUBBLE;
}

ZMK_LISTENER(battery_widget, battery_listener);

ZMK_SUBSCRIPTION(battery_widget, battery_state_changed);
ZMK_SUBSCRIPTION(battery_widget, split_peripheral_status_changed);

lv_obj_t *battery_widget_create(lv_obj_t *parent) {

    lv_obj_t *container = lv_obj_create(parent);
    lv_obj_set_size(container, LV_SIZE_CONTENT, LV_SIZE_CONTENT);

    lv_obj_set_flex_flow(container, LV_FLEX_FLOW_COLUMN);

    dongle_label = lv_label_create(container);
    left_label = lv_label_create(container);
    right_label = lv_label_create(container);

    update_labels();

    return container;
}