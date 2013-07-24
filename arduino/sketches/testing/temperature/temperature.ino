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
  Test of Maxim DS18B20 temperature sensors.
  We take readings from three temperature sensors connected to a single
  OneWire bus.
  
  This sketch makes use of the OneWire  and Dallas Temperature Control
  libraries, and is heavily influenced by their demo code. See
    http://www.pjrc.com/teensy/td_libs_OneWire.html
  and
    http://milesburton.com/Main_Page?title=Dallas_Temperature_Control_Library
*/

#include <OneWire.h>
#include <DallasTemperature.h>

OneWire onewire(8);                  // OneWire bus on pin 8
DallasTemperature sensors(&onewire); // temperature sensor handler

void setup(void) {
  Serial.begin(9600);
  sensors.begin();
}


void loop(void) {
  sensors.requestTemperatures();
  for(int i = 0; i < 3; i++) {
    float c = sensors.getTempCByIndex(i);
    Serial.print("Sensor ");   Serial.print(i);   Serial.print(" = ");
    Serial.print(c);   Serial.println("C");
  }
  
  delay(5000);
}
