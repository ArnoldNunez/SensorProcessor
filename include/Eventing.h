/*
 * Author: Arnold Nunez
 * Project: SensorProcessor
 * Module: Communication
 */

#ifndef EVENTING_H
#define EVENTING_H

#include <thread>
#include <vector>

#include "addressbook.pb.h"

namespace CoreMessaging {
/**
 * Enumeration of all the supported event ids.
 */
enum EventID {
  UNKNOWN = 0,
  CLIENT_CONNECT_REQUEST = 1,
  LOGIN_REQUEST = 2,
  LOGIN_RESPONSE = 3
};

/**
 * Base class for core messaging events.
 */
class IEvent {
 public:
  virtual ~IEvent() {}

  int getEventId() const { return mEventID; }

 protected:
  IEvent() : mEventID(0) {}
  IEvent(int eventId) : mEventID(eventId) {}

 private:
  int mEventID;
};

/**
 * Event for a login request.
 */
class LoginRequest : public IEvent {
 public:
  LoginRequest() : IEvent(), mCredentials() {}
  LoginRequest(EventID eventId) : IEvent(eventId), mCredentials() {}
  LoginRequest(EventID eventId, const tutorial::Person& credentials)
      : IEvent(eventId), mCredentials(credentials) {}

  ~LoginRequest() {}

  const tutorial::Person& getCredentials() const { return mCredentials; }

 private:
  tutorial::Person mCredentials;
};

/**
 * Base class for core messaging event handlers.
 */
class EventHandler {
 public:
  // Each event handler needs a handler id to support subscribing and
  // unsubscribing handlers.
  typedef unsigned long HandlerId;

  /**
   * Destructor
   */
  virtual ~EventHandler() {}

  /**
   * Handler for an Event
   */
  virtual void onEvent(const IEvent& event) {};

  /**
   * Gets this event handlers Id.
   */
  HandlerId getId() const { return mHandlerId; }

 protected:
  /**
   * Constructor
   * \param id    The id of to give this handler.
   */
  EventHandler(HandlerId id) : mHandlerId(id) {}

 private:
  // The id for this event handler.
  HandlerId mHandlerId;

  /**
   * Constructor
   */
  EventHandler() : mHandlerId(0) {}
};

/**
 * Clas for handling login request messages.
 */
class LoginRequestHandler : public EventHandler {
 public:
  /**
   * Constructor (disabled)
   */
  LoginRequestHandler() = delete;

  /**
   * Constructor.
   * \param id    The id of to give this handler.
   */
  LoginRequestHandler(HandlerId id) : EventHandler(0) {}

  /**
   * Destructor.
   */
  virtual ~LoginRequestHandler() {}

  /**
   * Method triggered when a \c LoginRequest event is received.
   * \param event The event received.
   */
  virtual void onEvent(const LoginRequest& event) {
    std::cout << "LoginRequestHandler::onEvent "
              << std::to_string(event.getEventId()) << std::endl;
  }
};
}  // namespace CoreMessaging

#endif  // EVENTING_H