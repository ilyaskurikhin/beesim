#ifndef FLOWER_H
#define FLOWER_H

#include <SFML/Graphics.hpp>
#include <Random/Random.hpp>
#include <Env/Collider.hpp>
#include <Env/Env.hpp>
#include <Utility/Utility.hpp>
#include <Application.hpp>
#include <Utility/Logging.hpp>

#include <Interface/Drawable.hpp>
#include <Interface/Updatable.hpp>

class Flower : public Collider, public Drawable, public Updatable
{

public:
    Flower ();

    Flower (const Vec2d& position, double radius, double pollen);

    double
    takePollen (const double& pollen);

    void
    loadTexture ();

    void
    update (sf::Time dt) override;

    void
    drawOn (sf::RenderTarget& target) const override;

    double
    getPollen ();

private:

    double pollen_;
    sf::Texture texture_;
};

#endif
