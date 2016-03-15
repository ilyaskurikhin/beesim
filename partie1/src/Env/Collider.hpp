#ifndef COLLIDER_H
#define COLLIDER_H
#include "../Utility/Vec2d.hpp"

class Collider 
{
	public:

	Collider (Vec2d position, double radius);
	
	//constructeur de copie
	Collider (const Collider& col);
	
	//surcharge de l'opérateur de copie
	Collider& operator= (const Collider& col)
	
	//methode de clamping corrigeant la position
	Vec2d clamping(); 

	//permet d'obtenir la position
	Vec2d getPosition() const;

	//permet d'obtenir le rayon
	double getRadius() const;
	
	
	private:

	Vec2d position_;
	double radius_;
};

#endif
