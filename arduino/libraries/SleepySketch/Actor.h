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

#ifndef ACTOR_H
#define ACTOR_H

/**
   \brief A routine intended to run for a bounded length of time, scheduled as part of a sleepy sketch.

   Each actor sub-class defines a behaviour, and may optionally define
   a pre- and post-behaviour action that can then be further inherited.
   (These might be used to enable peripherals prior to an observation
   and power them down afterwards, for example. The defaults do nothing.)

   There is nothing that prevents an actor defining a non-terminating
   behaviour, but to do so will screw up scheduling.
*/
class Actor {
 protected:
  Actor();

  /**
     The default pre-behaviour.
     The default does nothing. Override this function to support
     a family of actors with common initialisation.
  */
  virtual void pre();

  /**
     The default post-behaviour.
     The default does nothing. Override this function to support
     a family of actors with common finalisation.
  */
  virtual void post();

  /**
     The actor's behaviour.
     This function must be overridden by all sub-classes, and
     provides the "core" behaviour for the actor.
  */
  virtual void behaviour() = 0;

 public:
  /**
     Execute the actor's overall behaviour.
     This executes behavour(), bracketed by pre() and post().
  */
  void execute();
};

#endif

