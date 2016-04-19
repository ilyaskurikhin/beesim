#include <Env/Env.hpp>

#include <Env/Flower.hpp>
#include <Env/FlowerGenerator.hpp>

Env::Env () :
    world_ (new World()), 
    flowerGenerator_ (new FlowerGenerator) 
{
  try
    {
      world_->loadFromFile ();
    }
  catch (const std::runtime_error& e)
    {
      world_->reset (true);
    }
  world_->updateCache ();
}

Env::~Env()
{
  delete world_;
}

void
Env::update (sf::Time dt)
{
  //update du generateur
  flowerGenerator_->update (dt);

  // iterate through flowers
  size_t numberFlowers (flowers_.size ());
  for (size_t i = 0; i < numberFlowers; ++i)
    {
      flowers_[i]->update (dt);
      // TODO : make sure that new flowers get drawn

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
}

void
Env::reset ()
{
  world_->reset (true);
  for (size_t i = 0; i < flowers_.size (); ++i)
    {
      delete flowers_[i];
    }
  flowers_.clear ();
  flowerGenerator_->reset ();
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

  // check if flower can be made at position
  if (world_->isGrowable (position) && (flowers_.size () < maxFlowers))
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
