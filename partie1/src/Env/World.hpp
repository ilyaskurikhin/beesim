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
#include <Utility/Vertex.hpp>
#include <Utility/Utility.hpp>
#include <Utility/Vec2d.hpp>
#include <Utility/Logging.hpp>
#include <Application.hpp>
#include <Random/Random.hpp>
#include <JSON/JSON.hpp>
#include <Interface/Drawable.hpp>

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

class World : public Drawable
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
  reloadConfig();

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
  isGrowable(const Vec2d& position) const;

  bool
  isGrowable(size_t x, size_t y) const;

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

  /**
   * @brief Check if Hive can be build.
   *
   * Check for Rock, Water and other Hives.
   *
   * @param position graphic position to check for.
   * @param radius radius to check.
   *
   * @return true is can be placed.
   */
  bool
  isHiveable(const Vec2d& position, double radius);

private:

  /// number of cells in a row
  size_t numberColumns_;
  /// number of pixels in a cell
  float cellSize_;
  /// grid of all cells
  std::vector<Kind> cells_;

  /// cache of all pixels
  sf::RenderTexture renderingCache_;
  sf::RenderTexture humidityCache_;

  std::vector<sf::Vertex> grassVertexes_;
  std::vector<sf::Vertex> waterVertexes_;
  std::vector<sf::Vertex> rockVertexes_;

  /// vector of water and grass seeds
  std::vector<Seed> seeds_;
  unsigned int nbWaterSeeds_;
  unsigned int nbGrassSeeds_;

  double teleportProbability_;

  /// vector of humidity values
  std::vector<double> humidityLevels_;
  std::vector<double> humidityMatrix_;
  std::vector<sf::Vertex> humidityVertexes_;
  double humidityRange_;
  double humidityThreshold_;
  double humidityInitialLevel_;
  double humidityDecayRate_;

  /**
   * Find the cells to scan for given radius.
   * Make sure that we do not go over boundaries.
   */
  std::array<size_t, 4>
  calculateScanRange(size_t x, size_t y, unsigned int radius);
};
#endif
