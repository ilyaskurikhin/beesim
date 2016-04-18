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
  // if there is enough pollen, take pollen out
  if (pollen > pollen_)
    {
      taken = pollen;
      pollen_ = pollen_ - pollen;
    }
  // if there is not enough pollen, empty the flower
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

  // get the texture names from configuration
  j::Value textureNames (getAppConfig ()["simulation"]["flower"]["textures"]);
  textures.push_back (textureNames[0].toString ());
  textures.push_back (textureNames[1].toString ());
  textures.push_back (textureNames[2].toString ());

  // set a random texture from res based on its name
  size_t min (0);
  size_t max (textures.size () - 1);
  texture_ = getAppTexture (textures[uniform (min, max)]);
}

void
Flower::update (sf::Time dt, double humidity)
{
  // get variables from configuration
  double threshold(getAppConfig ()["simulation"]["flower"]["growth"]["threshold"].toDouble());

  // set the new pollen value
  pollen_ = pollen_ + dt.asSeconds() * std::log(humidity/threshold);
}

double
Flower::getPollen()
{
  return pollen_;
}

