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



std::ostream&
operator<< (std::ostream& oss, const std::runtime_error& error)
{
    oss << error.what();
    return oss;
}



j::Value
World::simulationWorld()
{
	return getAppConfig()["simulation"]["world"];
}



void
World::reloadConfig() 
{
	/*
	// numberbCulumns : number of cells in a row
	numberColumns_= simulationWorld()["cells"].toInt();
	
	// cellSize : size of a cell in 'pixels'
	cellSize_= simulationWorld()["size"].toDouble() / numberColumns_;
	
	// make a vector representing a square grid of Rock
	cells_ = vector<Kind> (numberColumns_ * numberColumns_, Kind::Rock);
	
	*/
	
	loadFromFile();
	
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

	for (size_t i(0); i < numberColumns_; ++i) {
		for (size_t j(0); j < numberColumns_; ++j) {
			
			vector <long unsigned int> Indexes = indexesForCellVertexes(i, j, numberColumns_);
			
			if (cells_[j + i * numberColumns_] == Kind::Rock) {
				for (size_t i(0); i<=Indexes.size(); ++i) {
						cout <<"Hello2";

				waterVertexes_[Indexes[i]].color.a=0;
				grassVertexes_[Indexes[i]].color.a=0;
				rockVertexes_[Indexes[i]].color.a=255;

				}
			}
			else if (cells_[j + i * numberColumns_] == Kind::Grass) {
				for (size_t i(0); i<=Indexes.size(); ++i) {
				waterVertexes_[Indexes[i]].color.a=0;
				grassVertexes_[Indexes[i]].color.a=255;
				rockVertexes_[Indexes[i]].color.a=0;
				}
			} 
			else if (cells_[j + i * numberColumns_] == Kind::Water) {
				for (size_t i(0); i<=Indexes.size(); ++i) {
				waterVertexes_[Indexes[i]].color.a=255;
				grassVertexes_[Indexes[i]].color.a=0;
				rockVertexes_[Indexes[i]].color.a=0;
				}
			}
			
		}
	}
	
	renderingCache_.clear();
	sf::RenderStates rs1;
	rs1.texture = &getAppTexture(simulationWorld()["textures"]["rock"].toString()); // ici pour la texture liée à la roche
	renderingCache_.draw(rockVertexes_.data(), rockVertexes_.size(), sf::Quads, rs1);
	renderingCache_.display();
	
	sf::RenderStates rs2;
	rs2.texture = &getAppTexture(simulationWorld()["textures"]["water"].toString()); // ici pour la texture liée à l'eau
	renderingCache_.draw(waterVertexes_.data(), waterVertexes_.size(), sf::Quads, rs2);
	renderingCache_.display();
	
	sf::RenderStates rs3;
	rs3.texture = &getAppTexture(simulationWorld()["textures"]["grass"].toString()); // ici pour la texture liée à l'herbe
	renderingCache_.draw(grassVertexes_.data(), grassVertexes_.size(), sf::Quads, rs3);
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

	string fileName(getApp().getResPath() + simulationWorld()["file"].toString());
	ifstream entree(fileName.c_str());
	ofstream sortie;
		
	try
	{
	if (entree.fail()) throw std::runtime_error ("Pas de fichier correspondant");

	string numberColumns;
	string cellSize;
	short var;
	
	entree >> numberColumns;
	//le nombre de colonnes vaut cette valeur convertie en entier
	numberColumns_ = stoi(numberColumns);
	entree >> std::ws;

	entree >> cellSize;
	//la taille de la cellule vaut cette valeur convertie en Float
	cellSize_= stof(cellSize);
	entree >> std::ws;
	
	for (size_t i(0); i <= numberColumns_; ++i) {
	// var est de type short
	entree >> var;
	
	//il faut convertir var en  Kind
	Kind values = static_cast<Kind>(var);
	cells_.push_back(values);
	entree >> std::ws;
	}
	}
	
	catch (std::runtime_error error) 
	{

	std::cerr << error;
	}
	
	entree.close();	
} 


