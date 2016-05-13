/*
 * Bear.hpp
 *
 *  Created on: May 13, 2016
 *      Author: ilya
 */

#ifndef BEAR_HPP_
#define BEAR_HPP_

#include <Interface/DrawableInterface.hpp>
#include <Interface/UpdatableInterface.hpp>
#include <Interface/ConfigurableInterface.hpp>

#include <Env/Movable.hpp>
#include <Env/CFSM.hpp>

#include <Utility/Vec2d.hpp>
#include <Utility/Logging.hpp>


class Bear : public DrawableInterface, public UpdatableInterface, public virtual ConfigurableInterface, public Movable, public CFSM
{
public:

  Bear(const Vec2d& position, std::vector<State> states);

  ~Bear();

  void
  reloadConfig() override;



};



#endif /* BEAR_HPP_ */
