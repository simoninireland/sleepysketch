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
#include "SleepySketch.h"
#include "RepeatingActor.h"

/**
   Actor execution framework.

   The sleepy sketch is a queue of actors that is maintained
   in time order. The system sleeps using a Sleeper in those periods
   when no actor is scheduled, minimising power consumption. The choice
   of sleeper determines the level of sleep (and hence the level of
   power-saving) available from this approach: deep sleep for purely
   software-driven applications can yield considerable savings.

   SleepySketch is intended to be a singleton class, since it makes
   no sense to have multiple queues. The instance Sleepy can then be
   used directly as a target for the public API, i.e. Sleepy.loop()
*/


/**
   The singleton instance.
*/
SleepySketch Sleepy;


/**
   Create a new sleepy sketch.
   The sketch needs to be inialised using init();
*/
SleepySketch::SleepySketch() { }


/**
   Return a free scheduling slot, removing it from the free queue.
   \return a scheduling slot
*/
ActorSchedule *SleepySketch::freeSchedule() {
  ActorSchedule *sched = freeQueue;
  if(sched != NULL) {
    freeQueue = sched->next;   sched->next = NULL;
  }
  return sched;
}


/**
   Return the next actor scheduled to run.
   \return a scheduling slot
*/
ActorSchedule *SleepySketch::nextSchedule() {
  ActorSchedule *sched = queue;
  if(sched != NULL) {
    queue = sched->next;   sched->next = NULL;
  }
  return sched;
}


/**
   Mark a schedule as free, reurning it to the free queue for re-use.
   We add the schedule to the front of the free queue for speed, since
   there's no point worrying about fairness.
   \param s a scheduling slot
*/
void SleepySketch::refreeSchedule( ActorSchedule *s ) {
  s->next = freeQueue;   freeQueue = s;
}


// ********** Public API **********

/**
   Initialise the new sleepy sketch.
   \param s the sleeper controlling down-time
   \param m the maximum number of schedulable actors
*/
void SleepySketch::begin( Sleeper *s, int m ) {
  sleeper = s;
  maxActors = m;

  // initialise the scheduling queues
  queue = NULL;
  freeQueue = new ActorSchedule[maxActors];  // all slots initially free
  for(int i = 0; i < maxActors; i++) {
    ActorSchedule *sched = &freeQueue[i];

    // link each schedule entry to the next, with all
    // being marked as un-used
    sched->actor = NULL;
    sched->next = ((i == maxActors - 1) ? NULL : &freeQueue[i + 1]);
  }
}


/**
   Schedule an actor.
   The actor is added to the scheduling queue along with its scheduled
   execution time.
   \param a the actor to be scheduled
   \param millis the time at which to schedule the actor
*/
void SleepySketch::scheduleIn( Actor *a, long millis ) {
  // set up the schedule for the actor
  ActorSchedule *sched = freeSchedule();
  if(sched == NULL) {
    Serial.println("No available schedule!");
  }
  sched->actor = a;
  sched->scheduledTime = now() + millis;

  // add new schedule to queue
  ActorSchedule *s = queue, *q = NULL;
  if(s == NULL) {
    // first schedule, add as new queue
    queue = sched;
  } else {
    // track down the schedule queue looking for earlier
    // schedules, and add the current schedule
    while(s != NULL) {
      if(s->scheduledTime > sched->scheduledTime) {
	// more recent, insert before
	if(q == NULL) {
	  // new first element
	  queue = sched;
	} else {
	  // point previous element to us
	  q->next = sched;
	}
	sched->next = s;
	return;
      }
      q = s;   s = s->next;
    }

    // if we get here, we've a new last element
    q->next = sched;
  }
}


/**
   Schedule the given actor repeatedly over the given period.
   We wrap the actor in a RepeatingActor combinator to provide
   the re-scheduling behaviour.
   \param a the actor
   \param millis the period to re-schedule.
*/
void SleepySketch::scheduleEvery( Actor *a, long millis ) {
  scheduleIn(new RepeatingActor(a, millis), millis);
}


/**
   Expand a time expressed in days, hours, minutes, and seconds into
   a time expressed in milliseconds.
   \param second number of seconds in the future
   \param minute number of minutes in the future
   \param hour number of hours in the future
   \param day number of days in the future
*/
long SleepySketch::expandTime( int second, int minute, int hour, int day ) {
  return ((second * 1000) +
	  (minute * 1000 * 60) +
	  (hour   * 1000 * 60 * 60) +
	  (day    * 1000 * 60 * 60 * 24));
}


/**
   Return the current time in milliseconds.
   This is an estimate of the number of milliseconds since the
   system restarted.
   \return the time in milliseconds
*/
long SleepySketch::now() {
  return sleeper->now();
}


/**
   Run the actor queue.
   The actor loop repeatedly acquires the next scheduled actor
   and then uses the sleeper to sleep for the appropriate time
   before running it.
*/
void SleepySketch::loop() {
  while(1) {
    ActorSchedule *sched = nextSchedule();

    if(sched == NULL) {
      Serial.println("No more actors scheduled");
      delay(1000);
    } else {
      // sleep for the time remaining until scheduled time
      long rem = sched->scheduledTime - now();
      if(rem < 0) {
	Serial.println("Running behind time...");
      } else {
	sleeper->sleepFor(rem);
      }

      // execute the actor
      sched->actor->execute();
      
      // free-up the schedule for re-use
      refreeSchedule(sched);
    }
  }
}
