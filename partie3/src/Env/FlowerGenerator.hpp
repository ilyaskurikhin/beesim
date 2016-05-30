#ifndef FLOWERGENERATOR_H
#define FLOWERGENERATOR_H

#include <SFML/Graphics.hpp>
#include <SFML/System.hpp>

#include <Application.hpp>
#include <Random/Random.hpp>
#include <Interface/Updatable.hpp>
#include <Env/Env.hpp>
#include <Utility/Logging.hpp>

class FlowerGenerator : public Updatable
{
public:

    FlowerGenerator ();

    void
    update (sf::Time dt);

    void
    reset ();

private:

    sf::Time counter_;
};

#endif
