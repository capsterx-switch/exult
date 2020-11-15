#pragma once
#include "touchui.h"

class SwitchUI {
public:
	SwitchUI();
	virtual ~SwitchUI() = default;
	void promptForName(const char *name) override;
	void showGameControls() override;
	void hideGameControls() override;
	void showButtonControls() override;
	void hideButtonControls() override;
	void onDpadLocationChanged() override;
};
