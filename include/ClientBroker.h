/*
 * Author: Arnold Nunez
 * Project: SensorProcessor
 * Module: Communication
 */

#ifndef CLIENT_BROKER_H
#define CLIENT_BROKER_H

#include <queue>

#include "Sensor.pb.h"

namespace CoreMessaging {

// Forward declares
class EventBus;

/**
 * Client broker handles the messaging between
 * the clients and the core application.
 */
class ClientBroker {
 public:
  /**
   * Constructor
   */
  ClientBroker();

  /**
   * Destructor
   */
  ~ClientBroker();

  void testClientPolling(EventBus* eventBus);

  void testPublishSensor(const Communication::Sensors::Sensor& sensor);

  void testPublishSensorValue(
      const Communication::Sensors::SensorValueScalar& value);

 private:
  /// @brief Contains the tasks required to be executed by this broker.
  //   std::queue<std::function<void()>> mClientMessagingTasks;
};

}  // namespace CoreMessaging

#endif