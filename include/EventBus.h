#ifndef EVENT_BUS_H
#define EVENT_BUS_H

#include "addressbook.pb.h"
#include "Eventing.h"

#include <unordered_map>
#include <functional>
#include <typeinfo>
#include <typeindex>

namespace CoreMessaging
{
class EventBus
{
public:

    /**
     * Constructor
     */
    EventBus();

    /**
     * Destructor.
     */
    ~EventBus();

    void registerTypeWithEvent(const std::type_info& typeInfo, EventID eventId);

    // void Subscribe(EventID eventId, std::function<void(const EventHandler&)> cb) {}
    /**
     * Subscribes a handler to a specific event with the given event ID.
     * \param eventId   The id of the event to subscribed to.
     * \param handler   The event handler that will triggered on the event.
     */
    void Subscribe(EventID eventId, EventHandler* handler) 
    {
        bool alreadySubscribed = false;
        for (const auto& listener : mListeners[eventId])
        {
            if (listener->getId() == handler->getId())
            {
                alreadySubscribed = true;
                break;
            }
        }

        if (alreadySubscribed)
        {
            std::cout << "Listener already subscribed" << std::endl;
        }
        else
        {
            std::cout << "Listener: " << handler->getId() << " subscribed to: " << std::to_string(eventId) << std::endl;
            mListeners[eventId].push_back(handler);
        }
    }

    /**
     * Processes a received message on the bus and turns it 
     * into an event.
     */
    template<typename Msg>
    void processMessage(const Msg& msg)
    {
        std::cout << "EventBus:processMessage: typeid:" << typeid(msg).name() << std::endl;
        // Push new message into the work item queue.

        // Determine the event
        EventID eventId = mTypeRegistrations[typeid(msg)];
        std::cout << "EventBus::processMessage type:" << std::to_string(eventId) << std::endl;
        switch (eventId)
        {
        case EventID::UNKNOWN:
            break;

        case EventID::CLIENT_CONNECT_REQUEST:
            break;

        case EventID::LOGIN_REQUEST:
        {
            std::cout << "EventBus::processMessage - LOGIN_REQUEST" << std::endl;
            LoginRequest loginRequest(EventID::LOGIN_REQUEST, msg);
            // TODO: Could push this to the dispatcher.
            triggerEventListeners(EventID::LOGIN_REQUEST, loginRequest);
            break;
        }

        default:
            break;
        }
    }

private:
    // Map tracking the received message data type with its corresponding event enumeration.
    std::unordered_map<std::type_index, EventID> mTypeRegistrations;

    // Map tracking an event with all of its registered listeners.
    // std::unordered_map<EventID, std::vector<std::function<void(const IEvent&)>>> mListeners;
    std::unordered_map<EventID, std::vector<EventHandler*>> mListeners;

    /**
     * Triggers all listeners subscribed to the given event id.
     * \param eventId   The event id.  
     * \param event     The event message containing the event data.
     */
    template<typename EVE>
    void triggerEventListeners(EventID eventId, const EVE& event)
    {
        // TODO: Use dispatcher to push this work onto other threads
        // or a threadpool. Don't want to do callback processing on
        // this event bus thread.
        for (const auto& listener : mListeners[eventId])
        {
            if (listener)
            {
                listener->onEvent(event);
            }
        }
    }
};
}

#endif // EVENT_BUS_H