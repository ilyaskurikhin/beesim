/**
 * @file ScoutBee.cpp
 * @brief 
 * @author Ilya Skurikhina
 * @author Julia Besson
 * @version p5.1
 * @date 2016-05-05
 */

#include <Env/ScoutBee.hpp>

ScoutBee::ScoutBee(Hive* hive, const Vec2d& position, std::vector<State> states) :
    Bee(hive, position, states), number_times_shared_(-1)
{
  logEvent("ScoutBee", "new constructed");
  reloadConfig();
  loadTexture();
}

ScoutBee::~ScoutBee()
{

}

void
ScoutBee::drawOn(sf::RenderTarget& target) const
{
  Bee::drawOn(target);
  if (isDebugOn())
    {
      std::string valueString("Scout: ");
      sf::Color color(sf::Color::Yellow);
      Vec2d position;
      position.x = this->getPosition().x;
      position.y = this->getPosition().y - debug_text_size_;

      valueString = +"energy " + std::to_string(this->getEnergy());
      valueString = +"\n" + debug_status_;
      sf::Text text = buildText(valueString, position, getAppFont(),
				debug_text_size_, color);
      target.draw(text);
    }
}

void
ScoutBee::reloadConfig()
{
  Bee::reloadConfig();

  // TODO load class attributes here
  rotation_probability_ =
      getConfig()["moving behaviour"]["random"]["rotation probability"].toDouble();
  max_angle_ =
      getConfig()["moving behaviour"]["random"]["rotation angle max"].toDouble();

  energy_seek_flowers_ = getConfig()["energy"]["to seek flowers"].toDouble();
  energy_leave_hive_ = getConfig()["energy"]["to leave hive"].toDouble();

  max_sharing_ = getConfig()["sharing"]["max"].toDouble();

  debug_text_size_ = getAppEnv().getTextSize();
}

void
ScoutBee::randomMove(sf::Time dt)
{
  Vec2d position(position_);
  if (bernoulli(rotation_probability_))
    {
      double angleA(uniform(-max_angle_, max_angle_));
      move_vec_.rotate(angleA);
    }

  position.x += dt.asSeconds() * move_vec_.x;
  position.y += dt.asSeconds() * move_vec_.y;

  Collider protoBee(position, radius_);
  protoBee.clamping();
  if (!getAppEnv().isFlyable(protoBee.getPosition()))
    {
      double angleB;
      if (bernoulli(0.5))
	{
	  angleB = PI / 4;
	}
      else
	{
	  angleB = -PI / 4;
	}
      move_vec_.rotate(angleB);

    }
  else
    {
      this->position_ = protoBee.getPosition();
    }
}

j::Value const&
ScoutBee::getConfig()
{
  // TODO make this refer to superclass method
  return getAppConfig()["simulation"]["bees"]["scout"];
}

void
ScoutBee::onState(State state, sf::Time dt)
{
  // create vector to compare to for empty status
  Vec2d empty;
  empty.x = -1.0;
  empty.y = -1.0;

  // first state
  if (state == IN_HIVE)
    {
      if (flower_location_ != empty)
	{
	  WorkerBee* worker = this->getHive()->getWorker();
	  if (worker)
	    {
	      worker->setFlower(flower_location_);
	      flower_location_ = empty;
	      debug_status_ = "in_hive_shating";
	      debug_status_ = debug_status_
		  + std::to_string(number_times_shared_);
	      number_times_shared_ = -1;
	    }
	}

      if (this->getEnergy() < energy_leave_hive_)
	{
	  debug_status_ = "in_hive_eat";
	  this->eatFromHive(dt);
	}
      else if (flower_location_ == empty)
	{
	  debug_status_ = "in_hive_leaving";
	  this->nextState();
	}
    }

  // second state
  else if (state == SEARCH_FLOWER)
    {
      Flower* flower = this->findVisibleFlower();
      if ((flower != nullptr) and (this->getEnergy() > energy_seek_flowers_))
	{
	  flower_location_ = flower->getPosition();
	  number_times_shared_ = 0;
	  this->nextState();
	}
    }

  // third state
  else if (state == RETURN_HIVE)
    {
      this->setMoveTarget(this->getHive()->getPosition());
      if (getAppEnv().getCollidingHive(this->getPosition()) == this->getHive())
      //why are you not using isCollidinginside
	{
	  this->nextState();
	}
    }
}

void
ScoutBee::onEnterState(State state)
{
  if (state == IN_HIVE)
    {
      move_state_ = AT_REST;
    }
  else if (state == SEARCH_FLOWER)
    {
      move_state_ = RANDOM;
    }
  else if (state == RETURN_HIVE)
    {
      move_state_ = TARGET;
    }
}

void
ScoutBee::targetMove(sf::Time dt)
{
  Vec2d target = this->getMoveTarget();
  Vec2d direction = this->directionTo(target);

  direction = direction.normalised();

  Vec2d position;
  position += dt.asSeconds() * direction * this->getSpeed();

  Collider protoBee(position, this->getRadius());
  protoBee.clamping();
  if (!getAppEnv().isFlyable(protoBee.getPosition()))
    {
      double angleB;
      if (bernoulli(0.5))
	{
	  angleB = PI / 4;
	}
      else
	{
	  angleB = -PI / 4;
	}
      move_vec_.rotate(angleB);

    }
  else
    {
      this->position_ = protoBee.getPosition();
    }
}

State const ScoutBee::IN_HIVE = createUid();

State const ScoutBee::SEARCH_FLOWER = createUid();

State const ScoutBee::RETURN_HIVE = createUid();

