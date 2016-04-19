#include <Env/FlowerGenerator.hpp>

FlowerGenerator::FlowerGenerator () :
    counter_ (sf::Time::Zero)
{
}

void
FlowerGenerator::update (sf::Time dt)
{

  counter_ += dt;
  if (((float) counter_
      > (float) sf::seconds (
          getAppConfig ()["simulation"]["flower generator"]["delay"].toDouble ()))
      && (getAppConfig ()["simulation"]["flower"]["active"].toBool ()))
    {
      counter_ = 0;
      Vec2d position;
      position.x = uniform ((float) 0, (float) getApp ().getWorldSize ());
      position.y = uniform ((float) 0, (float) getApp ().getWorldSize ());
      (getAppEnv ()).addFlowerAt (position);
    }

}

void
FlowerGenerator::reset ()
{
  counter_ = sf::Time::Zero;
}
