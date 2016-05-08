/*
 * CFSM.hpp
 *
 *  Created on: Apr 30, 2016
 *      Author: Ilya Skurikhin
 */

#ifndef CFSM_HPP
#define CFSM_HPP

#include <Utility/Utility.hpp>
#include <Utility/Logging.hpp>

#include <vector>
#include <string>

typedef Uid State;

class CFSM
{
public:

  /**
   * @brief Construct from existing states
   *
   * Construct new CFSM from states and set
   * state to the first element
   */
  CFSM(std::vector<State> states);

  /**
   * @brief Destructor.
   *
   * Destructor that does nothing.
   * It's here because c++ guidelines say so
   */
  virtual
  ~CFSM();

  /**
   * @brief Get current state.
   *
   * @return current state
   */
  State
  getState();

  /**
   * @brief Move to next state.
   *
   * Move to next state in states_.
   */
  void
  nextState();

  /**
   * @brief Change behavior to new state.
   *
   * @param new state
   */
  virtual void
  onEnterState(State state) =0;

  /**
   * @brief Things to do while in the state
   *
   * @param state current state
   * @param dt    time for counter
   */
  virtual void
  onState(State state, sf::Time dt) =0;

  /**
   * @brief Action to take in current state.
   *
   * @param time for counter
   */
  void
  action(sf::Time dt);

private:
  std::vector<State> states_;
  State state_;

};

#endif /* CFSM_HPP */
