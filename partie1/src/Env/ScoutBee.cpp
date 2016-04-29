/*
 * ScoutBee.cpp
 *
 *  Created on: Apr 28, 2016
 *      Author: ilya
 */

#include <Env/ScoutBee.hpp>

ScoutBee::ScoutBee(Hive* hive, const Vec2d& position)
: Bee (hive, position)
{
  reloadConfig();
}

ScoutBee::~ScoutBee()
{

}

void
ScoutBee::reloadConfig()
{
  Bee::reloadConfig();

  // TODO load class attributes here
  rotation_probability_ = getConfig ()["moving behaviour"]["random"]["rotation probability"].toDouble();
  max_angle_ = getConfig ()["moving behaviour"]["random"]["rotation angle max"].toDouble();

  energy_seek_flowers_ = getConfig ()["energy"]["to seek flowers"].toDouble();
  max_sharing_ = getConfig ()["sharing"]["max"].toDouble();
}


void
ScoutBee::moveRandom (sf::Time dt)
{
  Vec2d position (position_);
  if (bernoulli (rotation_probability_))
    {
      double angleA (uniform (- max_angle_, max_angle_));
      move_vec_.rotate (angleA);
    }

  position.x += dt.asSeconds () * move_vec_.x;
  position.y += dt.asSeconds () * move_vec_.y;

  Collider protoBee (position, radius_);
  protoBee.clamping ();

  if (!getAppEnv ().isFlyable (protoBee.getPosition ()))
    {
      double angleB;
      if (bernoulli (0.5))
        {
          angleB = PI / 4;
        }
      else
        {
          angleB = -PI / 4;
        }
      move_vec_.rotate (angleB);

    }
  else
    {
      this->position_ = protoBee.getPosition ();
    }

  energy_ -= 0.1 * dt.asSeconds ();

}

j::Value const&
ScoutBee::getConfig ()
{
  // TODO make this refer to superclass method
  return getAppConfig ()["simulation"]["bees"]["scout"];
}

