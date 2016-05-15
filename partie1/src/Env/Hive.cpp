#include <Env/Hive.hpp>
#include <Env/Bee.hpp>
#include <Env/WorkerBee.hpp>
#include <Env/ScoutBee.hpp>

Hive::Hive(const Vec2d& position, double radius) :
    Collider(position, radius), nectar_(
        getAppConfig()["simulation"]["hive"]["initial"]["nectar"].toDouble()), hive_texture_(
        getAppTexture(
            getAppConfig()["simulation"]["hive"]["texture"].toString()))
{
  reloadConfig();
}

Hive::~Hive()
{
  for (size_t i = 0; i < bees_.size(); ++i)
    {
      delete bees_[i];
    }
  bees_.clear();
}

void
Hive::reloadConfig()
{
  nectar_thresold_ =
      getAppConfig()["simulation"]["hive"]["reproduction"]["nectar threshold"].toDouble();
  max_bees_ =
      getAppConfig()["simulation"]["hive"]["reproduction"]["max bees"].toDouble();
  reproduction_probability_ =
      getAppConfig()["simulation"]["hive"]["reproduction"]["scout probability"].toDouble();
}

ScoutBee*
Hive::addScout()
{
  std::vector<State> states =
    { Bee::IN_HIVE, ScoutBee::SEARCH_FLOWER, ScoutBee::RETURN_HIVE };
  ScoutBee* scout(new ScoutBee(this, this->getPosition(), states));
  scout->reloadConfig();
  bees_.push_back(scout);
  scouts_.push_back(scout);
  return scout;
}

WorkerBee*
Hive::addWorker()
{
  std::vector<State> states =
    { Bee::IN_HIVE, WorkerBee::TO_FLOWER, WorkerBee::COLLECT_POLLEN,
        WorkerBee::RETURN_HIVE };
  WorkerBee* worker(new WorkerBee(this, this->getPosition(), states));
  worker->reloadConfig();
  bees_.push_back(worker);
  workers_.push_back(worker);
  return worker;
}

Bee*
Hive::getBeeAt(const Vec2d& position)
{
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
Hive::update(sf::Time dt)
{

  if (bees_.size() < max_bees_ && nectar_ > nectar_thresold_)
    {
      if (bernoulli(reproduction_probability_))
        {
          this->addWorker();
        }
      else
        {
          this->addScout();
        }
    }

  for (size_t i = 0; i < bees_.size(); ++i)
    {
      bees_[i]->update(dt);

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

  for (size_t i = 0; i < bees_.size(); ++i)
    {
      bees_[i]->drawOn(target);
    }

  if (isDebugOn())
    {
      std::string valueString;
      sf::Color color(sf::Color::Blue);
      Vec2d position;
      double text_size(getAppEnv().getTextSize());

      position = this->getPosition();
      position.y -= text_size;

      valueString = "Nectar: " + to_nice_string(nectar_);
      sf::Text text = buildText(valueString, position, getAppFont(), text_size,
                                color);
      target.draw(text);
    }
}

double
Hive::dropPollen(double nectar)
{
  if (nectar > 0)
    {
      nectar_ = nectar_ + nectar;
    }
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

double
Hive::getNectar() const
{
  return nectar_;
}

std::vector <Bee*>
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
Hive::getNumScouts() const
{
  int numScouts(0);
  for (size_t i = 0; i < bees_.size(); ++i)
    {
      if (bees_[i]->isScout())
        {
          ++numScouts;
        }
    }
  return numScouts;
}

int
Hive::getNumWorkers() const
{
  int numWorkers(0);
  for (size_t i = 0; i < bees_.size(); ++i)
    {
      if (bees_[i]->isWorker())
        {
          ++numWorkers;
        }
    }
  return numWorkers;
}

void
Hive::interactingBees()
{
  std::vector<Bee*> beesInHive;
  for (size_t i(0); i < bees_.size(); ++i)
    {
      if (bees_[i]->isInHive())
        {
          beesInHive.push_back(bees_[i]);
        }
    }
  for (size_t i(0); i < beesInHive.size(); ++i)
    {
      for (size_t j(0); j < beesInHive.size(); ++j)
        {
          beesInHive[i]->interact(beesInHive[j]);
        }
    }
}
