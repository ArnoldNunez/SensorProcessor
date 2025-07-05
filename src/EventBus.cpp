/*
 * Author: Arnold Nunez
 * Project: SensorProcessor
 * Module: Communication
 */

#include "EventBus.h"

#include <typeindex>

namespace CoreMessaging {

//-----
EventBus::EventBus() {}

//-----
EventBus::~EventBus() {}

//-----
void EventBus::subscribe(CoreServices::CORE_COMMAND_ID eventId,
                         EventHandler* handler) {
  bool alreadySubscribed = false;
  for (const auto& listener : mListeners[eventId]) {
    if (listener->getId() == handler->getId()) {
      alreadySubscribed = true;
      break;
    }
  }

  if (alreadySubscribed) {
    std::cout << "Listener already subscribed" << std::endl;
  } else {
    std::cout << "Listener: " << handler->getId()
              << " subscribed to: " << std::to_string(eventId) << std::endl;
    mListeners[eventId].push_back(handler);
  }
}

//-----
void EventBus::unsubscribe(CoreServices::CORE_COMMAND_ID eventId,
                           EventHandler* handler) {
  for (int i = 0; i < mListeners[eventId].size(); ++i) {
    const auto& listener = mListeners[eventId][i];

    if (listener->getId() == handler->getId()) {
      mListeners[eventId].erase(mListeners[eventId].begin() + i);
      break;
    }
  }
}

//-----
void EventBus::distribute(const CoreServices::Command& message) {
  switch (message.commandid()) {
    case CoreServices::COMMAND_ID_LOGIN_REQUEST:
      if (message.has_loginrequest()) {
        // Convert to event
        LoginRequestEvent event(message.loginrequest());
        triggerEventListeners<LoginRequestEvent>(message.commandid(), event);
      } else {
        std::cout << "EventBus::distribute - missing payload from login request"
                  << std::endl;
      }
      break;

    default:
      std::cout << "EventBus::distribute: unsupported event id: "
                << message.commandid() << std::endl;
      break;
  }
}

}  // namespace CoreMessaging
