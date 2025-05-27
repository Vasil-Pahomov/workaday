#pragma once
#include <pebble.h>

typedef Layer DateLayer;
#define DATE_LAYER_HEIGHT 42


DateLayer *date_layer_create(GRect frame);
void date_layer_destroy(DateLayer *this);
