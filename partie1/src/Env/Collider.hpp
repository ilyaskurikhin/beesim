#ifndef COLLIDER_H
#define COLLIDER_H
#include "../Utility/Vec2d.hpp"

class Collider 
{
	public:

	Collider (Vec2d position, double radius);
	
	//constructeur de copie
	Collider (const Collider& collider);
	
	//surcharge de l'opérateur de copie
	Collider& operator= (const Collider& collider);
	
	// return true if other is in this
	bool operator> (const Collider& other);
	
	// return true if this is colliding with other
	bool operator| (const Collider& other);
	
	// return true if point p is within radius_ of this
	bool operator> (const Vec2d& p);
	
	// print the contents of this to a stream
	ostream& operator<< ();
	
	//methode de clamping corrigeant la position
	Vec2d clamping(); 

	//permet d'obtenir la position
	Vec2d getPosition() const;

	//permet d'obtenir le rayon
	double getRadius() const;
	
	// returns true if this is within radius of other
	bool isColliderInside (const Collider& other);
	
	// return true if either of this or other are within
	// the others radius
	bool isColliding (const Collider& other);
	
	// return true is distance in between p and this < radius_
	bool isPointInside (Vec2d p);
	private:

	Vec2d position_;
	double radius_;
};

#endif
