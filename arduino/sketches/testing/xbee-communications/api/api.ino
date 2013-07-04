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
  Test of XBee API mode communications.
  We send a sequence of numbers to the network wo-ordinator, assuming
  that these will be picked up by the base station (and possibly displayed).

  This test makes use of the xbee-arduino library.
*/

#include <XBee.h>

#define LED 13

char *payload = "Hello world";

XBee radio = XBee();
XBeeAddress64 coordinator = XBeeAddress64(0, 0);  /* reserved address of mesh co-ordinator */
ZBTxRequest req = ZBTxRequest(coordinator, (uint8_t*) payload, sizeof(payload));
ZBTxStatusResponse res = ZBTxStatusResponse();


void blink( int n, int d ) {
  for(int i = 0; i < n; i++) {
    digitalWrite(LED, HIGH);
    delay(d);
    digitalWrite(LED, LOW);
    delay(100);
  }
}


void setup() {
  pinMode(LED, OUTPUT);
  Serial.begin(9600);
  radio.setSerial(Serial);
}


void loop() {
  radio.send(req);
  blink(1, 300);
  
  if(radio.readPacket(500)) {
    if(radio.getResponse().getApiId() == ZB_TX_STATUS_RESPONSE) {
      radio.getResponse().getZBTxStatusResponse(res);
      if(res.getDeliveryStatus() == SUCCESS) {
        blink(5, 100);
      } else {
        blink(3, 300);
      }
    } else {
       blink(5, 300);
    }
  } else {
    blink(7, 300);
  }

  delay(5000);
}


