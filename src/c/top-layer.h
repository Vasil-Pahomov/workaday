#pragma once
#include <pebble.h>

#define TOP_LAYER_HEIGHT 21
#define CB_WIDTH 27 //width of connection and battery widgets


typedef Layer TopLayer;

TopLayer *top_layer_create(GRect frame);
void top_layer_destroy(TopLayer *this);
