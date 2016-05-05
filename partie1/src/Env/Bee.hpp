/**
 * @file Bee.hpp
 * @brief 
 * @author Ilya Skurikhin
 * @version p5.1
 * @date 2016-05-05
 */

#ifndef BEE_H
#define BEE_H

#include <Random/Random.hpp>

#include <Utility/Vec2d.hpp>
#include <Utility/Utility.hpp>
#include <Utility/Logging.hpp>

#include <Env/World.hpp>
#include <Env/Collider.hpp>
#include <Env/Env.hpp>
#include <Env/CFSM.hpp>

#include <Interface/Drawable.hpp>
#include <Interface/Updatable.hpp>

#include <Application.hpp>

#include <cmath>

class Hive;


/**
 * @brief Abstract class Bee.
 */
class Bee : public Collider, public Drawable, public Updatable, public CFSM
{
public:

  /**
   * @brief Class constructor.
   *
   * @param hive      Where the bee will return to.
   * @param position  The starting position of the bee.
   * @param states    The possible CFSM states of the bee.
   */
  Bee (Hive* hive, const Vec2d& position, std::vector<State> states);

  /**
   * @brief Reload the configuration of attributes from JSON. 
   */
  void
  reloadConfig ();

  /**
   * @brief Move the bee.
   *
   * @param dt Time for the counter.
   */
  void
  move (sf::Time dt);

  /**
   * @brief Move the bee towards a target defined by the type of bee.
   *
   * @param dt Time for the counter.
   */
  void
  targetMove (sf::Time dt);

  /**
   * @brief Move the bee randomly.
   *
   * Currently only implemented in ScoutBee.
   *
   * @param dt
   */
  virtual void
  randomMove (sf::Time dt);

  /**
   * @brief Check if Bee is dead.
   *
   * @return true is energy is zero.
   */
  bool
  isDead ();

  /**
   * @brief Evolve the Bee for a given time.
   *
   * Move the bee, change its energy, etc.
   *
   * @param dt
   */
  void
  update (sf::Time dt) override;

  /**
   * @brief Draw the Bee onto target.
   *
   * @param target
   */
  void
  drawOn (sf::RenderTarget& target) const override;

  /**
   * @brief Get the energy of the Bee.
   *
   * @return Energy of the Bee.
   */
  double
  getEnergy () const;

  /**
   * @brief Get Bee configuration by type.
   *
   * @return Configuration values.
   */
  virtual j::Value
  const&
  getConfig () =0;

  /**
   * @brief Load the Bee texture.
   *
   * Load the Bee texture using getConfig.
   */
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

  // memory of flower location
  Vec2d flower_location_;

  State const AT_REST = 0;
  State const RANDOM = 1; 
  State const TARGET = 2;
  State move_state_;

};

#endif
