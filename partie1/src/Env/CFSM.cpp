/**
 * @file CFSM.cpp
 * @brief 
 * @author Ilya Skurikhin
 * @author Julia Besson
 * @version p5.1
 * @date 2016-05-05
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
  // get state number
  for (size_t i=0; i < states_.size(); ++i)
    {
      if (states_[i] == state_) {
          stateNumber = i;
      }
    }

  // cycle to next state
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
