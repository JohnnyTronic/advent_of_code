#include "Pulse.h"
#include "PulseQueue.h"
#include "Module.h"
#include <sstream>
#include <vector>

void Module::SendPulse(PulseLevel level) {
	for (auto downstreamModule : downstreamModules) {
		Pulse* newPulse = new Pulse(this, downstreamModule, level);
		pulseQueue->queue.push_back(newPulse);
	}
}