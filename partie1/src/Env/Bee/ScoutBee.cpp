/**
 * @file ScoutBee.cpp
 * @brief 
 * @author Ilya Skurikhina
 * @author Julia Besson
 * @version p5.1
 * @date 2016-05-05
 */

#include <Env/Bee/ScoutBee.hpp>

ScoutBee::ScoutBee(Hive* hive, const Vec2d& position) :
    Bee(hive, position, std::vector<State>(
      { IN_HIVE, SEARCH_FLOWER, RETURN_HIVE })), flower_location_(-1, -1), number_times_shared_(
        -1)
{
  // call to reloadConfig to intialise the attributes from config
  reloadConfig();
  loadTexture();
}

ScoutBee::~ScoutBee()
{

}


void
ScoutBee::reloadConfig()
{
  // call to Bee::reloadConfig to load the attributes common to bees
  Bee::reloadConfig();

  energy_seek_flowers_ = getConfig()["energy"]["to seek flowers"].toDouble();
  energy_leave_hive_ = getConfig()["energy"]["to leave hive"].toDouble();

  max_sharing_ = getConfig()["sharing"]["max"].toDouble();

  this->setRotationProbability(
      getConfig()["moving behaviour"]["random"]["rotation probability"].toDouble());
  this->setMaxAngle(
      getConfig()["moving behaviour"]["random"]["rotation angle max"].toDouble());
}

j::Value const&
ScoutBee::getConfig()
{
  return getAppConfig()["simulation"]["bees"]["scout"];
}

void
ScoutBee::onState(State state, sf::Time dt)
{
  // create vector to compare to for empty status
  Vec2d empty;
  empty.x = -1.0;
  empty.y = -1.0;

  // first state
  if (state == IN_HIVE)
    {
      // if there is a flower in memory share its position
      if (flower_location_ != empty)
        {
          this->getHive()->interactingBees();
          flower_location_ = empty;
          std::string status = "in_hive_sharing"
              + std::to_string(number_times_shared_);
          this->setDebugStatus(status);
          number_times_shared_ = 1;
        }
      
      // if there is not enough energy to leave hive eat nectar from it
      if (this->getEnergy() < energy_leave_hive_)
        {
          this->setDebugStatus("in_hive_eating");
          this->eatFromHive(dt);
        }
      // if there is no flower in memory and there is enough energy to 
      // leave hive go search flower
      else if (flower_location_ == empty)
        {
          this->setDebugStatus("in_hive_leaving");
          // change state to search flower
          this->nextState();
        }
    }

  // second state
  else if (state == SEARCH_FLOWER)
    {
      Flower* flower = this->findVisibleFlower();
      this->setDebugStatus("seeking_flower");
      
      // if there is a flower in visibility range and there is enough 
      // energy to leave search flower, remember the flower location
      if (this->getEnergy() > energy_seek_flowers_ && flower != nullptr)
        {
          flower_location_ = flower->getPosition();
          number_times_shared_ = 0;
          // change state to return hive
          this->nextState();
        }
    }

  // third state
  else if (state == RETURN_HIVE)
    {
      this->setDebugStatus("back_to_hive");
      this->setMoveTarget(this->getHive()->getPosition());
      // if the bee is in the hive change state to in hive
      if (this->getHive()->isColliderInside(this->getCollider()))
        {
          this->nextState();
        }
    }
}

void
ScoutBee::onEnterState(State state)
{
  if (state == IN_HIVE)
    this->setMoveState(MoveState::AT_REST);
    
  else if (state == SEARCH_FLOWER)
    this->setMoveState(MoveState::RANDOM);

  else if (state == RETURN_HIVE)
    this->setMoveState(MoveState::TARGET);
}

bool
ScoutBee::isScout() const
{
  return true;
}

bool
ScoutBee::isWorker() const
{
  return false;
}

bool
ScoutBee::isInHive() const
{
  if (this->getState() == IN_HIVE)
    return true;
  else
    return false;
}

void
ScoutBee::interact(Bee* other)
{
  other->interactWith(this);
}

void
ScoutBee::interactWith(ScoutBee*)
{
}

void
ScoutBee::interactWith(WorkerBee* working)
{
  Vec2d empty;
  empty.x = -1;
  empty.y = -1;
  
  // if there is a flower in memory and it was not share too much
  // give the flower position to a working bee

  if ((flower_location_ != empty) && number_times_shared_ < max_sharing_)
    {
      working->learnFlowerLocation(this->flower_location_);
      number_times_shared_ += 1;
    }

}

const Vec2d&
ScoutBee::getFlowerLocation() const
{
  return flower_location_;
}

void
ScoutBee::setFlowerLocation(const Vec2d& flowerLocation)
{
  flower_location_ = flowerLocation;
}

int
ScoutBee::getNumberTimesShared() const
{
  return number_times_shared_;
}


void
ScoutBee::drawDebug(sf::RenderTarget& target) const
{

  std::string valueString;
  sf::Color color(sf::Color::Yellow);
  Vec2d position;
  double text_size(getAppEnv().getTextSize());
  
  position = this->getPosition();
  position.y += text_size;

  valueString = "Scout: energy " + to_nice_string(this->getEnergy());
  sf::Text text = buildText(valueString, position, getAppFont(), text_size,
                              color);
  target.draw(text);

  position.y += text_size;
  sf::Text status = buildText(this->getDebugStatus(), position,
                                  getAppFont(), text_size, color);
  target.draw(status);

}
State const ScoutBee::IN_HIVE = createUid();
State const ScoutBee::SEARCH_FLOWER = createUid();
State const ScoutBee::RETURN_HIVE = createUid();

