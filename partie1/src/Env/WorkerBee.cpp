/*
 * WorkerBee.cpp
 *
 *  Created on: Apr 28, 2016
 *      Author: ilya
 */

#include <Env/WorkerBee.hpp>

WorkerBee::WorkerBee (Hive* hive, const Vec2d& position, std::vector<State> states) :
    Bee (hive, position, states)
{
  logEvent ("WorkerBee", "new constructed");
  reloadConfig ();
  loadTexture ();
}

WorkerBee::~WorkerBee ()
{

}

void
WorkerBee::reloadConfig ()
{
  Bee::reloadConfig ();
  // TODO load class attributes here
}

j::Value const&
WorkerBee::getConfig ()
{
  // TODO make this refer to superclass method
  return getAppConfig ()["simulation"]["bees"]["worker"];
}

void
WorkerBee::onState (State state, sf::Time dt)
{
  // TODO implement for states
}

void
WorkerBee::setFlower(const Vec2d& position)
{
  flower_location_ = position;
}

Vec2d
WorkerBee::getFlower() const
{
  return flower_location_;
}

State const
WorkerBee::IN_HIVE = createUid ();

State const
WorkerBee::COLLECT_POLLEN = createUid();
