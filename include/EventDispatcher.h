#ifndef EVENT_BUS_H
#define EVENT_BUS_H

#include <thread>
#include <vector>

namespace CoreMessaging {
class EventDispatcher {
 public:
  /**
   * Constructor
   */
  EventDispatcher();

  /**
   * Destructor.
   */
  ~EventDispatcher();

  template <typename T>
  void Dispatch(const T&);

 private:
};
}  // namespace CoreMessaging

#endif  // EVENT_BUS_H