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

#include <Arduino.h>
#include "RepeatingActor.h"
#include "SleepySketch.h"



RepeatingActor::RepeatingActor( Actor *a, long millis ) {
  underlying = a;
  period = millis;
}


/**
   Re-schedule the actor for a period into the future.
   Note that this happens before the actor's behaviour is executed,
   so the scheduling period is independent of how long the behaviour
   takes.
*/
void RepeatingActor::pre() {
  Sleepy.scheduleIn(this, period);
}


/**
   Execute the behaviour of the underlying actor.
*/
void RepeatingActor::behaviour() {
  underlying->execute();
}

