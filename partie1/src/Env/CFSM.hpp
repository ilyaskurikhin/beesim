/*
 * CFSM.hpp
 *
 *  Created on: Apr 30, 2016
 *      Author: Ilya Skurikhin
 */

#ifndef CFSM_HPP
#define CFSM_HPP

#include <Utility/Utility.hpp>
#include <vector>

typedef Uid State;

class CFSM
{
public:

  CFSM (std::vector<State> states);

  virtual ~CFSM ();

  State
  getState ();

  void
  nextState ();

  void
  onEnterState (State state);

  virtual void
  onState (State state, sf::Time dt) =0;

  void
  action (sf::Time dt);

private:
  std::vector<State> states_;
  State state_;

};


#endif /* CFSM_HPP */
