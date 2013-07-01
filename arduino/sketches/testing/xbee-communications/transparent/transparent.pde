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
  The pin driving the LED.
  */
#define LED 13


/**
  Set up the device.
  */
void setup() {
  pinMode(LED, OUTPUT);
  Serial.begin(9600);
}


/**
  Main program loop.
  Send a tick to the serial port, counting down in time just to make
  things a bit more interesting.
  */
void loop() {
  for(int i = 5; i > 0; i--) {
    digitalWrite(LED, HIGH);
    delay(200);
    Serial.print("Tick ");   Serial.println(i);
    digitalWrite(LED, LOW);
    delay(i * 1000);
  }
}


