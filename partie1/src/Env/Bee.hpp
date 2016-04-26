#ifndef BEE_H
#define BEE_H

#include <Utility/Vec2d.hpp>
#include <cmath>
#include <Utility/Utility.hpp>
#include <Application.hpp>
#include <Utility/Logging.hpp>
#include <Env/World.hpp>
#include <Env/Collider.hpp>
#include <Env/Hive.hpp>
#include <Interface/Drawable.hpp>
#include <Interface/Updatable.hpp>

class Bee: public Collider, public Drawable, public Updatable
{
public:

Bee(Hive& hive, const Vec2d& position, double radius, double speedAmpl, double energy);

Vec2d 
move(sf::Time dt);

bool
isDead();

void
update (sf::Time dt) override;

void
drawOn (sf::RenderTarget& target) const override;

virtual j::Value 
const& getConfig();


virtual void
loadTexture ();

protected:

Hive hive_;
Vec2d speed_;
double speedAmpl_;
double energy_;
sf::Texture beeTexture_;

};

#endif
