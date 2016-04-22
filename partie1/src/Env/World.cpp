#include <Env/World.hpp>

j::Value
simulationWorld ()
{
  return getAppConfig ()["simulation"]["world"];
}

World::World ()
{
  logEvent ("World", "building\tnew world");

  reloadConfig ();
  reloadCacheStructure ();

  // try to load a new world from file
  try
    {
      loadFromFile ();
    }
  catch (const std::runtime_error& e)
    {
      reset (true);
    }
}

void
World::reloadConfig ()
{
  logEvent ("World", "loading\tconfig");

  // numberbColumns : number of cells in a row
  numberColumns_ = simulationWorld ()["cells"].toInt ();

  // cellSize : size of a cell in 'pixels'
  cellSize_ = simulationWorld ()["size"].toDouble () / numberColumns_;

  // make a vector representing a square grid of Kind::Rock
  cells_ = std::vector<Kind> (numberColumns_ * numberColumns_, Kind::Rock);

  // get the number of seeds from configd
  nbWaterSeeds_ = simulationWorld ()["seeds"]["water"].toInt ();
  nbGrassSeeds_ = simulationWorld ()["seeds"]["grass"].toInt ();

  teleportProbability_ =
      simulationWorld ()["seeds"]["water teleport probability"].toDouble ();

  // make an empty vector of Seed
  std::vector<Seed> tmpSeeds (nbGrassSeeds_ + nbWaterSeeds_);
  seeds_ = tmpSeeds;

  std::vector<double> tmpHumidityLevels (numberColumns_ * numberColumns_);
  humidityLevels_ = tmpHumidityLevels;

  // set the humidity data from file
  humidityThreshold_ =
      simulationWorld ()["generation"]["humidity"]["threshold"].toDouble ();
  humidityInitialLevel_ =
      simulationWorld ()["generation"]["humidity"]["initial level"].toDouble ();
  humidityDecayRate_ =
      simulationWorld ()["generation"]["humidity"]["decay rate"].toDouble ();

  // set the humidityRange_
  humidityRange_ = 0;
  double humidity (humidityThreshold_ + 1);
  while (humidity > humidityThreshold_)
    {
      humidity = humidityInitialLevel_
          * std::exp ((double) -humidityRange_ / (double) humidityDecayRate_);
      ++humidityRange_;
    }
  appendLog ("World\tset\thumidityRange_ = " + std::to_string (humidityRange_));
}

void
World::reloadCacheStructure ()
{
  logEvent ("World", "loading\tcache structure");

  grassVertexes_ = generateVertexes (simulationWorld ()["textures"],
                                     numberColumns_, cellSize_);
  waterVertexes_ = grassVertexes_;
  rockVertexes_ = grassVertexes_;

  humidityVertexes_ = grassVertexes_;

  // create a cache of all the pixels to display
  renderingCache_.create (numberColumns_ * cellSize_,
                          numberColumns_ * cellSize_);
  humidityCache_.create (numberColumns_ * cellSize_,
                         numberColumns_ * cellSize_);
}

void
World::updateCache ()
{
  logEvent ("World", "updating\tcache");

  sf::RenderStates rsGrass;
  rsGrass.texture = &getAppTexture (
      simulationWorld ()["textures"]["grass"].toString ());
  sf::RenderStates rsWater;
  rsWater.texture = &getAppTexture (
      simulationWorld ()["textures"]["water"].toString ());
  sf::RenderStates rsRock;
  rsRock.texture = &getAppTexture (
      simulationWorld ()["textures"]["rock"].toString ());

  sf::RenderStates rsHumidity;

  // get and set max and min humidity levels
  auto minmax (
      std::minmax_element (humidityLevels_.begin (), humidityLevels_.end ()));
  double minHumidity (*minmax.first);
  double maxHumidity (*minmax.second);

  // iterate through the cells
  size_t size (cells_.size ());
  for (size_t i (0); i < size; ++i)
    {

      // use mod to get column number
      size_t x (i % numberColumns_);
      // use integer division to get line number
      size_t y (i / numberColumns_);

      // get the indexes of the vertixes of current cell
      std::vector<size_t> positionIndexes = indexesForCellVertexes (
          x, y, numberColumns_);

      // get the right alpha values of the three layers
      std::array<int, 3> aValues;
      switch (cells_[i])
        {
        case Kind::Grass:
          aValues =
            { 255,0,0};
          break;
          case Kind::Water :
          aValues =
            { 0,255,0};
          break;
          case Kind::Rock :
          aValues =
            { 0,0,255};
          break;
          default :
          aValues =
            { 0,0,0};
          break;
        }

      // get the right blue level for the humidity layer
      int blueLevel (
          ((humidityLevels_[i] - minHumidity) / (maxHumidity - minHumidity))
              * 255);

      // set the right color values for the layers
      for (size_t j (0); j < 4; ++j)
        {
          // set alpha values for textures
          grassVertexes_[positionIndexes[j]].color.a = aValues[0];
          waterVertexes_[positionIndexes[j]].color.a = aValues[1];
          rockVertexes_[positionIndexes[j]].color.a = aValues[2];

          // set humidity levels
          humidityVertexes_[positionIndexes[j]].color = sf::Color (0, 0,
                                                                   blueLevel);
        }
    }

  renderingCache_.clear ();
  humidityCache_.clear ();

  renderingCache_.draw (grassVertexes_.data (), grassVertexes_.size (),
                        sf::Quads, rsGrass);
  renderingCache_.draw (waterVertexes_.data (), waterVertexes_.size (),
                        sf::Quads, rsWater);
  renderingCache_.draw (rockVertexes_.data (), rockVertexes_.size (), sf::Quads,
                        rsRock);
  // draw the humidity on a new cache
  humidityCache_.draw (humidityVertexes_.data (), humidityVertexes_.size (),
                       sf::Quads, rsHumidity);
  humidityCache_.display ();

  renderingCache_.display ();
}

