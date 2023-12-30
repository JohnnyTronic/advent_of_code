#pragma once

#include "Module.h"
#include <map>

class ConjunctionModule : public Module {
public:
	ConjunctionModule(std::string name, PulseQueue* pulseQueue) : Module(name, pulseQueue) {};

	void AddUpstreamModule(Module* upstreamModule);
	void ReceivePulse(Pulse* pulse) override;

private:
	std::map<std::string, PulseLevel> upstreamModuleLevels;
};