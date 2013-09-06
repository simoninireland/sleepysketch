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

#ifndef REPEATINGACTOR_H
#define REPEATINGACTOR_H

#include "Actor.h"

/**
   \brief An Actor combinator that repeatedly runs another actor's behaviour.

   The behaviour of this actor is to run another actor and then
   re-schedule itself. This decouples the behaviour of an actor
   from its scheduling, while still allowing repeating behaviour.
   (An alternative design is to have the underlying actor re-schedule
   itself, thereby tying behaviour and scheduling together. This often
   makes perfect sense for a programmer.)

   The scheduling period runs from when the actor is first scheduled,
   not from when its behaviour ends. This allows for more precise timing
   periods, but means that if an actor takes longer to execute than
   its scheduled period, it will start to lag.
*/
class RepeatingActor : public Actor {
 protected:
  Actor *underlying;
  long period;

  void pre();
  void behaviour();

 public:
  /**
     Create a new repeating actor that repeatedly schedules the underlying Actor.
     The actor is initially un-scheduled.
     \param a the underlying actor to schedule
     \param millis the scheduling period in milliseconds
  */
  RepeatingActor( Actor *a, long millis );
};

#endif

