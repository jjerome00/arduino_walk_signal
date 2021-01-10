#include <Arduino.h>
#include "Light.h"

// unsigned long walk_time = 30000;
// unsigned long blink_time = 15000;
// unsigned long dont_time = 30000;

unsigned long walk_time = 4000;
unsigned long blink_time = 4000;
unsigned long dont_time = 4000;

state_enum currentState = WALK;

Light walk(11, walk_time, false, WALK, BLINK);
Light blink(12, blink_time, true, BLINK, DONTWALK);
Light dontWalk(12, dont_time, false, DONTWALK, WALK);

void setup() {
    Serial.begin(9600);

    // Start with walk ON (initial walk _will_ be longer)
    currentState = WALK;
    walk.setPinStateManually(LOW);
}

void loop() {
    switch(currentState) {
        case WALK:
            currentState = walk.update();
            break;
        case BLINK:
            currentState = blink.update();
            
            // switch to next state to work-around slow state change
            if (currentState != BLINK) {
                dontWalk.setPinStateManually(LOW); 
            }
            break;
        case DONTWALK:
            currentState = dontWalk.update();

            // switch to next state to work-around slow state change
            if (currentState != DONTWALK) {
                walk.setPinStateManually(LOW);
            }
            break;
    }
}