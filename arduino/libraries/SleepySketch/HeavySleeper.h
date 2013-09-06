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

#ifndef HEAVYSLEEPER_H
#define HEAVYSLEEPER_H

#include "Sleeper.h"

/**
  \brief Sleeper that uses the Atmel ATmega's watchdog timer to enter the deepest available sleep mode.

  The watchdog timer provides the deepest-available sleep mode,
  and therefore the highest potential power savings -- at a cost of
  reducing activity to a minumum, and possibly disabling interrupts
  that might be needed. If a system that is software-driven -- so the
  activity of the microcontroller is completely determined by the
  software it is running, not by peripherals -- this is a good trade-off.
  However, it does mean that we have to be careful in keeping track
  of the time, since the usual millis() function doesn't count across
  sleep periods.
*/
class HeavySleeper : public Sleeper {
 protected:
  long wakeUpOffset;

  void sleep();
  void wdtSleep( int i );
  long wdtSleepFor( long millis );
  long timerSleepFor( long millis );

 public:
  /**
     Create a new heavy sleeper,
  */
  HeavySleeper();

  /**
     Delay the Arduino for a given number of milliseconds

     The watchdog timer can put the system asleep for only fixed periods of time,
     essentially powers-of-two from 16ms up to 8s. We use this for as long as possible,
     and then transfer to use Timer1 which has more precise but delays but less
     power-saving potential.
     
     \param millis number of milliseconds to sleep for
     \return an estimate of how many milliseconds we slept for
  */
  long sleepFor( long millis );

  /**
     Return the sleeper's estimate of the current time.
     We use the estimate of the time we've stayed asleep to
     update a local estimator.
     \return the number of milliseconds we've been awake
  */
  long now();
};

#endif

