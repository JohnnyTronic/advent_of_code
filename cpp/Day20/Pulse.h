#pragma once

#include "PulseLevel.h"

class Module;

class Pulse {
public:
	Module* fromModule;
	Module* toModule;
	PulseLevel level;

	Pulse(Module* fromModule, Module* toModule, PulseLevel level) : fromModule(fromModule), toModule(toModule), level(level) {};
};