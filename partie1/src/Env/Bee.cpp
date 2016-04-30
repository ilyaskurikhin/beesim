#include <Env/Bee.hpp>

Bee::Bee (Hive* hive, const Vec2d& position) :
    Collider (position), CFSM({IN_HIVE}),
        hive_ (hive), move_vec_ (0, 0), speed_ (0),
        energy_ (0), energy_rate_idle_(0), energy_rate_moving_(0),
        debug_thickness_random_(5), debug_thickness_target_(3),
        flower_location_ (0, 0),
        IN_HIVE (createUid ()), AT_REST(createUid ()), RANDOM (createUid ()), TARGET (createUid ()),
        move_state_(AT_REST)
{
  // This constructor can not take care of its members
  // since it does not know what kind of bee it is

  // Initialization is done downstream
  // by calling the reloadConfig function
}



void
Bee::reloadConfig ()
{
  radius_ = this->getConfig ()["size"].toDouble ();

  energy_ = this->getConfig ()["energy"]["initial"].toDouble ();
  energy_rate_idle_ = this->getConfig()["energy"]["consumption rates"]["idle"].toDouble ();
  energy_rate_moving_ = this->getConfig()["energy"]["consumption rates"]["moving"].toDouble ();

  speed_ = this->getConfig ()["speed"].toDouble ();
}

void
Bee::move (sf::Time dt)
{
  if (move_state_ == AT_REST)
    {
      energy_ = energy_ - energy_rate_idle_ * dt.asSeconds();
    }
  else if (move_state_ == RANDOM)
    {
      randomMove (dt);
      energy_ = energy_ - energy_rate_moving_ * dt.asSeconds();
    }
  else if (move_state_ == TARGET)
    {
      targetMove (dt);
      energy_ = energy_ - energy_rate_moving_ * dt.asSeconds();
    }
}

void
Bee::targetMove (sf::Time dt)
{
  // TODO implement
}

bool
Bee::isDead ()
{
  if (energy_ == 0)
    {
      return true;
    }
  else
    {
      return false;
    }
}

void
Bee::update (sf::Time dt)
{
  this->action(dt);
  move (dt);
  // TODO implement energy loss
}

double
Bee::getEnergy () const
{
  return energy_;
}

void
Bee::drawOn (sf::RenderTarget& target) const
{

  auto beeSprite = buildSprite (position_, radius_, texture_);
  double angle (move_vec_.Vec2d::angle ());
  if ((angle >= PI / 2) or (angle <= -PI / 2))
    {
      beeSprite.scale (1, -1);
    }
  beeSprite.rotate (angle / DEG_TO_RAD);
  target.draw (beeSprite);

  if (isDebugOn())
    {
      double thickness(0);
      if (move_state_ == RANDOM)
        {
          thickness = debug_thickness_random_;
        }
      else if (move_state_ == TARGET)
        {
          thickness = debug_thickness_target_;
        }
      sf::CircleShape shape =
                buildAnnulus (
                    position_,
                    radius_,
                    sf::Color::Black, thickness);
            target.draw (shape);
    }
}

void
Bee::loadTexture ()
{
  texture_ = getAppTexture (this->getConfig ()["texture"].toString ());
}
