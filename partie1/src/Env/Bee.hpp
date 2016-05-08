/**
 * @file Bee.hpp
 * @brief 
 * @author Ilya Skurikhin
 * @author Julia Besson
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
class Flower;

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
  Bee(Hive* hive, const Vec2d& position, std::vector<State> states);

  /**
   * @brief Reload the configuration of attributes from JSON. 
   */
  void
  reloadConfig();

  /**
   * @brief Move the bee.
   *
   * @param dt Time for the counter.
   */
  void
  move(sf::Time dt);

  /**
   * @brief Set a target to move towards
   *
   * @param position graphic position.
   */
  void
  setMoveTarget(const Vec2d& position);

  /**
   * @brief Get the current move target
   *
   * @return graphic position of target.
   */
  const Vec2d&
  getMoveTarget() const;

  /**
   * @brief Move the bee towards a target defined by the type of bee.
   *
   * @param dt Time for the counter.
   */
  virtual void
  targetMove(sf::Time dt);

  /**
   * @brief Move the bee randomly.
   *
   * Currently only implemented in ScoutBee.
   *
   * @param dt
   */
  virtual void
  randomMove(sf::Time dt);

  /**
   * @brief Check if Bee is dead.
   *
   * @return true is energy is zero.
   */
  bool
  isDead();

  /**
   * @brief Evolve the Bee for a given time.
   *
   * Move the bee, change its energy, etc.
   *
   * @param dt
   */
  void
  update(sf::Time dt) override;

  /**
   * @brief Draw the Bee onto target.
   *
   * @param target
   */
  void
  drawOn(sf::RenderTarget& target) const override;

  /**
   * @brief Get current speed.
   *
   * @return speed
   */
  double
  getSpeed() const;

  /**
   * @brief Get the energy of the Bee.
   *
   * @return Energy of the Bee.
   */
  double
  getEnergy() const;

  /**
   * @brief Eat some nectar.
   */
  void
  eatFromHive(sf::Time dt);

  /**
   * @brief Get Bee Hive.
   *
   * @return pointer to current Bee Hive.
   */
  Hive*
  getHive() const;

  /**
   * @brief Get Bee configuration by type.
   *
   * @return Configuration values.
   */
  virtual j::Value
  const&
  getConfig() =0;

  /**
   * @brief Load the Bee texture.
   *
   * Load the Bee texture using getConfig.
   */
  void
  loadTexture();

  /**
   * @brief Find Flower in visible range.
   *
   * @return pointer to Flower.
   */
  Flower*
  findVisibleFlower() const;

  virtual void
  onEnterState(State state) = 0;

protected:

  Hive* hive_;
  Vec2d move_vec_;
  Vec2d move_target_;
  double speed_;
  double energy_;
  double energy_rate_idle_;
  double energy_rate_moving_;
  double energy_rate_eating_;

  sf::Texture texture_;
  double debug_thickness_random_;
  double debug_thickness_target_;

  Collider vision_range_;
  double visibility_;

  State const AT_REST = 0;
  State const RANDOM = 1;
  State const TARGET = 2;
  State move_state_;

};

#endif
