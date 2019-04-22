#pragma once
#include <pebble.h>

typedef Layer BatteryLayer;

#define BAT_W 19 //battery symbol width
#define BAT_H 14 //battery symbol height
#define BAT_T 6  //battery symbol top offset
#define BAT_G 5  //battery grip size ("plus" contact)

BatteryLayer *battery_layer_create(GRect frame);
void battery_layer_destroy(BatteryLayer *this);
