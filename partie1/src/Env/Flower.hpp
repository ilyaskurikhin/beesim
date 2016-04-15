#ifndef FLOWER_H
#define FLOWER_H
#include <SFML/Graphics.hpp>
#include <Random/Random.hpp>
#include <Env/Collider.hpp>
#include <Utility/Utility.hpp>
#include <Application.hpp>

class Flower : public Collider {

public: 
    Flower();

    Flower(const Vec2d& position, double radius, double pollen);
    
    double
    takePollen();
    
    void
    drawOn(sf::RenderTarget& target) const;
    
private:
    
    double pollen_;
};



#endif
