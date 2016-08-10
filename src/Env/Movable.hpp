/*
 * Movable.hpp
 *
 *  Created on: May 13, 2016
 *      Author: ilya
 */

#ifndef MOVABLE_HPP_
#define MOVABLE_HPP_

#include <SFML/System.hpp>

#include <Utility/Vec2d.hpp>
#include <Random/Random.hpp>

#include <Env/Collider.hpp>

#include <string>

enum class MoveState : short {
    AT_REST, RANDOM, TARGET
};

class Movable : public Collider
{
public:

    Movable(const Vec2d& position);

    Movable(const Vec2d& position, double radius);

    Movable(const Movable& movable) = delete;

    virtual
    ~Movable() = default;

    const Movable&
    operator=(const Movable& movable) = delete;

    /**
     * @brief Set a target to move towards
     *
     * @param target graphic position.
     */
    void
    setMoveTarget(const Vec2d& target);

    /**
     * @brief Get the current move target
     *
     * @return graphic position of target.
     */
    const Vec2d&
    getMoveTarget() const;

    /**
     * @brief Move the bee towards a target defined by the type of bee.
     *
     * @param dt Time for the counter.
     */
    void
    targetMove(sf::Time dt);

    /**
     * @brief Move the bee randomly.
     *
     * Currently only implemented in ScoutBee.
     *
     * @param dt
     */
    void
    randomMove(sf::Time dt);

    virtual void
    move(sf::Time dt) =0;

    virtual bool
    isMovablePosition(const Vec2d& position) const;

    const Vec2d&
    getMoveVec() const;

    void
    rotateMoveVec(double angle);

    /**
     * @brief Get current speed.
     *
     * @return speed
     */
    double
    getSpeed() const;

    void
    setSpeed(double speed);

    void
    setDelay(sf::Time delay);

    void
    setMoveVec(const Vec2d& move_vec);

    void
    setRotationProbability(double rotationProbability);

    void
    setMaxAngle(double maxAngle);
    sf::Time
    getAvoidanceClock() const;
    void
    setAvoidanceClock(sf::Time avoidanceClock);
    sf::Time
    getDelay() const;
    double
    getMaxAngle() const;
    double
    getRotationProbability() const;

    const MoveState&
    getMoveState() const;

    void
    setMoveState(const MoveState& moveState);

private:

    Vec2d move_vec_;
    Vec2d move_target_;
    MoveState move_state_;
    double speed_;

    double max_angle_;
    double rotation_probability_;

    sf::Time avoidance_clock_;
    sf::Time delay_;

};

#endif /* MOVABLE_HPP_ */
