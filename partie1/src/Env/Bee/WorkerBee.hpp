/*
 * WorkerBee.hpp
 *
 *  Created on: Apr 28, 2016
 *      Author: ilya
 */

#ifndef WORKERBEE_HPP
#define WORKERBEE_HPP

#include <JSON/JSON.hpp>

#include <Env/Bee/Bee.hpp>
#include <Env/Flower.hpp>

class WorkerBee : public Bee
{
public:

  WorkerBee(Hive* hive, const Vec2d& position);

  WorkerBee(const WorkerBee& workerbee) = delete;

  ~WorkerBee();

  const WorkerBee&
  operator=(const WorkerBee& workerbee) = delete;

  void
  reloadConfig();

  j::Value const&
  getConfig() const override;

  void
  drawDebug(sf::RenderTarget& target) const override;

  void
  onState(State state, sf::Time dt) override;

  void
  onEnterState(State state) override;

  void
  setFlower(const Vec2d& position);

  const Vec2d&
  getFlower() const;

  double
  getPollen() const;

  double
  takePollen(double pollen);

  double
  transferPollen(sf::Time dt);

  double
  eatPollen(Flower* flower, sf::Time dt);

  void
  learnFlowerLocation(const Vec2d& flowerPosition);

  void
  setPollen(double amount);

  void
  interact(Bee* other) override;

  void
  interactWith(ScoutBee* scouting) override;

  void
  interactWith(WorkerBee* working) override;

  const Vec2d&
  getFlowerLocation() const;

  void
  setFlowerLocation(const Vec2d& flowerLocation);

private:

  Vec2d flower_location_;
  double pollen_;
  double max_pollen_;
  double pollen_collecting_rate_;
  double energy_leave_hive_;
  double pollen_transfer_rate_;

  State static const TO_FLOWER;
  State static const COLLECT_POLLEN;
  State static const IN_HIVE;
  State static const RETURN_HIVE;

};

#endif /* WORKERBEE_HPP */
