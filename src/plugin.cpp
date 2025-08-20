#include "Logger.h"
#include "ResurrectionAPI.h"

void OnMessage(SKSE::MessagingInterface::Message* message) {
    if (message->type == SKSE::MessagingInterface::kDataLoaded) {
    }
    if (message->type == SKSE::MessagingInterface::kPostLoad) {
    }
}

SKSEPluginLoad(const SKSE::LoadInterface *skse) {
    SKSE::Init(skse);
    SetupLog();
    logger::info("Plugin loaded");

    RE::Actor::get
    return true;
}