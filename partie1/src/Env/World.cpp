#include "World.hpp"
#include <Utility/Vertex.hpp>
#include <Application.hpp>
#include <SFML/Graphics.hpp> 
#include <vector>
#include <JSON/JSON.hpp>
#include <fstream>
#include <iostream>
#include <stdexcept>
#include <Random/Random.hpp>
#include <string>

using namespace std;



j::Value
simulationWorld()
{
    return getAppConfig()["simulation"]["world"];
}



void
World::reloadConfig() 
{
    // numberbCulumns : number of cells in a row
    numberColumns_= simulationWorld()["cells"].toInt();

    // cellSize : size of a cell in 'pixels'
    cellSize_= simulationWorld()["size"].toDouble() / numberColumns_;

    // make a vector representing a square grid of Rock
    cells_ = vector<Kind> (numberColumns_ * numberColumns_, Kind::Water);

    nbWaterSeeds_ = simulationWorld()["seeds"]["water"].toInt();
    nbGrassSeeds_ = simulationWorld()["seeds"]["grass"].toInt();
    vector<Seed> tmp(nbGrassSeeds_*nbWaterSeeds_);
    seeds_ = tmp; 
}



void
World::reloadCacheStructure() 
{
    grassVertexes_ = generateVertexes(simulationWorld()["textures"], numberColumns_, cellSize_);
    waterVertexes_ = grassVertexes_;
    rockVertexes_ = grassVertexes_;

    renderingCache_.create(numberColumns_* cellSize_, numberColumns_ * cellSize_);
}



void
World::updateCache() 
{
    sf::RenderStates rsGrass;
    rsGrass.texture = &getAppTexture(simulationWorld()["textures"]["grass"].toString()); 
    sf::RenderStates rsWater;
    rsWater.texture = &getAppTexture(simulationWorld()["textures"]["water"].toString());
    sf::RenderStates rsRock;
    rsRock.texture = &getAppTexture(simulationWorld()["textures"]["rock"].toString()); 

    size_t size(cells_.size());
    for (size_t i(0); i < size; ++i) {

        // use mod to get column number
        size_t x(i % numberColumns_);
        // use integer division to get line number
        size_t y(i / numberColumns_);

        vector<size_t> positionIndexes = indexesForCellVertexes(x, y, numberColumns_);

        vector<int> aValues;

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

        for (size_t j(0); j < 4; ++j) {
            grassVertexes_[positionIndexes[j]].color.a = aValues[0];
            waterVertexes_[positionIndexes[j]].color.a = aValues[1];
            rockVertexes_[positionIndexes[j]].color.a = aValues[2];
        }
    }

    renderingCache_.clear();

    renderingCache_.draw(grassVertexes_.data(), grassVertexes_.size(), sf::Quads, rsGrass);
    renderingCache_.draw(waterVertexes_.data(), waterVertexes_.size(), sf::Quads, rsWater);
    renderingCache_.draw(rockVertexes_.data(), rockVertexes_.size(), sf::Quads, rsRock);

    renderingCache_.display();
}



void
World::reset(bool regenerate)
{
    reloadConfig();
    reloadCacheStructure();
   
    int max(numberColumns_ -1);
    for (size_t i(0); i<seeds_.size(); ++i) {
        seeds_[i].coords_.x = uniform(0, max);
        seeds_[i].coords_.y = uniform(0, max);
    }

    for (size_t j(0); j<numberColumns_; ++j) {
        for (size_t k(0); k<numberColumns_; ++k) {
            if (cells_[ (j * numberColumns_) + k] != Kind::Water) {
                cells_[ (j * numberColumns_) + k] = seeds_[ (j * numberColumns_) + k].Seedtexture_;
            }
        }
    }

    updateCache();
}	



void
World::drawOn(sf::RenderTarget& target) 
{
    sf::Sprite cache(renderingCache_.getTexture());
    target.draw(cache);
}
	
	

void
World::loadFromFile()
{	
    reloadConfig();

    string fileName(getApp().getResPath() + simulationWorld()["file"].toString());
    ifstream entree(fileName.c_str());

    if (entree.fail()) {
        throw std::runtime_error ("Pas de fichier correspondant");
    }

    else {
        string numberColumns;
        string cellSize;

        entree >> numberColumns;
        //le nombre de colonnes vaut cette valeur convertie en entier
        numberColumns_ = stoi(numberColumns);
        entree >> std::ws;

        entree >> cellSize;
        //la taille de la cellule vaut cette valeur convertie en Float
        cellSize_= stof(cellSize);
        entree >> std::ws;

        cells_.clear();

        size_t size(numberColumns_ * numberColumns_);
        for (size_t i(0); i < size; ++i) {
            short var;
            entree >> var;
            //il faut convertir var en  Kind
            Kind value = static_cast<Kind>(var);
            cells_.push_back(value);
        }
    }


    entree.close();	

    reloadCacheStructure();

    updateCache();
} 
