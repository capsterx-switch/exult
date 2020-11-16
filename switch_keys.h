#pragma once
#include <string>
namespace nswitch {
  class Switch_Key_Map;
}

void LoadFromFileInternal(nswitch::Switch_Key_Map *, std::string const &);
