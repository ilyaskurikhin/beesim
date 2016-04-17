#include <Env/Flower.hpp>

Flower::Flower (const Vec2d& position, double radius, double pollen) :
    Collider (position, radius), pollen_ (pollen)
{
  loadTexture ();
}

double
Flower::takePollen (const double& pollen)
{
  double taken (0);
  if (pollen > pollen_)
    {
      taken = pollen;
      pollen_ = pollen_ - pollen;
    }
  else
    {
      taken = pollen_;
      pollen_ = 0;
    }
  return taken;
}

void
Flower::drawOn (sf::RenderTarget& target) const
{
  auto flowerSprite = buildSprite (this->position_, this->radius_, texture_);
  target.draw (flowerSprite);
}

void
Flower::loadTexture ()
{
  std::vector<std::string> textures;
  j::Value textureNames (getAppConfig ()["simulation"]["flower"]["textures"]);
  textures.push_back (textureNames[0].toString ());
  textures.push_back (textureNames[1].toString ());
  textures.push_back (textureNames[2].toString ());
  size_t min (0);
  size_t max (textures.size () - 1);
  texture_ = getAppTexture (textures[uniform (min, max)]);
}

