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
#include "HeavySleeper.h"
#include <avr/power.h>
#include <avr/sleep.h>
#include <avr/wdt.h>

/*
  Sleeper that uses the Atmel ATmega's watchdog timer.
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


/*
   Dummy watchdog timer interrupt vector, needed to prevent
   a system reset but not otherwise used
*/ 
ISR( WDT_vect ) {
  /* dummy */
}


/**
   Create a new heavy sleeper,
*/
HeavySleeper::HeavySleeper() {
  wakeUpOffset = 0L;
}


/**
   Sleep the Arduino.
   The Arduino is put to sleep in the appropriate sleep mode. This
   assumes that everything has been set up already.
*/
void HeavySleeper::sleep() {
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
void HeavySleeper::wdtSleep( int i ) {
  // compute bitmask for sleep period
  int mask = 0;
  if(i & 1) mask |= 1 << WDP0;
  if(i & 2) mask |= 1 << WDP1;
  if(i & 4) mask |= 1 << WDP2;
  if(i & 8) mask |= 1 << WDP3;
    
  // set up watchdog timer
  set_sleep_mode(SLEEP_MODE_PWR_DOWN);   // very low-power sleep
  MCUSR &= ~(1 << WDRF);                 // clear reset flag
  WDTCSR |= (1 << WDCE) | (1 << WDE);    // allow updates
  WDTCSR = mask;                         // update delay time
  WDTCSR |= (1 << WDIE);                 // enable interrupts (to prevent reset)

  // go to sleep
  sleep();
}

/**
   Use Timer1 to delay for the given number of milliseconds.

   \param millis delay required in milliseconds
   \return number of milliseconds slept for
*/
long HeavySleeper::timerSleepFor( long millis ) {
  // for now we just busily delay
  delay(millis);
  return millis;
}


/**
   Use the watchdog timer to delay for as long as possible.

   We use the watchdog timer's delay modes to step down through the time
   delay requested until we can't delay for a short enough time, i.e. the
   remaining time to sleep is less than 16ms.

   \param millis the number of milliseconds to sleep
   \return the number of milliseconds actually slept for
*/
long HeavySleeper::wdtSleepFor( long millis ) {
  int i = 9;                  // largest delay available
  long thresh = 2048 << 2;    // ...which corresponds to this many milliseconds
  long ms = millis;
  
  while(i > 0) {
    while(ms > thresh) {
      wdtSleep(i);
      ms -= thresh;
    }
    i--;   thresh = thresh >> 1;
  }
    
  return (millis - ms);
}


/**
   Delay the Arduino for a given number of milliseconds

   The watchdog timer can put the system asleep for only fixed periods of time,
   essentially powers-of-two from 16ms up to 8s. We use this for as long as possible,
   and then transfer to use Timer1 which has more precise but delays but less
   power-saving potential.

   \param millis number of milliseconds to sleep for
   \return an estimate of how many milliseconds we slept for
*/
long HeavySleeper::sleepFor( long millis ) {
  long wdt = wdtSleepFor(millis);
  long remaining = millis - wdt;
  long timer = timerSleepFor(remaining);

  wakeUpOffset += wdt + timer;
  return wdt + timer;
}


/**
   Return the sleeper's estimate of the current time.
   We use the estimate of the time we've stayed asleep to
   update a local estimator.
   \return the number of milliseconds we've been awake
*/
long HeavySleeper::now() {
  return wakeUpOffset + millis();
}
