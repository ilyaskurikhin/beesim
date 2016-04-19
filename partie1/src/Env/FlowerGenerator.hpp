#ifndef FLOWERGENERATOR_H
#define FLOWERGENERATOR_H

#include <SFML/Graphics.hpp>
#include <SFML/System.hpp>

#include <Application.hpp>
#include <Random/Random.hpp>

class FlowerGenerator
{
public:

  FlowerGenerator ();

  void
  update (sf::Time dt);

  void
  reset ();

private:

  sf::Time counter_;
};

#endif
