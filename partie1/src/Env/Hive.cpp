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
}

Hive::~Hive()
{
  for (size_t i = 0; i < bees_.size(); ++i)
    {
      delete bees_[i];
    }
  bees_.clear();
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

WorkerBee*
Hive::getWorker() const
{
  for (size_t i = 0; i < workers_.size(); ++i)
    {
      if (workers_[i]->getState() == WorkerBee::IN_HIVE
          && workers_[i]->getFlower() == Vec2d(-1, -1))
        {
          return workers_[i];
        }
    }
  return nullptr;
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
  if ((nectar > 0) && (nectar_ - nectar > 0))
    {
      nectar_ = nectar_ - nectar;
    }
  return nectar_;
}

double
Hive::getNectar()
{
  return nectar_;
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
