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
  Test the sleeping capabilities of SleepySketch. 
*/

#include <SleepySketch.h>
#include <BusySleeper.h>
#include <Actor.h>

#define LED 13
 
void blink( int times, int on ) {
  for(int i = 0; i < times; i++) {
    digitalWrite(LED, HIGH);
    delay(on);
    digitalWrite(LED, LOW);
    delay(100);
  }
}

class PingActor : public Actor {
  protected:
    void behaviour();
    
  public:
    PingActor();
};

PingActor::PingActor() { }

void PingActor::behaviour() {
  Serial.println("Ping!");
  blink(5,100);
}

void setup() {
  delay(500);
  Serial.begin(9600);
  pinMode(LED, OUTPUT);
  Sleepy.begin(new BusySleeper());

  delay(500);
  Serial.println("Begin");
  Sleepy.scheduleEvery(new PingActor(), Sleepy.expandTime(5));
  Sleepy.scheduleEvery(new PingActor(), Sleepy.expandTime(10));
  Serial.println("Pause");
  delay(500);
}

void loop() {
  Sleepy.loop();
}

