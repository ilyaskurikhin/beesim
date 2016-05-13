/*
 * Stats.cpp
 *
 *  Created on: May 12, 2016
 *      Author: ilya
 */

#include <Stats/Stats.hpp>

Stats::Stats()

{

}

Stats::~Stats()
{

}

void
Stats::reloadConfig()
{
  delay_ = sf::seconds(static_cast<float>(getConfig()["refresh rate"].toDouble()));
}

const j::Value&
Stats::getConfig()
{
  return getAppConfig()["stats"];
}

void
Stats::reset()
{

}

void
Stats::drawOn(sf::RenderTarget& target) const
{
  getActiveGraph().drawOn(target);
}

void
Stats::update(sf::Time dt)
{
  counter_ += dt;

  if (counter_ > delay_)
    {

    }

}

void
Stats::setActive(int currentGraphId)
{
  active_ = currentGraphId;
}

const Graph&
Stats::getActiveGraph() const
{
  for (size_t i=0; i < titled_graphs_.size(); ++i)
    {
      if (titled_graphs_[i].id == active_)
        {
           return *titled_graphs_[i].graph;
        }
    }
}

void
Stats::addGraph(int graphId, std::string title, std::vector<std::string> series, double min, double max, const Vec2d& size)
{
  titled_graphs_.push_back({title, graphId, std::unique_ptr<Graph>( new Graph(series, size, min, max))});
}
