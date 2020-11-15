#include "switch_keys.h"
#include <string>
#include <SDL.h>
#include <vector>
#include <map>
#include <fstream>
#include <algorithm> 
#include <cctype>
#include <locale>

#include "files/utils.h"

const struct {
	const char *s;
	SDL_Keycode k;
} SDLKeyStringTable[] = {
	{"BACKSPACE", SDLK_BACKSPACE},
	{"TAB",       SDLK_TAB},
	{"ENTER",     SDLK_RETURN},
	{"PAUSE",     SDLK_PAUSE},
	{"ESC",       SDLK_ESCAPE},
	{"SPACE",     SDLK_SPACE},
	{"DEL",       SDLK_DELETE},
	{"KP0",       SDLK_KP_0},
	{"KP1",       SDLK_KP_1},
	{"KP2",       SDLK_KP_2},
	{"KP3",       SDLK_KP_3},
	{"KP4",       SDLK_KP_4},
	{"KP5",       SDLK_KP_5},
	{"KP6",       SDLK_KP_6},
	{"KP7",       SDLK_KP_7},
	{"KP8",       SDLK_KP_8},
	{"KP9",       SDLK_KP_9},
	{"KP0",       SDLK_KP_0},
	{"KP.",       SDLK_KP_PERIOD},
	{"KP/",       SDLK_KP_DIVIDE},
	{"KP*",       SDLK_KP_MULTIPLY},
	{"KP-",       SDLK_KP_MINUS},
	{"KP+",       SDLK_KP_PLUS},
	{"KP_ENTER",  SDLK_KP_ENTER},
	{"UP",        SDLK_UP},
	{"DOWN",      SDLK_DOWN},
	{"RIGHT",     SDLK_RIGHT},
	{"LEFT",      SDLK_LEFT},
	{"INSERT",    SDLK_INSERT},
	{"HOME",      SDLK_HOME},
	{"END",       SDLK_END},
	{"PAGEUP",    SDLK_PAGEUP},
	{"PAGEDOWN",  SDLK_PAGEDOWN},
	{"F1",        SDLK_F1},
	{"F2",        SDLK_F2},
	{"F3",        SDLK_F3},
	{"F4",        SDLK_F4},
	{"F5",        SDLK_F5},
	{"F6",        SDLK_F6},
	{"F7",        SDLK_F7},
	{"F8",        SDLK_F8},
	{"F9",        SDLK_F9},
	{"F10",       SDLK_F10},
	{"F11",       SDLK_F11},
	{"F12",       SDLK_F12},
	{"F13",       SDLK_F13},
	{"F14",       SDLK_F14},
	{"F15",       SDLK_F15},
	{"", SDLK_UNKNOWN} // terminator
};

auto Switch_Key_Map = std::vector<std::pair<std::string, int>>{
  {"A", 0},
  {"B", 1},
  {"X", 2},
  {"Y", 3},
  {"LSTICK", 4},
  {"RSTICK", 5},
  {"L", 6},
  {"R", 7},
  {"ZL", 8},
  {"ZR", 9},
  {"PLUS", 10},
  {"MINUS", 11},
  {"DLEFT", 12},
  {"DUP", 13},
  {"DRIGHT", 14},
  {"DDOWN", 15}};

static inline void ltrim(std::string &s) {
    s.erase(s.begin(), std::find_if(s.begin(), s.end(), [](unsigned char ch) {
        return !std::isspace(ch);
    }));
}

// trim from end (in place)
static inline void rtrim(std::string &s) {
    s.erase(std::find_if(s.rbegin(), s.rend(), [](unsigned char ch) {
        return !std::isspace(ch);
    }).base(), s.end());
}

// trim from both ends (in place)
static inline void trim(std::string &s) {
    ltrim(s);
    rtrim(s);
}

    

template<typename charType>
size_t splitWithBasicString(
    const std::basic_string<charType>& str,
    const charType delim,
    std::vector< std::basic_string<charType> > &tokens,
    const bool trimEmpty = false,
    const size_t maxTokens = (size_t)(-1))
{
    typedef std::basic_string<charType> my_string;
    typedef typename my_string::size_type my_size_type;
    tokens.clear();
    if (str.empty())
    {
        return 0;
    }
    my_size_type len = str.length();
    // Skip delimiters at beginning.
    my_size_type left = str.find_first_not_of(delim, 0);
    size_t i = 1;
    if (!trimEmpty && left != 0)
    {
        tokens.push_back(my_string());
        ++i;
    }
    while (i < maxTokens)
    {
        my_size_type right = str.find(delim, left);
        if (right == my_string::npos)
        {
            break;
        }
        if (!trimEmpty || right - left > 0)
        {
            tokens.push_back(str.substr(left, right - left));
            ++i;
        }
        left = right + 1;
    }
    if (left < len)
    {
        tokens.push_back(str.substr(left));
    }
    return tokens.size();
}

