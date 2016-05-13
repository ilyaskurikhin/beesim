/*
 * Bear.cpp
 *
 *  Created on: May 13, 2016
 *      Author: ilya
 */

#include <Env/Bear.hpp>

Bear::Bear(const Vec2d& position, std::vector<State> states)
: Movable(position), CFSM(states)
{

}

Bear::~Bear()
{

}

void
Bear::reloadConfig()
{

}


