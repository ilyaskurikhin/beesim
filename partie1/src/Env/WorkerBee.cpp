/*
 * WorkerBee.cpp
 *
 *  Created on: Apr 28, 2016
 *      Author: ilya
 */

#include <Env/WorkerBee.hpp>

WorkerBee::WorkerBee(Hive* hive, const Vec2d& position) :
    Bee(hive, position, std::vector<State> ({IN_HIVE, TO_FLOWER, COLLECT_POLLEN, RETURN_HIVE})),
    flower_location_(-1, -1), pollen_(0.0)
{
  reloadConfig();
  loadTexture();
}

WorkerBee::~WorkerBee()
{

}

void
WorkerBee::reloadConfig()
{
  Bee::reloadConfig();
  max_pollen_ = getConfig()["max pollen capacity"].toDouble();
  pollen_collecting_rate_ = getConfig()["harvest rate"].toDouble();
  energy_leave_hive_ = getConfig()["energy"]["to leave hive"].toDouble();
  pollen_transfer_rate_ = getConfig()["transfer rate"].toDouble();

}

j::Value const&
WorkerBee::getConfig()
{
  return getAppConfig()["simulation"]["bees"]["worker"];
}

void
WorkerBee::onState(State state, sf::Time dt)
{
  Vec2d empty(-1.0, -1.0);

  // first state
  if (state == IN_HIVE)
    {

      if (this->getPollen() > 0)
        {
          transferPollen(dt);
          flower_location_ = empty;
          this->setDebugStatus("in_hive_leaving_pollen");
        }
      else
        {
          if (this->getEnergy() < energy_leave_hive_)
            {
              this->setDebugStatus("in_hive_eating");
              this->eatFromHive(dt);
            }
          else if (flower_location_ != empty)
            {
              this->setDebugStatus("in_hive_leaving");
              this->setMoveTarget(flower_location_);
              this->nextState();
            }
          else
            {
              this->setDebugStatus("in_hive_no_flower");
            }
        }
    }

  // second state
  else if (state == TO_FLOWER)
    {
      this->setDebugStatus("to_flower");

      if (this->getCollider().isPointInside(flower_location_))
        {
          if (getAppEnv().getCollidingFlower(this->getCollider()) == nullptr)
            {
              // skip collection if no flower present
              this->nextState();
            }
          this->nextState();
        }
    }

  // third state
  else if (state == COLLECT_POLLEN)
    {
      this->setDebugStatus("collecting_pollen");
      this->setMoveTarget(empty);
      Flower* flower(getAppEnv().getCollidingFlower(this->getCollider()));
      if ((this->getPollen() < max_pollen_) && (flower != nullptr))
        {
          eatPollen(flower, dt);
        }
      else
        {
          this->nextState();
        }
    }
  else if (state == RETURN_HIVE)
    {
      this->setDebugStatus("back_to_hive");
      this->setMoveTarget(this->getHive()->getPosition());
      if (this->getHive()->isColliderInside(this->getCollider()))
        {
          this->nextState();
        }
    }
}

void
WorkerBee::onEnterState(State state)
{
  if (state == IN_HIVE or state == COLLECT_POLLEN)
    {
      this->setMoveStateAT_REST();
    }
  else if (state == TO_FLOWER or state == RETURN_HIVE)
    {
      this->setMoveStateTARGET();
    }
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
  double pollen(takePollen(pollen_transfer_rate_ * dt.asSeconds()));
  this->getHive()->dropPollen(pollen);
  return pollen;
}

void
WorkerBee::eatPollen(Flower* flower, sf::Time dt)
{
  double pollen(pollen_collecting_rate_ * dt.asSeconds());
  if (pollen + this->getPollen() > max_pollen_)
    pollen = max_pollen_ - this->getPollen();

  setPollen(this->getPollen() + flower->takePollen(pollen));
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
WorkerBee::drawOn(sf::RenderTarget& target) const
{
  Bee::drawOn(target);
  if (isDebugOn())
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
}

bool
WorkerBee::isScout() const
{
  return false;
}

bool
WorkerBee::isWorker() const
{
  return true;
}

bool
WorkerBee::isInHive() const
{
  if (this->getState() == IN_HIVE)
    {
      return true;
    }
  else
    {
      return false;
    }
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

State const WorkerBee::TO_FLOWER = createUid();
State const WorkerBee::COLLECT_POLLEN = createUid();
State const WorkerBee::IN_HIVE = createUid();
State const WorkerBee::RETURN_HIVE = createUid();
