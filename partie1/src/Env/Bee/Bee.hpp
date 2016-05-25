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

#include <Interface/ConfigurableInterface.hpp>
#include <Interface/DrawableInterface.hpp>
#include <Interface/UpdatableInterface.hpp>

#include <Utility/Vec2d.hpp>
#include <Utility/Utility.hpp>
#include <Utility/Logging.hpp>
#include <Random/Random.hpp>

#include <Env/World.hpp>
#include <Env/Collider.hpp>
#include <Env/Env.hpp>
#include <Env/CFSM.hpp>
#include <Env/Movable.hpp>
#include <Env/Cave.hpp>

#include <Application.hpp>

#include <cmath>

class ScoutBee;
class WorkerBee;
class Hive;
class Flower;

enum class BeeType : short
{
  Worker, Scout, Queen, Soldier
};

/**
 * @brief Abstract class Bee.
 */
class Bee : public DrawableInterface,
    public UpdatableInterface,
    public virtual ConfigurableInterface,
    public Movable,
    public CFSM
{
public:

  /**
   * @brief Class constructor.
   *
   * @param hive      Where the bee will return to.
   * @param position  The starting position of the bee.
   * @param states    The possible CFSM states of the bee.
   */
  Bee(Hive* hive, const Vec2d& position, std::vector<State> states, BeeType beeType);

  virtual
  ~Bee() = default;

  /**
   * @brief Get Bee configuration by type.
   *
   * @return Configuration values.
   */
  virtual j::Value const&
  getConfig() const =0;

  /**
   * @brief Move the bee.
   *
   * @param dt Time for the counter.
   */
  void
  move(sf::Time dt) override;

  bool
  isMovablePosition(const Vec2d& position) const override;

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

  virtual void
  drawDebug(sf::RenderTarget& target) const=0;

  /**
   * @brief Reload the configuration of attributes from JSON.
   */
  void
  reloadConfig() override;

  /**
   * @brief Get the energy of the Bee.
   *
   * @return Energy of the Bee.
   */
  double
  getEnergy() const;

  void
  setEnergy(double energy);

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

  void
  setDebugStatus(const std::string& status);

  const std::string&
  getDebugStatus() const;

  virtual bool
  isInHive() const =0;

  virtual void
  interact(Bee* other)=0;

  virtual void
  interactWith(ScoutBee* scouting)=0;

  virtual void
  interactWith(WorkerBee* working)=0;

  void
  setHive(Hive* hive);

  double
  getVisibility() const;

  void
  setVisibility(double visibility);

  const Collider&
  getVisionRange() const;

  void
  setVisionRange(const Collider& visionRange);

  const BeeType&
  getBeeType() const;

private:

  Hive* hive_;

  BeeType bee_type_;

  double energy_;
  double energy_rate_idle_;
  double energy_rate_moving_;
  double energy_rate_eating_;

  sf::Texture texture_;
  double debug_thickness_random_;
  double debug_thickness_target_;

  Collider vision_range_;
  double visibility_;

  std::string debug_status_;

};

#endif
