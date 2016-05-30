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
  // call to reloadConfig to intialise the attributes from config file

  reloadConfig();
  loadTexture();
}

QueenBee::~QueenBee()
{
}

void
QueenBee::reloadConfig()
{
  // call to Bee::reloadConfig to load the attributes common to bees
  Bee::reloadConfig();

  energy_migrate_ = getConfig()["energy"]["to migrate"].toDouble();
  energy_in_hive_ = getConfig()["energy"]["in hive"].toDouble();
  energy_create_hive_ = getConfig()["energy"]["create hive"].toDouble();
  migration_distance_ = getConfig()["migration distance"].toDouble();

  nectar_transfer_rate_ = getConfig()["energy"]["nectar transfer rate"].toDouble();
  nectar_threshold_ =
      getAppConfig()["simulation"]["hive"]["reproduction"]["nectar threshold"].toDouble();

  reproduction_probability_ = getConfig()["reproduction"]["scout probability"].toDouble();

  max_queens_ = getAppConfig()["simulation"]["hive"]["max queens"].toInt();

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
      // if queenbee is in hive and has not enough energy, eats nectar
      if (getEnergy() < energy_in_hive_)
        {
          setDebugStatus("in_hive_eating");
          eatFromHive(dt);
        }
      else if (getHive().canMigrate())
        {
          // if energy to migrate is too low, bee eats nectar 
          if (getEnergy() < energy_migrate_)
            {
              eatFromHive(dt);
              setDebugStatus("in_hive_leaving");
            }
          // else give birth to a new queen and migrate
          else
            {
              if (getHive().getNumBees(BeeType::Queen) < max_queens_)
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
      // if energy is more than a certain thresol and hive has not enough
      // nectar, transfer nectar to hive
      else if (getEnergy() > energy_in_hive_
              && getHive().getNectar() < nectar_threshold_)
        {
          transferNectar(dt);
        }
    }

  if (state == MIGRATING)
    {
      setDebugStatus("migrating");
      if (distanceTo(getHive()) > migration_distance_
          && getEnergy() > energy_create_hive_)
        {
          // if the bee is far enough from hive and queen has enough
          // energy to create new hive and it can be created at position
          if (getAppEnv().addHiveAt(getPosition()))
            {
              setMoveState(MoveState::AT_REST);
              // remove the queen from hive
              getHive().removeBee(this);
              // set the hive of queen as the hive newly created
              setHive(getAppEnv().getCollidingHive(getPosition()));
              // add queen as a member of the hive
              getHive().addBee(this);
              // queen energy decrease of the energy used to create hive
              setEnergy(getEnergy() - energy_create_hive_);
              
              // queen changes state into return hive
              nextState();
            }
        }
      // if there is not enough energy to create hive return to hive
      // skip to next state
      else if (getEnergy() < energy_create_hive_)
        {
          nextState();
        }
    }

  if (state == RETURN_HIVE)
    {
      setDebugStatus("return_hive");
      // if the queen is in hive skip to next state
      if (getHive().isColliderInside(getCollider()))
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

double
QueenBee::transferNectar(sf::Time dt)
{
  // pollen transfered is taken from bee at a certain rate and dropped in hive
  double pollen(nectar_transfer_rate_ * dt.asSeconds());
  this->getHive().dropPollen(pollen);
  setEnergy(getEnergy() - pollen);
  return pollen;
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
