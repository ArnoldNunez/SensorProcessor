
#include "EventBus.h"

#include <typeindex>

namespace CoreMessaging
{

EventBus::EventBus()
{
}

EventBus::~EventBus()
{
}

void EventBus::registerTypeWithEvent(const std::type_info& typeInfo, EventID eventId)
{
    // https://en.cppreference.com/w/cpp/types/type_index.html
    std::cout << "Registering type with event id: " << std::to_string(eventId) << std::endl;
    mTypeRegistrations[std::type_index(typeInfo)] = eventId;
}

}
