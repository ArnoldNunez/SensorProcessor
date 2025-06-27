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

  // Register message types with the event bus
  mEventBus->registerTypeWithEvent(typeid(tutorial::Person),
                                   CoreMessaging::EventID::LOGIN_REQUEST);
  mEventBus->registerTypeWithEvent(typeid(CoreServices::LoginRequest),
                                   CoreMessaging::EventID::LOGIN_REQUEST);

  // mClientBroker->testClientPolling(mEventBus.get());
  // mClientBroker->testClientPollingPull(mEventBus.get());
  // mClientBroker->testClientPush();
  mClientBroker->startCommunication();
}

//-----
void CoreApplication::exit() {}