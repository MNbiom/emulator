#pragma once

#include <cstdint>

extern bool showDisplayWindow;
extern bool resetDisplayPos;
extern bool resetDisplaySize;
extern const uint16_t DISPLAY_MAX_SIZE;

void init_display();
void clear_display(uint32_t color = 0xFF000000);
void update_display();
void shutdown_display();
void draw_pixel(uint8_t x, uint8_t y, uint32_t color = 0xFFFFFFFF);

void do_display();