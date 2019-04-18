#include <Arduino.h>
#include <NewPing.h>
#include <SoftwareSerial.h>
#include <DFMiniMp3.h>

#include "EasingLibrary.h"

// ----------------------------------------------- SETTINGS ------------------------------------------------------------

//Enable or disable DEBUG. Serial is fully disabled if DEBUG is false
#define DEBUG true // flag to turn on/off debugging
#define Serial if(DEBUG)Serial

// Settings for the first (lower) sensor
#define LOWER_SENSOR_COM_PIN 12
#define LOWER_SENSOR_POWER_PIN 11  // Pin to turn on power to the sensor
#define LOWER_SENSOR_MAX_DISTANCE 60 // In centimetres
#define AUDIO_TX 10
#define MOTOR_PIN 6

// ------------------------------------------------ENUMS----------------------------------------------------------------
class Mp3Notify
{
public:
  static void OnError(uint16_t errorCode)
  {
    // see DfMp3_Error for code meaning
    Serial.println();
    Serial.print("Com Error ");
    Serial.println(errorCode);
  }

  static void OnPlayFinished(uint16_t globalTrack)
  {
    Serial.println();
    Serial.print("Play finished for #");
    Serial.println(globalTrack);   
  }

  static void OnCardOnline(uint16_t code)
  {
    Serial.println();
    Serial.print("Card online ");
    Serial.println(code);
  }

  static void OnUsbOnline(uint16_t code)
  {
    Serial.println();
    Serial.print("USB Disk online ");
    Serial.println(code);
  }

  static void OnCardInserted(uint16_t code)
  {
    Serial.println();
    Serial.print("Card inserted ");
    Serial.println(code);
  }

  static void OnUsbInserted(uint16_t code)
  {
    Serial.println();
    Serial.print("USB Disk inserted ");
    Serial.println(code); 
  }

  static void OnCardRemoved(uint16_t code)
  {
    Serial.println();
    Serial.print("Card removed ");
    Serial.println(code);  
  }

  static void OnUsbRemoved(uint16_t code)
  {
    Serial.println();
    Serial.print("USB Disk removed ");
    Serial.println(code);
  }
};

// ----------------------------------------------GLOBAL VARIABLES-------------------------------------------------------
NewPing sonar_l(LOWER_SENSOR_COM_PIN, LOWER_SENSOR_COM_PIN);

SoftwareSerial serial2(AUDIO_TX, 9); // RX, TX
DFMiniMp3<SoftwareSerial, Mp3Notify> audio(serial2);

unsigned int distance;

// --------------------------------------------------------------------------------------------------------------------

void setup()
{
    // delay(800);
    Serial.begin(9600);
    Serial.println(F("Booting stick..."));
    pinMode(LOWER_SENSOR_POWER_PIN, OUTPUT);
    digitalWrite(LOWER_SENSOR_POWER_PIN, LOW);
    Serial.println(F("Sensor 1 ready"));
    pinMode(MOTOR_PIN, OUTPUT);

    audio.begin();
    audio.setVolume(10);
    uint16_t count = audio.getTotalTrackCount();
    Serial.print("files ");
    Serial.println(count);
}

void loop()
{
    unsigned long currentMillis = millis();
    Serial.print("Ping: ");
    distance = sonar_l.ping_median(10) / US_ROUNDTRIP_CM;
    Serial.print(distance);
    Serial.print("cm, Time Taken: ");
    Serial.print((millis() - currentMillis)/1000.0);
    Serial.println("s");

    if (distance != 0){
      if (distance <= 20) {
        // audio.playMp3FolderTrack(1);
        digitalWrite(MOTOR_PIN, HIGH);
        Serial.println("MOTOR HIGH");
        
      }
      else if (distance <= 50) {
        analogWrite(MOTOR_PIN, 200);
        Serial.println("MOTOR HIGH");
      }
        
    } else {
      digitalWrite(MOTOR_PIN, LOW);
      Serial.println("MOTOR LOW");
    }
    // delay(100);
    // Check distance in sensor 1
    //   if it's close
    //     sound alarm depending on distance
    // check sensor 2
    //  if close
    //      sound alarm depending on distance
    
}

