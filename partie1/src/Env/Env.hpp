#ifndef ENV_H
#define ENV_H

#include <SFML/Graphics.hpp>

#include <Env/World.hpp>
#include <Utility/Vec2d.hpp>
#include <Utility/Utility.hpp>
#include <Random/Random.hpp>
#include <Interface/Drawable.hpp>
#include <Interface/Updatable.hpp>

#include <memory>

class Flower;
class FlowerGenerator;

class Env : public Drawable, public Updatable
{
public:
  /**
   * @brief Default class constructor.
   *
   * Construct a World in world_ from file in configuration.
   * If file cannot be loaded, generate World.
   * Draw the world_ to cache.
   * Initialize numberFlowers_ to 0.
   */
  Env ();

  ~Env ();

  void
  update (sf::Time dt);

  void
  drawOn (sf::RenderTarget& target) const;

  void
  reset ();

  void
  loadWorldFromFile ();

  void
  saveWorldToFile ();

  double
  getHumidity (const Vec2d& position);

  bool
  isGrowable (const Vec2d& position);

  bool
  addFlowerAt (const Vec2d& position);

  void
  drawFlowerZone (sf::RenderTarget& target, const Vec2d& position);

private:

  World* world_;
  std::vector<Flower*> flowers_;
  FlowerGenerator* flowerGenerator_;
};
#endif
