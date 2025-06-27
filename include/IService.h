/*
 * Author: Arnold Nunez
 * Project: SensorProcessor
 * Module: CoreServices
 */

#ifndef I_SERVICE_H
#define I_SERVICE_H

namespace CoreMessaging {
class EventBus;
class ClientBroker;
}  // namespace CoreMessaging

namespace CoreServices {
/**
 * Base class for an application service.
 */
class IService {
 public:
  /**
   * Copy constructor (disabled).
   */
  IService(const IService&) = delete;

  /**
   * Destructor.
   */
  ~IService();

 protected:
  /**
   * Constructor.
   * \param eventBus        The event bus used to listen for messages.
   * \param clientBroker    The broker used to communicate with clients.
   */
  IService(CoreMessaging::EventBus* eventBus,
           CoreMessaging::ClientBroker* clientBroker);

  // Core messaging event bus.
  CoreMessaging::EventBus* mEventBus;

  // Core messaging client broker. Used to communicate with clients.
  CoreMessaging::ClientBroker* mClientBroker;

 private:
};
}  // namespace CoreServices

#endif