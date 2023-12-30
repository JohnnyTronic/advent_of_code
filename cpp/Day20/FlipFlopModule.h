#pragma once

#include "Module.h"

class FlipFlopModule : public Module {
public:
	bool isOn = false;

	FlipFlopModule(std::string name, PulseQueue* pulseQueue) : Module(name, pulseQueue) {};

	void ReceivePulse(Pulse* pulse) override;
};