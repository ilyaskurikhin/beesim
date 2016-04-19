#include <Env/Env.hpp>

Env::Env () :
    world_ ()
{
  try
    {
      world_.loadFromFile ();
    }
  catch (const std::runtime_error& e)
    {
      world_.reset (true);
    }
  world_.updateCache ();
}

void
Env::update (sf::Time dt)
{
  //update du generateur
  generator_.update (dt);

  // iterate through flowers
  size_t numberFlowers (flowers_.size ());
  for (size_t i = 0; i <= numberFlowers; ++i)
    {
      double humidity (world_.getHumidity (flowers_[i]->getPosition ()));
      flowers_[i]->update (dt, humidity);
      // TODO : make sure that new flowers get drawn

      double split (
          getAppConfig ()["simulation"]["flower"]["growth"]["split"].toDouble ());
      // split flower is has enough pollen
      if (flowers_[i]->getPollen () > split)
        {
          int i (0);
          bool placed (false);
          while ((!placed) && (i < 100)) //
            {
              // set a random distance
              double radius (flowers_[i]->getRadius ());
              double distance (uniform (0.5 * radius, 2 * radius));
              Vec2d position = flowers_[i]->getPosition ()
                  + Vec2d::fromRandomAngle () * distance;
              if (world_.isGrowable (position))
                {
                  addFlowerAt (position);
                  placed = true;
                }
              ++i;
            }
        }

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
Env::drawOn (sf::RenderTarget& target)
{
  world_.drawOn (target);
  for (size_t i = 0; i < flowers_.size (); ++i)
    {
      flowers_[i]->drawOn (target);
    }
}

void
Env::reset ()
{
  world_.reset (true);
  for (size_t i = 0; i < flowers_.size (); ++i)
    {
      delete flowers_[i];
    }
  flowers_.clear ();
  generator_.reset ();
}

void
Env::loadWorldFromFile ()
{
  world_.loadFromFile ();
}

void
Env::saveWorldToFile ()
{
  world_.saveToFile ();
}

bool
Env::addFlowerAt (const Vec2d& position)
{
  // get max number of Flower from configuration
  size_t maxFlowers =
      getAppConfig ()["simulation"]["env"]["max flowers"].toInt ();

  // check if flower can be made at position
  if (world_.isGrowable (position) && (flowers_.size () < maxFlowers))
    {
      // set a random number of pollen
      double pollen =
          uniform (
              getAppConfig ()["simulation"]["env"]["initial"]["flower"]["nectar"]["min"].toDouble (),
              getAppConfig ()["simulation"]["env"]["initial"]["flower"]["nectar"]["max"].toDouble ());

      flowers_.push_back (
          new Flower (
              position,
              getAppConfig ()["simulation"]["env"]["initial"]["flower"]["size"]["manual"].toDouble ()
                  / 2.0,
              pollen));
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
  Vec2d worldPosition = world_.positionInWorld (position);
  if (world_.isGrowable (worldPosition))
    {
      auto shape =
          buildAnnulus (
              worldPosition,
              getAppConfig ()["simulation"]["env"]["initial"]["flower"]["size"]["manual"].toDouble (),
              sf::Color::Green, 5.0);
      target.draw (shape);
    }
  else
    {
      auto shape =
          buildAnnulus (
              worldPosition,
              getAppConfig ()["simulation"]["env"]["initial"]["flower"]["size"]["manual"].toDouble (),
              sf::Color::Red, 5.0);
      target.draw (shape);
    }
}
