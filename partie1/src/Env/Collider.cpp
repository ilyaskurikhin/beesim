#include <Env/Collider.hpp>

Collider::Collider(const Vec2d& position, double radius)
// affecte par défaut le rayon et la position
:
    radius_(radius), position_(position)
{
  // PRECODITIONS for constructor
  if (radius_ < 0)
    {
      throw std::runtime_error(
          "Collider with negative radius created. (Collider::Collider)");
    }

  // puts Collider into map
  clamping();
}

Collider::Collider(const Vec2d& position) :
    Collider(position, 0)
{
}

Collider::Collider(const Collider& collider)
{
  radius_ = collider.getRadius();
  position_ = collider.getPosition();
}

bool
Collider::operator>(const Collider& other) const
{
  return isColliderInside(other);
}

bool
Collider::operator|(const Collider& other) const
{
  return isColliding(other);
}

bool
Collider::operator>(const Vec2d& p) const
{
  return isPointInside(p);
}

std::ostream&
operator<<(std::ostream& oss, const Collider& collider)
{
  oss << "Collider: position = (" << collider.getPosition().x << ","
      << collider.getPosition().y << ") ";
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
  Vec2d worldSize = getApp().getWorldSize();
  double width = worldSize.x;
  double height = worldSize.y;

  //tant que position en x <0, on lui incrémente la largeur du monde
  //tant que position > largeur du monde, on lui décremente la largeur du monde

  if (position_.x < 0)
    position_.x += width;

  if (position_.x > width)
    position_.x -= width;

  //idem pour position en y
  if (position_.y < 0)
    position_.y += height;

  if (position_.y > height)
    position_.y -= height;

  if (!isClamped())
    clamping();
  //retourne le nouveau vec2d position
  return position_;
}

bool
Collider::isClamped() const
{

  //permet d'obtenir largeur et hauteur du monde
  Vec2d worldSize = getApp().getWorldSize();
  double width = worldSize.x - 1;
  double height = worldSize.y - 1;

  if (position_.x < 0)
    return false;

  if (position_.x > width)
    return false;

  //idem pour position en y
  if (position_.y < 0)
    return false;

  if (position_.y > height)
    return false;

  return true;
}

bool
Collider::isColliderInside(const Collider& other) const
{
  if ((other.getRadius() > radius_)
      || (distanceTo(other) > radius_ - other.getRadius()))
    {
      return false;
    }
  else
    {
      return true;
    }
}

bool
Collider::isColliding(const Collider& other) const
{
  double minimumDistance(other.getRadius() + radius_);

  if (distanceTo(other) <= minimumDistance)
    {
      return true;
    }
  else
    {
      return false;
    }
}

bool
Collider::isPointInside(const Vec2d& p) const
{
  if (distanceTo(p) <= radius_)
    {
      return true;
    }
  else
    {
      return false;
    }
}

Vec2d
Collider::directionTo(const Vec2d& to) const
{
  Vec2d worldSize = getApp().getWorldSize();
  double width = worldSize.x;
  double height = worldSize.y;
  double max_x = width / 2;
  double max_y = height / 2;

  Vec2d direction;

  direction.x = to.x - position_.x;
  if (direction.x < -max_x)
    direction.x = to.x - position_.x + width;

  if (direction.x > max_x)
    direction.x = to.x - position_.x - width;

  direction.y = to.y - position_.y;
  if (direction.y < -max_y)
    direction.y = to.y - position_.y + height;

  if (direction.y > max_y)
    direction.y = to.y - position_.y - height;

  return direction;
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
  position_ += dx;

  clamping();
  return *this;
}

Vec2d
Collider::getPosition() const
{
  return position_;
}

void
Collider::setPosition(const Vec2d& position)
{
  position_ = position;
  clamping();
}

double
Collider::getRadius() const
{
  return radius_;
}

void
Collider::setRadius(double radius)
{
  try
    {
      radius_ = radius;
      if (radius_ < 0)
        {
          throw std::runtime_error("ERROR: Negative radius set (Collider)");
        }
    }
  catch (std::string& e)
    {
      radius_ = 0;
    }
}

const Collider&
Collider::getCollider()
{
  return *this;
}
