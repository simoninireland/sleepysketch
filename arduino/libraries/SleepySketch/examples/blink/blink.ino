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

/**
  The ever-fascinating blinking LED with lots of sleeping.
  
  The 
*/

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

