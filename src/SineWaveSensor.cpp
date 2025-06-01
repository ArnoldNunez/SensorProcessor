/*
 * Author: Arnold Nunez
 * Project: SensorProcessor
 * Module: IOService
 */

#include "SineWaveSensor.h"

namespace SimSensors {

SineWaveSensor::SineWaveSensor(float amplitude, unsigned int period)
    : mAmplitude(amplitude), mPeriod(period), mPhaseShift(0) {}

SineWaveSensor::~SineWaveSensor() {}

}  // namespace SimSensors
