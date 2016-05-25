/*
 * WorkerBee.cpp
 *
 *  Created on: Apr 28, 2016
 *      Author: ilya
 */

#include <Env/Bee/WorkerBee.hpp>

WorkerBee::WorkerBee(Hive* hive, const Vec2d& position) :
    Bee(hive, position, std::vector<State>(
      { IN_HIVE, TO_FLOWER, COLLECT_POLLEN, RETURN_HIVE }), BeeType::Worker), flower_location_(
        -1, -1), pollen_(0.0)
{
  // call to reloadConfig to intialise the attributes from config
  reloadConfig();
  loadTexture();
}

WorkerBee::~WorkerBee()
{

}

j::Value const&
WorkerBee::getConfig() const
{
  return getAppConfig()["simulation"]["bees"]["worker"];
}

void
WorkerBee::reloadConfig()
{
  // call to Bee::reloadConfig to load the attributes common to bees
  Bee::reloadConfig();
  max_pollen_ = getConfig()["max pollen capacity"].toDouble();
  pollen_collecting_rate_ = getConfig()["harvest rate"].toDouble();
  energy_leave_hive_ = getConfig()["energy"]["to leave hive"].toDouble();
  energy_collect_pollen_ = getConfig()["energy"]["to collect pollen"].toDouble();
  pollen_transfer_rate_ = getConfig()["transfer rate"].toDouble();

}

void
WorkerBee::onState(State state, sf::Time dt)
{
  Vec2d empty(-1.0, -1.0);

  // first state
  if (state == IN_HIVE)
    {
      // if bee has pollen transfer it to hive
      if (getPollen() > 0)
        {
          transferPollen(dt);
          flower_location_ = empty;
          setDebugStatus("in_hive_leaving_pollen");
        }
      else
        {
          // if bee has not enough energy to leave hive, eat its nectar
          if (getEnergy() < energy_leave_hive_
              && getHive().getNectar() > 0)
            {
              setDebugStatus("in_hive_eating");
              eatFromHive(dt);
            }
          // if there is a flower in memory and enough energy, target move
          // to this flower
          else if (flower_location_ != empty)
            {
              setDebugStatus("in_hive_leaving");
              setMoveTarget(flower_location_);
              // change state to to flower
              nextState();
            }
          else
            {
              setDebugStatus("in_hive_no_flower");
            }
        }
    }

  // second state
  else if (state == TO_FLOWER)
    {
      setDebugStatus("to_flower");

      if (getEnergy() < energy_collect_pollen_)
        {
          nextState();
          nextState();
        }


      // if bee inside the flower
      if (getCollider().isPointInside(flower_location_))
        {
          if (getAppEnv().getCollidingFlower(getCollider()) == nullptr)
            {
              // skip collection if no flower present
              nextState();
            }
          // change state to collect pollen
          nextState();
        }
    }

  // third state
  else if (state == COLLECT_POLLEN)
    {
      setDebugStatus("collecting_pollen");
      setMoveTarget(empty);
      // if there is a flower at flower location and it has pollen and 
      // bee has not enough pollen, eat pollen from flower
      Flower* flower(getAppEnv().getCollidingFlower(getCollider()));
      if ((getPollen() < max_pollen_)
	  && (flower != nullptr)
	  && (flower->getPollen() > 0))
        {
          eatPollen(flower, dt);
        }
      else
        {
          // else skip collection
          nextState();
        }
    }
  else if (state == RETURN_HIVE)
    {
      setDebugStatus("back_to_hive");
      setMoveTarget(getHive().getPosition());
      // if bee is in hive change state to in hive
      if (getHive().isColliderInside(getCollider()))
        {
          nextState();
        }
    }
}

void
WorkerBee::onEnterState(State state)
{
  if (state == IN_HIVE or state == COLLECT_POLLEN)
      setMoveState(MoveState::AT_REST);
      
  else if (state == TO_FLOWER or state == RETURN_HIVE)
      setMoveState(MoveState::TARGET);
}

void
WorkerBee::setFlower(const Vec2d& position)
{
  flower_location_ = position;
}

const Vec2d&
WorkerBee::getFlower() const
{
  return flower_location_;
}

double
WorkerBee::getPollen() const
{
  return pollen_;
}

double
WorkerBee::takePollen(double pollen)
{
  double taken(0);

  if (pollen_ < pollen)
    {
      taken = pollen_;
      pollen_ = 0;
    }
  else
    {
      taken = pollen;
      pollen_ -= pollen;
    }
  return taken;
}

double
WorkerBee::transferPollen(sf::Time dt)
{
  // pollen transfered is taken from bee at a certain rate and dropped in hive
  double pollen(takePollen(pollen_transfer_rate_ * dt.asSeconds()));
  this->getHive().dropPollen(pollen);
  return pollen;
}

double
WorkerBee::eatPollen(Flower* flower, sf::Time dt)
{
  double pollen(pollen_collecting_rate_ * dt.asSeconds());

  // if bee has too much pollen sets pollen to max pollen - its pollen
  if (pollen + this->getPollen() > max_pollen_)
    pollen = max_pollen_ - this->getPollen();
  // if no flower no pollen
  if (!flower)
    pollen = 0;
  // sets pollen to its pollen + the pollen taken from flower
  else
    setPollen(this->getPollen() + flower->takePollen(pollen));
  return pollen;
}

void
WorkerBee::learnFlowerLocation(const Vec2d& flowerPosition)
{
  flower_location_ = flowerPosition;
}

void
WorkerBee::setPollen(double amount)
{
  pollen_ = amount;
}

void
WorkerBee::drawDebug(sf::RenderTarget& target) const
{
      std::string valueString;
      sf::Color color(sf::Color::Magenta);
      Vec2d position;
      double text_size(getAppEnv().getTextSize());

      position.x = this->getPosition().x;
      position.y = this->getPosition().y + text_size;

      valueString = "Worker: energy " + to_nice_string(this->getEnergy());
      sf::Text text = buildText(valueString, position, getAppFont(), text_size,
                                color);
      target.draw(text);

      position.y = position.y + text_size;
      sf::Text status = buildText(this->getDebugStatus(), position,
                                  getAppFont(), text_size, color);
      target.draw(status);
}

void
WorkerBee::interact(Bee* other)
{
  other->interactWith(this);
}

void
WorkerBee::interactWith(ScoutBee*)
{

}

void
WorkerBee::interactWith(WorkerBee*)
{
}

const Vec2d&
WorkerBee::getFlowerLocation() const
{
  return flower_location_;
}

void
WorkerBee::setFlowerLocation(const Vec2d& flowerLocation)
{
  flower_location_ = flowerLocation;
}

State const WorkerBee::TO_FLOWER = createUid();
State const WorkerBee::COLLECT_POLLEN = createUid();
State const WorkerBee::IN_HIVE = createUid();
State const WorkerBee::RETURN_HIVE = createUid();
