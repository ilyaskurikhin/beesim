/*
 * Movable.cpp
 *
 *  Created on: May 13, 2016
 *      Author: ilya
 */

#include <Env/Movable.hpp>

#include <Env/Env.hpp>

Movable::Movable(const Vec2d& position) :
    Collider(position),
    move_state_(MoveState::AT_REST)
{

}

Movable::Movable(const Vec2d& position, double radius) :
    Collider(position, radius),
    move_state_(MoveState::AT_REST)
{

}

void
Movable::setMoveTarget(const Vec2d& position)
{
  move_target_ = position;
}

const Vec2d&
Movable::getMoveTarget() const
{
  return move_target_;
}

double
Movable::getSpeed() const
{
  return speed_;
}

void
Movable::randomMove(sf::Time dt)
{
  // make the move vector rotate randomly
  if (bernoulli(rotation_probability_))
    {
      double angleA(uniform(-max_angle_, max_angle_));
      this->rotateMoveVec(angleA);
    }

  Vec2d move(this->getMoveVec());

  move *= dt.asSeconds();

  this->Collider::move(move);

  // if walk to the new position impossible make move vector rotate again
  if (!isMovablePosition(this->getPosition()))
    {
      double angleB;
      if (bernoulli(0.5))
        angleB = PI / 4;
      else
        angleB = -PI / 4;

      this->rotateMoveVec(angleB);
      this->Collider::move(-move);
    }
}

void
Movable::targetMove(sf::Time dt)
{
  Vec2d target(this->getMoveTarget());
  Vec2d direction(this->directionTo(target));

  // normalise the direction
  direction = direction.normalised();

  if (avoidance_clock_ < sf::Time::Zero)
    move_vec_ = direction * move_vec_.length();
  else
    avoidance_clock_ -= dt;

  Vec2d move(move_vec_);
  move *= dt.asSeconds();

  this->Collider::move(move);

  // if walk to the new position impossible make move vector rotate
  if (!isMovablePosition(this->getPosition()))
    {
      double angleB;
      if (bernoulli(0.5))
        angleB = PI / 4;
      else
        angleB = -PI / 4;
        
      move_vec_.rotate(angleB);
      avoidance_clock_ = delay_;

      this->Collider::move(-move);
    }
}

bool
Movable::isMovablePosition(const Vec2d&) const
{
  return true;
}

const Vec2d&
Movable::getMoveVec() const
{
  return move_vec_;
}

void
Movable::rotateMoveVec(double angle)
{
  move_vec_.rotate(angle);
}

void
Movable::setSpeed(double speed)
{
  speed_ = speed;
}

void
Movable::setDelay(sf::Time delay)
{
  delay_ = delay;
}

void
Movable::setMoveVec(const Vec2d& move_vec)
{
  move_vec_ = move_vec;
}

void
Movable::setRotationProbability(double rotationProbability)
{
  rotation_probability_ = rotationProbability;
}

void
Movable::setMaxAngle(double maxAngle)
{
  max_angle_ = maxAngle;
}

sf::Time
Movable::getAvoidanceClock() const
{
  return avoidance_clock_;
}

void
Movable::setAvoidanceClock(sf::Time avoidanceClock)
{
  avoidance_clock_ = avoidanceClock;
}

sf::Time
Movable::getDelay() const
{
  return delay_;
}

double
Movable::getMaxAngle() const
{
  return max_angle_;
}

double
Movable::getRotationProbability() const
{
  return rotation_probability_;
}

const MoveState&
Movable::getMoveState() const
{
  return move_state_;
}

void
Movable::setMoveState(const MoveState& moveState)
{
  move_state_ = moveState;
}
