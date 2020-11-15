#include "event_convert.h"
#include "gamewin.h"
#include <SDL.h>
#include <switch_keys.h>
#include "exult.h"

typedef Uint32 SDL_MouseID;
extern "C" {
int SDL_SendMouseMotion	(SDL_Window *window, SDL_MouseID mouseID,
		int 	relative,
		int 	x,
		int 	y 
		);
int SDL_SendMouseButton	(SDL_Window * 	window,
		SDL_MouseID 	mouseID,
		Uint8 	state,
		Uint8 	button 
		);
}

bool convert_touch_to_mouse (Game_window *gwin, SDL_Event & event)
{
  if (event.type == SDL_FINGERDOWN || event.type == SDL_FINGERUP)
  {
     SDL_Finger* finger0 = SDL_GetTouchFinger(event.tfinger.touchId, 0);
     int numFingers = 0;
     if (finger0) {
     	numFingers = SDL_GetNumTouchFingers(event.tfinger.touchId);
     }
     printf("numfingers %d\n", numFingers);
     if (numFingers == 1) {
       int x = gwin->get_win()->get_display_width() * event.tfinger.x;
       int y = gwin->get_win()->get_display_height() * event.tfinger.y;
       SDL_SendMouseMotion(NULL, 0, 0, x, y);
       printf("Sending mition %d - %d\n", x, y);
       if(event.type == SDL_FINGERDOWN)
       {
         SDL_SendMouseButton(NULL, 0, SDL_PRESSED, SDL_BUTTON_LEFT);
	 printf("sending pressed\n");
       }
       return true;
     }
     if (event.type == SDL_FINGERUP)
     {
       SDL_SendMouseButton(NULL, 0, SDL_RELEASED, SDL_BUTTON_LEFT);
       printf("sending released\n");
       return true;
     }
  } else if (switchkeys != NULL && (event.type == SDL_JOYBUTTONDOWN || event.type == SDL_JOYBUTTONUP))
  {
    printf("Got joy button: %d\n", event.jbutton.button);
    switchkeys->key_event(event.jbutton.button, event.type == SDL_JOYBUTTONDOWN ? true : false);
    return true;
  }
  return false;
}
