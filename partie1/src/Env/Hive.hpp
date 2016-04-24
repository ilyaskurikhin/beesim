#ifndef HIVE_H
#define HIVE_H

#include <Env/World.hpp>
#include <Env/Bee.hpp>
#include <Env/Collider.hpp>
#include <Utility/Utility.hpp>
#include <Application.hpp>
#include <Utility/Logging.hpp>

#include <Interface/Drawable.hpp>
#include <Interface/Updatable.hpp>

class Hive : public Collider, public Drawable, public Updatable
{

public:

  Hive (const Vec2d& position, double radius);

  Hive (const Hive&) = delete;
  Hive
  operator= (const Hive&) = delete;

  ~Hive ();

  void
  addBee ();

  void
  update (sf::Time dt) override;

  void
  drawOn (sf::RenderTarget& targetWindow) const override;

  double
  dropPollen (double amount);

  double
  takeNectar (double amount);

  double
  getNectar ();

private:

  double nectar_;
  std::vector<Bee*> bees_;
  sf::Texture hiveTexture_;

};

#endif
