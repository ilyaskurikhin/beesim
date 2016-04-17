#include <Env/World.hpp>



j::Value
simulationWorld()
{
    return getAppConfig()["simulation"]["world"];
}



World::World()
{
    reloadConfig();
    reloadCacheStructure();
}



void
World::reloadConfig() 
{
    // numberbColumns : number of cells in a row
    numberColumns_= simulationWorld()["cells"].toInt();

    // cellSize : size of a cell in 'pixels'
    cellSize_= simulationWorld()["size"].toDouble() / numberColumns_;

    // make a vector representing a square grid of Kind::Rock
    cells_ = std::vector<Kind> (numberColumns_ * numberColumns_, Kind::Rock);

    // get the number of seeds from configd
    nbWaterSeeds_ = simulationWorld()["seeds"]["water"].toInt();
    nbGrassSeeds_ = simulationWorld()["seeds"]["grass"].toInt();

    // make an empty vector of Seed
    std::vector<Seed> tmp(nbGrassSeeds_ + nbWaterSeeds_);
    seeds_ = tmp; 

    // set the humidity data from file
    humidityThreshold_ = simulationWorld()["generation"]
                                    ["humidity"]["threshold"].toDouble();
    humidityInitialLevel_ = simulationWorld()["generation"]
                                    ["humidity"]["initial level"].toDouble();
    humidityDecayRate_ = simulationWorld()["generation"]
                                    ["humidity"]["decay rate"].toDouble();

    // set the humidityRange_
    humidityRange_ = 0;
    double humidity;
    while (humidity > humidityThreshold_) {
        humidity = humidityInitialLevel_ 
                    * std::exp((float) -humidityRange_ / (float) humidityDecayRate_);
        ++humidityRange_;
    }
}



void
World::reloadCacheStructure() 
{
    grassVertexes_ = generateVertexes(simulationWorld()["textures"],
                        numberColumns_, cellSize_);
    waterVertexes_ = grassVertexes_;
    rockVertexes_ = grassVertexes_;

    humidityVertexes_ = grassVertexes_;

    // create a cache of all the pixels to display
    renderingCache_.create(numberColumns_* cellSize_, 
                        numberColumns_ * cellSize_);
}



void
World::updateCache() 
{
    sf::RenderStates rsGrass;
    rsGrass.texture = &getAppTexture(simulationWorld()["textures"]
                        ["grass"].toString()); 
    sf::RenderStates rsWater;
    rsWater.texture = &getAppTexture(simulationWorld()["textures"]
                        ["water"].toString());
    sf::RenderStates rsRock;
    rsRock.texture = &getAppTexture(simulationWorld()["textures"]
                        ["rock"].toString()); 

    // get and set max and min humidity levels
    auto minmax(std::minmax_element(humidityLevels_.begin(), 
                                    humidityLevels_.end()));
    double minHumidity(*minmax.first);
    double maxHumidity(*minmax.second);

    // iterate through the cells
    size_t size(cells_.size());
    for (size_t i(0); i < size; ++i) {

        // use mod to get column number
        size_t x(i % numberColumns_);
        // use integer division to get line number
        size_t y(i / numberColumns_);

        // get the indexes of the vertixes of current cell
        std::vector<size_t> positionIndexes = indexesForCellVertexes(x, y, 
                                                            numberColumns_);
        
        // get the right alpha values of the three layers
        std::array<int,3> aValues;
        switch (cells_[i]) {
            case Kind::Grass :
                aValues = {255,0,0};
                break;
            case Kind::Water :
                aValues = {0,255,0};
                break;
            case Kind::Rock :
                aValues = {0,0,255};
                break;
            default :
                aValues = {0,0,0};
                break;
        }

        // get the right blue level for the humidity layer
        int blueLevel((humidityLevels_[i] - minHumidity) 
                        - (maxHumidity - minHumidity) * 255);

        // set the right color values for the layers
        for (size_t j(0); j < 4; ++j) {
            // set alpha values for textures
            grassVertexes_[positionIndexes[j]].color.a = aValues[0];
            waterVertexes_[positionIndexes[j]].color.a = aValues[1];
            rockVertexes_[positionIndexes[j]].color.a = aValues[2];

            // set humidity levels
            humidityVertexes_[positionIndexes[j]].color.b = blueLevel;
        }
    }

    renderingCache_.clear();
    
    renderingCache_.draw(grassVertexes_.data(), grassVertexes_.size(), 
                         sf::Quads, rsGrass);
    renderingCache_.draw(waterVertexes_.data(), waterVertexes_.size(), 
                         sf::Quads, rsWater);
    renderingCache_.draw(rockVertexes_.data(), rockVertexes_.size(), 
                         sf::Quads, rsRock);

    renderingCache_.display();
}



