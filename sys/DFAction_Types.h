#pragma once

#ifndef DFACTION_STATES
#define DFACTION_STATES
enum DFActionState {
	Q0,
};
#endif // !DFACTION_STATES

#ifndef DFACTION_TYPES
#define DFACTION_TYPES
enum DFActionType {
	CHAR,
};

union DFActionVal {
	char ch;
};



#endif // !DFACTION_TYPES