/**
 * @file Hive.hpp
 * @brief
 * @author Ilya Skurikhin
 * @author Julia Besson
 * @version p5.1
 * @date 2016-05-05
 */
#ifndef HIVE_H
#define HIVE_H

#include <Env/World.hpp>
#include <Env/Collider.hpp>
#include <Env/CFSM.hpp>

#include <Utility/Utility.hpp>
#include <Application.hpp>
#include <Utility/Logging.hpp>

#include <Interface/Drawable.hpp>
#include <Interface/Updatable.hpp>

class Bee;
class WorkerBee;
class ScoutBee;

/**
 * @brief Class simulating a Hive.
 */
class Hive : public Collider, public Drawable, public Updatable
{

public:

    /**
     * @brief Constructor.
     *
     * @param position graphic position of new Hive.
     * @param radius size of new Hive.
     */
    Hive(const Vec2d& position, double radius);

    Hive(const Hive&) = delete;

    Hive
    operator=(const Hive&) = delete;

    ~Hive();

    void
    reloadConfig();

    /**
     * @brief Add a new ScoutBee.
     *
     * @return pointer to new ScoutBee.
     */
    ScoutBee*
    addScout();

    /**
     * @brief Add a new WorkerBee.
     *
     * @return pointer to new WorkerBee.
     */
    WorkerBee*
    addWorker();

    /**
     * @brief Get a WorkerBee ready to get nectar.
     *
     * Get a WorkerBee that is in the hive and not busy.
     *
     * @return pointer to a WorkerBee.
     */
    WorkerBee*
    getWorker() const;

    /**
     * @brief Check is there is a Bee with such position.
     *
     * @param position graphic position
     *
     * @return poiter to existing Bee.
     */
    Bee*
    getBeeAt(const Vec2d& position);

    /**
     * @brief Evolve Hive.
     *
     * @param dt
     */
    void
    update(sf::Time dt) override;

    /**
     * @brief Draw hive on a target.
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
    double
    dropPollen(double amount);

    /**
     * @brief Take nectar from Hive.
     *
     * @param amount amount of nectar to take.
     *
     * @return amount of nectar taken
     */
    double
    takeNectar(double amount);

    /**
     * @brief Get amount to nectar in Hive.
     *
     * @return current amount of nectar.
     */
    double
    getNectar() const;

    int
    getNumberBees() const;

    void
    interactingBees();

private:

    double nectar_;
    double nectar_thresold_;
    double max_bees_;
    double reproduction_probability_;
    std::vector<Bee*> bees_;
    std::vector<WorkerBee*> workers_;
    std::vector<ScoutBee*> scouts_;
    sf::Texture hive_texture_;


};

#endif
