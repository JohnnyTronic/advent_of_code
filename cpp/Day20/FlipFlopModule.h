#pragma once

#include "Module.h"

class FlipFlopModule : public Module {
public:
	bool isOn = false;

	void ReceivePulse(Pulse* pulse) override {
		switch (pulse->level) {
		case HIGH: return; // Ignore
		case LOW: {
			isOn = !isOn;
			SendPulse(isOn ? HIGH : LOW); // Pulse level is same as new local On state
			return;
		}
		}
		if (pulse->level == HIGH) {
			// Ignore
			return;
		}

		
	}
};