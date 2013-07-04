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

#define LED 13


void setup() {
  pinMode(LED, OUTPUT);
  Serial.begin(9600);
}


void loop() {
  for(int i = 5; i > 0; i--) {
    digitalWrite(LED, HIGH);
    delay(200);
    Serial.print("Tick ");   Serial.println(i);
    digitalWrite(LED, LOW);
    delay(i * 1000);
  }
}


