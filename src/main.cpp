/*
 * Author: Arnold Nunez
 * Project: SensorProcessor
 * Module: SensorProcessorService
 */

#include <google/protobuf/stubs/common.h>

#include <chrono>
#include <csignal>
#include <fstream>
#include <iostream>
#include <string>
#include <thread>
#include <zmq.hpp>
#include <zmq_addon.hpp>

#include "AppConfig.h"
#include "ClientBroker.h"
#include "CoreApplication.h"
#include "EventBus.h"
#include "Eventing.h"
#include "SessionService.h"

/**
 * Handler for the SIGINT. Will attempt to exit
 * cleanly.
 */
void sigintHandler(int signal) { CoreApplication::EXIT_SIGNAL.store(true); }

/**
 * The Main entry point.
 * \param argc No command line parameters supported.
 * \param argv No command line parametesr supported.
 */
int main(int argc, char* argv[]) {
  // Verify that the version of the library that we linked against is
  // compatible with the version of the headers we compiled against.
  GOOGLE_PROTOBUF_VERIFY_VERSION;

  // Intercept SIGINT to attempt clean exit
  std::signal(SIGINT, sigintHandler);

  // Load application configuration here.
  AppConfig appConfig = AppConfig::defaultConfig();
  std::ifstream ifs("AppConfig.json");
  if (ifs.is_open()) {
    nlohmann::json configJson = nlohmann::json::parse(ifs);
    AppConfig::fromJson(configJson, appConfig);
  } else {
    std::cout << "Failed to load App config, defaulting app settings"
              << std::endl;
  }

  std::cout << appConfig.toStdString() << std::endl;

  // Initialize Core Application Dependencies
  auto applicationEventBus = std::make_unique<CoreMessaging::EventBus>();
  auto clientBroker =
      std::make_unique<CoreMessaging::ClientBroker>(applicationEventBus.get());
  auto sessionService = std::make_unique<CoreServices::SessionService>(
      applicationEventBus.get(), clientBroker.get());

  auto coreApplication = std::make_unique<CoreApplication>(
      appConfig, applicationEventBus, clientBroker);

  coreApplication->start();

  return 0;
}