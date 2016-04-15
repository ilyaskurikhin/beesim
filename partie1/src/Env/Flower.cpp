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
    auto const& textures = ["simulation"]["flower"]["textures"];
    size_t min(0);
    size_t max(textures.size());
    const sf::Texture& texture = getAppTexture(textures[uniform(min,max)]);
    auto flowerSprite = buildSprite(this->position_, this->radius_, texture);
    target.draw(flowerSprite);
	
}
	
