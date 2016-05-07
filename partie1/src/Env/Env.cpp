#include <Env/Env.hpp>

#include <Env/Flower.hpp>
#include <Env/FlowerGenerator.hpp>

bool ENV_VERBOSE (true);

Env::Env() :
    world_ (new World ()), flowerGenerator_ (new FlowerGenerator)
{
  logEvent ("Env", "generating environment");
  reloadConfig ();
}

Env::~Env()
{
  logEvent ("Env", "destroying environment");
  delete world_;

  for (size_t i = 0; i < flowers_.size (); ++i)
    {
      delete flowers_[i];
    }
  flowers_.clear ();

  for (size_t i = 0; i < hives_.size (); ++i)
    {
      delete hives_[i];
    }
  hives_.clear ();

}

void
Env::update(sf::Time dt)
{
  // update du generateur
  if ((getAppConfig ()["simulation"]["flower generator"]["active"].toBool ())
      && (flowers_.size () < maxFlowers_))
    {
      flowerGenerator_->update (dt);
    }

  // iterate through flowers
  for (size_t i = 0; i < flowers_.size (); ++i)
    {
      // flowers are updates
      // new flowers are drawn in next draw cycle
      flowers_[i]->update (dt);

      // check if flower is dead
      if (flowers_[i]->getPollen () <= 0)
	{
	  // remove dead flower
	  delete flowers_[i];
	  flowers_[i] = nullptr;
	}
    }
  // remove empty locations
  flowers_.erase (std::remove (flowers_.begin (), flowers_.end (), nullptr),
		  flowers_.end ());

  for (size_t i = 0; i < hives_.size (); ++i)
    {
      hives_[i]->update (dt);
    }

}

void
Env::drawOn(sf::RenderTarget& target) const
{
  world_->drawOn (target);
  for (size_t i = 0; i < flowers_.size (); ++i)
    {
      flowers_[i]->drawOn (target);
    }

  for (size_t i = 0; i < hives_.size (); ++i)
    {
      hives_[i]->drawOn (target);

    }

  // if debug is on, show values
  if (isDebugOn ())
    {
      // get cursor position
      Vec2d position = getApp ().getCursorPositionInView ();
      if (world_->isInWorld (position))
	{
	  bool isFlower (false);
	  std::string valueString ("empty");
	  sf::Color color (sf::Color::White);

	  // check for flowers
	  for (size_t i = 0; i < flowers_.size (); ++i)
	    {
	      if (*(flowers_[i]) > position)
		{
		  isFlower = true;
		  valueString = to_nice_string (flowers_[i]->getPollen ());
		  color = sf::Color::Yellow;
		}
	    }

	  // otherwise show ambient humidity
	  if (!isFlower)
	    {
	      valueString = to_nice_string (world_->getHumidity (position));
	      color = sf::Color::Red;
	    }

	  sf::Text text = buildText (valueString, position, getAppFont (),
				     debug_text_size_, color);
	  target.draw (text);

	}
    }

}

void
Env::reset()
{
  logEvent ("Env", "resetting environment");

  world_->reset (true);
  for (size_t i = 0; i < flowers_.size (); ++i)
    {
      delete flowers_[i];
    }
  flowers_.clear ();
  flowerGenerator_->reset ();

  for (size_t i = 0; i < hives_.size (); ++i)
    {
      delete hives_[i];
    }
  hives_.clear ();
}

void
Env::reloadConfig()
{
  // get variables from configuration
  flowerManualRadius_ =
      getAppConfig ()["simulation"]["env"]["initial"]["flower"]["size"]["manual"].toDouble ();
  flowerMaxNectar_ =
      getAppConfig ()["simulation"]["env"]["initial"]["flower"]["nectar"]["max"].toDouble ();
  flowerMinNectar_ =
      getAppConfig ()["simulation"]["env"]["initial"]["flower"]["nectar"]["min"].toDouble ();

  // get max number of Flower from configuration
  maxFlowers_ = getAppConfig ()["simulation"]["env"]["max flowers"].toInt ();

  hiveManualRadius_ =
      getAppConfig ()["simulation"]["env"]["initial"]["hive"]["size"]["manual"].toDouble ();

  debug_text_size_ = 10
      * (getAppConfig ()["simulation"]["world"]["size"].toDouble ()
	  / getAppConfig ()["simulation"]["world"]["cells"].toDouble ());

  hiveableFactor_ =
      getAppConfig ()["simulation"]["env"]["initial"]["hive"]["hiveable factor"].toDouble ();
}

void
Env::loadWorldFromFile()
{
  world_->loadFromFile ();
}

void
Env::saveWorldToFile() const
{
  world_->saveToFile ();
}

double
Env::getHumidity(const Vec2d& position) const
{
  return world_->getHumidity (position);
}

bool
Env::isGrowable(const Vec2d& position) const
{
  return world_->isGrowable (position);
}

bool
Env::isFlyable(const Vec2d& position) const
{
  return world_->isFlyable (position);
}

bool
Env::isPlaceable(const Vec2d& position, double radius) const
{
  if (world_->isGrowable (position))
    {
      Collider object (position, radius);

      // check if object can be made at position
      if (getCollidingHive (object) == nullptr)
	{
	  return true;
	}
    }
  return false;
}

bool
Env::addFlowerAt(const Vec2d& position)
{
  // check if flower can be made at position
  if ((flowers_.size () < maxFlowers_)
      && (isPlaceable (position, flowerManualRadius_)))
    {
      // set a random number of pollen
      double pollen = uniform (flowerMinNectar_, flowerMaxNectar_);

      flowers_.push_back (new Flower (position, flowerManualRadius_, pollen));
      return true;
    }
  else
    {
      return false;
    }
}

void
Env::drawFlowerZone(sf::RenderTarget& target, const Vec2d& position)
{
  if (world_->isGrowable (position))
    {
      sf::CircleShape shape = buildAnnulus (position, flowerManualRadius_,
					    sf::Color::Green, 5.0);
      target.draw (shape);
    }
  else
    {
      sf::CircleShape shape = buildAnnulus (position, flowerManualRadius_,
					    sf::Color::Red, 5.0);
      target.draw (shape);
    }
}

bool
Env::addHiveAt(const Vec2d& position)
{

  if (world_->isGrowable (position)
      && (isPlaceable (position, hiveManualRadius_)))
    {
      hives_.push_back (new Hive (position, hiveManualRadius_));
      return true;
    }
  else
    {
      return false;
    }
}

void
Env::drawHiveableZone(sf::RenderTarget& target, const Vec2d& position)
{
  // TODO implement
}

Hive*
Env::getCollidingHive(const Collider& body) const
{
  for (size_t i (0); i < hives_.size (); ++i)
    {
      Collider collidingHive (hives_[i]->getPosition (),
			      (hiveManualRadius_ * hiveableFactor_));

      if (collidingHive.isColliding (body))
	{
	  return hives_[i];
	}
    }
  return nullptr;
}

Flower*
Env::getCollidingFlower(const Collider& body) const
{
  for (size_t i (0); i < flowers_.size (); ++i)
    {
      Collider collidingFlower (flowers_[i]->getPosition (),
				flowers_[i]->getRadius ());

      if (collidingFlower.isColliding (body))
	{
	  return flowers_[i];
	}
    }
  return nullptr;

}

Bee*
Env::getBeeAt(const Vec2d& position) const
{
  for (size_t i = 0; i < hives_.size (); ++i)
    {
      Bee* bee = hives_[i]->getBeeAt (position);
      if (bee)
	{
	  return bee;
	}
    }
  return nullptr;
}

double
Env::getTextSize()
{
  return debug_text_size_;
}
