#include "Collider.hpp"
#include <iostream>
#include "../Utility/Vec2d.hpp"
#include <Application.hpp>
#include <cassert>

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


	 
Collider::Collider(const Collider& col) {
	radius_=col.getRadius();
	position_=col.getPosition();
}


	
Collider& 
operator= (const Collider& col) {
	Collider tmp(col);
	swap(radius_,tmp.getRadius());
	swap(position_,tmp.getPosition());
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



Vec2d 
Collider::getPosition() const {
	return position_;
}



double 
Collider::getRadius() const {
	return radius_;
}
