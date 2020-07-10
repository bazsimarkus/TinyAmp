# TinyAmp

**Code & Hardware Tutorial for using an LM386 amplifier with a 74HCT164 Shift Register VU Meter controlled by an ATtiny13A-PU AVR microcontroller.**

This is a simple LM386 amplifier circuit, with controllable amplification and a digital VU Meter. The circuit can be built and is working without the VU Meter part as well. The amplifier has a 3.5 mm Jack output, therefore it can be used to amplify line-level music besides electric guitar pickup signals as well, but in the first case the usage of a heat sink is strongly recommended.

*This project started out originally as a university project for my sensor-controlled electric guitar effect processor. For my circuit I needed a reliable amplifier base, so I designed this circuit as a sandbox for the bigger project.*

## Design & Planning

I divided my university project into two main parts, the purpose of this first part was to obtain a reliably working amplification circuit and create and easily processable amplified signal from an electric guitar. To implement the amplifier circuit, I used Texas Instruments’ popular LM386, an integrated amplifier circuit, that is extremely common in audio frequency applications.

Another goal was to be able to examine the amplified audio signal with the help of a microcontroller, therefore I also designed another module to the circuit with the usage of an 8-bit AVR microcontroller, namely an Attiny13A. The audio signal is connected to one of the ADC inputs of the microcontroller. The MCU processes the amplitude of the signal, and controls its output pins in such way that it operates a CD74HCT164E 8-bit shift register as a VU Meter. The VU Meter is a simple colored bar graph display that consists of 10 LEDs. (Two-two LEDs on both ends are connected to the MSB and LSB pins, so that the 10-unit bar graph display can be driven by 1 8-bit shift register).

## Photo of the final circuit board
![enter image description here](https://raw.githubusercontent.com/bazsimarkus/TinyAmp/master/TinyAmp_board.jpg)

## Schematic of the circuit

![enter image description here](https://raw.githubusercontent.com/bazsimarkus/TinyAmp/master/TinyAmp_schematic.png)

## How It Works

The microcontroller part of the amplifier circuit is powered by a linear power supply, in my case LM7805CV, which provides a stable 5V supply voltage to the microcontroller. The circuit can be turned on and off, a potentiometer voltage divider is applied to the input of the amplifier unit, so that the volume can be adjusted, while a 470uF electrolytic capacitor is added to the output to filter out the DC component. The DC component needs to be filtered out, because the device connected to the output of the amplifier usually has a low impedance (e.g. small speaker - 8 Ohm). This allows high currents to flow, which can lead to damage in the amplifier IC. The capacitor also has a discharge resistor in case the output is allowed to float (nothing is connected to it), this way sooner or later the charge will flow out from the capacitor, and the VU Meter remains at stable zero even when idling.

After an additional DC filter capacitor, I use a voltage divider formed from resistors to raise the base voltage level of the audio signal to 2.5V (VCC / 2), which is the middle of the ADC input range. Besides of that, I used some high-speed SN4148 signal diodes to protect the input of the ADC from unwanted high voltages. (This can happen when, for example, you connect an already amplified signal onto the input of the amplifier, so it over-amplifies, with this method we set the signal maximum to +/- VCC / 2 with the diodes. If there is an overvoltage, i.e. a voltage above 5V, the current flows away through the upper diode, while the lower diode plays a role at voltages below 0.)

After a current limiting resistor, the signal is connected to the ADC input of the microcontroller. The microcontroller controls the shift register and through the shift register the LEDs according to the amplitude of the input signal. I designed the circuit, has it manufactured and then soldered it to obtain the final circuit board.

## Software

I used an ATtiny13A microcontroller, because it’s really cheap, way cheaper than an ATtiny85. Even though the ATtiny13A has a relatively small flash memory (1K), the code is so simple (less than 100 lines) that it easily fits onto this cheap little microcontroller. I wrote the embedded software for the processing in the Arduino IDE, and then used an USBasp programmer to burn the code into the MCU’s memory.
