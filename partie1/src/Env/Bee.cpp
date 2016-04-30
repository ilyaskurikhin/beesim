#include <Env/Bee.hpp>

Bee::Bee (Hive* hive, const Vec2d& position) :
    Collider (position), hive_ (hive)
{
  // This constructor can not take care of its members
  // since it does not know what kind of bee it is

  // Initialization is done downstream
  // by calling the reloadConfig function
}

Bee::Bee (Hive* hive, const Vec2d& position, double radius, double speedAmpl,
          double energy) :
    Collider (position, radius), hive_ (hive), speed_ (speedAmpl), energy_ (
        energy)
{
}
/*
 void 
 Bee::move(sf::Time dt, Vec2d position, double speed)
 {
 //method is supposed to give back the new speed and position
 }
 */

void
Bee::reloadConfig ()
{
  radius_ = this->getConfig ()["size"].toDouble ();

  energy_ = this->getConfig ()["energy"]["initial"].toDouble ();
  speed_ = this->getConfig ()["speed"].toDouble ();
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
  // TODO implement update
}

double
Bee::getEnergy () const
{
  return energy_;
}

void
Bee::drawOn (sf::RenderTarget& target) const
{

  auto beeSprite = buildSprite (this->position_, this->radius_, texture_);
  double angle (move_vec_.Vec2d::angle ());
  if ((angle >= PI / 2) or (angle <= -PI / 2))
    {
      beeSprite.scale (1, -1);
    }
  beeSprite.rotate (angle / DEG_TO_RAD);
  target.draw (beeSprite);
}

void
Bee::loadTexture ()
{
  texture_ = getAppTexture (this->getConfig ()["texture"].toString ());
}
