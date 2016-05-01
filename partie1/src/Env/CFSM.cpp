/*
 * CFSM.cpp
 *
 *  Created on: Apr 30, 2016
 *      Author: Ilya Skurikhin
 */

#include <Env/CFSM.hpp>

CFSM::CFSM (std::vector<State> states)
: states_(states), state_(states[0])
{
}

CFSM::~CFSM ()
{

}

State
CFSM::getState ()
{
  return state_;
}

void
CFSM::nextState ()
{
  size_t stateNumber(0);
  for (size_t i=0; i < states_.size(); ++i)
    {
      if (states_[i] == state_) {
          stateNumber = i;
      }
    }

  if (stateNumber < states_.size() -1)
    {
      state_ = states_[stateNumber +1];
    }
  else
    {
      state_ = states_[0];
    }

  onEnterState(state_);
}

void
onEnterState (State state)
{
  // TODO implement
}

void
action (sf::Time dt)
{
  // TODO implement
}



