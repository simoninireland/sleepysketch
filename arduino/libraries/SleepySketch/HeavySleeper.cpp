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
  Sleeper that uses the Atmel ATmega's watchdog timer.
*/

#include "WatchdogSleeper.h"

#include <avr/power.h>
#include <avr/sleep.h>
#include <avr/wdt.h>


/**
   Sleep the Arduino.

   The Arduino is put to sleep in the appropriate sleep mode. This
   assumes that everything has been set up already.
*/
void WatchdogSleeper::sleep() {
  sleep_enable();
  sleep_mode();
  /* ...time passes ... */
  sleep_disable();
}


/**
   Use the watchdog timer to delay for the given number of milliseconds,
   encoded appropriately.

   The encoding is that i is such that the resulting delay will be
   (2048 << i) / 128000 seconds, for 0 <= i <= 9. This ranges from
   16ms up to 8s.

   \param i encoded millisecond delay
*/
void wdtSleep( int i ) {
  set_sleep_mode(SLEEP_MODE_PWR_DOWN);
  WDTCSR |= (1 << WDCE) | (1 << WDE);
  WDTCSR = (( (i & 1)       << WDP0) |  \\ write in encoded selection
	    (((i & 2) >> 1) << WDP1) |
	    (((i & 4) >> 2) << WDP2) |
	    (((i & 8) >> 3) << WDP3));
  WDTCSR |= (1 << WDIE);
}

/**
   Use Timer1 to delay for the given number of milliseconds.

   \param millis delay required in milliseconds
   \return number of milliseconds still to sleep for (may be <0 if we overslept)
*/
long timerSleep( long millis ) {
  // TBD
  return millis;
}


/**
   Use the watchdog timer to delay for along as possible.

   We use the watchdog timer's delay modes to step down through the time
   delay requested until we can't delay for a short enough time, i.e. the
   remaining time to sleep is less than 16ms.

   \param millis the number of milliseconds to sleep
   \return the number of milliseconds to stay asleep
*/
long WatchdogSleeper::wdtDelayFor( long millis ) {
  int i = 9;                  // largest delay  available
  long thresh = 2048 << 9;    // ...corresponds to this many milliseconds
  long ms = millis << 7;      // ms = millis * 128;
  
  while(i > 0 {
      while(ms > thresh) {
	wdtSleep(i);
	ms -= thresh;
      }
      i--;   thresh = thresh >> 1;
    }
    
    return (ms >> 7);   // return ms / 128
}


/**
   Delay the Arduino for millis ms

   The watchdog timer can put the system asleep for only fixed periods of time,
   essentially powers-of-two from 16ms up to 8s. We use this for as long as possible,
   and then transfer to use Timer1 which has more precise but delays but less
   power-saving potential.

   The estimation of actual sleep time is currently very poor.

   \param millis number of milliseconds to sleep for
   \return an estimate of how many milliseconds we slept for
*/
long WatchdogSleeper::sleepFor( long millis ) {
  long remaining = wdtDelayFor(millis);
  remaining = wdtDelayFor(millis);

  return (millis - remaining);
}
