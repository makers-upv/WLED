#pragma once

#include "wled.h"
#include "Arduino.h"

#include "pitches.h"


#include <deque>


#define USERMOD_ID_BUZZER 900
#ifndef USERMOD_BUZZER_PIN
#define USERMOD_BUZZER_PIN 13
#endif

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
    std::deque<std::pair<uint16_t, unsigned long>> sequence_ {}; // Freq, delay

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

      // Set test sequence
      /*sequence_.push_back({ NOTE_C4, 1000 });
      sequence_.push_back({ NOTE_D4, 1000 });
      sequence_.push_back({ NOTE_E4, 1000 });
      sequence_.push_back({ NOTE_F4, 1000 });
      sequence_.push_back({ NOTE_G4, 1000 });
      sequence_.push_back({ NOTE_A4, 1000 });
      sequence_.push_back({ NOTE_B4, 1000 });
      sequence_.push_back({ NOTE_C5, 1000 });
      */
      sequence_.push_back({ NOTE_E5, 8 });
      sequence_.push_back({ NOTE_E5, 8 });
      sequence_.push_back({ NOTE_E5, 4 });
      sequence_.push_back({ NOTE_E5, 8 });
      sequence_.push_back({ NOTE_E5, 8 });
      sequence_.push_back({ NOTE_E5, 4 });
      sequence_.push_back({ NOTE_E5, 8});
      sequence_.push_back({ NOTE_G5, 8 });
      sequence_.push_back({ NOTE_C5, 8 });
      sequence_.push_back({ NOTE_D5, 8 });
      sequence_.push_back({ NOTE_E5, 2 });

      
      // Start PWM
      analogWrite(USERMOD_BUZZER_PIN, 50);
    }


    /*
     * connected() is called every time the WiFi is (re)connected
     * Use it to initialize network interfaces
     */
    void connected() {

    }
    void buzz(int targetPin, uint16_t frequency, long length) {
      sequence_.push_back({ frequency, length });
    }

    /*
     * loop() is called continuously. Here you can check for events, read sensors, etc.
     */
    void loop() {
      if(MusicState == 1){
      if (sequence_.size() < 1) return; // Wait until there is a sequence
      if (millis() - lastTime_ <= delay_) return; // Wait until delay has elapsed
        analogWrite(USERMOD_BUZZER_PIN, 0);
        if(millis() - lastTime_ <= delay_2) return;
        
        
      //auto event = sequence_.front();
      //sequence_.pop_front();
      
      // Change frecuency of the actual note and set delay
      /*analogWriteFreq(event.first);
      analogWrite(USERMOD_BUZZER_PIN, 50);
      int noteDuration = 1000 / event.second;
      delay_ = noteDuration;
      delay_2 = 1.3* noteDuration;

      lastTime_ = millis();
      */
      analogWriteFreq(melody[index]);
      analogWrite(USERMOD_BUZZER_PIN, 10);
      int noteDuration = 2000 / tempo[index];
      delay_ = noteDuration;
      delay_2 = 1.3* noteDuration;
      index++;
      if(index > sizeof(melody)/sizeof(melody[0])){
        index = 0;
      }
      lastTime_ = millis();
    }else{
        analogWrite(USERMOD_BUZZER_PIN, 0);
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