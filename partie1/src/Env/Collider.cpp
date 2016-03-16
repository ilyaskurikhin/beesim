#include "Collider.hpp"
#include <iostream>
#include "../Utility/Vec2d.hpp"
#include <Application.hpp>
#include <cassert>
#include <vector>

using namespace std;


Collider::Collider(Vec2d position, double radius)
	// affecte par défaut le rayon et la position
	: radius_(radius), position_(position)
{ 
	//si le rayon<0 envoie un message d'erreur "assertion failed"
	assert (radius_ >= 0); 
	
	//fait appelle a la méthode clamping pour corriger la position entrée
	clamping();
}



bool
Collider::operator> (const Collider& other)
{   
        return isColliderInside(other);
}



bool
Collider::operator| (const Collider& other)
{
         return isColliding(other);
}



bool 
Collider::operator> (const Vec2d& p)
{
        return isPointInside(p);
}



std::ostream&
Collider::operator<< (std::ostream oss)
{
        oss << "Collider: position = (" << position_.x << "," << position_.y << ") ";
        oss << "radius = " << radius_ << std::endl;

        return oss;
}



	 
Collider::Collider(const Collider& col) {
	radius_=col.getRadius();
	position_=col.getPosition();
}



Collider Collider::operator=(Collider col) {
	swap(*this, col);
	return *this;
}



Vec2d 
Collider::clamping() {
                
        //permet d'obtenir largeur et hautueur du monde
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
                position_.y += width;
        }
                
        while(position_.y > width) {
                position_.y -= width;
        } 
        
        //retourne le nouveau vec2d position
        return position_;
}



bool
Collider::isColliderInside (const Collider& other)
{
        double dx(other.getPosition().x - position_.x);
        double dy(other.getPosition().y - position_.y);
        double d(sqrt(dx*dx + dy*dy));

        if (d > radius_) {
            return false;
        } else {
            return true;
        }
}



bool
Collider::isColliding (const Collider& other)
{
        double minimumDistance(other.getRadius() + radius_);

        double dx(other.getPosition().x - position_.x);
        double dy(other.getPosition().y - position_.y);
        double d(sqrt(dx*dx + dy*dy));

        if (d > minimumDistance) {
            return false;
        } else {
            return true;
        }
}



bool 
Collider::isPointInside (const Vec2d& p)
{
        double dx(p.x - position_.x);
        double dy(p.y - position_.y);
        double d(sqrt(dx*dx + dy*dy));

        if (d > radius_) {
            return false;
        } else {
            return true;
        }
}


Vec2d
Collider::directionTo(Vec2d to) {
	
			
	//permet d'obtenir largeur et hautueur du monde
	auto worldSize = getApp().getWorldSize();
	auto width  = worldSize.x;
	auto height = worldSize.y;
	
	Vec2d from(position_);
	
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
	
	// find the 'to' at minimal distance from 'from'
	Vec2d iTo;
	Vec2d toMin;
	Vec2d moveTo;
	double min(100000);
	size_t size(multipliers.size());
	for (size_t i(0); i < size; ++i) {
		iTo.x = multipliers[i][0]*width + to.x;
		iTo.y = multipliers[i][1]*height + to.y;
		
		moveTo.x = iTo.x - from.x;
		moveTo.y = iTo.y - from.y;

		if (moveTo.length() < min) {
			toMin = iTo;
			min = moveTo.length();
		}
	}

	return moveTo; 
}



Vec2d 
Collider::getPosition() const {
	return position_;
}



double 
Collider::getRadius() const {
	return radius_;
}
