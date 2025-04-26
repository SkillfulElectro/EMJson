#pragma once
#ifndef MUTEXIS_DFACTION
#define MUTEXIS_DFACTION

#include <unordered_map>
#include <iostream>
#include <vector>

#include "DFAction_Types.h"

enum DFActionReturnCode {
	ALL_REDUCTIONS_ARE_COMPLETED,
	FAILED_TO_DO_ALL_REDUCTIONS,
	NULL_MAIN_DFA_PASSED,
};

enum DFActionFlowCode {
	DFACTION_GO_TO_SP_DFA = 1,
	DFACTION_SAFE = 0,
	DFACTION_DO_NOT_CHANGE_STATE = -1,
	DFACTION_PANIC = -2,
	DFACTION_BACK_TO_PREV = -3,
	DFACTION_COMPILE_DONE = 2,
};

struct DFActionFlow {
	DFActionFlowCode code;
	DFActionState state_name;
};

struct DFActionToken {
	DFActionType type;
	DFActionVal value;
};

typedef std::unordered_map<DFActionState, std::unordered_map<DFActionType, DFActionState>> DFA;

struct PrevDFAState {
	DFA dfa;
	DFActionState state;
};

struct DFActionReturnVal {
	DFActionReturnCode status;
	std::vector<PrevDFAState> dfas;
};

class DFAction {
	
protected:
	DFA* machine;

	std::unordered_map<DFActionState, DFA> specials_dfa;

	virtual DFActionFlow action_function(
		size_t& index_in_tokens 
		, const std::vector<DFActionToken>& tokens
		, DFActionState& current_state
		, bool& go_next_index
	) = 0;


	inline DFAction(DFA* machine = nullptr) {
		this->machine = machine;
	}

	inline void new_dfa(DFA* states) {
		this->machine = states;
	}

	inline void add_special_dfa(DFActionState start_state, DFA dfa) {
		specials_dfa[start_state] = dfa;
	}

	inline DFActionReturnVal run_dfa_on(const std::vector<DFActionToken>& tokens 
		, const DFActionState& start_state) {

		if (machine == nullptr) {
			return { NULL_MAIN_DFA_PASSED , std::vector<PrevDFAState>() };
		}

		std::vector<PrevDFAState> dfa_stack;

		auto& dfa = *machine;

		DFActionState state = start_state;

		for (size_t index{ 0 }; index < tokens.size();) {

			bool next_index = true;

			DFActionFlow change_state = action_function(index , tokens , state, next_index);

			if (next_index)
			++index;

			if (change_state.code == DFACTION_PANIC) {
				std::cout << "PANIC";
				break;
			}
			else if (change_state.code == DFACTION_GO_TO_SP_DFA) {
				dfa_stack.push_back({
					dfa , state
					});

				dfa = specials_dfa[change_state.state_name];
				state = change_state.state_name;
			}
			else if (change_state.code == DFACTION_BACK_TO_PREV) {
				dfa = dfa_stack.back().dfa;
				state = dfa_stack.back().state;

				dfa_stack.pop_back();
			}
			else if (change_state.code != DFACTION_DO_NOT_CHANGE_STATE) {
				state =  dfa[state][tokens[index].type];
			} else if (change_state.code == DFACTION_COMPILE_DONE) {
				break;
			}
		}

		if (!dfa_stack.empty()) {
			return { FAILED_TO_DO_ALL_REDUCTIONS , dfa_stack };
		}

		return {ALL_REDUCTIONS_ARE_COMPLETED, dfa_stack };
	}
};

#endif