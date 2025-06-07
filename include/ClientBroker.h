/*
 * Author: Arnold Nunez
 * Project: SensorProcessor
 * Module: Communication
 */

#ifndef CLIENT_BROKER_H
#define CLIENT_BROKER_H

#include <queue>

#include "AppConfig.h"
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

  void testClientPush();
  void testClientPolling(EventBus* eventBus);
  void testClientPollingPull(EventBus* eventBus);

  void testPublishSensor(const Communication::Sensors::Sensor& sensor);

  void testPublishSensorValue(
      const Communication::Sensors::SensorValueScalar& value);

  /**
   * Applies the configuration to this broker
   * \param config  The application configuration
   */
  void setConfiguration(const AppConfig& config);

 private:
  /// @brief Contains the tasks required to be executed by this broker.
  //   std::queue<std::function<void()>> mClientMessagingTasks;

  /// @brief Configuration information for the client communication.
  AppConfig::NodeConfig mClientConfig;

  /// @brief Configuration information for the server communication.
  AppConfig::NodeConfig mServerConfig;
};

}  // namespace CoreMessaging

#endif