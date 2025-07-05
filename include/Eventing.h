/*
 * Author: Arnold Nunez
 * Project: SensorProcessor
 * Module: Communication
 */

#ifndef EVENTING_H
#define EVENTING_H

#include <thread>
#include <vector>

#include "Login.pb.h"

namespace CoreMessaging {

/**
 * Base class for core messaging events.
 */
class IEvent {
 public:
  /**
   * Destructor
   */
  virtual ~IEvent() {}

 protected:
  /**
   * Constructor.
   */
  IEvent() {}
};

/**
 * Event for a login request.
 */
class LoginRequestEvent : public IEvent {
 public:
  /**
   * Constructor.
   * \param data The login request data.
   */
  LoginRequestEvent(const CoreServices::LoginRequest& data)
      : IEvent(), mData(data) {}

  /**
   * Destructor.
   */
  ~LoginRequestEvent() {}

  /**
   * Get the event data.
   */
  const CoreServices::LoginRequest& data() const { return mData; }

 private:
  /**
   * The event data.
   */
  CoreServices::LoginRequest mData;
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
  LoginRequestHandler(HandlerId id) : EventHandler(id) {}

  /**
   * Destructor.
   */
  virtual ~LoginRequestHandler() {}

  /**
   * Handler for an Event
   */
  virtual void onEvent(const IEvent& event) override {
    onEvent(static_cast<const LoginRequestEvent&>(event));
  };

  /**
   * Method triggered when a \c LoginRequest event is received.
   * \param event The event received.
   */
  virtual void onEvent(const LoginRequestEvent& event) = 0;
};
}  // namespace CoreMessaging

#endif  // EVENTING_H