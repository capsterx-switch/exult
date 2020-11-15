#pragma once
#include <SDL.h>
#include <map>

class SwitchKeys
{
public:
   void LoadFromFileInternal(const char *filename);
   void ParseLine(const char * line);
   void key_event(size_t, bool press);
private:
   void send_event(size_t, bool press);
   std::map<size_t, SDL_Keysym> key_map_;
   size_t current_keys_;
};
