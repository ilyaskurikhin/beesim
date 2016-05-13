/*
 * Movable.cpp
 *
 *  Created on: May 13, 2016
 *      Author: ilya
 */

#include <Env/Movable.hpp>


#include <Env/Env.hpp>

Movable::Movable(const Vec2d& position)
: Collider(position)
{

}

Movable::Movable(const Vec2d& position, double radius)
: Collider(position, radius)
{

}

Movable::~Movable()
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
Movable::randomMove(sf::Time)
{
}

void
Movable::targetMove(sf::Time dt)
{
  Vec2d target(this->getMoveTarget());
  Vec2d direction(this->directionTo(target));

  direction = direction.normalised();

  if (avoidance_clock_ < sf::Time::Zero)
    {
      move_vec_ = direction * move_vec_.length();
    }
  else
    {
      avoidance_clock_ -= dt;
    }

  Vec2d move(move_vec_);
  move *= dt.asSeconds();

  this->Collider::move(move);

  if (!getAppEnv().isFlyable(this->getPosition()))
    {
      double angleB;
      if (bernoulli(0.5))
        {
          angleB = PI / 4;
        }
      else
        {
          angleB = -PI / 4;
        }
      move_vec_.rotate(angleB);
      avoidance_clock_ = delay_;

      this->Collider::move(-move);
    }
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


