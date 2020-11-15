#pragma once

#include <SDL2/SDL_events.h>

class Game_window;

bool convert_touch_to_mouse(Game_window *, SDL_Event &);
