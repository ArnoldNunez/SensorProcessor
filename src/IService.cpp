/*
 * Author: Arnold Nunez
 * Project: SensorProcessor
 * Module: CoreServices
 */

#include "IService.h"

#include "ClientBroker.h"
#include "EventBus.h"

namespace CoreServices {
//-----
IService::IService(CoreMessaging::EventBus* eventBus,
                   CoreMessaging::ClientBroker* clientBroker)
    : mEventBus(eventBus), mClientBroker(clientBroker) {}

//-----
IService::~IService() {}

}  // namespace CoreServices
