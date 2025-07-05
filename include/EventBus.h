/*
 * Author: Arnold Nunez
 * Project: SensorProcessor
 * Module: Communication
 */

#ifndef EVENT_BUS_H
#define EVENT_BUS_H

#include <functional>
#include <type_traits>
#include <typeindex>
#include <typeinfo>
#include <unordered_map>

#include "BaseMessages.pb.h"
#include "Eventing.h"

namespace CoreMessaging {
class EventBus {
 public:
  /**
   * Constructor
   */
  EventBus();

  /**
   * Destructor.
   */
  ~EventBus();

  /**
   * Subscribes a handler to the specific event with the given event ID.
   * \param eventId The id of the event to subscribe to.
   * \param handler The event handler that will be triggered on the event.
   */
  void subscribe(CoreServices::CORE_COMMAND_ID eventId, EventHandler* handler);

  /**
   * Unsubscribe the handler from the given event id. Ignored if the handler
   * is not already subscribed to the event.
   * \param eventId The id of the event to unsubscribe from.
   * \param handler The event handler to unsubscribe.
   */
  void unsubscribe(CoreServices::CORE_COMMAND_ID eventId,
                   EventHandler* handler);

  /**
   * Processees the base message and distributes corresponding
   * events to listeners.
   * \param message The message to process and distribute events for.
   */
  void distribute(const CoreServices::Command& message);

 private:
  // Map tracking an event with all of its registered listeners.
  std::unordered_map<CoreServices::CORE_COMMAND_ID, std::vector<EventHandler*>>
      mListeners;

  /**
   * Triggers all listeners subscribed to the given event id.
   * \param eventId   The event id.
   * \param event     The event message containing the event data.
   */
  template <typename EVE>
  void triggerEventListeners(CoreServices::CORE_COMMAND_ID eventId,
                             const EVE& event) {
    for (const auto& listener : mListeners[eventId]) {
      if (listener) {
        listener->onEvent(event);
      }
    }
  }
};
}  // namespace CoreMessaging

#endif  // EVENT_BUS_H