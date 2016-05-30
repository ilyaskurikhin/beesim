#ifndef COLLIDER_H
#define COLLIDER_H
#include "../Utility/Vec2d.hpp"

class Collider
{
public:

    //
    // CONSTRUCTORS
    //

    Collider (const Vec2d& position, double radius);

    //constructeur de copie
    Collider (const Collider& collider);

    //
    // OPERATORS
    //

    //surcharge de l'opérateur de copie
    Collider&
    operator= (Collider collider);

    // return true if other is in this
    bool
    operator> (const Collider& other);

    // return true if this is colliding with other
    bool
    operator| (const Collider& other);

    // return true if point p is within radius_ of this
    bool
    operator> (const Vec2d& p);

    // print the contents of this to a stream
    friend
    std::ostream&
    operator<< (std::ostream& oss, const Collider& collider);

    // move this horizontaly by dx
    Collider&
    operator+= (const Vec2d& dx);

    //
    // METHODS
    //

    //methode de clamping corrigeant la position
    Vec2d
    clamping();

    // returns true if this is within radius of other
    bool
    isColliderInside (const Collider& other);

    // return true if either of this or other are within
    // the others radius
    bool
    isColliding (const Collider& other);

    // return true is distance in between p and this < radius_
    bool
    isPointInside (const Vec2d& p);

    // return direction vector to other
    Vec2d
    directionTo(const Vec2d& other);

    // return direction vector to other
    Vec2d
    directionTo(const Collider& other);

    // return distance to other
    double
    distanceTo(const Vec2d& other);

    // return distance to other
    double
    distanceTo(const Collider& other);

    // move this horizontaly by dx
    void
    move(const Vec2d& dx);


    //
    // GETTERS & SETTERS
    //

    //permet d'obtenir la position
    Vec2d
    getPosition() const;

    //permet d'obtenir le rayon
    double
    getRadius() const;

private:

    double radius_;
    Vec2d position_;
};

#endif
