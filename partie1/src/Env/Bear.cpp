/*
 * Bear.cpp
 *
 *  Created on: May 13, 2016
 *      Author: ilya
 */

#include <Env/Bear.hpp>

#include <Env/Env.hpp>

Bear::Bear(Cave* cave, const Vec2d& position) :
    Movable(position), CFSM(std::vector<State>(
      { HIBERNATION, SEARCH_HIVE, EAT_HONEY, RETURN_CAVE })), cave_(cave), debug_thickness_random_(
        5), debug_thickness_target_(3), vision_range_(position), move_state_(
        AT_REST)
{
  reloadConfig();
  loadTexture();
}

Bear::~Bear()
{
}

void
Bear::reloadConfig()
{
  // configure Collider
  this->setRadius(getConfig()["size"].toDouble());

  // configure Moveable
  this->setDelay(
      sf::seconds(
          getConfig()["moving behaviour"]["target"]["avoidance delay"].toDouble()));
  this->setSpeed(getConfig()["speed"].toDouble());
  this->setMoveVec(Vec2d::fromRandomAngle() * this->getSpeed());
  this->setMaxAngle(
      getConfig()["moving behaviour"]["random"]["rotation angle max"].toDouble());
  this->setRotationProbability(
      getConfig()["moving behaviour"]["random"]["rotation probability"].toDouble());

  energy_ = getConfig()["energy"]["initial"].toDouble();
  energy_rate_idle_ =
      getConfig()["energy"]["consumption rates"]["idle"].toDouble();
  energy_leave_cave_ = getConfig()["energy"]["to leave cave"].toDouble();
  energy_rate_moving_ =
      getConfig()["energy"]["consumption rates"]["moving"].toDouble();
  energy_rate_eating_ =
      getConfig()["energy"]["consumption rates"]["eating"].toDouble();
  energy_seek_hives_ =
      getConfig()["energy"]["consumption rates"]["seeking hive"].toDouble();

  texture_delay_ = sf::seconds(
      static_cast<float>(getConfig()["texture delay"].toDouble()));
  texture_counter_.restart();

  honey_eating_rate_ = getConfig()["honey eating rate"].toDouble();

  max_hibernation_ =
      sf::seconds(
          static_cast<float>(getConfig()["hibernation"]["maximum time"].toDouble()));

  visibility_ = getConfig()["visibility range"].toDouble();
  vision_range_.setRadius(visibility_ + this->getRadius());
}

j::Value const&
Bear::getConfig()
{
  return getAppConfig()["simulation"]["bear"];
}

void
Bear::move(sf::Time dt)
{
  if (move_state_ == AT_REST)
    {
      energy_ = energy_ - energy_rate_idle_ * dt.asSeconds();
    }
  else if (move_state_ == RANDOM)
    {
      randomMove(dt);
      energy_ = energy_ - energy_rate_moving_ * dt.asSeconds();
    }
  else if (move_state_ == TARGET)
    {
      targetMove(dt);
      energy_ = energy_ - energy_rate_moving_ * dt.asSeconds();
    }
  vision_range_.setPosition(this->getPosition());
}

bool
Bear::isMovablePosition(const Vec2d& position) const
{
  return getAppEnv().isWalkable(position);
}

void
Bear::update(sf::Time dt)
{
  this->action(dt);
  this->move(dt);
}

double
Bear::getEnergy() const
{
  return energy_;
}

double
Bear::getHibernationLength() const
{
  return hibernation_length_.asSeconds();
}

double
Bear::eatHoney(Hive* hive, sf::Time dt)
{
  double eaten(honey_eating_rate_ * dt.asSeconds());
  if (hive)
    {
      eaten = hive->takeNectar(eaten);
    }
  else
    {
      eaten = 0;
    }
  energy_ += eaten;

  return eaten;
}

Cave*
Bear::getCave() const
{
  return cave_;
}

