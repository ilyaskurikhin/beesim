/*
 * QueenBee.hpp
 *
 *  Created on: May 25, 2016
 *      Author: ilya
 */

#ifndef QUEENBEE_HPP_
#define QUEENBEE_HPP_

#include <Random/Random.hpp>

#include <Env/Bee/Bee.hpp>

class QueenBee : public Bee
{
public:
  QueenBee(Hive* hive, const Vec2d& position);

  QueenBee(const QueenBee& queenBee) = delete;

  ~QueenBee();

  QueenBee&
  operator=(const QueenBee& queenBee) = delete;

  void
  reloadConfig();

  const j::Value&
  getConfig() const override;

  void
  drawDebug(sf::RenderTarget& target) const override;

  void
  onState(State state, sf::Time dt);

  void
  onEnterState(State state);

  void
  interact(Bee* other) override;

  void
  interactWith(ScoutBee* scoutbee) override;

  void
  interactWith(WorkerBee* workerbee) override;

  Bee*
  giveBirthTo(BeeType beeType) const;

private:

  double energy_migrate_;
  double energy_in_hive_;
  double energy_create_hive_;

  double reproduction_probability_;

  double migration_distance_;

  State static const IN_HIVE;
  State static const MIGRATING;
  State static const RETURN_HIVE;
};

#endif /* QUEENBEE_HPP_ */
