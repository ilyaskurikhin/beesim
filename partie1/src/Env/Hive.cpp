#include <Env/Hive.hpp>

Hive::Hive (const Vec2d& position, double radius) :
    Collider (position, radius), nectar_ (
        getAppConfig ()["simulation"]["hive"]["initial"]["nectar"].toDouble ()), hiveTexture_ (
        getAppTexture (
            getAppConfig ()["simulation"]["hive"]["texture"].toString ()))
{
}

Hive::~Hive ()
{
  for (size_t i = 0; i < bees_.size (); ++i)
    {
      delete bees_[i];
    }
  bees_.clear ();
}

void
Hive::addBee ()
{
  // TODO implement addBee method
}

void
Hive::update (sf::Time dt)
{
  // TODO implement update method
}

void
Hive::drawOn (sf::RenderTarget& target) const
{
  auto hiveSprite = buildSprite (this->position_, this->radius_, hiveTexture_);
  target.draw (hiveSprite);
}

double
Hive::dropPollen (double nectar)
{
  if (nectar > 0)
    {
      nectar_ = nectar_ + nectar;
    }
  return nectar_;
}

double
Hive::takeNectar (double nectar)
{
  if ((nectar > 0) && (nectar_ - nectar > 0))
    {
      nectar_ = nectar_ - nectar;
    }
  return nectar_;
}

double
Hive::getNectar ()
{
  return nectar_;
}

