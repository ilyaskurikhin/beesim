/*
 * ScoutBee.hpp
 *
 *  Created on: Apr 28, 2016
 *      Author: Ilya Skurikhin
 */

#ifndef SCOUTBEE_HPP
#define SCOUTBEE_HPP

#include <Env/Bee.hpp>
#include <JSON/JSON.hpp>

class ScoutBee : public Bee
{
public:

  ScoutBee (Hive* hive, const Vec2d& position, std::vector<State> states);

  ~ScoutBee ();

  void
  reloadConfig ();

  void
  randomMove (sf::Time dt) override;

  j::Value const&
  getConfig () override;

  void
  onState (State state, sf::Time dt) override;

  void
  onEnterState () override;
  
  void 
  targetMove () override;

  // TODO make private
  State static const IN_HIVE;
  State static const SEARCH_FLOWER;
  State static const RETURN_HIVE;


private:
  double energy_seek_flowers_;

  double max_sharing_;

  double rotation_probability_;
  double max_angle_;

};

#endif /* SCOUTBEE_HPP */
