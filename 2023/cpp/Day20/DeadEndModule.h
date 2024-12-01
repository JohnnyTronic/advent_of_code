#pragma once

#include "Module.h"

class DeadEndModule : public Module {
public:
	DeadEndModule(std::string name) : Module(name, nullptr), hasReceivedLowPulse(false) {};
	void AddUpstreamModule(Module* upstreamModule) override {};
	void AddDownstreamModule(Module* downstreamModule) override {};
	void ReceivePulse(Pulse* pulse) override;

	bool HasReceivedLowPulse() {
		return hasReceivedLowPulse;
	}

protected:
	bool hasReceivedLowPulse;
};