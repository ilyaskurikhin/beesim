#ifndef ENV_H
#define ENV_H

#include <Env/World.hpp>
#include <Utility/Vec2d.hpp>
#include <SFML/Graphics.hpp>
#include <Env/Flower.hpp>
#include <Utility/Utility.hpp>
#include <Random/Random.hpp>
#include <Env/FlowerGenerator.hpp>
class Env
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

  void
  update (sf::Time dt);

  void
  drawOn (sf::RenderTarget& target);

  void
  reset ();

  void
  loadWorldFromFile ();

  void
  saveWorldToFile ();

  bool
  addFlowerAt (const Vec2d& position);

  void
  drawFlowerZone (sf::RenderTarget& target, const Vec2d& position);

private:

  World world_;
  std::vector<Flower*> flowers_;
  
  FlowerGenerator generator_;
};
#endif
