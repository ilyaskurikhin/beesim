/*
 * Stats.hpp
 *
 *  Created on: May 12, 2016
 *      Author: ilya
 */

#ifndef STATS_HPP
#define STATS_HPP

#include <Interface/Drawable.hpp>
#include <Interface/Updatable.hpp>

class Stats: public Drawable, public Updatable
{
public:

  Stats();

  ~Stats();

  void
  drawOn();

  void
  updata();


private:

};



#endif /* STATS_HPP */
