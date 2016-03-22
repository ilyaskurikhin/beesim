#include <Application.hpp>
#include <World.hpp>
#include <SFML/Graphics.hpp> 
#include <vector>


using namespace std;

void
World::reloadConfig() 
{
	
	j::Value simulationWorld;
	simulationWorld= getAppConfig()["simulation"]["world"];
	
	nbCells_= simulationWorld["cells"].toDouble();
	cellSize_= simulationWorld["size"].toDouble()) / nbCells_;
	cells_ = vector<Kind>((simulationWorld["size"].toDouble() + nbCells_ * nbCells_),Rock);
}



void
World::reloadCacheStructure() 
{

}



void
World::drawOn(sf::RenderTarget& target) 
{
	sf::Sprite cache(renderingCache_.getTexture());
	target.draw(cache);
}
	 
