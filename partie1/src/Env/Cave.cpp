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

Bear*
Cave::getBear()
{
  return bear_;
}

void
Cave::update(sf::Time dt)
{
  if (getBear())
  {
    std::cout << "There is a bear" << std::endl;
  }
  
  getBear()->update(dt);

  std::cout << "updated" << std::endl;

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

