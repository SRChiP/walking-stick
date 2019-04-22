#include <Arduino.h>
#include <NewPing.h>
#include <SoftwareSerial.h>
#include <DFPlayerMini_Fast.h>

// ----------------------------------------------- SETTINGS ------------------------------------------------------------

//Enable or disable DEBUG. Serial is fully disabled if DEBUG is false
#define DEBUG true // flag to turn on/off debugging
#define Serial if(DEBUG)Serial

// Settings for the first (lower) sensor
#define LOWER_SENSOR_COM_PIN 12
#define LOWER_SENSOR_POWER_PIN 11  // Pin to turn on power to the sensor
#define LOWER_SENSOR_MAX_DISTANCE 60 // In centimetres

#define UPPER_SENSOR_COM_PIN 10
#define UPPER_SENSOR_MAX_DISTANCE 100 // In centimetres
#define UPPER_SENSOR_POWER_PIN 9

#define PINGS_PER_CYCLE 3

#define MOTOR_PIN 6

#define AUDIO_TX 15
#define AUDIO_RX 14

#define AUDIO_VOLUME 20

// ----------------------------------------------- AUDIO ---------------------------------------------------------------
#define AUDIO_FILE_NUMBER 1
#define AUDIO_FULL_LENGTH 5825
#define AUDIO_MIN_LENGTH 752


// ----------------------------------------------GLOBAL VARIABLES-------------------------------------------------------

NewPing sonar_l(LOWER_SENSOR_COM_PIN, LOWER_SENSOR_COM_PIN, LOWER_SENSOR_MAX_DISTANCE);
NewPing sonar_u(UPPER_SENSOR_COM_PIN, UPPER_SENSOR_COM_PIN, UPPER_SENSOR_MAX_DISTANCE);

SoftwareSerial Serial2(AUDIO_RX, AUDIO_TX); // RX, TX
DFPlayerMini_Fast audio;

int dist;  //To use in the readSensor function
int distance;  // Lower sensor value to use in the main code
int distance_2;  // Lower sensor value to use in the main code
unsigned int ping_latency;
unsigned long currentMillis;
unsigned long audioStartMillis;

typedef enum {
    OFF,
    PLAYING,
} AUDIO_STATE;

AUDIO_STATE current_state = AUDIO_STATE::OFF;

// --------------------------------------------------------------------------------------------------------------------

/* If sensor is 1, it's the lower sensor. Else it's the upper one */
int readSensor(int sensor)
{
  currentMillis = millis();
  Serial.print(F("S"));
  Serial.print(sensor);
  Serial.print(F(" ("));
  Serial.print((sensor == 1) ? F("LOWER") : F("UPPER"));
  Serial.print(F(") Ping: "));
  if (sensor == 1) {
    ping_latency = sonar_l.ping_median(PINGS_PER_CYCLE);
  }
  else {
    ping_latency = sonar_u.ping_median(PINGS_PER_CYCLE);
  }
  dist = ping_latency / US_ROUNDTRIP_CM;
  Serial.print(distance);
  Serial.print(F("cm, Time Taken: "));
  Serial.print((millis() - currentMillis)/1000.0);
  Serial.println(F("s"));
  return (int) dist;
}

/* Start playing audio or continue playing the audio */
void startPlayingAudio()
{
  if (current_state == AUDIO_STATE::OFF) {
    audioStartMillis = millis();
    current_state = AUDIO_STATE::PLAYING;
    audio.play(AUDIO_FILE_NUMBER);
    Serial.println(F("Play Audio"));
  }
  else 
  {
    if (millis() >= (audioStartMillis + AUDIO_FULL_LENGTH)) {
      current_state = AUDIO_STATE::OFF;
      audio.pause();
    }
    else 
    {
      Serial.println(F("Continue Audio"));
    }
  }
}

/* Stop audio after the current partial tone finishes */
void stopAudio()
{
  if (current_state == AUDIO_STATE::PLAYING)
  {
    if (millis() >= (audioStartMillis + AUDIO_MIN_LENGTH)) {
      current_state = AUDIO_STATE::OFF;
      audio.pause();
      }
  }
}

void vibrate(int amount)
{
  if (amount < 1)
  {
    digitalWrite(MOTOR_PIN, LOW);
    Serial.println(F("VIBRATION OFF"));
  }
  else if (amount > 254)
  {
    digitalWrite(MOTOR_PIN, HIGH);
    Serial.println(F("VIBRATION ON"));
  }
  else
  {
    analogWrite(MOTOR_PIN, amount);
    Serial.println(F("VIBRATION SLOW"));
  }
}

void setup()
{
    Serial.begin(9600);  // Serial communication for debug.
    Serial.println(F("Booting stick..."));
    pinMode(LOWER_SENSOR_POWER_PIN, OUTPUT);
    digitalWrite(LOWER_SENSOR_POWER_PIN, LOW);
    Serial.println(F("Sensor 1 ready"));
    
    pinMode(UPPER_SENSOR_POWER_PIN, OUTPUT);
    digitalWrite(UPPER_SENSOR_POWER_PIN, LOW);
    Serial.println(F("Sensor 2 ready"));

    pinMode(MOTOR_PIN, OUTPUT);
    Serial.println(F("Motor ready"));

    Serial2.begin(9600);  // Serial communication for mp3 player.
    audio.begin(Serial2);
    audio.volume(20);
    Serial.println(F("Audio ready"));
}

void loop()
{
  distance = readSensor(1);
  distance_2 = readSensor(2);

  if ((distance != 0) || (distance_2 != 0))
  {
    if (distance <= 20 || distance_2 <= 30)
    {
      vibrate(255);
      startPlayingAudio();
    }
    else if (distance <= 50 || distance_2 <= 40) {
      vibrate(200);
      stopAudio();
    }
    else 
    {
      vibrate(0);
      stopAudio();
    }
  }
  else {
    stopAudio();
    vibrate(0);
  }
}
