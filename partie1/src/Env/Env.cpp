#include <Env/Env.hpp>

Env::Env () :
    world_ (), numberFlowers_ (0)
{
  flowerHead_.flower (nullptr);
  flowerHead_.next (nullptr);
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
  world_.updateCache ();
}

void
Env::drawOn (sf::RenderTarget& target)
{
  world_.drawOn (target);
  flowerNode* node (*flowerHead_);
  while ((*node).next != nullptr)
    {
      (*(*node).flower).drawOn (target);
    }
}

void
Env::reset ()
{
  world_.reset (true);
  flowerNode* currentNode (&flowerHead_);
  while ((*currentNode).next != nullptr)
    {
      delete (*currentNode).flower;
      flowerNode* oldNode = currentNode;
      flowerNode* newNode = (*currentNode).next;
      delete oldNode;
      currentNode = newNode;
    }
  numberFlowers_ = 0;
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
  if (world_.isGrowable (position) && (numberFlowers_ < maxFlowers))
    {
      // set a random number of pollen
      double pollen =
          uniform (
              getAppConfig ()["simulation"]["env"]["initial"]["flower"]["nectar"]["min"].toDouble (),
              getAppConfig ()["simulation"]["env"]["initial"]["flower"]["nectar"]["max"].toDouble ());

      // find the current tail of the Flower linked list
      flowerNode* currentNode (&flowerHead_);
      while ((*currentNode).next != nullptr)
        {
          currentNode = (*currentNode).next;
        }
      // create a new linked list element
      (*currentNode).next = new flowerNode;

      // create a flower at the element
      (*(*currentNode).next).flower =
          new Flower (
              position,
              getAppConfig ()["simulation"]["env"]["initial"]["flower"]["size"]["manual"].toDouble ()
                  / 2.0,
              pollen);
      // set this as the last element
      (*(*currentNode).next).next = nullptr;
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

