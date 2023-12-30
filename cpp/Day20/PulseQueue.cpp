#include "Pulse.h"
#include "PulseQueue.h"
#include "Module.h"

void PulseQueue::SendPulse(Pulse* pulse) {
	queue.push_back(pulse);
	switch (pulse->level) {
	case HIGH: highPulseTally++; break;
	case LOW: lowPulseTally++; break;
	}
}

void PulseQueue::Reset() {
	lowPulseTally = 0;
	highPulseTally = 0;
	for (auto pulse : queue) {
		delete pulse;
		queue.clear();
	}
}

void PulseQueue::SimulatePulses() {
	while (queue.size() > 0) {
		Pulse* pulse = queue.front();
		queue.pop_front();

		pulse->toModule->ReceivePulse(pulse);
	}
}