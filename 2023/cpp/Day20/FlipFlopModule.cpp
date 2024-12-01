#include "FlipFlopModule.h"
#include "Pulse.h"

void FlipFlopModule::ReceivePulse(Pulse* pulse) {
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