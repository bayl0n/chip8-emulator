#ifndef GRAPHICS_H
#define GRAPHICS_H

#include <stdbool.h>

int init_display();
void draw(unsigned char* display);
void sdl_ehandler(unsigned char* keypad);
bool should_quit();
void stop_display();

#endif
