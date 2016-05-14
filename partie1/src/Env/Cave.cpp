#include <Env/Cave.hpp>

Cave::Cave(const Vec2d& position, double radius) :
    Collider(position, radius)

{
  reloadConfig();
  addBear();
}

void
Cave::reloadConfig()
{
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
  std::vector<State> states =
    { Bear::HIBERNATION, Bear::SEARCH_HIVE, Bear::EAT_HONEY, Bear::RETURN_CAVE };
  Bear* bear(new Bear(this, this->getPosition(), states));
  bear->reloadConfig();
  return bear;
}

Bear*
Cave::getBearAt(const Vec2d& position)
{

  if (bear_->isPointInside(position))
    {
      return bear_;
    }
  return nullptr;
}

void
Cave::update(sf::Time dt)
{
  bear_->update(dt);

  if (bear_->isDead())
    {
      delete bear_;
      bear_ = nullptr;
    }
}

void
Cave::drawOn(sf::RenderTarget& target) const
{
  auto caveSprite = buildSprite(this->getPosition(), this->getRadius(),
                                texture_);
  target.draw(caveSprite);
  bear_->drawOn(target);
}

