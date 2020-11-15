#pragma once
#include "touchui.h"

class SwitchUI 
  : public TouchUI
{
public:
	SwitchUI();
	~SwitchUI();
	void promptForName(const char *name) override;
	void showGameControls() override;
	void hideGameControls() override;
	void showButtonControls() override;
	void hideButtonControls() override;
	void onDpadLocationChanged() override;
};
