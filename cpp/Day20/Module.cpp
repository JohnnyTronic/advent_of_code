#include "Pulse.h"
#include "PulseQueue.h"
#include "Module.h"
#include <sstream>
#include <vector>

void Module::SendPulse(PulseLevel level) {
	for (auto downstreamModule : downstreamModules) {
		Pulse* newPulse = new Pulse(this, downstreamModule, level);
		pulseQueue->SendPulse(newPulse);
	}
}

void Module::AddUpstreamModule(Module* upstreamModule) {
	upstreamModules.push_back(upstreamModule);
}

void Module::AddDownstreamModule(Module* downstreamModule) {
	downstreamModules.push_back(downstreamModule);
}