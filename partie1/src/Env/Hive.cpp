#include <Env/Hive.hpp>


  Hive::Hive(const Vec2d& position, double radius) :
  Collider (position, radius), nectar_ (getAppConfig()["simulation"]["hive"]["initial"]["nectar"].toDouble()),  
  hiveTexture_ (getAppTexture(getAppConfig()["simulation"]["hive"]["texture"].toString()))
  {}

  
  Hive::~Hive ()
  {
  for (size_t i = 0; i < bees_.size (); ++i)
    {
      delete bees_[i];
    }
  bees_.clear ();
  }
  
  void
  Hive::addBee ()
  {
  }
  
  
  void
  Hive::update(sf::Time dt)
  {
  }
  
  void
  Hive::drawOn(sf::RenderTarget& target) const
  {
    auto hiveSprite = buildSprite (this->position_, this->radius_, hiveTexture_);
    target.draw (hiveSprite);
  
    /*if (isDebugOn ())
    {
        Vec2d position = getApp ().getCursorPositionInView ();
          {
            std::stringstream valueStream;
            valueStream << std::fixed << std::setprecision(0) << nectar_;
            sf::Text text = buildText (valueStream.str(), position, getAppFont (), 30,
  				  sf::Color::Green);
            target.draw (text);
          }  
    }
    */
  }

  double
  Hive::dropPollen(double amount)
  {
  return nectar_;
  }
  
  double
  Hive::takeNectar(double amount)
  {
  return nectar_;
  }
    
  double
  Hive::getNectar() {
  return nectar_;
  }


