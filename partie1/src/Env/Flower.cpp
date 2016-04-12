#include <Flower.hpp>

Flower::Flower (const vec2d& position, double radius, double pollen) 
: Collider(position, radius), pollen_(pollen)
{}


pollen_
Flower::takePollen() 
{
	
}
		
void
drawOn(sf::RenderTarget& target) const
{
    auto const& texture = getAppTexture(textures[random(0,textures.size()-1].toString());
    auto flowerSprite = buildSprite(position_, radius_, texture);
    target.draw(flowerSprite);
	
}
	
