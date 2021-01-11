#include "Light.h"

/**
 * Light.cpp
 * Ecapsulate a Light that would be used in a walk signal
 * i.e. walk (solid burn), blink (blinking "don't walk"), don't walk (solid burn)
 * */

/**
 * Constructor
 * --------------------------------
 * pinToUse: The arduino pin that controls the light 
 * lightTime: The total length of time for the light's state
 * thisState: The state that identifies this instance (i.e. Walk, Blink, Don't walk )
 * stateWhenFinished: When time elapses, which state to indicate is next
 * */
Light::Light(int pinToUse, unsigned long lightTime, bool blinkLight, state_enum thisState, state_enum stateWhenFinished) {
    pin = pinToUse;
    pinMode(pin, OUTPUT);

    totalLightTime = lightTime;
    blink = blinkLight; 
    state = thisState;
    nextState = stateWhenFinished;

    pinState = HIGH;
    previousMillis = 0;
    blinkMillis = 0;
    digitalWrite(pin, pinState);
}

String Light::pinStateName(int pinState) {
    return (pinState == LOW) ? "ON" : "OFF";
}

/**
 * Set pin state manually - WITHOUT keeping track of the official state in object
 * */
void Light::setPinStateManually(int newPinState) {
    String stateName = (String)stringFromState(state);
    logMessage("setting pin manually for " + stateName + ": " + pinStateName(newPinState));
    digitalWrite(pin, newPinState);
}

/**
 * Update the state - handle blinking or steady burn (non-blinking)
 * remember the arduino loop will call this repeatively
 * */
state_enum Light::update() {
    unsigned long currentMillis = millis();

    logState("State: " + (String)stringFromState(state));

    if (blink) {
        return updateBlinkState(currentMillis);
    } else {
        return updateSolidState(currentMillis);
    }
}

/**
 * Avoids repeating the same message on serial, 
 * since we're calling these things in a loop
 * */
void Light::logMessage(String msg) {
    if (msg != message) {
        message = msg;
        Serial.println(message);
    }
}

/**
 * Log State changes - keep separate from other messages
 * */
void Light::logState(String msg) {
    if (msg != stateMessage) {
        stateMessage = msg;
        Serial.println(stateMessage);
    }
}

/**
 * Update the state for blinking
 * Blinking requires 2 timers that are compared against currentMillis:
 * 1. Total time - the entire time it should be blinking (previousMillis)
 * 2. Blink time - the on/off cycle of blinking (blinkMillis)
 * */
state_enum Light::updateBlinkState(unsigned long currentMillis) {
    
    // total blink time requires countdown, so set previousMillis to current
    if (previousMillis == 0) {
        previousMillis = currentMillis;
    }

    if (currentMillis - previousMillis <= totalLightTime) {
        logMessage("blinking state on");
        updateBlinking(currentMillis);
        return state;
    } else {
        // turn off - return next state
        logMessage("blinking state off; change state");
        pinState = HIGH;
        previousMillis = 0;
        digitalWrite(pin, pinState);
        return nextState;
    }
}


/**
 * update solely for blinking (on/off at a set interval)
 * This will never update the state; it does not return anything
 * **/
void Light::updateBlinking(unsigned long currentMillis) {
    if (currentMillis - blinkMillis >= blinkInterval) {
        if (pinState == HIGH) {
            pinState = LOW;
        } else {
            pinState = HIGH;
        }
        blinkMillis = currentMillis;
        digitalWrite(pin, pinState);
    }
}

/**
 * Update the state for solid (burn) state (i.e. no blinking)
 * If within the total time span, turn the light on and return the current state
 * If time has elapsed, turn light off and return the next state
 * */
state_enum Light::updateSolidState(unsigned long currentMillis) {
    if (pinState == HIGH && currentMillis - previousMillis >= totalLightTime) {
        logMessage("solid state on");
        pinState = LOW;
        previousMillis = currentMillis;
        digitalWrite(pin, pinState);
        return state; 
    }
    else if (pinState == LOW && currentMillis - previousMillis >= totalLightTime) {
        logMessage("solid state off; change state");
        pinState = HIGH;
        previousMillis = currentMillis;
        digitalWrite(pin, pinState);
        return nextState;
    }
    // otherwise return the current state
    return state;
}