void
World::reset (bool regenerate)
{
  logEvent ("World", "resetting\tworld");

  reloadConfig ();
  reloadCacheStructure ();

  // set the first part of seeds_ as grass
  for (size_t i (0); i < nbGrassSeeds_; ++i)
    {
      seeds_[i].texture = Kind::Grass;
    }

  // complete seeds_ with water
  for (size_t i (nbGrassSeeds_); i < seeds_.size (); ++i)
    {
      seeds_[i].texture = Kind::Water;
    }

  // set random positions for the seeds_
  size_t min (0);
  size_t max (numberColumns_ - 1);
  for (size_t i (0); i < seeds_.size (); ++i)
    {
      seeds_[i].position.x = uniform (min, max);
      seeds_[i].position.y = uniform (min, max);
    }

  // unless they landed in water, plant the seeds
  for (size_t i (0); i < seeds_.size (); ++i)
    {
      size_t index (
          seeds_[i].position.y * numberColumns_ + seeds_[i].position.x);
      if (cells_[index] != Kind::Water)
        {
          cells_[index] = seeds_[i].texture;
        }
    }

  if (regenerate)
    {
      logEvent ("World", "generating\tworld");

      steps (simulationWorld ()["generation"]["steps"].toInt ());
      smooths (
          simulationWorld ()["generation"]["smoothness"]["level"].toInt ());
      humidify ();
    }

  updateCache ();
}

void
World::drawOn (sf::RenderTarget& target) const
{
  if (simulationWorld ()["show humidity"].toBool ()
      || getAppConfig ()["debug"].toBool ())
    {
      sf::Sprite cache (humidityCache_.getTexture ());
      target.draw (cache);
    }
  else
    {
      sf::Sprite cache (renderingCache_.getTexture ());
      target.draw (cache);
    }

  // if debug is on, show individual levels   
  if (isDebugOn ())
    {
      Vec2d position = getApp ().getCursorPositionInView ();
      if (isInWorld (position))
        {
          size_t cell (getCellIndex (position));
          sf::Text text = buildText (to_nice_string (humidityLevels_[cell]),
                                     position, getAppFont (), 30,
                                     sf::Color::Red);
          target.draw (text);
        }
    }

}

void
World::loadFromFile ()
{
  logEvent ("World", "loading\tworld from file");

  // get app values for current config
  reloadConfig ();

  // open config .map file
  std::string fileName (
      getApp ().getResPath () + simulationWorld ()["file"].toString ());
  std::ifstream input (fileName.c_str ());

  // check existance of file
  if (input.fail ())
    {
      throw std::runtime_error ("Input map file not found.");
    }
  else
    {
      std::string numberColumns;
      std::string cellSize;

      // read the file header
      input >> numberColumns;
      numberColumns_ = stoi (numberColumns);
      input >> std::ws;

      input >> cellSize;
      cellSize_ = stof (cellSize);
      input >> std::ws;

      // clear cells_ before writing
      cells_.clear ();

      // read .map file values for cell Kind
      size_t size (numberColumns_ * numberColumns_);
      for (size_t i (0); i < size; ++i)
        {
          short var;
          input >> var;
          Kind value = static_cast<Kind> (var);
          cells_.push_back (value);
        }

      // read .map file values for cell humidity
      for (size_t i (0); i < size; ++i)
        {
          double humidity;
          input >> humidity;
          humidityLevels_[i] = humidity;
        }
    }

  input.close ();

  reloadCacheStructure ();
  updateCache ();
}

