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

#include "LightSleeper.h"

/**
   A "light sleeper" that uses the Arduino's ordinary sleep() function
   to go to sleep. This doesn't perform any power-saving or other
   tricks, and so should be regarded as the sleeper used for debugging.
*/


/**
   Create a new light sleeper.
*/
LightSleeper::LightSleeper() {
}


/**
   Sleep for the given period.

   The Arduino is slept without power-saving.

   \param millis the ampunt of time to sleep for
   \return 0
*/
long LightSleeper::sleepFor( long millis ) {
  sleep(millis);
}

