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
#include "switch_keys.h"
#include "switch/keymap.hpp"


void LoadFromFileInternal(nswitch::Switch_Key_Map * keys, std::string const &file)
{
  std::ifstream keyfile;
  U7open(keyfile, file.c_str(), true);
  keys->load_file(keyfile);
  keyfile.close();
}
