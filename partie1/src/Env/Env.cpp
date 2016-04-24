#include <Env/Env.hpp>

#include <Env/Flower.hpp>
#include <Env/FlowerGenerator.hpp>

bool ENV_VERBOSE (true);

Env::Env () :
    world_ (new World ()), flowerGenerator_ (new FlowerGenerator)
{
  logEvent ("Env", "generating environment");
}

Env::~Env ()
{
  logEvent ("Env", "destroying environment");
  delete world_;

  for (size_t i = 0; i < flowers_.size (); ++i)
    {
      delete flowers_[i];
    }
  flowers_.clear ();

  for (size_t i = 0; i < hives_.size (); ++i)
    {
      delete hives_[i];
    }
  hives_.clear ();

}

void
Env::update (sf::Time dt)
{
  // update du generateur
  flowerGenerator_->update (dt);

  // iterate through flowers
  size_t numberFlowers (flowers_.size ());
  for (size_t i = 0; i < numberFlowers; ++i)
    {
      // flowers are updates
      // new flowers are drawn in next draw cycle
      flowers_[i]->update (dt);

      // check if flower is dead
      if (flowers_[i]->getPollen () <= 0)
        {
          // remove dead flower
          delete flowers_[i];
          flowers_[i] = nullptr;
        }
    }
  // remove empty locations
  flowers_.erase (std::remove (flowers_.begin (), flowers_.end (), nullptr),
                  flowers_.end ());
}

void
Env::drawOn (sf::RenderTarget& target) const
{
  world_->drawOn (target);
  for (size_t i = 0; i < flowers_.size (); ++i)
    {
      flowers_[i]->drawOn (target);
    }

  for (size_t i = 0; i < hives_.size (); ++i)
    {
      hives_[i]->drawOn (target);
    }

  // if debug is on, show values
  if (isDebugOn ())
    {
      // get cursor position
      Vec2d position = getApp ().getCursorPositionInView ();
      if (world_->isInWorld (position))
        {
          bool isHive (false);
          std::string valueString ("empty");
          sf::Color color (sf::Color::White);

          // check for hives
          for (size_t i = 0; i < hives_.size (); ++i)
            {
              if (position > *hives_[i])
                {
                  isHive = true;
                  valueString = to_nice_string (hives_[i]->getNectar ());
                  color = sf::Color::Green;
                }
            }

          // otherwise show ambient humidity
          if (!isHive)
            {
              valueString = to_nice_string (world_->getHumidity (position));
              color = sf::Color::Red;
            }

          // calculate appropriate text size
          int textSize (
              10
                  * (getAppConfig ()["simulation"]["world"]["size"].toDouble ()
                      / getAppConfig ()["simulation"]["world"]["cells"].toDouble ()));
          sf::Text text = buildText (valueString, position, getAppFont (),
                                     textSize, color);
          target.draw (text);

        }
    }

}

void
Env::reset ()
{
  logEvent ("Env", "resetting environment");

  world_->reset (true);
  for (size_t i = 0; i < flowers_.size (); ++i)
    {
      delete flowers_[i];
    }
  flowers_.clear ();
  flowerGenerator_->reset ();

  for (size_t i = 0; i < hives_.size (); ++i)
    {
      delete hives_[i];
    }
  hives_.clear ();
}

void
Env::loadWorldFromFile ()
{
  world_->loadFromFile ();
}

void
Env::saveWorldToFile ()
{
  world_->saveToFile ();
}

double
Env::getHumidity (const Vec2d& position)
{
  return world_->getHumidity (position);
}

bool
Env::isGrowable (const Vec2d& position)
{
  return world_->isGrowable (position);
}

bool
Env::addFlowerAt (const Vec2d& position)
{
  // get max number of Flower from configuration
  size_t maxFlowers =
      getAppConfig ()["simulation"]["env"]["max flowers"].toInt ();

  double radius (
      getAppConfig ()["simulation"]["env"]["initial"]["hive"]["size"]["manual"].toDouble ()
          / 2.0);

  Collider newFlower (position, radius);

  // check if flower can be made at position
  if (world_->isGrowable (position) && (flowers_.size () < maxFlowers)
      && (getCollidingFlower (newFlower) == nullptr))
    {
      // set a random number of pollen
      double pollen =
          uniform (
              getAppConfig ()["simulation"]["env"]["initial"]["flower"]["nectar"]["min"].toDouble (),
              getAppConfig ()["simulation"]["env"]["initial"]["flower"]["nectar"]["max"].toDouble ());

      flowers_.push_back (new Flower (position, radius, pollen));
      return true;
    }
  else
    {
      return false;
    }
}

void
Env::drawFlowerZone (sf::RenderTarget& target, const Vec2d& position)
{
  if (world_->isGrowable (position))
    {
      auto shape =
          buildAnnulus (
              position,
              getAppConfig ()["simulation"]["env"]["initial"]["flower"]["size"]["manual"].toDouble (),
              sf::Color::Green, 5.0);
      target.draw (shape);
    }
  else
    {
      auto shape =
          buildAnnulus (
              position,
              getAppConfig ()["simulation"]["env"]["initial"]["flower"]["size"]["manual"].toDouble (),
              sf::Color::Red, 5.0);
      target.draw (shape);
    }
}

bool
Env::addHiveAt (const Vec2d& position)
{
  double radius (
      getAppConfig ()["simulation"]["env"]["initial"]["hive"]["size"]["manual"].toDouble ()
          / 2.0);

  Collider colNewHive (position, radius);
  if (getCollidingHive (colNewHive) == nullptr
      && getCollidingFlower (colNewHive) == nullptr)
    {
      hives_.push_back (new Hive (position, radius));

      return true;
    }
  else
    {
      return false;
    }
}

Hive*
Env::getCollidingHive (const Collider& body)
{
  double size (
      getAppConfig ()["simulation"]["env"]["initial"]["hive"]["size"]["manual"].toDouble ());
  double factor (
      getAppConfig ()["simulation"]["env"]["initial"]["hive"]["hiveable factor"].toDouble ());
  for (size_t i (0); i < hives_.size (); ++i)
    {
      Collider collidingHive (hives_[i]->getPosition (), (size * factor));

      if (collidingHive.isColliding (body))
        {
          return hives_[i];
        }
    }
  return nullptr;
}

Flower*
Env::getCollidingFlower (const Collider& body)
{
  for (size_t i (0); i < flowers_.size (); ++i)
    {
      Collider collidingFlower (flowers_[i]->getPosition (),
                                flowers_[i]->getRadius ());

      if (collidingFlower.isColliding (body))
        {
          return flowers_[i];
        }
    }
  return nullptr;

}
