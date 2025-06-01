/*
 * Author: Arnold Nunez
 * Project: SensorProcessor
 * Module: IOLayer
 */

#ifndef SINE_WAVE_SENSOR_H
#define SINE_WAVE_SENSOR_H

#include <cmath>

namespace SimSensors {
/**
 * Simulated sinewave sensor
 */
class SineWaveSensor {
 public:
  /**
   * Constructor
   * \param amplitude   The amplitude of the sine wave.
   * \param period      The period of the sine wave (ms).
   */
  SineWaveSensor(float amplitude, unsigned int period);

  /**
   * Destructor
   */
  ~SineWaveSensor();

  /**
   * Get a sample of the sine wave at the given time.
   * Uses the time based formulate for a sine wave
   * value = A * sin(wt + phi)
   * A = amplitude
   * w = angular frequency (2 * pi * frequency (Hz) )
   * phi = Phase shift (radians)
   */
  float getSample(long long time) const {
    return mAmplitude *
           sin(2.0 * M_PI * (1.0 / (mPeriod * 1000.0)) + mPhaseShift);
  }

 private:
  float mAmplitude;
  unsigned int mPeriod;
  float mPhaseShift;
};
}  // namespace SimSensors

#endif