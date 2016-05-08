#include <Env/Bee.hpp>

#include <Env/Hive.hpp>
#include <Env/Flower.hpp>

Bee::Bee(Hive* hive, const Vec2d& position, std::vector<State> states) :
    Collider(position), CFSM(states), hive_(hive), debug_thickness_random_(5), debug_thickness_target_(
	3), vision_range_(position)
{
  // This constructor can not take care of its members
  // since it does not know what kind of bee it is

  // Initialization is done downstream
  // by calling the reloadConfig function
}

void
Bee::reloadConfig()
{
  // TODO resolve function calls to subclass
  radius_ = getConfig()["size"].toDouble();

  energy_ = getConfig()["energy"]["initial"].toDouble();
  energy_rate_idle_ =
      getConfig()["energy"]["consumption rates"]["idle"].toDouble();
  energy_rate_moving_ =
      getConfig()["energy"]["consumption rates"]["moving"].toDouble();
  energy_rate_eating_ =
      getConfig()["energy"]["consumption rates"]["eating"].toDouble();

  visibility_ = getConfig()["visibility range"].toDouble();
  vision_range_.setRadius(visibility_ + this->radius_);
  vision_range_.setPosition(this->position_);

  speed_ = getConfig()["speed"].toDouble();
  
  move_vec_ = Vec2d::fromRandomAngle () * speed_;
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
}

void
Bee::randomMove(sf::Time dt)
{
  logEvent("Bee", "randomMove called");
}

void
Bee::setMoveTarget(const Vec2d& position)
{
  move_target_ = position;
}

const Vec2d&
Bee::getMoveTarget() const
{
  return move_target_;
}

double
Bee::getSpeed() const
{
  return speed_;
}

void
Bee::targetMove(sf::Time dt)
{
  // TODO implement
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

double
Bee::getEnergy() const
{
  return energy_;
}

void
Bee::eatFromHive(sf::Time dt)
{
  energy_ = +hive_->takeNectar(energy_rate_eating_ * dt.asSeconds());
}

Hive*
Bee::getHive() const
{
  return hive_;
}

void
Bee::drawOn(sf::RenderTarget& target) const
{

  auto beeSprite = buildSprite(position_, radius_, texture_);
  double angle(move_vec_.Vec2d::angle());
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
        sf::CircleShape shape = buildAnnulus(position_, radius_,
                    sf::Color::Black, thickness);
        target.draw(shape);
      }
      else if (move_state_ == TARGET)
      {
        thickness = debug_thickness_target_;
        sf::CircleShape shape = buildAnnulus(position_, radius_,
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
