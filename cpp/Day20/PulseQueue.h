#pragma once

#include "Pulse.h"
#include <queue>

class PulseQueue {
public:
	std::deque<Pulse*> queue;
	long long lowPulseTally;
	long long highPulseTally;

	void SendPulse(Pulse* pulse) {
		queue.push_back(pulse);
		switch (pulse->level) {
		case HIGH: highPulseTally++; break;
		case LOW: lowPulseTally++; break;
		}
	}
};