void
World::reset(bool regenerate)
{
    reloadConfig();
    reloadCacheStructure();
  
    // set the first part of seeds_ as grass
    for (size_t i(0); i < nbGrassSeeds_; ++i) {
        seeds_[i].texture = Kind::Grass;
    }

    // complete seeds_ with water
    for (size_t i(nbGrassSeeds_); i < seeds_.size(); ++i) {
        seeds_[i].texture = Kind::Water;
    }

    // set random positions for the seeds_
    size_t min(0);
    size_t max(numberColumns_ -1);
        for (size_t i(0); i < seeds_.size(); ++i) {
        seeds_[i].position.x = uniform(min, max);
        seeds_[i].position.y = uniform(min, max);
    }

    // unless they landed in water, plant the seeds
    for (size_t i(0); i < seeds_.size(); ++i) {
        size_t index(seeds_[i].position.y * numberColumns_ 
                        + seeds_[i].position.x);
        if (cells_[index] != Kind::Water) {
            cells_[index] = seeds_[i].texture;
        }
    }
    
    if (regenerate) {
        std::cout << "regenerating" << std::endl;
        steps(simulationWorld()["generation"]
                                    ["steps"].toInt());
        smooths(simulationWorld()["generation"]
                                    ["smoothness"]["level"].toInt());
    }

    updateCache();
}	



void
World::drawOn(sf::RenderTarget& target) 
{
    if (simulationWorld()["show humidity"] == 1) {
        renderingCache_.draw(humidityVertexes_.data(), humidityVertexes_.size(),
                             sf::Quads);

    }
    
    sf::Sprite cache(renderingCache_.getTexture());
    target.draw(cache);
}
	
	

void
World::loadFromFile()
{	
    // get app values for current config
    reloadConfig();

    // open config .map file
    std::string fileName(getApp().getResPath() 
                            + simulationWorld()["file"].toString());
    std::ifstream input(fileName.c_str());

    // check existance of file
    if (input.fail()) {
        throw std::runtime_error ("Input map file not found.");
    } else {
        std::string numberColumns;
        std::string cellSize;

        // read the file header
        input >> numberColumns;
        numberColumns_ = stoi(numberColumns);
        input >> std::ws;

        input >> cellSize;
        cellSize_= stof(cellSize);
        input >> std::ws;

        // clear cells_ before writing
        cells_.clear();

        // read .map file values for cell Kind
        size_t size(numberColumns_ * numberColumns_);
        for (size_t i(0); i < size; ++i) {
            short var;
            input >> var;
            Kind value = static_cast<Kind>(var);
            cells_.push_back(value);
        }

        // read .map file values for cell humidity
        for (size_t i(0); i < size; ++i) {
            double humidity;
            input >> humidity;
            humidityLevels_.push_back(humidity);
        }
    }


    input.close();	

    reloadCacheStructure();
    updateCache();
}



void
World::saveToFile() const
{
    // open config .map file
    std::string fileName(getApp().getResPath() 
                            + simulationWorld()["file"].toString());
    std::ofstream output(fileName.c_str());

    // check if output can be made
    if (output.fail()) {
        throw std::runtime_error ("Output map file cannot be made.");
    } else {

        // write the file header
        output << numberColumns_ << std::endl;
        output << cellSize_ << std::endl;

        // write the map contents
        size_t size(numberColumns_ * numberColumns_);
        for (size_t i(0); i < size; ++i) {
            switch (cells_[i]) {
                case Kind::Grass :
                    output << "0";
                    break;
                case Kind::Water :
                    output << "1";
                    break;
                case Kind::Rock :
                    output << "2";
                    break;
                default :
                    output << "2";
                    break;
            }
            output << " ";
        }
    }

    output.close();
}



void 
World::moveSeed(Seed& seed, int min, int max) 
{
    // move the seed by a random amount in range [min:max]
    seed.position.x = seed.position.x + uniform(min, max);
    seed.position.y = seed.position.y + uniform(min, max);

    // make sure the new position is in range
    int worldMax(numberColumns_ - 1);
    if (seed.position.x > worldMax) {
        seed.position.x = numberColumns_- 1;
    }                                                                          
    if (seed.position.y > worldMax) {
        seed.position.y = numberColumns_ -1;
    }

    if (seed.position.x < 0) {
        seed.position.x = 0;
    }
    if (seed.position.y < 0) {
        seed.position.y = 0;
    }

    // set the texture of the seed's cell
    cells_[(seed.position.x * numberColumns_) 
            + seed.position.y] = seed.texture;
    
    // if water seed, humidify for new position
    if (seed.texture == Kind::Water) {
        humidify(seed.position.y * numberColumns_ + seed.position.x);
    }
}



void
World::step()
{
    // loop through seeds_ to move them
    for (size_t i(0); i < seeds_.size(); ++i) {

        int min(0);
        int max(0);

        // set the move distance
        if ((seeds_[i].texture == Kind::Water) 
                && (bernoulli(simulationWorld()["seeds"]
                        ["water teleport probability"].toDouble() == 0))) {
            min = 0;
            max = numberColumns_ - 1;
        } else {
            min = -1;
            max = 1;
        }

        moveSeed(seeds_[i], min, max);
    }
}