void
World::saveToFile () const
{
  logEvent ("World", "saving\tworld to file");

  // open config .map file
  std::string fileName (
      getApp ().getResPath () + simulationWorld ()["file"].toString ());
  std::ofstream output (fileName.c_str ());

  // check if output can be made
  if (output.fail ())
    {
      throw std::runtime_error ("Output map file cannot be made.");
    }
  else
    {

      // write the file header
      output << numberColumns_ << std::endl;
      output << cellSize_ << std::endl;

      // write the map contents
      size_t size (numberColumns_ * numberColumns_);
      for (size_t i (0); i < size; ++i)
        {
          switch (cells_[i])
            {
            case Kind::Grass:
              output << "0";
              break;
            case Kind::Water:
              output << "1";
              break;
            case Kind::Rock:
              output << "2";
              break;
            default:
              output << "2";
              break;
            }
          output << " ";
        }

      output << std::endl;

      // write the humidity
      for (size_t i (0); i < size; ++i)
        {
          output << humidityLevels_[i] << " ";
        }
    }

  output.close ();
}

void
World::step ()
{
  // declare here to save time later
  bool teleport;

  // array of directions to choose from
  std::array<std::array<int, 2>, 4> directions;
  directions[0] =
    { 1,0};
  directions[1] =
    { -1,0};
  directions[2] =
    { 0,1};
  directions[3] =
    { 0,-1};

  // loop through seeds_ to move them
  for (size_t i (0); i < seeds_.size (); ++i)
    {
      /* logEvent("World", "moving seed " + std::to_string(i));*/

      // get the teleport bool
      teleport = bernoulli (teleportProbability_);

      // choose to move or teleport
      if ((seeds_[i].texture == Kind::Water) && teleport)
        {
          // teleport to random location
          seeds_[i].position.x = uniform ((size_t) 0, numberColumns_ - 1);
          seeds_[i].position.y = uniform ((size_t) 0, numberColumns_ - 1);
        }
      else
        {
          // pick a random direction
          size_t index (uniform (0, 3));
          seeds_[i].position.x += directions[index][0];
          seeds_[i].position.y += directions[index][1];

          // clamp the seed to the board
          seeds_[i].position.x = std::max ((int) 0, (int) seeds_[i].position.x);
          seeds_[i].position.x = std::min ((int) numberColumns_ - 1,
                                           (int) seeds_[i].position.x);
          seeds_[i].position.y = std::max ((int) 0, (int) seeds_[i].position.y);
          seeds_[i].position.y = std::min ((int) numberColumns_ - 1,
                                           (int) seeds_[i].position.y);
        }

      // set the texture of the seed to cells
      cells_[(seeds_[i].position.y * numberColumns_) + seeds_[i].position.x] =
          seeds_[i].texture;
    }
}

void
World::steps (unsigned int n, bool update)
{
  logEvent ("World", "moving seeds");

  for (unsigned int i (0); i < n; ++i)
    {
      //logEvent("World", "moving seeds (" + std::to_string(i) + "/" + std::to_string(n) + ")", false);
      step ();
    }

  if (update)
    {
      humidify ();
      updateCache ();
    }
}

void
World::smooth ()
{
  // copy cells_ so as to not have directional bias
  // decisions are made on original, written in copy
  std::vector<Kind> localCells = cells_;
  double sWaterRatio (
      simulationWorld ()["generation"]["smoothness"]["water neighbourhood ratio"].toDouble ());
  double sGrassRatio (
      simulationWorld ()["generation"]["smoothness"]["grass neighbourhood ratio"].toDouble ());
  // Initialize counters for neighbors
  double nbGrass (0);
  double nbWater (0);
  double nbRock (0);

  for (size_t i (0); i < cells_.size (); ++i)
    {

      // get the indexes for the cell
      size_t x (i % numberColumns_);
      size_t y (i / numberColumns_);
      /*
       // set the radius of neighborhood
       unsigned int radius (1);

       // set the start and end of neighborhood
       // possibility of adjusting to toric here
       sf::Rect<size_t> scanRange = calculateScanRange (x, y, radius);

       // list through the neighbors
       for (size_t dx (scanRange.left); dx <= scanRange.left + scanRange.width;
       ++dx)
       {
       for (size_t dy (scanRange.top);
       dy <= scanRange.top + scanRange.height; ++dy)
       {
       */
      size_t left (std::max ((int) x - 1, 0));
      size_t right (std::min ((int) x + 2, (int) numberColumns_ - 1));
      size_t top (std::max ((int) y - 1, 0));
      size_t bottom (std::min ((int) y + 2, (int) numberColumns_ - 1));

      for (size_t column (left); column < right; ++column)
        {
          for (size_t row (top); row < bottom; ++row)
            {
              // check that we are not on original cell 
              if (!((column == x) && (row == y)))
                {

                  // increment for each type of cell
                  switch (cells_[row * numberColumns_ + column])
                    {
                    case Kind::Rock:
                      ++nbRock;
                      break;
                    case Kind::Water:
                      ++nbWater;
                      break;
                    case Kind::Grass:
                      ++nbGrass;
                      break;
                    default:
                      break;
                    }
                }
            }
        }

      // define the water and grass ratios
      double waterRatio (nbWater / (nbRock + nbGrass + nbWater));
      double grassRatio (nbGrass / (nbRock + nbGrass + nbWater));
      switch (cells_[i])
        {
        case Kind::Rock:
          if (waterRatio > sWaterRatio)
            {
              localCells[i] = Kind::Water;
              humidify (i);
            }
          else if (grassRatio > sGrassRatio)
            {
              localCells[i] = Kind::Grass;
            }
          break;
        case Kind::Grass:
          if (waterRatio > sWaterRatio)
            {
              localCells[i] = Kind::Water;
            }
          break;
        case Kind::Water:
          break;
        default:
          break;
        }
    }
  // swap the changes and original cells_
  std::swap (cells_, localCells);
}

