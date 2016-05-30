/**
 * @file Env.hpp
 * @brief
 * @author Ilya Skurikhin
 * @author Julia Besson
 * @version p5.1
 * @date 2016-05-05
 */
#ifndef ENV_H
#define ENV_H

#include <SFML/Graphics.hpp>

#include <Interface/ConfigurableInterface.hpp>
#include <Interface/DrawableInterface.hpp>
#include <Interface/UpdatableInterface.hpp>

#include <Utility/Vec2d.hpp>
#include <Utility/Utility.hpp>
#include <Utility/Logging.hpp>
#include <Random/Random.hpp>

#include <Env/Bee/Bee.hpp>
#include <Env/Hive.hpp>
#include <Env/Collider.hpp>
#include <Env/World.hpp>
#include <Env/Bear.hpp>
#include <Env/Cave.hpp>

#include <memory>
#include <vector>
#include <string>
#include <unordered_map>

class Flower;
class FlowerGenerator;
class Hive;
class Bee;

/**
 * @brief Simulation Environment
 */
class Env : public DrawableInterface,
    public UpdatableInterface,
    public ConfigurableInterface
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
    Env();

    /**
     * @brief Class destructor
     */
    ~Env();

    void
    regenerate();

    /**
     * @brief Evolve the environment.
     *
     * Evolve the contents of the environement like the
     * World, Bees, etc.
     *
     * @param dt
     */
    void
    update(sf::Time dt) override;

    /**
     * @brief Draw the Env on the target
     *
     * Draw all the members on the target.
     *
     * TODO think about not having this call the members, but each memeber called
     *
     * @param target where to draw
     */
    void
    drawOn(sf::RenderTarget& target) const override;

    /**
     * @brief Reset the environment by regenerating.
     */
    void
    reset();

    /**
     * @brief Reload the config file
     */
    void
    reloadConfig() override;

    /**
     * @brief Load the world_ from file.
     */
    void
    loadWorldFromFile();

    /**
     * @brief Save the world_ to file.
     */
    void
    saveWorldToFile() const;

    const Vec2d&
    getWorldSize() const;

    /**
     * @brief Get humidity for position.
     *
     * @param position graphic position in world.
     *
     * @return humidity.
     */
    double
    getHumidity(const Vec2d& position) const;

    /**
     * @brief Check if flower can grow.
     *
     * Check for presence, rock or water.
     *
     * @param position where to grow.
     *
     * @return true is can be grown.
     */
    bool
    isGrowable(const Vec2d& position) const;

    /**
     * @brief Check if a Collider could be placed.
     *
     * Check for presence of Hives.
     *
     * @param position where to place.
     * @param radius radius of Collider.
     *
     * @return true is placement is possible.
     */
    bool
    isPlaceable(const Vec2d& position, double radius) const;

    bool
    isCavePlaceable(const Vec2d& position, double radius) const;

    /**
     * @brief Check is can be flown.
     *
     * Check for rock terrain.
     *
     * @param position where to check.
     *
     * @return true is flight possible.
     */
    bool
    isFlyable(const Vec2d& position) const;

    bool
    isWalkable(const Vec2d& position) const;

    bool
    canAddFlower();

    /**
     * @brief Add a Flower at position.
     *
     * @param position where to add flower.
     *
     * @return true if Flower can be added.
     */
    bool
    addFlowerAt(const Vec2d& position);

    bool
    addFlowerAt(const Vec2d& position, double size);

    bool
    addCaveAt(const Vec2d& position);

    bool
    addCaveAt(const Vec2d& position, double size);
    /**
     * @brief Draw zone occupied by flower.
     *
     * @param target where to draw.
     * @param position where flower would be.
     */
    void
    drawFlowerZone(sf::RenderTarget& target, const Vec2d& position);

    /**
     * @brief Add a Hive.
     *
     * @param position where to add Hive.
     *
     * @return true if Hive can be added.
     */
    bool
    addHiveAt(const Vec2d& position);

    bool
    addHiveAt(const Vec2d& position, double size);

    /**
     * @brief Draw zone occupied by Hive.
     *
     * @param target where to draw.
     * @param position where new Hive would be.
     */
    void
    drawHiveableZone(sf::RenderTarget& target, const Vec2d& position);

    /**
     * @brief Check for Hive collision, return if exists.
     *
     * @param body Collider to check for.
     *
     * @return pointer to Hive if exists.
     */
    Hive*
    getCollidingHive(const Collider& body) const;

    /**
     * @brief Check for Flower collision, return is exists.
     *
     * @param body Collider to check for.
     *
     * @return pointer to Flower if exists.
     */
    Flower*
    getCollidingFlower(const Collider& body) const;

    Cave*
    getCollidingCave(const Collider& body) const;

    /**
     * @brief Check for Bee at position.
     *
     * @param position where to check.
     *
     * @return pointer to Bee if exists.
     */
    Bee*
    getBeeAt(const Vec2d& position) const;

    int
    getNumScouts() const;

    int
    getNumWorkers() const;

    /**
     * @brief Get text size for env.
     *
     * @return text size
     */
    double
    getTextSize();

    std::unordered_map<std::string, double>
    fetchData(std::string graph) const;

private:

    World* world_;

    std::vector<Flower*> flowers_;
    FlowerGenerator* flower_generator_;
    std::vector<Hive*> hives_;
    std::vector<Cave*> caves_;

    double flower_min_nectar_;
    double flower_max_nectar_;
    double flower_manual_radius_;
    size_t max_flowers_;


    double hive_manual_radius_;
    double hiveable_factor_;

    double cave_manual_radius_;
    size_t max_caves_;

    double debug_text_size_;
};
#endif
