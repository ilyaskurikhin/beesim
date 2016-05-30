#ifndef ENV_H
#define ENV_H

#include <SFML/Graphics.hpp>

#include <Utility/Vec2d.hpp>
#include <Utility/Utility.hpp>
#include <Utility/Logging.hpp>
#include <Random/Random.hpp>
#include <Interface/Drawable.hpp>
#include <Interface/Updatable.hpp>
#include <Env/Hive.hpp>
#include <Env/Collider.hpp>
#include <Env/World.hpp>

#include <memory>

class Flower;
class FlowerGenerator;

class Env : public Drawable, public Updatable
{
public:
    /**
     * @brief Default class constructor.
     *
     * Construct a World in world_ from file in configuration.
     * If file cannot be loaded, generate World.
     * Draw the world_ to cache.
     * Initialize numberFlowers_ to 0.
     */
    Env ();

    ~Env ();

    void
    update (sf::Time dt) override;

    void
    drawOn (sf::RenderTarget& target) const override;

    void
    reset ();

    void
    reloadConfig ();

    void
    loadWorldFromFile ();

    void
    saveWorldToFile () const;

    double
    getHumidity (const Vec2d& position) const;

    bool
    isGrowable (const Vec2d& position) const;

    bool
    isPlaceable (const Vec2d& position, double radius) const;

    bool
    isFlyable (const Vec2d& position) const;

    bool
    addFlowerAt (const Vec2d& position);

    void
    drawFlowerZone (sf::RenderTarget& target, const Vec2d& position);

    bool
    addHiveAt (const Vec2d& position);

    Hive*
    getCollidingHive (const Collider& body) const;

    Flower*
    getCollidingFlower (const Collider& body) const;

private:

    World* world_;
    std::vector<Flower*> flowers_;
    FlowerGenerator* flowerGenerator_;
    std::vector<Hive*> hives_;

    double flowerMinNectar_;
    double flowerMaxNectar_;
    double flowerManualRadius_;
    size_t maxFlowers_;

    double hiveManualRadius_;
};
#endif
