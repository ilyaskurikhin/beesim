#include <Env/Bee/Bee.hpp>
#include <Env/CFSM.hpp>
#include <Env/Hive.hpp>
#include <Env/Flower.hpp>

Bee::Bee(Hive* hive, const Vec2d& position, std::vector<State> states, BeeType beeType) :
    Movable(position), CFSM(states), hive_(hive), bee_type_(beeType), vision_range_(position)
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

  debug_thickness_random_ = 3.0;
  debug_thickness_target_ = 3.0;

  visibility_ = getConfig()["visibility range"].toDouble();
  vision_range_.setRadius(visibility_ + this->getRadius());
}

void
Bee::move(sf::Time dt)
{
  // bee at rest loses its energy at a certain rate
  if (getMoveState() == MoveState::AT_REST)
    {
      energy_ = energy_ - energy_rate_idle_ * dt.asSeconds();
    }
    
  // bee moving randomly loses its energy at a certain rate
  else if (getMoveState() == MoveState::RANDOM)
    {
      randomMove(dt);
      energy_ = energy_ - energy_rate_moving_ * dt.asSeconds();
    }
    
  // bee moving to a target loses its energy at a certain rate
  else if (getMoveState() == MoveState::TARGET)
    {
      targetMove(dt);
      energy_ = energy_ - energy_rate_moving_ * dt.asSeconds();
    }
  // sets current bee position to the vision range position
  vision_range_.setPosition(this->getPosition());
}

bool
Bee::isMovablePosition(const Vec2d& position) const
{
  return getAppEnv().isFlyable(position);
}

bool
Bee::isDead() const
{
  if (energy_ <= 0)
      return true;
  else
      return false;
}

bool
Bee::isInHive() const
{
  if (this->isColliderInside(*hive_))
    return true;
  else
    return false;
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
  // energy increases by the amount of nectar taken from hive
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

  // creates the bee sprite
  auto beeSprite = buildSprite(position, radius, texture_);
  double angle(this->getMoveVec().Vec2d::angle());
  
  // if it is not in the correct angle moves the sprite
  if ((angle >= PI / 2) or (angle <= -PI / 2))
    {
      beeSprite.scale(1, -1);
    }
  beeSprite.rotate(angle / DEG_TO_RAD);
  
  // draw the sprite
  target.draw(beeSprite);

  if (isDebugOn())
    {
      // if bee moves randomly draw a black annulus around it
      if (getMoveState() == MoveState::RANDOM)
        {
          sf::CircleShape shape = buildAnnulus(position, radius,
                                               sf::Color::Black, debug_thickness_random_);
          target.draw(shape);
        }
      
      // if bee moves to a target draw a blue annulus around it
      else if (getMoveState() == MoveState::TARGET)
        {
          sf::CircleShape shape = buildAnnulus(position, radius,
                                               sf::Color::Blue, debug_thickness_target_);
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
  return getAppEnv().getCollidingFlower(vision_range_);
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
Bee::setHive(Hive* hive)
{
  hive_ = hive;
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

const BeeType&
Bee::getBeeType() const
{
  return bee_type_;
}
