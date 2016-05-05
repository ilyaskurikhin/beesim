/**
 * @file Collider.hpp
 * @brief 
 * @author Ilya Skurikhin
 * @author Julia Besson
 * @version p5.1
 * @date 2016-05-05
 */

#ifndef COLLIDER_H
#define COLLIDER_H

#include <Utility/Vec2d.hpp>
#include <Application.hpp>
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
   * @brief Class constructor.
   *
   * Construct a Collider from a postions and a radius.
   *
   * @param position vector of the Collider
   * @param radius radius of the Collider
   */
  Collider (const Vec2d& position, double radius);

  /**
   * @brief Small constructor.
   *
   * Constructor for only a position, 0 radius
   *
   * @param position where to create the Collider.
   */
  Collider (const Vec2d& position);

  /**
   * @brief Copy constructor.
   *
   * Copy all the atttributes from another Collider.
   *
   * @param collider Collider to copy.
   */
  Collider (const Collider& collider);

  //
  // OPERATORS
  //

  /**
   * @brief Check if other is in this.
   *
   * @param other another collider.
   *
   * @return true if other is in this.
   */
  bool
  operator> (const Collider& other) const;

  /**
   * @brief Check if this Collider is colliding with another.
   *
   * @param other another collider.
   *
   * @return true if this is colliding with other.
   */
  bool
  operator| (const Collider& other) const;

  /**
   * @brief Check if a point p is in the collider.
   *
   * @param p a vector point
   *
   * @return true if point p is within radius_ of this.
   */
  bool
  operator> (const Vec2d& p) const;

  /**
   * @brief Copy operator.
   *
   * Call to copy constructor.
   *
   * @param collider Collider to copy from.
   */
  Collider&
  operator= (Collider collider);

  /** 
   * @brief Move this horizontaly by dx.
   *
   * @param Vector to move this Collider by.
   */
  Collider&
  operator+= (const Vec2d& dx);

  //
  // METHODS
  //

  /** 
   * @brief Calculate new toric position.
   *
   * Clamping method checks that position is within toric grid.
   * This method will check that the position is not on a different
   * face of the world, and will correct the position if it is.
   */
  Vec2d
  clamping ();

  /**
   * @brief Check if other is in this Collider. 
   *
   * @param Collider to check for.
   *
   * @return true if this is within radius of other.
   */
  bool
  isColliderInside (const Collider& other) const;

  /**
   * @brief Check if colliding.
   *
   * @param other Collider to check collision with.
   *
   * @return true if either of this or other are within the others radius.
   */
  bool
  isColliding (const Collider& other) const;

  /**
   * @brief Check if inside other.
   * 
   * @param p position to check for.
   *
   * @return true is distance in between p and this < radius_.
   */
  bool
  isPointInside (const Vec2d& p) const;

  /**
   * @brief Calculate path to point. 
   *
   * Calculate the shortest toric path to a position on the grid.
   * 
   * @param other A position on the grid.
   * 
   * @return direction vector to other.
   */
  Vec2d
  directionTo (const Vec2d& other) const;

  /**
   * @brief Calculate path to Collider.
   *
   * Calculate the shortest toric path to another Collider on the grid.
   *
   * @return direction vector to other.
   *
   * @param other Another Collider.
   */
  Vec2d
  directionTo (const Collider& other) const;

  /**
   * @brief Calculate distance to point.
   *
   * @param other A position on the grid.
   *
   * @return distance to other.
   */
  double
  distanceTo (const Vec2d& other) const;

  /**
   * @brief Calculate distance to Collider.
   *
   * @param collider another Collider.
   *
   * @return distance to other.
   */
  double
  distanceTo (const Collider& other) const;

  /**
   * @brief Move this Collider by the vector dx.
   *
   * @param dx Vector to move Collider by.
   *
   * @return Collider with new location.
   */
  Collider&
  move (const Vec2d& dx);

  // 
  // GETTERS & SETTERS
  //

  /**
   * @brief Get the position.
   *
   * @return position of this Collider.
   */
  Vec2d
  getPosition () const;

  /**
   * @brief Get the radius.
   *
   * @return radius of this Collider.
   */
  double
  getRadius () const;

protected:

  double radius_; /** radius of this Collider. */
  Vec2d position_; /** position of this Collider. */
};

/** 
 * @brief Print the contents of this to a stream.
 *
 * @param oss stream to print to.
 * @param collider Collider to print from.
 */
std::ostream&
operator<< (std::ostream& oss, const Collider& collider);

#endif
