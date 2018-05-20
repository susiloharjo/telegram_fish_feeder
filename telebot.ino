/*
Name:		 lightBot.ino
Created:	 17/01/2018
Author:	     Stefano Ledda <shurillu@tiscalinet.it>
Description: a simple example that do:
             1) parse incoming messages
			 2) if "LIGHT ON" message is received, turn on the onboard LED
			 3) if "LIGHT OFF" message is received, turn off the onboard LED
			 4) otherwise, reply to sender with a welcome message

*/
#include "CTBot.h"
#include <Servo.h>

Servo myservo;  // create servo object to control a servo
// twelve servo objects can be created on most boards

int pos = 0;    // variable to store the servo position


CTBot myBot;

String ssid = "ssid";     // REPLACE mySSID WITH YOUR WIFI SSID
String pass = "password"; // REPLACE myPassword YOUR WIFI PASSWORD, IF ANY
String token = "token";   // REPLACE myToken WITH YOUR TELEGRAM BOT TOKEN
uint8_t led = LED_BUILTIN;            // the onboard ESP8266 LED.    
                            // If you have a NodeMCU you can use the BUILTIN_LED pin
                            // (replace 2 with BUILTIN_LED)							

void setup() {
	// initialize the Serial
	Serial.begin(115200);
 myservo.attach(2);

	// connect the ESP8266 to the desired access point
	myBot.wifiConnect(ssid, pass);

	// set the telegram bot token
	myBot.setTelegramToken(token);

	// check if all things are ok
	if (myBot.testConnection())
		Serial.println("\ntestConnection OK");
	else
		Serial.println("\ntestConnection NOK");

	// set the pin connected to the LED to act as output pin
	pinMode(led, OUTPUT);
	digitalWrite(led, HIGH); // turn off the led (inverted logic!)

}

void loop() {
	// a variable to store telegram message data
	TBMessage msg;

	// if there is an incoming message...
	if (myBot.getNewMessage(msg)) {

		if (msg.text.equalsIgnoreCase("FEEDING TIME")) {              // if the received message is "LIGHT ON"...
			digitalWrite(led, LOW);                               // turn on the LED (inverted logic!)
       for (pos = 0; pos <= 180; pos += 1) { // goes from 0 degrees to 180 degrees
    // in steps of 1 degree
      myservo.write(pos);              // tell servo to go to position in variable 'pos'
      delay(20);                       // waits 15ms for the servo to reach the position
            }
          for (pos = 180; pos >= 0; pos -= 1) { // goes from 180 degrees to 0 degrees
            myservo.write(pos);              // tell servo to go to position in variable 'pos'
            delay(20);                       // waits 15ms for the servo to reach the position
          }
          
			myBot.sendMessage(msg.sender.id, "Fish ALready Feed");  // notify the sender
     Serial.println("Fish Already Feed");
		}
		
		else {                                                    // otherwise...
			// generate the message for the sender
			String reply;
			reply = (String)"Welcome " + msg.sender.username + (String)". WRITE FEEDING TIME FOR FEEDING FISH.";
			myBot.sendMessage(msg.sender.id, reply);             // and send it
		}
	}
	// wait 500 milliseconds
	delay(500);
}
