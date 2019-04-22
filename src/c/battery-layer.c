#include "common.h"
#include <pebble-events/pebble-events.h>
#include <pdc-transform/pdc-transform.h>
#include <lazy-fonts/lazy-fonts.h>
#include "colors.h"
#include "battery-layer.h"

static const GPathInfo PATH_INFO = {
    .num_points = 8,
    .points = (GPoint []) {
		{0, BAT_T}, 
		{BAT_W-1, BAT_T}, 
		{BAT_W-1, BAT_T+(BAT_H+BAT_G)/2}, 
		{BAT_W,   BAT_T+(BAT_H+BAT_G)/2}, 
		{BAT_W,   BAT_T+(BAT_H-BAT_G)/2}, 
		{BAT_W-1, BAT_T+(BAT_H-BAT_G)/2}, 
		{BAT_W-1, BAT_T+BAT_H}, 
		{0, BAT_T+BAT_H} 
		} 
};

typedef struct __attribute__((packed)) {
    BatteryChargeState state;
    EventHandle battery_state_event_handle;
    EventHandle settings_event_handle;
    GPath *gpath;
} Data;

static void update_proc(Layer *this, GContext *ctx) {
    log_func();
    GRect bounds = layer_get_bounds(this);
    Data *data = layer_get_data(this);

    graphics_context_set_stroke_color(ctx, colors_get_foreground_color());
    gpath_draw_outline(ctx, data->gpath);

    if (data->state.is_charging) {
        //gdraw_command_image_draw(ctx, data->battery_charging_pdc, offset);
    } else {
        
				uint8_t percent = data->state.charge_percent > 0 ? data->state.charge_percent : 5;
        GColor color = colors_get_foreground_color();
				
        graphics_context_set_fill_color(ctx, color);

        int w = (BAT_W-4) * percent / 100;
        graphics_fill_rect(ctx, GRect(2, BAT_T+2, w, BAT_H-3), 0, GCornerNone);
    }
}

static void battery_state_handler(BatteryChargeState state, void *this) {
    log_func();
    Data *data = layer_get_data(this);
    memcpy(&data->state, &state, sizeof(BatteryChargeState));
    layer_mark_dirty(this);
}

static void settings_handler(void *this) {
    log_func();
    Data *data = layer_get_data(this);    
    layer_mark_dirty(this);
}

BatteryLayer *battery_layer_create(GRect frame) {
    log_func();
    BatteryLayer *this = layer_create_with_data(frame, sizeof(Data));
    layer_set_update_proc(this, update_proc);
    Data *data = layer_get_data(this);
    GRect bounds = layer_get_bounds(this);

    data->gpath = gpath_create(&PATH_INFO);

    battery_state_handler(battery_state_service_peek(), this);
    data->battery_state_event_handle = events_battery_state_service_subscribe_context(battery_state_handler, this);

    settings_handler(this);
    data->settings_event_handle = enamel_settings_received_subscribe(settings_handler, this);

    return this;
}

void battery_layer_destroy(BatteryLayer *this) {
    log_func();
    Data *data = layer_get_data(this);
    enamel_settings_received_unsubscribe(data->settings_event_handle);
    events_battery_state_service_unsubscribe(data->battery_state_event_handle);
    //text_layer_destroy(data->text_layer);
    //gdraw_command_image_destroy(data->battery_pdc);
    //gdraw_command_image_destroy(data->battery_charging_pdc);
    gpath_destroy(data->gpath);
    layer_destroy(this);
}
