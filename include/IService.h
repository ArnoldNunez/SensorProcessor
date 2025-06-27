/*
 * Author: Arnold Nunez
 * Project: SensorProcessor
 * Module: CoreServices
 */

#ifndef I_SERVICE_H
#define I_SERVICE_H

namespace CoreMessaging {
class EventBus;
}

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
   */
  IService(CoreMessaging::EventBus* eventBus);

  // Core messaging event bus
  CoreMessaging::EventBus* mEventBus;

 private:
};
}  // namespace CoreServices

#endif