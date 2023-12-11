#pragma once

#include "wled.h"
#include "Arduino.h"

#include "pitches.h"


#define USERMOD_ID_BUZZER 900
#ifndef USERMOD_BUZZER_PIN
#define USERMOD_BUZZER_PIN 13
#endif

#define SONG_SPEED_FACTOR 1.3 // 1.3 is a good value for Jingle Bells but you can play with it

extern bool MusicState ;

// // Jingle Bells

 int melody[] = {
   NOTE_E5, NOTE_E5, NOTE_E5,
   NOTE_E5, NOTE_E5, NOTE_E5,
   NOTE_E5, NOTE_G5, NOTE_C5, NOTE_D5,
   NOTE_E5,
   NOTE_F5, NOTE_F5, NOTE_F5, NOTE_F5,
   NOTE_F5, NOTE_E5, NOTE_E5, NOTE_E5, NOTE_E5,
   NOTE_E5, NOTE_D5, NOTE_D5, NOTE_E5,
   NOTE_D5, NOTE_G5
 };

 int tempo[]  = {
  8, 8, 4,
  8, 8, 4,
  8, 8, 8, 8,
  2,
  8, 8, 8, 8,
  8,  8, 8, 16, 16,
  8, 8, 8, 8,
  4, 4
 };

// // We wish you a merry Christmas

 int  wish_melody[] = {
   NOTE_B3, 
   NOTE_F4, NOTE_F4, NOTE_G4, NOTE_F4, NOTE_E4,
   NOTE_D4, NOTE_D4, NOTE_D4,
   NOTE_G4, NOTE_G4, NOTE_A4, NOTE_G4, NOTE_F4,
   NOTE_E4, NOTE_E4, NOTE_E4,
   NOTE_A4, NOTE_A4, NOTE_B4, NOTE_A4, NOTE_G4,
   NOTE_F4, NOTE_D4, NOTE_B3, NOTE_B3,
   NOTE_D4, NOTE_G4, NOTE_E4,
   NOTE_F4
 };

 int  wish_tempo[] = {
   4,
   4, 8, 8, 8, 8,
   4, 4, 4,
   4, 8, 8, 8, 8,
   4, 4, 4,
   4, 8, 8, 8, 8,
   4, 4, 8, 8,
   4, 4, 4,
   2
 };

// //  Santa Claus is coming to town

// int santa_melody[] = {
//   NOTE_G4,
//   NOTE_E4,  NOTE_F4, NOTE_G4, NOTE_G4, NOTE_G4,
//   NOTE_A4, NOTE_B4, NOTE_C5, NOTE_C5, NOTE_C5,
//   NOTE_E4, NOTE_F4, NOTE_G4, NOTE_G4, NOTE_G4,
//   NOTE_A4, NOTE_G4, NOTE_F4, NOTE_F4,
//   NOTE_E4, NOTE_G4, NOTE_C4, NOTE_E4,
//   NOTE_D4, NOTE_F4, NOTE_B3,
//   NOTE_C4
// };

// int  santa_tempo[] = {
//   8,
//   8, 8, 4, 4, 4,
//   8, 8, 4, 4, 4,
//   8, 8, 4,  4, 4,
//   8, 8, 4, 2,
//   4, 4, 4, 4,
//   4, 2, 4,
//   1
// };

// Reference: https://projecthub.arduino.cc/joshi/piezo-christmas-songs-c0e7aa


/*
 * Usermods allow you to add own functionality to WLED more easily
 * See: https://github.com/Aircoookie/WLED/wiki/Add-own-functionality
 * 
 * Using a usermod:
 * 1. Copy the usermod into the sketch folder (same folder as wled00.ino)
 * 2. Register the usermod by adding #include "usermod_filename.h" in the top and registerUsermod(new MyUsermodClass()) in the bottom of usermods_list.cpp
 */


class ChristmasSongUsermod : public Usermod {
  private:
  // Private class members. You can declare variables and functions only accessible to your usermod here
    bool enabled = false;
    bool initDone = false;
    
    unsigned long lastTime_ = 0;
    unsigned long delay_ = 0;
    unsigned long delay_2 = 0;
    unsigned int index = 0;

