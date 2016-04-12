#include <Env/Env.hpp>



Env::Env()
: world_(),
  numberFlowers_(0)
{
   world_.loadFromFile();
   world_.updateCache();
}



void
Env::update(sf::Time dt)
{
    world_.updateCache();
}

void
Env::drawOn(sf::RenderTarget& target)
{
	world_.drawOn(target);
	for (size_t i(0); i < flowers_.size(); ++i) {
		(*flowers_[i]).drawOn(target);
   
	}
}



void
Env::reset()
{
    world_.reset(true);
    for (size_t i(0); i < flowers_.size(); ++i) {
		delete flowers_[i];
	}
	flowers_.clear();
}

void
Env::loadWorldFromFile()
{
    world_.loadFromFile();
}


void
Env::saveWorldToFile()
{
    world_.saveToFile();
}

bool
Env::addFlowerAt(const Vec2d& position)
{
    size_t maxFlowers = getAppConfig()["simulation"]["env"]["max flowers"].toInt();
    if (world_.isGrowable(position) && (flowers_.size() < maxFlowers)) {
        double pollen = uniform(getAppConfig()["simulation"]["env"]["initial"]["flower"]["nectar"]["min"].toDouble(), getAppConfig()["simulation"]["env"]["initial"]["flower"]["nectar"]["max"].toDouble());

        flowers_.push_back(new Flower(position, getAppConfig()["simulation"]["env"]["initial"]["flower"]["size"]["manual"].toDouble()/2.0, pollen));
        return true;
    } else {
        return false;
    }
}



void 
Env::drawFlowerZone(sf::RenderTarget& target, const Vec2d& position)
{
	if (world_.isGrowable(position)) {
		auto shape = buildAnnulus(position, getAppConfig()["simulation"]["env"]["initial"]["flower"]["size"]["manual"].toDouble(), sf::Color::Green, 5.0);
        target.draw(shape);
    }
    else {
	auto shape = buildAnnulus(position, getAppConfig()["simulation"]["env"]["initial"]["flower"]["size"]["manual"].toDouble(), sf::Color::Red, 5.0);
        target.draw(shape);
	}
}

