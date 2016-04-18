#ifndef FLOWER_H
#define FLOWER_H
#include <SFML/Graphics.hpp>
#include <Random/Random.hpp>
#include <Env/Collider.hpp>
#include <Utility/Utility.hpp>
#include <Application.hpp>

class Flower : public Collider
{

public:
  Flower ();

  Flower (const Vec2d& position, double radius, double pollen);

  double
  takePollen (const double& pollen);

  void
  drawOn (sf::RenderTarget& target) const;

  void
  loadTexture ();

  void
  update (sf::Time dt, double humidity);

  double
  getPollen();

private:

  double pollen_;
  sf::Texture texture_;
};

#endif
