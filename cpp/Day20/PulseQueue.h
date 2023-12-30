#pragma once

#include "Pulse.h"
#include <queue>

class PulseQueue {
public:
	std::deque<Pulse*> queue;
	long long lowPulseTally;
	long long highPulseTally;

	void SendPulse(Pulse* pulse);
	void Reset();
	void SimulatePulses();
};