/*
 * BeeTracker.hpp
 *
 *  Created on: May 20, 2016
 *      Author: ilya
 */

#ifndef UTILITY_BEETRACKER_HPP_
#define UTILITY_BEETRACKER_HPP_

#include <Interface/UpdatableInterface.hpp>

#include <Utility/Vec2d.hpp>

class Movable;

class AnimalTracker
{
public:
  AnimalTracker();

  AnimalTracker(const AnimalTracker&) = delete;

  ~AnimalTracker() = default;

  AnimalTracker
  operator=(const AnimalTracker&) = delete;

  void
  startTracking(Movable* animal);

  void
  stopTracking();

  bool
  isTracking();

  const Vec2d&
  getTrackedPosition();

private:

  Vec2d tracked_position_;
  Movable* tracked_animal_;

};

#endif /* UTILITY_BEETRACKER_HPP_ */
