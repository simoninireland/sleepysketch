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
   \mainpage SleepySketch: Arduino sketches that spend most of their time asleep

   \author Simon Dobson <simon.dobson@st-andrews.ac.uk>
   \copyright Copyright (c) 2013, the University Court of the University of St Andrews. Licensed under the GNU General Public Licence.

   \section sec_overview Overview

   The Arduino is a popular platform for embedded and sensing systems.
   A major challenge with projects like this is power management,
   especially when running from batteries. The normal Arduino software
   development environment, perhaps surprisingly, is based around a
   model where programs ("sketches") run like normal programs and are
   assumed to be doing things.  If you want to access a sleep mode,
   you have to manage it yourself using a library like Narcolepsy.

   Sometimes, however, it is easier and more appropriate to use a
   model where a program sleeps by default and wakes up to run code
   according to a schedule. A sensor network, for example, might only
   wake up to make an observation every hour, spending the rest of its
   time asleep. Equally, it's sometimes attractive to have several tasks
   described independently, for example to take samples from different
   sensors on different schedules. These systems are hard to write cleanly
   when explicitly managing the sleep mode.

   The SleepySketch library provides a simple approach to writing
   programs using this "sleepy" mode. A program's code is structured
   as "actors" that can be scheduled to run at a given time. Outside
   the times when code is running, the sketch is automatically placed
   into a sleep mode. This can result in enormous power savings, and
   makes programming a lot simpler: you can separate functional code
   from scheduling, and combine different tasks cleanly, with
   SleepySketch taking care of the interleaving of tasks.

   \section sec_example Example

   A variant on the classic blinking lights demo as a SleepySketch
   looks like this:

   \code
   #include <SleepySketch.h>
   #include <HeavySleeper.h>

   #define LED 13

   class BlinkyActor : public Actor {
     private:
       int number;
     public:
       BlinkyActor( int n );
       void behaviour();
   };

   BlinkyActor::BlinkyActor( int n ) { number = n; }

   void BlinkyActor::behaviour() {
     for(int i = 0; i < number; i++) {
       digitalWrite(LED, HIGH);
       delay(300);
       digitalWrite(LED, LOW);
       delay(200);
     }
   }

   void setup() {
     pinMode(LED, OUTPUT);
     Sleepy.begin(new HeavySleeper());
     
     Sleepy.scheduleEvery(new BlinkyActor(10), 30 * 1000);
   }

   void loop() {
     Sleepy.loop();
   }
   \endcode

   The BlinkyActor class defines an Actor that blinks the on-board LED
   for a given number of flashes. In SleepySketch::begin() we schedule
   an instance of this Actor every 30s. The SleepySketch::loop()
   function runs the scheduler, and will cause the sketch to sleep
   using a HeavySleeper between runs of the BlinkyActor.

   For a full class diagram see \subpage classdiagram

   On a recent build this sketch required 4,370 bytes of storage. For
   comparison, the traditional blink demo requires 1,084 bytes. You're
   therefore paying a penalty of about 3k -- 9% of a standard Arduino's
   program memory -- for more convenient power management and potentially
   far better battery life.

   \section sec_usage Usage

   To create a sleepy sketch we need to do four things:

   - Choose a Sleeper to control the sleep mode
   - Initialise the SleepySketch to use this Sleeper
   - Create and schedule some actors
   - Run the main scheduling loop

   \subsection sec_sleeper Choosing a Sleeper

   The choice of Sleeper depends on how deep a sleep is required.
   The two ends of the spectrum are:

   - BusySleeper, which is simply a `delay()` loop wrapped-up as a Sleeper
   - HeavySleeper, which uses the Atmel's watchdog timer to enter its deepest sleep mode

   The deeper the sleep mode, the more hardware is disabled and the more
   power is saved, at a cost of having less hardware control over the
   microcontroller. Systems that rely on external interrupts can't generally
   use the deepest sleep mode.

   \subsection sec_initialise Initialise

   SleepySketch has a singleton instance, Sleepy, that is created
   automatically ans manages scheduling.  Calling Sleepy.init()
   initialises the sleepy sketch with the selected Sleeper.

   \subsection sec_actors Create actors

   An Actor is simply a piece of code that can be scheduled. Actors are
   intended to run for a bounded amount of time, and so shouldn't contain
   unbounded loops and the like.

   You create actors by sub-classing the Actor class and providing a
   definition of Actor::behaviour(). You can also override the
   Actor::pre() and Actor::post() methods to create families of actors
   with common wrappers before and after their specific
   behaviours. There are also some Actor combinators that can be used
   to combine and modify Actor's behaviours.

   \subsection sec_loop Run the main loop

   Running SleepySketch::loop() on \ref Sleepy runs the scheduling loop for the
   sleepy sketch. The scheduler will sleep using the Sleeper
   when there is no Actor scheduled, waking up as required
   to execute an Actor. This inverts the usual behaviour of
   Aruino code, in the sense that the system is asleep by default
   and wakes up to run code, rather than being generally awake and
   having the manage its own sleep schedule.


   \page classdiagram Class design

   \image html sleepysketch.png
*/


/**
   The singleton instance.

   All the SleepySketch functions should be called on Sleepy. It
   provides the actor queue and scheduling behaviour.
*/
SleepySketch Sleepy;


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


void SleepySketch::scheduleEvery( Actor *a, long millis ) {
  scheduleIn(new RepeatingActor(a, millis), millis);
}


long SleepySketch::expandTime( int second, int minute, int hour, int day ) {
  return ((second * 1000) +
	  (minute * 1000 * 60) +
	  (hour   * 1000 * 60 * 60) +
	  (day    * 1000 * 60 * 60 * 24));
}


long SleepySketch::now() {
  return sleeper->now();
}


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
