#pragma once

#include "Module.h"
#include <sstream>

class BroadcasterModule : public Module {
public:

	BroadcasterModule(std::string name, PulseQueue* pulseQueue) : Module(name, pulseQueue) {};
	
	void ReceivePulse(Pulse* pulse) override {
		if (pulse == nullptr)
			throw "nullptr pulse received";

		for (auto downstreamModule : downstreamModules) {
			Pulse* newPulse = new Pulse(this, downstreamModule, pulse->level);
			pulseQueue->SendPulse(newPulse);
		}
	}
};