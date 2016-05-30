/**
 * @file World.hpp
 * @brief 
 * @author Ilya Skurikhin
 * @author Julia Besson
 * @version p5.1
 * @date 2016-05-05
 */
#ifndef WORLD_H
#define WORLD_H

#include <SFML/Graphics.hpp>

#include <Interface/ConfigurableInterface.hpp>
#include <Interface/DrawableInterface.hpp>

#include <Utility/Vertex.hpp>
#include <Utility/Utility.hpp>
#include <Utility/Vec2d.hpp>
#include <Utility/Logging.hpp>
#include <JSON/JSON.hpp>
#include <Random/Random.hpp>

#include <Application.hpp>

#include <vector>
#include <array>
#include <fstream>
#include <ostream>
#include <iostream>
#include <sstream>
#include <iomanip>
#include <stdexcept>
#include <string>
#include <cmath>

enum class Kind
  : short
    { Grass, Water, Rock
};

struct Seed
{
  sf::Vector2i position;
  Kind texture;
};

class World : public DrawableInterface, public virtual ConfigurableInterface
{
public:

  /**
   * @brief Default class constructor.
   *
   * Load parameters from configuration file.
   */
  World();

  /**
   * @brief Load the app configuration from the config file.
   *
   * Get and set the size of the world, the size of a cell, the number
   * of seeds and fill a blank Rock world.
   */
  void
  reloadConfig() override;

  /**
   * @brief Generate the texture cache.
   *
   * Generate the Grass, Water and Rock layers and store them to cache.
   */
  void
  reloadCacheStructure();

  /**
   * @brief Draw the current world on a target.
   *
   * @param target where to draw this world
   */
  void
  drawOn(sf::RenderTarget& target) const override;

  /**
   * @brief Set the texture to show in each cell.
   *
   * Load the textures for each layer, set the alpha values of each layer
   * and load them into the cache.
   * Set the blue levels for each cell based on humidity.
   */
  void
  updateCache();

  /**
   * @brief Reload the config and generate a new world.a
   *
   * Use the seeds to generate a new world according to the app config
   * files.
   * 
   * @param regenerate use seeds to generate terrain, then smooth.
   */
  void
  reset(bool regenerate);

  /**
   * @brief Load a map from file.
   *
   * Load a map from the file specified in the config.
   * Throw an error if not possible.
   */
  void
  loadFromFile();

  /**
   * @brief Save a map to file.
   *
   * Save a map to teh file specified in the config.
   */
  void
  saveToFile() const;

  /**
   * @brief Get clamped position
   *
   * @param position graphic position to clamp.
   *
   * @return clamped position
   */
  Vec2d
  clamping(Vec2d& position);

  sf::Vector2i
  clamping(sf::Vector2i& position);

  /**
   * @brief Move all the seeds_, teleport water.
   */
  void
  step();

  /**
   * @brief Move all the seeds_ n times.
   *
   * @param n number of times to move
   * @param update update cache after moving
   */
  void
  steps(unsigned int n, bool update = false);

  /**
   * @brief Smoothen the map.
   *
   * Flood cells near water and grow grass on rock.
   */
  void
  smooth();

  /**
   * @brief Smoothen the map n times.
   *
   * @param n number of times to smoothen
   * @param update update cache after smoothening
   */
  void
  smooths(unsigned int n, bool update = false);

  /**
   * @brief Reset texture of World to Kind::Rock.
   */
  void
  clear();

  void
  calculateHumidityMatrix();

  /**
   * @brief Calculate humidity for whole World.
   */
  void
  humidify();

  /**
   * @brief Calculate humidity contribution of a cell.
   *
   * @param i cell to calculate for.
   */
  void
  humidify(size_t i);

  /**
   * @brief Check if a FLower can be grown.
   *
   * Check for Rocks, Water.
   *
   * @param position
   *
   * @return true is can be grown.  
   * */
  bool
  isGrass(const Vec2d& position) const;

  bool
  isGrass(size_t x, size_t y) const;

  /**
   * @brief Check if the area is all grass.
   *
   * @param topLeft top left corner graphic position
   * @param bottomRight bottom right corner graphic position
   *
   * @return true if is all grass
   */
  bool
  isGrassArea(const Vec2d& topLeft, const Vec2d& bottomRight);

  bool
  isRock(const Vec2d& position) const;

  /**
   * @brief Check if Bee can be flown.
   *
   * Check for presence of Rock.
   *
   * @param position
   *
   * @return true if can fly.
   */
  bool
  isFlyable(Vec2d const& position) const;

  /**
   * @brief Get grid coordinates from graphic coordinates. 
   *
   * @param position graphic position.
   *
   * @return grid coordinates in world vector.
   */

  bool
  isWalkable(Vec2d const& position) const;

  Vec2d
  getCellPosition(const Vec2d& position) const;

  /**
   * @brief Get vector table index from graphic position.
   *
   * @param position graphix position.
   *
   * @return Index of corresponding cell.
   */
  size_t
  getCellIndex(const Vec2d& position) const;

  /**
   * @brief Get humidity for a graphic position. 
   *
   * @param position graphic position.
   *
   * @return humidity
   */
  double
  getHumidity(const Vec2d& position) const;

  /**
   * @brief Check if position is in World bounds.
   *
   * @param position graphic position to check.
   *
   * @return true if is in bounds of World.
   */
  bool
  isInWorld(const Vec2d& position) const;

  const Vec2d&
  getWorldSize() const;
  float
  getCellSize() const;
  size_t
  getNumberColumns() const;

private:

  Vec2d world_size_;
  /// number of cells in a row
  size_t number_columns_;
  /// number of pixels in a cell
  float cell_size_;
  /// grid of all cells
  std::vector<Kind> cells_;

  /// cache of all pixels
  sf::RenderTexture rendering_cache_;
  sf::RenderTexture humidity_cache_;

  std::vector<sf::Vertex> grass_vertexes_;
  std::vector<sf::Vertex> water_vertexes_;
  std::vector<sf::Vertex> rock_vertexes_;

  /// vector of water and grass seeds
  std::vector<Seed> seeds_;
  unsigned int num_water_seeds_;
  unsigned int num_grass_seeds_;
  double water_neighbour_ratio_;
  double grass_neighbour_ratio_;

  double teleport_probability_;

  /// vector of humidity values
  std::vector<double> humidity_levels_;
  std::vector<double> humidity_matrix_;
  std::vector<sf::Vertex> humidity_vertexes_;
  double humidity_range_;
  double humidity_threshold_;
  double humidity_initial_level_;
  double humidity_decay_rate_;

  /**
   * Find the cells to scan for given radius.
   * Make sure that we do not go over boundaries.
   */
  std::array<size_t, 4>
  calculateScanRange(size_t x, size_t y, unsigned int radius);
};
#endif
