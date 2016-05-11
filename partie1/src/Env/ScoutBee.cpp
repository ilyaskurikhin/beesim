/**
 * @file ScoutBee.cpp
 * @brief 
 * @author Ilya Skurikhina
 * @author Julia Besson
 * @version p5.1
 * @date 2016-05-05
 */

#include <Env/ScoutBee.hpp>

ScoutBee::ScoutBee(Hive* hive, const Vec2d& position, std::vector<State> states) :
    Bee(hive, position, states), flower_location_(-1, -1), number_times_shared_(
        -1)
{
  reloadConfig();
  loadTexture();
}

ScoutBee::~ScoutBee()
{

}

void
ScoutBee::drawOn(sf::RenderTarget& target) const
{
  Bee::drawOn(target);
  if (isDebugOn())
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
}

void
ScoutBee::reloadConfig()
{
  Bee::reloadConfig();

  energy_seek_flowers_ = getConfig()["energy"]["to seek flowers"].toDouble();
  energy_leave_hive_ = getConfig()["energy"]["to leave hive"].toDouble();

  max_sharing_ = getConfig()["sharing"]["max"].toDouble();

  rotation_probability_ =
      getConfig()["moving behaviour"]["random"]["rotation probability"].toDouble();
  max_angle_ =
      getConfig()["moving behaviour"]["random"]["rotation angle max"].toDouble();
}

void
ScoutBee::randomMove(sf::Time dt)
{
  if (bernoulli(rotation_probability_))
    {
      double angleA(uniform(-max_angle_, max_angle_));
      this->rotateMoveVec(angleA);
    }

  Vec2d move(this->getMoveVec());

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

      this->rotateMoveVec(angleB);
      this->Collider::move(-move);
    }
}

j::Value const&
ScoutBee::getConfig()
{
  // TODO make this refer to superclass method
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
      if (flower_location_ != empty)
        {
          WorkerBee* worker = this->getHive()->getWorker();
          if (worker)
            {
              worker->setFlower(flower_location_);
              flower_location_ = empty;
              std::string status = "in_hive_sharing"
                  + std::to_string(number_times_shared_);
              this->setDebugStatus(status);
              number_times_shared_ = -1;
            }
        }

      if (this->getEnergy() < energy_leave_hive_)
        {
          this->setDebugStatus("in_hive_eating");
          this->eatFromHive(dt);
        }
      else if (flower_location_ == empty)
        {
          this->setDebugStatus("in_hive_leaving");
          this->nextState();
        }
    }

  // second state
  else if (state == SEARCH_FLOWER)
    {
      if (this->getEnergy() > energy_seek_flowers_)
        {
          this->setDebugStatus("seeking_flower");
          Flower* flower = this->findVisibleFlower();
          if (flower != nullptr)
            {
              flower_location_ = flower->getPosition();
              number_times_shared_ = 0;
              this->nextState();
            }
        }
      else
        {
          this->nextState();
        }
    }

  // third state
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
ScoutBee::onEnterState(State state)
{
  if (state == IN_HIVE)
    {
      this->setMoveStateAT_REST();
    }
  else if (state == SEARCH_FLOWER)
    {
      this->setMoveStateRANDOM();
    }
  else if (state == RETURN_HIVE)
    {
      this->setMoveStateTARGET();
    }
}

void
ScoutBee::targetMove(sf::Time dt)
{
  Bee::targetMove(dt);
}

State const ScoutBee::IN_HIVE = createUid();

State const ScoutBee::SEARCH_FLOWER = createUid();

State const ScoutBee::RETURN_HIVE = createUid();

