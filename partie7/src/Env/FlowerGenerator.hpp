/**
 * @file FlowerGenerator.hpp
 * @brief
 * @author Ilya Skurikhin
 * @author Julia Besson
 * @version p5.1
 * @date 2016-05-05
 */
#ifndef FLOWERGENERATOR_H
#define FLOWERGENERATOR_H

#include <SFML/Graphics.hpp>
#include <SFML/System.hpp>

#include <Interface/UpdatableInterface.hpp>

#include <Utility/Logging.hpp>
#include <Random/Random.hpp>

#include <Env/Env.hpp>

#include <Application.hpp>

/**
 * @brief Class generating Flower s in an Env ironment
 */
class FlowerGenerator : public UpdatableInterface
{
public:

    /**
     * @brief Default constructor.
     */
    FlowerGenerator();

    /**
     * @brief Evolve FlowerGeneratr.
     *
     * @param dt
     */
    void
    update(sf::Time dt);

    /**
     * @brief Reset counter of FlowerGenerator.
     */
    void
    reset();

private:

    sf::Time counter_;
    sf::Time delay_;
};

#endif
