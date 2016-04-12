#include <Env/Env.hpp>



Env::Env()
: world_()
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
}



void
Env::reset()
{
    world_.reset(true);
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
