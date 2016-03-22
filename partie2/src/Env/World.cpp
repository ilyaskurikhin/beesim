#include <Application.hpp>
#include <World.hpp>
#include <SFML/Graphics.hpp> 
#include <vector>


using namespace std;

void
World::reloadConfig() {
	nbCells_= getAppConfig()["simulation"]["world"]["cells"].toDouble();
	cellSize_= (getAppConfig()["simulation"]["world"]["size"].toDouble()) / nbCells_;
	cells_ = vector<Kind>((nbCells_* ( getAppConfig()["simulation"]["world"]["size"].toDouble() / cellSize_)),Rock);
}


void
World::reloadCacheStructure() {
}

void
World::draw(sf::RenderTarget& target) {
}
	 
