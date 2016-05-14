/*
 * Bear.hpp
 *
 *  Created on: May 13, 2016
 *      Author: ilya
 */

#ifndef BEAR_HPP_
#define BEAR_HPP_

#include <Interface/DrawableInterface.hpp>
#include <Interface/UpdatableInterface.hpp>
#include <Interface/ConfigurableInterface.hpp>

#include <Env/Movable.hpp>
#include <Env/CFSM.hpp>

#include <Utility/Vec2d.hpp>
#include <Utility/Logging.hpp>

class Cave;

class Bear : public DrawableInterface, public UpdatableInterface, public virtual ConfigurableInterface, public Movable, public CFSM
{
public:

  Bear(const Vec2d& position, std::vector<State> states);

  ~Bear();

  void
  reloadConfig() override;
  
  j::Value const&
  getConfig();
  
  void 
  move(sf::Time dt);

  bool
  isMovablePosition(const Vec2d& position) const override;


 void
  update(sf::Time dt) override;

  /**
   * @brief Draw the Bear onto target.
   *
   * @param target
   */
  void
  drawOn(sf::RenderTarget& target) const override;

  /**
   * @brief Get the energy of the Bear.
   *
   * @return Energy of the Bear.
   */
  double
  getEnergy() const;

  /**
   * @brief Eat some nectar.
   */
  void
  eatHoney(sf::Time dt);

  /**
   * @brief Get bear cave.
   *
   * @return pointer to current bear cave.
   */
  Cave*
  getCave() const;

  bool
  isInCave() const;

  /**
   * @brief Load the Bear texture.
   *
   * Load the Bear texture using getConfig.
   */
  void
  loadTexture();

  /**
   * @brief Find Hive in visible range.
   *
   * @return pointer to Hive.
   */
  Hive*
  findVisibleHive() const;

  bool
  isDead();

  virtual void
  onEnterState(State state);

  void
  setMoveStateAT_REST();

  void
  setMoveStateRANDOM();

  void
  setMoveStateTARGET();

  void
  setDebugStatus(const std::string& status);

  const std::string&
  getDebugStatus() const;

  Hive* 
  getHive() const;
  
  void
  setHive(Hive* hive);

  State static const SEARCH_HIVE;
  State static const EAT_HONEY;
  State static const RETURN_CAVE;
  State static const HIBERNATION;



private:

  Cave* cave_;
  
  Vec2d hive_location_;
  
  double energy_;
  double energy_leave_cave_;
  double energy_rate_eating_;
  double energy_rate_moving_;
  double energy_seek_hives_;
  double max_hibernation_;

  sf::Texture texture_;
  double debug_thickness_random_;
  double debug_thickness_target_;

  Collider vision_range_;
  double visibility_;

  State const AT_REST = 0;
  State const RANDOM = 1;
  State const TARGET = 2;
  State move_state_;

  std::string debug_status_;

  sf::Time hibernationLength_;

};



#endif /* BEAR_HPP_ */