void
Bear::drawOn(sf::RenderTarget& target) const
{
  Vec2d position(this->getPosition());
  double radius(this->getRadius());
  sf::Time elapsedTime(texture_counter_.getElapsedTime());
  sf::Time delay(texture_delay_);

  sf::Texture texture;
  if ((elapsedTime % delay) > (delay / 2.f))
    {
      texture = texture_walking_1_;
    }
  else
    {
      texture = texture_walking_2_;
    }

  auto bearSprite = buildSprite(position, radius, texture);
  double angle(this->getMoveVec().Vec2d::angle());
  if ((angle >= PI / 2) or (angle <= -PI / 2))
    {
      bearSprite.scale(1, -1);
    }
  bearSprite.rotate(angle / DEG_TO_RAD);

  if (!isInCave())
    {
      target.draw(bearSprite);
    }

  if (isDebugOn())
    {
      double thickness(0);
      if (move_state_ == RANDOM)
        {
          thickness = debug_thickness_random_;
          sf::CircleShape shape = buildAnnulus(position, radius,
                                               sf::Color::Black, thickness);
          target.draw(shape);
        }
      else if (move_state_ == TARGET)
        {
          thickness = debug_thickness_target_;
          sf::CircleShape shape = buildAnnulus(position, radius,
                                               sf::Color::Blue, thickness);
          target.draw(shape);
        }

      std::string valueString;
      sf::Color color(sf::Color::Red);
      Vec2d position;
      double text_size(getAppEnv().getTextSize());

      position = this->getPosition();
      position.y += text_size;

      valueString = "Bear: energy " + to_nice_string(this->getEnergy())
          + " Bear: hibernation length "
          + to_nice_string(this->getHibernationLength());
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
Bear::loadTexture()
{
  texture_walking_1_ = getAppTexture(
      this->getConfig()["texture walking 1"].toString());
  texture_walking_2_ = getAppTexture(
      this->getConfig()["texture walking 2"].toString());
}

void
Bear::onState(State state, sf::Time dt)
{
  // first state
  if (state == HIBERNATION)
    {
      if (hibernation_length_ < max_hibernation_
          && this->getEnergy() > energy_leave_cave_)
        {
          std::string status = "in_cave_hibernating_";
          this->setDebugStatus(status);
          hibernation_length_ += dt;
        }

      if (hibernation_length_ >= max_hibernation_
          && this->getEnergy() > energy_leave_cave_)
        {
          this->setDebugStatus("in_cave_leaving");
          this->nextState();
        }
    }

  // second state
  else if (state == SEARCH_HIVE)
    {
      this->setDebugStatus("seeking_hives");
      Hive* hive = this->findVisibleHive();
      if (this->getEnergy() > energy_seek_hives_ && hive != nullptr)
        {
          this->setMoveTarget(hive->getPosition());
          this->setMoveStateTARGET();
          if (this->isColliderInside(hive->getCollider()))
            this->nextState();
        }
    }

  // third state
  else if (state == EAT_HONEY)
    {
      this->setDebugStatus("eating_honey");
      Hive* hive(getAppEnv().getCollidingHive(this->getCollider()));
      if (hive != nullptr)
        {
          eatHoney(hive, dt);
          if (hive->getNectar() <= 0)
            this->nextState();
        }
      else
        {
          this->nextState();
        }
    }

  // fourth state
  else if (state == RETURN_CAVE)
    {
      this->setDebugStatus("back_to_cave");
      this->setMoveTarget(this->getCave()->getPosition());
      if (this->getCave()->isColliderInside(this->getCollider()))
        {
          this->nextState();
        }
    }
}

void
Bear::onEnterState(State state)
{
  if (state == HIBERNATION or state == EAT_HONEY)
    {
      this->setMoveStateAT_REST();
    }
  else if (state == SEARCH_HIVE)
    {
      this->setMoveStateRANDOM();
      hibernation_length_ = sf::Time::Zero;
    }
  else if (state == RETURN_CAVE)
    {
      this->setMoveStateTARGET();
    }
}

bool
Bear::isDead()
{
  if (energy_ <= 0)
    {
      return true;
    }
  else
    {
      return false;
    }
}

Hive*
Bear::findVisibleHive() const
{
  return getAppEnv().getCollidingHive(vision_range_);
}

void
Bear::setMoveStateAT_REST()
{
  move_state_ = AT_REST;
}

void
Bear::setMoveStateRANDOM()
{
  move_state_ = RANDOM;
}

void
Bear::setMoveStateTARGET()
{
  move_state_ = TARGET;
}

void
Bear::setDebugStatus(const std::string& status)
{
  debug_status_ = status;
}

const std::string&
Bear::getDebugStatus() const
{
  return debug_status_;
}

bool
Bear::isInCave() const
{
  if (this->getState() == HIBERNATION)
    {
      return true;
    }
  else
    {
      return false;
    }
}

State const Bear::HIBERNATION = createUid();
State const Bear::SEARCH_HIVE = createUid();
State const Bear::EAT_HONEY = createUid();
State const Bear::RETURN_CAVE = createUid();

