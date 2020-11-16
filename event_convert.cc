#include "event_convert.h"
#include "gamewin.h"
#include <SDL.h>
#include <switch_keys.h>
#include "exult.h"
#include "switch/rstick_to_mouse.hpp"
#include "switch/touch_to_mouse.hpp"
#include "switch/keymap.hpp"

bool convert_touch_to_mouse (Game_window * gwin, SDL_Event & event)
{
  bool ret = nswitch::touch_to_mouse(gwin->get_win()->get_display_width(),
		  gwin->get_win()->get_display_height(),
		  event);
  if (!ret)
    ret = nswitch::rstick_to_mouse(event);;
  if (!ret && switchkeys)
  {
    ret = switchkeys->event(event);
  }
  return ret;
}
