#include <Env/Cave.hpp>

Cave::Cave(const Vec2d& position, double radius) :
    Collider(position, radius)

{
  // call to realod config to intialise attributes from config file
  reloadConfig();
  // add a Bear (there is only one per cave)
  addBear();
}

void
Cave::reloadConfig()
{
  this->setRadius(getAppConfig()["simulation"]["cave"]["size"].toDouble());
  texture_ = getAppTexture(
      getAppConfig()["simulation"]["cave"]["texture"].toString());
}

Cave::~Cave()
{
  delete bear_;
}

Bear*
Cave::addBear()
{
  // create a new bear in the cave
  bear_ = new Bear(this, this->getPosition());
  bear_->reloadConfig();
  return bear_;
}

Bear*
Cave::getBearAt(const Vec2d& position)
{
  // if there is a bear at position return a pointer to the bear
  if (bear_->isPointInside(position))
    {
      return bear_;
    }
  return nullptr;
}

Bear*
Cave::getBear()
{
  return bear_;
}

void
Cave::update(sf::Time dt)
{
  // update the bear
  getBear()->update(dt);

  if (bear_->isDead())
    {
      delete bear_;
      bear_ = nullptr;
    }
}

void
Cave::drawOn(sf::RenderTarget& target) const
{
  // create the cave sprite
  auto caveSprite = buildSprite(this->getPosition(), this->getRadius(),
                                texture_);
  // draw cave sprite
  target.draw(caveSprite);
  
  // draw the bear
  bear_->drawOn(target);
}

