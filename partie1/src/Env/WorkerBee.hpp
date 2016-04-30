/*
 * WorkerBee.hpp
 *
 *  Created on: Apr 28, 2016
 *      Author: ilya
 */

#ifndef WORKERBEE_HPP
#define WORKERBEE_HPP

#include <Env/Bee.hpp>
#include <JSON/JSON.hpp>

class WorkerBee : public Bee
{
public:

  WorkerBee (Hive* hive, const Vec2d& position);

  ~WorkerBee ();

  void
  reloadConfig ();

  j::Value const&
  getConfig () override;

  void
  onState (State state, sf::Time dt) override;

private:

  State static const COLLECT_POLLEN;

};

#endif /* WORKERBEE_HPP */
