#include <Arduino.h>
#include <NewPing.h>
#include <DFMiniMp3.h>

#include "EasingLibrary.h"


// ----------------------------------------------- SETTINGS ------------------------------------------------------------

//Enable or disable DEBUG. Serial is fully disabled if DEBUG is false
#define DEBUG true // flag to turn on/off debugging
#define Serial if(DEBUG)Serial

// Settings for the first (lower) sensor
#define LOWER_SENSOR_TRIGGER_PIN 12
#define LOWER_SENSOR_ECHO_PIN 11
#define LOWER_SENSOR_MAX_DISTANCE 60 // In centimetres

// ------------------------------------------------ENUMS----------------------------------------------------------------
typedef enum {
    OFF,
    TURNING_ON,
    RUNNING,
    TURNING_OFF,
} STATE;

// ----------------------------------------------GLOBAL VARIABLES-------------------------------------------------------
NewPing sonar_l(LOWER_SENSOR_TRIGGER_PIN, LOWER_SENSOR_TRIGGER_PIN);

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
    // delay(50);
    Serial.print("Ping: ");
    // Serial.print(sonar_l.ping_cm());
    Serial.print(sonar_l.ping_median(10) / US_ROUNDTRIP_CM);
    Serial.print("cm, Time Taken: ");
    Serial.print((millis() - currentMillis)/1000.0);
    Serial.println("s");
    
    // Check distance in sensor 1
    //   if it's close
    //     sound alarm depending on distance
    // check sensor 2
    //  if close
    //      sound alarm depending on distance
    
}

