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

  ScoutBee(Hive* hive, const Vec2d& position);

  ~ScoutBee();

  void
  reloadConfig ();

  void
  moveRandom (sf::Time dt);

  j::Value const&
  getConfig () override;

private:
  double energy_seek_flowers_;

  double max_sharing_;

  double rotation_probability_;
  double max_angle_;

};

#endif /* SCOUTBEE_HPP */
