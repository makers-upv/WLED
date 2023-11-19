#pragma once

#include "wled.h"
#include "Arduino.h"

#include "pitches.h"

#include <deque>

#define USERMOD_ID_BUZZER 900
#ifndef USERMOD_BUZZER_PIN
#define USERMOD_BUZZER_PIN 13
#endif


// // Jingle Bells

// int melody[] = {
//   NOTE_E5, NOTE_E5, NOTE_E5,
//   NOTE_E5, NOTE_E5, NOTE_E5,
//   NOTE_E5, NOTE_G5, NOTE_C5, NOTE_D5,
//   NOTE_E5,
//   NOTE_F5, NOTE_F5, NOTE_F5, NOTE_F5,
//   NOTE_F5, NOTE_E5, NOTE_E5, NOTE_E5, NOTE_E5,
//   NOTE_E5, NOTE_D5, NOTE_D5, NOTE_E5,
//   NOTE_D5, NOTE_G5
// };

// int tempo[]  = {
//   8, 8, 4,
//   8, 8, 4,
//   8, 8, 8, 8,
//   2,
//   8, 8, 8, 8,
//   8,  8, 8, 16, 16,
//   8, 8, 8, 8,
//   4, 4
// };

// // We wish you a merry Christmas

// int  wish_melody[] = {
//   NOTE_B3, 
//   NOTE_F4, NOTE_F4, NOTE_G4, NOTE_F4, NOTE_E4,
//   NOTE_D4, NOTE_D4, NOTE_D4,
//   NOTE_G4, NOTE_G4, NOTE_A4, NOTE_G4, NOTE_F4,
//   NOTE_E4, NOTE_E4, NOTE_E4,
//   NOTE_A4, NOTE_A4, NOTE_B4, NOTE_A4, NOTE_G4,
//   NOTE_F4, NOTE_D4, NOTE_B3, NOTE_B3,
//   NOTE_D4, NOTE_G4, NOTE_E4,
//   NOTE_F4
// };

// int  wish_tempo[] = {
//   4,
//   4, 8, 8, 8, 8,
//   4, 4, 4,
//   4, 8, 8, 8, 8,
//   4, 4, 4,
//   4, 8, 8, 8, 8,
//   4, 4, 8, 8,
//   4, 4, 4,
//   2
// };

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
    unsigned long lastTime_ = 0;
    unsigned long delay_ = 0;
    std::deque<std::pair<uint16_t, unsigned long>> sequence_ {}; // Freq, delay
  public:
    /*
     * setup() is called once at boot. WiFi is not yet connected at this point.
     * You can use it to initialize variables, sensors or similar.
     */
    void setup() {
      // Setup the pin, and default to LOW
      pinMode(USERMOD_BUZZER_PIN, OUTPUT);
      digitalWrite(USERMOD_BUZZER_PIN, LOW);

      // Set test sequence
      sequence_.push_back({ NOTE_C4, 1000 });
      sequence_.push_back({ NOTE_D4, 1000 });
      sequence_.push_back({ NOTE_E4, 1000 });
      sequence_.push_back({ NOTE_F4, 1000 });
      sequence_.push_back({ NOTE_G4, 1000 });
      sequence_.push_back({ NOTE_A4, 1000 });
      sequence_.push_back({ NOTE_B4, 1000 });
      sequence_.push_back({ NOTE_C5, 1000 });

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
      if (sequence_.size() < 1) return; // Wait until there is a sequence
      if (millis() - lastTime_ <= delay_) return; // Wait until delay has elapsed

      auto event = sequence_.front();
      sequence_.pop_front();
      
      // Change frecuency of the actual note and set delay
      analogWriteFreq(event.first);
      delay_ = event.second;

      lastTime_ = millis();
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