#ifndef CAVE_H
#define CAVE_H

#include <Interface/DrawableInterface.hpp>
#include <Interface/UpdatableInterface.hpp>

#include <Utility/Logging.hpp>
#include <Utility/Utility.hpp>

#include <Env/World.hpp>
#include <Env/Collider.hpp>
#include <Env/CFSM.hpp>
#include <Env/Bear.hpp>

#include <Application.hpp>

class Bear;

/**
 * @brief Class simulating a Hive.
 */
class Cave : public Collider,
    public DrawableInterface,
    public UpdatableInterface,
    public virtual ConfigurableInterface
{
public:

    /**
     * @brief Constructor.
     *
     * @param position graphic position of new Hive.
     * @param radius size of new Hive.
     */
    Cave(const Vec2d& position, double radius);

    Cave(const Cave&) = delete;

    Cave
    operator=(const Cave&) = delete;

    ~Cave();

    void
    reloadConfig() override;

    /**
     * @brief Add a new Bear.
     *
     * @return pointer to new Bear.
     */
    Bear*
    addBear();

    /**
     * @brief Check is there is a Bear with such position.
     *
     * @param position graphic position
     *
     * @return poiter to existing Bear.
     */
    Bear*
    getBearAt(const Vec2d& position);

    Bear*
    getBear();

    /**
     * @brief Evolve Cave.
     *
     * @param dt
     */
    void
    update(sf::Time dt) override;

    /**
     * @brief Draw cave on a target.
     *
     * @param target where to draw
     */
    void
    drawOn(sf::RenderTarget& target) const override;

    /**
     * @brief Drop pollen into Hive.
     *
     * @param amount amount of pollen to add to Hive.
     *
     * @return new amont of pollen (nectar) in Hive.
     */

private:

    Bear* bear_;
    sf::Texture texture_;

};
#endif
