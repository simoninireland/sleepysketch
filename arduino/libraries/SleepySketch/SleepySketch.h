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

/** \file */

/**
   Version string.
*/
#define SLEEPYSKETCH_VERSION "0.1"

#include "Actor.h"
#include "Sleeper.h"

/**
   \def MAX_ACTORS
   The maximum number of scheduleable actors. Defaults to 10.
   Re-define this macro before importing the header file if
   you'd like a larger or smaller value.

   Each schedulable actor takes up 8 bytes of RAM (1 long
   and two pointers).
*/

#ifndef MAX_ACTORS
#define MAX_ACTORS 10
#endif

/**
   A scheduled Actor. The scheduling records are created statically
   to improve memory usage and managed as two queues, free and
   scheduled. The scheduled time is measured in milliseconds from
   system start-up, and is used in conjunction with the time estimate
   of the installed Sleeper to decide how long the SleepySketch should
   sleep for before executing the scheduled Actor.
*/
typedef struct _ActorSchedule {
  Actor *actor;
  long scheduledTime;
  struct _ActorSchedule *next;
} ActorSchedule;


/**
   \brief Actor execution framework.

   The sleepy sketch is a queue of actors that is maintained
   in time order. The system sleeps using a Sleeper in those periods
   when no actor is scheduled, minimising power consumption. The choice
   of sleeper determines the level of sleep (and hence the level of
   power-saving) available from this approach: deep sleep for purely
   software-driven applications can yield considerable savings.

   SleepySketch is intended to be a singleton class, since it makes
   no sense to have multiple queues. The instance Sleepy can then be
   used directly as a target for the public API, i.e. `Sleepy.loop()`.
*/
class SleepySketch {
 protected:
  int maxActors;
  Sleeper *sleeper;
  ActorSchedule *queue, *freeQueue;

  ActorSchedule *freeSchedule();
  ActorSchedule *nextSchedule();
  void refreeSchedule( ActorSchedule *s );

 public:
  /**
     Create a new sleepy sketch.
     The sketch needs to be initalised using `init()`.
  */
  SleepySketch();

  /**
     Initialise the new sleepy sketch.
     \param s the sleeper controlling down-time
     \param m the maximum number of schedulable actors
  */
  void begin( Sleeper *s, int m = MAX_ACTORS );

  /**
     Run the actor queue.
     The actor loop repeatedly acquires the next scheduled actor
     and then uses the sleeper to sleep for the appropriate time
     before running it.
  */
  void loop();

  /**
     Return the current time in milliseconds.
     This is an estimate of the number of milliseconds since the
     system restarted.
     \return the time in milliseconds
  */
  long now();

  /**
     Expand a time expressed in days, hours, minutes, and seconds into
     a time expressed in milliseconds.
     \param second number of seconds in the future
     \param minute number of minutes in the future
     \param hour number of hours in the future
     \param day number of days in the future
  */
  long expandTime( int second, int minute = 0, int hour = 0, int day = 0 );

  /**
     Schedule an actor.
     The actor is added to the scheduling queue along with its scheduled
     execution time.
     \param a the actor to be scheduled
     \param millis the time at which to schedule the actor
  */
  void scheduleIn( Actor *a, long millis );

  /**
     Schedule the given actor repeatedly over the given period.
     We wrap the actor in a RepeatingActor combinator to provide
     the re-scheduling behaviour.
     \param a the actor
     \param millis the period to re-schedule.
  */
  void scheduleEvery( Actor *a, long millis );
};

/**
   The singleton instance. Use this similar to Serial, i.e.
   Sleepy.loop().
*/
extern SleepySketch Sleepy;

#endif


