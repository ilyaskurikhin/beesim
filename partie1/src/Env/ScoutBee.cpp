/**
 * @file ScoutBee.cpp
 * @brief 
 * @author Ilya Skurikhina
 * @author Julia Besson
 * @version p5.1
 * @date 2016-05-05
 */

#include <Env/ScoutBee.hpp>

ScoutBee::ScoutBee (Hive* hive, const Vec2d& position, std::vector<State> states) :
    Bee (hive, position, states)
{
  logEvent ("ScoutBee", "new constructed");
  reloadConfig ();
  loadTexture ();
}

ScoutBee::~ScoutBee ()
{
  
}

void
ScoutBee::reloadConfig ()
{
  Bee::reloadConfig ();

  // TODO load class attributes here
  rotation_probability_ =
      getConfig ()["moving behaviour"]["random"]["rotation probability"].toDouble ();
  max_angle_ =
      getConfig ()["moving behaviour"]["random"]["rotation angle max"].toDouble ();

  energy_seek_flowers_ = getConfig ()["energy"]["to seek flowers"].toDouble ();
  max_sharing_ = getConfig ()["sharing"]["max"].toDouble ();
}

void
ScoutBee::randomMove (sf::Time dt)
{
  Vec2d position (position_);
  if (bernoulli (rotation_probability_))
    {
      double angleA (uniform (-max_angle_, max_angle_));
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
}

j::Value const&
ScoutBee::getConfig ()
{
  // TODO make this refer to superclass method
  return getAppConfig ()["simulation"]["bees"]["scout"];
}

void
ScoutBee::onState (State state, sf::Time dt)
{
  switch (this->getState()) 
    {
      case IN_HIVE: 
        move_state_ = AT_REST
        if (energy_ < energy_leave_hive_)
          {
            energy =+ hive_->takeNectar(energy_leave_hive_ - energy_);
          }
        else if (flower_location_ == (-1,-1));
          {
            state_ = SEARCH_FLOWER;
            onEnterState();
          }
        break;
      case SEARCH_FLOWER : 
        move_state_ = RANDOM;
        break;
      case RETURN_HIVE: 
        move_state_ = TARGET;
        flower_location_.clear();
        //il faut fixer la ruche comme cible à avoir
        break;
    }
}

void 
ScoutBee::onEnterState (State state)
{
}

void 
ScoutBee::targetMove()
{
  // TODO implement for states
}

State const
ScoutBee::IN_HIVE = createUid ();

State const
ScoutBee::SEARCH_FLOWER = createUid ();

State const
ScoutBee::RETURN_HIVE = createUid ();

