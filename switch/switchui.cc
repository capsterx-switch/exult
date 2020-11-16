#include "switchui.h"
#include <switch.h>
#include <string>
#include <switch/keyboard.hpp>


SwitchUI::SwitchUI()
{
}

SwitchUI::~SwitchUI()
{
}

void
SwitchUI::
promptForName(const char *input)
{
  auto name = nswitch::get_keyboard_input(input);
  onTextInput(name.c_str());
}

void
SwitchUI::
showGameControls()
{
}

void
SwitchUI::
hideGameControls()
{
}

void
SwitchUI::
showButtonControls()
{
}

void
SwitchUI::
hideButtonControls()
{
}

void
SwitchUI::
onDpadLocationChanged()
{
}

