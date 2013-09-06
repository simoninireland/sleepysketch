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
  Test program for power reduction measurements.
*/

#include <HeavySleeper.h>
#include <Actor.h>
#include <SleepySketch.h>
#include <XBee.h>

#define LED 13

char *payload = "Hello world";
XBee radio = XBee();
XBeeAddress64 coordinator = XBeeAddress64(0, 0);  /* mesh co-ordinator */
ZBTxRequest req = ZBTxRequest(coordinator, (uint8_t*) payload, sizeof(payload));
ZBTxStatusResponse res = ZBTxStatusResponse();

class BlinkyActor : public Actor {
  private:
    int blinking;
  public:
    BlinkyActor( int bl );
    void behaviour();
};

BlinkyActor::BlinkyActor( int bl ) { blinking = bl; }

void BlinkyActor::behaviour() {
  for(int i = 0; i < blinking; i++) {
    digitalWrite(LED, HIGH);
    delay(700);
    digitalWrite(LED, LOW);
    delay(300);
  }
  
  Sleepy.scheduleIn(this, 30 * 1000);
}

class ChattyActor : public Actor {
  private:
    int chatting;
  public:
    ChattyActor( int ch );
    void behaviour();
};

ChattyActor::ChattyActor( int ch ) { chatting = ch; }

void ChattyActor::behaviour() {
  int t = millis();
  while((millis() - t) < (chatting * 1000)) {
    digitalWrite(LED, HIGH);
    delay(200);
    digitalWrite(LED, LOW);

    radio.send(req);
    radio.readPacket(500); /* retrieve and discard */
    
    delay(500);
  }
  
  Sleepy.scheduleIn(this, 30 * 1000);
}

void setup() {
  pinMode(LED, OUTPUT);
  Serial.begin(9600);
  radio.setSerial(Serial);
  Sleepy.begin(new HeavySleeper());
  
  BlinkyActor *b = new BlinkyActor(10);
  Sleepy.scheduleIn(b, 10 * 1000);

  ChattyActor *c = new ChattyActor(10);
  Sleepy.scheduleIn(c, 20 * 1000); 
}

void loop() {
  Sleepy.loop();
}

