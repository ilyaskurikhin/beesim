#include <Env/Bee.hpp>

Bee::Bee(Hive& hive, const Vec2d& position, double radius, double speedAmpl, double energy):
hive_(hive), Collider(position, radius), speedAmpl_(speedAmpl), energy_(energy)
{
  speed_ = Vec2d::fromRandomAngle()*speedAmpl_;
  loadTexture();
}

Vec2d 
Bee::move(sf::Time dt)
{
  Vec2d newPosition(position_);
  newPosition += dt * speed_;
  return newPosition;
}

bool
Bee::isDead()
{
  if (energy_ ==0) 
  { 
    return true;
  }
}

void
Bee::update (sf::Time dt)
{
}

void
Bee::drawOn (sf::RenderTarget& target) const 
{
  
  auto beeSprite = buildSprite (this->position_, this->radius_, beeTexture_);
  double alpha(speed_.Vec2d::angle());
  if ((alpha >= PI/2) or (alpha <= -PI/2))
  {
     beeSprite.scale(1, -1);
  }
  beeSprite.rotate(alpha / DEG_TO_RAD); 
  target.draw (beeSprite);
}

void
Bee::loadTexture () 
{
  beeTexture_= getAppTexture (getConfig()["texture"].toString ());
}

j::Value const&
Bee::getConfig()
{
  return getAppConfig()["simulation"]["bees"]["generic"];
}
