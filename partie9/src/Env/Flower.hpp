/**
 * @file Flower.hpp
 * @brief
 * @author Ilya Skurikhin
 * @author Julia Besson
 * @version p5.1
 * @date 2016-05-05
 */
#ifndef FLOWER_H
#define FLOWER_H

#include <SFML/Graphics.hpp>

#include <Interface/ConfigurableInterface.hpp>
#include <Interface/DrawableInterface.hpp>
#include <Interface/UpdatableInterface.hpp>

#include <Utility/Utility.hpp>
#include <Utility/Logging.hpp>
#include <Random/Random.hpp>

#include <Env/Collider.hpp>
#include <Env/Env.hpp>

#include <Application.hpp>


/**
 * @brief Class simulating a Flower.
 */
class Flower : public Collider,
    public DrawableInterface,
    public UpdatableInterface,
    public virtual ConfigurableInterface
{

public:
    /**
     * @brief Default constructor.
     */
    Flower();

    Flower(const Flower& flower) = delete;

    /**
     * @brief Detailed constructor.
     *
     * @param position graphic position of Flower.
     * @param radius size of Flower.
     * @param pollen initial pollen of Flower.
     */
    Flower(const Vec2d& position, double radius, double pollen);

    const Flower&
    operator=(const Flower& flower) = delete;

    void
    reloadConfig() override;

    /**
     * @brief Remove given quantity of pollen from Flower.
     *
     * @param pollen Quantity of pollen to take.
     *
     * @return Quantity of pollen taken.
     */
    double
    takePollen(const double& pollen);

    /**
     * @brief Load Flower texture.
     */
    void
    loadTexture();

    /**
     * @brief Evolve Flower.
     *
     * @param dt
     */
    void
    update(sf::Time dt) override;

    /**
     * @brief Draw Flower on target.
     *
     * @param target where to draw.
     */
    void
    drawOn(sf::RenderTarget& target) const override;

    /**
     * @brief Get current flower pollen content.
     *
     * @return current pollen content.
     */
    double
    getPollen();
    void
    setPollen(double pollen);

private:

    double pollen_;
    sf::Texture texture_;

    double humidity_factor_;
    double split_threshold_;
    double humidity_threshold_;
    double humidity_;
};

#endif
