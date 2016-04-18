#include <Env/Env.hpp>

Env::Env () :
    world_ (), numberFlowers_ (0)
{
  flowerHead_.flower = nullptr;
  flowerHead_.previous = nullptr;
  flowerHead_.next = nullptr;
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
  unsigned int i (0);
  // get number of flowers before update
  unsigned int oldFlowers (numberFlowers_);
  flowerNode* currentNode (&flowerHead_);

  // cycle through linked list
  while ((*currentNode).next != nullptr)
    {
      // update if flower is old
      if (i <= oldFlowers)
        {
          double humidity (
              world_.getHumidity (currentNode->flower->getPosition ()));
          currentNode->flower->update (dt, humidity);
          // TODO : make sure that new flowers get drawn

          double split (
              getAppConfig ()["simulation"]["flower"]["growth"]["split"].toDouble ());
          if (currentNode->flower->getPollen () > split)
            {
              bool placed (false);
              while (!placed)
                {
                  // set a random distance
                  double radius (currentNode->flower->getRadius ());
                  double distance (uniform (0.5 * radius, 2 * radius));
                  Vec2d position = currentNode->flower->getPosition ()
                      + Vec2d::fromRandomAngle () * distance;
                  if (world_.isGrowable (position))
                    {
                      addFlowerAt (position);
                      placed = true;
                    }
                }
            }
          // check if flower is dead
          if (currentNode->flower->getPollen () <= 0)
            {
              // remove dead flower
              delete (*currentNode).flower;

              // remove node from list
              currentNode->next->previous = currentNode->previous;
              currentNode->previous->next = currentNode->next;

              // delete node and move to the next one
              flowerNode* oldNode (currentNode);
              currentNode = oldNode->next;
              delete oldNode;
            }
          else
            {
              // move to next node
              currentNode = (*currentNode).next;
            }
        }
      ++i;
    }
}

void
Env::drawOn (sf::RenderTarget& target)
{
  world_.drawOn (target);
  flowerNode* currentNode (&flowerHead_);
  while ((*currentNode).next != nullptr)
    {
      (*(*currentNode).flower).drawOn (target);
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
      // set the previous element
      (*(*currentNode).next).previous = (*currentNode).next;
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
