/*
 * Author: Arnold Nunez
 * Project: SensorProcessor
 * Module: CoreServices
 */

#include "IService.h"

#include "EventBus.h"

namespace CoreServices {
//-----
IService::IService(CoreMessaging::EventBus* eventBus) : mEventBus(eventBus) {}

//-----
IService::~IService() {}

}  // namespace CoreServices
