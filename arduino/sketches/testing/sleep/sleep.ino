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
  Test of the sleeping using the watchdog timer.
  The Arduino's watchdog timer provides the longest timeouts and
  the most power-saving, so is good for putting the machine into
  the deepest useful sleep state
*/

#include <avr/sleep.h>
#include <avr/power.h>
#include <avr/wdt.h>

#define LED 13

volatile int f_wdt = 1;

ISR( WDT_vect ) {
  if(f_wdt == 0)
  {
    f_wdt=1;
  }
  else
  {
    Serial.println("WDT Overrun!!!");
  }
}
 
void blink( int times, int on ) {
  for(int i = 0; i < times; i++) {
    digitalWrite(LED, HIGH);
    delay(on);
    digitalWrite(LED, LOW);
    delay(100);
  }
}


void sleep(void) {
  /* select the sleep mode */  
  set_sleep_mode(SLEEP_MODE_PWR_SAVE);  
  sleep_enable();  
 
  /* enter the sleep mode */
  sleep_mode();  
  
  /* ... time passes ... */

  /* disable sleep mode as soon as we come back */
  sleep_disable();   
  
  /* re-enable all the peripheral devices */
  power_all_enable();
}  
 
void setup() {
  Serial.begin(9600);
  pinMode(LED, OUTPUT);

  // set up the watchdog timer
  MCUSR &= ~(1 << WDRF);
  WDTCSR |= (1 << WDCE) | (1 << WDE);
  WDTCSR ~= (1 << WDP0) | (1 << WDP3); /* 8s */
  WDTCSR |= _BV(WDIE);
}

void loop() {
   blink(1,100);
   sleep();
   blink(5, 200);
   delay(5000);
 }
 
