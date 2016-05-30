#include <Env/Bee.hpp>

Bee::Bee(Hive* hive, const Vec2d& position, double radius, double speedAmpl, double energy):
Collider(position, radius), hive_(hive), speedAmpl_(speedAmpl), energy_(energy)
{
  reloadConfig();
  speed_ = Vec2d::fromRandomAngle()*speedAmpl_;
  loadTexture();
}
/*
void 
Bee::move(sf::Time dt, Vec2d position, double speed)
{
  //method is supposed to give back the new speed and position
}
*/

void 
Bee::reloadConfig()
{
  rotationProbability_ = (getAppConfig() ["simulation"]["bees"]["generic"]["moving behaviour"]["random"]["rotation probability"].toDouble());
  maxAngle_ = (getAppConfig()["simulation"]["bees"]["generic"]["moving behaviour"]["random"]["rotation angle max"].toDouble());
}

bool
Bee::isDead()
{
  if (energy_ ==0) 
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
  Vec2d position(position_);
  if (bernoulli(rotationProbability_)) 
  {
    double angleA(uniform(-maxAngle_, maxAngle_));
    speed_.rotate(angleA);
  }
  
  position.x += dt.asSeconds() * speed_.x;
  position.y += dt.asSeconds() * speed_.y;
    
  Collider protoBee (position, radius_);
  protoBee.clamping ();

    if (!getAppEnv().isFlyable(protoBee.getPosition()))
      {
        double angleB;
        if(bernoulli(0.5)) 
          {
            angleB= PI/4;
          }
        else 
          {
            angleB= -PI/4;
          }
        speed_.rotate(angleB);

      }
    else 
      {
      this->position_=protoBee.getPosition();
      }
      
  energy_ -= 0.1 * dt.asSeconds();
  
}

double
Bee::getEnergy() const 
{
  return energy_;
}


void
Bee::drawOn (sf::RenderTarget& target) const 
{
  
  auto beeSprite = buildSprite (this->position_, this->radius_, beeTexture_);
  double angle(speed_.Vec2d::angle());
  if ((angle >= PI/2) or (angle <= -PI/2))
  {
     beeSprite.scale(1, -1);
  }
  beeSprite.rotate(angle / DEG_TO_RAD); 
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
