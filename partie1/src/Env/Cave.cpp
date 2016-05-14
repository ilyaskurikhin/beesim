#include <Env/Cave.hpp>
#include <Env/Bear.hpp>

Cave::Cave(const Vec2d& position, double radius) :
    Collider(position, radius),
        getAppTexture(
            getAppConfig()["simulation"]["cave"]["texture"].toString()))
{
  reloadConfig()
}

void
Cave::reloadConfig()
{
  max_bees_ =
        getAppConfig()["simulation"]["hive"]["reproduction"]["max bees"].toDouble();
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
Hive::getBearAt(const Vec2d& position)
{

      if (bear_->isPointInside(position))
        {
          return bear_;
        }
    }
  return nullptr;
}

void
Hive::update(sf::Time dt)
{

  if (bear_ = nullptr)
    {
      this->addBear();
    }

  bear_->update(dt);

  if (bear_->isDead())
    {
      delete bear_;
      bear_ = nullptr;
    }
  }
}

void
Cave::drawOn(sf::RenderTarget& target) const
{
  auto caveSprite = buildSprite(this->getPosition(), this->getRadius(),
                                cave_texture_);
  target.draw(caveSprite);
  bear_->drawOn(target);
}


