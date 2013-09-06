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

#ifndef SLEEPER_H
#define SLEEPER_H

/**
   \brief Abstract class of objects that manage sleep modes.

   A sleeper keeps the system asleep for a given number of milliseconds,
   and is used by a sleepy sketch to put the system into a power-down
   mode. Sub-classes adopt different strategies for sleeping, from
   "busy" sleeping (a tight loop) to "deep" sleeping (power-down using
   a low-power mode), trading-off power saving against access to
   exterbal hardware.
*/
class Sleeper {
 protected:
  Sleeper();

 public:
  /**
     Sleep for a given period.
     The sleeper uses whatever mechanism the sub-class deploys
     to sleep for the given period. The method may return a different
     number of milliseconds actually slept for (or an estimate), to
     allow for corrections.
     \param millis the number of milliseconds to sleep for
     \return the number of milliseconds actually slept for
  */
  virtual long sleepFor( long millis ) = 0;

  /**
     Return the time since start-up, in milliseconds.
     This is like the usual `millis()` function, except that it
     has the option to take account of time spent in a deep sleep
     mode where `millis()` stops counting. The value returned
     may be an estimate, and shouldn't be relied upon too much.
     \return the time in milliseconds since start-up
  */
  virtual long now() = 0;
};

#endif

