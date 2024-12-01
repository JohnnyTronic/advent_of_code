#pragma once

#include "PulseLevel.h"
#include "PulseQueue.h"
#include <sstream>
#include <vector>
#include <queue>

class Module {
public:
	std::string name;	

	Module(std::string name, PulseQueue* pulseQueue) : name(name), pulseQueue(pulseQueue) {};
	virtual ~Module() = default;

	virtual void ReceivePulse(Pulse* pulse) = 0;
	void SendPulse(PulseLevel level);

	virtual void AddUpstreamModule(Module* upstreamModule);
	virtual void AddDownstreamModule(Module* downstreamModule);

protected:
	std::vector<Module*> upstreamModules{};
	std::vector<Module*> downstreamModules{};
	PulseQueue* pulseQueue;
};