#include <Env/Bee.hpp>


Bee::Bee (Hive* hive, const Vec2d& position)
: Collider(position), hive_(hive)
{
  radius_ = this->getConfig ()["size"].toDouble();

  energy_ = this->getConfig ()["energy"]["initial"].toDouble();
  speed_ = this->getConfig ()["speed"].toDouble();

  move_vec_ = Vec2d::fromRandomAngle () * speed_;
  loadTexture();
}

Bee::Bee (Hive* hive, const Vec2d& position, double radius, double speedAmpl,
          double energy) :
    Collider (position, radius), hive_ (hive), speed_ (speedAmpl), energy_ (
        energy)
{
  reloadConfig ();
  move_vec_ = Vec2d::fromRandomAngle () * speed_;
  loadTexture ();
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
update (sf::Time dt)
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

  auto beeSprite = buildSprite (this->position_, this->radius_, beeTexture_);
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
