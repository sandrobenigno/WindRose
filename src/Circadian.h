/**
 * @file Circadian.h
 * @brief Defines the Circadian class for creating and maintaining cycles with specified durations or frequencies.
 * The term "circadian" is used metaphorically to describe the regular, repeating cycles that the automata experience, analogous to a 24-hour rhythm but not necessarily tied to a real-world day.
 * @author Sandro Benigno
 * @version 1.0
 * @date 2024-05-23
 */

#ifndef Circadian_h
#define Circadian_h

#include "arduino.h"

/**
 * @class Circadian
 * @brief A class to handle periodic time intervals.
 */
class Circadian {
private:
  /**
   * @brief Stores the last recorded time in milliseconds.
   */
  unsigned long previousMillis;

  /**
   * @brief Stores the interval duration in milliseconds.
   */
  unsigned long interval;

public:
  /**
   * @brief Constructor to initialize the interval and record the current time.
   * @param _interval The duration of the interval in milliseconds.
   */
  Circadian(unsigned long _interval);

  /**
   * @brief Checks if the interval has elapsed.
   * @return True if the interval has elapsed, false otherwise.
   */
  bool isElapsed();

  /**
   * @brief Converts seconds to milliseconds.
   * @param seconds The number of seconds.
   * @return The equivalent duration in milliseconds.
   */
  static unsigned long secondsToMillis(unsigned int seconds);

  /**
   * @brief Converts minutes to milliseconds.
   * @param minutes The number of minutes.
   * @return The equivalent duration in milliseconds.
   */
  static unsigned long minutesToMillis(unsigned int minutes);

  /**
   * @brief Converts hours to milliseconds.
   * @param hours The number of hours.
   * @return The equivalent duration in milliseconds.
   */
  static unsigned long hoursToMillis(unsigned int hours);

  /**
   * @brief Converts frequency in hertz to milliseconds (period of the frequency).
   * @param hertz The frequency in hertz.
   * @return The equivalent period in milliseconds.
   */
  static unsigned long hertzToMillis(float hertz);
};

#endif // Circadian_h
