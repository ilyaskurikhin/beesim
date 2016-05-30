/*
 * Stats.hpp
 *
 *  Created on: May 12, 2016
 *      Author: ilya
 */

#ifndef STATS_HPP
#define STATS_HPP

#include <SFML/Graphics.hpp>
#include <SFML/System.hpp>

#include <Utility/Logging.hpp>
#include <Utility/Constants.hpp>
#include <Utility/Vec2d.hpp>

#include <Interface/Drawable.hpp>
#include <Interface/Updatable.hpp>

#include <Stats/Graph.hpp>

#include <Application.hpp>

#include <memory>
#include <map>

class Stats: public Drawable, public Updatable
{
public:

    Stats();

    ~Stats();

    void
    reloadConfig();

    const j::Value&
    getConfig();

    void
    reset();

    void
    drawOn(sf::RenderTarget& target) const override;

    void
    update(sf::Time dt) override;

    void
    setActive(int currentGraphID);

    void
    addGraph(int graphId, std::string title, std::vector<std::string> series, double min, double max, const Vec2d& size);


private:
    struct TitledGraph {
        std::string title;
        int id;
        std::unique_ptr<Graph> graph;
    };

    std::vector<TitledGraph> titled_graphs_;

    sf::Time delay_;
    sf::Time counter_;

    int active_;

};



#endif /* STATS_HPP */
