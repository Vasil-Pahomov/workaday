#include "common.h"
#include <pebble-events/pebble-events.h>
#ifndef PBL_PLATFORM_APLITE
//#include <@smallstoneapps/sliding-text-layer/sliding-text-layer.h>
#endif
#include <lazy-fonts/lazy-fonts.h>
#include "colors.h"
#include "date-layer.h"

typedef struct __attribute__((packed)) {
    char buf_date[16];
    TextLayer *text_layer;
    EventHandle tick_timer_event_handle;
    EventHandle settings_event_handle;
} Data;

static void tick_handler(struct tm *tick_time, TimeUnits units_changed, void *context) {
    log_func();

    Data *data = layer_get_data(context);
    const char *format = enamel_get_DATE_FORMAT();
    if (strlen(format) == 0) {
				char * buf = data->buf_date;
				switch (tick_time->tm_wday) {
					case 0:
						strcpy(buf,"ВС "); 
						break;
					case 1:
						strcpy(buf,"ПН "); 
						break;
					case 2:
						strcpy(buf,"ВТ "); 
						break;
					case 3:
						strcpy(buf,"СР "); 
						break;
					case 4:
						strcpy(buf,"ЧТ "); 
						break;
					case 5:
						strcpy(buf,"ПТ "); 
						break;
					case 6:
						strcpy(buf,"СБ "); 
						break;
				}
				buf+=5;
				snprintf(buf, 3, "%d", tick_time->tm_mday);
				while(buf[0] != 0) {
					buf++;
				}

				switch (tick_time->tm_mon) {
					case 0:
						strcpy(buf," ЯНВ"); 
						break;
					case 1:
						strcpy(buf," ФЕВ"); 
						break;
					case 2:
						strcpy(buf," МАР"); 
						break;
					case 3:
						strcpy(buf," АПР"); 
						break;
					case 4:
						strcpy(buf," МАЙ"); 
						break;
					case 5:
						strcpy(buf," ИЮН"); 
						break;
					case 6:
						strcpy(buf," ИЮЛ"); 
						break;
					case 7:
						strcpy(buf," АВГ"); 
						break;
					case 8:
						strcpy(buf," СЕН"); 
						break;
					case 9:
						strcpy(buf," ОКТ"); 
						break;
					case 10:
						strcpy(buf," НОЯ"); 
						break;
					case 11:
						strcpy(buf," ДЕК"); 
						break;
				}
    } else {
        strftime(data->buf_date, sizeof(data->buf_date), format, tick_time);
    }
    layer_mark_dirty(context);
}

static void settings_handler(void *context) {
    log_func();
    Data *data = layer_get_data(context);
    text_layer_set_text_color(data->text_layer, colors_get_foreground_color());
    time_t now = time(NULL);
}

static void text_update_proc(Layer *this, GContext *ctx) {
    log_func();
	
    GRect bounds = layer_get_bounds(this);

    graphics_context_set_stroke_color(ctx, colors_get_foreground_color());
    graphics_draw_line(ctx, GPoint(0, bounds.size.h), GPoint(bounds.size.w, 0));
	
}

static void this_update_proc(Layer *this, GContext *ctx) {
    log_func();
	
    GRect bounds = layer_get_bounds(this);

    graphics_context_set_stroke_color(ctx, colors_get_foreground_color());
    graphics_draw_line(ctx, GPoint(0, 0), GPoint(bounds.size.w, bounds.size.h));
	
}

DateLayer *date_layer_create(GRect frame) {
    log_func();
    DateLayer *this = layer_create_with_data(frame, sizeof(Data));
    Data *data = layer_get_data(this);
    GRect bounds = layer_get_bounds(this);
	int voffset = (DATE_LAYER_HEIGHT-20)/2;
    data->text_layer = text_layer_create(GRect(0,voffset,bounds.size.w, bounds.size.h-voffset));
    text_layer_set_font(data->text_layer, lazy_fonts_get(RESOURCE_ID_GILROY_LIGHT_28)); 
    text_layer_set_text(data->text_layer, data->buf_date);
    text_layer_set_text_alignment(data->text_layer, GTextAlignmentCenter);
    text_layer_set_background_color(data->text_layer, GColorClear);
    layer_add_child(this, text_layer_get_layer(data->text_layer));
    //layer_set_update_proc(text_layer_get_layer(data->text_layer), text_update_proc);
    //layer_set_update_proc(this, this_update_proc);

//#endif

    time_t now = time(NULL);
    tick_handler(localtime(&now), DAY_UNIT, this);
    data->tick_timer_event_handle = events_tick_timer_service_subscribe_context(DAY_UNIT, tick_handler, this);

#ifndef PBL_PLATFORM_APLITE
//    data->tap_event_handle = events_accel_tap_service_subscribe_context(tap_handler, this);
#endif

    settings_handler(this);
    data->settings_event_handle = enamel_settings_received_subscribe(settings_handler, this);

    return this;
}

void date_layer_destroy(DateLayer *this) {
    log_func();
    Data *data = layer_get_data(this);
    enamel_settings_received_unsubscribe(data->settings_event_handle);
#ifndef PBL_PLATFORM_APLITE
//    events_accel_tap_service_unsubscribe(data->tap_event_handle);
#endif
    events_tick_timer_service_unsubscribe(data->tick_timer_event_handle);
//#ifndef PBL_PLATFORM_APLITE
//    sliding_text_layer_destroy(data->text_layer);
//#else
    text_layer_destroy(data->text_layer);
//#endif
    layer_destroy(this);
}
