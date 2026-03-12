#include "layer_status.h"

#include <stdio.h>

#include <zmk/events/layer_state_changed.h>

static lv_obj_t *layer_label;

static void update_layer(uint8_t layer) {

    char text[16];
    snprintf(text, sizeof(text), "Layer: %d", layer);

    lv_label_set_text(layer_label, text);
}

static int layer_listener(const zmk_event_t *eh) {

    if (as_zmk_layer_state_changed(eh) != NULL) {

        const struct zmk_layer_state_changed *ev =
            as_zmk_layer_state_changed(eh);

        if (ev->state)
            update_layer(ev->layer);
    }

    return ZMK_EV_EVENT_BUBBLE;
}

ZMK_LISTENER(layer_status, layer_listener);

ZMK_SUBSCRIPTION(layer_status, zmk_layer_state_changed);

lv_obj_t *layer_status_widget_create(lv_obj_t *parent) {

    layer_label = lv_label_create(parent);

    lv_obj_align(layer_label, LV_ALIGN_BOTTOM_LEFT, 0, -10);

    update_layer(0);

    return layer_label;
}