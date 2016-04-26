#ifndef BEE_H
#define BEE_H

#include <Random/Random.hpp>
#include <Utility/Vec2d.hpp>
#include <cmath>
#include <Utility/Utility.hpp>
#include <Application.hpp>
#include <Utility/Logging.hpp>
#include <Env/World.hpp>
#include <Env/Collider.hpp>
#include <Env/Hive.hpp>
#include <Env/Env.hpp>
#include <Interface/Drawable.hpp>
#include <Interface/Updatable.hpp>

class Bee : public Collider, public Drawable, public Updatable
{
public:

  Bee (Hive* hive, const Vec2d& position, double radius, double speedAmpl,
       double energy);

  void
  reloadConfig ();

  /*
   Void 
   move(sf::Time dt, Vec2d position, double speed); */

  bool
  isDead ();

  void
  update (sf::Time dt) override;

  void
  drawOn (sf::RenderTarget& target) const override;

  double
  getEnergy () const;

  virtual j::Value
  const&
  getConfig ();

  virtual void
  loadTexture ();

protected:

  Hive* hive_;
  Vec2d speed_;
  double speedAmpl_;
  double energy_;
  double maxAngle_;
  double rotationProbability_;
  sf::Texture beeTexture_;

};

#endif