    // string that are used multiple time (this will save some flash memory)
    static const char _name[];
    static const char _enabled[];

  public:

   // non WLED related methods, may be used for data exchange between usermods (non-inline methods should be defined out of class)

    /**
     * Enable/Disable the usermod
     */
    inline void enable(bool enable) { enabled = enable; }

    /**
     * Get usermod enabled/disabled state
     */
    inline bool isEnabled() { return enabled; }


    /*
     * setup() is called once at boot. WiFi is not yet connected at this point.
     * You can use it to initialize variables, sensors or similar.
     */
    void setup() {
      // Setup the pin, and default to LOW
      pinMode(USERMOD_BUZZER_PIN, OUTPUT);
      digitalWrite(USERMOD_BUZZER_PIN, LOW);
    }


    /*
     * connected() is called every time the WiFi is (re)connected
     * Use it to initialize network interfaces
     */
    void connected() {
      // We do nothing here
    }

    /*
     * loop() is called continuously. Here you can check for events, read sensors, etc.
     */
    void loop() {
      // If the music is activated, play the song
      if(MusicState)
      {
        // Wait for the note to finish playing before turning off the PWM signal
        if (millis() - lastTime_ <= delay_) return; // Wait until delay has elapsed
        analogWrite(USERMOD_BUZZER_PIN, 0);
        if(millis() - lastTime_ <= delay_2) return; // Wait until delay_2 has elapsed

        // Set pitch frequency to the PWM signal and turn on at 10% duty cycle
        analogWriteFreq(melody[index]);
        analogWrite(USERMOD_BUZZER_PIN, 10);

        // Calculate note duration and delay between notes
        int noteDuration = 2000 / tempo[index];
        delay_ = noteDuration;
        delay_2 = SONG_SPEED_FACTOR * noteDuration;

        // Advance to next note and reset index if we reach the end of the song
        index++;
        if(index > sizeof(melody)/sizeof(melody[0])) index = 0;

        // Update lastTime_
        lastTime_ = millis();
    }else
    {
      // Turn off the PWM signal
      analogWrite(USERMOD_BUZZER_PIN, 0);

      // Reset index
      index = 0;
    }
    }
        /*
     * addToJsonInfo() can be used to add custom entries to the /json/info part of the JSON API.
     * Creating an "u" object allows you to add custom key/value pairs to the Info section of the WLED web UI.
     * Below it is shown how this could be used for e.g. a light sensor
     */
    void addToJsonInfo(JsonObject& root)
    {
      // if "u" object does not exist yet wee need to create it
      JsonObject user = root["u"];
      if (user.isNull()) user = root.createNestedObject("u");

      //this code adds "u":{"ExampleUsermod":[20," lux"]} to the info object
      //int reading = 20;
      //JsonArray lightArr = user.createNestedArray(FPSTR(_name))); //name
      //lightArr.add(reading); //value
      //lightArr.add(F(" lux")); //unit

      // if you are implementing a sensor usermod, you may publish sensor data
      //JsonObject sensor = root[F("sensor")];
      //if (sensor.isNull()) sensor = root.createNestedObject(F("sensor"));
      //temp = sensor.createNestedArray(F("light"));
      //temp.add(reading);
      //temp.add(F("lux"));
    }


    /*
     * addToJsonState() can be used to add custom entries to the /json/state part of the JSON API (state object).
     * Values in the state object may be modified by connected clients
     */
    void addToJsonState(JsonObject& root)
    {
      if (!initDone || !enabled) return;  // prevent crash on boot applyPreset()

      JsonObject usermod = root[FPSTR(_name)];
      if (usermod.isNull()) usermod = root.createNestedObject(FPSTR(_name));

      //usermod["user0"] = userVar0;
    }


    /*
     * getId() allows you to optionally give your V2 usermod an unique ID (please define it in const.h!).
     * This could be used in the future for the system to determine whether your usermod is installed.
     */
    uint16_t getId()
    {
      return USERMOD_ID_BUZZER;
    }
};

// add more strings here to reduce flash memory usage
const char ChristmasSongUsermod::_name[]    PROGMEM = "Christmas";
const char ChristmasSongUsermod::_enabled[] PROGMEM = "enabled";


// implementation of non-inline member methods