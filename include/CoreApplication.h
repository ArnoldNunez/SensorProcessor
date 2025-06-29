/*
 * Author: Arnold Nunez
 * Project: SensorProcessor
 * Module: SensorProcessorService
 */

#ifndef CORE_APPLICATION_H
#define CORE_APPLICATION_H

#include <thread>

#include "AppConfig.h"

/**
 * Forward declares
 */
namespace CoreMessaging {
class EventBus;
class ClientBroker;
}  // namespace CoreMessaging

/**
 * Class representing the Core application. This is
 * the entry point for the application.
 */
class CoreApplication {
 public:
  /// @brief Exit signal for the entire application tied to SIGINT.
  static volatile std::atomic<bool> EXIT_SIGNAL;

  /**
   * Constructor
   * \param appConfig       The application configuration.
   * \param eventBus        The event bus used for internal component
   *                        communications.
   * \param clientBroker    The client broker that handles messaging between
   *                        this application and the clients.
   */
  CoreApplication(const AppConfig& appConfig,
                  std::unique_ptr<CoreMessaging::EventBus>& eventBus,
                  std::unique_ptr<CoreMessaging::ClientBroker>& clientBroker);

  /**
   * Destructor
   */
  ~CoreApplication();

  /**
   * Entry point of the application
   */
  void start();

  /**
   * Exit the application. Cleans up gracefully.
   */
  void exit();

 private:
  /// @brief  The event bus used for internal component communications.
  std::unique_ptr<CoreMessaging::EventBus> mEventBus;

  /// @brief  The client broker handling messaging between the app and clients.
  std::unique_ptr<CoreMessaging::ClientBroker> mClientBroker;

  /**
   * Test function for polling client communication.
   */
  void testClientPolling();
};

#endif  // CORE_APPLICATION_H