#ifndef FLOWER_H
#define FLOWER_H
#include <SFML/Graphics.hpp>
#include <Random/Random.hpp>

class Flower : public Collider {

public: 
    Flower ();
    
	pollen_ 
	takePollen();
	
	void
	drawOn(sf::RenderTarget& target) const;
	
private:
	
	double pollen_;
};



#endif
