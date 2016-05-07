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

  ScoutBee (Hive* hive, const Vec2d& position, std::vector<State> states);

  ~ScoutBee ();

  void
  drawOn(sf::RenderTarget& target) const;

  void
  reloadConfig ();

  void
  randomMove (sf::Time dt) override;

  j::Value const&
  getConfig () override;

  void
  onState (State state, sf::Time dt) override;

  void
  onEnterState (State state) override;
  
  void 
  targetMove (sf::Time dt) override;

  // TODO make private
  State static const IN_HIVE;
  State static const SEARCH_FLOWER;
  State static const  RETURN_HIVE;


private:
  Vec2d flower_location_;
  int number_times_shared_;

  std::string debug_status_;
  double debug_text_size_;

  double energy_leave_hive_;
  double energy_seek_flowers_;

  double max_sharing_;

  double rotation_probability_;
  double max_angle_;

};

#endif /* SCOUTBEE_HPP */
