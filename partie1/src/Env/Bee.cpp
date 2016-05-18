#include <Env/Bee.hpp>
#include <Env/CFSM.hpp>
#include <Env/Hive.hpp>
#include <Env/Flower.hpp>

Bee::Bee(Hive* hive, const Vec2d& position, std::vector<State> states) :
    Movable(position), CFSM(states), hive_(hive), debug_thickness_random_(5), debug_thickness_target_(
        3), vision_range_(position), move_state_(AT_REST)
{
  // This constructor can not take care of its members
  // since it does not know what kind of bee it is

  // Initialization is done downstream
  // by calling the reloadConfig function
}

void
Bee::reloadConfig()
{
  // configure Collider
  this->setRadius(getConfig()["size"].toDouble());

  // configure Moveable
  this->setDelay(
      sf::seconds(
          getConfig()["moving behaviour"]["target"]["avoidance delay"].toDouble()));
  this->setSpeed(getConfig()["speed"].toDouble());
  this->setMoveVec(Vec2d::fromRandomAngle() * this->getSpeed());

  energy_ = getConfig()["energy"]["initial"].toDouble();
  energy_rate_idle_ =
      getConfig()["energy"]["consumption rates"]["idle"].toDouble();
  energy_rate_moving_ =
      getConfig()["energy"]["consumption rates"]["moving"].toDouble();
  energy_rate_eating_ =
      getConfig()["energy"]["consumption rates"]["eating"].toDouble();

  visibility_ = getConfig()["visibility range"].toDouble();
  vision_range_.setRadius(visibility_ + this->getRadius());
}

void
Bee::move(sf::Time dt)
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
Bee::isMovablePosition(const Vec2d& position) const
{
  return getAppEnv().isFlyable(position);
}

bool
Bee::isDead()
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

void
Bee::update(sf::Time dt)
{
  this->action(dt);
  this->move(dt);
}

void
Bee::setEnergy(double energy)
{
  energy_ = energy;
}

double
Bee::getEnergy() const
{
  return energy_;
}

void
Bee::eatFromHive(sf::Time dt)
{
  energy_ += this->getHive()->takeNectar(energy_rate_eating_ * dt.asSeconds());
}

Hive*
Bee::getHive() const
{
  return hive_;
}

void
Bee::drawOn(sf::RenderTarget& target) const
{
  Vec2d position(this->getPosition());
  double radius(this->getRadius());

  auto beeSprite = buildSprite(position, radius, texture_);
  double angle(this->getMoveVec().Vec2d::angle());
  if ((angle >= PI / 2) or (angle <= -PI / 2))
    {
      beeSprite.scale(1, -1);
    }
  beeSprite.rotate(angle / DEG_TO_RAD);
  target.draw(beeSprite);

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

    }
}

void
Bee::loadTexture()
{
  texture_ = getAppTexture(this->getConfig()["texture"].toString());
}

Flower*
Bee::findVisibleFlower() const
{
  //what if two flowers are colliding?  
  return getAppEnv().getCollidingFlower(vision_range_);
}

void
Bee::setMoveStateAT_REST()
{
  move_state_ = AT_REST;
}

void
Bee::setMoveStateRANDOM()
{
  move_state_ = RANDOM;
}

void
Bee::setMoveStateTARGET()
{
  move_state_ = TARGET;
}

void
Bee::setDebugStatus(const std::string& status)
{
  debug_status_ = status;
}

const std::string&
Bee::getDebugStatus() const
{
  return debug_status_;
}

void
Bee::setHive(const Hive*& hive)
{
  hive_ = hive;
}

State
Bee::getMoveState() const
{
  return move_state_;
}

void
Bee::setMoveState(State moveState)
{
  move_state_ = moveState;
}

double
Bee::getVisibility() const
{
  return visibility_;
}

void
Bee::setVisibility(double visibility)
{
  visibility_ = visibility;
}

const Collider&
Bee::getVisionRange() const
{
  return vision_range_;
}

void
Bee::setVisionRange(const Collider& visionRange)
{
  vision_range_ = visionRange;
}
