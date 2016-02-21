//Demonstrates use of hardware or software SPI on an ATtiny2313/4313
//to make an 8-LED chaser with a 74HC595 shift register.
//Written using Arduino 1.5.8 with the Arduino-Tiny ATtiny
//core, http://code.google.com/p/arduino-tiny/
//
//Connections:
//  ATtiny4313 pin 9  (DIP pin 12, PB0) to 74HC595 pin 12 (storage register clock)
//  ATtiny4313 pin 10 (DIP pin 13, PB1) to 74HC595 pin 14 (data in)
//  ATtiny4313 pin 11 (DIP pin 14, PB2) to 74HC595 pin 11 (shift register clock)
//
//Connect eight LEDs from the shift register's outputs (Q0-Q7) to ground, through
//appropriate current-limiting resistors.
//
//Jack Christensen 24Oct2013
//
//CC BY-SA-NC:
//This work is licensed under the Creative Commons Attribution-
//ShareAlike NotCommercial 4.0 Unported License. To view a copy of this license,
//visit http://creativecommons.org/licenses/by-sa-nc/4.0/ or send a
//letter to Creative Commons, 171 Second Street, Suite 300,
//San Francisco, California, 94105, USA.

#include <tinySPI.h>               //http://github.com/JChristensen/tinySPI

#define HARDWARE_SPI 1             //set to 1 to use hardware SPI, set to 0 to use shiftOut (software SPI)

//pin definitions
const int LATCH_PIN = 9;           //storage register clock (slave select)
const int DATA_PIN = 10;            //data in
const int CLOCK_PIN = 11;           //shift register clock

void setup(void)
{
    //pullups on for unused pins to minimize power consumption
    pinMode(3, INPUT_PULLUP);
    pinMode(4, INPUT_PULLUP);
    
    #if HARDWARE_SPI == 1
    SPI.begin();                   //start hardware SPI (the library sets up the clock and data pins)
    #else
    pinMode(CLOCK_PIN, OUTPUT);    //set up the pins for software SPI
    pinMode(DATA_PIN, OUTPUT);
    #endif
    pinMode(LATCH_PIN, OUTPUT);    //latch pin needs to be set up for hardware or software
    digitalWrite(LATCH_PIN, HIGH);
}

void loop(void)
{
    for (int b=0; b<8; b++) {
        uint8_t myByte = 1 << b;
        digitalWrite(LATCH_PIN, LOW);
        
        //write the same byte 1000 times so the difference can be seen
        //  between hardware and software SPI.
        for(int n=0; n<1000; n++) {
            #if HARDWARE_SPI == 1
            SPI.transfer(myByte);
            #else
            shiftOut(DATA_PIN, CLOCK_PIN, MSBFIRST, myByte);
            #endif
        }
        digitalWrite(LATCH_PIN, HIGH);
    }
}

