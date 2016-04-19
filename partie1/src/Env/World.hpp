#ifndef WORLD_H
#define WORLD_H

#include <SFML/Graphics.hpp>
#include <Utility/Vertex.hpp>
#include <Utility/Utility.hpp>
#include <Utility/Vec2d.hpp>
#include <Application.hpp>
#include <Random/Random.hpp>
#include <JSON/JSON.hpp>

#include <Interface/Drawable.hpp>

#include <vector>
#include <array>
#include <fstream>
#include <ostream>
#include <iostream>
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
  World ();

  /**
   * @brief Load the app configuration from the config file.
   *
   * Get and set the size of the world, the size of a cell, the number
   * of seeds and fill a blank Rock world.
   */
  void
  reloadConfig ();

  /**
   * Generate the Grass, Water and Rock layers and store them to cache.
   */
  void
  reloadCacheStructure ();

  /**
   * Draw the current world on a target
   * @param target where to draw this world
   */
  void
  drawOn (sf::RenderTarget& target) const;

  /**
   * Set the layer to show in each cell.
   * Load the textures for each layer, set the alpha values of each layer
   * and load them into the cache.
   * Set the blue levels for each cell based on humidity.
   */
  void
  updateCache ();

  /**
   * Reload the config and generate a new world.
   * Use the seeds to generate a new world according to the app config
   * files.
   * @param regenerate use seeds to generate terrain, then smooth.
   */
  void
  reset (bool regenerate);

  /**
   * @brief Load a map from file.
   * Load a map from the file specified in the config.
   * Throw an error if not possible.
   */
  void
  loadFromFile ();

  /**
   * Save a map to file.
   * Save a map to teh file specified in the config.
   */
  void
  saveToFile () const;

  /**
   * Move a seed.
   * Move a seed within a given range.
   * @param seed the seed to move
   * @param min the left and bottom distance
   * @param max the right and top distance
   */
  void
  moveSeed (Seed& seed, int min, int max);

  /**
   * Move all the seeds_, teleport water.
   */
  void
  step ();

  /**
   * Move all the seeds_ n times.
   * @param n number of times to move
   * @param update update cache after moving
   */
  void
  steps (unsigned int n, bool update = false);

  /**
   * Smoothen the map.
   * Flood cells near water and grow grass on rock.
   */
  void
  smooth ();

  /**
   * Smoothen the map n times.
   * @param n number of times to smoothen
   * @param update update cache after smoothening
   */
  void
  smooths (unsigned int n, bool update = false);

  void
  humidify ();

  void
  humidify (size_t i);

  bool
  isGrowable (const Vec2d& position) const;

  Vec2d
  positionInWorld (const Vec2d& position) const;

  /**
   * @brief Get cell number for current position.
   */
  size_t
  positionInTab (const Vec2d& position) const;

  size_t
  getIndex (const Vec2d& position) const;

  double
  getHumidity (const Vec2d& position) const;

  bool
  isInWorld (const Vec2d& position) const;

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

  /// vector of humidity values
  std::vector<double> humidityLevels_;
  std::vector<sf::Vertex> humidityVertexes_;
  double humidityRange_;
  double humidityThreshold_;
  double humidityInitialLevel_;
  double humidityDecayRate_;

  /**
   * Find the cells to scan for given radius.
   * Make sure that we do not go over boundaries.
   */
  sf::Rect<size_t>
  calculateScanRange (size_t x, size_t y, unsigned int radius);
};
#endif
