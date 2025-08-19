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

    auto formId = 0x123456;
    auto text = "Sweet Roll";
    auto number = 69;

    logger::trace("The text is \" {}\"", text);
    logger::info("The form ID is: {} and as hex {:x}", formId, formId);

    auto output = std::format("Number is {}", number);
    logger::info("{}", output);
    return true;
}