/*
 * WorkerBee.cpp
 *
 *  Created on: Apr 28, 2016
 *      Author: ilya
 */

#include <Env/WorkerBee.hpp>

WorkerBee::WorkerBee (Hive* hive, const Vec2d& position,
		      std::vector<State> states) :
    Bee (hive, position, states), flower_location_(-1, -1), pollen_ (0.0)
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
  maxPollen_ = getConfig ()["max pollen capacity"].toDouble ();
  pollen_collecting_rate_ = getConfig ()["harvest rate"].toDouble ();
  energy_leave_hive_ = getConfig ()["energy"]["to leave hive"].toDouble ();
  pollen_transfer_rate_ = getConfig()["transfer rate"].toDouble();
  debug_text_size_ = getAppEnv().getTextSize();

}

j::Value const&
WorkerBee::getConfig ()
{
  return getAppConfig ()["simulation"]["bees"]["worker"];
}

void
WorkerBee::onState (State state, sf::Time dt)
{
  Vec2d empty(-1.0,-1.0);

  // first state
  if (state == IN_HIVE)
    {
      
      if (this->getPollen () != 0)
	{
	  transferPollen(dt);
    if (this->getPollen() < 0) 
    {
      setPollen(0);
    }
	  debug_status_ = "in_hive_leaving_pollen";
	}

  if ((this->getPollen () == 0))
	{
	  if (this->getEnergy () < energy_leave_hive_)
	    {
	      debug_status_ = "in_hive_eating";
	      this->eatFromHive (dt);
	    }
	  else if (flower_location_ != empty)
	    {
	      debug_status_ = "in_hive_leaving";
	      this->setMoveTarget (flower_location_);
	      this->nextState ();
	    }
	  else
	    {
      debug_status_ = "in_hive_no_flower";	    }
    }
    }

  // second state
  else if (state == TO_FLOWER)
  {
      debug_status_ = "to_flower";

      if (this->getCollider ().isPointInside (flower_location_))
      { 
        if (getAppEnv ().getCollidingFlower (this->getCollider ()) != nullptr)
        {
          this->nextState ();
        }
      else
      {
        //if the flower doesn't exist anymore, goes two states further
        this->nextState ();
        this->nextState ();
      }
    }
  }

  // third state
  else if (state == COLLECT_POLLEN)
    {
      debug_status_ = "collecting_pollen";
      move_target_ = empty;
      if (this->getPollen () < maxPollen_
	  && getAppEnv ().getCollidingFlower (this->getCollider ()) != nullptr)
	{
	  eatPollen (dt);
	}
      else
	{
	  this->nextState ();
	}
    }
  else if (state == RETURN_HIVE)
    {
      debug_status_ = "back_to_hive";
      this->setMoveTarget (this->getHive ()->getPosition ());
      if (this->getHive ()->isColliderInside (this->getCollider ()))
	{
	  this->nextState ();
	}
    }
}

void
WorkerBee::onEnterState (State state)
{
  if (state == IN_HIVE or state == COLLECT_POLLEN)
    {
      move_state_ = AT_REST;
    }
  else if (state == TO_FLOWER or state == RETURN_HIVE)
    {
      move_state_ = TARGET;
    }
}

void
WorkerBee::setFlower (const Vec2d& position)
{
  flower_location_ = position;
}

Vec2d
WorkerBee::getFlower () const
{
  return flower_location_;
}

double
WorkerBee::getPollen () const
{
  return pollen_;
}

void
WorkerBee::transferPollen (sf::Time dt)
{
  this->getHive ()->dropPollen (pollen_transfer_rate_ * dt.asSeconds ());
  setPollen (this->getPollen () - pollen_transfer_rate_ * dt.asSeconds ());
}

void
WorkerBee::eatPollen (sf::Time dt)
{
  setPollen (this->getPollen () + pollen_collecting_rate_ * dt.asSeconds ());
}

void
WorkerBee::learnFlowerLocation (const Vec2d& flowerPosition)
{
  flower_location_ = flowerPosition;
}

void
WorkerBee::setPollen (double amount)
{
  pollen_ = amount;
}

void
WorkerBee::drawOn (sf::RenderTarget& target) const
{
  Bee::drawOn (target);
  if (isDebugOn ())
    {
      std::string valueString;
      sf::Color color (sf::Color::Magenta);
      Vec2d position;

      position.x = this->getPosition ().x;
      position.y = this->getPosition ().y + debug_text_size_;

      valueString = "Worker: energy " + to_nice_string (this->getEnergy ());
      sf::Text text = buildText (valueString, position, getAppFont (),
				 debug_text_size_, color);
      target.draw (text);

      std::string statusString (debug_status_);
      position.y = position.y + debug_text_size_;
      sf::Text status = buildText (statusString, position, getAppFont (),
				   debug_text_size_, color);
      target.draw (status);
    }
}


void
WorkerBee::targetMove(sf::Time dt)
{
  Bee::targetMove(dt);
}

State const WorkerBee::IN_HIVE = createUid ();
State const WorkerBee::TO_FLOWER = createUid ();
State const WorkerBee::COLLECT_POLLEN = createUid ();
State const WorkerBee::RETURN_HIVE = createUid ();
