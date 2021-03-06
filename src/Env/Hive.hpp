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

#include <Interface/DrawableInterface.hpp>
#include <Interface/UpdatableInterface.hpp>

#include <Utility/Logging.hpp>
#include <Utility/Utility.hpp>

#include <Env/World.hpp>
#include <Env/Collider.hpp>
#include <Env/CFSM.hpp>

#include <Application.hpp>

enum class BeeType : short;
    class Bee;
    class WorkerBee;
    class ScoutBee;

    /**
     * @brief Class simulating a Hive.
     */
    class Hive : public Collider,
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
    Hive(const Vec2d& position, double radius);

    Hive(const Hive&) = delete;

    ~Hive();

    Hive
    operator=(const Hive&) = delete;

    /**
     * @brief Add a new Bee.
     *
     * @param beeType type of Bee.
     *
     * @return pointer to new ScoutBee.
     */
    Bee*
    addBee(BeeType beeType);

    Bee*
    addBee(Bee* bee);

    // methods for compatibility with tests
    ScoutBee*
    addScout();
    WorkerBee*
    addWorker();

    /**
     * @brief Check is there is a Bee with such position.
     *
     * @param position graphic position
     *
     * @return poiter to existing Bee.
     */
    Bee*
    getBeeAt(const Vec2d& position);

    void
    interactingBees();

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

    void
    drawDebug(sf::RenderTarget& target) const;

    void
    reloadConfig() override;

    bool
    canMigrate() const;

    bool
    canReproduce() const;

    void
    removeBee(Bee* bee);

    /**
     * @brief Get amount to nectar in Hive.
     *
     * @return current amount of nectar.
     */
    double
    getNectar() const;

    std::vector<Bee*>
    getBees() const;

    int
    getNumBees() const;

    int
    getNumBees(BeeType beeType) const;

private:

    double nectar_;
    double nectar_threshold_;
    double migration_threshold_;
    int max_bees_;
    int max_hives_;
    double reproduction_probability_;
    std::vector<Bee*> bees_;
    sf::Texture hive_texture_;

};

#endif
