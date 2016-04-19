#ifndef FLOWER_H
#define FLOWER_H

#include <SFML/Graphics.hpp>
#include <Random/Random.hpp>
#include <Env/Collider.hpp>
#include <Utility/Utility.hpp>
#include <Application.hpp>

#include <Interface/Drawable.hpp>
#include <Interface/Updatable.hpp>

class Flower : public Collider, public Drawable, public Updatable
{

public:
  Flower ();

  Flower (const Vec2d& position, double radius, double pollen);

  double
  takePollen (const double& pollen);

  void
  loadTexture ();

  void
  update (sf::Time dt);

  double
  getPollen ();

private:

  double pollen_;
  sf::Texture texture_;
};

#endif