void
World::steps(unsigned int n, bool update) 
{
    for (unsigned int i(0); i < n; ++i) {
        step();
    }

    if (update) {
        updateCache();
    }
}



void 
World::smooth()
{
    // copy cells_ so as to not have directional bias
    // decisions are made on original, written in copy
    std::vector<Kind> localCells = cells_;

    for (size_t i(0); i < cells_.size(); ++i) {

        // initialise counters for neighbours
        double nbGrass(0);
        double nbWater(0);
        double nbRock(0);

        // get the indexes for the cell
        size_t x(i % numberColumns_);
        size_t y(i / numberColumns_);

        // set the radius of neighbourhood
        unsigned int radius(1);

        // set the start and end of neighbourhood
        // possibility of adjusting to toric here
        sf::Rect<size_t> scanRange = calculateScanRange(x,y,radius);

        // list through the neighbors
        for (size_t dx(scanRange.left); 
                    dx <= scanRange.left + scanRange.width; 
                    ++dx) {
            for (size_t dy(scanRange.top); 
                        dy <= scanRange.top + scanRange.height; 
                        ++dy) {

                // check that we are not on original cell 
                if (!((dx == x) && (dy == y))) {

                    // increment for each type of cell
                    switch (cells_[dy*numberColumns_ + dx]) {
                        case Kind::Rock :
                            ++nbRock;
                            break;
                        case Kind::Water :
                            ++nbWater;
                            break;
                        case Kind::Grass :
                            ++nbGrass;
                            break;
                        default :
                            break;
                    }
                }
            }
        }

        // define the water and grass ratios
        double waterRatio(nbWater / (nbRock + nbGrass + nbWater));
        double grassRatio(nbGrass / (nbRock + nbGrass + nbWater));
        switch (cells_[i]) {
            case Kind::Rock :
                if (grassRatio > simulationWorld()["generation"]
                        ["smoothness"]["grass neighbourhood ratio"].toInt()) {
                    localCells[i] = Kind::Grass;
                }
            case Kind::Grass :
                if (waterRatio > simulationWorld()["generation"]
                        ["smoothness"]["water neighbourhood ratio"].toInt()) {
                    localCells[i] = Kind::Water;
                    humidify(i);
                }
                break;
            case Kind::Water :
                break;
            default :
                break;
        }
    }
    // swap the changes and original cells_
    std::swap(cells_, localCells);
}



void
World::smooths(unsigned int n, bool update) 
{
    for (unsigned int i(0); i < n; ++i) {
        smooth();
    }

    if (update) {
        updateCache();
    }
}



void
World::humidify()
{
    size_t size(numberColumns_ * numberColumns_);
    for (size_t i(0); i < size; ++i) {
        if (cells_[i] == Kind::Water) {
            humidify(i);
        }
    }
}



void
World::humidify(size_t i)
{
    size_t x(i % numberColumns_);
    size_t y(i / numberColumns_);

    // get the area to calculate humidity for
    sf::Rect<size_t> scanRange = calculateScanRange(x,y,humidityRange_);

    size_t right(scanRange.left + scanRange.width);
    size_t bottom(scanRange.top + scanRange.height);

    for (size_t dx(scanRange.left); dx <= right; ++dx) {
        for (size_t dy(scanRange.top); dy <= bottom; ++dx) {
            humidityLevels_[dy*numberColumns_ + dx] 
                        += humidityInitialLevel_ 
                        * exp( - std::hypot(x - dx, y - dy) / humidityDecayRate_);
        }           
    }
}



sf::Rect<size_t>
World::calculateScanRange(size_t x, size_t y, unsigned int radius)
{
    sf::Rect<size_t> scanRange; 
    
    // find start x
    if (x >= radius) {
        scanRange.left = x - radius;
    } else {
        scanRange.left = 0;
    }

    // find start y
    if (y >= radius) {
        scanRange.top = y - radius;
    } else {
        scanRange.top = 0;
    }

    // find width
    if ((x + radius) <= numberColumns_) {
        scanRange.width = radius;
    } else {
        scanRange.width = numberColumns_ - x;
    }

    // find height
    if ((y + radius) <= numberColumns_) {
        scanRange.height = radius;
    } else {
        scanRange.height = numberColumns_ - y;
    }

    return scanRange;
}

bool
World::isGrowable(const Vec2d& position)
{
    if (cells_[positionInTab(position)] == Kind::Grass) {
	return true;
    } else {
	return false;
    }
}

double
World::positionInTab(const Vec2d& position)
{
    return ((position.x / cells_.size()) * numberColumns_  + position.y / cells_.size());
}
