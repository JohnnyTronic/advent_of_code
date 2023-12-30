#pragma once

#include "BroadcasterModule.h"
#include "FlipFlopModule.h"
#include "ConjunctionModule.h"
#include "DeadEndModule.h"
#include <map>

class ModuleMap {
public:
	ButtonModule* buttonModule;
	BroadcasterModule* broadcasterModule;
	std::map<std::string, FlipFlopModule*> flipFlopModules;
	std::map<std::string, ConjunctionModule*> conjuctionModules;
	std::map<std::string, DeadEndModule*> deadEndModules;

	void AddModule(ConjunctionModule* module) {
		conjuctionModules[module->name] = module;
	}

	void AddModule(FlipFlopModule* module) {
		flipFlopModules[module->name] = module;
	}

	void AddModule(BroadcasterModule* module) {
		broadcasterModule = module;
	}

	void AddModule(ButtonModule* module) {
		buttonModule = module;
	}

	Module* GetModule(std::string moduleName) {
		if (moduleName == "broadcaster") {
			return broadcasterModule;
		}
		else if (moduleName == "button") {
			return buttonModule;
		}
		else {
			if (flipFlopModules.contains(moduleName)) {
				return flipFlopModules[moduleName];
			}
			else if (conjuctionModules.contains(moduleName)) {
				return conjuctionModules[moduleName];
			}
		}

		// A "dead end" module that's only mentioned in some other module's output
		DeadEndModule* deadEndModule = new DeadEndModule(moduleName);
		deadEndModules[moduleName] = deadEndModule;

		return deadEndModule;
	}
};