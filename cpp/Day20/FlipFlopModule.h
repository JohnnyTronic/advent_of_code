#pragma once

#include "Module.h"

class FlipFlopModule : public Module {
public:
	bool isOn = false;

	void ReceivePulse(Pulse* pulse) override;
};