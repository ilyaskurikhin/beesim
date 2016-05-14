/*
 * WorkerBee.hpp
 *
 *  Created on: Apr 28, 2016
 *      Author: ilya
 */

#ifndef WORKERBEE_HPP
#define WORKERBEE_HPP

#include <Env/Bee.hpp>
#include <Env/Flower.hpp>
#include <JSON/JSON.hpp>

class WorkerBee : public Bee
{
public:

  WorkerBee(Hive* hive, const Vec2d& position, std::vector<State> states);

  ~WorkerBee();

  void
  reloadConfig();

  j::Value const&
  getConfig() override;

  void
  drawOn(sf::RenderTarget& target) const;

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

  void
  eatPollen(Flower* flower, sf::Time dt);

  void
  learnFlowerLocation(const Vec2d& flowerPosition);

  void
  setPollen(double amount);

  bool
  isScout() const override;

  bool
  isWorker() const override;

  void
  interact(Bee* other) override;

  void
  interactWith(ScoutBee* scouting) override;

  void
  interactWith(WorkerBee* working) override;

  // TODO make private

  State static const TO_FLOWER;
  State static const COLLECT_POLLEN;

private:

  Vec2d flower_location_;
  double pollen_;
  double max_pollen_;
  double pollen_collecting_rate_;
  double energy_leave_hive_;
  double pollen_transfer_rate_;

};

#endif /* WORKERBEE_HPP */
