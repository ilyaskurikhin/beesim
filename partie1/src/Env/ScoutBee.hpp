/*
 * ScoutBee.hpp
 *
 *  Created on: Apr 28, 2016
 *      Author: Ilya Skurikhin
 */

#ifndef SCOUTBEE_HPP
#define SCOUTBEE_HPP

#include <Env/Bee.hpp>
#include <Env/Flower.hpp>
#include <Env/WorkerBee.hpp>
#include <Env/Env.hpp>

#include <JSON/JSON.hpp>
#include <Application.hpp>

class ScoutBee : public Bee
{
public:

  ScoutBee(Hive* hive, const Vec2d& position, std::vector<State> states);

  ~ScoutBee();

  void
  drawOn(sf::RenderTarget& target) const override;

  void
  reloadConfig();

  j::Value const&
  getConfig() override;

  void
  onState(State state, sf::Time dt) override;

  void
  onEnterState(State state) override;

  // TODO make private
  State static const SEARCH_FLOWER;
  State static const IN_HIVE;
  State static const RETURN_HIVE;

  bool
  isScout() const override;

  bool
  isWorker() const override;

  bool
  isInHive() const override;

  void
  interact(Bee* other) override;

  void
  interactWith(ScoutBee* scouting) override;

  void
  interactWith(WorkerBee* working) override;

private:
  Vec2d flower_location_;
  int number_times_shared_;

  double energy_leave_hive_;
  double energy_seek_flowers_;

  double max_sharing_;

};

#endif /* SCOUTBEE_HPP */