void
World::smooths (unsigned int n, bool update)
{
  logEvent ("World", "smoothing world");

  for (unsigned int i (0); i < n; ++i)
    {
      logEvent (
          "World",
          "smoothing (" + std::to_string (i) + "/" + std::to_string (n) + ")",
          false);
      smooth ();
    }

  if (update)
    {
      updateCache ();
      humidify ();
    }
}

void
World::humidify ()
{
  logEvent ("World", "calculating\tglobal humidity");

  size_t size (numberColumns_ * numberColumns_);
  for (size_t i (0); i < size; ++i)
    {
      if (cells_[i] == Kind::Water)
        {
          humidify (i);
        }
    }
}

void
World::humidify (size_t i)
{
  size_t x (i % numberColumns_);
  size_t y (i / numberColumns_);

  size_t left (std::max (x - humidityRange_, 0));
  size_t right (std::min (x + humidityRange_, numberColumns_));
  size_t top (std::max (y - humidityRange_, 0));
  size_t bottom (std::min (y + humidityRange_, numberColumns_));

  for (size_t column (left); column < right; ++column)
    {
      for (size_t row (top); row < bottom; ++row)
        {
          double currentLevel (
              humidityInitialLevel_
                  * exp (
                      -std::hypot (x - column, y - row) / humidityDecayRate_x));
          humidityLevels_[row * numberColumns_ + column] += currentLevel;
        }
    }
}

sf::Rect<size_t>
World::calculateScanRange (size_t x, size_t y, unsigned int radius)
{
  sf::Rect<size_t> scanRange;

  // find start x
  if (x >= radius)
    {
      scanRange.left = x - radius;
    }
  else
    {
      scanRange.left = 0;
    }

  // find start y
  if (y >= radius)
    {
      scanRange.top = y - radius;
    }
  else
    {
      scanRange.top = 0;
    }

  // find width
  if ((x + radius) <= numberColumns_)
    {
      scanRange.width = radius;
    }
  else
    {
      scanRange.width = numberColumns_ - x;
    }

  // find height
  if ((y + radius) <= numberColumns_)
    {
      scanRange.height = radius;
    }
  else
    {
      scanRange.height = numberColumns_ - y;
    }

  return scanRange;
}

bool
World::isGrowable (const Vec2d& position) const
{
  if (cells_[getCellIndex (position)] == Kind::Grass)
    {
      return true;
    }
  else
    {
      return false;
    }
}

Vec2d
World::getCellPosition (const Vec2d& position) const
{
  Vec2d cellPosition;
  cellPosition.x = position.x / (int) cellSize_;
  cellPosition.y = position.y / (int) cellSize_;
  return cellPosition;
}

size_t
World::getCellIndex (const Vec2d& position) const
{
  if (!isInWorld (position))
    {
      throw std::runtime_error ("Position not in world. (World::getIndex)");
    }
  Vec2d cellPosition;
  cellPosition = getCellPosition (position);
  size_t index (
      (size_t) cellPosition.y * numberColumns_ + (size_t) cellPosition.x);
  if (index > numberColumns_ * numberColumns_)
    {
      throw std::runtime_error ("Index not in world. (World::getIndex)");
    }
  return index;
}

double
World::getHumidity (const Vec2d& position) const
{
  return humidityLevels_[getCellIndex (position)];
}

bool
World::isInWorld (const Vec2d& position) const
{
  if ((position.x > numberColumns_ * cellSize_)
      || (position.y > numberColumns_ * cellSize_) || (position.x < 0)
      || (position.y < 0))
    {
      return false;
    }
  else
    {
      return true;
    }
}
