/*
 * Author: Arnold Nunez
 * Project: SensorProcessor
 * Module: CoreServices
 */

#ifndef SESSION_SERVICE_H
#define SESSION_SERVICE_H

#include "Eventing.h"
#include "IService.h"

namespace CoreMessaging {
class EventBus;
class ClientBroker;
}  // namespace CoreMessaging

namespace CoreServices {
/**
 * Service responsible for handling session related messaging.
 */
class SessionService : public IService,
                       public CoreMessaging::LoginRequestHandler {
 public:
  /**
   * Constructor.
   * \param eventBus    The event bus used to communicate to other services.
   */
  SessionService(CoreMessaging::EventBus* eventBus,
                 CoreMessaging::ClientBroker* clientBroker);

  /**
   * Destructor.
   */
  ~SessionService();

  /**
   * Handler for the login request message.
   * \param event The login request event.
   */
  virtual void onEvent(const CoreMessaging::LoginRequest& event) override;

 private:
  // Identifier assigned to this subscriber of the event bus.
  static const unsigned long EVENT_BUS_SUBSCRIBER_ID;

  /**
   * Subscribe to the event bus
   */
  void subscribeToEvents();
};
}  // namespace CoreServices

#endif