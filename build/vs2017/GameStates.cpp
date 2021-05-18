#include "GameStates.h"
// Set the current state
void GameStates::setCurrentState(State s)
{
    currentState = s;
}

// Returns the current state.
State GameStates::getCurrentState()
{
    return currentState;
}
