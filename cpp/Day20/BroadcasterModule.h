#pragma once

#include "Module.h"
#include <sstream>

class BroadcasterModule : public Module {
public:

	BroadcasterModule(std::string name, PulseQueue* pulseQueue) : Module(name, pulseQueue) {};
	
	void ReceivePulse(Pulse* pulse) override {

	}
};