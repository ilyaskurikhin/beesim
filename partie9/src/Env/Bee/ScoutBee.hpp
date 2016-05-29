/*
 * ScoutBee.hpp
 *
 *  Created on: Apr 28, 2016
 *      Author: Ilya Skurikhin
 */

#ifndef SCOUTBEE_HPP
#define SCOUTBEE_HPP

#include <JSON/JSON.hpp>

#include <Env/Bee/Bee.hpp>
#include <Env/Bee/WorkerBee.hpp>
#include <Env/Flower.hpp>
#include <Env/Env.hpp>

#include <Application.hpp>

class ScoutBee : public Bee
{
public:

    ScoutBee(Hive* hive, const Vec2d& position);

    ScoutBee(const ScoutBee& scoutbee) = delete;

    ~ScoutBee();

    const ScoutBee&
    operator=(const ScoutBee& scoutbee) = delete;

    void
    drawDebug(sf::RenderTarget& target) const override;

    void
    reloadConfig();

    j::Value const&
    getConfig() const override;

    void
    onState(State state, sf::Time dt) override;

    void
    onEnterState(State state) override;

    void
    interact(Bee* other) override;

    void
    interactWith(ScoutBee* scouting) override;

    void
    interactWith(WorkerBee* working) override;
    const Vec2d&
    getFlowerLocation() const;
    void
    setFlowerLocation(const Vec2d& flowerLocation);
    int
    getNumberTimesShared() const;

private:
    Vec2d flower_location_;
    int number_times_shared_;

    double energy_leave_hive_;
    double energy_seek_flowers_;

    double max_sharing_;

    State static const SEARCH_FLOWER;
    State static const IN_HIVE;
    State static const RETURN_HIVE;

};

#endif /* SCOUTBEE_HPP */
