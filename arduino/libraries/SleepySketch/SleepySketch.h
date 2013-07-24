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

#ifndef SLEEPYSKETCH_H
#define SLEEPYSKETCH_H

#define SLEEPYSKETCH_VERSION "0.1"

#include "Actor.h"
#include "Sleeper.h"

#ifndef MAX_ACTORS
/**
   The maximum number of scheduleable actors. Defaults to 10.
   Re-define this macro before importing the header file if
   you'd like a larger or smaller value.

   Each schedulable actor takes up 8 bytes of RAM (1 long
   and two pointers).
*/
#define MAX_ACTORS 10
#endif

#include "Actor.h"
#include "Sleeper.h"

typedef struct _ActorSchedule {
  Actor *actor;
  long scheduledTime;
  struct _ActorSchedule *next;
} ActorSchedule;

class SleepySketch {
 protected:
  int maxActors;
  long tickTime;
  Sleeper *sleeper;
  ActorSchedule *queue, *freeQueue;

  ActorSchedule *freeSchedule();
  ActorSchedule *nextSchedule();
  void refreeSchedule( ActorSchedule *s );

 public:
  SleepySketch();

  void begin( Sleeper *s, int m = MAX_ACTORS );
  void loop();

  long now();
  long expandTime( int second, int minute = 0, int hour = 0, int day = 0 );

  void scheduleIn( Actor *a, long millis );
  void scheduleEvery( Actor *a, long millis );
};

extern SleepySketch Sleepy;

#endif


