#include <Env/Flower.hpp>

Flower::Flower(const Vec2d& position, double radius, double pollen) :
    Collider(position, radius), pollen_(pollen), humidity_(
        getAppEnv().getHumidity(position))
{
  loadTexture();
  splitThreshold_ =
      getAppConfig()["simulation"]["flower"]["growth"]["split"].toDouble();
  humidityThreshold_ =
      getAppConfig()["simulation"]["flower"]["growth"]["threshold"].toDouble();
  humidityFactor_ = std::log(humidity_ / humidityThreshold_);
}

double
Flower::takePollen(const double& pollen)
{
  double taken(0);
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
Flower::drawOn(sf::RenderTarget& target) const
{
  auto flowerSprite = buildSprite(this->getPosition(), this->getRadius(),
                                  texture_);
  target.draw(flowerSprite);
}

void
Flower::loadTexture()
{
  std::vector<std::string> textures;

  // get the texture names from configuration
  j::Value textureNames(getAppConfig()["simulation"]["flower"]["textures"]);
  textures.push_back(textureNames[0].toString());
  textures.push_back(textureNames[1].toString());
  textures.push_back(textureNames[2].toString());

  // set a random texture from res based on its name
  size_t min(0);
  size_t max(textures.size() - 1);
  texture_ = getAppTexture(textures[uniform(min, max)]);
}

void
Flower::update(sf::Time dt)
{
  // set the new pollen value
  pollen_ = pollen_ + dt.asSeconds() * humidityFactor_;

  // split flower if has enough pollen
  if (pollen_ > splitThreshold_)
    {
      int i(0);
      bool placed(false);
      while ((!placed) && (i < 100)) //
        {
          // set a random distance
          double distance(
              uniform(0.5 * this->getRadius(), 2 * this->getRadius()));

          // calculate a new position and clamp it
          Vec2d position = this->getPosition()
              + Vec2d::fromRandomAngle() * distance;
          Collider protoFlower(position, this->getRadius());
          protoFlower.clamping();

          if (getAppEnv().addFlowerAt(protoFlower.getPosition()))
            {
              placed = true;
              pollen_ = 0.5 * pollen_;
            }
          ++i;
        }
    }
}

double
Flower::getPollen()
{
  return pollen_;
}

