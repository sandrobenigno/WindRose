/**
 * @file Circadian.cpp
 * @brief This file implements the Circadian class, which is used for creating and maintaining cycles of a specified duration or frequency.
 * @author Sandro Benigno
 * @version 1.0
 * @date 2024-05-23
 */

#include "Circadian.h"

/**
 * @brief Constructor to initialize the interval and record the current time.
 * @param _interval The duration of the interval in milliseconds.
 */
Circadian::Circadian(unsigned long _interval) {
  interval = _interval;
  previousMillis = millis();
}

/**
 * @brief Checks if the interval has elapsed.
 * @return True if the interval has elapsed, false otherwise.
 */
bool Circadian::isElapsed() {
  // Get the current time in milliseconds
  unsigned long currentMillis = millis();
  // Check if the elapsed time is greater than or equal to the interval
  if (currentMillis - previousMillis >= interval) {
    // Update the previous time to the current time
    previousMillis = currentMillis;
    // Return true indicating the interval has elapsed
    return true;
  }
  // Return false indicating the interval has not elapsed
  return false;
}

/**
 * @brief Converts seconds to milliseconds.
 * @param seconds The number of seconds.
 * @return The equivalent duration in milliseconds.
 */
unsigned long Circadian::secondsToMillis(unsigned int seconds) {
  return (unsigned long)seconds * 1000;
}

/**
 * @brief Converts minutes to milliseconds.
 * @param minutes The number of minutes.
 * @return The equivalent duration in milliseconds.
 */
unsigned long Circadian::minutesToMillis(unsigned int minutes) {
  return (unsigned long)minutes * 60 * 1000;
}

/**
 * @brief Converts hours to milliseconds.
 * @param hours The number of hours.
 * @return The equivalent duration in milliseconds.
 */
unsigned long Circadian::hoursToMillis(unsigned int hours) {
  return (unsigned long)hours * 60 * 60 * 1000;
}

/**
 * @brief Converts frequency in hertz to milliseconds (period of the frequency).
 * @param hertz The frequency in hertz.
 * @return The equivalent period in milliseconds.
 */
unsigned long Circadian::hertzToMillis(float hertz) {
  // Check if the frequency is non-positive
  if (hertz <= 0.0) {
    // Return 0 for non-positive frequencies
    return 0;
  }
  // Return the period in milliseconds for the given frequency
  return (unsigned long)(1000.0 / hertz);
}
