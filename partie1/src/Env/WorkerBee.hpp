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

  WorkerBee (Hive* hive, const Vec2d& position, std::vector<State> states);

  ~WorkerBee ();

  void
  reloadConfig ();

  j::Value const&
  getConfig () override;

  void
  onState (State state, sf::Time dt) override;

  void
  setFlower(const Vec2d& position);

  // TODO make private
  State static const IN_HIVE;
  State static const COLLECT_POLLEN;

private:
  Vec2d flower_location_;

};

#endif /* WORKERBEE_HPP */
