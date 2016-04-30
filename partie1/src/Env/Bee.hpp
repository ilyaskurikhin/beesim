#ifndef BEE_H
#define BEE_H

#include <Random/Random.hpp>

#include <Utility/Vec2d.hpp>
#include <Utility/Utility.hpp>
#include <Utility/Logging.hpp>

#include <Env/World.hpp>
#include <Env/Collider.hpp>
#include <Env/Hive.hpp>
#include <Env/Env.hpp>
#include <Env/CFSM.hpp>

#include <Interface/Drawable.hpp>
#include <Interface/Updatable.hpp>

#include <Application.hpp>

#include <cmath>

class Hive;

class Bee : public Collider, public Drawable, public Updatable, public CFSM
{
public:

  Bee (Hive* hive, const Vec2d& position);

  void
  reloadConfig ();

  void
  move (sf::Time dt);

  void
  targetMove (sf::Time dt);

  virtual void
  randomMove (sf::Time dt);

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
  getConfig () =0;

  void
  loadTexture ();

protected:

  Hive* hive_;
  Vec2d move_vec_;
  double speed_;
  double energy_;
  double energy_rate_idle_;
  double energy_rate_moving_;

  sf::Texture texture_;
  double debug_thickness_random_;
  double debug_thickness_target_;

  Vec2d flower_location_;

  // TODO make move states static
  State static const IN_HIVE = createUid ();

  State static AT_REST;
  State static RANDOM;
  State static TARGET;
  State move_state_;

};

#endif
