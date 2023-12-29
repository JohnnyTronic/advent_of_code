#pragma once

#include "Module.h"

class Module;

enum PulseLevel {
	LOW,
	HIGH
};

class Pulse {
public:
	Module* fromModule;
	Module* toModule;
	PulseLevel level;

	Pulse(Module* fromModule, Module* toModule, PulseLevel level) : fromModule(fromModule), toModule(toModule), level(level) {};
};