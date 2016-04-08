#ifndef COLLIDER_H
#define COLLIDER_H

#include <Utility/Vec2d.hpp>
#include <Application.hpp>
#include <Utility/Vec2d.hpp>
#include <iostream>
#include <cassert>
#include <vector>

/**
 * Allows checking for object collisions.
 * Methods allow checking for collisions in between class implementations.
 */
class Collider 
{
    public:

    // 
    // CONSTRUCTORS
    //

        
            
        /**
         * Class constructor.
         * Construct a Collider from a postions and a radius.
         * @param position vector of the Collider
         * @param radius radius of the Collider
         */
        Collider (const Vec2d& position, double radius);
	
	
        
        /**
         * Copy constructor.
         * Copy all the atttributes from another Collider.
         */
        Collider (const Collider& collider);
	
    //
    // OPERATORS
    //
        
        
        
        /**
         * Check if other is in this.
         * @param other another collider.
	 * @return true if other is in this.
         */
        bool 
        operator> (const Collider& other) const;
	
	
        
        /**
         * Check if this Collider is colliding with another.
         * @param other another collider.
         * @return true if this is colliding with other.
         */
        bool
        operator| (const Collider& other) const;
	
	
        
        /**
         * Check if a point p is in the collider.
         *@param p a vector point
         *@return true if point p is within radius_ of this.
         */
        bool 
        operator> (const Vec2d& p) const;
	


        /**
	 * Copy opperator, call to copy constructor.
         * @param collider Collider to copy from.
         */
        Collider& 
        operator= (Collider collider);



        /** 
         * Move this horizontaly by dx.
         * @param Vector to move this Collider by.
         */
        Collider& 
        operator+= (const Vec2d& dx);



    //
    // METHODS
    //



	/** 
         * Clamping method checks that position is within toric grid.
         * This method will check that the position is not on a different
         * face of the world, and will correct the position if it is.
         */
        Vec2d 
        clamping(); 



	/**
         * Check if other is in this Collider
         * @return true if this is within radius of other.
         */
        bool 
        isColliderInside (const Collider& other) const;



	/**
         * @return true if either of this or other are within the others radius.
         */
        bool 
        isColliding (const Collider& other) const;



	/**
         * @return true is distance in between p and this < radius_.
         */
        bool 
        isPointInside (const Vec2d& p) const;



        /**
         * Calculate the shortest toric path to a position on the grid.
         * @return direction vector to other.
         * @param other A position on the grid.
         */
        Vec2d 
        directionTo(const Vec2d& other) const;



        /**
         * Calculate the shortest toric path to another Collider on the grid.
         * @return direction vector to other.
         * @param other Another Collider.
         */
        Vec2d
        directionTo(const Collider& other) const;



        /**
         * @return distance to other.
         * @param other A position on the grid.
         */
        double
        distanceTo(const Vec2d& other) const;



        /**
         * @return distance to other.
         * @param collider another Collider
         */
        double 
        distanceTo(const Collider& other) const;
        


        /**
         * move this Collider by the vector dx.
         * @param dx Vector to move Collider by.
         */
        Collider& 
        move(const Vec2d& dx);



    // 
    // GETTERS & SETTERS
    //



	/**
         * Get the position.
         * @return position of this Collider.
         */
        Vec2d 
        getPosition() const;



	/**
         * Get the radius.
         * @return radius of this Collider.
         */
        double 
        getRadius() const;
	
    private:

        double radius_; /** radius of this Collider. */
        Vec2d position_; /** position of this Collider. */
};



/** 
 * Print the contents of this to a stream.
 * @param oss stream to print to.
 * @param collider Collider to print from.
 */
std::ostream& 
operator<< (std::ostream& oss, const Collider& collider);



#endif
