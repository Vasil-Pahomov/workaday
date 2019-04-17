#pragma once
#include <pebble.h>

#define TOP_LAYER_HEIGHT PBL_IF_DISPLAY_LARGE_ELSE(38, 28)
#define CB_WIDTH 20 //width of connection and battery widgets


typedef Layer TopLayer;

TopLayer *top_layer_create(GRect frame);
void top_layer_destroy(TopLayer *this);
