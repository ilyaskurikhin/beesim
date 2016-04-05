#include "World.hpp"
#include "../Utility/Vertex.hpp"
#include <Application.hpp>
#include <SFML/Graphics.hpp> 
#include <vector>
#include <string>
#include <fstream>
#include <iostream>
#include <stdexcept>

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
	cells_ = vector<Kind> (numberColumns_ * numberColumns_, Kind::Rock);
	
	
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
	sf::RenderStates rs1;
	rs1.texture = &getAppTexture(simulationWorld()["textures"]["rock"].toString()); 
	sf::RenderStates rs3;
	rs3.texture = &getAppTexture(simulationWorld()["textures"]["grass"].toString()); 
	sf::RenderStates rs2;
	rs2.texture = &getAppTexture(simulationWorld()["textures"]["water"].toString());
	
	for (size_t i(0); i < numberColumns_; ++i) {
		for (size_t j(0); j < numberColumns_; ++j) {
			
			vector <size_t> indexes = indexesForCellVertexes(i, j, cellSize_);
			
			if (cells_[(i * numberColumns_) + j] == Kind::Rock) {
				for (size_t k(0); k<4; ++k) {
				waterVertexes_[indexes[k]].color.a=0;
				grassVertexes_[indexes[k]].color.a=0;
				rockVertexes_[indexes[k]].color.a=255;
				}
			}
			else if (cells_[(i * numberColumns_) + j] == Kind::Grass) {
				for (size_t k(0); k<4; ++k) {
				waterVertexes_[indexes[k]].color.a=0;
				grassVertexes_[indexes[k]].color.a=255;
				rockVertexes_[indexes[k]].color.a=0;
				}
			} 
			else if (cells_[(i * numberColumns_) + j] == Kind::Water) {
				for (size_t k(0); k<4; ++k) {
				waterVertexes_[indexes[k]].color.a=255;
				grassVertexes_[indexes[k]].color.a=0;
				rockVertexes_[indexes[k]].color.a=0;
				}
			}
			
		}
	}
	
	renderingCache_.clear();
	// ici pour la texture liée à la roche
	renderingCache_.draw(rockVertexes_.data(), rockVertexes_.size(), sf::Quads, rs1);
	
	// ici pour la texture liée à l'herbe
	renderingCache_.draw(grassVertexes_.data(), grassVertexes_.size(), sf::Quads, rs3);
	
	 // ici pour la texture liée à l'eau
	renderingCache_.draw(waterVertexes_.data(), waterVertexes_.size(), sf::Quads, rs2);
	
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
	
		for (size_t i(0); i <= cells_.size() ; ++i) {
			
			entree >> std::ws;
			short var;
			entree >> var;
			//il faut convertir var en  Kind
			Kind value = static_cast<Kind>(var);
			cells_[i]= value;
			
		}
	}
	
	
	entree.close();	
	
	reloadCacheStructure();
	
	updateCache();
	
} 





