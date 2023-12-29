#pragma once

#include "Pulse.h"
#include "PulseQueue.h"
#include <sstream>
#include <vector>
#include <queue>

class Pulse;

class Module {
public:
	std::string name;
	std::vector<Module*> upstreamModules{};
	std::vector<Module*> downstreamModules{};
	PulseQueue& pulseQueue;

	Module(std::string name, PulseQueue& pulseQueue) : name(name), pulseQueue(pulseQueue) {};

	virtual void ReceivePulse(Pulse* pulse) = 0;

	void SendPulse(PulseLevel level) {
		for (auto downstreamModule : downstreamModules) {
			Pulse* newPulse = new Pulse(this, downstreamModule, level);
			pulseQueue.queue.push_back(newPulse);
		}
	}
};