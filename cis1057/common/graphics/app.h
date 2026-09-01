#pragma once

#include "keys.h"

/* APPLICATION STATE */

// Opens a new application window with the given title
// and window size.
// Returns a reference to the frame buffer.
uint32_t *app_open(const char *title, int w, int h);

// Closes the open application window.
void app_close();

// Controls the application loop and limits the frame rate.
// Returns 1 while the application is still running, and 0 when it
// has terminated.
// If an fps_limit > 0 is provided, then the application will pause
// if necessary to maintain the limit.
int app_continue(int fps_limit);

/* KEYBOARD INPUT HANDLERS      */
/* Refer to keys.h for keycodes */

// Returns the time (in ms) that the key has been continuously pressed down
int key_down(int keycode);

// Returns the time (in ms) that the key was down when it is lifted
int key_up(int keycode);