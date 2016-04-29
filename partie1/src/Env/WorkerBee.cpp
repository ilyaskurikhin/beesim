/*
 * WorkerBee.cpp
 *
 *  Created on: Apr 28, 2016
 *      Author: ilya
 */

#include <Env/WorkerBee.hpp>

WorkerBee::WorkerBee (Hive* hive, const Vec2d& position)
: Bee (hive, position)
  {
    reloadConfig();
  }

WorkerBee::~WorkerBee ()
{

}

void
WorkerBee::reloadConfig ()
{
  Bee::reloadConfig();
  // TODO load class attributes here
}

j::Value const&
WorkerBee::getConfig()
{
  // TODO make this refer to superclass method
  return getAppConfig ()["simulation"]["bees"]["worker"];
}
