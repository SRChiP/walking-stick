#include <Arduino.h>
#include "EasingLibrary.h"


// --------------------------- SETTINGS ---------------------------------------------------------------------

//Enable or disable DEBUG. Serial is fully disabled if DEBUG is false
#define DEBUG true // flag to turn on/off debugging
#define Serial if(DEBUG)Serial

// ------------------------------------------------ENUMS----------------------------------------------------------------
typedef enum {
    OFF,
    TURNING_ON,
    RUNNING,
    TURNING_OFF,
} STATE;

// ----------------------------------------------GLOBAL VARIABLES-------------------------------------------------------


// --------------------------------------------------------------------------------------------------------------------

void setup()
{
    // delay(800);
    Serial.begin(9600);
    Serial.println(F("Booting stick..."));
    
}

void loop()
{
    unsigned long currentMillis = millis();
    
}