void SwitchKeys::LoadFromFileInternal(const char *filename) {
  std::ifstream keyfile;
  
  U7open(keyfile, filename, true);
  char temp[1024]; // 1024 should be long enough
  while (!keyfile.eof()) {
  	keyfile.getline(temp, 1024);
  	if (keyfile.gcount() >= 1023) {
  		std::cerr << "Keybinder: parse error: line too long. Skipping rest of file."
  		     << std::endl;
  		break;
  	}
  	ParseLine(temp);
  }
  keyfile.close();
}


void SwitchKeys::ParseLine(const char * line_)
{
  std::string line(line_);
  std::vector<std::string> args;
  size_t len = splitWithBasicString(
    line, '=', args);
  if (len != 2)
  {
    printf("Bad line: %s\n", line_);
    return;
  }


  std::vector<std::string> switch_keys;
  len = splitWithBasicString(args[0], '-', switch_keys);
  size_t switch_key=0;
  for (auto str : switch_keys)
  {
    trim(str);
    bool valid=false;
    for (auto && key_map : Switch_Key_Map)
    {
      printf("Checking '%s' - '%s'\n", key_map.first.c_str(), str.c_str());
      if (key_map.first == str)
      {
        valid = true;
        switch_key |= (1 << key_map.second);
        break;
      }
    }
    if (!valid)
    {
      printf("Invalid key: %s\n", str.c_str());
      return;
    }
  }

  std::vector<std::string> keyboard_key;
  len = splitWithBasicString(args[1], '-', keyboard_key);
  
  SDL_Keysym k;
  k.sym      = SDLK_UNKNOWN;
  k.mod      = KMOD_NONE;

  for (auto str : keyboard_key)
  {
    trim(str);
    if (str == "ALT")
    {
      k.mod = static_cast<SDL_Keymod>(k.mod | KMOD_ALT);
    }
    else if (str == "CTRL")
    {
      k.mod = static_cast<SDL_Keymod>(k.mod | KMOD_CTRL);
    }
    else if (str == "SHIFT")
    {
      k.mod = static_cast<SDL_Keymod>(k.mod | KMOD_SHIFT);
    }
    else
    {
	if (str.length() == 0) {
          std::cout << "Keybinder: parse error in line: " << line << std::endl;
          return;
	} else if (str.length() == 1) {
          // translate 1-letter keys straight to SDL_Keycode
          auto c = static_cast<unsigned char>(str[0]);
          if (std::isgraph(c) && c != '%' && c != '{' && c != '|' && c != '}' && c != '~') {
            c = std::tolower(c);	// need lowercase
            k.sym = static_cast<SDL_Keycode>(c);
          } else {
            std::cout << "Keybinder: unsupported key: " << str << std::endl;
            return;
          }
      } else {
        for (size_t i = 0; strlen(SDLKeyStringTable[i].s) > 0; i++)
        {
          if (str == SDLKeyStringTable[i].s)
          {
            k.sym = SDLKeyStringTable[i].k;
            break;
          }
        }
      }
      if (k.sym == SDLK_UNKNOWN)
      {
        printf("Unknown key %s\n", str.c_str());
        return;
      }
    }
  }
  printf("Mapping %lu to %d.%d\n", switch_key, k.mod, k.sym);
  key_map_[switch_key] = k;
}

void SwitchKeys::send_event(size_t joy, bool press)
{
  printf("Checking for mapping of: %d\n", joy);
  auto itr = key_map_.find(joy);
  if (itr == key_map_.end())
  {
    return;
  }
  SDL_Event event;
  event.type = press ? SDL_KEYDOWN : SDL_KEYUP;
  event.key.keysym = itr->second;
  printf("Sending event %d - %d\n", event.key.keysym.sym, event.key.keysym.mod);
  SDL_PushEvent(&event);
}

void SwitchKeys::key_event(size_t key, bool press)
{
  if (press)
  {
    current_keys_ |= (1 << key);
    send_event(current_keys_, press);
  }
  else
  {
    send_event(current_keys_, press);
    current_keys_ &= ~(1 << key);
  }
}
