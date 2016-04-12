#include <Env/Flower.hpp>

Flower::Flower (const Vec2d& position, double radius, double pollen) 
: Collider(position, radius), pollen_(pollen)
{}


double
Flower::takePollen() 
{
    double pollen(0);
    return pollen;
}
		
void
Flower::drawOn(sf::RenderTarget& target) const
{
    auto textures = 
    auto const& texture = getAppTexture([uniform(0,textures.size()-1)].toString());
    auto flowerSprite = buildSprite(this->position_, this->radius_, texture);
    target.draw(flowerSprite);
	
}
	
