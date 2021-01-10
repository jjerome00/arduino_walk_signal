#ifndef WALK_SIGN_LIGHT_H
#define WALK_SIGN_LIGHT_H

#include <Arduino.h>

enum state_enum {WALK, BLINK, DONTWALK};

// convience function to identify an enum
static inline const char *stringFromState(enum state_enum f) {
    static const char *strings[] = { "walk", "blink", "dontwalk" };
    return strings[f];
}

/**
 * Light.cpp
 * Ecapsulate a Light that would be used in a walk signal
 * examples: walk (solid burn), blink (blinking "don't walk"), don't walk (solid burn)
 * 
 * See Cpp file for more information
 * */
class Light {
    public:
        Light(int pinToUse, unsigned long lightTime, bool blinkLight, state_enum thisState, state_enum stateWhenFinished);
        
        state_enum update();
        void setPinStateManually(int newPinState);

    private:
        int pin;
        int pinState;
        bool blink = false;
        unsigned long totalLightTime = 4000;
        
        state_enum state;
        state_enum nextState;

        unsigned long blinkInterval = 1000;
        unsigned long previousMillis = 0;
        unsigned long blinkMillis = 0;

        String message = "";
        String stateMessage = "";

        void updateBlinking(unsigned long currentMillis);

        state_enum updateSolidState(unsigned long currentMillis);
        state_enum updateBlinkState(unsigned long currentMillis);

        void logMessage(String msg);
        void logState(String msg);
        String pinStateName(int pinState);        
};

#endif