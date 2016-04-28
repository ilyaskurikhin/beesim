/*
 * WorkerBee.hpp
 *
 *  Created on: Apr 28, 2016
 *      Author: ilya
 */

#ifndef WORKERBEE_HPP
#define WORKERBEE_HPP

class WorkerBee : public Bee
{
public:

  WorkerBee (Hive* hive, Vec2d& position);

  ~WorkerBee ();

  void
  reloadConfig ();

  j::Value const&
  getConfig () const override;

private:

};

#endif /* WORKERBEE_HPP */
