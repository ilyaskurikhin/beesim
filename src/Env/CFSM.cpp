/**
 * @file CFSM.cpp
 * @brief
 * @author Ilya Skurikhin
 * @author Julia Besson
 * @version p5.1
 * @date 2016-05-05
 */

#include <Env/CFSM.hpp>

CFSM::CFSM(std::vector<State> states) :
    states_(states), state_(states[0])
{
}

CFSM::CFSM(const CFSM& cfsm)
    : states_(cfsm.getStates()),
      state_(cfsm.getState())
{

}

CFSM&
CFSM::operator=(const CFSM& cfsm)
{
    states_ = cfsm.getStates();
    state_ = cfsm.getState();
    return *this;
}

State
CFSM::getState() const
{
    return state_;
}

std::vector<State>
CFSM::getStates() const
{
    return states_;
}

void
CFSM::nextState()
{

    size_t stateNumber(0);
    // get state number, number_times_shared_(-1)
    for (size_t i = 0; i < states_.size(); ++i) {
        if (states_[i] == state_) {
            stateNumber = i;
        }
    }

    // cycle to next state
    if (stateNumber < states_.size() - 1) {
        state_ = states_[stateNumber + 1];
    } else {
        state_ = states_[0];
    }

    onEnterState(state_);
}

void
CFSM::action(sf::Time dt)
{
    onState(state_, dt);
}

