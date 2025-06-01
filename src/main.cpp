/*
 * Author: Arnold Nunez
 * Project: SensorProcessor
 * Module: SensorProcessorService
 */

#include <chrono>
#include <iostream>
#include <string>
#include <thread>
#include <zmq.hpp>
#include <zmq_addon.hpp>

#include "ClientBroker.h"
#include "CoreApplication.h"
#include "EventBus.h"
#include "Eventing.h"
#include "addressbook.pb.h"

/**
 * The Main entry point.
 * \param argc No command line parameters supported.
 * \param argv No command line parametesr supported.
 */
int main(int argc, char* argv[]) {
  // Verify that the version of the library that we linked against is
  // compatible with the version of the headers we compiled against.
  GOOGLE_PROTOBUF_VERIFY_VERSION;

  // Load application configuration here.

  // Initialize Core Application Dependencies
  auto applicationEventBus = std::make_unique<CoreMessaging::EventBus>();
  auto clientBroker = std::make_unique<CoreMessaging::ClientBroker>();

  // Inject dependencies
  auto coreApplication =
      std::make_unique<CoreApplication>(applicationEventBus, clientBroker);

  coreApplication->start();

  return 0;
}