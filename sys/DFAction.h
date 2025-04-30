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
	PANIC_WHILE_PROCESSING,
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
	size_t end_tokens_index;
};

struct DFActionContext {
	std::vector<PrevDFAState> dfa_stack;
	DFActionState current_state;
	DFA current_dfa;
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

	inline DFActionContext get_default_context(const DFActionState& start_state = DFActionState{}) {
		if (machine == nullptr) {
			return { {}, DFActionState{}, DFA{} };
		}
		
		return {
			{},          
			start_state, 
			*machine     
		};
	}
	
	inline DFActionReturnVal run_dfa_on(const std::vector<DFActionToken>& tokens 
		, const DFActionState& start_state , size_t* index_in_tks = nullptr, DFActionContext* ctx = nullptr ) {

		size_t default_index = 0;
		
		size_t* index_ptr;
		if (index_in_tks) {
			index_ptr = index_in_tks;
		} else {
			index_ptr = &default_index;
		}

		size_t& index = *index_ptr;


		if (machine == nullptr) {
			return { NULL_MAIN_DFA_PASSED , std::vector<PrevDFAState>() , index};
		}

		std::vector<PrevDFAState>* dfa_stack_ptr;
		DFA* dfa_ptr;
		DFActionState* state_ptr;

		std::vector<PrevDFAState> local_dfa_stack;
		DFActionState local_state;


		if (ctx != nullptr) {
			dfa_stack_ptr = &ctx->dfa_stack;
			dfa_ptr = &ctx->current_dfa;
			state_ptr = &ctx->current_state;
		} else {
			dfa_stack_ptr = &local_dfa_stack;
			dfa_ptr = machine; // Use the machine passed to the constructor or new_dfa
			local_state = start_state; // Initialize local state if no context
			state_ptr = &local_state;
		}

		auto& dfa_stack = *dfa_stack_ptr;
		auto& dfa = *dfa_ptr;
		auto& state = *state_ptr;


		for (; index < tokens.size();) {

			bool next_index = true;

			DFActionFlow change_state = action_function(index , tokens , state, next_index);



			if (next_index)
			++index;

			if (change_state.code == DFACTION_PANIC) {
				std::cout << "PANIC";
				return {PANIC_WHILE_PROCESSING, dfa_stack , index };
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
			} else if (change_state.code == DFACTION_COMPILE_DONE) {


				break;
			}
			else if (change_state.code != DFACTION_DO_NOT_CHANGE_STATE) {


				state =  dfa[state][tokens[index].type];
			}
		}

		if (!dfa_stack.empty()) {
			return { FAILED_TO_DO_ALL_REDUCTIONS , dfa_stack , index };
		}

		return {ALL_REDUCTIONS_ARE_COMPLETED, dfa_stack , index };
	}
};

#endif