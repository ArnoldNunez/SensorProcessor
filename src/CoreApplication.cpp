/*
 * Author: Arnold Nunez
 * Project: SensorProcessor
 * Module: SensorProcessorService
 */

#include "CoreApplication.h"

#include <iostream>
#include <zmq.hpp>

#include "ClientBroker.h"
#include "EventBus.h"
#include "Eventing.h"
#include "Login.pb.h"
#include "addressbook.pb.h"

/// @brief Application exit signal declaration;
volatile std::atomic<bool> CoreApplication::EXIT_SIGNAL(false);

//-----
CoreApplication::CoreApplication(
    const AppConfig& appConfig,
    std::unique_ptr<CoreMessaging::EventBus>& eventBus,
    std::unique_ptr<CoreMessaging::ClientBroker>& clientBroker)
    : mEventBus(std::move(eventBus)), mClientBroker(std::move(clientBroker)) {
  mClientBroker->setConfiguration(appConfig);
}

//-----
CoreApplication::~CoreApplication() {}

//-----
void CoreApplication::start() {
  if (mClientBroker == nullptr) {
    std::cout << "CoreApplication::start [ERROR] Failed to initialize client "
                 "broker."
              << std::endl;
    return;
  }

  if (mEventBus == nullptr) {
    std::cout
        << "CoreApplication::start [ERROR] Failed to initialize Event Bus."
        << std::endl;
    return;
  }

  mClientBroker->startCommunication();
}

//-----
void CoreApplication::exit() {}