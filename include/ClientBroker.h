/*
 * Author: Arnold Nunez
 * Project: SensorProcessor
 * Module: Communication
 */

#ifndef CLIENT_BROKER_H
#define CLIENT_BROKER_H

#include <atomic>
#include <queue>
#include <thread>
#include <zmq.hpp>

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
  ClientBroker(EventBus* eventBus);

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
   * Blocking call to start bi-directional communication with clients.
   */
  void startCommunication();

  /**
   * Applies the configuration to this broker
   * \param config  The application configuration
   */
  void setConfiguration(const AppConfig& config);

 private:
  /// @brief Reference to the client broker.
  EventBus* mEventBus;

  /// @brief Configuration information for the client communication.
  AppConfig::NodeConfig mClientConfig;

  /// @brief Configuration information for the server communication.
  AppConfig::NodeConfig mServerConfig;

  /// @brief Thread used for receiving messages from clients
  std::thread mRecvThread;

  /// @brief Thread used for sending messages to clients.
  std::thread mSendThread;

  /// @brief The ZeroMQ lib socket context.
  zmq::context_t mContext;

  /// @brief Flag signaling that worker threads should exit.
  std::atomic<bool> mExitSignal;

  /**
   * Method that handles receving data from clients.
   */
  void receiveWork();

  /**
   * Method that handles sending data to clients.
   */
  void sendWork();
};

}  // namespace CoreMessaging

#endif