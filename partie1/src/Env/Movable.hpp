/*
 * Movable.hpp
 *
 *  Created on: May 13, 2016
 *      Author: ilya
 */

#ifndef MOVABLE_HPP_
#define MOVABLE_HPP_

#include <SFML/System.hpp>
#include <string>
#include <Env/Collider.hpp>

#include <Utility/Vec2d.hpp>
#include <Random/Random.hpp>


class Movable : public Collider
{
public:

  Movable(const Vec2d& position);

  Movable(const Vec2d& position, double radius);

  ~Movable();

  /**
   * @brief Set a target to move towards
   *
   * @param target graphic position.
   */
  void
  setMoveTarget(const Vec2d& target);

  /**
   * @brief Get the current move target
   *
   * @return graphic position of target.
   */
  const Vec2d&
  getMoveTarget() const;

  /**
   * @brief Move the bee towards a target defined by the type of bee.
   *
   * @param dt Time for the counter.
   */
  void
  targetMove(sf::Time dt);

  /**
   * @brief Move the bee randomly.
   *
   * Currently only implemented in ScoutBee.
   *
   * @param dt
   */
  void
  randomMove(sf::Time dt);

  virtual bool
  isMovablePosition(const Vec2d& position) const;


  const Vec2d&
  getMoveVec() const;

  void
  rotateMoveVec(double angle);

  /**
   * @brief Get current speed.
   *
   * @return speed
   */
  double
  getSpeed() const;

  void
  setSpeed(double speed);

  void
  setDelay(sf::Time delay);

  void
  setMoveVec(const Vec2d& move_vec);

private:

  Vec2d move_vec_;
  Vec2d move_target_;
  double speed_;

  double max_angle_;

  sf::Time avoidance_clock_;
  sf::Time delay_;

};


#endif /* MOVABLE_HPP_ */
