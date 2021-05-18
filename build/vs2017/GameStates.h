#pragma once
enum class State { Init, MENU,OPTIONS, CONTROLS, PAUSE_CONTROLS, LEVEL, PAUSE, CREDITS, DEATH };

class GameStates
{
public:
    // Set the current state
    void setCurrentState(State s);
    // Returns the current state.
    State getCurrentState();

protected:
    State currentState;
};

