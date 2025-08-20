#include "Logger.h"
#include "ResurrectionAPI.h"
using namespace RE;
using namespace SKSE;
using namespace logger;

const char* plugin_name = "Daminikov_Perk.esp";
const int f314RM_Mgef = 0x813;
const int f314RM_Perk = 0x816;
const int f314RM_Spel = 0x815;
const int f314RM_Mesg = 0x817;
void cast_spell(RE::Actor* victim, RE::Actor* attacker, RE::SpellItem* spell);
void debug_notification(RE::BGSMessage* msg);
void addSubscriber();



class PerkResurrection : public ResurrectionAPI {
    bool should_resurrect(RE::Actor* a) const override 
    {
        
        static auto perk = RE::TESDataHandler::GetSingleton()->LookupForm<RE::BGSPerk>(f314RM_Perk, plugin_name);
        static auto mgef = RE::TESDataHandler::GetSingleton()->LookupForm<RE::EffectSetting>(f314RM_Mgef, plugin_name);
        return a->HasPerk(perk) && !a->AsMagicTarget()->HasMagicEffect(mgef);
    }

    void resurrect(RE::Actor* a) override 
    {
        a->AsActorValueOwner()->RestoreActorValue(ACTOR_VALUE_MODIFIER::kDamage, ActorValue::kHealth, 10000);
        static auto spel = RE::TESDataHandler::GetSingleton()->LookupForm<RE::SpellItem>(f314RM_Spel, plugin_name);
        cast_spell(a, a, spel);
        static auto mesg = RE::TESDataHandler::GetSingleton()->LookupForm<RE::BGSMessage>(f314RM_Mesg, plugin_name);
        debug_notification(mesg);
    }
};


static void SKSEMessageHandler(SKSE::MessagingInterface::Message* message) {
    switch (message->type) {
        case SKSE::MessagingInterface::kDataLoaded:
            addSubscriber();

            break;
    }
}


void OnMessage(MessagingInterface::Message* message) {
    

    if (message->type == MessagingInterface::kDataLoaded) {
        info("DATA загрузилась");
    }
    
    if (message->type == MessagingInterface::kPostLoad) {
    }
}



SKSEPluginLoad(const LoadInterface *skse) {
    Init(skse);
    AllocTrampoline(1 << 10);
    SetupLog();
    info("Loading OK!!!");
    auto g_messaging = reinterpret_cast<SKSE::MessagingInterface*>(skse->QueryInterface(SKSE::LoadInterface::kMessaging));
    if (!g_messaging) {
        logger::critical("Failed to load messaging interface! This error is fatal, plugin will not load.");

        return false;
    }
    g_messaging->RegisterListener("SKSE", SKSEMessageHandler);
    return true;
}


void cast_spell(RE::Actor* victim, RE::Actor* attacker, RE::SpellItem* spell) {
    RE::MagicCaster* caster = attacker->GetMagicCaster(RE::MagicSystem::CastingSource::kInstant);
    if (caster && spell) {
        caster->CastSpellImmediate(spell, false, victim, 1.0f, false, 0.0f, attacker);
    }
}

void debug_notification(RE::BGSMessage* msg) {
    info("DEBUG OK!!!");
    RE::BSString a;
    msg->GetDescription(a, msg);
    RE::DebugNotification(a.c_str());
}

void addSubscriber() {
    if (auto pluginHandle = REX::W32::GetModuleHandleA("ResurrectionAPI.dll")) {
        if (auto AddSubscriber = (AddSubscriber_t)GetProcAddress(pluginHandle, "ResurrectionAPI_AddSubscriber")) {
            AddSubscriber(std::make_unique<PerkResurrection>());
        }
    }
}