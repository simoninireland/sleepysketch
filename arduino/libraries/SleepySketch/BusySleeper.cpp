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
#include "BusySleeper.h"

/**
   A "busy sleeper" that uses the Arduino's ordinary delay() function
   to go to sleep. delay() is simply a busy loop that locks-out anything
   else running on the Arduino apart from interrupts.

   This doesn't perform any power-saving or other
   tricks, and so should be regarded as the sleeper used for debugging.
*/


/**
   Create a new busy sleeper.
*/
BusySleeper::BusySleeper() { /* nothing */ }


/**
   Sleep for the given period.

   The Arduino is slept without power-saving.

   \param millis the amount of time to sleep for
   \return the same value, as we assume we slept correctly
*/
long BusySleeper::sleepFor( long millis ) {
  delay(millis);
  return millis;
}


/**
   Return the current time in milliseconds.
   There is no need for any corrections.
   \return the current time in milliseconds
*/
long BusySleeper::now() {
  return millis();
}
