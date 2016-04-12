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
	flowers_.clear()
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
if (World::isGrowable(position) && flowers_.size() < ["simulation"]["env"]["max flowers"])
{

	   
flowers_.pushback(new Flower(["simulation"]["env"]["initial"]["flower"]["size"]["manual"]/2, position, 
				random(["simulation"]["env"]["initial"]["flower"]["nectar"]["min"],
				["simulation"]["env"]["initial"]["flower"]["nectar"]["max"])));

}
void 
World::drawFlowerZone(sf::RenderTarget& target, Vec2d const& position)
{
	if (World::isGrowable(position)) {
		auto shape = buildAnnulus(position, ["simulation"]["env"]["initial"]["flower"]["size"]["manual"], sf::Color::Green, 5.0);
        target.draw(shape);
    }
    else {
	auto shape = buildAnnulus(position, ["simulation"]["env"]["initial"]["flower"]["size"]["manual"], sf::Color::Red, 5.0);
        target.draw(shape);
	}
}

