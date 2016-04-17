#include <Env/Flower.hpp>

Flower::Flower (const Vec2d& position, double radius, double pollen) :
    Collider (position, radius), pollen_ (pollen)
{
}

double
Flower::takePollen ()
{
  double pollen (0);
  return pollen;
}

void
Flower::drawOn (sf::RenderTarget& target) const
{
  std::vector<std::string> textures;
  j::Value textureNames (getAppConfig ()["simulation"]["flower"]["textures"]);
  textures.push_back (textureNames[0].toString ());
  textures.push_back (textureNames[1].toString ());
  textures.push_back (textureNames[2].toString ());
  size_t min (0);
  size_t max (textures.size () - 1);
  const sf::Texture& texture = getAppTexture (textures[uniform (min, max)]);
  auto flowerSprite = buildSprite (this->position_, this->radius_, texture);
  target.draw (flowerSprite);

}

