# arduino_walk_signal

A representation of a walk signal for Arudino
pattern: walk, (blink) dont walk, (sold) dont walk, (repeat)

## Hardware

The hardware setup is fairly simple.
* Arudino (Uno, Nano)
* 2-channel Relay (i.e. 2 relays on the same board)
* A  decommisioned walk signal with individual leads for the light bulbs

Arduino pins:
* 1 digital pin for relay controlling the light bulb for walk
* 1 digital pin for relay controlling the light bulb for dont walk
* 2-channel Relay requires 5v + ground

## Tools

* visual studio code
* processingio

## Code

* I broke down the lights into 3 states:   
(solid) walk, (blinking) dont walk, (solid) dont walk
* blinking/solid dont walk share the same arduino pin
* rely on an enum for each state

Main()
* main program
* setup a Light() instance for each of the 3 states
* keep track of the current state (via enum)
* call Light::update() in loop - each object will handle controlling the light 
    * when it's finished it will pass back either it's state or the next state

Light()
* Class representation of a light state
* Pass all options into the constructor
* Option for the light to be for a solid burn or blinking
* Uses `millis()` to determine it's current settings so as to not block the thread
