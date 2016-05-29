/*
 * Stats.cpp
 *
 *  Created on: May 12, 2016
 *      Author: ilya
 */

#include <Stats/Stats.hpp>
#include <Env/Env.hpp>

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
    for (size_t i=0; i < titled_graphs_.size(); ++i) {
        titled_graphs_[i].graph->reset();
    }
}

void
Stats::drawOn(sf::RenderTarget& target) const
{
    for (size_t i=0; i < titled_graphs_.size(); ++i) {
        if (titled_graphs_[i].id == active_) {
            titled_graphs_[i].graph->drawOn(target);
        }
    }
}

void
Stats::update(sf::Time dt)
{
    counter_ += dt;

    if (counter_ > delay_) {
        for (size_t i=0; i < titled_graphs_.size(); ++i) {
            titled_graphs_[i].graph->updateData(dt,getAppEnv().fetchData(titled_graphs_[i].title));
        }
    }

}

void
Stats::setActive(int currentGraphId)
{
    active_ = currentGraphId;
}

void
Stats::addGraph(int graphId, std::string title, std::vector<std::string> series, double min, double max, const Vec2d& size)
{
    titled_graphs_.push_back({title, graphId, std::unique_ptr<Graph>( new Graph(series, size, min, max))});
}
