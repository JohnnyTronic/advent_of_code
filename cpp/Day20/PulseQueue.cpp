#include "Pulse.h"
#include "PulseQueue.h"
#include "Module.h"
#include <iostream>

void PulseQueue::SendPulse(Pulse* pulse) {
	queue.push_back(pulse);
	switch (pulse->level) {
	case HIGH: highPulseTally++; break;
	case LOW: lowPulseTally++; break;
	}
	//std::cout << "PulseQueue.SendPulse received - From: " << pulse->fromModule->name << ", To: " << pulse->toModule->name << ", Level: " << pulse->level << "\n";
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