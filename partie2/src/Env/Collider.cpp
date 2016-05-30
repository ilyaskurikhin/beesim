#include "Collider.hpp"
#include <iostream>
#include "../Utility/Vec2d.hpp"
#include <Application.hpp>
#include <cassert>
#include <vector>

using namespace std;


Collider::Collider (const Vec2d& position, double radius)
	// affecte par défaut le rayon et la position
	: radius_(radius), position_(position)
{ 
	// si le rayon<0 envoie un message d'erreur "assertion failed"
	assert (radius_ >= 0); 
	
	// fait appelle a la méthode clamping pour corriger la position entrée
	clamping();
}



Collider::Collider (const Collider& collider) 
{
	radius_ = collider.getRadius();
	position_ = collider.getPosition();
}



bool
Collider::operator> (const Collider& other) const
{   
        return isColliderInside(other);
}



bool
Collider::operator| (const Collider& other) const
{
         return isColliding(other);
}



bool 
Collider::operator> (const Vec2d& p) const
{
        return isPointInside(p);
}



std::ostream&
operator<< (std::ostream& oss, const Collider& collider)
{
        oss << "Collider: position = (" << collider.getPosition().x << "," << collider.getPosition().y << ") ";
        oss << "radius = " << collider.getRadius() << std::endl;

        return oss;
}



Collider& 
Collider::operator=(Collider other)
{
        position_ = other.getPosition();
        radius_ = other.getRadius();
	return *this;
}



Collider& 
Collider::operator+=(const Vec2d& dx) 
{
        return move(dx);
}



Vec2d 
Collider::clamping() 
{
                
        //permet d'obtenir largeur et hauteur du monde
        auto worldSize = getApp().getWorldSize();
        auto width  = worldSize.x;
        auto height = worldSize.y;
        
        //tant que position en x <0, on lui incrémente la largeur du monde
        //tant que position > largeur du monde, on lui décremente la largeur du monde
        
        while (position_.x < 0) {
                position_.x += width;
        }
                
        while(position_.x > width) {
                position_.x -= width;
        } 
                
        //idem pour position en y
        while (position_.y < 0) {
                position_.y += height;
        }
                
        while(position_.y > width) {
                position_.y -= height;
        } 
        
        //retourne le nouveau vec2d position
        return position_;
}



bool
Collider::isColliderInside (const Collider& other) const
{
        if ((other.getRadius() > radius_) || (distanceTo(other) > radius_ - other.getRadius())) {
            return false;
        } else {
            return true;
        }
}



bool
Collider::isColliding (const Collider& other) const
{
        double minimumDistance(other.getRadius() + radius_);

        if (distanceTo(other) <= minimumDistance) {
            return true;
        } else {
            return false;
        }
}



bool 
Collider::isPointInside (const Vec2d& p) const
{
        if (distanceTo(p) <= radius_) {
            return true;
        } else {
            return false;
        }
}


Vec2d
Collider::directionTo(const Vec2d& to) const
{
			
	// permet d'obtenir largeur et hautueur du monde
	auto worldSize = getApp().getWorldSize();
	auto width  = worldSize.x;
	auto height = worldSize.y;
	
	// create vector of possible positions of to 
	vector<vector<double>> multipliers; 
	multipliers.push_back({0,0});
	multipliers.push_back({0,1});
	multipliers.push_back({0,-1});
	multipliers.push_back({1,0});
	multipliers.push_back({-1,0});
	multipliers.push_back({1,1});
	multipliers.push_back({1,-1});
	multipliers.push_back({-1,1});
	multipliers.push_back({-1,-1});
	
	// find the 'to' at minimal distance from position_'
	Vec2d currentTo;
	Vec2d minimumTo;
        Vec2d moveTo;
	double min(100000);
	size_t size(multipliers.size());
	for (size_t i(0); i < size; ++i) {
		currentTo.x = multipliers[i][0]*width + to.x;
		currentTo.y = multipliers[i][1]*height + to.y;

                if (distance(position_, currentTo) < min) {
			minimumTo = currentTo;
			min = distance(position_, currentTo);
                        moveTo.x = minimumTo.x - position_.x;
                        moveTo.y = minimumTo.y - position_.y;
		}
	}

	return moveTo; 
}



Vec2d
Collider::directionTo(const Collider& other) const 
{
        return directionTo(other.getPosition());
}



double
Collider::distanceTo(const Vec2d& to) const 
{
        return directionTo(to).length();
}



double 
Collider::distanceTo(const Collider& other) const 
{
        return distanceTo(other.getPosition());
}



Collider&
Collider::move(const Vec2d& dx)
{
        position_.x += dx.x;
        position_.y += dx.y;

        clamping();
        return *this;
}



Vec2d 
Collider::getPosition() const 
{
	return position_;
}



double 
Collider::getRadius() const 
{
	return radius_;
}
