#include <Env/FlowerGenerator.hpp>

FlowerGenerator::FlowerGenerator () :
    counter_ (sf::Time::Zero)
{
}

void
FlowerGenerator::update (sf::Time dt)
{

  counter_ += dt;

  // if generation is active and a delay has passed
  // generate flowers at random locations
  if ((getAppConfig ()["simulation"]["flower generator"]["active"].toBool ())
      && (counter_
          > sf::seconds (
              (float) getAppConfig ()["simulation"]["flower generator"]["delay"].toDouble ())))
    {
      // reset the counter
      counter_ = sf::Time::Zero;
      int i(0);
      bool added;
      while ((!added) && (i < 100))
        {
          Vec2d position;
          position.x = uniform ((float) 0, (float) getApp ().getWorldSize ().x);
          position.y = uniform ((float) 0, (float) getApp ().getWorldSize ().y);
          added = (getAppEnv ()).addFlowerAt (position);
          ++i;
        }
    }

}

void
FlowerGenerator::reset ()
{
  counter_ = sf::Time::Zero;
}
