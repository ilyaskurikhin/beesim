#include "World.hpp"
#include "../Utility/Vertex.hpp"
#include <Application.hpp>
#include <SFML/Graphics.hpp> 
#include <vector>


using namespace std;


void
World::reloadConfig() 
{
	
	// numberbCulumns : number of cells in a row
	numberColumns_= getAppConfig()["simulation"]["world"]["cells"].toDouble();
	// cellSize : size of a cell in 'pixels'
	cellSize_= getAppConfig()["simulation"]["world"]["size"].toDouble() / numberColumns_;
	// make a vector representing a square grid of Rock
        size_t numberCells(numberColumns_ * numberColumns_);
	for (size_t i(0); i < numberCells; ++i) {
		cells_.push_back(Kind::Rock);
	}
}



void
World::reloadCacheStructure() 
{
	grassVertexes_ = generateVertexes(getAppConfig()["simulation"]["world"]["textures"], numberColumns_, cellSize_);
	waterVertexes_ = grassVertexes_;
	rockVertexes_ = grassVertexes_;
	
        renderingCache_.create(numberColumns_* cellSize_, numberColumns_ * cellSize_);
}



void
World::updateCache() 
{
	renderingCache_.clear();
	sf::RenderStates rs;
	rs.texture = &getAppTexture(getAppConfig()["simulation"]["world"]["textures"]["rock"].toString()); // ici pour la texture liée à la roche
	renderingCache_.draw(rockVertexes_.data(), rockVertexes_.size(), sf::Quads, rs);
	renderingCache_.display();
}



void
World::reset(bool regenerate)
{
	reloadConfig();
	reloadCacheStructure();
	updateCache();
}	



void
World::drawOn(sf::RenderTarget& target) 
{
	sf::Sprite cache(renderingCache_.getTexture());
	target.draw(cache);
}
	
	
	 
	 	/*
	// we'll need this someday
	size_t numberVertexColumns(4 * numberColumns_);
	size_t numberVertexRows(4 * numberColumns_);
	for (size_t i(0); i < numberVertexRows; ++i) {
		for (size_t j(0); j < numberVertexColumns; ++j) {
			grassVertexes_[i * numberVertexColumns + j] = 
		}
	}
	*/
