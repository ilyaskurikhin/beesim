#ifndef ENV_H
#define ENV_H

#include <Env/World.hpp>
#include <Utility/Vec2d.hpp>
#include <SFML/Graphics.hpp>
#include <Env/Flower.hpp>
#include <Utility/Utility.hpp>

class Env
{
    public :
        
        Env();

        void
        update(sf::Time dt);

        void
        drawOn(sf::RenderTarget& target);

        void
        reset();

        void
        loadWorldFromFile();

        void
        saveWorldToFile();

        bool
        addFlowerAt(const Vec2d& position);

        void
        drawFlowerZone(sf::RenderTarget& target, const Vec2d& position);

    private :

        World world_;
        std::vector<Flower*> flowers_;
		double numberFlowers_;
};
#endif
