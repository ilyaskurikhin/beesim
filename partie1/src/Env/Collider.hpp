#ifndef COLLIDER_H
#define COLLIDER_H
#include "../Utility/Vec2d.hpp"

class Collider 
{
	public:

        // 
        // CONSTRUCTORS
        //

	Collider (Vec2d position, double radius);
	
	//constructeur de copie
	Collider (const Collider& collider);
	
        //
        // OPERATORS
        //

	//surcharge de l'opérateur de copie
	Collider& 
        operator= (const Collider& collider);
	
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
        std::ostream& 
        operator<< (std::ostream odd);
	
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
	
	Vec2d 
        directionTo(Vec2d to);
	
	// return true is distance in between p and this < radius_
	bool 
        isPointInside (Vec2d p);

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

	Vec2d position_;
	double radius_;
};

#endif
