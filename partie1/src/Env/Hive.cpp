#include <Env/Hive.hpp>
#include <Env/Bee.hpp>
#include <Env/WorkerBee.hpp>
#include <Env/ScoutBee.hpp>

Hive::Hive (const Vec2d& position, double radius) :
    Collider (position, radius), nectar_ (
        getAppConfig ()["simulation"]["hive"]["initial"]["nectar"].toDouble ()), hiveTexture_ (
        getAppTexture (
            getAppConfig ()["simulation"]["hive"]["texture"].toString ()))
{
}

Hive::~Hive ()
{
  for (size_t i = 0; i < bees_.size (); ++i)
    {
      delete bees_[i];
    }
  bees_.clear ();
}

ScoutBee*
Hive::addScout ()
{
  ScoutBee* scout (new ScoutBee (this, this->getPosition ()));
  scout->reloadConfig ();
  bees_.push_back (scout);
  return scout;
}

WorkerBee*
Hive::addWorker ()
{
  WorkerBee* worker (new WorkerBee (this, this->getPosition ()));
  worker->reloadConfig ();
  bees_.push_back (worker);
  return worker;
}

void
Hive::update (sf::Time dt)
{
  for (size_t i = 0; i < bees_.size (); ++i)
    {
      bees_[i]->update (dt);

      if (bees_[i]->getEnergy () == 0)
        {
          delete bees_[i];
          bees_[i] = nullptr;
        }

      bees_.erase (std::remove (bees_.begin (), bees_.end (), nullptr),
                   bees_.end ());

    }
}

void
Hive::drawOn (sf::RenderTarget& target) const
{
  auto hiveSprite = buildSprite (this->position_, this->radius_, hiveTexture_);
  target.draw (hiveSprite);

  for (size_t i = 0; i < bees_.size (); ++i)
    {
      bees_[i]->drawOn (target);
    }
}

double
Hive::dropPollen (double nectar)
{
  if (nectar > 0)
    {
      nectar_ = nectar_ + nectar;
    }
  return nectar_;
}

double
Hive::takeNectar (double nectar)
{
  if ((nectar > 0) && (nectar_ - nectar > 0))
    {
      nectar_ = nectar_ - nectar;
    }
  return nectar_;
}

double
Hive::getNectar ()
{
  return nectar_;
}

