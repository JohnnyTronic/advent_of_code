#include "DeadEndModule.h"

void DeadEndModule::ReceivePulse(Pulse* pulse) {
	if (pulse->level == LOW)
		hasReceivedLowPulse = true;
}