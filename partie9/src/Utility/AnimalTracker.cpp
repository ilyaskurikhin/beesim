/*
 * BeeTracker.cpp
 *
 *  Created on: May 20, 2016
 *      Author: ilya
 */

#include <Env/Bee/Bee.hpp>
#include <Utility/AnimalTracker.hpp>

AnimalTracker::AnimalTracker() :
    tracked_position_(Vec2d(0, 0)), tracked_animal_(nullptr)
{
}

void
AnimalTracker::startTracking(Movable* animal)
{
  if (animal)
    tracked_animal_ = animal;
}

void
AnimalTracker::stopTracking()
{
  tracked_animal_ = nullptr;
}

bool
AnimalTracker::isTracking()
{
  if (tracked_animal_)
    return true;
  else
    return false;
}

const Vec2d&
AnimalTracker::getTrackedPosition()
{
  if (tracked_animal_)
    tracked_position_ = tracked_animal_->getPosition();
  return tracked_position_;
}
