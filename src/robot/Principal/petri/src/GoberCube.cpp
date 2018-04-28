/*
 * Generated by PetriLab - https://sigilence-technologies.com/petrilab
 * Version 3.0.0-beta1
 */

#include "../Deplacement.h"
#include "../Moteur.h"
#include "../Servo.h"
#include "../Utils.h"
#include <cstdint>
#include <petrilab/Cpp/Action.h>
#include <petrilab/Cpp/MemberPetriDynamicLib.h>
#include <petrilab/Cpp/PetriDebug.h>
#include <petrilab/Cpp/PetriUtils.h>
#include <petrilab/Cpp/VarSlot.h>
#include <petrilab/Cpp/Variable.h>

using namespace Petri;

struct FillResult {
	void* start;
	void* end;
};

enum Petri_Var_Enum : std::uint_fast32_t { Petri_Var_Enum_Vvar0 = 0 };

static void fillVariables(Petri::VarSlot& variables) {
	variables[Petri_Var_Enum_Vvar0].setName("$var0");
	variables[Petri_Var_Enum_Vvar0].setDefaultValue(0);
}

extern "C" Petri_actionResult_t GoberCube_entryInit(VarSlot& _PETRI_PRIVATE_GET_VARIABLES_) {
	if(!_PETRI_PRIVATE_GET_VARIABLES_.isFirstSlot()) {
		_PETRI_PRIVATE_GET_VARIABLES_.pushVariables(1);
	}
	fillVariables(_PETRI_PRIVATE_GET_VARIABLES_);
	return static_cast<actionResult_t>(Petri::Utility::doNothing());
}

extern "C" Petri_actionResult_t GoberCube_exitAction(VarSlot& _PETRI_PRIVATE_GET_VARIABLES_) {
	auto _PETRI_PRIVATE_EXEC_RESULT_ = static_cast<actionResult_t>(Petri::Utility::doNothing());
	_PETRI_PRIVATE_GET_VARIABLES_.pop();
	_PETRI_PRIVATE_GET_VARIABLES_.pushReturnValues(0);
	return _PETRI_PRIVATE_EXEC_RESULT_;
}

static Petri_actionResult_t state_8_invocation(VarSlot& _PETRI_PRIVATE_GET_VARIABLES_) {
	return static_cast<actionResult_t>(Petri::Utility::doNothing());
}

static bool transition_22_invocation(VarSlot const& _PETRI_PRIVATE_GET_VARIABLES_, Petri_actionResult_t _PETRI_PRIVATE_GET_ACTION_RESULT_) {
	return true;
}

static bool transition_21_invocation(VarSlot const& _PETRI_PRIVATE_GET_VARIABLES_, Petri_actionResult_t _PETRI_PRIVATE_GET_ACTION_RESULT_) {
	return true;
}


extern "C" FillResult GoberCube_fill(PetriNet& petriNet,
                                     std::uint64_t entitiesOffset,
                                     bool firstLevel,
                                     Petri_actionResult_t (*initEntryPtr)(VarSlot&),
                                     Petri_actionResult_t (*exitActionPtr)(VarSlot&)) {
	auto& state_0 = petriNet.addAction(Action(0 + entitiesOffset, "Root_Entry", initEntryPtr, 0), firstLevel);
	auto& state_7 = petriNet.addAction(Action(7 + entitiesOffset, "Root_End", exitActionPtr, 1), false);
	auto& state_8 = petriNet.addAction(Action(8 + entitiesOffset, "Root_BEGIN", &state_8_invocation, 0), false);


	state_0.addTransition(22 + entitiesOffset, "Root_Entry_to_BEGIN", state_8, &transition_22_invocation);
	state_8.addTransition(21 + entitiesOffset, "Root_", state_7, &transition_21_invocation);

	return (FillResult){&state_0, &state_7};
}
namespace Petri {
	namespace Generated {
		namespace GoberCube {
			std::unique_ptr<::Petri::PetriNet> createPetriNet() {
				auto petriNet = std::make_unique<PetriNet>("GoberCube", 1);
				petriNet->setLogVerbosity(PetriNet::VerbosityNothing);
				GoberCube_fill(*petriNet, 0, true, &GoberCube_entryInit, &GoberCube_exitAction);
				fillVariables(petriNet->variables());
				return petriNet;
			}

			std::unique_ptr<::Petri::PetriDebug> createDebugPetriNet() {
				auto petriNet = std::make_unique<PetriDebug>("GoberCube", 1);
				petriNet->setLogVerbosity(PetriNet::VerbosityNothing);
				GoberCube_fill(*petriNet, 0, true, &GoberCube_entryInit, &GoberCube_exitAction);
				fillVariables(petriNet->variables());
				return petriNet;
			}
		}
	}
}

extern "C" void* GoberCube_create() {
	return Petri::Generated::GoberCube::createPetriNet().release();
}

extern "C" void* GoberCube_createDebug() {
	return Petri::Generated::GoberCube::createDebugPetriNet().release();
}

extern "C" char* GoberCube_evaluate(void* vars, char const* libPath) {
	return Petri::Utility::loadEvaluateAndInvoke(vars, libPath, "GoberCube");
}
extern "C" char const* GoberCube_getHash() {
	return "CF2BC762F9F9A8D4324C33AB88C623E0EDE4F0492A69DA60D89A60B8A4EC9136";
}


extern "C" void* GoberCube_createLibForEditor() {
	return ::Petri::MemberPetriDynamicLib::libForEditor("GoberCube", 12346);
}
