/*
 * Author: Arnold Nunez
 * Project: SensorProcessor
 * Module: CoreServices
 */

#ifndef SESSION_SERVICE_H
#define SESSION_SERVICE_H

namespace CoreMessaging {
class EventBus;
}  // namespace CoreMessaging

namespace CoreServices {
class SessionService {
 public:
  /**
   * Constructor.
   * \param eventBus    The event bus used to communicate to other services.
   */
  SessionService(CoreMessaging::EventBus* eventBus);

  /**
   * Destructor.
   */
  ~SessionService();
};
}  // namespace CoreServices

#endif