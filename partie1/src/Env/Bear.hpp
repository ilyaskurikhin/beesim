/*
 * Bear.hpp
 *
 *  Created on: May 13, 2016
 *      Author: ilya
 */

#ifndef BEAR_HPP_
#define BEAR_HPP_

#include <SFML/Graphics.hpp>
#include <SFML/System.hpp>

#include <Interface/DrawableInterface.hpp>
#include <Interface/UpdatableInterface.hpp>
#include <Interface/ConfigurableInterface.hpp>

#include <Utility/Vec2d.hpp>
#include <Utility/Logging.hpp>

#include <Env/Movable.hpp>
#include <Env/CFSM.hpp>
#include <Env/Hive.hpp>

class Cave;

class Bear : public DrawableInterface,
    public UpdatableInterface,
    public virtual ConfigurableInterface,
    public Movable,
    public CFSM
{
public:

  Bear(Cave* cave, const Vec2d& position);

  ~Bear();

  Bear(const Bear& bear) = delete;

  Bear
  operator=(const Bear& bear) = delete;

  j::Value const&
  getConfig();

  /**
   * @brief Eat some nectar.
   */
  double
  eatHoney(Hive* hive, sf::Time dt);

  bool
  isInCave() const;

  bool
  isWalking() const;

  /**
   * @brief Load the Bear texture.
   *
   * Load the Bear texture using getConfig.
   */
  void
  loadTexture();

  void
  chooseTexture();

  /**
   * @brief Find Hive in visible range.
   *
   * @return pointer to Hive.
   */
  Hive*
  findVisibleHive() const;

  bool
  isDead();

  void
  setMoveState(const State& state);

  void
  update(sf::Time dt) override;

  /**
   * @brief Draw the Bear onto target.
   *
   * @param target
   */
  void
  drawOn(sf::RenderTarget& target) const override;


  void
  reloadConfig() override;

  void
  onState(State state, sf::Time dt) override;

  void
  onEnterState(State state) override;

  void
  move(sf::Time dt) override;

  bool
  isMovablePosition(const Vec2d& position) const override;

  void
  setDebugStatus(const std::string& status);

  const std::string&
  getDebugStatus() const;

  Hive*
  getHive() const;

  void
  setHive(Hive* hive);

  Cave*
  getCave() const;

  void
  setCave(Cave* cave);

  double
  getEnergy() const;

  void
  setEnergy(double energy);

  /*
  double 
  getEatenHoney() const;
  
  void 
  setEatenHoney(double honey);
  */
  
  double
  getHibernationLength() const;

  const Vec2d&
  getHiveLocation() const;

  void
  setHiveLocation(const Vec2d& hiveLocation);

  State
  getMoveState() const;

  double
  getVisibility() const;

  const Collider&
  getVisionRange() const;

private:

  Cave* cave_;

  Vec2d hive_location_;

  double energy_;
  double energy_leave_cave_;
  double energy_rate_idle_;
  double energy_rate_eating_;
  double energy_rate_moving_;
  double energy_seek_hives_;

  double honey_eating_rate_;
  double max_honey_capacity_;

  size_t current_texture_index_;
  std::vector<sf::Texture> walking_textures_;

  sf::Clock texture_counter_;
  sf::Time texture_delay_;

  double debug_thickness_random_;
  double debug_thickness_target_;

  Collider vision_range_;
  double visibility_;

  State const AT_REST = 0;
  State const RANDOM = 1;
  State const TARGET = 2;
  State move_state_;

  std::string debug_status_;

  sf::Time hibernation_length_;
  sf::Time max_hibernation_;

  State static const SEARCH_HIVE;
  State static const EAT_HONEY;
  State static const RETURN_CAVE;
  State static const HIBERNATION;

};

#endif /* BEAR_HPP_ */
