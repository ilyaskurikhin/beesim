/*
 * QueenBee.cpp
 *
 *  Created on: May 25, 2016
 *      Author: ilya
 */

#include <Env/Bee/QueenBee.hpp>

QueenBee::QueenBee(Hive* hive, const Vec2d& position)
: Bee(hive, position, std::vector<State>({IN_HIVE, MIGRATING, RETURN_HIVE}), BeeType::Queen)
{
  reloadConfig();
  loadTexture();
}

QueenBee::~QueenBee()
{
}

void
QueenBee::reloadConfig()
{
  Bee::reloadConfig();

  energy_migrate_ = getConfig()["energy"]["to migrate"].toDouble();
  energy_in_hive_ = getConfig()["energy"]["in hive"].toDouble();
  energy_create_hive_ = getConfig()["energy"]["create hive"].toDouble();
  migration_distance_ = getConfig()["migration distance"].toDouble();

  reproduction_probability_ = getConfig()["reproduction"]["scout probability"].toDouble();

  this->setRotationProbability(
      getConfig()["moving behaviour"]["random"]["rotation probability"].toDouble());
  this->setMaxAngle(
      getConfig()["moving behaviour"]["random"]["rotation angle max"].toDouble());
}

const j::Value&
QueenBee::getConfig() const
{
  return getAppConfig()["simulation"]["bees"]["queen"];
}

void
QueenBee::drawDebug(sf::RenderTarget& target) const
{
  std::string valueString;
  sf::Color color(sf::Color::Yellow);
  Vec2d position;
  double text_size(getAppEnv().getTextSize());

  position = this->getPosition();
  position.y -= text_size;

  valueString = "Queen: energy " + to_nice_string(this->getEnergy());
  sf::Text text = buildText(valueString, position, getAppFont(), text_size,
                              color);
  target.draw(text);

  position.y += text_size;
  sf::Text status = buildText(this->getDebugStatus(), position,
                                  getAppFont(), text_size, color);
  target.draw(status);
}

void
QueenBee::onState(State state, sf::Time dt)
{
  if (state == IN_HIVE)
    {
      setDebugStatus("in_hive");
      if (getEnergy() < energy_in_hive_)
        {
          setDebugStatus("in_hive_eating");
          eatFromHive(dt);
        }
      else if (getHive().canMigrate())
        {
          if (getEnergy() < energy_migrate_)
            {
              eatFromHive(dt);
              setDebugStatus("in_hive_leaving");
            }
          else
            {
              giveBirthTo(BeeType::Queen);
              nextState();
            }
        }

      if (getHive().canReproduce())
        {
            // add randomly a workerbee or a scoutbee
            if (bernoulli(reproduction_probability_))
              giveBirthTo(BeeType::Scout);
            else
              giveBirthTo(BeeType::Worker);
        }
    }

  if (state == MIGRATING)
    {
      setDebugStatus("migrating");
      if (distanceTo(getHive()) > migration_distance_
          && getEnergy() > energy_create_hive_)
        {
          if (getAppEnv().addHiveAt(getPosition()))
            {
              getHive().removeBee(this);
              setHive(getAppEnv().getCollidingHive(getPosition()));
              getHive().addBee(this);
              setEnergy(getEnergy() - energy_create_hive_);
              nextState();
            }
        }
      else if (getEnergy() < energy_create_hive_)
        {
          nextState();
        }
    }

  if (state == RETURN_HIVE)
    {
      setDebugStatus("return_hive");
      if (getAppEnv().getCollidingHive(getPosition()) == &getHive())
        {
          nextState();
        }
    }
}

void
QueenBee::onEnterState(State state)
{
  if (state == IN_HIVE)
    {
      setMoveState(MoveState::AT_REST);
    }
  else if (state == MIGRATING)
    {
      setMoveState(MoveState::RANDOM);
    }
  else if (state == RETURN_HIVE)
    {
      setMoveTarget(getHive().getPosition());
      setMoveState(MoveState::TARGET);
    }
}

void
QueenBee::interact(Bee*)
{
}

void
QueenBee::interactWith(ScoutBee*)
{
}

void
QueenBee::interactWith(WorkerBee*)
{
}

Bee*
QueenBee::giveBirthTo(BeeType beeType) const
{
  return getHive().addBee(beeType);
}

State const QueenBee::IN_HIVE = createUid();
State const QueenBee::MIGRATING = createUid();
State const QueenBee::RETURN_HIVE = createUid();
