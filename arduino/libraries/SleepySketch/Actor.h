/* $Id$

This file is part of Citizen Sensing, sensor networking for the citizen scientist
Copyright (c) 2013, the University Court of the University of St Andrews

Citizen Sensing is free software; you can redistribute it and/or
modify it under the terms of the GNU General Public License
as published by the Free Software Foundation; either version 2
of the License, or (at your option) any later version.

Citizen Sensing is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program; if not, write to the Free Software
Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA 02111, USA.
*/

/*
  Actor abstract class.
  An actor is a wrapper around some time-limited behaviour that can
  be scheduled via an ActorQueue.
*/

#ifndef ACTOR_H
#define ACTOR_H

/**
   Actor abstract class.
*/
class Actor {
 protected:
  /**
     Create a new actor.
  */
  Actor();

  /**
     Pre-behaviour action.
     Executed before the behaviour() code. The default does nothing.
  */
  virtual void pre();

  /**
     Post-behaviour action.
     Executed after the behaviour() code. The default does nothing.
  */
  virtual void post();

  /**
     The behaviour the actor should execute.
     This needs to be overridden by all sub-classes.
  */
  virtual void behaviour() = 0;

 public:
  /**
     Execute the actor's behaviour.
  */
  void execute();
};

#endif

