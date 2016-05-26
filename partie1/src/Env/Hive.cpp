#include <Env/Hive.hpp>
#include <Env/Bee/Bee.hpp>
#include <Env/Bee/WorkerBee.hpp>
#include <Env/Bee/ScoutBee.hpp>
#include <Env/Bee/QueenBee.hpp>

Hive::Hive(const Vec2d& position, double radius) :
    Collider(position, radius), nectar_(
        getAppConfig()["simulation"]["hive"]["initial"]["nectar"].toDouble())
{
  reloadConfig();
}

Hive::~Hive()
{
  // delete all the bees that belong to the hive
  for (size_t i = 0; i < bees_.size(); ++i)
    {
      delete bees_[i];
    }
  bees_.clear();
}

Bee*
Hive::addBee(BeeType beeType)
{
  Bee* bee;
  switch (beeType)
  {
    case BeeType::Scout :
      bee = new ScoutBee(this, getPosition());
      break;
    case BeeType::Worker :
      bee = new WorkerBee(this, getPosition());
      break;
    case BeeType::Queen :
      bee = new QueenBee(this, getPosition());
      break;
    default :
      return nullptr;
      break;
  }
  bee->reloadConfig();
  bees_.push_back(bee);
  return bee;
}

Bee*
Hive::addBee(Bee* bee)
{
  bees_.push_back(bee);
  return bee;
}

WorkerBee*
Hive::addWorker()
{
  WorkerBee* worker(new WorkerBee(this, this->getPosition()));
  worker->reloadConfig();
  bees_.push_back(worker);
  return worker;
}

ScoutBee*
Hive::addScout()
{
  ScoutBee* scout(new ScoutBee(this, this->getPosition()));
  scout->reloadConfig();
  bees_.push_back(scout);
  return scout;
}

Bee*
Hive::getBeeAt(const Vec2d& position)
{
  // check for each bee of the hive if they are at position
  for (size_t i = 0; i < bees_.size(); ++i)
    {
      if (bees_[i]->isPointInside(position))
        {
          return bees_[i];
        }
    }
  return nullptr;
}

void
Hive::interactingBees()
{
  // create a new vector that will contain bees in hive
  std::vector<Bee*> beesInHive;
  
  for (size_t i(0); i < bees_.size(); ++i)
    {
      // if bee is in hive add it to the vector
      if (bees_[i]->isInHive())
        {
          beesInHive.push_back(bees_[i]);
        }
    }
  
  // make each bee in hive interact with every other bee in hive
  for (size_t i(0); i < beesInHive.size(); ++i)
    {
      for (size_t j(0); j < beesInHive.size(); ++j)
        {
          beesInHive[i]->interact(beesInHive[j]);
        }
    }
}

double
Hive::dropPollen(double nectar)
{
  if (nectar > 0)
    nectar_ = nectar_ + nectar;
    
  return nectar_;
}

double
Hive::takeNectar(double nectar)
{
  double taken(0);

  if (nectar_ - nectar > 0)
    {
      nectar_ = nectar_ - nectar;
      taken = nectar;
    }
  else
    {
      nectar_ = 0;
      taken = nectar_;
    }
  return taken;
}

void
Hive::update(sf::Time dt)
{
  for (size_t i = 0; i < bees_.size(); ++i)
    {
      // update each bee of the hive
      bees_[i]->update(dt);
      
      // if one bee is dead delete it 
      if (bees_[i]->isDead())
        {
          delete bees_[i];
          bees_[i] = nullptr;
        }

      bees_.erase(std::remove(bees_.begin(), bees_.end(), nullptr),
                  bees_.end());
    }
}

void
Hive::drawOn(sf::RenderTarget& target) const
{
  auto hiveSprite = buildSprite(this->getPosition(), this->getRadius(),
                                hive_texture_);
  target.draw(hiveSprite);

  // call function drawOn for each bee of the hive
  for (size_t i = 0; i < bees_.size(); ++i)
    {
      bees_[i]->drawOn(target);
    }
}

void
Hive::drawDebug(sf::RenderTarget& target) const
{
      std::string valueString;
      sf::Color color(sf::Color::Blue);
      Vec2d position;
      double text_size(getAppEnv().getTextSize());

      // if debug mode is on, shows the amount of nectar
      valueString = "Nectar: " + to_nice_string(nectar_);


      position = this->getPosition();
      position.x += 75;

      sf::Text text = buildText(valueString, position, getAppFont(), text_size,
                                color);
      target.draw(text);

      // call function drawOn for each bee of the hive
      for (size_t i = 0; i < bees_.size(); ++i)
        {
          bees_[i]->drawDebug(target);
        }
}

void
Hive::reloadConfig()
{
  nectar_threshold_ =
      getAppConfig()["simulation"]["hive"]["reproduction"]["nectar threshold"].toDouble();
  migration_threshold_ =
      getAppConfig()["simulation"]["hive"]["migration threshold"].toDouble();
  max_bees_ =
      getAppConfig()["simulation"]["hive"]["reproduction"]["max bees"].toInt();


  max_hives_ = getAppConfig()["simulation"]["env"]["max hives"].toInt();
  max_queens_ = getAppConfig()["simulation"]["hive"]["max queens"].toInt();


  hive_texture_ =
          getAppTexture(
              getAppConfig()["simulation"]["hive"]["texture"].toString());
}

bool
Hive::canMigrate() const
{
  if (getNectar() > migration_threshold_
      && getAppEnv().getNumHives() < max_hives_
      && getNumBees(BeeType::Queen) < max_queens_)
    return true;
  else
    return false;
}

bool
Hive::canReproduce() const
{
  if ((getNectar() > nectar_threshold_)
      && (getNumBees() < max_bees_))
    return true;
  else
    return false;
}

void
Hive::removeBee(Bee* beeRemove)
{
  if (getNumBees(BeeType::Queen) > 0)
    {
      int i(0);
      int remove(-1);
      for (Bee* bee : bees_)
        {
          if (bee == beeRemove)
            {
              remove = i;
            }
          ++i;
        }
      if (remove >= 0)
        {
          bees_.erase(bees_.begin() + remove);
        }
    }
}

double
Hive::getNectar() const
{
  return nectar_;
}

std::vector<Bee*>
Hive::getBees() const
{
  return bees_;
}

int
Hive::getNumBees() const
{
  return bees_.size();
}

int
Hive::getNumBees(BeeType beeType) const
{
  int numScouts(0);
  for (size_t i = 0; i < bees_.size(); ++i)
    {
      if (bees_[i]->getBeeType() == beeType)
        {
          ++numScouts;
        }
    }
  return numScouts;
}
