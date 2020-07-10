//**************************************************************//
//  Name    : TinyAmp                                           //
//  Author  : Balazs Markus                                     //
//  Version : 1.0                                               //
//  Notes   : Code for using an LM386 amplifier with a          //
//          : 74HCT164 Shift Register controlled by an          //
//          : ATtiny13A-PU AVR microcontroller.                 //
//          : This code easily fits into 1K of flash memory.    //
//**************************************************************//

//Pin connected to CP of 74HCT164
int clockPin = 2;
////Pin connected to DS1 and DS2 of 74HCT164
int dataPin = 1;

void setup()
{
    //No need to setup, the shiftOut function does everything
}

void loop()
{
    int sensorValue = analogRead(A2);
    int volume = abs((sensorValue - 512) * 5);
    int displayVolume = volume / 32; // divide by 32 - don't forget the 1000uF capacitor in series with the output!

    writeVolumeToDisplay(displayVolume);

    delay(10);
}

void writeVolumeToDisplay(int n)
{
    //Defines a local variable
    byte outputValue = 0;

    //We get an integer in the input parameter (n), which gives the number of LED-s we'd like to light up.
    //So the number given in the parameter is the number of the top LED, all the LEDs below the top LED should be also turned on.
    //So we write a binary one value in the output value n times, and we also right shift it, so after the loop we'll have n ones in the output value, starting from the LSB.
    //For example, if n=4, we will have outputValue == B00001111
    for (int j = 0; j < n; j++) {
        outputValue = (outputValue << 1) | 1;
    }

    //We shift out the ready outputValue
    shiftOut(dataPin, clockPin, outputValue);
}

//shiftOut function
//Taken from Carlyn Maw, Tom Igoe-s "shiftOutCode, Dual One By One" example on arduino.cc
//This function puts the given byte to the output of the shift register bit-by-bit
void shiftOut(int myDataPin, int myClockPin, byte myDataOut)
{
    // This shifts 8 bits out MSB first,
    //on the rising edge of the clock,
    //clock idles low

    //internal function setup
    int i = 0;
    int pinState;
    pinMode(myClockPin, OUTPUT);
    pinMode(myDataPin, OUTPUT);

    //clear everything out just in case to
    //prepare shift register for bit shifting
    digitalWrite(myDataPin, 0);
    digitalWrite(myClockPin, 0);

    //for each bit in the byte myDataOut
    //NOTICE THAT WE ARE COUNTING DOWN in our for loop
    //This means that %00000001 or "1" will go through such
    //that it will be pin Q0 that lights.
    for (i = 0; i < 8; i++) {
        digitalWrite(myClockPin, 0);

        //if the value passed to myDataOut and a bitmask result
        // true then... so if we are at i=6 and our value is
        // %11010100 it would the code compares it to %01000000
        // and proceeds to set pinState to 1.
        if (myDataOut & (1 << i)) {
            pinState = 1;
        }
        else {
            pinState = 0;
        }

        //Sets the pin to HIGH or LOW depending on pinState
        digitalWrite(myDataPin, pinState);
        //register shifts bits on upstroke of clock pin
        digitalWrite(myClockPin, 1);
        //zero the data pin after shift to prevent bleed through
        digitalWrite(myDataPin, 0);
    }

    //stop shifting
    digitalWrite(myClockPin, 0);
}
