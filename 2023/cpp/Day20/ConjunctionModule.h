#pragma once

#include "Module.h"
#include <map>

class ConjunctionModule : public Module {
public:
	ConjunctionModule(std::string name, PulseQueue* pulseQueue) : Module(name, pulseQueue) {};

	void AddUpstreamModule(Module* upstreamModule) override;
	void ReceivePulse(Pulse* pulse) override;

	void Reset() {
		for (auto& kvp : upstreamModuleLevels) {
			kvp.second = LOW;
		}
	}

	void ClearFlag() {
		lastSentPulse = -1;
	}

	int lastSentPulse = -1;

private:
	std::map<std::string, PulseLevel> upstreamModuleLevels;
};