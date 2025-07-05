/*
 * Author: Arnold Nunez
 * Project: SensorProcessor
 * Module: Communication
 */

#include "SessionService.h"

#include "EventBus.h"

namespace CoreServices {

// Uniqe id for this service, used to subscribe uniquely to the
// event bus.
const unsigned long SessionService::EVENT_BUS_SUBSCRIBER_ID = 34954;

//-----
SessionService::SessionService(CoreMessaging::EventBus* eventBus,
                               CoreMessaging::ClientBroker* clientBroker)
    : IService(eventBus, clientBroker),
      LoginRequestHandler(EVENT_BUS_SUBSCRIBER_ID) {
  subscribeToEvents();
}

//-----
SessionService::~SessionService() {}

//-----
void SessionService::onEvent(const CoreMessaging::LoginRequestEvent& event) {
  std::cout << "=== SessionService::onEvent ===" << std::endl;
  std::cout << event.data().DebugString() << std::endl;
  std::cout << "===============================" << std::endl;
}

//-----
void SessionService::subscribeToEvents() {
  // Subscribe to the login request message
  mEventBus->subscribe(CoreServices::CORE_COMMAND_ID::COMMAND_ID_LOGIN_REQUEST,
                       this);
}

}  // namespace CoreServices
