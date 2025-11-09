#include <RE/Skyrim.h>
#include <SKSE/SKSE.h>
#include <shlobj.h>
#include <spdlog/sinks/basic_file_sink.h>
#include <windows.h>

#include <algorithm>
#include <atomic>
#include <chrono>
#include <ctime>
#include <deque>
#include <filesystem>
#include <fstream>
#include <iomanip>
#include <map>
#include <mutex>
#include <sstream>
#include <string>
#include <thread>
#include <unordered_set>
#include <vector>
#include <optional>
#include <memory>

namespace fs = std::filesystem;
namespace logger = SKSE::log;

struct SKSELogsPaths {
    fs::path primary;
    fs::path secondary;
};

enum class SpellSystemType {
    EmotionalTears,
    VampireTears,
    BloodyNose
};

struct PluginConfig {
    struct {
        bool enabled = true;
        int amount = 300;
        int intervalMinutes = 7;
        bool showNotification = true;
    } gold;

    struct {
        bool enabled = true;
        int restorationAmount = 50;
        int intervalSeconds = 120;
        bool showNotification = true;
    } attributes;

    struct {
        bool enabled = false;
        std::string itemName = "none";
        std::string id = "xxxxxx";
        std::string plugin = "none";
        int amount = 1;
        int intervalMinutes = 1;
        bool showNotification = true;
    } item1;

    struct {
        bool enabled = false;
        std::string itemName = "none";
        std::string id = "xxxxxx";
        std::string plugin = "none";
        int amount = 1;
        int intervalMinutes = 1;
        bool showNotification = true;
    } item2;

    struct {
        bool enabled = true;
        std::string id = "003534";
        std::string plugin = "Dawnguard.esm";
        int amount = 3;
        int intervalMinutes = 3;
        bool showNotification = true;
    } milk;

    struct {
        bool enabled = true;
        std::string id = "000D73";
        std::string plugin = "YurianaWench.esp";
        int amount = 3;
        int intervalMinutes = 3;
        bool showNotification = true;
    } milkWench;

    struct {
        bool enabled = true;
        std::string id = "65FEC3";
        std::string pluginItem = "YurianaWench.esp";
        std::string npc = "576A03";
        std::string pluginNPC = "YurianaWench.esp";
        int amount = 1;
        int intervalMinutes = 3;
        bool showNotification = true;
    } milkEthel;

    struct {
        bool enabled = true;
        std::string spellID = "801";
        std::string plugin = "EmoTearsSpells.esp";
        std::string actor = "player";
        std::string objetivo = "RefID_NPC";
        int intervalActiveSeconds = 35;
        std::string event = "ostim_actor_orgasm";
        bool male = true;
        bool female = true;
        bool showNotification = true;
        bool tagsNameAnimationEnabled = false;
        std::string tagsNameAnimationList = "";
        std::string tagsNameAnimationGender = "";
        bool speedTagEnabled = false;
        bool lowIntensity = false;
        bool mediumIntensity = false;
        bool highIntensity = false;
        bool afterOStim = false;
        int intervalActiveSecondsAF = 15;
        int eventAF = 2;
        std::string pluginFaction = "EmoTearsFaction Ostim Patch.esp";
        std::string factionName = "zzEmotionalTearsFaction";
    } emotionalTearsNPC;

    struct {
        bool enabled = true;
        std::string spellID = "802";
        std::string plugin = "EmoTearsSpells.esp";
        std::string actor = "player";
        std::string objetivo = "player";
        int intervalActiveSeconds = 35;
        std::string event = "ostim_actor_orgasm";
        bool male = true;
        bool female = true;
        bool showNotification = true;
        bool tagsNameAnimationEnabled = false;
        std::string tagsNameAnimationList = "";
        std::string tagsNameAnimationGender = "";
        bool speedTagEnabled = false;
        bool lowIntensity = false;
        bool mediumIntensity = false;
        bool highIntensity = false;
        bool afterOStim = false;
        int intervalActiveSecondsAF = 15;
        int eventAF = 2;
        std::string pluginFaction = "EmoTearsFaction Ostim Patch.esp";
        std::string factionName = "zzEmotionalTearsFaction";
    } emotionalTearsPlayer;

    struct {
        bool activeMode = true;
        bool enabled = true;
        std::string spellID = "819";
        std::string plugin = "AnimatedVampireTears.esp";
        std::string actor = "player";
        std::string objetivo = "RefID_NPC";
        int intervalActiveSeconds = 60;
        std::string event = "ostim_actor_orgasm";
        bool male = true;
        bool female = true;
        bool showNotification = true;
        bool tagsNameAnimationEnabled = false;
        std::string tagsNameAnimationList = "";
        std::string tagsNameAnimationGender = "";
        bool speedTagEnabled = false;
        bool lowIntensity = false;
        bool mediumIntensity = false;
        bool highIntensity = false;
        bool afterOStim = false;
        int intervalActiveSecondsAF = 15;
        int eventAF = 2;
        std::string pluginFaction = "AnimatedVampireTears.esp";
        std::string factionName = "zzEmotionalTearsVAMPFaction";
    } vampireTearsNPC;

    struct {
        bool activeMode = true;
        bool enabled = true;
        std::string spellID = "81A";
        std::string plugin = "AnimatedVampireTears.esp";
        std::string actor = "player";
        std::string objetivo = "player";
        int intervalActiveSeconds = 60;
        std::string event = "ostim_actor_orgasm";
        bool male = true;
        bool female = true;
        bool showNotification = true;
        bool tagsNameAnimationEnabled = false;
        std::string tagsNameAnimationList = "";
        std::string tagsNameAnimationGender = "";
        bool speedTagEnabled = false;
        bool lowIntensity = false;
        bool mediumIntensity = false;
        bool highIntensity = false;
        bool afterOStim = false;
        int intervalActiveSecondsAF = 15;
        int eventAF = 2;
        std::string pluginFaction = "AnimatedVampireTears.esp";
        std::string factionName = "zzEmotionalTearsVAMPFaction";
    } vampireTearsPlayer;

    struct {
        bool enabled = true;
        std::string spellID = "819";
        std::string plugin = "AnimatedBloodyTexture.esp";
        std::string actor = "player";
        std::string objetivo = "RefID_NPC";
        int intervalActiveSeconds = 60;
        std::string event = "ostim_actor_orgasm";
        bool male = true;
        bool female = true;
        bool showNotification = true;
        bool tagsNameAnimationEnabled = false;
        std::string tagsNameAnimationList = "";
        std::string tagsNameAnimationGender = "";
        bool speedTagEnabled = false;
        bool lowIntensity = false;
        bool mediumIntensity = false;
        bool highIntensity = false;
        bool afterOStim = false;
        int intervalActiveSecondsAF = 15;
        int eventAF = 2;
        std::string pluginFaction = "AnimatedBloodyTexture.esp";
        std::string factionName = "btBleedingNoseEffectFaction";
        bool bloodyNoses = false;
        int bloodyNosescounter = 5;
        bool bloodyNosesMale = true;
        bool bloodyNosesFemale = true;
        int bloodyNosesTimeSeconds = 60;
    } bloodyNoseNPC;

    struct {
        bool enabled = true;
        std::string spellID = "81A";
        std::string plugin = "AnimatedBloodyTexture.esp";
        std::string actor = "player";
        std::string objetivo = "player";
        int intervalActiveSeconds = 60;
        std::string event = "ostim_actor_orgasm";
        bool male = true;
        bool female = true;
        bool showNotification = true;
        bool tagsNameAnimationEnabled = false;
        std::string tagsNameAnimationList = "";
        std::string tagsNameAnimationGender = "";
        bool speedTagEnabled = false;
        bool lowIntensity = false;
        bool mediumIntensity = false;
        bool highIntensity = false;
        bool afterOStim = false;
        int intervalActiveSecondsAF = 15;
        int eventAF = 2;
        std::string pluginFaction = "AnimatedBloodyTexture.esp";
        std::string factionName = "btBleedingNoseEffectFaction";
        bool bloodyNoses = false;
        int bloodyNosescounter = 5;
        bool bloodyNosesMale = true;
        bool bloodyNosesFemale = true;
        int bloodyNosesTimeSeconds = 60;
    } bloodyNosePlayer;

    struct {
        bool enabled = true;
        int restorationAmount = 50;
        std::string event = "ostim_actor_orgasm";
        bool male = true;
        bool female = true;
        bool showNotification = true;
    } attributesEvent;

    struct {
        bool enabled = false;
        std::string itemName = "none";
        std::string id = "xxxxxx";
        std::string plugin = "none";
        int amount = 1;
        std::string event = "ostim_actor_orgasm";
        bool male = true;
        bool female = true;
        bool showNotification = true;
    } item1Event;

    struct {
        bool enabled = false;
        std::string itemName = "none";
        std::string id = "xxxxxx";
        std::string plugin = "none";
        int amount = 1;
        std::string event = "ostim_actor_orgasm";
        bool male = true;
        bool female = true;
        bool showNotification = true;
    } item2Event;

    struct {
        bool enabled = false;
        std::string id = "003534";
        std::string plugin = "HearthFires.esm";
        int amount = 1;
        std::string event = "ostim_actor_orgasm";
        bool male = true;
        bool female = true;
        bool showNotification = true;
    } milkEvent;

    struct {
        bool enabled = false;
        std::string id = "000D73";
        std::string plugin = "YurianaWench.esp";
        int amount = 1;
        std::string event = "ostim_actor_orgasm";
        bool male = true;
        bool female = true;
        bool showNotification = true;
    } milkWenchEvent;

    struct {
        bool enabled = false;
        std::string id = "65FEC3";
        std::string pluginItem = "YurianaWench.esp";
        std::string npc = "576A03";
        std::string pluginNPC = "YurianaWench.esp";
        int amount = 1;
        std::string event = "ostim_actor_orgasm";
        bool male = true;
        bool female = true;
        bool showNotification = true;
    } milkEthelEvent;

    struct {
        bool enabled = true;
    } notification;
};

struct CapturedNPCData {
    RE::FormID formID = 0;
    std::string pluginName;
    bool captured = false;
    std::chrono::steady_clock::time_point lastSeen;
};

struct CachedFormIDs {
    RE::FormID item1 = 0;
    RE::FormID item2 = 0;
    RE::FormID milkDawnguard = 0;
    RE::FormID milkWench = 0;
    RE::FormID milkEthel = 0;
    RE::FormID item1Event = 0;
    RE::FormID item2Event = 0;
    RE::FormID milkEvent = 0;
    RE::FormID milkWenchEvent = 0;
    RE::FormID milkEthelEvent = 0;
    bool resolved = false;
};

struct CachedSpellIDs {
    RE::FormID emotionalTearsNPC = 0;
    RE::FormID emotionalTearsPlayer = 0;
    RE::FormID vampireTearsNPC = 0;
    RE::FormID vampireTearsPlayer = 0;
    RE::FormID bloodyNoseNPC = 0;
    RE::FormID bloodyNosePlayer = 0;
    bool resolved = false;
};

struct CachedFactionIDs {
    RE::FormID emotionalTearsFaction = 0;
    RE::FormID vampireTearsFaction = 0;
    RE::FormID bloodyNoseFaction = 0;
    bool resolved = false;
};

struct OStimEventData {
    std::string eventType;
    std::string sceneID;
    std::string actorName;
    int threadID = 0;
    int speed = -1;
    std::vector<std::string> tags;
    std::chrono::steady_clock::time_point timestamp;
};

struct AnimationTagInfo {
    std::string animationName;
    std::vector<std::string> implicitTags;
    std::string position;
    std::string intensity;
    std::chrono::steady_clock::time_point detectedTime;
};

struct TagAnalyzer {
    static std::vector<std::string> ExtractTagsFromEventData(const SKSE::ModCallbackEvent* event) {
        std::vector<std::string> tags;
        
        std::string eventName = event->eventName.c_str();
        std::string strArg = (event->strArg.c_str() != nullptr && strlen(event->strArg.c_str()) > 0) 
            ? std::string(event->strArg.c_str()) : "";
            
        if (eventName.find("ostim_scenechanged_") == 0) {
            std::string animName = eventName.substr(19);
            auto animTags = ExtractTagsFromAnimationName(animName);
            tags.insert(tags.end(), animTags.begin(), animTags.end());
        }
        
        if (!strArg.empty()) {
            auto argTags = ExtractTagsFromAnimationName(strArg);
            tags.insert(tags.end(), argTags.begin(), argTags.end());
        }
        
        if (eventName == "ostim_thread_speedchanged") {
            std::string speedStr = strArg;
            try {
                int speed = std::stoi(speedStr);
                if (speed >= 3) tags.push_back("HighIntensity");
                else if (speed >= 2) tags.push_back("MediumIntensity");
                else tags.push_back("LowIntensity");
            } catch (...) {}
        }
        
        if (eventName == "ostim_orgasm" || eventName == "ostim_actor_orgasm") {
            tags.push_back("Climax");
            if (!strArg.empty()) {
                auto climaxTags = ExtractTagsFromAnimationName(strArg);
                tags.insert(tags.end(), climaxTags.begin(), climaxTags.end());
            }
        }
        
        return tags;
    }
    
    static std::vector<std::string> ExtractTagsFromAnimationName(const std::string& animName) {
        std::vector<std::string> tags;
        std::string lower = animName;
        std::transform(lower.begin(), lower.end(), lower.begin(), ::tolower);
        
        if (lower.find("standing") != std::string::npos) tags.push_back("Standing");
        if (lower.find("sitting") != std::string::npos) tags.push_back("Sitting");
        if (lower.find("lying") != std::string::npos || lower.find("laying") != std::string::npos) tags.push_back("Lying");
        if (lower.find("doggy") != std::string::npos || lower.find("behind") != std::string::npos) tags.push_back("Doggy");
        if (lower.find("missionary") != std::string::npos || lower.find("mating") != std::string::npos) tags.push_back("Missionary");
        
        if (lower.find("oral") != std::string::npos || lower.find("bj") != std::string::npos || 
            lower.find("blowjob") != std::string::npos) tags.push_back("Oral");
        if (lower.find("vaginal") != std::string::npos || lower.find("penetration") != std::string::npos) tags.push_back("Vaginal");
        if (lower.find("anal") != std::string::npos) tags.push_back("Anal");
        if (lower.find("kiss") != std::string::npos) tags.push_back("Kissing");
        if (lower.find("touch") != std::string::npos || lower.find("caress") != std::string::npos) tags.push_back("Touching");
        
        if (lower.find("rough") != std::string::npos || lower.find("hard") != std::string::npos) tags.push_back("Rough");
        if (lower.find("gentle") != std::string::npos || lower.find("soft") != std::string::npos) tags.push_back("Gentle");
        if (lower.find("aggressive") != std::string::npos || lower.find("dom") != std::string::npos) tags.push_back("Aggressive");
        
        if (lower.find("close") != std::string::npos) tags.push_back("Intimate");
        if (lower.find("approach") != std::string::npos || lower.find("goto") != std::string::npos) tags.push_back("Transition");
        if (lower.find("spread") != std::string::npos) tags.push_back("Spread");
        if (lower.find("press") != std::string::npos) tags.push_back("Pressing");
        if (lower.find("oare") != std::string::npos) tags.push_back("OARE");
        
        return tags;
    }
};

struct ActorInfo {
    std::string name;
    RE::FormID refID = 0;
    RE::FormID baseID = 0;
    std::string race;
    std::string gender;
    bool isVampire = false;
    bool isWerewolf = false;
    bool captured = false;
};

struct ActiveSpellEffect {
    RE::FormID actorFormID;
    std::string actorName;
    bool isPlayer;
    bool isNPCCast;
    std::string gender;
    std::chrono::steady_clock::time_point activationTime;
    int durationSeconds;
    bool manuallyDeactivated = false;
    bool isTagBased = false;
    bool spellActivated = false;
    bool spellDeactivated = false;
    std::vector<std::string> activeTags;
    std::string activeAnimationName;
    SpellSystemType systemType = SpellSystemType::EmotionalTears;
};

struct ActorOrgasmCounter {
    RE::FormID actorFormID;
    std::string actorName;
    bool isPlayer;
    std::string gender;
    int orgasmCount = 0;
};

struct BloodyNoseCounter {
    RE::FormID actorFormID;
    std::string actorName;
    bool isPlayer;
    std::string gender;
    int orgasmCount = 0;
    bool spellActive = false;
    std::chrono::steady_clock::time_point spellActivationTime;
};

struct PendingSpellCast {
    RE::FormID rawActorFormID;
    RE::FormID spellFormID;
    std::string actorName;
    bool isPlayer;
    bool isNPCCast;
    std::chrono::steady_clock::time_point nextRetryTime;
    int attemptsRemaining;
    bool isTagBased;
    std::string gender;
    int durationSeconds;
    SpellSystemType systemType;
};

struct ActorSpellRecord {
    std::string actorName;
    RE::FormID actorFormID;
    bool isNPCCast;
    bool isActive;
    std::chrono::steady_clock::time_point timestamp;
    SpellSystemType systemType;
};

static std::deque<std::string> g_actionLines;
static std::deque<std::string> g_animationLines;
static std::deque<std::string> g_ostimEventLines;
static std::string g_documentsPath;
static std::string g_gamePath;
static bool g_isInitialized = false;
static std::mutex g_logMutex;
static std::mutex g_sceneMutex;
static std::mutex g_configMutex;
static std::mutex g_effectsMutex;
static std::mutex g_cacheMutex;
static std::mutex g_orgasmMutex;
static std::mutex g_orgasmCounterMutex;
static std::mutex g_pendingSpellMutex;
static std::mutex g_bloodyNoseCounterMutex;
static std::streampos g_lastOStimLogPosition = 0;
static bool g_monitoringActive = false;
static std::thread g_monitorThread;
static int g_monitorCycles = 0;
static std::unordered_set<std::string> g_processedLines;
static size_t g_lastFileSize = 0;
static std::string g_lastAnimation = "";
static std::chrono::steady_clock::time_point g_monitoringStartTime;
static bool g_initialDelayComplete = false;
static std::atomic<bool> g_isShuttingDown(false);
static SKSELogsPaths g_ostimLogPaths;
static PluginConfig g_config;

static bool g_inOStimScene = false;
static std::chrono::steady_clock::time_point g_lastGoldRewardTime;
static std::atomic<bool> g_goldRewardActive(false);

static std::chrono::steady_clock::time_point g_lastAttributesRestorationTime;
static std::atomic<bool> g_attributesRestorationActive(false);

static std::chrono::steady_clock::time_point g_lastItem1RewardTime;
static std::atomic<bool> g_item1RewardActive(false);

static std::chrono::steady_clock::time_point g_lastItem2RewardTime;
static std::atomic<bool> g_item2RewardActive(false);

static std::chrono::steady_clock::time_point g_lastMilkRewardTime;
static std::atomic<bool> g_milkRewardActive(false);

static std::chrono::steady_clock::time_point g_lastMilkWenchRewardTime;
static std::atomic<bool> g_milkWenchRewardActive(false);
static std::chrono::steady_clock::time_point g_lastMilkEthelRewardTime;
static std::atomic<bool> g_milkEthelRewardActive(false);

static bool g_wenchMilkNPCDetected = false;
static bool g_ethelNPCDetected = false;
static std::chrono::steady_clock::time_point g_lastNPCDetectionCheck;

static CapturedNPCData g_capturedYurianaWenchNPC;
static CapturedNPCData g_capturedEthelNPC;

static CachedFormIDs g_cachedItemFormIDs;
static CachedSpellIDs g_cachedSpellFormIDs;
static CachedFactionIDs g_cachedFactionIDs;

static HANDLE g_directoryHandle = INVALID_HANDLE_VALUE;
static std::thread g_fileWatchThread;
static std::atomic<bool> g_fileWatchActive(false);

static std::vector<std::string> g_detectedNPCNames;
static std::map<std::string, RE::FormID> g_npcNameToRefID;
static std::map<std::string, ActorInfo> g_nearbyNPCsCache;
static std::map<RE::FormID, std::chrono::steady_clock::time_point> g_lastOrgasmTimestamps;

static std::vector<ActiveSpellEffect> g_activeSpellEffects;
static std::vector<ActorOrgasmCounter> g_actorOrgasmCounters;
static std::vector<BloodyNoseCounter> g_bloodyNoseCounters;
static std::vector<PendingSpellCast> g_pendingSpellCasts;

static std::map<int, OStimEventData> g_currentOStimEvents;
static int g_currentOStimSpeed = 0;
static std::vector<std::string> g_currentOStimTags;
static std::chrono::steady_clock::time_point g_lastOStimEventCheck;

static AnimationTagInfo g_currentAnimationInfo;
static std::vector<std::string> g_detectedTagsFromAnimation;

static std::vector<ActorInfo> g_sceneActors;

static bool g_wenchPluginChecked = false;
static bool g_wenchPluginExists = false;
static bool g_ethelPluginChecked = false;
static bool g_ethelPluginExists = false;

static bool g_isSlowPC = false;

static std::string g_lastProcessedAnimationForTags = "";

static std::chrono::steady_clock::time_point g_sceneStartTime;
static std::chrono::steady_clock::time_point g_sceneEndTime;
static std::atomic<bool> g_cleanupPending(false);

static bool g_vampireTearsPluginDetected = false;

void StartMonitoringThread();
void StopMonitoringThread();
void WriteToActionsLog(const std::string& message, int lineNumber = 0);
void WriteToAnimationsLog(const std::string& message, int lineNumber = 0);
void WriteToOStimEventsLog(const std::string& message, int lineNumber = 0);
void CheckAndRewardGold();
void CheckAndRestoreAttributes();
void CheckAndRewardItem1();
void CheckAndRewardItem2();
void CheckAndRewardMilk();
void CheckAndRewardMilkWench();
void CheckAndRewardMilkEthel();
void CheckForNearbyNPCs();
void TryCaptureNPCFormIDs();
void ResolveItemFormIDs();
void ValidateAndUpdatePluginsInINI();
bool LoadConfiguration();
void SaveDefaultConfiguration();
std::string GetLastAnimation();
void SetLastAnimation(const std::string& animation);
bool IsInOStimScene();
void SetInOStimScene(bool inScene);
fs::path GetPluginINIPath();
RE::FormID GetFormIDFromPlugin(const std::string& pluginName, const std::string& localFormID);
bool IsAnyNPCFromPluginNearPlayer(const std::string& pluginName, float maxDistance);
bool IsSpecificNPCNearPlayer(RE::FormID npcFormID, float maxDistance);
void DetectNPCNamesFromLine(const std::string& line);
void FindAndCacheNPCRefIDs();
void ExecuteConsoleCommand(const std::string& command);
void CheckExpiredSpellEffects();
void DeactivateAllSpellEffects();
void CleanupSpellEffectsFromLog();
void CleanupSpellEffectsByFaction();
void ParseOStimEventFromLine(const std::string& line);
void ProcessOStimEventData();
void AnalyzeAnimationForTags(const std::string& animationName);
void GenerateTagsReport();
void LogDetectedTags(const std::vector<std::string>& tags, const std::string& eventName);
ActorInfo CapturePlayerInfo();
ActorInfo CaptureNPCInfo(const std::string& npcName);
void LogActorInfo(const ActorInfo& info, bool isPlayer);
bool IsActorFromPlugin(RE::FormID actorFormID, const std::string& pluginName);
void GiveAttributesEventReward(int amount, const std::string& actorName, const std::string& gender);
void GiveItemEventReward(const std::string& itemID, const std::string& pluginName, int amount, const std::string& itemName, const std::string& actorName, const std::string& gender);
void ProcessOrgasmEventRewards(const std::string& actorName, RE::FormID actorFormID, bool isPlayer, const std::string& gender);
void OnOStimOrgasmEventForSpellSystems(const std::string& actorName, RE::FormID actorFormID, bool isPlayer, const std::string& gender);
void ApplySpellEffect(RE::FormID actorFormID, bool isPlayer, bool isNPCCast, SpellSystemType systemType);
void RegisterActiveEffect(RE::FormID actorFormID, const std::string& actorName, bool isPlayer, bool isNPCCast, const std::string& gender, int durationSeconds, bool isTagBased, SpellSystemType systemType);
ActiveSpellEffect* FindActiveEffect(RE::FormID actorFormID, bool isNPCCast, SpellSystemType systemType);
void RemoveActiveEffect(RE::FormID actorFormID, bool isNPCCast, SpellSystemType systemType);
void BuildNPCsCacheForScene();
void ClearNPCsCache();
bool ShouldProcessOrgasmEvent(RE::FormID actorFormID);
void UpdateOrgasmTimestamp(RE::FormID actorFormID);
void IncrementOrgasmCounter(RE::FormID actorFormID, const std::string& actorName, bool isPlayer, const std::string& gender);
void ClearOrgasmCounters();
bool IsDLCInstalled(const std::string& dlcName);
bool IsActorVampire(RE::Actor* actor);
bool IsActorWerewolf(RE::Actor* actor);
void CheckAnimationTagsForSpellSystems(const std::string& animationName, const std::vector<std::string>& detectedTags);
void CheckAnimationTagsForSingleActor(const ActorInfo& actorInfo, const std::string& animationName, const std::vector<std::string>& detectedTags);
void RemoveTagBasedSpellEffects();
bool MatchesConfiguredTags(const std::string& animationName, const std::vector<std::string>& detectedTags, const std::string& configuredTagsList);
std::vector<std::string> SplitString(const std::string& str, char delimiter);
bool MatchesGenderFilter(const std::string& actorGender, const std::string& configuredGenders);
std::string NormalizeName(const std::string& name);
void ProcessPendingSpellCasts();
RE::FormID ResolveStableActorId(RE::FormID observedId, const std::string& observedName);
bool IsActorReadyForSpell(RE::FormID id);
bool IsRuntimeFF(RE::FormID id);
void InitializeSpellCache();
void InitializeFactionCache();
RE::FormID GetCachedSpellFormID(bool isNPCCast, SpellSystemType systemType);
RE::FormID GetCachedFactionFormID(SpellSystemType systemType);
bool CanApplySpellEffect(RE::FormID actorFormID, bool isNPCCast, SpellSystemType systemType, bool wouldBeTagBased);
std::string GetSpellSystemName(SpellSystemType type);
void IncrementBloodyNoseCounter(RE::FormID actorFormID, const std::string& actorName, bool isPlayer, const std::string& gender);
void ClearBloodyNoseCounters();
void CheckBloodyNoseCounters();
void ProcessBloodyNoseOrgasmEvent(const std::string& actorName, RE::FormID actorFormID, bool isPlayer, const std::string& gender);
void CheckVampireTearsPluginAvailability();

inline bool IsRuntimeFF(RE::FormID id) {
    return ((id >> 24) & 0xFF) == 0xFF;
}

std::string SafeWideStringToString(const std::wstring& wstr) {
    if (wstr.empty()) return std::string();
    try {
        int size_needed = WideCharToMultiByte(CP_UTF8, 0, wstr.c_str(), (int)wstr.size(), NULL, 0, NULL, NULL);
        if (size_needed <= 0) {
            size_needed = WideCharToMultiByte(CP_ACP, 0, wstr.c_str(), (int)wstr.size(), NULL, 0, NULL, NULL);
            if (size_needed <= 0) return std::string();
            std::string result(size_needed, 0);
            int converted = WideCharToMultiByte(CP_ACP, 0, wstr.c_str(), (int)wstr.size(), &result[0], size_needed, NULL, NULL);
            if (converted <= 0) return std::string();
            return result;
        }
        std::string result(size_needed, 0);
        int converted = WideCharToMultiByte(CP_UTF8, 0, wstr.c_str(), (int)wstr.size(), &result[0], size_needed, NULL, NULL);
        if (converted <= 0) return std::string();
        return result;
    } catch (...) {
        std::string result;
        result.reserve(wstr.size());
        for (wchar_t wc : wstr) {
            if (wc <= 127) {
                result.push_back(static_cast<char>(wc));
            } else {
                result.push_back('?');
            }
        }
        return result;
    }
}

std::string GetEnvVar(const std::string& key) {
    char* buf = nullptr;
    size_t sz = 0;
    if (_dupenv_s(&buf, &sz, key.c_str()) == 0 && buf != nullptr) {
        std::string value(buf);
        free(buf);
        return value;
    }
    return "";
}

std::vector<std::string> SplitString(const std::string& str, char delimiter) {
    std::vector<std::string> tokens;
    std::stringstream ss(str);
    std::string token;
    while (std::getline(ss, token, delimiter)) {
        token.erase(0, token.find_first_not_of(" \t\r\n"));
        token.erase(token.find_last_not_of(" \t\r\n") + 1);
        if (!token.empty()) {
            tokens.push_back(token);
        }
    }
    return tokens;
}

bool MatchesGenderFilter(const std::string& actorGender, const std::string& configuredGenders) {
    if (configuredGenders.empty()) {
        return true;
    }
    
    std::string lowerGender = actorGender;
    std::transform(lowerGender.begin(), lowerGender.end(), lowerGender.begin(), ::tolower);
    
    std::string lowerConfigured = configuredGenders;
    std::transform(lowerConfigured.begin(), lowerConfigured.end(), lowerConfigured.begin(), ::tolower);
    
    std::vector<std::string> allowedGenders = SplitString(lowerConfigured, ',');
    
    for (const auto& allowedGender : allowedGenders) {
        if (allowedGender == lowerGender) {
            return true;
        }
    }
    
    return false;
}

bool MatchesConfiguredTags(const std::string& animationName, const std::vector<std::string>& detectedTags, const std::string& configuredTagsList) {
    if (configuredTagsList.empty()) {
        return false;
    }
    
    std::vector<std::string> configuredTags = SplitString(configuredTagsList, ',');
    
    if (configuredTags.empty()) {
        return false;
    }
    
    std::string lowerAnimName = animationName;
    std::transform(lowerAnimName.begin(), lowerAnimName.end(), lowerAnimName.begin(), ::tolower);
    
    for (const auto& configTag : configuredTags) {
        std::string lowerConfigTag = configTag;
        std::transform(lowerConfigTag.begin(), lowerConfigTag.end(), lowerConfigTag.begin(), ::tolower);
        
        if (lowerAnimName.find(lowerConfigTag) != std::string::npos) {
            return true;
        }
        
        for (const auto& detectedTag : detectedTags) {
            std::string lowerDetectedTag = detectedTag;
            std::transform(lowerDetectedTag.begin(), lowerDetectedTag.end(), lowerDetectedTag.begin(), ::tolower);
            
            if (lowerDetectedTag == lowerConfigTag) {
                return true;
            }
        }
    }
    
    return false;
}

bool AnimationStillHasConfiguredTags(const std::string& newAnimationName, const std::vector<std::string>& newTags, const std::string& configuredTagsList) {
    if (configuredTagsList.empty()) {
        return false;
    }
    
    return MatchesConfiguredTags(newAnimationName, newTags, configuredTagsList);
}

std::string NormalizeName(const std::string& name) {
    std::string normalized = name;
    normalized.erase(0, normalized.find_first_not_of(" \t\r\n"));
    normalized.erase(normalized.find_last_not_of(" \t\r\n") + 1);
    return normalized;
}

std::string GetCurrentTimeString() {
    auto now = std::chrono::system_clock::now();
    std::time_t time_t = std::chrono::system_clock::to_time_t(now);
    std::tm buf;
    localtime_s(&buf, &time_t);
    std::stringstream ss;
    ss << std::put_time(&buf, "%Y-%m-%d %H:%M:%S");
    return ss.str();
}

std::string GetCurrentTimeStringWithMillis() {
    auto now = std::chrono::system_clock::now();
    auto ms = std::chrono::duration_cast<std::chrono::milliseconds>(now.time_since_epoch()) % 1000;
    std::time_t time_t = std::chrono::system_clock::to_time_t(now);
    std::tm buf;
    localtime_s(&buf, &time_t);
    std::stringstream ss;
    ss << std::put_time(&buf, "%Y-%m-%d %H:%M:%S");
    ss << "." << std::setfill('0') << std::setw(3) << ms.count();
    return ss.str();
}

std::string GetLastAnimation() {
    std::lock_guard<std::mutex> lock(g_sceneMutex);
    return g_lastAnimation;
}

void SetLastAnimation(const std::string& animation) {
    std::lock_guard<std::mutex> lock(g_sceneMutex);
    g_lastAnimation = animation;
}

bool IsInOStimScene() {
    std::lock_guard<std::mutex> lock(g_sceneMutex);
    return g_inOStimScene;
}

void SetInOStimScene(bool inScene) {
    std::lock_guard<std::mutex> lock(g_sceneMutex);
    g_inOStimScene = inScene;
    
    if (inScene) {
        g_sceneStartTime = std::chrono::steady_clock::now();
    }
    
    if (!inScene) {
        ClearNPCsCache();
    }
}

bool DetectSlowPC() {
    auto start = std::chrono::high_resolution_clock::now();
    
    volatile long long dummy = 0;
    for (long long i = 0; i < 100000000; i++) {
        dummy += i;
    }
    
    auto end = std::chrono::high_resolution_clock::now();
    auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(end - start).count();
    
    return duration > 200;
}

void WriteToAnimationsLog(const std::string& message, int lineNumber) {
    std::lock_guard<std::mutex> lock(g_logMutex);

    auto logsFolder = SKSE::log::log_directory();
    if (!logsFolder) return;

    auto logPath = *logsFolder / "ORisk-and-Reward-NG-Animations.log";

    auto now = std::chrono::system_clock::now();
    auto ms = std::chrono::duration_cast<std::chrono::milliseconds>(now.time_since_epoch()) % 1000;
    std::time_t time_t = std::chrono::system_clock::to_time_t(now);
    std::tm buf;
    localtime_s(&buf, &time_t);

    std::stringstream ss;
    ss << "[" << std::put_time(&buf, "%Y-%m-%d %H:%M:%S");
    ss << "." << std::setfill('0') << std::setw(3) << ms.count() << "] ";
    ss << "[log] [info] ";
    ss << "[plugin.cpp:" << lineNumber << "] ";
    ss << message;

    std::string newLine = ss.str();
    g_animationLines.push_back(newLine);

    if (g_animationLines.size() > 4000) {
        g_animationLines.pop_front();
    }

    static size_t lineCount = 0;
    lineCount++;

    if (lineCount >= 4500) {
        std::ofstream animationsFile(logPath, std::ios::trunc);
        if (animationsFile.is_open()) {
            for (const auto& line : g_animationLines) {
                animationsFile << line << std::endl;
            }
            animationsFile.close();
        }
        lineCount = g_animationLines.size();
    } else {
        std::ofstream animationsFile(logPath, std::ios::app);
        if (animationsFile.is_open()) {
            animationsFile << newLine << std::endl;
            animationsFile.close();
        }
    }
}

void WriteToActionsLog(const std::string& message, int lineNumber) {
    std::lock_guard<std::mutex> lock(g_logMutex);

    auto logsFolder = SKSE::log::log_directory();
    if (!logsFolder) return;

    auto logPath = *logsFolder / "ORisk-and-Reward-NG-Actions.log";

    auto now = std::chrono::system_clock::now();
    auto ms = std::chrono::duration_cast<std::chrono::milliseconds>(now.time_since_epoch()) % 1000;
    std::time_t time_t = std::chrono::system_clock::to_time_t(now);
    std::tm buf;
    localtime_s(&buf, &time_t);

    std::stringstream ss;
    ss << "[" << std::put_time(&buf, "%Y-%m-%d %H:%M:%S");
    ss << "." << std::setfill('0') << std::setw(3) << ms.count() << "] ";
    ss << "[log] [info] ";
    ss << "[plugin.cpp:" << lineNumber << "] ";
    ss << message;

    std::string newLine = ss.str();
    g_actionLines.push_back(newLine);

    if (g_actionLines.size() > 4000) {
        g_actionLines.pop_front();
    }

    static size_t lineCount = 0;
    lineCount++;

    if (lineCount >= 4500) {
        std::ofstream actionsFile(logPath, std::ios::trunc);
        if (actionsFile.is_open()) {
            for (const auto& line : g_actionLines) {
                actionsFile << line << std::endl;
            }
            actionsFile.close();
        }
        lineCount = g_actionLines.size();
    } else {
        std::ofstream actionsFile(logPath, std::ios::app);
        if (actionsFile.is_open()) {
            actionsFile << newLine << std::endl;
            actionsFile.close();
        }
    }
}

void WriteToOStimEventsLog(const std::string& message, int lineNumber) {
    std::lock_guard<std::mutex> lock(g_logMutex);

    auto logsFolder = SKSE::log::log_directory();
    if (!logsFolder) return;

    auto logPath = *logsFolder / "ORisk-and-Reward-NG-OStimEvents.log";

    auto now = std::chrono::system_clock::now();
    auto ms = std::chrono::duration_cast<std::chrono::milliseconds>(now.time_since_epoch()) % 1000;
    std::time_t time_t = std::chrono::system_clock::to_time_t(now);
    std::tm buf;
    localtime_s(&buf, &time_t);

    std::stringstream ss;
    ss << "[" << std::put_time(&buf, "%Y-%m-%d %H:%M:%S");
    ss << "." << std::setfill('0') << std::setw(3) << ms.count() << "] ";
    ss << "[ostim_events] [info] ";
    ss << "[plugin.cpp:" << lineNumber << "] ";
    ss << message;

    std::string newLine = ss.str();
    g_ostimEventLines.push_back(newLine);

    if (g_ostimEventLines.size() > 4000) {
        g_ostimEventLines.pop_front();
    }

    static size_t lineCount = 0;
    lineCount++;

    if (lineCount >= 4500) {
        std::ofstream eventsFile(logPath, std::ios::trunc);
        if (eventsFile.is_open()) {
            for (const auto& line : g_ostimEventLines) {
                eventsFile << line << std::endl;
            }
            eventsFile.close();
        }
        lineCount = g_ostimEventLines.size();
    } else {
        std::ofstream eventsFile(logPath, std::ios::app);
        if (eventsFile.is_open()) {
            eventsFile << newLine << std::endl;
            eventsFile.close();
        }
    }
}

void ExecuteConsoleCommand(const std::string& command) {
    auto* task = SKSE::GetTaskInterface();
    if (!task) {
        WriteToActionsLog("ERROR: Task interface not available", __LINE__);
        return;
    }
    
    task->AddTask([command]() {
        try {
            const auto scriptFactory = RE::IFormFactory::GetConcreteFormFactoryByType<RE::Script>();
            const auto script = scriptFactory ? scriptFactory->Create() : nullptr;
            
            std::unique_ptr<RE::Script> scriptPtr(script);
            
            if (scriptPtr) {
                auto* player = RE::PlayerCharacter::GetSingleton();
                if (player) {
                    scriptPtr->SetCommand(command);
                    scriptPtr->CompileAndRun(player);
                }
            }
        } catch (...) {}
    });
}

bool IsActorReadyForSpell(RE::FormID id) {
    if (id == 0) {
        return false;
    }
    
    auto* actor = RE::TESForm::LookupByID<RE::Actor>(id);
    if (!actor) {
        WriteToActionsLog("IsActorReadyForSpell: Actor not found for FormID 0x" + std::to_string(id), __LINE__);
        return false;
    }
    
    if (actor->IsDisabled() || actor->IsDeleted()) {
        WriteToActionsLog("IsActorReadyForSpell: Actor disabled/deleted for FormID 0x" + std::to_string(id), __LINE__);
        return false;
    }
    
    if (!actor->Is3DLoaded()) {
        WriteToActionsLog("IsActorReadyForSpell: Actor 3D not loaded for FormID 0x" + std::to_string(id), __LINE__);
        return false;
    }
    
    auto* magicTarget = actor->GetMagicTarget();
    if (!magicTarget) {
        WriteToActionsLog("IsActorReadyForSpell: Actor magicTarget is NULL for FormID 0x" + std::to_string(id), __LINE__);
        return false;
    }
    
    return true;
}

RE::FormID ResolveStableActorId(RE::FormID observedId, const std::string& observedName) {
    if (!IsRuntimeFF(observedId)) {
        auto* actor = RE::TESForm::LookupByID<RE::Actor>(observedId);
        if (actor) {
            WriteToActionsLog("ResolveStableActorId: Using direct stable ID 0x" + std::to_string(observedId), __LINE__);
            return observedId;
        }
    }
    
    if (IsRuntimeFF(observedId)) {
        WriteToActionsLog("ResolveStableActorId: Detected FF runtime ID 0x" + std::to_string(observedId) + " - attempting resolution", __LINE__);
        
        if (!observedName.empty()) {
            std::lock_guard<std::mutex> lock(g_cacheMutex);
            
            auto it = g_npcNameToRefID.find(observedName);
            if (it != g_npcNameToRefID.end() && !IsRuntimeFF(it->second)) {
                WriteToActionsLog("ResolveStableActorId: Resolved FF via name '" + observedName + "' to stable ID 0x" + std::to_string(it->second), __LINE__);
                return it->second;
            }
            
            std::string normalizedName = NormalizeName(observedName);
            auto cacheIt = g_nearbyNPCsCache.find(normalizedName);
            if (cacheIt != g_nearbyNPCsCache.end() && !IsRuntimeFF(cacheIt->second.refID)) {
                WriteToActionsLog("ResolveStableActorId: Resolved FF via cache '" + normalizedName + "' to stable ID 0x" + std::to_string(cacheIt->second.refID), __LINE__);
                return cacheIt->second.refID;
            }
        }
        
        for (const auto& sceneActor : g_sceneActors) {
            if (!observedName.empty() && 
                NormalizeName(sceneActor.name) == NormalizeName(observedName) &&
                !IsRuntimeFF(sceneActor.refID)) {
                WriteToActionsLog("ResolveStableActorId: Resolved FF via scene actors '" + observedName + "' to stable ID 0x" + std::to_string(sceneActor.refID), __LINE__);
                return sceneActor.refID;
            }
        }
        
        WriteToActionsLog("WARNING: ResolveStableActorId: Cannot resolve FF runtime ID 0x" + std::to_string(observedId) + " to stable ID", __LINE__);
        return 0;
    }
    
    return 0;
}

fs::path GetPluginINIPath() {
    wchar_t exePath[MAX_PATH];
    GetModuleFileNameW(NULL, exePath, MAX_PATH);
    fs::path gamePath = fs::path(exePath).parent_path();
    fs::path pluginConfigDir = gamePath / "Data" / "SKSE" / "Plugins";
    
    if (!fs::exists(pluginConfigDir)) {
        fs::create_directories(pluginConfigDir);
    }
    
    return pluginConfigDir;
}

RE::FormID GetFormIDFromPlugin(const std::string& pluginName, const std::string& localFormID) {
    auto* dataHandler = RE::TESDataHandler::GetSingleton();
    if (!dataHandler) {
        logger::error("Failed to get TESDataHandler");
        return 0;
    }

    auto* file = dataHandler->LookupModByName(pluginName);
    if (!file) {
        logger::error("Plugin not found: {}", pluginName);
        return 0;
    }

    std::string cleanID = localFormID;
    if (cleanID.length() >= 2 && cleanID.substr(0, 2) == "XX") {
        cleanID = cleanID.substr(2);
    }

    RE::FormID localID = 0;
    try {
        localID = std::stoul(cleanID, nullptr, 16);
    } catch (...) {
        logger::error("Failed to parse FormID: {}", cleanID);
        return 0;
    }

    uint8_t modIndex = file->compileIndex;
    if (modIndex == 0xFF) {
        modIndex = file->smallFileCompileIndex;
    }

    RE::FormID fullFormID = (static_cast<RE::FormID>(modIndex) << 24) | (localID & 0x00FFFFFF);
    
    return fullFormID;
}

bool ShouldProcessOrgasmEvent(RE::FormID actorFormID) {
    std::lock_guard<std::mutex> lock(g_orgasmMutex);
    
    auto now = std::chrono::steady_clock::now();
    
    auto it = g_lastOrgasmTimestamps.find(actorFormID);
    if (it != g_lastOrgasmTimestamps.end()) {
        auto elapsed = std::chrono::duration_cast<std::chrono::seconds>(now - it->second).count();
        
        if (elapsed < 2) {
            return false;
        }
    }
    
    return true;
}

void UpdateOrgasmTimestamp(RE::FormID actorFormID) {
    std::lock_guard<std::mutex> lock(g_orgasmMutex);
    g_lastOrgasmTimestamps[actorFormID] = std::chrono::steady_clock::now();
}

void IncrementOrgasmCounter(RE::FormID actorFormID, const std::string& actorName, bool isPlayer, const std::string& gender) {
    std::lock_guard<std::mutex> lock(g_orgasmCounterMutex);
    
    for (auto& counter : g_actorOrgasmCounters) {
        if (counter.actorFormID == actorFormID) {
            counter.orgasmCount++;
            WriteToOStimEventsLog("Orgasm counter updated: " + actorName + " = " + std::to_string(counter.orgasmCount), __LINE__);
            return;
        }
    }
    
    ActorOrgasmCounter newCounter;
    newCounter.actorFormID = actorFormID;
    newCounter.actorName = actorName;
    newCounter.isPlayer = isPlayer;
    newCounter.gender = gender;
    newCounter.orgasmCount = 1;
    
    g_actorOrgasmCounters.push_back(newCounter);
    
    WriteToOStimEventsLog("Orgasm counter created: " + actorName + " = 1", __LINE__);
}

void ClearOrgasmCounters() {
    std::lock_guard<std::mutex> lock(g_orgasmCounterMutex);
    g_actorOrgasmCounters.clear();
    WriteToOStimEventsLog("Orgasm counters cleared", __LINE__);
}

void IncrementBloodyNoseCounter(RE::FormID actorFormID, const std::string& actorName, bool isPlayer, const std::string& gender) {
    std::lock_guard<std::mutex> lock(g_bloodyNoseCounterMutex);

    for (auto& counter : g_bloodyNoseCounters) {
        if (counter.actorFormID == actorFormID) {
            counter.orgasmCount++;
            WriteToOStimEventsLog("BloodyNose counter updated: " + actorName + " = " + std::to_string(counter.orgasmCount) + " (Spell active: " + (counter.spellActive ? "Yes" : "No") + ")", __LINE__);
            return;
        }
    }

    BloodyNoseCounter newCounter;
    newCounter.actorFormID = actorFormID;
    newCounter.actorName = actorName;
    newCounter.isPlayer = isPlayer;
    newCounter.gender = gender;
    newCounter.orgasmCount = 1;
    newCounter.spellActive = false;

    g_bloodyNoseCounters.push_back(newCounter);

    WriteToOStimEventsLog("BloodyNose counter created: " + actorName + " = 1", __LINE__);
}

void ClearBloodyNoseCounters() {
    std::lock_guard<std::mutex> lock(g_bloodyNoseCounterMutex);
    g_bloodyNoseCounters.clear();
    WriteToOStimEventsLog("BloodyNose counters cleared", __LINE__);
}

// ===== FIXED BLOODY NOSE COUNTER SYSTEM WITH PROPER SPELL DEACTIVATION =====
void CheckBloodyNoseCounters() {
    std::lock_guard<std::mutex> lock(g_bloodyNoseCounterMutex);
    
    auto now = std::chrono::steady_clock::now();
    
    for (auto& counter : g_bloodyNoseCounters) {
        int threshold = counter.isPlayer ? g_config.bloodyNosePlayer.bloodyNosescounter : g_config.bloodyNoseNPC.bloodyNosescounter;
        int duration = counter.isPlayer ? g_config.bloodyNosePlayer.bloodyNosesTimeSeconds : g_config.bloodyNoseNPC.bloodyNosesTimeSeconds;
        
        if (counter.spellActive) {
            auto elapsed = std::chrono::duration_cast<std::chrono::seconds>(now - counter.spellActivationTime).count();
            
            if (elapsed >= duration) {
                ApplySpellEffect(counter.actorFormID, counter.isPlayer, !counter.isPlayer, SpellSystemType::BloodyNose);
                
                std::string systemName = GetSpellSystemName(SpellSystemType::BloodyNose);
                std::stringstream ss;
                ss << "[SPELL_STATE]|" << systemName << "|" << counter.actorName << "|0x" << std::hex << std::uppercase << counter.actorFormID
                   << "|" << (counter.isPlayer ? "Player" : "NPC") << "|INACTIVE|" << GetCurrentTimeString();
                WriteToActionsLog(ss.str(), __LINE__);
                
                counter.spellActive = false;
                
                WriteToOStimEventsLog("BloodyNose spell deactivated for: " + counter.actorName + " (Duration: " + std::to_string(elapsed) + "s) - Counter: " + std::to_string(counter.orgasmCount), __LINE__);
            }
        }
        
        if (!counter.spellActive && counter.orgasmCount >= threshold) {
            ApplySpellEffect(counter.actorFormID, counter.isPlayer, !counter.isPlayer, SpellSystemType::BloodyNose);
            
            counter.spellActive = true;
            counter.spellActivationTime = now;
            
            std::string systemName = GetSpellSystemName(SpellSystemType::BloodyNose);
            std::stringstream ss;
            ss << "[SPELL_STATE]|" << systemName << "|" << counter.actorName << "|0x" << std::hex << std::uppercase << counter.actorFormID
               << "|" << (counter.isPlayer ? "Player" : "NPC") << "|ACTIVE|" << GetCurrentTimeString();
            WriteToActionsLog(ss.str(), __LINE__);
            
            WriteToOStimEventsLog("BloodyNose spell reactivated for: " + counter.actorName + " (Counter: " + std::to_string(counter.orgasmCount) + " >= Threshold: " + std::to_string(threshold) + ")", __LINE__);
        }
    }
}

bool IsDLCInstalled(const std::string& dlcName) {
    auto* dataHandler = RE::TESDataHandler::GetSingleton();
    if (!dataHandler) return false;
    
    auto* file = dataHandler->LookupModByName(dlcName);
    return (file != nullptr);
}

bool IsActorVampire(RE::Actor* actor) {
    if (!actor) return false;
    
    auto* actorBase = actor->GetActorBase();
    if (!actorBase) return false;
    
    auto* actorClass = actorBase->npcClass;
    if (actorClass) {
        RE::FormID classID = actorClass->GetFormID();
        if (classID == 0x0002E00F) {
            return true;
        }
    }
    
    auto* dataHandler = RE::TESDataHandler::GetSingleton();
    if (dataHandler && IsDLCInstalled("Dawnguard.esm")) {
        auto* vampireFaction = dataHandler->LookupForm<RE::TESFaction>(0x020142E6, "Dawnguard.esm");
        if (vampireFaction && actor->IsInFaction(vampireFaction)) {
            return true;
        }
    }
    
    return false;
}

bool IsActorWerewolf(RE::Actor* actor) {
    if (!actor) return false;
    
    auto* actorBase = actor->GetActorBase();
    if (!actorBase) return false;
    
    auto* actorClass = actorBase->npcClass;
    if (actorClass) {
        RE::FormID classID = actorClass->GetFormID();
        if (classID == 0x000A1993 || classID == 0x000A1994 || classID == 0x000A1995) {
            return true;
        }
    }
    
    auto* dataHandler = RE::TESDataHandler::GetSingleton();
    if (dataHandler) {
        auto* werewolfFaction = dataHandler->LookupForm<RE::TESFaction>(0x0009A741, "Skyrim.esm");
        if (werewolfFaction && actor->IsInFaction(werewolfFaction)) {
            return true;
        }
    }
    
    return false;
}

bool IsAnyNPCFromPluginNearPlayer(const std::string& pluginName, float maxDistance) {
    auto* player = RE::PlayerCharacter::GetSingleton();
    if (!player) {
        return false;
    }
    
    auto* dataHandler = RE::TESDataHandler::GetSingleton();
    if (!dataHandler) {
        return false;
    }
    
    auto* file = dataHandler->LookupModByName(pluginName);
    if (!file) {
        return false;
    }
    
    uint8_t modIndex = file->compileIndex;
    if (modIndex == 0xFF) {
        modIndex = file->smallFileCompileIndex;
    }
    
    auto* processLists = RE::ProcessLists::GetSingleton();
    if (!processLists) {
        return false;
    }
    
    RE::NiPoint3 playerPos = player->GetPosition();
    
    for (auto& actorHandle : processLists->highActorHandles) {
        auto actor = actorHandle.get();
        if (!actor) continue;
        
        auto* actorBase = actor->GetActorBase();
        if (!actorBase) continue;
        
        uint8_t npcModIndex = (actorBase->formID >> 24) & 0xFF;
        if (npcModIndex == modIndex) {
            RE::NiPoint3 npcPos = actor->GetPosition();
            float distance = playerPos.GetDistance(npcPos);
            
            if (distance <= maxDistance) {
                return true;
            }
        }
    }
    
    return false;
}

bool IsSpecificNPCNearPlayer(RE::FormID npcFormID, float maxDistance) {
    auto* player = RE::PlayerCharacter::GetSingleton();
    if (!player) {
        return false;
    }
    
    auto* processLists = RE::ProcessLists::GetSingleton();
    if (!processLists) {
        return false;
    }
    
    RE::NiPoint3 playerPos = player->GetPosition();
    
    for (auto& actorHandle : processLists->highActorHandles) {
        auto actor = actorHandle.get();
        if (!actor) continue;
        
        auto* actorBase = actor->GetActorBase();
        if (!actorBase) continue;
        
        if (actorBase->formID == npcFormID) {
            RE::NiPoint3 npcPos = actor->GetPosition();
            float distance = playerPos.GetDistance(npcPos);
            
            if (distance <= maxDistance) {
                return true;
            }
        }
    }
    
    return false;
}

void BuildNPCsCacheForScene() {
    std::lock_guard<std::mutex> lock(g_cacheMutex);
    
    g_nearbyNPCsCache.clear();
    
    auto* player = RE::PlayerCharacter::GetSingleton();
    if (!player) return;
    
    auto* processLists = RE::ProcessLists::GetSingleton();
    if (!processLists) return;
    
    RE::NiPoint3 playerPos = player->GetPosition();
    float maxDistance = 3000.0f;
    
    auto processActorList = [&](auto& actorHandles) {
        for (auto& actorHandle : actorHandles) {
            auto actor = actorHandle.get();
            if (!actor) continue;
            
            auto* actorBase = actor->GetActorBase();
            if (!actorBase) continue;
            
            RE::NiPoint3 npcPos = actor->GetPosition();
            float distance = playerPos.GetDistance(npcPos);
            
            if (distance <= maxDistance) {
                ActorInfo info;
                info.name = actorBase->GetName();
                info.refID = actor->GetFormID();
                info.baseID = actorBase->GetFormID();
                
                auto* race = actorBase->GetRace();
                if (race) {
                    info.race = race->GetName();
                } else {
                    info.race = "Unknown";
                }
                
                info.gender = actorBase->IsFemale() ? "Female" : "Male";
                
                info.isVampire = IsActorVampire(actor.get());
                info.isWerewolf = IsActorWerewolf(actor.get());
                
                info.captured = true;
                
                std::string normalizedName = NormalizeName(info.name);
                
                g_nearbyNPCsCache[normalizedName] = info;
            }
        }
    };
    
    processActorList(processLists->highActorHandles);
    processActorList(processLists->middleHighActorHandles);
    processActorList(processLists->lowActorHandles);
    
    WriteToAnimationsLog("NPC cache built: " + std::to_string(g_nearbyNPCsCache.size()) + " NPCs within 3000 units", __LINE__);
}

void ClearNPCsCache() {
    std::lock_guard<std::mutex> lock(g_cacheMutex);
    g_nearbyNPCsCache.clear();
    WriteToAnimationsLog("NPC cache cleared", __LINE__);
}

ActorInfo CapturePlayerInfo() {
    ActorInfo info;
    
    auto* player = RE::PlayerCharacter::GetSingleton();
    if (!player) {
        return info;
    }
    
    auto* playerBase = player->GetActorBase();
    if (!playerBase) {
        return info;
    }
    
    info.name = playerBase->GetName();
    info.refID = player->GetFormID();
    info.baseID = playerBase->GetFormID();
    
    auto* race = playerBase->GetRace();
    if (race) {
        info.race = race->GetName();
    } else {
        info.race = "Unknown";
    }
    
    info.gender = playerBase->IsFemale() ? "Female" : "Male";
    
    info.isVampire = IsActorVampire(player);
    info.isWerewolf = IsActorWerewolf(player);
    
    info.captured = true;
    
    return info;
}

ActorInfo CaptureNPCInfo(const std::string& npcName) {
    ActorInfo info;
    info.name = npcName;
    
    auto* player = RE::PlayerCharacter::GetSingleton();
    if (!player) {
        return info;
    }
    
    auto* processLists = RE::ProcessLists::GetSingleton();
    if (!processLists) {
        return info;
    }
    
    RE::NiPoint3 playerPos = player->GetPosition();
    float maxDistance = 3000.0f;
    
    std::string normalizedSearchName = NormalizeName(npcName);
    
    auto searchInList = [&](auto& actorHandles) -> bool {
        for (auto& actorHandle : actorHandles) {
            auto actor = actorHandle.get();
            if (!actor) continue;
            
            auto* actorBase = actor->GetActorBase();
            if (!actorBase) continue;
            
            std::string actorName = actorBase->GetName();
            std::string normalizedActorName = NormalizeName(actorName);
            
            if (normalizedActorName == normalizedSearchName) {
                RE::NiPoint3 npcPos = actor->GetPosition();
                float distance = playerPos.GetDistance(npcPos);
                
                if (distance <= maxDistance) {
                    info.refID = actor->GetFormID();
                    info.baseID = actorBase->GetFormID();
                    
                    auto* race = actorBase->GetRace();
                    if (race) {
                        info.race = race->GetName();
                    } else {
                        info.race = "Unknown";
                    }
                    
                    info.gender = actorBase->IsFemale() ? "Female" : "Male";
                    
                    info.isVampire = IsActorVampire(actor.get());
                    info.isWerewolf = IsActorWerewolf(actor.get());
                    
                    info.captured = true;
                    
                    return true;
                }
            }
        }
        return false;
    };
    
    if (searchInList(processLists->highActorHandles)) return info;
    if (searchInList(processLists->middleHighActorHandles)) return info;
    if (searchInList(processLists->lowActorHandles)) return info;
    
    return info;
}

void LogActorInfo(const ActorInfo& info, bool isPlayer) {
    if (!info.captured) {
        WriteToAnimationsLog("Failed to capture info for: " + info.name, __LINE__);
        return;
    }
    
    std::string actorType = isPlayer ? "PLAYER" : "NPC";
    
    WriteToAnimationsLog("========================================", __LINE__);
    WriteToAnimationsLog(actorType + " DETECTED IN OSTIM SCENE", __LINE__);
    WriteToAnimationsLog("Name: " + info.name, __LINE__);
    
    std::stringstream refIDStr;
    refIDStr << "Reference ID: 0x" << std::hex << std::uppercase << info.refID;
    WriteToAnimationsLog(refIDStr.str(), __LINE__);
    
    std::stringstream baseIDStr;
    baseIDStr << "Base ID: 0x" << std::hex << std::uppercase << info.baseID;
    WriteToAnimationsLog(baseIDStr.str(), __LINE__);
    
    WriteToAnimationsLog("Race: " + info.race, __LINE__);
    WriteToAnimationsLog("Gender: " + info.gender, __LINE__);
    WriteToAnimationsLog("Is Vampire: " + std::string(info.isVampire ? "Yes" : "No"), __LINE__);
    WriteToAnimationsLog("Is Werewolf: " + std::string(info.isWerewolf ? "Yes" : "No"), __LINE__);
    WriteToAnimationsLog("========================================", __LINE__);
}

void AnalyzeAnimationForTags(const std::string& animationName) {
    if (animationName.empty()) return;
    
    g_currentAnimationInfo.animationName = animationName;
    g_currentAnimationInfo.implicitTags = TagAnalyzer::ExtractTagsFromAnimationName(animationName);
    g_currentAnimationInfo.detectedTime = std::chrono::steady_clock::now();
    
    std::string position = "Unknown";
    for (const auto& tag : g_currentAnimationInfo.implicitTags) {
        if (tag == "Standing" || tag == "Sitting" || tag == "Lying" || tag == "Doggy" || tag == "Missionary") {
            position = tag;
            break;
        }
    }
    g_currentAnimationInfo.position = position;
    
    std::string intensity = "Normal";
    for (const auto& tag : g_currentAnimationInfo.implicitTags) {
        if (tag == "Rough" || tag == "Aggressive") {
            intensity = "High";
            break;
        } else if (tag == "Gentle") {
            intensity = "Low";
            break;
        }
    }
    g_currentAnimationInfo.intensity = intensity;
    
    if (!g_currentAnimationInfo.implicitTags.empty()) {
        WriteToOStimEventsLog("========================================", __LINE__);
        WriteToOStimEventsLog("ANIMATION TAGS ANALYSIS", __LINE__);
        WriteToOStimEventsLog("Animation: " + animationName, __LINE__);
        WriteToOStimEventsLog("Position: " + position, __LINE__);
        WriteToOStimEventsLog("Intensity: " + intensity, __LINE__);
        
        std::string tagsStr = "Detected Tags: ";
        for (size_t i = 0; i < g_currentAnimationInfo.implicitTags.size(); i++) {
            tagsStr += g_currentAnimationInfo.implicitTags[i];
            if (i < g_currentAnimationInfo.implicitTags.size() - 1) {
                tagsStr += ", ";
            }
        }
        WriteToOStimEventsLog(tagsStr, __LINE__);
        WriteToOStimEventsLog("========================================", __LINE__);
    }
}

void LogDetectedTags(const std::vector<std::string>& tags, const std::string& eventName) {
    if (tags.empty()) return;
    
    WriteToOStimEventsLog("========================================", __LINE__);
    WriteToOStimEventsLog("TAGS DETECTED FROM EVENT", __LINE__);
    WriteToOStimEventsLog("Event: " + eventName, __LINE__);
    
    std::string tagsStr = "Tags: ";
    for (size_t i = 0; i < tags.size(); i++) {
        tagsStr += tags[i];
        if (i < tags.size() - 1) {
            tagsStr += ", ";
        }
    }
    WriteToOStimEventsLog(tagsStr, __LINE__);
    WriteToOStimEventsLog("========================================", __LINE__);
}

void GenerateTagsReport() {
    if (!IsInOStimScene() || g_currentAnimationInfo.animationName.empty()) {
        return;
    }
    
    auto now = std::chrono::steady_clock::now();
    auto elapsed = std::chrono::duration_cast<std::chrono::seconds>(now - g_lastOStimEventCheck).count();
    
    if (elapsed < 10) return;
    
    g_lastOStimEventCheck = now;
    
    if (!g_currentOStimTags.empty() || g_currentOStimSpeed > 0) {
        WriteToOStimEventsLog("========================================", __LINE__);
        WriteToOStimEventsLog("PERIODIC TAGS REPORT", __LINE__);
        WriteToOStimEventsLog("Current Animation: " + g_currentAnimationInfo.animationName, __LINE__);
        WriteToOStimEventsLog("Position: " + g_currentAnimationInfo.position, __LINE__);
        WriteToOStimEventsLog("Intensity: " + g_currentAnimationInfo.intensity, __LINE__);
        WriteToOStimEventsLog("Speed Level: " + std::to_string(g_currentOStimSpeed), __LINE__);
        
        if (!g_currentAnimationInfo.implicitTags.empty()) {
            std::string tagsStr = "Active Tags: ";
            for (size_t i = 0; i < g_currentAnimationInfo.implicitTags.size(); i++) {
                tagsStr += g_currentAnimationInfo.implicitTags[i];
                if (i < g_currentAnimationInfo.implicitTags.size() - 1) {
                    tagsStr += ", ";
                }
            }
            WriteToOStimEventsLog(tagsStr, __LINE__);
        } else {
            WriteToOStimEventsLog("Active Tags: None", __LINE__);
        }
        
        WriteToOStimEventsLog("========================================", __LINE__);
    }
}

void DetectNPCNamesFromLine(const std::string& line) {
    bool hasVoiceSetFound = (line.find("voice set") != std::string::npos && 
                             line.find("found for actor") != std::string::npos);
    bool hasNoVoiceSet = (line.find("no voice set found for actor") != std::string::npos);
    
    if (!hasVoiceSetFound && !hasNoVoiceSet) {
        return;
    }

    size_t actorPos = line.find("found for actor ");
    if (actorPos == std::string::npos) {
        return;
    }

    size_t nameStart = actorPos + 16;
    
    size_t nameEndBy = line.find(" by", nameStart);
    size_t nameEndComma = line.find(", using", nameStart);
    
    size_t nameEnd = std::string::npos;
    if (nameEndBy != std::string::npos && nameEndComma != std::string::npos) {
        nameEnd = std::min(nameEndBy, nameEndComma);
    } else if (nameEndBy != std::string::npos) {
        nameEnd = nameEndBy;
    } else if (nameEndComma != std::string::npos) {
        nameEnd = nameEndComma;
    }
    
    if (nameEnd == std::string::npos || nameEnd <= nameStart) {
        return;
    }

    std::string npcName = line.substr(nameStart, nameEnd - nameStart);
    
    npcName.erase(0, npcName.find_first_not_of(" \t\r\n"));
    npcName.erase(npcName.find_last_not_of(" \t\r\n") + 1);
    
    if (npcName.empty() || npcName == ",") {
        return;
    }
    
    bool alreadyDetected = false;
    for (const auto& detectedName : g_detectedNPCNames) {
        if (detectedName == npcName) {
            alreadyDetected = true;
            break;
        }
    }
    
    if (!alreadyDetected) {
        auto* player = RE::PlayerCharacter::GetSingleton();
        if (player) {
            auto* playerBase = player->GetActorBase();
            if (playerBase) {
                std::string playerName = playerBase->GetName();
                std::string normalizedPlayerName = NormalizeName(playerName);
                std::string normalizedNPCName = NormalizeName(npcName);
                
                if (normalizedPlayerName == normalizedNPCName) {
                    WriteToAnimationsLog("Detected player name in OStim log, skipping: " + npcName, __LINE__);
                    return;
                }
            }
        }
        
        g_detectedNPCNames.push_back(npcName);
        
        std::string normalizedNPCName = NormalizeName(npcName);
        
        std::lock_guard<std::mutex> lock(g_cacheMutex);
        auto it = g_nearbyNPCsCache.find(normalizedNPCName);
        
        if (it != g_nearbyNPCsCache.end()) {
            ActorInfo npcInfo = it->second;
            g_sceneActors.push_back(npcInfo);
            LogActorInfo(npcInfo, false);
            
            if (!g_currentAnimationInfo.animationName.empty()) {
                CheckAnimationTagsForSingleActor(npcInfo, g_currentAnimationInfo.animationName, g_currentAnimationInfo.implicitTags);
            }
        } else {
            WriteToAnimationsLog("========================================", __LINE__);
            WriteToAnimationsLog("NPC NOT FOUND IN CACHE", __LINE__);
            WriteToAnimationsLog("Name from OStim log: " + npcName, __LINE__);
            WriteToAnimationsLog("Normalized name: " + normalizedNPCName, __LINE__);
            WriteToAnimationsLog("Cache size: " + std::to_string(g_nearbyNPCsCache.size()) + " NPCs", __LINE__);
            
            std::string cachedNames = "Cached NPCs: ";
            int count = 0;
            for (const auto& pair : g_nearbyNPCsCache) {
                if (count > 0) cachedNames += ", ";
                cachedNames += pair.second.name;
                count++;
                if (count >= 10) {
                    cachedNames += "...";
                    break;
                }
            }
            WriteToAnimationsLog(cachedNames, __LINE__);
            WriteToAnimationsLog("Possible causes:", __LINE__);
            WriteToAnimationsLog("- NPC outside 3000 unit radius", __LINE__);
            WriteToAnimationsLog("- NPC name mismatch (encoding issue)", __LINE__);
            WriteToAnimationsLog("- NPC spawned after cache build", __LINE__);
            WriteToAnimationsLog("========================================", __LINE__);
        }
    }
}

void FindAndCacheNPCRefIDs() {
    if (g_detectedNPCNames.empty()) {
        return;
    }

    auto* player = RE::PlayerCharacter::GetSingleton();
    if (!player) {
        return;
    }

    auto* processLists = RE::ProcessLists::GetSingleton();
    if (!processLists) {
        return;
    }

    RE::NiPoint3 playerPos = player->GetPosition();

    for (const auto& npcName : g_detectedNPCNames) {
        if (g_npcNameToRefID.find(npcName) != g_npcNameToRefID.end()) {
            continue;
        }

        bool found = false;
        std::string normalizedSearchName = NormalizeName(npcName);

        auto searchInList = [&](auto& actorHandles) -> bool {
            for (auto& actorHandle : actorHandles) {
                auto actor = actorHandle.get();
                if (!actor) continue;

                auto* actorBase = actor->GetActorBase();
                if (!actorBase) continue;

                std::string actorName = actorBase->GetName();
                std::string normalizedActorName = NormalizeName(actorName);
                
                if (normalizedActorName == normalizedSearchName) {
                    RE::NiPoint3 npcPos = actor->GetPosition();
                    float distance = playerPos.GetDistance(npcPos);

                    if (distance <= 1000.0f) {
                        g_npcNameToRefID[npcName] = actor->GetFormID();
                        
                        WriteToActionsLog("NPC RefID cached: " + npcName + " = 0x" + 
                            std::to_string(actor->GetFormID()), __LINE__);
                        return true;
                    }
                }
            }
            return false;
        };

        if (searchInList(processLists->highActorHandles)) {
            found = true;
        } else if (searchInList(processLists->middleHighActorHandles)) {
            found = true;
        } else if (searchInList(processLists->lowActorHandles)) {
            found = true;
        }

        if (!found) {
            if (g_config.notification.enabled) {
                std::string msg = "ORisk-and-Reward - " + npcName + " apparently it's like a ghost";
                RE::DebugNotification(msg.c_str());
            }
        }
    }
}

ActorInfo* FindActorInfo(const std::string& actorName) {
    for (auto& actorInfo : g_sceneActors) {
        if (actorInfo.name == actorName) {
            return &actorInfo;
        }
    }
    return nullptr;
}

bool IsActorFromPlugin(RE::FormID actorFormID, const std::string& pluginName) {
    auto* dataHandler = RE::TESDataHandler::GetSingleton();
    if (!dataHandler) return false;
    
    auto* file = dataHandler->LookupModByName(pluginName);
    if (!file) return false;
    
    uint8_t modIndex = file->compileIndex;
    if (modIndex == 0xFF) {
        modIndex = file->smallFileCompileIndex;
    }
    
    uint8_t actorModIndex = (actorFormID >> 24) & 0xFF;
    
    return (actorModIndex == modIndex);
}

std::string GetSpellSystemName(SpellSystemType type) {
    switch (type) {
        case SpellSystemType::EmotionalTears: return "Emotional Tears";
        case SpellSystemType::VampireTears: return "Vampire Tears";
        case SpellSystemType::BloodyNose: return "Bloody Nose";
        default: return "Unknown";
    }
}

int GetActorFactionRank(RE::Actor* actor, RE::TESFaction* faction) {
    if (!actor || !faction) return -1;

    auto* extraFactionChanges = actor->extraList.GetByType<RE::ExtraFactionChanges>();
    if (extraFactionChanges) {
        for (auto& change : extraFactionChanges->factionChanges) {
            if (change.faction == faction) {
                return change.rank;
            }
        }
    }

    auto* actorBase = actor->GetActorBase();
    if (actorBase) {
        for (auto& factionInfo : actorBase->factions) {
            if (factionInfo.faction == faction) {
                return factionInfo.rank;
            }
        }
    }

    return -1;
}

ActiveSpellEffect* FindActiveEffect(RE::FormID actorFormID, bool isNPCCast, SpellSystemType systemType) {
    std::lock_guard<std::mutex> lock(g_effectsMutex);
    
    for (auto& effect : g_activeSpellEffects) {
        if (effect.actorFormID == actorFormID && effect.isNPCCast == isNPCCast && effect.systemType == systemType) {
            return &effect;
        }
    }
    return nullptr;
}

void RemoveActiveEffect(RE::FormID actorFormID, bool isNPCCast, SpellSystemType systemType) {
    std::lock_guard<std::mutex> lock(g_effectsMutex);
    
    for (auto it = g_activeSpellEffects.begin(); it != g_activeSpellEffects.end(); ++it) {
        if (it->actorFormID == actorFormID && it->isNPCCast == isNPCCast && it->systemType == systemType) {
            std::string systemName = GetSpellSystemName(systemType);
            WriteToOStimEventsLog("Removed active " + systemName + " effect for: " + it->actorName, __LINE__);
            g_activeSpellEffects.erase(it);
            return;
        }
    }
}

void CheckVampireTearsPluginAvailability() {
    auto* dataHandler = RE::TESDataHandler::GetSingleton();
    if (!dataHandler) {
        g_vampireTearsPluginDetected = false;
        return;
    }
    
    auto* vampirePlugin = dataHandler->LookupModByName(g_config.vampireTearsNPC.plugin);
    if (vampirePlugin) {
        g_vampireTearsPluginDetected = true;
        WriteToActionsLog("AnimatedVampireTears.esp detected - Available for vampires if ACTIVE_MODE enabled", __LINE__);
    } else {
        g_vampireTearsPluginDetected = false;
        WriteToActionsLog("AnimatedVampireTears.esp not detected - EmotionalTears will be used for vampires", __LINE__);
    }
}

void InitializeFactionCache() {
    WriteToActionsLog("========================================", __LINE__);
    WriteToActionsLog("INITIALIZING FACTION CACHE BY NAME", __LINE__);
    WriteToActionsLog("========================================", __LINE__);
    
    auto* dataHandler = RE::TESDataHandler::GetSingleton();
    if (!dataHandler) {
        WriteToActionsLog("ERROR: TESDataHandler not available", __LINE__);
        return;
    }
    
    if (g_config.emotionalTearsNPC.enabled || g_config.emotionalTearsPlayer.enabled) {
        std::string pluginName = g_config.emotionalTearsNPC.enabled ? g_config.emotionalTearsNPC.pluginFaction : g_config.emotionalTearsPlayer.pluginFaction;
        std::string factionName = g_config.emotionalTearsNPC.enabled ? g_config.emotionalTearsNPC.factionName : g_config.emotionalTearsPlayer.factionName;
        
        auto* plugin = dataHandler->LookupModByName(pluginName);
        if (plugin) {
            WriteToActionsLog("Searching for EmotionalTears Faction by name: " + factionName + " in " + pluginName, __LINE__);
            
            bool found = false;
            for (auto* faction : dataHandler->GetFormArray<RE::TESFaction>()) {
                if (!faction) continue;
                
                auto* factionFile = faction->GetFile(0);
                if (!factionFile || factionFile->fileName != pluginName) continue;
                
                std::string editorID = faction->GetFormEditorID();
                if (editorID == factionName) {
                    g_cachedFactionIDs.emotionalTearsFaction = faction->GetFormID();
                    
                    std::stringstream fullID;
                    fullID << "0x" << std::hex << std::uppercase << std::setfill('0') << std::setw(8) << faction->GetFormID();
                    
                    WriteToActionsLog("EMOTIONAL TEARS FACTION CACHED SUCCESSFULLY", __LINE__);
                    WriteToActionsLog("   Name: " + std::string(faction->GetName()), __LINE__);
                    WriteToActionsLog("   Editor ID: " + editorID, __LINE__);
                    WriteToActionsLog("   Full FormID: " + fullID.str(), __LINE__);
                    found = true;
                    break;
                }
            }
            
            if (!found) {
                WriteToActionsLog("ERROR: EmotionalTears Faction not found by name", __LINE__);
            }
        } else {
            WriteToActionsLog("ERROR: Plugin not found: " + pluginName, __LINE__);
        }
    }
    
    if (g_config.vampireTearsNPC.enabled || g_config.vampireTearsPlayer.enabled) {
        std::string pluginName = g_config.vampireTearsNPC.enabled ? g_config.vampireTearsNPC.pluginFaction : g_config.vampireTearsPlayer.pluginFaction;
        std::string factionName = g_config.vampireTearsNPC.enabled ? g_config.vampireTearsNPC.factionName : g_config.vampireTearsPlayer.factionName;
        
        auto* plugin = dataHandler->LookupModByName(pluginName);
        if (plugin) {
            WriteToActionsLog("Searching for VampireTears Faction by name: " + factionName + " in " + pluginName, __LINE__);
            
            bool found = false;
            for (auto* faction : dataHandler->GetFormArray<RE::TESFaction>()) {
                if (!faction) continue;
                
                auto* factionFile = faction->GetFile(0);
                if (!factionFile || factionFile->fileName != pluginName) continue;
                
                std::string editorID = faction->GetFormEditorID();
                if (editorID == factionName) {
                    g_cachedFactionIDs.vampireTearsFaction = faction->GetFormID();
                    
                    std::stringstream fullID;
                    fullID << "0x" << std::hex << std::uppercase << std::setfill('0') << std::setw(8) << faction->GetFormID();
                    
                    WriteToActionsLog("VAMPIRE TEARS FACTION CACHED SUCCESSFULLY", __LINE__);
                    WriteToActionsLog("   Name: " + std::string(faction->GetName()), __LINE__);
                    WriteToActionsLog("   Editor ID: " + editorID, __LINE__);
                    WriteToActionsLog("   Full FormID: " + fullID.str(), __LINE__);
                    found = true;
                    break;
                }
            }
            
            if (!found) {
                WriteToActionsLog("ERROR: VampireTears Faction not found by name", __LINE__);
            }
        } else {
            WriteToActionsLog("ERROR: Plugin not found: " + pluginName, __LINE__);
        }
    }
    
    if (g_config.bloodyNoseNPC.enabled || g_config.bloodyNosePlayer.enabled) {
        std::string pluginName = g_config.bloodyNoseNPC.enabled ? g_config.bloodyNoseNPC.pluginFaction : g_config.bloodyNosePlayer.pluginFaction;
        std::string factionName = g_config.bloodyNoseNPC.enabled ? g_config.bloodyNoseNPC.factionName : g_config.bloodyNosePlayer.factionName;
        
        auto* plugin = dataHandler->LookupModByName(pluginName);
        if (plugin) {
            WriteToActionsLog("Searching for Bloody Faction by name: " + factionName + " in " + pluginName, __LINE__);
            
            bool found = false;
            for (auto* faction : dataHandler->GetFormArray<RE::TESFaction>()) {
                if (!faction) continue;
                
                auto* factionFile = faction->GetFile(0);
                if (!factionFile || factionFile->fileName != pluginName) continue;
                
                std::string editorID = faction->GetFormEditorID();
                if (editorID == factionName) {
                    g_cachedFactionIDs.bloodyNoseFaction = faction->GetFormID();
                    
                    std::stringstream fullID;
                    fullID << "0x" << std::hex << std::uppercase << std::setfill('0') << std::setw(8) << faction->GetFormID();
                    
                    WriteToActionsLog("BLOODY FACTION CACHED SUCCESSFULLY", __LINE__);
                    WriteToActionsLog("   Name: " + std::string(faction->GetName()), __LINE__);
                    WriteToActionsLog("   Editor ID: " + editorID, __LINE__);
                    WriteToActionsLog("   Full FormID: " + fullID.str(), __LINE__);
                    found = true;
                    break;
                }
            }
            
            if (!found) {
                WriteToActionsLog("ERROR: Bloody Faction not found by name", __LINE__);
            }
        } else {
            WriteToActionsLog("ERROR: Plugin not found: " + pluginName, __LINE__);
        }
    }
    
    g_cachedFactionIDs.resolved = true;
    
    WriteToActionsLog("========================================", __LINE__);
    WriteToActionsLog("FACTION CACHE INITIALIZATION COMPLETE", __LINE__);
    WriteToActionsLog("========================================", __LINE__);
}

void InitializeSpellCache() {
    WriteToActionsLog("========================================", __LINE__);
    WriteToActionsLog("INITIALIZING SPELL CACHE", __LINE__);
    WriteToActionsLog("========================================", __LINE__);
    
    auto* dataHandler = RE::TESDataHandler::GetSingleton();
    if (!dataHandler) {
        WriteToActionsLog("ERROR: TESDataHandler not available", __LINE__);
        return;
    }
    
    if (g_config.emotionalTearsNPC.enabled) {
        auto* plugin = dataHandler->LookupModByName(g_config.emotionalTearsNPC.plugin);
        if (plugin) {
            WriteToActionsLog("Searching for EmotionalTears NPC spell ending with: " + g_config.emotionalTearsNPC.spellID + 
                             " in " + g_config.emotionalTearsNPC.plugin, __LINE__);
            
            for (auto* spell : dataHandler->GetFormArray<RE::SpellItem>()) {
                if (!spell) continue;
                
                auto* spellFile = spell->GetFile(0);
                if (!spellFile || spellFile->fileName != g_config.emotionalTearsNPC.plugin) continue;
                
                uint32_t localID = spell->GetFormID() & 0x00FFFFFF;
                std::stringstream ss;
                ss << std::hex << std::uppercase << localID;
                std::string localIDStr = ss.str();
                
                if (localIDStr.length() >= g_config.emotionalTearsNPC.spellID.length()) {
                    std::string ending = localIDStr.substr(localIDStr.length() - g_config.emotionalTearsNPC.spellID.length());
                    std::string upperSpellID = g_config.emotionalTearsNPC.spellID;
                    std::transform(upperSpellID.begin(), upperSpellID.end(), upperSpellID.begin(), ::toupper);
                    
                    if (ending == upperSpellID) {
                        g_cachedSpellFormIDs.emotionalTearsNPC = spell->GetFormID();
                        
                        std::stringstream fullID;
                        fullID << "0x" << std::hex << std::uppercase << std::setfill('0') << std::setw(8) << spell->GetFormID();
                        
                        WriteToActionsLog("EMOTIONAL TEARS NPC SPELL CACHED SUCCESSFULLY", __LINE__);
                        WriteToActionsLog("   Name: " + std::string(spell->GetName()), __LINE__);
                        WriteToActionsLog("   Full FormID: " + fullID.str(), __LINE__);
                        WriteToActionsLog("   Local ID: " + localIDStr, __LINE__);
                        break;
                    }
                }
            }
            
            if (g_cachedSpellFormIDs.emotionalTearsNPC == 0) {
                WriteToActionsLog("ERROR: EmotionalTears NPC spell not found", __LINE__);
            }
        } else {
            WriteToActionsLog("ERROR: Plugin not found: " + g_config.emotionalTearsNPC.plugin, __LINE__);
        }
    }
    
    if (g_config.emotionalTearsPlayer.enabled) {
        auto* plugin = dataHandler->LookupModByName(g_config.emotionalTearsPlayer.plugin);
        if (plugin) {
            WriteToActionsLog("Searching for EmotionalTears Player spell ending with: " + g_config.emotionalTearsPlayer.spellID + 
                             " in " + g_config.emotionalTearsPlayer.plugin, __LINE__);
            
            for (auto* spell : dataHandler->GetFormArray<RE::SpellItem>()) {
                if (!spell) continue;
                
                auto* spellFile = spell->GetFile(0);
                if (!spellFile || spellFile->fileName != g_config.emotionalTearsPlayer.plugin) continue;
                
                uint32_t localID = spell->GetFormID() & 0x00FFFFFF;
                std::stringstream ss;
                ss << std::hex << std::uppercase << localID;
                std::string localIDStr = ss.str();
                
                if (localIDStr.length() >= g_config.emotionalTearsPlayer.spellID.length()) {
                    std::string ending = localIDStr.substr(localIDStr.length() - g_config.emotionalTearsPlayer.spellID.length());
                    std::string upperSpellID = g_config.emotionalTearsPlayer.spellID;
                    std::transform(upperSpellID.begin(), upperSpellID.end(), upperSpellID.begin(), ::toupper);
                    
                    if (ending == upperSpellID) {
                        g_cachedSpellFormIDs.emotionalTearsPlayer = spell->GetFormID();
                        
                        std::stringstream fullID;
                        fullID << "0x" << std::hex << std::uppercase << std::setfill('0') << std::setw(8) << spell->GetFormID();
                        
                        WriteToActionsLog("EMOTIONAL TEARS PLAYER SPELL CACHED SUCCESSFULLY", __LINE__);
                        WriteToActionsLog("   Name: " + std::string(spell->GetName()), __LINE__);
                        WriteToActionsLog("   Full FormID: " + fullID.str(), __LINE__);
                        WriteToActionsLog("   Local ID: " + localIDStr, __LINE__);
                        break;
                    }
                }
            }
            
            if (g_cachedSpellFormIDs.emotionalTearsPlayer == 0) {
                WriteToActionsLog("ERROR: EmotionalTears Player spell not found", __LINE__);
            }
        } else {
            WriteToActionsLog("ERROR: Plugin not found: " + g_config.emotionalTearsPlayer.plugin, __LINE__);
        }
    }
    
    if (g_config.vampireTearsNPC.enabled) {
        auto* plugin = dataHandler->LookupModByName(g_config.vampireTearsNPC.plugin);
        if (plugin) {
            WriteToActionsLog("Searching for VampireTears NPC spell ending with: " + g_config.vampireTearsNPC.spellID + 
                             " in " + g_config.vampireTearsNPC.plugin, __LINE__);
            
            for (auto* spell : dataHandler->GetFormArray<RE::SpellItem>()) {
                if (!spell) continue;
                
                auto* spellFile = spell->GetFile(0);
                if (!spellFile || spellFile->fileName != g_config.vampireTearsNPC.plugin) continue;
                
                uint32_t localID = spell->GetFormID() & 0x00FFFFFF;
                std::stringstream ss;
                ss << std::hex << std::uppercase << localID;
                std::string localIDStr = ss.str();
                
                if (localIDStr.length() >= g_config.vampireTearsNPC.spellID.length()) {
                    std::string ending = localIDStr.substr(localIDStr.length() - g_config.vampireTearsNPC.spellID.length());
                    std::string upperSpellID = g_config.vampireTearsNPC.spellID;
                    std::transform(upperSpellID.begin(), upperSpellID.end(), upperSpellID.begin(), ::toupper);
                    
                    if (ending == upperSpellID) {
                        g_cachedSpellFormIDs.vampireTearsNPC = spell->GetFormID();
                        
                        std::stringstream fullID;
                        fullID << "0x" << std::hex << std::uppercase << std::setfill('0') << std::setw(8) << spell->GetFormID();
                        
                        WriteToActionsLog("VAMPIRE TEARS NPC SPELL CACHED SUCCESSFULLY", __LINE__);
                        WriteToActionsLog("   Name: " + std::string(spell->GetName()), __LINE__);
                        WriteToActionsLog("   Full FormID: " + fullID.str(), __LINE__);
                        WriteToActionsLog("   Local ID: " + localIDStr, __LINE__);
                        break;
                    }
                }
            }
            
            if (g_cachedSpellFormIDs.vampireTearsNPC == 0) {
                WriteToActionsLog("ERROR: VampireTears NPC spell not found", __LINE__);
            }
        } else {
            WriteToActionsLog("ERROR: Plugin not found: " + g_config.vampireTearsNPC.plugin, __LINE__);
        }
    }
    
    if (g_config.vampireTearsPlayer.enabled) {
        auto* plugin = dataHandler->LookupModByName(g_config.vampireTearsPlayer.plugin);
        if (plugin) {
            WriteToActionsLog("Searching for VampireTears Player spell ending with: " + g_config.vampireTearsPlayer.spellID + 
                             " in " + g_config.vampireTearsPlayer.plugin, __LINE__);
            
            for (auto* spell : dataHandler->GetFormArray<RE::SpellItem>()) {
                if (!spell) continue;
                
                auto* spellFile = spell->GetFile(0);
                if (!spellFile || spellFile->fileName != g_config.vampireTearsPlayer.plugin) continue;
                
                uint32_t localID = spell->GetFormID() & 0x00FFFFFF;
                std::stringstream ss;
                ss << std::hex << std::uppercase << localID;
                std::string localIDStr = ss.str();
                
                if (localIDStr.length() >= g_config.vampireTearsPlayer.spellID.length()) {
                    std::string ending = localIDStr.substr(localIDStr.length() - g_config.vampireTearsPlayer.spellID.length());
                    std::string upperSpellID = g_config.vampireTearsPlayer.spellID;
                    std::transform(upperSpellID.begin(), upperSpellID.end(), upperSpellID.begin(), ::toupper);
                    
                    if (ending == upperSpellID) {
                        g_cachedSpellFormIDs.vampireTearsPlayer = spell->GetFormID();
                        
                        std::stringstream fullID;
                        fullID << "0x" << std::hex << std::uppercase << std::setfill('0') << std::setw(8) << spell->GetFormID();
                        
                        WriteToActionsLog("VAMPIRE TEARS PLAYER SPELL CACHED SUCCESSFULLY", __LINE__);
                        WriteToActionsLog("   Name: " + std::string(spell->GetName()), __LINE__);
                        WriteToActionsLog("   Full FormID: " + fullID.str(), __LINE__);
                        WriteToActionsLog("   Local ID: " + localIDStr, __LINE__);
                        break;
                    }
                }
            }
            
            if (g_cachedSpellFormIDs.vampireTearsPlayer == 0) {
                WriteToActionsLog("ERROR: VampireTears Player spell not found", __LINE__);
            }
        } else {
            WriteToActionsLog("ERROR: Plugin not found: " + g_config.vampireTearsPlayer.plugin, __LINE__);
        }
    }
    
    if (g_config.bloodyNoseNPC.enabled) {
        auto* plugin = dataHandler->LookupModByName(g_config.bloodyNoseNPC.plugin);
        if (plugin) {
            WriteToActionsLog("Searching for BloodyNose NPC spell ending with: " + g_config.bloodyNoseNPC.spellID + 
                             " in " + g_config.bloodyNoseNPC.plugin, __LINE__);
            
            for (auto* spell : dataHandler->GetFormArray<RE::SpellItem>()) {
                if (!spell) continue;
                
                auto* spellFile = spell->GetFile(0);
                if (!spellFile || spellFile->fileName != g_config.bloodyNoseNPC.plugin) continue;
                
                uint32_t localID = spell->GetFormID() & 0x00FFFFFF;
                std::stringstream ss;
                ss << std::hex << std::uppercase << localID;
                std::string localIDStr = ss.str();
                
                if (localIDStr.length() >= g_config.bloodyNoseNPC.spellID.length()) {
                    std::string ending = localIDStr.substr(localIDStr.length() - g_config.bloodyNoseNPC.spellID.length());
                    std::string upperSpellID = g_config.bloodyNoseNPC.spellID;
                    std::transform(upperSpellID.begin(), upperSpellID.end(), upperSpellID.begin(), ::toupper);
                    
                    if (ending == upperSpellID) {
                        g_cachedSpellFormIDs.bloodyNoseNPC = spell->GetFormID();
                        
                        std::stringstream fullID;
                        fullID << "0x" << std::hex << std::uppercase << std::setfill('0') << std::setw(8) << spell->GetFormID();
                        
                        WriteToActionsLog("BLOODY NOSE NPC SPELL CACHED SUCCESSFULLY", __LINE__);
                        WriteToActionsLog("   Name: " + std::string(spell->GetName()), __LINE__);
                        WriteToActionsLog("   Full FormID: " + fullID.str(), __LINE__);
                        WriteToActionsLog("   Local ID: " + localIDStr, __LINE__);
                        break;
                    }
                }
            }
            
            if (g_cachedSpellFormIDs.bloodyNoseNPC == 0) {
                WriteToActionsLog("ERROR: BloodyNose NPC spell not found", __LINE__);
            }
        } else {
            WriteToActionsLog("ERROR: Plugin not found: " + g_config.bloodyNoseNPC.plugin, __LINE__);
        }
    }
    
    if (g_config.bloodyNosePlayer.enabled) {
        auto* plugin = dataHandler->LookupModByName(g_config.bloodyNosePlayer.plugin);
        if (plugin) {
            WriteToActionsLog("Searching for BloodyNose Player spell ending with: " + g_config.bloodyNosePlayer.spellID + 
                             " in " + g_config.bloodyNosePlayer.plugin, __LINE__);
            
            for (auto* spell : dataHandler->GetFormArray<RE::SpellItem>()) {
                if (!spell) continue;
                
                auto* spellFile = spell->GetFile(0);
                if (!spellFile || spellFile->fileName != g_config.bloodyNosePlayer.plugin) continue;
                
                uint32_t localID = spell->GetFormID() & 0x00FFFFFF;
                std::stringstream ss;
                ss << std::hex << std::uppercase << localID;
                std::string localIDStr = ss.str();
                
                if (localIDStr.length() >= g_config.bloodyNosePlayer.spellID.length()) {
                    std::string ending = localIDStr.substr(localIDStr.length() - g_config.bloodyNosePlayer.spellID.length());
                    std::string upperSpellID = g_config.bloodyNosePlayer.spellID;
                    std::transform(upperSpellID.begin(), upperSpellID.end(), upperSpellID.begin(), ::toupper);
                    
                    if (ending == upperSpellID) {
                        g_cachedSpellFormIDs.bloodyNosePlayer = spell->GetFormID();
                        
                        std::stringstream fullID;
                        fullID << "0x" << std::hex << std::uppercase << std::setfill('0') << std::setw(8) << spell->GetFormID();
                        
                        WriteToActionsLog("BLOODY NOSE PLAYER SPELL CACHED SUCCESSFULLY", __LINE__);
                        WriteToActionsLog("   Name: " + std::string(spell->GetName()), __LINE__);
                        WriteToActionsLog("   Full FormID: " + fullID.str(), __LINE__);
                        WriteToActionsLog("   Local ID: " + localIDStr, __LINE__);
                        break;
                    }
                }
            }
            
            if (g_cachedSpellFormIDs.bloodyNosePlayer == 0) {
                WriteToActionsLog("ERROR: BloodyNose Player spell not found", __LINE__);
            }
        } else {
            WriteToActionsLog("ERROR: Plugin not found: " + g_config.bloodyNosePlayer.plugin, __LINE__);
        }
    }
    
    g_cachedSpellFormIDs.resolved = true;
    
    WriteToActionsLog("========================================", __LINE__);
    WriteToActionsLog("SPELL CACHE INITIALIZATION COMPLETE", __LINE__);
    WriteToActionsLog("========================================", __LINE__);
}

RE::FormID GetCachedSpellFormID(bool isNPCCast, SpellSystemType systemType) {
    if (!g_cachedSpellFormIDs.resolved) {
        WriteToActionsLog("WARNING: Spell cache not initialized, calling InitializeSpellCache()", __LINE__);
        InitializeSpellCache();
    }
    
    RE::FormID spellID = 0;
    
    switch (systemType) {
        case SpellSystemType::EmotionalTears:
            spellID = isNPCCast ? g_cachedSpellFormIDs.emotionalTearsNPC : g_cachedSpellFormIDs.emotionalTearsPlayer;
            break;
        case SpellSystemType::VampireTears:
            spellID = isNPCCast ? g_cachedSpellFormIDs.vampireTearsNPC : g_cachedSpellFormIDs.vampireTearsPlayer;
            break;
        case SpellSystemType::BloodyNose:
            spellID = isNPCCast ? g_cachedSpellFormIDs.bloodyNoseNPC : g_cachedSpellFormIDs.bloodyNosePlayer;
            break;
    }
    
    if (spellID == 0) {
        std::string systemName = GetSpellSystemName(systemType);
        std::string castType = isNPCCast ? "NPC" : "Player";
        WriteToActionsLog("ERROR: Cached spell FormID is 0 for " + systemName + " " + castType, __LINE__);
    }
    
    return spellID;
}

RE::FormID GetCachedFactionFormID(SpellSystemType systemType) {
    if (!g_cachedFactionIDs.resolved) {
        WriteToActionsLog("WARNING: Faction cache not initialized, calling InitializeFactionCache()", __LINE__);
        InitializeFactionCache();
    }
    
    RE::FormID factionID = 0;
    
    switch (systemType) {
        case SpellSystemType::EmotionalTears:
            factionID = g_cachedFactionIDs.emotionalTearsFaction;
            break;
        case SpellSystemType::VampireTears:
            factionID = g_cachedFactionIDs.vampireTearsFaction;
            break;
        case SpellSystemType::BloodyNose:
            factionID = g_cachedFactionIDs.bloodyNoseFaction;
            break;
    }
    
    if (factionID == 0) {
        std::string systemName = GetSpellSystemName(systemType);
        WriteToActionsLog("ERROR: Cached faction FormID is 0 for " + systemName, __LINE__);
    }
    
    return factionID;
}

bool CanApplySpellEffect(RE::FormID actorFormID, bool isNPCCast, SpellSystemType systemType, bool wouldBeTagBased) {
    std::lock_guard<std::mutex> lock(g_effectsMutex);
    
    for (const auto& effect : g_activeSpellEffects) {
        if (effect.actorFormID == actorFormID && effect.isNPCCast == isNPCCast && effect.systemType == systemType) {
            return false;
        }
    }
    
    return true;
}

void ApplySpellEffect(RE::FormID actorFormID, bool isPlayer, bool isNPCCast, SpellSystemType systemType) {
    std::string actorName = "";
    std::string gender = "";
    
    for (const auto& sceneActor : g_sceneActors) {
        if (sceneActor.refID == actorFormID) {
            actorName = sceneActor.name;
            gender = sceneActor.gender;
            break;
        }
    }
    
    if (actorName.empty()) {
        auto* actor = RE::TESForm::LookupByID<RE::Actor>(actorFormID);
        if (actor && actor->GetActorBase()) {
            actorName = actor->GetActorBase()->GetName();
            gender = actor->GetActorBase()->IsFemale() ? "Female" : "Male";
        } else {
            actorName = "Unknown Actor";
            gender = "Unknown";
        }
    }
    
    std::string systemName = GetSpellSystemName(systemType);
    
    WriteToActionsLog("========================================", __LINE__);
    WriteToActionsLog(systemName + " SPELL CAST REQUEST", __LINE__);
    WriteToActionsLog("Actor: " + actorName, __LINE__);
    WriteToActionsLog("FormID: 0x" + std::to_string(actorFormID), __LINE__);
    WriteToActionsLog("Is NPC Cast: " + std::string(isNPCCast ? "Yes" : "No"), __LINE__);
    
    RE::FormID spellID = GetCachedSpellFormID(isNPCCast, systemType);
    
    if (spellID == 0) {
        WriteToActionsLog("ERROR: Cached spell FormID is 0", __LINE__);
        WriteToActionsLog("========================================", __LINE__);
        return;
    }
    
    std::stringstream ss;
    ss << "[SPELL_STATE]|" << systemName << "|" << actorName << "|0x" << std::hex << std::uppercase << actorFormID 
       << "|" << (isNPCCast ? "NPC" : "Player") << "|ACTIVE|" << GetCurrentTimeString();
    WriteToActionsLog(ss.str(), __LINE__);
    
    auto* task = SKSE::GetTaskInterface();
    if (!task) {
        WriteToActionsLog("ERROR: Task interface not available", __LINE__);
        WriteToActionsLog("========================================", __LINE__);
        return;
    }
    
    task->AddTask([spellID, actorFormID, actorName, systemName]() {
        auto* player = RE::PlayerCharacter::GetSingleton();
        if (!player) return;
        
        auto* targetActor = RE::TESForm::LookupByID<RE::Actor>(actorFormID);
        if (!targetActor) return;
        
        auto* spell = RE::TESForm::LookupByID<RE::SpellItem>(spellID);
        if (!spell) return;
        
        auto* magicCaster = player->GetMagicCaster(RE::MagicSystem::CastingSource::kInstant);
        if (magicCaster) {
            magicCaster->CastSpellImmediate(spell, false, targetActor, 1.0f, false, 0.0f, targetActor);
            WriteToActionsLog(systemName + " spell cast SUCCESSFULLY for: " + actorName, __LINE__);
        }
    });
    
    WriteToActionsLog("Spell cast task queued", __LINE__);
    WriteToActionsLog("========================================", __LINE__);
}

void RegisterActiveEffect(RE::FormID actorFormID, const std::string& actorName, bool isPlayer, bool isNPCCast, const std::string& gender, int durationSeconds, bool isTagBased, SpellSystemType systemType) {
    std::lock_guard<std::mutex> lock(g_effectsMutex);
    
    ActiveSpellEffect effect;
    effect.actorFormID = actorFormID;
    effect.actorName = actorName;
    effect.isPlayer = isPlayer;
    effect.isNPCCast = isNPCCast;
    effect.gender = gender;
    effect.activationTime = std::chrono::steady_clock::now();
    effect.durationSeconds = durationSeconds;
    effect.manuallyDeactivated = false;
    effect.isTagBased = isTagBased;
    effect.spellActivated = true;
    effect.spellDeactivated = false;
    effect.activeTags = g_currentAnimationInfo.implicitTags;
    effect.activeAnimationName = g_currentAnimationInfo.animationName;
    effect.systemType = systemType;
    
    g_activeSpellEffects.push_back(effect);
    
    std::string systemName = GetSpellSystemName(systemType);
    std::string effectType = isTagBased ? "Tag-Based" : "Event-Based";
    WriteToOStimEventsLog("Registered active " + systemName + " effect (" + effectType + ") for: " + actorName, __LINE__);
}

void CheckExpiredSpellEffects() {
    if (g_activeSpellEffects.empty()) {
        return;
    }
    
    std::lock_guard<std::mutex> lock(g_effectsMutex);
    
    auto now = std::chrono::steady_clock::now();
    
    for (auto it = g_activeSpellEffects.begin(); it != g_activeSpellEffects.end();) {
        if (it->manuallyDeactivated) {
            it = g_activeSpellEffects.erase(it);
            continue;
        }
        
        if (it->isTagBased) {
            ++it;
            continue;
        }
        
        auto elapsed = std::chrono::duration_cast<std::chrono::seconds>(now - it->activationTime).count();
        
        if (elapsed >= it->durationSeconds && !it->spellDeactivated) {
            ApplySpellEffect(it->actorFormID, it->isPlayer, it->isNPCCast, it->systemType);
            
            std::string systemName = GetSpellSystemName(it->systemType);
            
            std::stringstream ss;
            ss << "[SPELL_STATE]|" << systemName << "|" << it->actorName << "|0x" << std::hex << std::uppercase << it->actorFormID 
               << "|" << (it->isNPCCast ? "NPC" : "Player") << "|INACTIVE|" << GetCurrentTimeString();
            WriteToActionsLog(ss.str(), __LINE__);
            
            it->spellDeactivated = true;
            
            WriteToOStimEventsLog(systemName + " EVENT expired and deactivated for: " + it->actorName + " (" + std::to_string(elapsed) + "s)", __LINE__);
            
            it = g_activeSpellEffects.erase(it);
        } else {
            ++it;
        }
    }
}

void DeactivateAllSpellEffects() {
    std::lock_guard<std::mutex> lock(g_effectsMutex);
    WriteToOStimEventsLog("Clearing internal spell systems tracking", __LINE__);
    g_activeSpellEffects.clear();
}

void CleanupSpellEffectsFromLog() {
    WriteToOStimEventsLog("========================================", __LINE__);
    WriteToOStimEventsLog("LOG-BASED SPELL CLEANUP INITIATED", __LINE__);
    
    auto logsFolder = SKSE::log::log_directory();
    if (!logsFolder) {
        WriteToOStimEventsLog("ERROR: Cannot access logs folder", __LINE__);
        WriteToOStimEventsLog("========================================", __LINE__);
        return;
    }
    
    auto logPath = *logsFolder / "ORisk-and-Reward-NG-Actions.log";
    
    if (!fs::exists(logPath)) {
        WriteToOStimEventsLog("WARNING: Actions log does not exist", __LINE__);
        WriteToOStimEventsLog("========================================", __LINE__);
        return;
    }
    
    std::ifstream logFile(logPath);
    if (!logFile.is_open()) {
        WriteToOStimEventsLog("ERROR: Cannot open Actions log for reading", __LINE__);
        WriteToOStimEventsLog("========================================", __LINE__);
        return;
    }
    
    std::map<std::string, ActorSpellRecord> actorRecords;
    
    std::string line;
    while (std::getline(logFile, line)) {
        if (line.find("[SPELL_STATE]") == std::string::npos) {
            continue;
        }
        
        size_t markerPos = line.find("[SPELL_STATE]");
        std::string data = line.substr(markerPos + 13);
        
        std::vector<std::string> parts = SplitString(data, '|');
        
        if (parts.size() < 6) {
            continue;
        }
        
        std::string systemName = parts[0];
        std::string actorName = parts[1];
        std::string formIDStr = parts[2];
        std::string spellType = parts[3];
        std::string state = parts[4];
        std::string timestamp = parts[5];
        
        RE::FormID actorFormID = 0;
        try {
            if (formIDStr.find("0x") == 0 || formIDStr.find("0X") == 0) {
                actorFormID = std::stoull(formIDStr.substr(2), nullptr, 16);
            } else {
                actorFormID = std::stoull(formIDStr, nullptr, 16);
            }
        } catch (...) {
            continue;
        }
        
        bool isNPCCast = (spellType == "NPC");
        bool isActive = (state == "ACTIVE");
        
        SpellSystemType systemType = SpellSystemType::EmotionalTears;
        if (systemName == "Vampire Tears") systemType = SpellSystemType::VampireTears;
        else if (systemName == "Bloody Nose") systemType = SpellSystemType::BloodyNose;
        
        std::string key = actorName + "_" + spellType + "_" + systemName;
        
        ActorSpellRecord record;
        record.actorName = actorName;
        record.actorFormID = actorFormID;
        record.isNPCCast = isNPCCast;
        record.isActive = isActive;
        record.timestamp = std::chrono::steady_clock::now();
        record.systemType = systemType;
        
        actorRecords[key] = record;
    }
    
    logFile.close();
    
    WriteToOStimEventsLog("Parsed " + std::to_string(actorRecords.size()) + " unique actor spell records from log", __LINE__);
    
    int actorsWithActiveSpells = 0;
    int successfulCleanups = 0;
    int failedCleanups = 0;
    
    for (const auto& pair : actorRecords) {
        const ActorSpellRecord& record = pair.second;
        
        if (!record.isActive) {
            continue;
        }
        
        actorsWithActiveSpells++;
        
        std::string systemName = GetSpellSystemName(record.systemType);
        
        WriteToOStimEventsLog("Found active " + systemName + " spell for: " + record.actorName + " (Type: " + 
            std::string(record.isNPCCast ? "NPC" : "Player") + ")", __LINE__);
        
        auto* actor = RE::TESForm::LookupByID<RE::Actor>(record.actorFormID);
        
        if (!actor) {
            WriteToOStimEventsLog("WARNING: Actor not loaded in memory: " + record.actorName, __LINE__);
            failedCleanups++;
            continue;
        }
        
        if (!actor->Is3DLoaded()) {
            WriteToOStimEventsLog("WARNING: Actor 3D not loaded: " + record.actorName, __LINE__);
            failedCleanups++;
            continue;
        }
        
        ApplySpellEffect(record.actorFormID, (record.actorFormID == 0x14), record.isNPCCast, record.systemType);
        
        std::stringstream ss;
        ss << "[SPELL_STATE]|" << systemName << "|" << record.actorName << "|0x" << std::hex << std::uppercase << record.actorFormID 
           << "|" << (record.isNPCCast ? "NPC" : "Player") << "|INACTIVE|" << GetCurrentTimeString();
        WriteToActionsLog(ss.str(), __LINE__);
        
        successfulCleanups++;
        
        WriteToOStimEventsLog("Deactivated " + systemName + " spell for: " + record.actorName, __LINE__);
    }
    
    WriteToOStimEventsLog("----------------------------------------", __LINE__);
    WriteToOStimEventsLog("LOG CLEANUP SUMMARY:", __LINE__);
    WriteToOStimEventsLog("  Actors with active spells: " + std::to_string(actorsWithActiveSpells), __LINE__);
    WriteToOStimEventsLog("  Successful cleanups: " + std::to_string(successfulCleanups), __LINE__);
    WriteToOStimEventsLog("  Failed cleanups: " + std::to_string(failedCleanups), __LINE__);
    WriteToOStimEventsLog("========================================", __LINE__);
}

void CleanupSpellEffectsByFaction() {
    WriteToOStimEventsLog("========================================", __LINE__);
    WriteToOStimEventsLog("FACTION-BASED SPELL CLEANUP INITIATED", __LINE__);
    
    int totalActorsChecked = 0;
    int totalActorsInFactions = 0;
    int totalSuccessfulRemovals = 0;
    int totalFailedRemovals = 0;
    
    std::vector<SpellSystemType> systemsToCheck = {
        SpellSystemType::EmotionalTears,
        SpellSystemType::VampireTears,
        SpellSystemType::BloodyNose
    };
    
    for (const auto& systemType : systemsToCheck) {
        std::string systemName = GetSpellSystemName(systemType);
        
        RE::FormID factionID = GetCachedFactionFormID(systemType);
        
        if (factionID == 0) {
            WriteToOStimEventsLog("ERROR: " + systemName + " Faction FormID is 0, skipping", __LINE__);
            continue;
        }
        
        auto* faction = RE::TESForm::LookupByID<RE::TESFaction>(factionID);
        
        if (!faction) {
            WriteToOStimEventsLog("ERROR: " + systemName + " Faction not found for FormID 0x" + std::to_string(factionID), __LINE__);
            continue;
        }
        
        WriteToOStimEventsLog("Processing " + systemName + " Faction: " + std::string(faction->GetName()), __LINE__);
        
        int actorsChecked = 0;
        int actorsInFaction = 0;
        int successfulRemovals = 0;
        int failedRemovals = 0;
        
        for (const auto& actorInfo : g_sceneActors) {
            actorsChecked++;
            
            auto* actor = RE::TESForm::LookupByID<RE::Actor>(actorInfo.refID);
            if (!actor) {
                WriteToOStimEventsLog("Actor not found: " + actorInfo.name, __LINE__);
                failedRemovals++;
                continue;
            }
            
            int factionRank = GetActorFactionRank(actor, faction);
            
            if (factionRank >= 1) {
                actorsInFaction++;
                
                WriteToOStimEventsLog("Actor in " + systemName + " faction: " + actorInfo.name + " (0x" + std::to_string(actorInfo.refID) + ") - Rank: " + std::to_string(factionRank), __LINE__);
                
                bool isPlayer = (actorInfo.refID == 0x14);
                bool isNPCCast = !isPlayer;
                
                ApplySpellEffect(actorInfo.refID, isPlayer, isNPCCast, systemType);
                
                std::stringstream ss;
                ss << "[SPELL_FACTION_CLEANUP]|" << systemName << "|" << actorInfo.name << "|0x" << std::hex << std::uppercase << actorInfo.refID 
                   << "|" << (isNPCCast ? "NPC" : "Player") << "|REMOVED_VIA_FACTION|" << GetCurrentTimeString();
                WriteToActionsLog(ss.str(), __LINE__);
                
                successfulRemovals++;
                
                WriteToOStimEventsLog(systemName + " spell removal triggered for: " + actorInfo.name, __LINE__);
            }
        }
        
        WriteToOStimEventsLog("----------------------------------------", __LINE__);
        WriteToOStimEventsLog(systemName + " FACTION CLEANUP:", __LINE__);
        WriteToOStimEventsLog("  Actors checked: " + std::to_string(actorsChecked), __LINE__);
        WriteToOStimEventsLog("  Actors in faction: " + std::to_string(actorsInFaction), __LINE__);
        WriteToOStimEventsLog("  Successful removals: " + std::to_string(successfulRemovals), __LINE__);
        WriteToOStimEventsLog("  Failed removals: " + std::to_string(failedRemovals), __LINE__);
        
        totalActorsChecked += actorsChecked;
        totalActorsInFactions += actorsInFaction;
        totalSuccessfulRemovals += successfulRemovals;
        totalFailedRemovals += failedRemovals;
    }
    
    WriteToOStimEventsLog("========================================", __LINE__);
    WriteToOStimEventsLog("TOTAL FACTION CLEANUP SUMMARY:", __LINE__);
    WriteToOStimEventsLog("  Total actors checked: " + std::to_string(totalActorsChecked), __LINE__);
    WriteToOStimEventsLog("  Total actors in factions: " + std::to_string(totalActorsInFactions), __LINE__);
    WriteToOStimEventsLog("  Total successful removals: " + std::to_string(totalSuccessfulRemovals), __LINE__);
    WriteToOStimEventsLog("  Total failed removals: " + std::to_string(totalFailedRemovals), __LINE__);
    WriteToOStimEventsLog("========================================", __LINE__);
}
void CleanupTagBasedEffectsAtSceneStart() {
    WriteToOStimEventsLog("========================================", __LINE__);
    WriteToOStimEventsLog("SCENE START CLEANUP - TAG-BASED EFFECTS", __LINE__);
    
    int totalActorsChecked = 0;
    int totalActorsInFactions = 0;
    int totalSuccessfulRemovals = 0;
    
    std::vector<SpellSystemType> systemsToCheck = {
        SpellSystemType::EmotionalTears,
        SpellSystemType::VampireTears,
        SpellSystemType::BloodyNose
    };
    
    for (const auto& systemType : systemsToCheck) {
        std::string systemName = GetSpellSystemName(systemType);
        
        RE::FormID factionID = GetCachedFactionFormID(systemType);
        
        if (factionID == 0) {
            WriteToOStimEventsLog("Skipping " + systemName + " - Faction not cached", __LINE__);
            continue;
        }
        
        auto* faction = RE::TESForm::LookupByID<RE::TESFaction>(factionID);
        
        if (!faction) {
            WriteToOStimEventsLog("Skipping " + systemName + " - Faction not found", __LINE__);
            continue;
        }
        
        WriteToOStimEventsLog("Checking " + systemName + " faction: " + std::string(faction->GetName()), __LINE__);
        
        int actorsInFaction = 0;
        int successfulRemovals = 0;
        
        for (const auto& actorInfo : g_sceneActors) {
            totalActorsChecked++;

            auto* actor = RE::TESForm::LookupByID<RE::Actor>(actorInfo.refID);
            if (!actor) {
                continue;
            }

            int factionRank = GetActorFactionRank(actor, faction);

            if (factionRank >= 1) {
                actorsInFaction++;
                totalActorsInFactions++;

                WriteToOStimEventsLog("Residual tag-based effect found: " + actorInfo.name + " in " + systemName + " faction (Rank: " + std::to_string(factionRank) + ")", __LINE__);

                bool isPlayer = (actorInfo.refID == 0x14);
                bool isNPCCast = !isPlayer;

                ApplySpellEffect(actorInfo.refID, isPlayer, isNPCCast, systemType);

                std::stringstream ss;
                ss << "[SPELL_CLEANUP_START]|" << systemName << "|" << actorInfo.name << "|0x" << std::hex << std::uppercase << actorInfo.refID
                   << "|" << (isNPCCast ? "NPC" : "Player") << "|REMOVED|" << GetCurrentTimeString();
                WriteToActionsLog(ss.str(), __LINE__);

                successfulRemovals++;
                totalSuccessfulRemovals++;

                WriteToOStimEventsLog("Cleaned " + systemName + " tag-based effect from: " + actorInfo.name, __LINE__);
            }
        }
        
        if (actorsInFaction > 0) {
            WriteToOStimEventsLog(systemName + " cleanup: " + std::to_string(successfulRemovals) + " effects removed", __LINE__);
        }
    }
    
    WriteToOStimEventsLog("========================================", __LINE__);
    WriteToOStimEventsLog("SCENE START CLEANUP SUMMARY:", __LINE__);
    WriteToOStimEventsLog("  Total actors checked: " + std::to_string(totalActorsChecked), __LINE__);
    WriteToOStimEventsLog("  Total residual effects: " + std::to_string(totalActorsInFactions), __LINE__);
    WriteToOStimEventsLog("  Total cleaned: " + std::to_string(totalSuccessfulRemovals), __LINE__);
    WriteToOStimEventsLog("========================================", __LINE__);
}


void ProcessBloodyNoseOrgasmEvent(const std::string& actorName, RE::FormID actorFormID, bool isPlayer, const std::string& gender) {
    bool bloodyNosesEnabled = isPlayer ? g_config.bloodyNosePlayer.bloodyNoses : g_config.bloodyNoseNPC.bloodyNoses;

    if (!bloodyNosesEnabled) {
        return;
    }

    bool genderMatch = isPlayer ?
        (gender == "Male" && g_config.bloodyNosePlayer.bloodyNosesMale) || (gender == "Female" && g_config.bloodyNosePlayer.bloodyNosesFemale) :
        (gender == "Male" && g_config.bloodyNoseNPC.bloodyNosesMale) || (gender == "Female" && g_config.bloodyNoseNPC.bloodyNosesFemale);

    if (!genderMatch) {
        return;
    }

    IncrementBloodyNoseCounter(actorFormID, actorName, isPlayer, gender);
}

// ===== FIXED VAMPIRE TEARS SYSTEM WITH PROPER VAMPIRE DETECTION FOR ORGASM EVENTS =====
void OnOStimOrgasmEventForSpellSystems(const std::string& actorName, RE::FormID actorFormID, bool isPlayer, const std::string& gender) {
    LoadConfiguration();
    CheckVampireTearsPluginAvailability();
    
    bool isVampire = false;
    
    for (const auto& sceneActor : g_sceneActors) {
        if (sceneActor.refID == actorFormID || NormalizeName(sceneActor.name) == NormalizeName(actorName)) {
            isVampire = sceneActor.isVampire;
            WriteToOStimEventsLog("Vampire status from sceneActors: " + actorName + " = " + (isVampire ? "YES" : "NO"), __LINE__);
            break;
        }
    }
    
    if (!isVampire) {
        auto* actor = RE::TESForm::LookupByID<RE::Actor>(actorFormID);
        if (actor) {
            isVampire = IsActorVampire(actor);
            WriteToOStimEventsLog("Vampire status from direct check: " + actorName + " = " + (isVampire ? "YES" : "NO"), __LINE__);
        }
    }
    
    std::vector<SpellSystemType> systemsToCheck;
    
    if (isVampire && g_vampireTearsPluginDetected && 
        g_config.vampireTearsNPC.activeMode && g_config.vampireTearsPlayer.activeMode) {
        systemsToCheck.push_back(SpellSystemType::VampireTears);
        WriteToOStimEventsLog("Vampire detected: " + actorName + " - Using VampireTears system", __LINE__);
    } else {
        systemsToCheck.push_back(SpellSystemType::EmotionalTears);
        if (isVampire) {
            if (!g_vampireTearsPluginDetected) {
                WriteToOStimEventsLog("Vampire detected: " + actorName + " - VampireTears plugin not found, using EmotionalTears", __LINE__);
            } else if (!g_config.vampireTearsNPC.activeMode || !g_config.vampireTearsPlayer.activeMode) {
                WriteToOStimEventsLog("Vampire detected: " + actorName + " - VampireTears ACTIVE_MODE disabled, using EmotionalTears", __LINE__);
            }
        }
    }
    
    for (const auto& systemType : systemsToCheck) {
        bool shouldApplyNPC = false;
        bool shouldApplyPlayer = false;
        int intervalActiveSeconds = 0;
        bool showNotification = false;
        
        if (systemType == SpellSystemType::EmotionalTears) {
            if (isPlayer) {
                if (!g_config.emotionalTearsPlayer.enabled) continue;
                if (gender == "Male" && g_config.emotionalTearsPlayer.male) shouldApplyPlayer = true;
                if (gender == "Female" && g_config.emotionalTearsPlayer.female) shouldApplyPlayer = true;
                intervalActiveSeconds = g_config.emotionalTearsPlayer.intervalActiveSeconds;
                showNotification = g_config.emotionalTearsPlayer.showNotification;
            } else {
                if (!g_config.emotionalTearsNPC.enabled) continue;
                if (gender == "Male" && g_config.emotionalTearsNPC.male) shouldApplyNPC = true;
                if (gender == "Female" && g_config.emotionalTearsNPC.female) shouldApplyNPC = true;
                intervalActiveSeconds = g_config.emotionalTearsNPC.intervalActiveSeconds;
                showNotification = g_config.emotionalTearsNPC.showNotification;
            }
        } else if (systemType == SpellSystemType::VampireTears) {
            if (isPlayer) {
                if (!g_config.vampireTearsPlayer.enabled) continue;
                if (gender == "Male" && g_config.vampireTearsPlayer.male) shouldApplyPlayer = true;
                if (gender == "Female" && g_config.vampireTearsPlayer.female) shouldApplyPlayer = true;
                intervalActiveSeconds = g_config.vampireTearsPlayer.intervalActiveSeconds;
                showNotification = g_config.vampireTearsPlayer.showNotification;
            } else {
                if (!g_config.vampireTearsNPC.enabled) continue;
                if (gender == "Male" && g_config.vampireTearsNPC.male) shouldApplyNPC = true;
                if (gender == "Female" && g_config.vampireTearsNPC.female) shouldApplyNPC = true;
                intervalActiveSeconds = g_config.vampireTearsNPC.intervalActiveSeconds;
                showNotification = g_config.vampireTearsNPC.showNotification;
            }
        }
        
        if (isPlayer && shouldApplyPlayer) {
            if (!CanApplySpellEffect(actorFormID, false, systemType, false)) {
                std::string systemName = GetSpellSystemName(systemType);
                WriteToOStimEventsLog("EVENT Player " + systemName + " blocked: Actor already has active effect", __LINE__);
                continue;
            }
            
            ApplySpellEffect(actorFormID, true, false, systemType);
            RegisterActiveEffect(actorFormID, actorName, true, false, gender, intervalActiveSeconds, false, systemType);
            
            std::string systemName = GetSpellSystemName(systemType);
            WriteToOStimEventsLog("EVENT Player " + systemName + " activated for: " + actorName, __LINE__);
            
            if (g_config.notification.enabled && showNotification) {
                std::string msg = "ORisk-and-Reward - " + actorName + " " + systemName + " activated";
                RE::DebugNotification(msg.c_str());
            }
        } else if (!isPlayer && shouldApplyNPC) {
            if (!CanApplySpellEffect(actorFormID, true, systemType, false)) {
                std::string systemName = GetSpellSystemName(systemType);
                WriteToOStimEventsLog("EVENT NPC " + systemName + " blocked: Actor already has active effect", __LINE__);
                continue;
            }
            
            ApplySpellEffect(actorFormID, false, true, systemType);
            RegisterActiveEffect(actorFormID, actorName, false, true, gender, intervalActiveSeconds, false, systemType);
            
            std::string systemName = GetSpellSystemName(systemType);
            WriteToOStimEventsLog("EVENT NPC " + systemName + " activated for: " + actorName, __LINE__);
            
            if (g_config.notification.enabled && showNotification) {
                std::string msg = "ORisk-and-Reward - " + actorName + " " + systemName + " activated";
                RE::DebugNotification(msg.c_str());
            }
        }
    }
}

// ===== FIXED VAMPIRE TEARS SYSTEM WITH PROPER VAMPIRE DETECTION FOR TAG-BASED ACTIVATION =====
void CheckAnimationTagsForSingleActor(const ActorInfo& actorInfo, const std::string& animationName, const std::vector<std::string>& detectedTags) {
    if (!IsInOStimScene()) {
        return;
    }
    
    LoadConfiguration();
    
    bool isPlayer = (actorInfo.refID == 0x14);
    
    bool isVampire = actorInfo.isVampire;
    
    std::vector<SpellSystemType> systemsToCheck;
    
    if (isVampire && g_vampireTearsPluginDetected) {
        systemsToCheck.push_back(SpellSystemType::VampireTears);
    } else {
        systemsToCheck.push_back(SpellSystemType::EmotionalTears);
    }
    
    systemsToCheck.push_back(SpellSystemType::BloodyNose);
    
    for (const auto& systemType : systemsToCheck) {
        bool enabled = false;
        bool tagsEnabled = false;
        std::string tagsList;
        std::string tagsGender;
        bool showNotification = false;
        
        if (systemType == SpellSystemType::EmotionalTears) {
            if (isPlayer) {
                enabled = g_config.emotionalTearsPlayer.enabled;
                tagsEnabled = g_config.emotionalTearsPlayer.tagsNameAnimationEnabled;
                tagsList = g_config.emotionalTearsPlayer.tagsNameAnimationList;
                tagsGender = g_config.emotionalTearsPlayer.tagsNameAnimationGender;
                showNotification = g_config.emotionalTearsPlayer.showNotification;
            } else {
                enabled = g_config.emotionalTearsNPC.enabled;
                tagsEnabled = g_config.emotionalTearsNPC.tagsNameAnimationEnabled;
                tagsList = g_config.emotionalTearsNPC.tagsNameAnimationList;
                tagsGender = g_config.emotionalTearsNPC.tagsNameAnimationGender;
                showNotification = g_config.emotionalTearsNPC.showNotification;
            }
        } else if (systemType == SpellSystemType::VampireTears) {
            if (isPlayer) {
                enabled = g_config.vampireTearsPlayer.enabled;
                tagsEnabled = g_config.vampireTearsPlayer.tagsNameAnimationEnabled;
                tagsList = g_config.vampireTearsPlayer.tagsNameAnimationList;
                tagsGender = g_config.vampireTearsPlayer.tagsNameAnimationGender;
                showNotification = g_config.vampireTearsPlayer.showNotification;
            } else {
                enabled = g_config.vampireTearsNPC.enabled;
                tagsEnabled = g_config.vampireTearsNPC.tagsNameAnimationEnabled;
                tagsList = g_config.vampireTearsNPC.tagsNameAnimationList;
                tagsGender = g_config.vampireTearsNPC.tagsNameAnimationGender;
                showNotification = g_config.vampireTearsNPC.showNotification;
            }
        } else if (systemType == SpellSystemType::BloodyNose) {
            if (isPlayer) {
                enabled = g_config.bloodyNosePlayer.enabled;
                tagsEnabled = g_config.bloodyNosePlayer.tagsNameAnimationEnabled;
                tagsList = g_config.bloodyNosePlayer.tagsNameAnimationList;
                tagsGender = g_config.bloodyNosePlayer.tagsNameAnimationGender;
                showNotification = g_config.bloodyNosePlayer.showNotification;
            } else {
                enabled = g_config.bloodyNoseNPC.enabled;
                tagsEnabled = g_config.bloodyNoseNPC.tagsNameAnimationEnabled;
                tagsList = g_config.bloodyNoseNPC.tagsNameAnimationList;
                tagsGender = g_config.bloodyNoseNPC.tagsNameAnimationGender;
                showNotification = g_config.bloodyNoseNPC.showNotification;
            }
        }
        
        if (!enabled || !tagsEnabled) {
            continue;
        }
        
        if (tagsList.empty()) {
            continue;
        }
        
        bool matchesTags = MatchesConfiguredTags(animationName, detectedTags, tagsList);
        bool genderMatches = MatchesGenderFilter(actorInfo.gender, tagsGender);
        
        if (matchesTags && genderMatches) {
            if (!CanApplySpellEffect(actorInfo.refID, !isPlayer, systemType, true)) {
                std::string systemName = GetSpellSystemName(systemType);
                WriteToOStimEventsLog("TAG " + systemName + " blocked: Actor already has active effect", __LINE__);
                continue;
            }
            
            ApplySpellEffect(actorInfo.refID, isPlayer, !isPlayer, systemType);
            RegisterActiveEffect(actorInfo.refID, actorInfo.name, isPlayer, !isPlayer, actorInfo.gender, 0, true, systemType);
            
            std::string systemName = GetSpellSystemName(systemType);
            WriteToOStimEventsLog("Tag-Based " + systemName + " activated for: " + actorInfo.name, __LINE__);
            
            if (g_config.notification.enabled && showNotification) {
                std::string msg = "ORisk-and-Reward - " + actorInfo.name + " " + systemName + " activated (tag)";
                RE::DebugNotification(msg.c_str());
            }
        }
    }
}

// ===== FIXED VAMPIRE TEARS SYSTEM WITH PROPER VAMPIRE DETECTION FOR TAG-BASED SYSTEM =====
void CheckAnimationTagsForSpellSystems(const std::string& animationName, const std::vector<std::string>& detectedTags) {
    if (!IsInOStimScene()) {
        return;
    }
    
    if (animationName == g_lastProcessedAnimationForTags) {
        return;
    }
    
    g_lastProcessedAnimationForTags = animationName;
    
    LoadConfiguration();
    CheckVampireTearsPluginAvailability();
    
    WriteToOStimEventsLog("========================================", __LINE__);
    WriteToOStimEventsLog("CHECKING ANIMATION TAGS FOR ALL SPELL SYSTEMS", __LINE__);
    WriteToOStimEventsLog("Animation: " + animationName, __LINE__);
    
    for (auto& actorInfo : g_sceneActors) {
        bool isPlayer = (actorInfo.refID == 0x14);
        bool isVampire = actorInfo.isVampire;
        
        std::vector<SpellSystemType> systemsToCheck;
        
        if (isVampire && g_vampireTearsPluginDetected && 
            g_config.vampireTearsNPC.activeMode && g_config.vampireTearsPlayer.activeMode) {
            systemsToCheck.push_back(SpellSystemType::VampireTears);
        } else {
            systemsToCheck.push_back(SpellSystemType::EmotionalTears);
        }
        
        systemsToCheck.push_back(SpellSystemType::BloodyNose);
        
        for (const auto& systemType : systemsToCheck) {
            bool enabled = false;
            bool tagsEnabled = false;
            std::string tagsList;
            std::string tagsGender;
            bool showNotification = false;
            
            if (systemType == SpellSystemType::EmotionalTears) {
                if (isPlayer) {
                    enabled = g_config.emotionalTearsPlayer.enabled;
                    tagsEnabled = g_config.emotionalTearsPlayer.tagsNameAnimationEnabled;
                    tagsList = g_config.emotionalTearsPlayer.tagsNameAnimationList;
                    tagsGender = g_config.emotionalTearsPlayer.tagsNameAnimationGender;
                    showNotification = g_config.emotionalTearsPlayer.showNotification;
                } else {
                    enabled = g_config.emotionalTearsNPC.enabled;
                    tagsEnabled = g_config.emotionalTearsNPC.tagsNameAnimationEnabled;
                    tagsList = g_config.emotionalTearsNPC.tagsNameAnimationList;
                    tagsGender = g_config.emotionalTearsNPC.tagsNameAnimationGender;
                    showNotification = g_config.emotionalTearsNPC.showNotification;
                }
            } else if (systemType == SpellSystemType::VampireTears) {
                if (isPlayer) {
                    enabled = g_config.vampireTearsPlayer.enabled;
                    tagsEnabled = g_config.vampireTearsPlayer.tagsNameAnimationEnabled;
                    tagsList = g_config.vampireTearsPlayer.tagsNameAnimationList;
                    tagsGender = g_config.vampireTearsPlayer.tagsNameAnimationGender;
                    showNotification = g_config.vampireTearsPlayer.showNotification;
                } else {
                    enabled = g_config.vampireTearsNPC.enabled;
                    tagsEnabled = g_config.vampireTearsNPC.tagsNameAnimationEnabled;
                    tagsList = g_config.vampireTearsNPC.tagsNameAnimationList;
                    tagsGender = g_config.vampireTearsNPC.tagsNameAnimationGender;
                    showNotification = g_config.vampireTearsNPC.showNotification;
                }
            } else if (systemType == SpellSystemType::BloodyNose) {
                if (isPlayer) {
                    enabled = g_config.bloodyNosePlayer.enabled;
                    tagsEnabled = g_config.bloodyNosePlayer.tagsNameAnimationEnabled;
                    tagsList = g_config.bloodyNosePlayer.tagsNameAnimationList;
                    tagsGender = g_config.bloodyNosePlayer.tagsNameAnimationGender;
                    showNotification = g_config.bloodyNosePlayer.showNotification;
                } else {
                    enabled = g_config.bloodyNoseNPC.enabled;
                    tagsEnabled = g_config.bloodyNoseNPC.tagsNameAnimationEnabled;
                    tagsList = g_config.bloodyNoseNPC.tagsNameAnimationList;
                    tagsGender = g_config.bloodyNoseNPC.tagsNameAnimationGender;
                    showNotification = g_config.bloodyNoseNPC.showNotification;
                }
            }
            
            if (!enabled || !tagsEnabled) {
                continue;
            }
            
            if (tagsList.empty()) {
                continue;
            }
            
            bool matchesTags = MatchesConfiguredTags(animationName, detectedTags, tagsList);
            bool genderMatches = MatchesGenderFilter(actorInfo.gender, tagsGender);
            
            if (matchesTags && genderMatches) {
                bool alreadyActiveWithSameTags = false;
                for (const auto& effect : g_activeSpellEffects) {
                    if (effect.actorFormID == actorInfo.refID && effect.isNPCCast == !isPlayer && effect.isTagBased && effect.systemType == systemType) {
                        if (effect.activeAnimationName == animationName) {
                            alreadyActiveWithSameTags = true;
                            break;
                        }
                    }
                }
                
                if (alreadyActiveWithSameTags) {
                    continue;
                }
                
                if (!CanApplySpellEffect(actorInfo.refID, !isPlayer, systemType, true)) {
                    std::string systemName = GetSpellSystemName(systemType);
                    WriteToOStimEventsLog("TAG " + systemName + " blocked for " + actorInfo.name + ": Actor already has active effect", __LINE__);
                    continue;
                }
                
                ApplySpellEffect(actorInfo.refID, isPlayer, !isPlayer, systemType);
                RegisterActiveEffect(actorInfo.refID, actorInfo.name, isPlayer, !isPlayer, actorInfo.gender, 0, true, systemType);
                
                std::string systemName = GetSpellSystemName(systemType);
                WriteToOStimEventsLog("Tag-Based " + systemName + " activated for: " + actorInfo.name, __LINE__);
                
                if (g_config.notification.enabled && showNotification) {
                    std::string msg = "ORisk-and-Reward - " + actorInfo.name + " " + systemName + " activated (tag)";
                    RE::DebugNotification(msg.c_str());
                }
            }
        }
    }
    
    WriteToOStimEventsLog("========================================", __LINE__);
}

void RemoveTagBasedSpellEffects() {
    std::lock_guard<std::mutex> lock(g_effectsMutex);
    
    g_lastProcessedAnimationForTags = "";
    
    LoadConfiguration();
    
    for (auto it = g_activeSpellEffects.begin(); it != g_activeSpellEffects.end();) {
        if (it->isTagBased && !it->spellDeactivated) {
            bool stillMatches = false;
            std::string tagsList;
            
            if (it->systemType == SpellSystemType::EmotionalTears) {
                if (it->isNPCCast && g_config.emotionalTearsNPC.enabled && g_config.emotionalTearsNPC.tagsNameAnimationEnabled) {
                    tagsList = g_config.emotionalTearsNPC.tagsNameAnimationList;
                } else if (!it->isNPCCast && g_config.emotionalTearsPlayer.enabled && g_config.emotionalTearsPlayer.tagsNameAnimationEnabled) {
                    tagsList = g_config.emotionalTearsPlayer.tagsNameAnimationList;
                }
            } else if (it->systemType == SpellSystemType::VampireTears) {
                if (it->isNPCCast && g_config.vampireTearsNPC.enabled && g_config.vampireTearsNPC.tagsNameAnimationEnabled) {
                    tagsList = g_config.vampireTearsNPC.tagsNameAnimationList;
                } else if (!it->isNPCCast && g_config.vampireTearsPlayer.enabled && g_config.vampireTearsPlayer.tagsNameAnimationEnabled) {
                    tagsList = g_config.vampireTearsPlayer.tagsNameAnimationList;
                }
            } else if (it->systemType == SpellSystemType::BloodyNose) {
                if (it->isNPCCast && g_config.bloodyNoseNPC.enabled && g_config.bloodyNoseNPC.tagsNameAnimationEnabled) {
                    tagsList = g_config.bloodyNoseNPC.tagsNameAnimationList;
                } else if (!it->isNPCCast && g_config.bloodyNosePlayer.enabled && g_config.bloodyNosePlayer.tagsNameAnimationEnabled) {
                    tagsList = g_config.bloodyNosePlayer.tagsNameAnimationList;
                }
            }
            
            if (!tagsList.empty()) {
                stillMatches = AnimationStillHasConfiguredTags(
                    g_currentAnimationInfo.animationName, 
                    g_currentAnimationInfo.implicitTags, 
                    tagsList
                );
            }
            
            if (stillMatches) {
                std::string systemName = GetSpellSystemName(it->systemType);
                WriteToOStimEventsLog("Tag-Based " + systemName + " MAINTAINED for: " + it->actorName + " (tags still present)", __LINE__);
                ++it;
                continue;
            }
            
            ApplySpellEffect(it->actorFormID, it->isPlayer, it->isNPCCast, it->systemType);
            
            std::string systemName = GetSpellSystemName(it->systemType);
            
            std::stringstream ss;
            ss << "[SPELL_STATE]|" << systemName << "|" << it->actorName << "|0x" << std::hex << std::uppercase << it->actorFormID 
               << "|" << (it->isNPCCast ? "NPC" : "Player") << "|INACTIVE|" << GetCurrentTimeString();
            WriteToActionsLog(ss.str(), __LINE__);
            
            it->spellDeactivated = true;
            
            WriteToOStimEventsLog("Tag-Based " + systemName + " deactivated for: " + it->actorName, __LINE__);
            
            it = g_activeSpellEffects.erase(it);
        } else {
            ++it;
        }
    }
}

void ProcessPendingSpellCasts() {
    std::lock_guard<std::mutex> lock(g_pendingSpellMutex);
    
    if (g_pendingSpellCasts.empty()) {
        return;
    }
    
    auto now = std::chrono::steady_clock::now();
    
    for (auto it = g_pendingSpellCasts.begin(); it != g_pendingSpellCasts.end();) {
        if (now < it->nextRetryTime) {
            ++it;
            continue;
        }
        
        if (IsActorReadyForSpell(it->rawActorFormID)) {
            auto* actor = RE::TESForm::LookupByID<RE::Actor>(it->rawActorFormID);
            if (actor && actor->Is3DLoaded()) {
                ApplySpellEffect(it->rawActorFormID, it->isPlayer, it->isNPCCast, it->systemType);
                RegisterActiveEffect(
                    it->rawActorFormID, 
                    it->actorName, 
                    it->isPlayer, 
                    it->isNPCCast, 
                    it->gender, 
                    it->durationSeconds, 
                    it->isTagBased,
                    it->systemType
                );
                
                WriteToActionsLog("Retry successful for: " + it->actorName, __LINE__);
                
                it = g_pendingSpellCasts.erase(it);
            } else {
                it->attemptsRemaining--;
                
                if (it->attemptsRemaining <= 0) {
                    WriteToActionsLog("Retry exhausted for: " + it->actorName + " - actor never became ready", __LINE__);
                    it = g_pendingSpellCasts.erase(it);
                } else {
                    it->nextRetryTime = now + std::chrono::milliseconds(500);
                    WriteToActionsLog("Retry attempt failed for: " + it->actorName + " - " + std::to_string(it->attemptsRemaining) + " attempts remaining", __LINE__);
                    ++it;
                }
            }
        } else {
            it->attemptsRemaining--;
            
            if (it->attemptsRemaining <= 0) {
                WriteToActionsLog("Retry exhausted for: " + it->actorName + " - max attempts reached", __LINE__);
                it = g_pendingSpellCasts.erase(it);
            } else {
                it->nextRetryTime = now + std::chrono::milliseconds(500);
                WriteToActionsLog("Actor not ready, scheduling retry for: " + it->actorName + " - " + std::to_string(it->attemptsRemaining) + " attempts remaining", __LINE__);
                ++it;
            }
        }
    }
}

void GiveAttributesEventReward(int amount, const std::string& actorName, const std::string& gender) {
    auto* player = RE::PlayerCharacter::GetSingleton();
    if (!player) return;
    
    auto* actorValueOwner = player->AsActorValueOwner();
    if (!actorValueOwner) return;
    
    float fAmount = static_cast<float>(amount);
    actorValueOwner->RestoreActorValue(RE::ACTOR_VALUE_MODIFIER::kDamage, RE::ActorValue::kHealth, fAmount);
    actorValueOwner->RestoreActorValue(RE::ACTOR_VALUE_MODIFIER::kDamage, RE::ActorValue::kMagicka, fAmount);
    actorValueOwner->RestoreActorValue(RE::ACTOR_VALUE_MODIFIER::kDamage, RE::ActorValue::kStamina, fAmount);
    
    if (g_config.notification.enabled && g_config.attributesEvent.showNotification) {
        std::string msg = "ORisk-and-Reward - " + actorName + " orgasm restored " + std::to_string(amount) + " attributes";
        RE::DebugNotification(msg.c_str());
    }
    
    WriteToActionsLog("Player received " + std::to_string(amount) + 
                      " attributes from " + actorName + " (" + gender + ") orgasm event", __LINE__);
}

void GiveItemEventReward(const std::string& itemID, const std::string& pluginName, int amount, 
                         const std::string& itemName, const std::string& actorName, const std::string& gender) {
    auto* player = RE::PlayerCharacter::GetSingleton();
    if (!player) return;
    
    RE::FormID formID = GetFormIDFromPlugin(pluginName, itemID);
    if (formID == 0) {
        WriteToActionsLog("ERROR: FormID not resolved for " + itemName, __LINE__);
        return;
    }
    
    auto* itemForm = RE::TESForm::LookupByID(formID);
    if (!itemForm) {
        WriteToActionsLog("ERROR: Item form not found for " + itemName, __LINE__);
        return;
    }
    
    auto* item = itemForm->As<RE::TESBoundObject>();
    if (!item) {
        WriteToActionsLog("ERROR: Item is not TESBoundObject for " + itemName, __LINE__);
        return;
    }
    
    player->AddObjectToContainer(item, nullptr, amount, nullptr);
    
    WriteToActionsLog("Player received " + std::to_string(amount) + " " + itemName + 
                      " from " + actorName + " (" + gender + ") orgasm event", __LINE__);
}

void ProcessOrgasmEventRewards(const std::string& actorName, RE::FormID actorFormID, 
                                bool isPlayer, const std::string& gender) {
    
    if (actorName.empty() || actorFormID == 0 || gender.empty()) return;
    
    WriteToOStimEventsLog("========================================", __LINE__);
    WriteToOStimEventsLog("PROCESSING ORGASM EVENT REWARDS", __LINE__);
    WriteToOStimEventsLog("Actor: " + actorName, __LINE__);
    WriteToOStimEventsLog("Gender: " + gender, __LINE__);
    WriteToOStimEventsLog("Is Player: " + std::string(isPlayer ? "Yes" : "No"), __LINE__);
    
    if (g_config.attributesEvent.enabled) {
        bool shouldApply = false;
        if (gender == "Male" && g_config.attributesEvent.male) shouldApply = true;
        if (gender == "Female" && g_config.attributesEvent.female) shouldApply = true;
        
        if (shouldApply) {
            GiveAttributesEventReward(g_config.attributesEvent.restorationAmount, actorName, gender);
            WriteToOStimEventsLog("Attributes reward applied", __LINE__);
        } else {
            WriteToOStimEventsLog("Attributes reward skipped (gender mismatch)", __LINE__);
        }
    }
    
    if (g_config.item1Event.enabled && g_config.item1Event.plugin != "none") {
        bool shouldApply = false;
        if (gender == "Male" && g_config.item1Event.male) shouldApply = true;
        if (gender == "Female" && g_config.item1Event.female) shouldApply = true;
        
        if (shouldApply) {
            GiveItemEventReward(g_config.item1Event.id, g_config.item1Event.plugin, 
                               g_config.item1Event.amount, g_config.item1Event.itemName, 
                               actorName, gender);
            
            if (g_config.notification.enabled && g_config.item1Event.showNotification) {
                std::string msg = "ORisk-and-Reward - " + actorName + " gave you " + 
                                 std::to_string(g_config.item1Event.amount) + " " + 
                                 g_config.item1Event.itemName;
                RE::DebugNotification(msg.c_str());
            }
            WriteToOStimEventsLog("Item1 reward applied", __LINE__);
        } else {
            WriteToOStimEventsLog("Item1 reward skipped (gender mismatch)", __LINE__);
        }
    }
    
    if (g_config.item2Event.enabled && g_config.item2Event.plugin != "none") {
        bool shouldApply = false;
        if (gender == "Male" && g_config.item2Event.male) shouldApply = true;
        if (gender == "Female" && g_config.item2Event.female) shouldApply = true;
        
        if (shouldApply) {
            GiveItemEventReward(g_config.item2Event.id, g_config.item2Event.plugin, 
                               g_config.item2Event.amount, g_config.item2Event.itemName, 
                               actorName, gender);
            
            if (g_config.notification.enabled && g_config.item2Event.showNotification) {
                std::string msg = "ORisk-and-Reward - " + actorName + " gave you " + 
                                 std::to_string(g_config.item2Event.amount) + " " + 
                                 g_config.item2Event.itemName;
                RE::DebugNotification(msg.c_str());
            }
            WriteToOStimEventsLog("Item2 reward applied", __LINE__);
        } else {
            WriteToOStimEventsLog("Item2 reward skipped (gender mismatch)", __LINE__);
        }
    }
    
    bool milkAwarded = false;
    
    if (!isPlayer && g_config.milkEthelEvent.enabled && g_ethelPluginExists) {
        RE::FormID ethelFormID = GetFormIDFromPlugin(g_config.milkEthelEvent.pluginNPC, 
                                                      g_config.milkEthelEvent.npc);
        
        auto* actorBase = RE::TESForm::LookupByID<RE::TESNPC>(actorFormID);
        RE::FormID baseFormID = actorBase ? actorBase->GetFormID() : 0;
        
        if (baseFormID == ethelFormID || 
            (baseFormID >> 24) == (ethelFormID >> 24) && 
            (baseFormID & 0x00FFFFFF) == (ethelFormID & 0x00FFFFFF)) {
            
            bool shouldApply = false;
            if (gender == "Male" && g_config.milkEthelEvent.male) shouldApply = true;
            if (gender == "Female" && g_config.milkEthelEvent.female) shouldApply = true;
            
            if (shouldApply) {
                GiveItemEventReward(g_config.milkEthelEvent.id, g_config.milkEthelEvent.pluginItem, 
                                   g_config.milkEthelEvent.amount, "Milk Ethel", actorName, gender);
                
                if (g_config.notification.enabled && g_config.milkEthelEvent.showNotification) {
                    std::string msg = "ORisk-and-Reward - Ethel gave you " + 
                                     std::to_string(g_config.milkEthelEvent.amount) + " Milk Ethel";
                    RE::DebugNotification(msg.c_str());
                }
                WriteToOStimEventsLog("Milk Ethel reward applied (specific NPC Ethel)", __LINE__);
                milkAwarded = true;
            } else {
                WriteToOStimEventsLog("Milk Ethel reward skipped (gender mismatch)", __LINE__);
            }
        }
    }
    
    if (!milkAwarded && !isPlayer && g_config.milkWenchEvent.enabled && g_wenchPluginExists) {
        if (IsActorFromPlugin(actorFormID, g_config.milkWenchEvent.plugin)) {
            bool shouldApply = false;
            if (gender == "Male" && g_config.milkWenchEvent.male) shouldApply = true;
            if (gender == "Female" && g_config.milkWenchEvent.female) shouldApply = true;
            
            if (shouldApply) {
                GiveItemEventReward(g_config.milkWenchEvent.id, g_config.milkWenchEvent.plugin, 
                                   g_config.milkWenchEvent.amount, "Wench Milk", actorName, gender);
                
                if (g_config.notification.enabled && g_config.milkWenchEvent.showNotification) {
                    std::string msg = "ORisk-and-Reward - " + actorName + " gave you " + 
                                     std::to_string(g_config.milkWenchEvent.amount) + " Wench Milk";
                    RE::DebugNotification(msg.c_str());
                }
                WriteToOStimEventsLog("Wench Milk reward applied (NPC from YurianaWench.esp)", __LINE__);
                milkAwarded = true;
            } else {
                WriteToOStimEventsLog("Wench Milk reward skipped (gender mismatch)", __LINE__);
            }
        }
    }
    
    if (!milkAwarded && g_config.milkEvent.enabled && g_wenchPluginExists) {
        bool shouldApply = false;
        if (gender == "Male" && g_config.milkEvent.male) shouldApply = true;
        if (gender == "Female" && g_config.milkEvent.female) shouldApply = true;
        
        if (shouldApply) {
            GiveItemEventReward(g_config.milkEvent.id, g_config.milkEvent.plugin, 
                               g_config.milkEvent.amount, "Milk", actorName, gender);
            
            if (g_config.notification.enabled && g_config.milkEvent.showNotification) {
                std::string msg = "ORisk-and-Reward - " + actorName + " gave you " + 
                                 std::to_string(g_config.milkEvent.amount) + " Milk";
                RE::DebugNotification(msg.c_str());
            }
            WriteToOStimEventsLog("Milk reward applied", __LINE__);
            milkAwarded = true;
        } else {
            WriteToOStimEventsLog("Milk reward skipped (gender mismatch)", __LINE__);
        }
    }
    
    WriteToOStimEventsLog("========================================", __LINE__);
}

void ParseOStimEventFromLine(const std::string& line) {
    if (line.find("[Thread.cpp") != std::string::npos && line.find("changed speed to") != std::string::npos) {
        size_t speedPos = line.find("changed speed to ");
        if (speedPos != std::string::npos) {
            try {
                std::string speedStr = line.substr(speedPos + 17);
                speedStr = speedStr.substr(0, speedStr.find_first_not_of("0123456789"));
                int newSpeed = std::stoi(speedStr);
                
                if (newSpeed != g_currentOStimSpeed) {
                    g_currentOStimSpeed = newSpeed;
                    
                    std::vector<std::string> speedNames = {"Slow", "Medium", "Fast", "Rough"};
                    std::string speedName = (newSpeed >= 0 && newSpeed < static_cast<int>(speedNames.size())) 
                        ? speedNames[newSpeed] : "Unknown";
                    
                    WriteToOStimEventsLog("========================================", __LINE__);
                    WriteToOStimEventsLog("SPEED CHANGE EVENT", __LINE__);
                    WriteToOStimEventsLog("New speed: " + speedName + " (Level " + std::to_string(newSpeed) + ")", __LINE__);
                    WriteToOStimEventsLog("Current animation: " + GetLastAnimation(), __LINE__);
                    WriteToOStimEventsLog("========================================", __LINE__);
                }
            } catch (...) {
            }
        }
    }
    
    if (line.find("[Graph.cpp") != std::string::npos && line.find("applying node metadata") != std::string::npos) {
        size_t metadataPos = line.find("applying node metadata ");
        if (metadataPos != std::string::npos) {
            std::string metadata = line.substr(metadataPos + 23);
            size_t endPos = metadata.find(" to");
            if (endPos != std::string::npos) {
                metadata = metadata.substr(0, endPos);
                
                metadata.erase(0, metadata.find_first_not_of(" \t"));
                metadata.erase(metadata.find_last_not_of(" \t") + 1);
                
                if (!metadata.empty()) {
                    bool alreadyTagged = false;
                    for (const auto& tag : g_currentOStimTags) {
                        if (tag == metadata) {
                            alreadyTagged = true;
                            break;
                        }
                    }
                    
                    if (!alreadyTagged) {
                        g_currentOStimTags.push_back(metadata);
                        
                        WriteToOStimEventsLog("========================================", __LINE__);
                        WriteToOStimEventsLog("TAG DETECTED EVENT", __LINE__);
                        WriteToOStimEventsLog("Tag: " + metadata, __LINE__);
                        WriteToOStimEventsLog("Current animation: " + GetLastAnimation(), __LINE__);
                        
                        std::string allTags = "All current tags: ";
                        for (size_t i = 0; i < g_currentOStimTags.size(); i++) {
                            allTags += g_currentOStimTags[i];
                            if (i < g_currentOStimTags.size() - 1) {
                                allTags += ", ";
                            }
                        }
                        WriteToOStimEventsLog(allTags, __LINE__);
                        WriteToOStimEventsLog("========================================", __LINE__);
                    }
                }
            }
        }
    }
    
    if (line.find("[Thread.cpp:195] thread 0 changed to node") != std::string::npos) {
        g_currentOStimTags.clear();
        g_currentOStimSpeed = 0;
        
        WriteToOStimEventsLog("========================================", __LINE__);
        WriteToOStimEventsLog("ANIMATION CHANGE EVENT", __LINE__);
        WriteToOStimEventsLog("Animation changed - tags and speed reset", __LINE__);
        WriteToOStimEventsLog("New animation: " + GetLastAnimation(), __LINE__);
        WriteToOStimEventsLog("========================================", __LINE__);
    }
}

void ProcessOStimEventData() {
    if (!IsInOStimScene()) {
        return;
    }
    
    GenerateTagsReport();
    
    auto now = std::chrono::steady_clock::now();
    auto elapsed = std::chrono::duration_cast<std::chrono::seconds>(now - g_lastOStimEventCheck).count();
    
    if (elapsed < 5) {
        return;
    }
    
    g_lastOStimEventCheck = now;
    
    if (!g_currentOStimTags.empty() || g_currentOStimSpeed > 0) {
        WriteToOStimEventsLog("========================================", __LINE__);
        WriteToOStimEventsLog("PERIODIC STATUS UPDATE", __LINE__);
        WriteToOStimEventsLog("Current animation: " + GetLastAnimation(), __LINE__);
        WriteToOStimEventsLog("Current speed level: " + std::to_string(g_currentOStimSpeed), __LINE__);
        
        if (!g_currentOStimTags.empty()) {
            std::string tagsStr = "Active tags: ";
            for (size_t i = 0; i < g_currentOStimTags.size(); i++) {
                tagsStr += g_currentOStimTags[i];
                if (i < g_currentOStimTags.size() - 1) {
                    tagsStr += ", ";
                }
            }
            WriteToOStimEventsLog(tagsStr, __LINE__);
        } else {
            WriteToOStimEventsLog("Active tags: None", __LINE__);
        }
        
        WriteToOStimEventsLog("========================================", __LINE__);
    }
}

class OStimModEventSink : public RE::BSTEventSink<SKSE::ModCallbackEvent> {
public:
    static OStimModEventSink& GetSingleton() {
        static OStimModEventSink singleton;
        return singleton;
    }

    RE::BSEventNotifyControl ProcessEvent(const SKSE::ModCallbackEvent* event,
                                          RE::BSTEventSource<SKSE::ModCallbackEvent>*) override {
        if (!event) {
            return RE::BSEventNotifyControl::kContinue;
        }

        std::string eventName = event->eventName.c_str();
        
        if (eventName.find("ostim_") == 0) {
            LogEventBasicInfo(event);
            
            if (eventName == "ostim_prestart" || eventName == "ostim_thread_start") {
                HandleThreadPrestart(event);
            }
            else if (eventName.find("ostim_scenechanged_") == 0) {
                HandleSceneChange(event);
            }
            else if (eventName == "ostim_thread_scenechanged") {
                HandleThreadSceneChange(event);
            }
            else if (eventName == "ostim_thread_speedchanged") {
                HandleSpeedChange(event);
            }
            else if (eventName == "ostim_orgasm" || eventName == "ostim_actor_orgasm") {
                HandleOrgasm(event);
            }
            else if (eventName == "ostim_end" || eventName == "ostim_totalend" || eventName == "ostim_thread_end") {
                HandleThreadEnd(event);
            }
            
            auto tags = TagAnalyzer::ExtractTagsFromEventData(event);
            if (!tags.empty()) {
                LogDetectedTags(tags, eventName);
            }
        }
        
        return RE::BSEventNotifyControl::kContinue;
    }

private:
    void LogEventBasicInfo(const SKSE::ModCallbackEvent* event) {
        std::string eventName = event->eventName.c_str();
        
        WriteToOStimEventsLog("========================================", __LINE__);
        WriteToOStimEventsLog("OSTIM MOD EVENT RECEIVED", __LINE__);
        WriteToOStimEventsLog("Event Name: " + eventName, __LINE__);
        
        std::string strArg = (event->strArg.c_str() != nullptr && strlen(event->strArg.c_str()) > 0) 
            ? std::string(event->strArg.c_str()) : "(null)";
        WriteToOStimEventsLog("String Argument: " + strArg, __LINE__);
        WriteToOStimEventsLog("Numeric Argument: " + std::to_string(event->numArg), __LINE__);
        
        if (event->sender) {
            auto* actor = event->sender->As<RE::Actor>();
            if (actor) {
                auto* base = actor->GetActorBase();
                if (base) {
                    WriteToOStimEventsLog("Sender Actor: " + std::string(base->GetName()), __LINE__);
                } else {
                    WriteToOStimEventsLog("Sender Actor: (no base)", __LINE__);
                }
            } else {
                WriteToOStimEventsLog("Sender: (not an actor)", __LINE__);
            }
        } else {
            WriteToOStimEventsLog("Sender: (null)", __LINE__);
        }
        WriteToOStimEventsLog("========================================", __LINE__);
        
        if (strArg != "(null)" && !strArg.empty()) {
            WriteToOStimEventsLog("JSON DATA DETECTED IN EVENT: " + eventName, __LINE__);
            WriteToOStimEventsLog("JSON Content: " + strArg, __LINE__);
        }
    }
    
    void HandleThreadPrestart(const SKSE::ModCallbackEvent* event) {
        std::string eventName = event->eventName.c_str();
        
        if (!IsInOStimScene()) {
            WriteToOStimEventsLog("========================================", __LINE__);
            WriteToOStimEventsLog("THREAD PRESTART DETECTED - BUILDING NPC CACHE", __LINE__);
            WriteToOStimEventsLog("Event Name: " + eventName, __LINE__);
            
            BuildNPCsCacheForScene();
            
            WriteToOStimEventsLog("NPC Cache ready for scene start", __LINE__);
            WriteToOStimEventsLog("========================================", __LINE__);
        }
    }
    
    void HandleSceneChange(const SKSE::ModCallbackEvent* event) {
        std::string eventName = event->eventName.c_str();
        
        if (!IsInOStimScene()) {
            WriteToOStimEventsLog("========================================", __LINE__);
            WriteToOStimEventsLog("THREAD START DETECTED (via ostim_scenechanged_)", __LINE__);
            WriteToOStimEventsLog("Event Name: " + eventName, __LINE__);
            
            if (eventName.find("ostim_scenechanged_") == 0) {
                std::string animName = eventName.substr(19);
                WriteToOStimEventsLog("Animation: " + animName, __LINE__);
                AnalyzeAnimationForTags(animName);
            }
            
            WriteToOStimEventsLog("========================================", __LINE__);
        }
    }
    
    void HandleThreadSceneChange(const SKSE::ModCallbackEvent* event) {
        WriteToOStimEventsLog("========================================", __LINE__);
        WriteToOStimEventsLog("ANIMATION CHANGE FROM MOD EVENT", __LINE__);
        
        std::string newAnimationName;
        if (event->strArg.c_str() != nullptr && strlen(event->strArg.c_str()) > 0) {
            newAnimationName = std::string(event->strArg.c_str());
            WriteToOStimEventsLog("New Animation: " + newAnimationName, __LINE__);
            
            AnalyzeAnimationForTags(newAnimationName);
        }
        
        LoadConfiguration();
        
        bool anyTagsEnabled = false;
        
        if (g_config.emotionalTearsNPC.enabled && g_config.emotionalTearsNPC.tagsNameAnimationEnabled) anyTagsEnabled = true;
        if (g_config.emotionalTearsPlayer.enabled && g_config.emotionalTearsPlayer.tagsNameAnimationEnabled) anyTagsEnabled = true;
        if (g_config.vampireTearsNPC.enabled && g_config.vampireTearsNPC.tagsNameAnimationEnabled) anyTagsEnabled = true;
        if (g_config.vampireTearsPlayer.enabled && g_config.vampireTearsPlayer.tagsNameAnimationEnabled) anyTagsEnabled = true;
        if (g_config.bloodyNoseNPC.enabled && g_config.bloodyNoseNPC.tagsNameAnimationEnabled) anyTagsEnabled = true;
        if (g_config.bloodyNosePlayer.enabled && g_config.bloodyNosePlayer.tagsNameAnimationEnabled) anyTagsEnabled = true;
        
        if (!anyTagsEnabled) {
            WriteToOStimEventsLog("Tag-based spell systems disabled in config - skipping check", __LINE__);
            WriteToOStimEventsLog("Thread ID: " + std::to_string(static_cast<int>(event->numArg)), __LINE__);
            WriteToOStimEventsLog("========================================", __LINE__);
            return;
        }
        
        bool newAnimationMatchesAny = false;
        
        if (!newAnimationName.empty()) {
            std::vector<std::string> tagsListsToCheck;
            
            if (g_config.emotionalTearsNPC.enabled && g_config.emotionalTearsNPC.tagsNameAnimationEnabled) {
                tagsListsToCheck.push_back(g_config.emotionalTearsNPC.tagsNameAnimationList);
            }
            if (g_config.emotionalTearsPlayer.enabled && g_config.emotionalTearsPlayer.tagsNameAnimationEnabled) {
                tagsListsToCheck.push_back(g_config.emotionalTearsPlayer.tagsNameAnimationList);
            }
            if (g_config.vampireTearsNPC.enabled && g_config.vampireTearsNPC.tagsNameAnimationEnabled) {
                tagsListsToCheck.push_back(g_config.vampireTearsNPC.tagsNameAnimationList);
            }
            if (g_config.vampireTearsPlayer.enabled && g_config.vampireTearsPlayer.tagsNameAnimationEnabled) {
                tagsListsToCheck.push_back(g_config.vampireTearsPlayer.tagsNameAnimationList);
            }
            if (g_config.bloodyNoseNPC.enabled && g_config.bloodyNoseNPC.tagsNameAnimationEnabled) {
                tagsListsToCheck.push_back(g_config.bloodyNoseNPC.tagsNameAnimationList);
            }
            if (g_config.bloodyNosePlayer.enabled && g_config.bloodyNosePlayer.tagsNameAnimationEnabled) {
                tagsListsToCheck.push_back(g_config.bloodyNosePlayer.tagsNameAnimationList);
            }
            
            for (const auto& tagsList : tagsListsToCheck) {
                if (!tagsList.empty() && MatchesConfiguredTags(newAnimationName, g_currentAnimationInfo.implicitTags, tagsList)) {
                    newAnimationMatchesAny = true;
                    break;
                }
            }
        }
        
        if (!newAnimationMatchesAny) {
            RemoveTagBasedSpellEffects();
        } else {
            CheckAnimationTagsForSpellSystems(newAnimationName, g_currentAnimationInfo.implicitTags);
        }
        
        WriteToOStimEventsLog("Thread ID: " + std::to_string(static_cast<int>(event->numArg)), __LINE__);
        WriteToOStimEventsLog("========================================", __LINE__);
    }
    
    void HandleSpeedChange(const SKSE::ModCallbackEvent* event) {
        std::string speedStr = (event->strArg.c_str() != nullptr) ? std::string(event->strArg.c_str()) : "0";
        int speed = 0;
        try {
            speed = std::stoi(speedStr);
        } catch (...) {
            speed = static_cast<int>(event->numArg);
        }
        
        std::vector<std::string> speedNames = {"Slow", "Medium", "Fast", "Rough", "Climax"};
        std::string speedName = (speed >= 0 && speed < static_cast<int>(speedNames.size())) ? speedNames[speed] : "Unknown";
        
        WriteToOStimEventsLog("========================================", __LINE__);
        WriteToOStimEventsLog("SPEED CHANGE FROM MOD EVENT", __LINE__);
        WriteToOStimEventsLog("New Speed: " + speedName + " (Level " + std::to_string(speed) + ")", __LINE__);
        WriteToOStimEventsLog("Current Animation: " + GetLastAnimation(), __LINE__);
        
        if (speed >= 3) {
            WriteToOStimEventsLog("Speed Tag: High Intensity", __LINE__);
        } else if (speed >= 2) {
            WriteToOStimEventsLog("Speed Tag: Medium Intensity", __LINE__);
        } else {
            WriteToOStimEventsLog("Speed Tag: Low Intensity", __LINE__);
        }
        
        WriteToOStimEventsLog("========================================", __LINE__);
    }

    void HandleThreadStart(const SKSE::ModCallbackEvent* event) {
        WriteToOStimEventsLog("========================================", __LINE__);
        WriteToOStimEventsLog("OSTIM THREAD START EVENT", __LINE__);
        
        if (event->strArg.c_str() != nullptr && strlen(event->strArg.c_str()) > 0) {
            WriteToOStimEventsLog("Scene ID: " + std::string(event->strArg.c_str()), __LINE__);
        }
        
        WriteToOStimEventsLog("Thread ID: " + std::to_string(static_cast<int>(event->numArg)), __LINE__);
        WriteToOStimEventsLog("Current Animation: " + GetLastAnimation(), __LINE__);
        WriteToOStimEventsLog("========================================", __LINE__);
    }

    void HandleThreadEnd(const SKSE::ModCallbackEvent* event) {
        WriteToOStimEventsLog("========================================", __LINE__);
        WriteToOStimEventsLog("OSTIM THREAD END EVENT RECEIVED", __LINE__);
        WriteToOStimEventsLog("Event Type: " + std::string(event->eventName.c_str()), __LINE__);
        WriteToOStimEventsLog("Thread ID: " + std::to_string(static_cast<int>(event->numArg)), __LINE__);
        
        if (IsInOStimScene()) {
            WriteToOStimEventsLog("OStim scene is currently active - initiating cleanup", __LINE__);
            WriteToOStimEventsLog("Actors in scene: " + std::to_string(g_sceneActors.size()), __LINE__);
            
            std::vector<ActorInfo> sceneActorsCopy = g_sceneActors;
        
            for (const auto& actor : sceneActorsCopy) {
                WriteToOStimEventsLog("Actor to verify: " + actor.name + " (RefID: 0x" + 
                    std::to_string(actor.refID) + ")", __LINE__);
            }
        
            g_sceneEndTime = std::chrono::steady_clock::now();
            g_cleanupPending = true;
            
            WriteToOStimEventsLog("Cleanup scheduled with 1-second delay to avoid OStim collision", __LINE__);
            
            g_currentOStimTags.clear();
            g_currentOStimSpeed = 0;
            g_currentAnimationInfo = AnimationTagInfo{};
            
            SetInOStimScene(false);
            
            g_goldRewardActive = false;
            g_item1RewardActive = false;
            g_item2RewardActive = false;
            g_milkRewardActive = false;
            g_milkWenchRewardActive = false;
            g_milkEthelRewardActive = false;
            g_attributesRestorationActive = false;
            
            g_detectedNPCNames.clear();
            g_npcNameToRefID.clear();
            
            {
                std::lock_guard<std::mutex> lock(g_orgasmMutex);
                g_lastOrgasmTimestamps.clear();
            }
            
            ClearOrgasmCounters();
            ClearBloodyNoseCounters();
            
            {
                std::lock_guard<std::mutex> lock(g_pendingSpellMutex);
                if (!g_pendingSpellCasts.empty()) {
                    WriteToOStimEventsLog("Clearing " + std::to_string(g_pendingSpellCasts.size()) + 
                                         " pending spell casts (scene ended)", __LINE__);
                    g_pendingSpellCasts.clear();
                }
            }
            
            g_sceneActors.clear();
            g_lastProcessedAnimationForTags = "";
            
            WriteToOStimEventsLog("All reward systems stopped", __LINE__);
            WriteToOStimEventsLog("Scene state cleared successfully", __LINE__);
            WriteToOStimEventsLog("g_sceneActors cleared", __LINE__);
            WriteToActionsLog("OStim scene ended via Mod Event - all systems stopped and spell effects deactivated", __LINE__);
            
        } else {
            WriteToOStimEventsLog("Thread end event received but no active scene detected", __LINE__);
        }
        
        WriteToOStimEventsLog("========================================", __LINE__);
    }

    void HandleOrgasm(const SKSE::ModCallbackEvent* event) {
        WriteToOStimEventsLog("========================================", __LINE__);
        WriteToOStimEventsLog("ORGASM EVENT DETECTED", __LINE__);
        WriteToOStimEventsLog("Event Type: " + std::string(event->eventName.c_str()), __LINE__);
        
        std::string actorName = "";
        RE::FormID actorFormID = 0;
        bool isPlayer = false;
        std::string gender = "";
        
        if (event->sender) {
            auto* actor = event->sender->As<RE::Actor>();
            if (actor) {
                auto* base = actor->GetActorBase();
                if (base) {
                    actorName = std::string(base->GetName());
                    actorFormID = actor->GetFormID();
                    gender = base->IsFemale() ? "Female" : "Male";
                    
                    auto* player = RE::PlayerCharacter::GetSingleton();
                    isPlayer = (actor == player);
                    
                    WriteToOStimEventsLog("Actor: " + actorName, __LINE__);
                    WriteToOStimEventsLog("Gender: " + gender, __LINE__);
                    WriteToOStimEventsLog("Is Player: " + std::string(isPlayer ? "Yes" : "No"), __LINE__);
                }
            }
        }
        
        WriteToOStimEventsLog("Current Animation: " + GetLastAnimation(), __LINE__);
        
        if (!actorName.empty() && actorFormID != 0 && !gender.empty()) {
            if (!ShouldProcessOrgasmEvent(actorFormID)) {
                WriteToOStimEventsLog("DUPLICATE ORGASM EVENT IGNORED: " + actorName + " (within 2 seconds)", __LINE__);
                WriteToOStimEventsLog("========================================", __LINE__);
                return;
            }
            
            UpdateOrgasmTimestamp(actorFormID);
            
            IncrementOrgasmCounter(actorFormID, actorName, isPlayer, gender);
            
            ProcessBloodyNoseOrgasmEvent(actorName, actorFormID, isPlayer, gender);
            
            OnOStimOrgasmEventForSpellSystems(actorName, actorFormID, isPlayer, gender);
            ProcessOrgasmEventRewards(actorName, actorFormID, isPlayer, gender);
        }
        
        WriteToOStimEventsLog("========================================", __LINE__);
    }
};

class GameEventProcessor : public RE::BSTEventSink<RE::MenuOpenCloseEvent> {
    GameEventProcessor() = default;
    ~GameEventProcessor() = default;
    GameEventProcessor(const GameEventProcessor&) = delete;
    GameEventProcessor(GameEventProcessor&&) = delete;
    GameEventProcessor& operator=(const GameEventProcessor&) = delete;
    GameEventProcessor& operator=(GameEventProcessor&&) = delete;

public:
    static GameEventProcessor& GetSingleton() {
        static GameEventProcessor singleton;
        return singleton;
    }

    RE::BSEventNotifyControl ProcessEvent(const RE::MenuOpenCloseEvent* event,
                                          RE::BSTEventSource<RE::MenuOpenCloseEvent>*) override {
        if (event) {
            std::stringstream msg;
            msg << "Menu " << event->menuName.c_str() << " " << (event->opening ? "opened" : "closed");
            WriteToActionsLog(msg.str(), __LINE__);
        }
        return RE::BSEventNotifyControl::kContinue;
    }
};

bool DetectSceneEnd(const std::string& line) {
    if (line.find("[Thread.cpp:634] closing thread") != std::string::npos) {
        WriteToAnimationsLog("DETECTED: OStim thread closing", __LINE__);
        return true;
    }
    if (line.find("[ThreadManager.cpp:174] trying to stop thread") != std::string::npos) {
        WriteToAnimationsLog("DETECTED: OStim trying to stop thread", __LINE__);
        return true;
    }
    return false;
}

std::string DetectAnimationChange(const std::string& line) {
    std::string animationName;

    if (line.find("[info]") != std::string::npos &&
        line.find("[Thread.cpp:195] thread 0 changed to node") != std::string::npos) {
        size_t nodePos = line.find("changed to node ");
        if (nodePos != std::string::npos) {
            size_t startPos = nodePos + 16;
            if (startPos < line.length()) {
                animationName = line.substr(startPos);
            }
        }
    } else if (line.find("[info]") != std::string::npos &&
               line.find("[OStimMenu.h:48] UI_TransitionRequest") != std::string::npos) {
        size_t lastOpenBrace = line.rfind('{');
        size_t lastCloseBrace = line.rfind('}');
        if (lastOpenBrace != std::string::npos && lastCloseBrace != std::string::npos &&
            lastCloseBrace > lastOpenBrace) {
            animationName = line.substr(lastOpenBrace + 1, lastCloseBrace - lastOpenBrace - 1);
        }
    }

    if (!animationName.empty()) {
        animationName.erase(animationName.find_last_not_of(" \n\r\t") + 1);
    }

    return animationName;
}

void ProcessNewLine(const std::string& line, const std::string& hashStr) {
    if (line.find("[warning]") != std::string::npos) {
        return;
    }

    if (g_processedLines.find(hashStr) != g_processedLines.end()) {
        return;
    }

    std::string animationName = DetectAnimationChange(line);
    
    DetectNPCNamesFromLine(line);
    
    ParseOStimEventFromLine(line);

    if (DetectSceneEnd(line)) {
        g_processedLines.insert(hashStr);
        if (IsInOStimScene()) {
            WriteToOStimEventsLog("========================================", __LINE__);
            WriteToOStimEventsLog("SCENE END EVENT DETECTED", __LINE__);
            WriteToOStimEventsLog("Actors in scene: " + std::to_string(g_sceneActors.size()), __LINE__);
            
            std::vector<ActorInfo> sceneActorsCopy = g_sceneActors;
            
            for (const auto& actor : sceneActorsCopy) {
                WriteToOStimEventsLog("Actor to verify: " + actor.name + " (RefID: 0x" + 
                    std::to_string(actor.refID) + ")", __LINE__);
            }
            
            g_sceneEndTime = std::chrono::steady_clock::now();
            g_cleanupPending = true;
            
            WriteToOStimEventsLog("Cleanup scheduled with 1-second delay to avoid OStim collision", __LINE__);
            
            g_currentOStimTags.clear();
            g_currentOStimSpeed = 0;
            g_currentAnimationInfo = AnimationTagInfo{};
            
            SetInOStimScene(false);
            
            g_goldRewardActive = false;
            g_item1RewardActive = false;
            g_item2RewardActive = false;
            g_milkRewardActive = false;
            g_milkWenchRewardActive = false;
            g_milkEthelRewardActive = false;
            g_attributesRestorationActive = false;
            
            g_detectedNPCNames.clear();
            g_npcNameToRefID.clear();
            
            {
                std::lock_guard<std::mutex> lock(g_orgasmMutex);
                g_lastOrgasmTimestamps.clear();
            }
            
            ClearOrgasmCounters();
            ClearBloodyNoseCounters();
            
            {
                std::lock_guard<std::mutex> lock(g_pendingSpellMutex);
                if (!g_pendingSpellCasts.empty()) {
                    WriteToOStimEventsLog("Clearing " + std::to_string(g_pendingSpellCasts.size()) + 
                                         " pending spell casts (scene ended)", __LINE__);
                    g_pendingSpellCasts.clear();
                }
            }
            
            g_sceneActors.clear();
            g_lastProcessedAnimationForTags = "";
            
            WriteToOStimEventsLog("OStim scene ended - all event data cleared", __LINE__);
            WriteToOStimEventsLog("g_sceneActors cleared", __LINE__);
            WriteToOStimEventsLog("========================================", __LINE__);
            
            WriteToActionsLog("OStim scene ended - all reward systems stopped and spell effects deactivated", __LINE__);
        }
        WriteToAnimationsLog("OStim scene ended", __LINE__);
        return;
    }

    if (!animationName.empty()) {
        g_processedLines.insert(hashStr);

        if (animationName == GetLastAnimation()) {
            return;
        }

        SetLastAnimation(animationName);
        AnalyzeAnimationForTags(animationName);
        
        if (!IsInOStimScene()) {
            BuildNPCsCacheForScene();
            
            ActorInfo playerInfo = CapturePlayerInfo();
            if (playerInfo.captured) {
                g_sceneActors.push_back(playerInfo);
            }
            
            CleanupTagBasedEffectsAtSceneStart();
            
            SetInOStimScene(true);
            
            WriteToOStimEventsLog("========================================", __LINE__);
            WriteToOStimEventsLog("SCENE START EVENT", __LINE__);
            WriteToOStimEventsLog("New OStim scene started", __LINE__);
            WriteToOStimEventsLog("Starting animation: " + animationName, __LINE__);
            WriteToOStimEventsLog("Initializing event monitoring systems", __LINE__);
            WriteToOStimEventsLog("========================================", __LINE__);
            
            g_currentOStimTags.clear();
            g_currentOStimSpeed = 0;
            g_lastOStimEventCheck = std::chrono::steady_clock::now();
            
            ResolveItemFormIDs();
            
            g_lastGoldRewardTime = std::chrono::steady_clock::now();
            g_goldRewardActive = true;
            g_lastItem1RewardTime = std::chrono::steady_clock::now();
            g_item1RewardActive = true;
            g_lastItem2RewardTime = std::chrono::steady_clock::now();
            g_item2RewardActive = true;
            g_lastMilkRewardTime = std::chrono::steady_clock::now();
            g_milkRewardActive = true;
            g_lastMilkWenchRewardTime = std::chrono::steady_clock::now();
            g_milkWenchRewardActive = true;
            g_lastMilkEthelRewardTime = std::chrono::steady_clock::now();
            g_milkEthelRewardActive = true;
            g_lastAttributesRestorationTime = std::chrono::steady_clock::now();
            g_attributesRestorationActive = true;
            g_lastNPCDetectionCheck = std::chrono::steady_clock::now();
            g_wenchMilkNPCDetected = false;
            g_ethelNPCDetected = false;

            WriteToActionsLog("OStim scene started - all reward systems activated", __LINE__);
        } else {
            RemoveTagBasedSpellEffects();
            CheckAnimationTagsForSpellSystems(animationName, g_currentAnimationInfo.implicitTags);
        }

        if (g_processedLines.size() > 500) {
            g_processedLines.clear();
        }

        std::string formattedAnimation = "{" + animationName + "}";
        WriteToAnimationsLog(formattedAnimation, __LINE__);
    }
}

void ProcessOStimLog() {
    try {
        if (g_isShuttingDown.load()) {
            return;
        }

        if (!g_initialDelayComplete) {
            auto currentTime = std::chrono::steady_clock::now();
            auto elapsedSeconds =
                std::chrono::duration_cast<std::chrono::seconds>(currentTime - g_monitoringStartTime).count();
            if (elapsedSeconds < 5) {
                return;
            } else {
                g_initialDelayComplete = true;
                WriteToAnimationsLog("5-second initial delay complete, starting dual-path OStim.log monitoring",
                                     __LINE__);
            }
        }

        std::vector<fs::path> ostimLogPaths = {g_ostimLogPaths.primary / "OStim.log",
                                               g_ostimLogPaths.secondary / "OStim.log"};

        fs::path activeOStimLogPath;
        bool foundLog = false;

        for (const auto& logPath : ostimLogPaths) {
            if (fs::exists(logPath)) {
                activeOStimLogPath = logPath;
                foundLog = true;
                break;
            }
        }

        if (!foundLog) {
            return;
        }

        size_t currentFileSize = fs::file_size(activeOStimLogPath);
        if (currentFileSize < g_lastFileSize) {
            g_lastOStimLogPosition = 0;
            g_processedLines.clear();
            SetLastAnimation("");
            WriteToAnimationsLog("OStim.log reset detected - restarting monitoring", __LINE__);
        } else if (currentFileSize == g_lastFileSize && g_lastOStimLogPosition > 0) {
            return;
        }

        g_lastFileSize = currentFileSize;

        std::ifstream ostimLog(activeOStimLogPath, std::ios::in);
        if (!ostimLog.is_open()) {
            return;
        }

        if (g_lastOStimLogPosition == 0) {
            ostimLog.seekg(0, std::ios::beg);
        } else {
            ostimLog.seekg(g_lastOStimLogPosition);
        }

        std::string line;
        while (std::getline(ostimLog, line)) {
            size_t lineHash = std::hash<std::string>{}(line);
            std::string hashStr = std::to_string(lineHash);
            ProcessNewLine(line, hashStr);
        }

        g_lastOStimLogPosition = ostimLog.tellg();
        if (ostimLog.eof()) {
            ostimLog.clear();
            ostimLog.seekg(0, std::ios::end);
            g_lastOStimLogPosition = ostimLog.tellg();
        }

        ostimLog.close();

    } catch (const std::exception& e) {
        logger::error("Error processing OStim.log: {}", e.what());
    } catch (...) {
        logger::error("Unknown error processing OStim.log");
    }
}

void FileWatchThreadFunction() {
    WriteToAnimationsLog("File watch thread started using Windows ReadDirectoryChangesW", __LINE__);

    std::vector<fs::path> watchPaths = {g_ostimLogPaths.primary, g_ostimLogPaths.secondary};

    for (const auto& watchPath : watchPaths) {
        if (!fs::exists(watchPath)) {
            continue;
        }

        HANDLE hDir = CreateFileW(watchPath.wstring().c_str(), FILE_LIST_DIRECTORY,
                                  FILE_SHARE_READ | FILE_SHARE_WRITE | FILE_SHARE_DELETE, NULL, OPEN_EXISTING,
                                  FILE_FLAG_BACKUP_SEMANTICS, NULL);

        if (hDir == INVALID_HANDLE_VALUE) {
            continue;
        }

        g_directoryHandle = hDir;

        char buffer[4096];
        DWORD bytesReturned;

        while (g_fileWatchActive && !g_isShuttingDown.load()) {
            BOOL result = ReadDirectoryChangesW(
                hDir, buffer, sizeof(buffer), FALSE,
                FILE_NOTIFY_CHANGE_LAST_WRITE | FILE_NOTIFY_CHANGE_SIZE | FILE_NOTIFY_CHANGE_FILE_NAME, &bytesReturned,
                NULL, NULL);

            if (!result) {
                break;
            }

            if (bytesReturned == 0) {
                continue;
            }

            FILE_NOTIFY_INFORMATION* pNotify = reinterpret_cast<FILE_NOTIFY_INFORMATION*>(buffer);

            do {
                std::wstring filename(pNotify->FileName, pNotify->FileNameLength / sizeof(wchar_t));
                if (filename == L"OStim.log") {
                    ProcessOStimLog();
                }

                if (pNotify->NextEntryOffset == 0) {
                    break;
                }
                pNotify = reinterpret_cast<FILE_NOTIFY_INFORMATION*>(reinterpret_cast<BYTE*>(pNotify) +
                                                                      pNotify->NextEntryOffset);
            } while (true);
        }

        CloseHandle(hDir);
        g_directoryHandle = INVALID_HANDLE_VALUE;

        if (g_fileWatchActive && !g_isShuttingDown.load()) {
            continue;
        } else {
            break;
        }
    }
}

void StartFileWatch() {
    if (!g_fileWatchActive) {
        g_fileWatchActive = true;
        g_fileWatchThread = std::thread(FileWatchThreadFunction);
        WriteToAnimationsLog("File watch system activated", __LINE__);
    }
}

void StopFileWatch() {
    if (g_fileWatchActive) {
        g_fileWatchActive = false;
        if (g_directoryHandle != INVALID_HANDLE_VALUE) {
            CloseHandle(g_directoryHandle);
            g_directoryHandle = INVALID_HANDLE_VALUE;
        }
        if (g_fileWatchThread.joinable()) {
            g_fileWatchThread.join();
        }
    }
}

void MonitoringThreadFunction() {
    WriteToAnimationsLog("Monitoring thread started - Watching OStim.log for animations", __LINE__);
    WriteToAnimationsLog("Monitoring OStim.log on dual paths (Primary & Secondary)", __LINE__);
    WriteToAnimationsLog("Primary: " + g_ostimLogPaths.primary.string(), __LINE__);
    WriteToAnimationsLog("Secondary: " + g_ostimLogPaths.secondary.string(), __LINE__);
    WriteToAnimationsLog("Waiting 5 seconds before starting OStim.log analysis", __LINE__);

    g_monitoringStartTime = std::chrono::steady_clock::now();
    g_initialDelayComplete = false;

    while (g_monitoringActive && !g_isShuttingDown.load()) {
        g_monitorCycles++;
        
        if (g_cleanupPending) {
            auto now = std::chrono::steady_clock::now();
            auto elapsed = std::chrono::duration_cast<std::chrono::seconds>(now - g_sceneEndTime).count();
            
            if (elapsed >= 1) {
                WriteToOStimEventsLog("========================================", __LINE__);
                WriteToOStimEventsLog("EXECUTING DELAYED CLEANUP (1 second after OStim end)", __LINE__);
                WriteToOStimEventsLog("========================================", __LINE__);
                
                CleanupSpellEffectsFromLog();
                CleanupSpellEffectsByFaction();
                DeactivateAllSpellEffects();
                
                g_cleanupPending = false;
                
                WriteToOStimEventsLog("Delayed cleanup completed successfully", __LINE__);
            }
        }
        
        ProcessOStimLog();
        FindAndCacheNPCRefIDs();
        CheckForNearbyNPCs();
        ProcessOStimEventData();
        CheckAndRewardGold();
        CheckAndRewardItem1();
        CheckAndRewardItem2();
        CheckAndRewardMilk();
        CheckAndRewardMilkWench();
        CheckAndRewardMilkEthel();
        CheckExpiredSpellEffects();
        CheckBloodyNoseCounters();
        CheckAndRestoreAttributes();
        ProcessPendingSpellCasts();
        std::this_thread::sleep_for(std::chrono::milliseconds(1000));
    }
}

void StartMonitoringThread() {
    if (!g_monitoringActive) {
        g_monitoringActive = true;
        g_monitorCycles = 0;
        g_lastOStimLogPosition = 0;
        g_lastFileSize = 0;
        g_processedLines.clear();
        SetLastAnimation("");
        g_initialDelayComplete = false;
        SetInOStimScene(false);
        g_goldRewardActive = false;
        g_item1RewardActive = false;
        g_item2RewardActive = false;
        g_milkRewardActive = false;
        g_milkWenchRewardActive = false;
        g_milkEthelRewardActive = false;
        g_attributesRestorationActive = false;
        g_wenchMilkNPCDetected = false;
        g_ethelNPCDetected = false;
        g_capturedYurianaWenchNPC.captured = false;
        g_capturedYurianaWenchNPC.formID = 0;
        g_capturedEthelNPC.captured = false;
        g_capturedEthelNPC.formID = 0;
        g_cachedItemFormIDs.resolved = false;
        g_cachedItemFormIDs.item1 = 0;
        g_cachedItemFormIDs.item2 = 0;
        g_cachedItemFormIDs.milkDawnguard = 0;
        g_cachedItemFormIDs.milkWench = 0;
        g_cachedItemFormIDs.milkEthel = 0;
        g_cachedItemFormIDs.item1Event = 0;
        g_cachedItemFormIDs.item2Event = 0;
        g_cachedItemFormIDs.milkEvent = 0;
        g_cachedItemFormIDs.milkWenchEvent = 0;
        g_cachedItemFormIDs.milkEthelEvent = 0;
        g_cachedSpellFormIDs.resolved = false;
        g_cachedSpellFormIDs.emotionalTearsNPC = 0;
        g_cachedSpellFormIDs.emotionalTearsPlayer = 0;
        g_cachedSpellFormIDs.vampireTearsNPC = 0;
        g_cachedSpellFormIDs.vampireTearsPlayer = 0;
        g_cachedSpellFormIDs.bloodyNoseNPC = 0;
        g_cachedSpellFormIDs.bloodyNosePlayer = 0;
        g_cachedFactionIDs.resolved = false;
        g_cachedFactionIDs.emotionalTearsFaction = 0;
        g_cachedFactionIDs.vampireTearsFaction = 0;
        g_cachedFactionIDs.bloodyNoseFaction = 0;
        g_detectedNPCNames.clear();
        g_npcNameToRefID.clear();
        g_activeSpellEffects.clear();
        g_currentOStimTags.clear();
        g_currentOStimSpeed = 0;
        g_currentAnimationInfo = AnimationTagInfo{};
        g_sceneActors.clear();
        g_lastOrgasmTimestamps.clear();
        g_pendingSpellCasts.clear();
        g_wenchPluginChecked = false;
        g_wenchPluginExists = false;
        g_ethelPluginChecked = false;
        g_ethelPluginExists = false;
        g_lastProcessedAnimationForTags = "";
        ClearOrgasmCounters();
        ClearBloodyNoseCounters();
        CheckVampireTearsPluginAvailability();
        g_monitorThread = std::thread(MonitoringThreadFunction);

        WriteToAnimationsLog("MONITORING SYSTEM ACTIVATED", __LINE__);
    }
}

void StopMonitoringThread() {
    if (g_monitoringActive) {
        g_monitoringActive = false;
        if (g_monitorThread.joinable()) {
            g_monitorThread.join();
        }
    }
}

void CheckAndRewardGold() {
    LoadConfiguration();

    if (!g_config.gold.enabled || !IsInOStimScene() || GetLastAnimation().empty()) {
        return;
    }

    auto now = std::chrono::steady_clock::now();
    auto elapsed = std::chrono::duration_cast<std::chrono::seconds>(now - g_lastGoldRewardTime).count();

    int intervalSeconds = g_config.gold.intervalMinutes * 60;
    if (elapsed >= intervalSeconds) {
        auto* player = RE::PlayerCharacter::GetSingleton();
        auto* gold = RE::TESForm::LookupByID<RE::TESBoundObject>(0x0000000F);

        if (player && gold) {
            player->AddObjectToContainer(gold, nullptr, g_config.gold.amount, nullptr);

            if (g_config.notification.enabled && g_config.gold.showNotification) {
                std::string msg = "ORisk-and-Reward - Incredible resistance rewarded with " +
                                  std::to_string(g_config.gold.amount) + " gold";
                RE::DebugNotification(msg.c_str());
            }

            WriteToActionsLog("Player received " + std::to_string(g_config.gold.amount) +
                                  " gold (OStim scene: " + GetLastAnimation() + ")",
                              __LINE__);
        }

        g_lastGoldRewardTime = now;
    }
}

void ResolveItemFormIDs() {
    if (g_cachedItemFormIDs.resolved) {
        return;
    }
    
    if (g_config.item1.enabled && g_config.item1.plugin != "none" && g_config.item1.id != "xxxxxx") {
        g_cachedItemFormIDs.item1 = GetFormIDFromPlugin(g_config.item1.plugin, g_config.item1.id);
        if (g_cachedItemFormIDs.item1 != 0) {
            WriteToActionsLog("Item1 (" + g_config.item1.itemName + ") resolved successfully - FormID: 0x" + 
                std::to_string(g_cachedItemFormIDs.item1), __LINE__);
        } else {
            WriteToActionsLog("WARNING: Item1 (" + g_config.item1.itemName + ") FormID resolution failed", __LINE__);
        }
    }
    
    if (g_config.item2.enabled && g_config.item2.plugin != "none" && g_config.item2.id != "xxxxxx") {
        g_cachedItemFormIDs.item2 = GetFormIDFromPlugin(g_config.item2.plugin, g_config.item2.id);
        if (g_cachedItemFormIDs.item2 != 0) {
            WriteToActionsLog("Item2 (" + g_config.item2.itemName + ") resolved successfully - FormID: 0x" + 
                std::to_string(g_cachedItemFormIDs.item2), __LINE__);
        } else {
            WriteToActionsLog("WARNING: Item2 (" + g_config.item2.itemName + ") FormID resolution failed", __LINE__);
        }
    }
    
    if (g_config.milk.enabled) {
        g_cachedItemFormIDs.milkDawnguard = GetFormIDFromPlugin(g_config.milk.plugin, g_config.milk.id);
        if (g_cachedItemFormIDs.milkDawnguard != 0) {
            WriteToActionsLog("Milk (Dawnguard) resolved successfully - FormID: 0x" + 
                std::to_string(g_cachedItemFormIDs.milkDawnguard), __LINE__);
        } else {
            WriteToActionsLog("WARNING: Milk (Dawnguard) FormID resolution failed", __LINE__);
        }
    }
    
    if (g_config.milkWench.enabled) {
        g_cachedItemFormIDs.milkWench = GetFormIDFromPlugin(g_config.milkWench.plugin, g_config.milkWench.id);
        if (g_cachedItemFormIDs.milkWench != 0) {
            WriteToActionsLog("Wench Milk resolved successfully - FormID: 0x" + 
                std::to_string(g_cachedItemFormIDs.milkWench), __LINE__);
        } else {
            WriteToActionsLog("WARNING: Wench Milk FormID resolution failed", __LINE__);
        }
    }
    
    if (g_config.milkEthel.enabled) {
        g_cachedItemFormIDs.milkEthel = GetFormIDFromPlugin(g_config.milkEthel.pluginItem, g_config.milkEthel.id);
        if (g_cachedItemFormIDs.milkEthel != 0) {
            WriteToActionsLog("Milk (Ethel) resolved successfully - FormID: 0x" + 
                std::to_string(g_cachedItemFormIDs.milkEthel), __LINE__);
        } else {
            WriteToActionsLog("WARNING: Milk (Ethel) FormID resolution failed", __LINE__);
        }
    }
    
    if (g_config.item1Event.enabled && g_config.item1Event.plugin != "none" && g_config.item1Event.id != "xxxxxx") {
        g_cachedItemFormIDs.item1Event = GetFormIDFromPlugin(g_config.item1Event.plugin, g_config.item1Event.id);
        if (g_cachedItemFormIDs.item1Event != 0) {
            WriteToActionsLog("Item1Event (" + g_config.item1Event.itemName + ") resolved successfully - FormID: 0x" + 
                std::to_string(g_cachedItemFormIDs.item1Event), __LINE__);
        } else {
            WriteToActionsLog("WARNING: Item1Event (" + g_config.item1Event.itemName + ") FormID resolution failed", __LINE__);
        }
    }
    
    if (g_config.item2Event.enabled && g_config.item2Event.plugin != "none" && g_config.item2Event.id != "xxxxxx") {
        g_cachedItemFormIDs.item2Event = GetFormIDFromPlugin(g_config.item2Event.plugin, g_config.item2Event.id);
        if (g_cachedItemFormIDs.item2Event != 0) {
            WriteToActionsLog("Item2Event (" + g_config.item2Event.itemName + ") resolved successfully - FormID: 0x" + 
                std::to_string(g_cachedItemFormIDs.item2Event), __LINE__);
        } else {
            WriteToActionsLog("WARNING: Item2Event (" + g_config.item2Event.itemName + ") FormID resolution failed", __LINE__);
        }
    }
    
    if (g_config.milkEvent.enabled) {
        g_cachedItemFormIDs.milkEvent = GetFormIDFromPlugin(g_config.milkEvent.plugin, g_config.milkEvent.id);
        if (g_cachedItemFormIDs.milkEvent != 0) {
            WriteToActionsLog("MilkEvent resolved successfully - FormID: 0x" + 
                std::to_string(g_cachedItemFormIDs.milkEvent), __LINE__);
        } else {
            WriteToActionsLog("WARNING: MilkEvent FormID resolution failed", __LINE__);
        }
    }
    
    if (g_config.milkWenchEvent.enabled) {
        g_cachedItemFormIDs.milkWenchEvent = GetFormIDFromPlugin(g_config.milkWenchEvent.plugin, g_config.milkWenchEvent.id);
        if (g_cachedItemFormIDs.milkWenchEvent != 0) {
            WriteToActionsLog("MilkWenchEvent resolved successfully - FormID: 0x" + 
                std::to_string(g_cachedItemFormIDs.milkWenchEvent), __LINE__);
        } else {
            WriteToActionsLog("WARNING: MilkWenchEvent FormID resolution failed", __LINE__);
        }
    }
    
    if (g_config.milkEthelEvent.enabled) {
        g_cachedItemFormIDs.milkEthelEvent = GetFormIDFromPlugin(g_config.milkEthelEvent.pluginItem, g_config.milkEthelEvent.id);
        if (g_cachedItemFormIDs.milkEthelEvent != 0) {
            WriteToActionsLog("MilkEthelEvent resolved successfully - FormID: 0x" + 
                std::to_string(g_cachedItemFormIDs.milkEthelEvent), __LINE__);
        } else {
            WriteToActionsLog("WARNING: MilkEthelEvent FormID resolution failed", __LINE__);
        }
    }
    
    g_cachedItemFormIDs.resolved = true;
}

void CheckAndRewardItem1() {
    LoadConfiguration();

    if (!g_config.item1.enabled || !IsInOStimScene() || GetLastAnimation().empty()) {
        return;
    }

    auto now = std::chrono::steady_clock::now();
    auto elapsed = std::chrono::duration_cast<std::chrono::seconds>(now - g_lastItem1RewardTime).count();

    int intervalSeconds = g_config.item1.intervalMinutes * 60;
    if (elapsed >= intervalSeconds) {
        if (g_cachedItemFormIDs.item1 == 0) {
            WriteToActionsLog("DEBUG: Item1 - Cached FormID is 0, skipping reward", __LINE__);
            g_lastItem1RewardTime = now;
            return;
        }

        auto* player = RE::PlayerCharacter::GetSingleton();
        if (!player) {
            WriteToActionsLog("DEBUG: Item1 - Player pointer is nullptr", __LINE__);
            g_lastItem1RewardTime = now;
            return;
        }

        auto* itemForm = RE::TESForm::LookupByID(g_cachedItemFormIDs.item1);
        if (!itemForm) {
            WriteToActionsLog("DEBUG: Item1 - TESForm::LookupByID returned nullptr for FormID: 0x" + 
                std::to_string(g_cachedItemFormIDs.item1), __LINE__);
            g_lastItem1RewardTime = now;
            return;
        }

        auto* item = itemForm->As<RE::TESBoundObject>();
        if (!item) {
            WriteToActionsLog("DEBUG: Item1 - Form is not a TESBoundObject, FormType: " + 
                std::to_string(static_cast<int>(itemForm->GetFormType())), __LINE__);
            g_lastItem1RewardTime = now;
            return;
        }

        player->AddObjectToContainer(item, nullptr, g_config.item1.amount, nullptr);

        if (g_config.notification.enabled && g_config.item1.showNotification) {
            std::string msg = "ORisk-and-Reward - Received " + std::to_string(g_config.item1.amount) + " " + g_config.item1.itemName;
            RE::DebugNotification(msg.c_str());
        }

        WriteToActionsLog("Player received " + std::to_string(g_config.item1.amount) +
                              " " + g_config.item1.itemName + " (OStim scene: " + GetLastAnimation() + ")",
                          __LINE__);

        g_lastItem1RewardTime = now;
    }
}

void CheckAndRewardItem2() {
    LoadConfiguration();

    if (!g_config.item2.enabled || !IsInOStimScene() || GetLastAnimation().empty()) {
        return;
    }

    auto now = std::chrono::steady_clock::now();
    auto elapsed = std::chrono::duration_cast<std::chrono::seconds>(now - g_lastItem2RewardTime).count();

    int intervalSeconds = g_config.item2.intervalMinutes * 60;
    if (elapsed >= intervalSeconds) {
        if (g_cachedItemFormIDs.item2 == 0) {
            WriteToActionsLog("DEBUG: Item2 - Cached FormID is 0, skipping reward", __LINE__);
            g_lastItem2RewardTime = now;
            return;
        }

        auto* player = RE::PlayerCharacter::GetSingleton();
        if (!player) {
            WriteToActionsLog("DEBUG: Item2 - Player pointer is nullptr", __LINE__);
            g_lastItem2RewardTime = now;
            return;
        }

        auto* itemForm = RE::TESForm::LookupByID(g_cachedItemFormIDs.item2);
        if (!itemForm) {
            WriteToActionsLog("DEBUG: Item2 - TESForm::LookupByID returned nullptr for FormID: 0x" + 
                std::to_string(g_cachedItemFormIDs.item2), __LINE__);
            g_lastItem2RewardTime = now;
            return;
        }

        auto* item = itemForm->As<RE::TESBoundObject>();
        if (!item) {
            WriteToActionsLog("DEBUG: Item2 - Form is not a TESBoundObject, FormType: " + 
                std::to_string(static_cast<int>(itemForm->GetFormType())), __LINE__);
            g_lastItem2RewardTime = now;
            return;
        }

        player->AddObjectToContainer(item, nullptr, g_config.item2.amount, nullptr);

        if (g_config.notification.enabled && g_config.item2.showNotification) {
            std::string msg = "ORisk-and-Reward - Received " + std::to_string(g_config.item2.amount) + " " + g_config.item2.itemName;
            RE::DebugNotification(msg.c_str());
        }

        WriteToActionsLog("Player received " + std::to_string(g_config.item2.amount) +
                              " " + g_config.item2.itemName + " (OStim scene: " + GetLastAnimation() + ")",
                          __LINE__);

        g_lastItem2RewardTime = now;
    }
}

void CheckAndRewardMilk() {
    LoadConfiguration();

    if (!g_config.milk.enabled || !IsInOStimScene() || GetLastAnimation().empty()) {
        return;
    }

    auto now = std::chrono::steady_clock::now();
    auto elapsed = std::chrono::duration_cast<std::chrono::seconds>(now - g_lastMilkRewardTime).count();

    int intervalSeconds = g_config.milk.intervalMinutes * 60;
    if (elapsed >= intervalSeconds) {
        if (g_cachedItemFormIDs.milkDawnguard == 0) {
            WriteToActionsLog("DEBUG: Milk (Dawnguard) - Cached FormID is 0, skipping reward", __LINE__);
            g_lastMilkRewardTime = now;
            return;
        }

        auto* player = RE::PlayerCharacter::GetSingleton();
        if (!player) {
            WriteToActionsLog("DEBUG: Milk (Dawnguard) - Player pointer is nullptr", __LINE__);
            g_lastMilkRewardTime = now;
            return;
        }

        auto* milkForm = RE::TESForm::LookupByID(g_cachedItemFormIDs.milkDawnguard);
        if (!milkForm) {
            WriteToActionsLog("DEBUG: Milk (Dawnguard) - TESForm::LookupByID returned nullptr for FormID: 0x" + 
                std::to_string(g_cachedItemFormIDs.milkDawnguard), __LINE__);
            g_lastMilkRewardTime = now;
            return;
        }

        auto* milkItem = milkForm->As<RE::TESBoundObject>();
        if (!milkItem) {
            WriteToActionsLog("DEBUG: Milk (Dawnguard) - Form is not a TESBoundObject, FormType: " + 
                std::to_string(static_cast<int>(milkForm->GetFormType())), __LINE__);
            g_lastMilkRewardTime = now;
            return;
        }

        player->AddObjectToContainer(milkItem, nullptr, g_config.milk.amount, nullptr);

        if (g_config.notification.enabled && g_config.milk.showNotification) {
            std::string msg = "ORisk-and-Reward - Received " + std::to_string(g_config.milk.amount) + " Milk";
            RE::DebugNotification(msg.c_str());
        }

        WriteToActionsLog("Player received " + std::to_string(g_config.milk.amount) +
                              " Milk (OStim scene: " + GetLastAnimation() + ")",
                          __LINE__);

        g_lastMilkRewardTime = now;
    }
}

void TryCaptureNPCFormIDs() {
    auto* player = RE::PlayerCharacter::GetSingleton();
    if (!player) return;
    
    auto* processLists = RE::ProcessLists::GetSingleton();
    if (!processLists) return;
    
    RE::NiPoint3 playerPos = player->GetPosition();
    auto* dataHandler = RE::TESDataHandler::GetSingleton();
    if (!dataHandler) return;
    
    if (!g_wenchPluginChecked) {
        g_wenchPluginExists = (dataHandler->LookupModByName(g_config.milkWench.plugin) != nullptr);
        g_wenchPluginChecked = true;
        
        if (g_wenchPluginExists) {
            WriteToActionsLog("YurianaWench.esp detected - BWY Milk system active", __LINE__);
        } else {
            WriteToActionsLog("YurianaWench.esp not installed - BWY Milk system disabled", __LINE__);
        }
    }
    
    if (!g_ethelPluginChecked) {
        g_ethelPluginExists = (dataHandler->LookupModByName(g_config.milkEthel.pluginNPC) != nullptr);
        g_ethelPluginChecked = true;
        
        if (g_ethelPluginExists) {
            WriteToActionsLog("Ethel plugin detected - Milk Ethel system active", __LINE__);
        } else {
            WriteToActionsLog("Ethel plugin not installed - Milk Ethel system disabled", __LINE__);
        }
    }
    
    if (g_config.milkWench.enabled && g_wenchPluginExists && !g_capturedYurianaWenchNPC.captured) {
        auto* file = dataHandler->LookupModByName(g_config.milkWench.plugin);
        if (file) {
            uint8_t modIndex = file->compileIndex;
            if (modIndex == 0xFF) {
                modIndex = file->smallFileCompileIndex;
            }
            
            auto searchInList = [&](auto& actorHandles) -> bool {
                for (auto& actorHandle : actorHandles) {
                    auto actor = actorHandle.get();
                    if (!actor) continue;
                    
                    auto* actorBase = actor->GetActorBase();
                    if (!actorBase) continue;
                    
                    uint8_t npcModIndex = (actorBase->formID >> 24) & 0xFF;
                    if (npcModIndex == modIndex) {
                        RE::NiPoint3 npcPos = actor->GetPosition();
                        float distance = playerPos.GetDistance(npcPos);
                        
                        if (distance <= 500.0f) {
                            g_capturedYurianaWenchNPC.formID = actorBase->formID;
                            g_capturedYurianaWenchNPC.pluginName = g_config.milkWench.plugin;
                            g_capturedYurianaWenchNPC.captured = true;
                            g_capturedYurianaWenchNPC.lastSeen = std::chrono::steady_clock::now();
                            
                            WriteToActionsLog("Auto-captured YurianaWench NPC", __LINE__);
                            return true;
                        }
                    }
                }
                return false;
            };
            
            if (!searchInList(processLists->highActorHandles)) {
                if (!searchInList(processLists->middleHighActorHandles)) {
                    searchInList(processLists->lowActorHandles);
                }
            }
        }
    }
    
    if (g_config.milkEthel.enabled && g_ethelPluginExists && !g_capturedEthelNPC.captured) {
        std::string cleanID = g_config.milkEthel.npc;
        
        if (cleanID.length() >= 2 && cleanID.substr(0, 2) == "XX") {
            cleanID = cleanID.substr(2);
        }
        
        RE::FormID targetFormID = GetFormIDFromPlugin(g_config.milkEthel.pluginNPC, cleanID);
        
        if (targetFormID != 0) {
            auto searchInList = [&](auto& actorHandles) -> bool {
                for (auto& actorHandle : actorHandles) {
                    auto actor = actorHandle.get();
                    if (!actor) continue;
                    
                    auto* actorBase = actor->GetActorBase();
                    if (!actorBase) continue;
                    
                    if (actorBase->formID == targetFormID) {
                        RE::NiPoint3 npcPos = actor->GetPosition();
                        float distance = playerPos.GetDistance(npcPos);
                        
                        if (distance <= 500.0f) {
                            g_capturedEthelNPC.formID = targetFormID;
                            g_capturedEthelNPC.pluginName = g_config.milkEthel.pluginNPC;
                            g_capturedEthelNPC.captured = true;
                            g_capturedEthelNPC.lastSeen = std::chrono::steady_clock::now();
                            
                            WriteToActionsLog("Auto-captured Ethel NPC", __LINE__);
                            return true;
                        }
                    }
                }
                return false;
            };
            
            if (!searchInList(processLists->highActorHandles)) {
                if (!searchInList(processLists->middleHighActorHandles)) {
                    searchInList(processLists->lowActorHandles);
                }
            }
        }
    }
}

void CheckForNearbyNPCs() {
    if (!IsInOStimScene()) {
        if (g_wenchMilkNPCDetected || g_ethelNPCDetected) {
            g_wenchMilkNPCDetected = false;
            g_ethelNPCDetected = false;
        }
        return;
    }
    
    auto now = std::chrono::steady_clock::now();
    auto elapsed = std::chrono::duration_cast<std::chrono::seconds>(now - g_lastNPCDetectionCheck).count();
    
    if (elapsed < 2) {
        return;
    }
    
    g_lastNPCDetectionCheck = now;
    
    TryCaptureNPCFormIDs();
    
    if (g_config.milkWench.enabled && g_wenchPluginExists) {
        bool isNearby = false;
        
        if (g_capturedYurianaWenchNPC.captured) {
            isNearby = IsSpecificNPCNearPlayer(g_capturedYurianaWenchNPC.formID, 500.0f);
            
            if (isNearby) {
                g_capturedYurianaWenchNPC.lastSeen = now;
            }
        } else {
            isNearby = IsAnyNPCFromPluginNearPlayer(g_config.milkWench.plugin, 500.0f);
        }
        
        if (isNearby && !g_wenchMilkNPCDetected) {
            g_wenchMilkNPCDetected = true;
            if (g_config.notification.enabled && g_config.milkWench.showNotification) {
                RE::DebugNotification("ORisk-and-Reward - You have a wench nearby who will assist you on this cold evening");
            }
            WriteToActionsLog("YurianaWench NPC detected nearby (Wench Milk eligible)", __LINE__);
        } else if (!isNearby && g_wenchMilkNPCDetected) {
            g_wenchMilkNPCDetected = false;
            WriteToActionsLog("YurianaWench NPC left detection range", __LINE__);
        }
    }
    
    if (g_config.milkEthel.enabled && g_ethelPluginExists) {
        bool isNearby = false;
        
        if (g_capturedEthelNPC.captured) {
            isNearby = IsSpecificNPCNearPlayer(g_capturedEthelNPC.formID, 500.0f);
            
            if (isNearby) {
                g_capturedEthelNPC.lastSeen = now;
            }
        }
        
        if (isNearby && !g_ethelNPCDetected) {
            g_ethelNPCDetected = true;
            if (g_config.notification.enabled && g_config.milkEthel.showNotification) {
                RE::DebugNotification("ORisk-and-Reward - Ethel the Cute little Cow is with you!");
            }
            WriteToActionsLog("Ethel NPC detected nearby (Milk Ethel eligible)", __LINE__);
        } else if (!isNearby && g_ethelNPCDetected) {
            g_ethelNPCDetected = false;
            WriteToActionsLog("Ethel NPC left detection range", __LINE__);
        }
    }
}

void CheckAndRewardMilkWench() {
    LoadConfiguration();

    if (!g_config.milkWench.enabled || !IsInOStimScene() || GetLastAnimation().empty()) {
        return;
    }
    
    if (!g_wenchMilkNPCDetected) {
        return;
    }
    
    auto now = std::chrono::steady_clock::now();
    auto elapsed = std::chrono::duration_cast<std::chrono::seconds>(now - g_lastMilkWenchRewardTime).count();
    int intervalSeconds = g_config.milkWench.intervalMinutes * 60;
    
    if (elapsed >= intervalSeconds) {
        if (g_cachedItemFormIDs.milkWench == 0) {
            WriteToActionsLog("DEBUG: Wench Milk - Cached FormID is 0, skipping reward", __LINE__);
            g_lastMilkWenchRewardTime = now;
            return;
        }
        
        auto* player = RE::PlayerCharacter::GetSingleton();
        if (!player) {
            WriteToActionsLog("DEBUG: Wench Milk - Player pointer is nullptr", __LINE__);
            g_lastMilkWenchRewardTime = now;
            return;
        }

        auto* milkForm = RE::TESForm::LookupByID(g_cachedItemFormIDs.milkWench);
        if (!milkForm) {
            WriteToActionsLog("DEBUG: Wench Milk - TESForm::LookupByID returned nullptr for FormID: 0x" + 
                std::to_string(g_cachedItemFormIDs.milkWench), __LINE__);
            g_lastMilkWenchRewardTime = now;
            return;
        }

        auto* milkItem = milkForm->As<RE::TESBoundObject>();
        if (!milkItem) {
            WriteToActionsLog("DEBUG: Wench Milk - Form is not a TESBoundObject, FormType: " + 
                std::to_string(static_cast<int>(milkForm->GetFormType())), __LINE__);
            g_lastMilkWenchRewardTime = now;
            return;
        }
        
        player->AddObjectToContainer(milkItem, nullptr, g_config.milkWench.amount, nullptr);
        
        if (g_config.notification.enabled && g_config.milkWench.showNotification) {
            std::string msg = "ORisk-and-Reward - Received " + std::to_string(g_config.milkWench.amount) + " Wench Milk";
            RE::DebugNotification(msg.c_str());
        }
        
        WriteToActionsLog("Player received " + std::to_string(g_config.milkWench.amount) +
                          " Wench Milk with NPC nearby (OStim scene: " + GetLastAnimation() + ")",
                          __LINE__);
        
        g_lastMilkWenchRewardTime = now;
    }
}

void CheckAndRewardMilkEthel() {
    LoadConfiguration();

    if (!g_config.milkEthel.enabled || !IsInOStimScene() || GetLastAnimation().empty()) {
        return;
    }
    
    if (!g_ethelNPCDetected) {
        return;
    }
    
    auto now = std::chrono::steady_clock::now();
    auto elapsed = std::chrono::duration_cast<std::chrono::seconds>(now - g_lastMilkEthelRewardTime).count();
    int intervalSeconds = g_config.milkEthel.intervalMinutes * 60;
    
    if (elapsed >= intervalSeconds) {
        if (g_cachedItemFormIDs.milkEthel == 0) {
            WriteToActionsLog("DEBUG: Milk Ethel - Cached FormID is 0, skipping reward", __LINE__);
            g_lastMilkEthelRewardTime = now;
            return;
        }
        
        auto* player = RE::PlayerCharacter::GetSingleton();
        if (!player) {
            WriteToActionsLog("DEBUG: Milk Ethel - Player pointer is nullptr", __LINE__);
            g_lastMilkEthelRewardTime = now;
            return;
        }

        auto* milkForm = RE::TESForm::LookupByID(g_cachedItemFormIDs.milkEthel);
        if (!milkForm) {
            WriteToActionsLog("DEBUG: Milk Ethel - TESForm::LookupByID returned nullptr for FormID: 0x" + 
                std::to_string(g_cachedItemFormIDs.milkEthel), __LINE__);
            g_lastMilkEthelRewardTime = now;
            return;
        }

        auto* milkItem = milkForm->As<RE::TESBoundObject>();
        if (!milkItem) {
            WriteToActionsLog("DEBUG: Milk Ethel - Form is not a TESBoundObject, FormType: " + 
                std::to_string(static_cast<int>(milkForm->GetFormType())), __LINE__);
            g_lastMilkEthelRewardTime = now;
            return;
        }
        
        player->AddObjectToContainer(milkItem, nullptr, g_config.milkEthel.amount, nullptr);
        
        if (g_config.notification.enabled && g_config.milkEthel.showNotification) {
            std::string msg = "ORisk-and-Reward - Received " + std::to_string(g_config.milkEthel.amount) + " Milk Ethel";
            RE::DebugNotification(msg.c_str());
        }
        
        WriteToActionsLog("Player received " + std::to_string(g_config.milkEthel.amount) +
                          " Milk Ethel with Ethel nearby (OStim scene: " + GetLastAnimation() + ")",
                          __LINE__);
        
        g_lastMilkEthelRewardTime = now;
    }
}

void CheckAndRestoreAttributes() {
    LoadConfiguration();

    if (!g_config.attributes.enabled || !IsInOStimScene() || GetLastAnimation().empty()) {
        return;
    }

    auto now = std::chrono::steady_clock::now();
    auto elapsed = std::chrono::duration_cast<std::chrono::seconds>(now - g_lastAttributesRestorationTime).count();

    if (elapsed < g_config.attributes.intervalSeconds) {
        return;
    }

    auto* player = RE::PlayerCharacter::GetSingleton();
    if (!player) {
        return;
    }

    auto* actorValueOwner = player->AsActorValueOwner();
    if (actorValueOwner) {
        float amount = static_cast<float>(g_config.attributes.restorationAmount);
        actorValueOwner->RestoreActorValue(RE::ACTOR_VALUE_MODIFIER::kDamage, RE::ActorValue::kHealth, amount);
        actorValueOwner->RestoreActorValue(RE::ACTOR_VALUE_MODIFIER::kDamage, RE::ActorValue::kMagicka, amount);
        actorValueOwner->RestoreActorValue(RE::ACTOR_VALUE_MODIFIER::kDamage, RE::ActorValue::kStamina, amount);

        if (g_config.notification.enabled && g_config.attributes.showNotification) {
            std::string msg =
                "ORisk-and-Reward - Attributes restored " + std::to_string(g_config.attributes.restorationAmount) + " points";
            RE::DebugNotification(msg.c_str());
        }

        WriteToActionsLog("Player received " + std::to_string(g_config.attributes.restorationAmount) +
                              " points in all attributes (Health, Magicka, Stamina)",
                          __LINE__);
    }

    g_lastAttributesRestorationTime = now;
}

void SaveDefaultConfiguration() {
    fs::path configDir = GetPluginINIPath();
    
    WriteToActionsLog("Creating default configuration files in: " + configDir.string(), __LINE__);
    
    fs::path iniItemAttributesTime = configDir / "ORisk-and-Reward-NG-ITEM-Attributes-TIME.ini";
    std::ofstream fileItemAttributesTime(iniItemAttributesTime, std::ios::trunc);
    if (fileItemAttributesTime.is_open()) {
        fileItemAttributesTime << "[Gold]" << std::endl;
        fileItemAttributesTime << "Enabled=false" << std::endl;
        fileItemAttributesTime << "Amount=300" << std::endl;
        fileItemAttributesTime << "IntervalMinutes=1" << std::endl;
        fileItemAttributesTime << "ShowNotification=true" << std::endl;
        fileItemAttributesTime << std::endl;
        
        fileItemAttributesTime << "[Attributes]" << std::endl;
        fileItemAttributesTime << "Enabled=false" << std::endl;
        fileItemAttributesTime << "RestorationAmount=50" << std::endl;
        fileItemAttributesTime << "IntervalSeconds=120" << std::endl;
        fileItemAttributesTime << "ShowNotification=true" << std::endl;
        fileItemAttributesTime << std::endl;
        
        fileItemAttributesTime << "[Milk]" << std::endl;
        fileItemAttributesTime << "Enabled=false" << std::endl;
        fileItemAttributesTime << "ID=003534" << std::endl;
        fileItemAttributesTime << "Plugin=HearthFires.esm" << std::endl;
        fileItemAttributesTime << "Amount=1" << std::endl;
        fileItemAttributesTime << "IntervalMinutes=1" << std::endl;
        fileItemAttributesTime << "ShowNotification=false" << std::endl;
        fileItemAttributesTime << std::endl;
        
        fileItemAttributesTime << "[BWY_Wench_Milk]" << std::endl;
        fileItemAttributesTime << "Enabled=false" << std::endl;
        fileItemAttributesTime << "ID=000D73" << std::endl;
        fileItemAttributesTime << "Plugin=YurianaWench.esp" << std::endl;
        fileItemAttributesTime << "Amount=1" << std::endl;
        fileItemAttributesTime << "IntervalMinutes=1" << std::endl;
        fileItemAttributesTime << "ShowNotification=false" << std::endl;
        fileItemAttributesTime << std::endl;
        
        fileItemAttributesTime << "[BWY_Milk_Ethel]" << std::endl;
        fileItemAttributesTime << "Enabled=false" << std::endl;
        fileItemAttributesTime << "ID=65FEC3" << std::endl;
        fileItemAttributesTime << "PluginItem=YurianaWench.esp" << std::endl;
        fileItemAttributesTime << "NPC=576A03" << std::endl;
        fileItemAttributesTime << "PluginNPC=YurianaWench.esp" << std::endl;
        fileItemAttributesTime << "Amount=1" << std::endl;
        fileItemAttributesTime << "IntervalMinutes=1" << std::endl;
        fileItemAttributesTime << "ShowNotification=false" << std::endl;
        fileItemAttributesTime << std::endl;
        
        fileItemAttributesTime << "[Item1]" << std::endl;
        fileItemAttributesTime << "Enabled=false" << std::endl;
        fileItemAttributesTime << "ItemName=none" << std::endl;
        fileItemAttributesTime << "ID=xxxxxx" << std::endl;
        fileItemAttributesTime << "Plugin=none" << std::endl;
        fileItemAttributesTime << "Amount=1" << std::endl;
        fileItemAttributesTime << "IntervalMinutes=1" << std::endl;
        fileItemAttributesTime << "ShowNotification=false" << std::endl;
        fileItemAttributesTime << std::endl;
        
        fileItemAttributesTime << "[Item2]" << std::endl;
        fileItemAttributesTime << "Enabled=false" << std::endl;
        fileItemAttributesTime << "ItemName=none" << std::endl;
        fileItemAttributesTime << "ID=xxxxxx" << std::endl;
        fileItemAttributesTime << "Plugin=none" << std::endl;
        fileItemAttributesTime << "Amount=1" << std::endl;
        fileItemAttributesTime << "IntervalMinutes=1" << std::endl;
        fileItemAttributesTime << "ShowNotification=false" << std::endl;
        
        fileItemAttributesTime.close();
        WriteToActionsLog("Created: ORisk-and-Reward-NG-ITEM-Attributes-TIME.ini", __LINE__);
    }
    
    fs::path iniItemEventClimax = configDir / "ORisk-and-Reward-NG-ITEM-EVENT-CLIMAX.ini";
    std::ofstream fileItemEventClimax(iniItemEventClimax, std::ios::trunc);
    if (fileItemEventClimax.is_open()) {
        fileItemEventClimax << "[Attributes_EVENT]" << std::endl;
        fileItemEventClimax << "Enabled=false" << std::endl;
        fileItemEventClimax << "RestorationAmount=50" << std::endl;
        fileItemEventClimax << "EVENT=ostim_actor_orgasm" << std::endl;
        fileItemEventClimax << "Male=true" << std::endl;
        fileItemEventClimax << "Female=true" << std::endl;
        fileItemEventClimax << "ShowNotification=true" << std::endl;
        fileItemEventClimax << std::endl;
        
        fileItemEventClimax << "[Item1_EVENT]" << std::endl;
        fileItemEventClimax << "Enabled=false" << std::endl;
        fileItemEventClimax << "ItemName=none" << std::endl;
        fileItemEventClimax << "ID=xxxxxx" << std::endl;
        fileItemEventClimax << "Plugin=none" << std::endl;
        fileItemEventClimax << "Amount=1" << std::endl;
        fileItemEventClimax << "EVENT=ostim_actor_orgasm" << std::endl;
        fileItemEventClimax << "Male=true" << std::endl;
        fileItemEventClimax << "Female=true" << std::endl;
        fileItemEventClimax << "ShowNotification=true" << std::endl;
        fileItemEventClimax << std::endl;
        
        fileItemEventClimax << "[Item2_EVENT]" << std::endl;
        fileItemEventClimax << "Enabled=false" << std::endl;
        fileItemEventClimax << "ItemName=none" << std::endl;
        fileItemEventClimax << "ID=xxxxxx" << std::endl;
        fileItemEventClimax << "Plugin=none" << std::endl;
        fileItemEventClimax << "Amount=1" << std::endl;
        fileItemEventClimax << "EVENT=ostim_actor_orgasm" << std::endl;
        fileItemEventClimax << "Male=true" << std::endl;
        fileItemEventClimax << "Female=true" << std::endl;
        fileItemEventClimax << "ShowNotification=true" << std::endl;
        fileItemEventClimax << std::endl;
        
        fileItemEventClimax << "[Milk_EVENT]" << std::endl;
        fileItemEventClimax << "Enabled=true" << std::endl;
        fileItemEventClimax << "ID=003534" << std::endl;
        fileItemEventClimax << "Plugin=HearthFires.esm" << std::endl;
        fileItemEventClimax << "Amount=2" << std::endl;
        fileItemEventClimax << "EVENT=ostim_actor_orgasm" << std::endl;
        fileItemEventClimax << "Male=false" << std::endl;
        fileItemEventClimax << "Female=true" << std::endl;
        fileItemEventClimax << "ShowNotification=true" << std::endl;
        fileItemEventClimax << std::endl;
        
        fileItemEventClimax << "[BWY_Wench_Milk_EVENT]" << std::endl;
        fileItemEventClimax << "Enabled=true" << std::endl;
        fileItemEventClimax << "ID=000D73" << std::endl;
        fileItemEventClimax << "Plugin=YurianaWench.esp" << std::endl;
        fileItemEventClimax << "Amount=2" << std::endl;
        fileItemEventClimax << "EVENT=ostim_actor_orgasm" << std::endl;
        fileItemEventClimax << "Male=false" << std::endl;
        fileItemEventClimax << "Female=true" << std::endl;
        fileItemEventClimax << "ShowNotification=true" << std::endl;
        fileItemEventClimax << std::endl;
        
        fileItemEventClimax << "[BWY_Milk_Ethel_EVENT]" << std::endl;
        fileItemEventClimax << "Enabled=true" << std::endl;
        fileItemEventClimax << "ID=65FEC3" << std::endl;
        fileItemEventClimax << "PluginItem=YurianaWench.esp" << std::endl;
        fileItemEventClimax << "NPC=576A03" << std::endl;
        fileItemEventClimax << "PluginNPC=YurianaWench.esp" << std::endl;
        fileItemEventClimax << "Amount=2" << std::endl;
        fileItemEventClimax << "EVENT=ostim_actor_orgasm" << std::endl;
        fileItemEventClimax << "Male=false" << std::endl;
        fileItemEventClimax << "Female=true" << std::endl;
        fileItemEventClimax << "ShowNotification=true" << std::endl;
        
        fileItemEventClimax.close();
        WriteToActionsLog("Created: ORisk-and-Reward-NG-ITEM-EVENT-CLIMAX.ini", __LINE__);
    }
    
    fs::path iniBloodyEffect = configDir / "ORisk-and-Reward-NG-Animated_Bloody_Effect.ini";
    std::ofstream fileBloodyEffect(iniBloodyEffect, std::ios::trunc);
    if (fileBloodyEffect.is_open()) {
        fileBloodyEffect << "[AnimatedBloody_Effect_NPC_cast]" << std::endl;
        fileBloodyEffect << "Enabled=true" << std::endl;
        fileBloodyEffect << "SpellID=819" << std::endl;
        fileBloodyEffect << "Plugin=AnimatedBloodyTexture.esp" << std::endl;
        fileBloodyEffect << "actor=player" << std::endl;
        fileBloodyEffect << "objetivo=RefID_NPC" << std::endl;
        fileBloodyEffect << "IntervalActiveSeconds=60" << std::endl;
        fileBloodyEffect << "EVENT=ostim_actor_orgasm" << std::endl;
        fileBloodyEffect << "Male=true" << std::endl;
        fileBloodyEffect << "Female=true" << std::endl;
        fileBloodyEffect << "Tags/NameAnimationEnabled=false" << std::endl;
        fileBloodyEffect << "Tags/NameAnimationList=" << std::endl;
        fileBloodyEffect << "Tags/NameAnimationGender=" << std::endl;
        fileBloodyEffect << "SpeedTagEnabled=false" << std::endl;
        fileBloodyEffect << "LowIntensity=false" << std::endl;
        fileBloodyEffect << "MediumIntensity=false" << std::endl;
        fileBloodyEffect << "HighIntensity=false" << std::endl;
        fileBloodyEffect << "AfterOStim=false" << std::endl;
        fileBloodyEffect << "IntervalActiveSecondsAF=15" << std::endl;
        fileBloodyEffect << "EVENTAF=2" << std::endl;
        fileBloodyEffect << "Pluginfaction=AnimatedBloodyTexture.esp" << std::endl;
        fileBloodyEffect << "factionName=btBleedingNoseEffectFaction" << std::endl;
        fileBloodyEffect << "BloodyNoses=false" << std::endl;
        fileBloodyEffect << "BloodyNosescounter=5" << std::endl;
        fileBloodyEffect << "BloodyNosesMale=true" << std::endl;
        fileBloodyEffect << "BloodyNosesFemale=true" << std::endl;
        fileBloodyEffect << "BloodyNosesTimeSeconds=60" << std::endl;
        fileBloodyEffect << "ShowNotification=true" << std::endl;
        fileBloodyEffect << std::endl;
        
        fileBloodyEffect << "[AnimatedBloody_Effect_PLAYER_cast]" << std::endl;
        fileBloodyEffect << "Enabled=true" << std::endl;
        fileBloodyEffect << "SpellID=81A" << std::endl;
        fileBloodyEffect << "Plugin=AnimatedBloodyTexture.esp" << std::endl;
        fileBloodyEffect << "actor=player" << std::endl;
        fileBloodyEffect << "objetivo=player" << std::endl;
        fileBloodyEffect << "IntervalActiveSeconds=60" << std::endl;
        fileBloodyEffect << "EVENT=ostim_actor_orgasm" << std::endl;
        fileBloodyEffect << "Male=true" << std::endl;
        fileBloodyEffect << "Female=true" << std::endl;
        fileBloodyEffect << "Tags/NameAnimationEnabled=false" << std::endl;
        fileBloodyEffect << "Tags/NameAnimationList=" << std::endl;
        fileBloodyEffect << "Tags/NameAnimationGender=" << std::endl;
        fileBloodyEffect << "SpeedTagEnabled=false" << std::endl;
        fileBloodyEffect << "LowIntensity=false" << std::endl;
        fileBloodyEffect << "MediumIntensity=false" << std::endl;
        fileBloodyEffect << "HighIntensity=false" << std::endl;
        fileBloodyEffect << "AfterOStim=false" << std::endl;
        fileBloodyEffect << "IntervalActiveSecondsAF=15" << std::endl;
        fileBloodyEffect << "EVENTAF=2" << std::endl;
        fileBloodyEffect << "Pluginfaction=AnimatedBloodyTexture.esp" << std::endl;
        fileBloodyEffect << "factionName=btBleedingNoseEffectFaction" << std::endl;
        fileBloodyEffect << "BloodyNoses=false" << std::endl;
        fileBloodyEffect << "BloodyNosescounter=5" << std::endl;
        fileBloodyEffect << "BloodyNosesMale=true" << std::endl;
        fileBloodyEffect << "BloodyNosesFemale=true" << std::endl;
        fileBloodyEffect << "BloodyNosesTimeSeconds=60" << std::endl;
        fileBloodyEffect << "ShowNotification=true" << std::endl;
        
        fileBloodyEffect.close();
        WriteToActionsLog("Created: ORisk-and-Reward-NG-Animated_Bloody_Effect.ini", __LINE__);
    }
    
    fs::path iniEmotionalTears = configDir / "ORisk-and-Reward-NG-Emotional_Tears_Effect.ini";
    std::ofstream fileEmotionalTears(iniEmotionalTears, std::ios::trunc);
    if (fileEmotionalTears.is_open()) {
        fileEmotionalTears << "[Emotional_Tears_Effect_NPC_cast]" << std::endl;
        fileEmotionalTears << "Enabled=true" << std::endl;
        fileEmotionalTears << "SpellID=801" << std::endl;
        fileEmotionalTears << "Plugin=EmoTearsSpells.esp" << std::endl;
        fileEmotionalTears << "actor=player" << std::endl;
        fileEmotionalTears << "objetivo=RefID_NPC" << std::endl;
        fileEmotionalTears << "IntervalActiveSeconds=15" << std::endl;
        fileEmotionalTears << "EVENT=ostim_actor_orgasm" << std::endl;
        fileEmotionalTears << "Male=true" << std::endl;
        fileEmotionalTears << "Female=true" << std::endl;
        fileEmotionalTears << "Tags/NameAnimationEnabled=false" << std::endl;
        fileEmotionalTears << "Tags/NameAnimationList=" << std::endl;
        fileEmotionalTears << "Tags/NameAnimationGender=" << std::endl;
        fileEmotionalTears << "SpeedTagEnabled=false" << std::endl;
        fileEmotionalTears << "LowIntensity=false" << std::endl;
        fileEmotionalTears << "MediumIntensity=false" << std::endl;
        fileEmotionalTears << "HighIntensity=false" << std::endl;
        fileEmotionalTears << "AfterOStim=false" << std::endl;
        fileEmotionalTears << "IntervalActiveSecondsAF=15" << std::endl;
        fileEmotionalTears << "EVENTAF=2" << std::endl;
        fileEmotionalTears << "Pluginfaction=EmoTearsFaction Ostim Patch.esp" << std::endl;
        fileEmotionalTears << "factionName=zzEmotionalTearsFaction" << std::endl;
        fileEmotionalTears << "ShowNotification=true" << std::endl;
        fileEmotionalTears << std::endl;
        
        fileEmotionalTears << "[Emotional_Tears_Effect_PLAYER_cast]" << std::endl;
        fileEmotionalTears << "Enabled=true" << std::endl;
        fileEmotionalTears << "SpellID=802" << std::endl;
        fileEmotionalTears << "Plugin=EmoTearsSpells.esp" << std::endl;
        fileEmotionalTears << "actor=player" << std::endl;
        fileEmotionalTears << "objetivo=player" << std::endl;
        fileEmotionalTears << "IntervalActiveSeconds=15" << std::endl;
        fileEmotionalTears << "EVENT=ostim_actor_orgasm" << std::endl;
        fileEmotionalTears << "Male=true" << std::endl;
        fileEmotionalTears << "Female=true" << std::endl;
        fileEmotionalTears << "Tags/NameAnimationEnabled=false" << std::endl;
        fileEmotionalTears << "Tags/NameAnimationList=" << std::endl;
        fileEmotionalTears << "Tags/NameAnimationGender=" << std::endl;
        fileEmotionalTears << "SpeedTagEnabled=false" << std::endl;
        fileEmotionalTears << "LowIntensity=false" << std::endl;
        fileEmotionalTears << "MediumIntensity=false" << std::endl;
        fileEmotionalTears << "HighIntensity=false" << std::endl;
        fileEmotionalTears << "AfterOStim=false" << std::endl;
        fileEmotionalTears << "IntervalActiveSecondsAF=15" << std::endl;
        fileEmotionalTears << "EVENTAF=2" << std::endl;
        fileEmotionalTears << "Pluginfaction=EmoTearsFaction Ostim Patch.esp" << std::endl;
        fileEmotionalTears << "factionName=zzEmotionalTearsFaction" << std::endl;
        fileEmotionalTears << "ShowNotification=true" << std::endl;
        
        fileEmotionalTears.close();
        WriteToActionsLog("Created: ORisk-and-Reward-NG-Emotional_Tears_Effect.ini", __LINE__);
    }
    
    fs::path iniVampireTears = configDir / "ORisk-and-Reward-NG-Animated_Vampire_Tears.ini";
    std::ofstream fileVampireTears(iniVampireTears, std::ios::trunc);
    if (fileVampireTears.is_open()) {
        fileVampireTears << "[ACTIVE_MODE]" << std::endl;
        fileVampireTears << "enable=true" << std::endl;
        fileVampireTears << std::endl;
        fileVampireTears << "[AnimatedVampireTears_Effect_NPC_cast]" << std::endl;
        fileVampireTears << "Enabled=true" << std::endl;
        fileVampireTears << "SpellID=819" << std::endl;
        fileVampireTears << "Plugin=AnimatedVampireTears.esp" << std::endl;
        fileVampireTears << "actor=player" << std::endl;
        fileVampireTears << "objetivo=RefID_NPC" << std::endl;
        fileVampireTears << "IntervalActiveSeconds=60" << std::endl;
        fileVampireTears << "EVENT=ostim_actor_orgasm" << std::endl;
        fileVampireTears << "Male=true" << std::endl;
        fileVampireTears << "Female=true" << std::endl;
        fileVampireTears << "Tags/NameAnimationEnabled=false" << std::endl;
        fileVampireTears << "Tags/NameAnimationList=" << std::endl;
        fileVampireTears << "Tags/NameAnimationGender=" << std::endl;
        fileVampireTears << "SpeedTagEnabled=false" << std::endl;
        fileVampireTears << "LowIntensity=false" << std::endl;
        fileVampireTears << "MediumIntensity=false" << std::endl;
        fileVampireTears << "HighIntensity=false" << std::endl;
        fileVampireTears << "AfterOStim=false" << std::endl;
        fileVampireTears << "IntervalActiveSecondsAF=15" << std::endl;
        fileVampireTears << "EVENTAF=2" << std::endl;
        fileVampireTears << "Pluginfaction=AnimatedVampireTears.esp" << std::endl;
        fileVampireTears << "factionName=zzEmotionalTearsVAMPFaction" << std::endl;
        fileVampireTears << "ShowNotification=true" << std::endl;
        fileVampireTears << std::endl;
        
        fileVampireTears << "[AnimatedVampireTears_Effect_PLAYER_cast]" << std::endl;
        fileVampireTears << "Enabled=true" << std::endl;
        fileVampireTears << "SpellID=81A" << std::endl;
        fileVampireTears << "Plugin=AnimatedVampireTears.esp" << std::endl;
        fileVampireTears << "actor=player" << std::endl;
        fileVampireTears << "objetivo=player" << std::endl;
        fileVampireTears << "IntervalActiveSeconds=60" << std::endl;
        fileVampireTears << "EVENT=ostim_actor_orgasm" << std::endl;
        fileVampireTears << "Male=true" << std::endl;
        fileVampireTears << "Female=true" << std::endl;
        fileVampireTears << "Tags/NameAnimationEnabled=false" << std::endl;
        fileVampireTears << "Tags/NameAnimationList=" << std::endl;
        fileVampireTears << "Tags/NameAnimationGender=" << std::endl;
        fileVampireTears << "SpeedTagEnabled=false" << std::endl;
        fileVampireTears << "LowIntensity=false" << std::endl;
        fileVampireTears << "MediumIntensity=false" << std::endl;
        fileVampireTears << "HighIntensity=false" << std::endl;
        fileVampireTears << "AfterOStim=false" << std::endl;
        fileVampireTears << "IntervalActiveSecondsAF=15" << std::endl;
        fileVampireTears << "EVENTAF=2" << std::endl;
        fileVampireTears << "Pluginfaction=AnimatedVampireTears.esp" << std::endl;
        fileVampireTears << "factionName=zzEmotionalTearsVAMPFaction" << std::endl;
        fileVampireTears << "ShowNotification=true" << std::endl;
        
        fileVampireTears.close();
        WriteToActionsLog("Created: ORisk-and-Reward-NG-Animated_Vampire_Tears.ini", __LINE__);
    }
    
    fs::path iniNotification = configDir / "ORisk-and-Reward-NG-Notification.ini";
    std::ofstream fileNotification(iniNotification, std::ios::trunc);
    if (fileNotification.is_open()) {
        fileNotification << "[Notification]" << std::endl;
        fileNotification << "Enabled=true" << std::endl;
        
        fileNotification.close();
        WriteToActionsLog("Created: ORisk-and-Reward-NG-Notification.ini", __LINE__);
    }
    
    WriteToActionsLog("All default configuration files created successfully", __LINE__);
}

bool LoadConfiguration() {
    std::lock_guard<std::mutex> lock(g_configMutex);

    fs::path configDir = GetPluginINIPath();
    
    std::vector<fs::path> iniFiles = {
        configDir / "ORisk-and-Reward-NG-ITEM-Attributes-TIME.ini",
        configDir / "ORisk-and-Reward-NG-ITEM-EVENT-CLIMAX.ini",
        configDir / "ORisk-and-Reward-NG-Animated_Bloody_Effect.ini",
        configDir / "ORisk-and-Reward-NG-Emotional_Tears_Effect.ini",
        configDir / "ORisk-and-Reward-NG-Animated_Vampire_Tears.ini",
        configDir / "ORisk-and-Reward-NG-Notification.ini"
    };
    
    bool allFilesExist = true;
    for (const auto& iniFile : iniFiles) {
        if (!fs::exists(iniFile)) {
            allFilesExist = false;
            WriteToActionsLog("Missing configuration file: " + iniFile.filename().string(), __LINE__);
        }
    }
    
    if (!allFilesExist) {
        WriteToActionsLog("Creating missing configuration files", __LINE__);
        SaveDefaultConfiguration();
    }
    
    for (const auto& iniPath : iniFiles) {
        if (!fs::exists(iniPath)) {
            WriteToActionsLog("WARNING: INI file does not exist after creation attempt: " + iniPath.string(), __LINE__);
            continue;
        }
        
        std::ifstream iniFile(iniPath);
        if (!iniFile.is_open()) {
            WriteToActionsLog("ERROR: Failed to open INI file: " + iniPath.string(), __LINE__);
            continue;
        }
        
        std::string line;
        std::string currentSection;
        
        while (std::getline(iniFile, line)) {
            line.erase(0, line.find_first_not_of(" \t\r\n"));
            line.erase(line.find_last_not_of(" \t\r\n") + 1);

            if (line.empty() || line[0] == ';' || line[0] == '#') {
                continue;
            }

            if (line[0] == '[' && line[line.length() - 1] == ']') {
                currentSection = line.substr(1, line.length() - 2);
                continue;
            }

            size_t equalPos = line.find('=');
            if (equalPos != std::string::npos) {
                std::string key = line.substr(0, equalPos);
                std::string value = line.substr(equalPos + 1);

                key.erase(0, key.find_first_not_of(" \t"));
                key.erase(key.find_last_not_of(" \t") + 1);
                value.erase(0, value.find_first_not_of(" \t"));
                value.erase(value.find_last_not_of(" \t") + 1);

                if (currentSection == "Gold") {
                    if (key == "Enabled") {
                        g_config.gold.enabled = (value == "1" || value == "true" || value == "True");
                    } else if (key == "Amount") {
                        g_config.gold.amount = std::stoi(value);
                    } else if (key == "IntervalMinutes") {
                        g_config.gold.intervalMinutes = std::stoi(value);
                    } else if (key == "ShowNotification") {
                        g_config.gold.showNotification = (value == "1" || value == "true" || value == "True");
                    }
                } else if (currentSection == "Emotional_Tears_Effect_NPC_cast") {
                    if (key == "Enabled") {
                        g_config.emotionalTearsNPC.enabled = (value == "1" || value == "true" || value == "True");
                    } else if (key == "SpellID") {
                        g_config.emotionalTearsNPC.spellID = value;
                    } else if (key == "Plugin") {
                        g_config.emotionalTearsNPC.plugin = value;
                    } else if (key == "actor") {
                        g_config.emotionalTearsNPC.actor = value;
                    } else if (key == "objetivo") {
                        g_config.emotionalTearsNPC.objetivo = value;
                    } else if (key == "IntervalActiveSeconds") {
                        g_config.emotionalTearsNPC.intervalActiveSeconds = std::stoi(value);
                    } else if (key == "EVENT") {
                        g_config.emotionalTearsNPC.event = value;
                    } else if (key == "Male") {
                        g_config.emotionalTearsNPC.male = (value == "1" || value == "true" || value == "True");
                    } else if (key == "Female") {
                        g_config.emotionalTearsNPC.female = (value == "1" || value == "true" || value == "True");
                    } else if (key == "ShowNotification") {
                        g_config.emotionalTearsNPC.showNotification = (value == "1" || value == "true" || value == "True");
                    } else if (key == "Tags/NameAnimationEnabled") {
                        g_config.emotionalTearsNPC.tagsNameAnimationEnabled = (value == "1" || value == "true" || value == "True");
                    } else if (key == "Tags/NameAnimationList") {
                        g_config.emotionalTearsNPC.tagsNameAnimationList = value;
                    } else if (key == "Tags/NameAnimationGender") {
                        g_config.emotionalTearsNPC.tagsNameAnimationGender = value;
                    } else if (key == "SpeedTagEnabled") {
                        g_config.emotionalTearsNPC.speedTagEnabled = (value == "1" || value == "true" || value == "True");
                    } else if (key == "LowIntensity") {
                        g_config.emotionalTearsNPC.lowIntensity = (value == "1" || value == "true" || value == "True");
                    } else if (key == "MediumIntensity") {
                        g_config.emotionalTearsNPC.mediumIntensity = (value == "1" || value == "true" || value == "True");
                    } else if (key == "HighIntensity") {
                        g_config.emotionalTearsNPC.highIntensity = (value == "1" || value == "true" || value == "True");
                    } else if (key == "AfterOStim") {
                        g_config.emotionalTearsNPC.afterOStim = (value == "1" || value == "true" || value == "True");
                    } else if (key == "IntervalActiveSecondsAF") {
                        g_config.emotionalTearsNPC.intervalActiveSecondsAF = std::stoi(value);
                    } else if (key == "EVENTAF") {
                        g_config.emotionalTearsNPC.eventAF = std::stoi(value);
                    } else if (key == "Pluginfaction") {
                        g_config.emotionalTearsNPC.pluginFaction = value;
                    } else if (key == "factionName") {
                        g_config.emotionalTearsNPC.factionName = value;
                    }
                } else if (currentSection == "Emotional_Tears_Effect_PLAYER_cast") {
                    if (key == "Enabled") {
                        g_config.emotionalTearsPlayer.enabled = (value == "1" || value == "true" || value == "True");
                    } else if (key == "SpellID") {
                        g_config.emotionalTearsPlayer.spellID = value;
                    } else if (key == "Plugin") {
                        g_config.emotionalTearsPlayer.plugin = value;
                    } else if (key == "actor") {
                        g_config.emotionalTearsPlayer.actor = value;
                    } else if (key == "objetivo") {
                        g_config.emotionalTearsPlayer.objetivo = value;
                    } else if (key == "IntervalActiveSeconds") {
                        g_config.emotionalTearsPlayer.intervalActiveSeconds = std::stoi(value);
                    } else if (key == "EVENT") {
                        g_config.emotionalTearsPlayer.event = value;
                    } else if (key == "Male") {
                        g_config.emotionalTearsPlayer.male = (value == "1" || value == "true" || value == "True");
                    } else if (key == "Female") {
                        g_config.emotionalTearsPlayer.female = (value == "1" || value == "true" || value == "True");
                    } else if (key == "ShowNotification") {
                        g_config.emotionalTearsPlayer.showNotification = (value == "1" || value == "true" || value == "True");
                    } else if (key == "Tags/NameAnimationEnabled") {
                        g_config.emotionalTearsPlayer.tagsNameAnimationEnabled = (value == "1" || value == "true" || value == "True");
                    } else if (key == "Tags/NameAnimationList") {
                        g_config.emotionalTearsPlayer.tagsNameAnimationList = value;
                    } else if (key == "Tags/NameAnimationGender") {
                        g_config.emotionalTearsPlayer.tagsNameAnimationGender = value;
                    } else if (key == "SpeedTagEnabled") {
                        g_config.emotionalTearsPlayer.speedTagEnabled = (value == "1" || value == "true" || value == "True");
                    } else if (key == "LowIntensity") {
                        g_config.emotionalTearsPlayer.lowIntensity = (value == "1" || value == "true" || value == "True");
                    } else if (key == "MediumIntensity") {
                        g_config.emotionalTearsPlayer.mediumIntensity = (value == "1" || value == "true" || value == "True");
                    } else if (key == "HighIntensity") {
                        g_config.emotionalTearsPlayer.highIntensity = (value == "1" || value == "true" || value == "True");
                    } else if (key == "AfterOStim") {
                        g_config.emotionalTearsPlayer.afterOStim = (value == "1" || value == "true" || value == "True");
                    } else if (key == "IntervalActiveSecondsAF") {
                        g_config.emotionalTearsPlayer.intervalActiveSecondsAF = std::stoi(value);
                    } else if (key == "EVENTAF") {
                        g_config.emotionalTearsPlayer.eventAF = std::stoi(value);
                    } else if (key == "Pluginfaction") {
                        g_config.emotionalTearsPlayer.pluginFaction = value;
                    } else if (key == "factionName") {
                        g_config.emotionalTearsPlayer.factionName = value;
                    }
                } else if (currentSection == "ACTIVE_MODE") {
                    if (key == "enable") {
                        bool activeModeValue = (value == "1" || value == "true" || value == "True");
                        g_config.vampireTearsNPC.activeMode = activeModeValue;
                        g_config.vampireTearsPlayer.activeMode = activeModeValue;
                    }
                } else if (currentSection == "AnimatedVampireTears_Effect_NPC_cast") {
                    if (key == "Enabled") {
                        g_config.vampireTearsNPC.enabled = (value == "1" || value == "true" || value == "True");
                    } else if (key == "SpellID") {
                        g_config.vampireTearsNPC.spellID = value;
                    } else if (key == "Plugin") {
                        g_config.vampireTearsNPC.plugin = value;
                    } else if (key == "actor") {
                        g_config.vampireTearsNPC.actor = value;
                    } else if (key == "objetivo") {
                        g_config.vampireTearsNPC.objetivo = value;
                    } else if (key == "IntervalActiveSeconds") {
                        g_config.vampireTearsNPC.intervalActiveSeconds = std::stoi(value);
                    } else if (key == "EVENT") {
                        g_config.vampireTearsNPC.event = value;
                    } else if (key == "Male") {
                        g_config.vampireTearsNPC.male = (value == "1" || value == "true" || value == "True");
                    } else if (key == "Female") {
                        g_config.vampireTearsNPC.female = (value == "1" || value == "true" || value == "True");
                    } else if (key == "ShowNotification") {
                        g_config.vampireTearsNPC.showNotification = (value == "1" || value == "true" || value == "True");
                    } else if (key == "Tags/NameAnimationEnabled") {
                        g_config.vampireTearsNPC.tagsNameAnimationEnabled = (value == "1" || value == "true" || value == "True");
                    } else if (key == "Tags/NameAnimationList") {
                        g_config.vampireTearsNPC.tagsNameAnimationList = value;
                    } else if (key == "Tags/NameAnimationGender") {
                        g_config.vampireTearsNPC.tagsNameAnimationGender = value;
                    } else if (key == "SpeedTagEnabled") {
                        g_config.vampireTearsNPC.speedTagEnabled = (value == "1" || value == "true" || value == "True");
                    } else if (key == "LowIntensity") {
                        g_config.vampireTearsNPC.lowIntensity = (value == "1" || value == "true" || value == "True");
                    } else if (key == "MediumIntensity") {
                        g_config.vampireTearsNPC.mediumIntensity = (value == "1" || value == "true" || value == "True");
                    } else if (key == "HighIntensity") {
                        g_config.vampireTearsNPC.highIntensity = (value == "1" || value == "true" || value == "True");
                    } else if (key == "AfterOStim") {
                        g_config.vampireTearsNPC.afterOStim = (value == "1" || value == "true" || value == "True");
                    } else if (key == "IntervalActiveSecondsAF") {
                        g_config.vampireTearsNPC.intervalActiveSecondsAF = std::stoi(value);
                    } else if (key == "EVENTAF") {
                        g_config.vampireTearsNPC.eventAF = std::stoi(value);
                    } else if (key == "Pluginfaction") {
                        g_config.vampireTearsNPC.pluginFaction = value;
                    } else if (key == "factionName") {
                        g_config.vampireTearsNPC.factionName = value;
                    }
                } else if (currentSection == "AnimatedVampireTears_Effect_PLAYER_cast") {
                    if (key == "Enabled") {
                        g_config.vampireTearsPlayer.enabled = (value == "1" || value == "true" || value == "True");
                    } else if (key == "SpellID") {
                        g_config.vampireTearsPlayer.spellID = value;
                    } else if (key == "Plugin") {
                        g_config.vampireTearsPlayer.plugin = value;
                    } else if (key == "actor") {
                        g_config.vampireTearsPlayer.actor = value;
                    } else if (key == "objetivo") {
                        g_config.vampireTearsPlayer.objetivo = value;
                    } else if (key == "IntervalActiveSeconds") {
                        g_config.vampireTearsPlayer.intervalActiveSeconds = std::stoi(value);
                    } else if (key == "EVENT") {
                        g_config.vampireTearsPlayer.event = value;
                    } else if (key == "Male") {
                        g_config.vampireTearsPlayer.male = (value == "1" || value == "true" || value == "True");
                    } else if (key == "Female") {
                        g_config.vampireTearsPlayer.female = (value == "1" || value == "true" || value == "True");
                    } else if (key == "ShowNotification") {
                        g_config.vampireTearsPlayer.showNotification = (value == "1" || value == "true" || value == "True");
                    } else if (key == "Tags/NameAnimationEnabled") {
                        g_config.vampireTearsPlayer.tagsNameAnimationEnabled = (value == "1" || value == "true" || value == "True");
                    } else if (key == "Tags/NameAnimationList") {
                        g_config.vampireTearsPlayer.tagsNameAnimationList = value;
                    } else if (key == "Tags/NameAnimationGender") {
                        g_config.vampireTearsPlayer.tagsNameAnimationGender = value;
                    } else if (key == "SpeedTagEnabled") {
                        g_config.vampireTearsPlayer.speedTagEnabled = (value == "1" || value == "true" || value == "True");
                    } else if (key == "LowIntensity") {
                        g_config.vampireTearsPlayer.lowIntensity = (value == "1" || value == "true" || value == "True");
                    } else if (key == "MediumIntensity") {
                        g_config.vampireTearsPlayer.mediumIntensity = (value == "1" || value == "true" || value == "True");
                    } else if (key == "HighIntensity") {
                        g_config.vampireTearsPlayer.highIntensity = (value == "1" || value == "true" || value == "True");
                    } else if (key == "AfterOStim") {
                        g_config.vampireTearsPlayer.afterOStim = (value == "1" || value == "true" || value == "True");
                    } else if (key == "IntervalActiveSecondsAF") {
                        g_config.vampireTearsPlayer.intervalActiveSecondsAF = std::stoi(value);
                    } else if (key == "EVENTAF") {
                        g_config.vampireTearsPlayer.eventAF = std::stoi(value);
                    } else if (key == "Pluginfaction") {
                        g_config.vampireTearsPlayer.pluginFaction = value;
                    } else if (key == "factionName") {
                        g_config.vampireTearsPlayer.factionName = value;
                    }
                } else if (currentSection == "AnimatedBloody_Effect_NPC_cast") {
                    if (key == "Enabled") {
                        g_config.bloodyNoseNPC.enabled = (value == "1" || value == "true" || value == "True");
                    } else if (key == "SpellID") {
                        g_config.bloodyNoseNPC.spellID = value;
                    } else if (key == "Plugin") {
                        g_config.bloodyNoseNPC.plugin = value;
                    } else if (key == "actor") {
                        g_config.bloodyNoseNPC.actor = value;
                    } else if (key == "objetivo") {
                        g_config.bloodyNoseNPC.objetivo = value;
                    } else if (key == "IntervalActiveSeconds") {
                        g_config.bloodyNoseNPC.intervalActiveSeconds = std::stoi(value);
                    } else if (key == "EVENT") {
                        g_config.bloodyNoseNPC.event = value;
                    } else if (key == "Male") {
                        g_config.bloodyNoseNPC.male = (value == "1" || value == "true" || value == "True");
                    } else if (key == "Female") {
                        g_config.bloodyNoseNPC.female = (value == "1" || value == "true" || value == "True");
                    } else if (key == "ShowNotification") {
                        g_config.bloodyNoseNPC.showNotification = (value == "1" || value == "true" || value == "True");
                    } else if (key == "Tags/NameAnimationEnabled") {
                        g_config.bloodyNoseNPC.tagsNameAnimationEnabled = (value == "1" || value == "true" || value == "True");
                    } else if (key == "Tags/NameAnimationList") {
                        g_config.bloodyNoseNPC.tagsNameAnimationList = value;
                    } else if (key == "Tags/NameAnimationGender") {
                        g_config.bloodyNoseNPC.tagsNameAnimationGender = value;
                    } else if (key == "SpeedTagEnabled") {
                        g_config.bloodyNoseNPC.speedTagEnabled = (value == "1" || value == "true" || value == "True");
                    } else if (key == "LowIntensity") {
                        g_config.bloodyNoseNPC.lowIntensity = (value == "1" || value == "true" || value == "True");
                    } else if (key == "MediumIntensity") {
                        g_config.bloodyNoseNPC.mediumIntensity = (value == "1" || value == "true" || value == "True");
                    } else if (key == "HighIntensity") {
                        g_config.bloodyNoseNPC.highIntensity = (value == "1" || value == "true" || value == "True");
                    } else if (key == "AfterOStim") {
                        g_config.bloodyNoseNPC.afterOStim = (value == "1" || value == "true" || value == "True");
                    } else if (key == "IntervalActiveSecondsAF") {
                        g_config.bloodyNoseNPC.intervalActiveSecondsAF = std::stoi(value);
                    } else if (key == "EVENTAF") {
                        g_config.bloodyNoseNPC.eventAF = std::stoi(value);
                    } else if (key == "Pluginfaction") {
                        g_config.bloodyNoseNPC.pluginFaction = value;
                    } else if (key == "factionName") {
                        g_config.bloodyNoseNPC.factionName = value;
                    } else if (key == "BloodyNoses") {
                        g_config.bloodyNoseNPC.bloodyNoses = (value == "1" || value == "true" || value == "True");
                    } else if (key == "BloodyNosescounter") {
                        g_config.bloodyNoseNPC.bloodyNosescounter = std::stoi(value);
                    } else if (key == "BloodyNosesMale") {
                        g_config.bloodyNoseNPC.bloodyNosesMale = (value == "1" || value == "true" || value == "True");
                    } else if (key == "BloodyNosesFemale") {
                        g_config.bloodyNoseNPC.bloodyNosesFemale = (value == "1" || value == "true" || value == "True");
                    } else if (key == "BloodyNosesTimeSeconds") {
                        g_config.bloodyNoseNPC.bloodyNosesTimeSeconds = std::stoi(value);
                    }
                } else if (currentSection == "AnimatedBloody_Effect_PLAYER_cast") {
                    if (key == "Enabled") {
                        g_config.bloodyNosePlayer.enabled = (value == "1" || value == "true" || value == "True");
                    } else if (key == "SpellID") {
                        g_config.bloodyNosePlayer.spellID = value;
                    } else if (key == "Plugin") {
                        g_config.bloodyNosePlayer.plugin = value;
                    } else if (key == "actor") {
                        g_config.bloodyNosePlayer.actor = value;
                    } else if (key == "objetivo") {
                        g_config.bloodyNosePlayer.objetivo = value;
                    } else if (key == "IntervalActiveSeconds") {
                        g_config.bloodyNosePlayer.intervalActiveSeconds = std::stoi(value);
                    } else if (key == "EVENT") {
                        g_config.bloodyNosePlayer.event = value;
                    } else if (key == "Male") {
                        g_config.bloodyNosePlayer.male = (value == "1" || value == "true" || value == "True");
                    } else if (key == "Female") {
                        g_config.bloodyNosePlayer.female = (value == "1" || value == "true" || value == "True");
                    } else if (key == "ShowNotification") {
                        g_config.bloodyNosePlayer.showNotification = (value == "1" || value == "true" || value == "True");
                    } else if (key == "Tags/NameAnimationEnabled") {
                        g_config.bloodyNosePlayer.tagsNameAnimationEnabled = (value == "1" || value == "true" || value == "True");
                    } else if (key == "Tags/NameAnimationList") {
                        g_config.bloodyNosePlayer.tagsNameAnimationList = value;
                    } else if (key == "Tags/NameAnimationGender") {
                        g_config.bloodyNosePlayer.tagsNameAnimationGender = value;
                    } else if (key == "SpeedTagEnabled") {
                        g_config.bloodyNosePlayer.speedTagEnabled = (value == "1" || value == "true" || value == "True");
                    } else if (key == "LowIntensity") {
                        g_config.bloodyNosePlayer.lowIntensity = (value == "1" || value == "true" || value == "True");
                    } else if (key == "MediumIntensity") {
                        g_config.bloodyNosePlayer.mediumIntensity = (value == "1" || value == "true" || value == "True");
                    } else if (key == "HighIntensity") {
                        g_config.bloodyNosePlayer.highIntensity = (value == "1" || value == "true" || value == "True");
                    } else if (key == "AfterOStim") {
                        g_config.bloodyNosePlayer.afterOStim = (value == "1" || value == "true" || value == "True");
                    } else if (key == "IntervalActiveSecondsAF") {
                        g_config.bloodyNosePlayer.intervalActiveSecondsAF = std::stoi(value);
                    } else if (key == "EVENTAF") {
                        g_config.bloodyNosePlayer.eventAF = std::stoi(value);
                    } else if (key == "Pluginfaction") {
                        g_config.bloodyNosePlayer.pluginFaction = value;
                    } else if (key == "factionName") {
                        g_config.bloodyNosePlayer.factionName = value;
                    } else if (key == "BloodyNoses") {
                        g_config.bloodyNosePlayer.bloodyNoses = (value == "1" || value == "true" || value == "True");
                    } else if (key == "BloodyNosescounter") {
                        g_config.bloodyNosePlayer.bloodyNosescounter = std::stoi(value);
                    } else if (key == "BloodyNosesMale") {
                        g_config.bloodyNosePlayer.bloodyNosesMale = (value == "1" || value == "true" || value == "True");
                    } else if (key == "BloodyNosesFemale") {
                        g_config.bloodyNosePlayer.bloodyNosesFemale = (value == "1" || value == "true" || value == "True");
                    } else if (key == "BloodyNosesTimeSeconds") {
                        g_config.bloodyNosePlayer.bloodyNosesTimeSeconds = std::stoi(value);
                    }
                } else if (currentSection == "Attributes") {
                    if (key == "Enabled") {
                        g_config.attributes.enabled = (value == "1" || value == "true" || value == "True");
                    } else if (key == "RestorationAmount") {
                        g_config.attributes.restorationAmount = std::stoi(value);
                    } else if (key == "IntervalSeconds") {
                        g_config.attributes.intervalSeconds = std::stoi(value);
                    } else if (key == "ShowNotification") {
                        g_config.attributes.showNotification = (value == "1" || value == "true" || value == "True");
                    }
                } else if (currentSection == "Item1") {
                    if (key == "Enabled") {
                        g_config.item1.enabled = (value == "1" || value == "true" || value == "True");
                    } else if (key == "ItemName") {
                        g_config.item1.itemName = value;
                    } else if (key == "ID") {
                        g_config.item1.id = value;
                    } else if (key == "Plugin") {
                        g_config.item1.plugin = value;
                    } else if (key == "Amount") {
                        g_config.item1.amount = std::stoi(value);
                    } else if (key == "IntervalMinutes") {
                        g_config.item1.intervalMinutes = std::stoi(value);
                    } else if (key == "ShowNotification") {
                        g_config.item1.showNotification = (value == "1" || value == "true" || value == "True");
                    }
                } else if (currentSection == "Item2") {
                    if (key == "Enabled") {
                        g_config.item2.enabled = (value == "1" || value == "true" || value == "True");
                    } else if (key == "ItemName") {
                        g_config.item2.itemName = value;
                    } else if (key == "ID") {
                        g_config.item2.id = value;
                    } else if (key == "Plugin") {
                        g_config.item2.plugin = value;
                    } else if (key == "Amount") {
                        g_config.item2.amount = std::stoi(value);
                    } else if (key == "IntervalMinutes") {
                        g_config.item2.intervalMinutes = std::stoi(value);
                    } else if (key == "ShowNotification") {
                        g_config.item2.showNotification = (value == "1" || value == "true" || value == "True");
                    }
                } else if (currentSection == "Milk") {
                    if (key == "Enabled") {
                        g_config.milk.enabled = (value == "1" || value == "true" || value == "True");
                    } else if (key == "ID") {
                        g_config.milk.id = value;
                    } else if (key == "Plugin") {
                        g_config.milk.plugin = value;
                    } else if (key == "Amount") {
                        g_config.milk.amount = std::stoi(value);
                    } else if (key == "IntervalMinutes") {
                        g_config.milk.intervalMinutes = std::stoi(value);
                    } else if (key == "ShowNotification") {
                        g_config.milk.showNotification = (value == "1" || value == "true" || value == "True");
                    }
                } else if (currentSection == "BWY_Wench_Milk") {
                    if (key == "Enabled") {
                        g_config.milkWench.enabled = (value == "1" || value == "true" || value == "True");
                    } else if (key == "ID") {
                        g_config.milkWench.id = value;
                    } else if (key == "Plugin") {
                        g_config.milkWench.plugin = value;
                    } else if (key == "Amount") {
                        g_config.milkWench.amount = std::stoi(value);
                    } else if (key == "IntervalMinutes") {
                        g_config.milkWench.intervalMinutes = std::stoi(value);
                    } else if (key == "ShowNotification") {
                        g_config.milkWench.showNotification = (value == "1" || value == "true" || value == "True");
                    }
                } else if (currentSection == "BWY_Milk_Ethel") {
                    if (key == "Enabled") {
                        g_config.milkEthel.enabled = (value == "1" || value == "true" || value == "True");
                    } else if (key == "ID") {
                        g_config.milkEthel.id = value;
                    } else if (key == "PluginItem") {
                        g_config.milkEthel.pluginItem = value;
                    } else if (key == "NPC") {
                        g_config.milkEthel.npc = value;
                    } else if (key == "PluginNPC") {
                        g_config.milkEthel.pluginNPC = value;
                    } else if (key == "Amount") {
                        g_config.milkEthel.amount = std::stoi(value);
                    } else if (key == "IntervalMinutes") {
                        g_config.milkEthel.intervalMinutes = std::stoi(value);
                    } else if (key == "ShowNotification") {
                        g_config.milkEthel.showNotification = (value == "1" || value == "true" || value == "True");
                    }
                } else if (currentSection == "Attributes_EVENT") {
                    if (key == "Enabled") {
                        g_config.attributesEvent.enabled = (value == "1" || value == "true" || value == "True");
                    } else if (key == "RestorationAmount") {
                        g_config.attributesEvent.restorationAmount = std::stoi(value);
                    } else if (key == "EVENT") {
                        g_config.attributesEvent.event = value;
                    } else if (key == "Male") {
                        g_config.attributesEvent.male = (value == "1" || value == "true" || value == "True");
                    } else if (key == "Female") {
                        g_config.attributesEvent.female = (value == "1" || value == "true" || value == "True");
                    } else if (key == "ShowNotification") {
                        g_config.attributesEvent.showNotification = (value == "1" || value == "true" || value == "True");
                    }
                } else if (currentSection == "Item1_EVENT") {
                    if (key == "Enabled") {
                        g_config.item1Event.enabled = (value == "1" || value == "true" || value == "True");
                    } else if (key == "ItemName") {
                        g_config.item1Event.itemName = value;
                    } else if (key == "ID") {
                        g_config.item1Event.id = value;
                    } else if (key == "Plugin") {
                        g_config.item1Event.plugin = value;
                    } else if (key == "Amount") {
                        g_config.item1Event.amount = std::stoi(value);
                    } else if (key == "EVENT") {
                        g_config.item1Event.event = value;
                    } else if (key == "Male") {
                        g_config.item1Event.male = (value == "1" || value == "true" || value == "True");
                    } else if (key == "Female") {
                        g_config.item1Event.female = (value == "1" || value == "true" || value == "True");
                    } else if (key == "ShowNotification") {
                        g_config.item1Event.showNotification = (value == "1" || value == "true" || value == "True");
                    }
                } else if (currentSection == "Item2_EVENT") {
                    if (key == "Enabled") {
                        g_config.item2Event.enabled = (value == "1" || value == "true" || value == "True");
                    } else if (key == "ItemName") {
                        g_config.item2Event.itemName = value;
                    } else if (key == "ID") {
                        g_config.item2Event.id = value;
                    } else if (key == "Plugin") {
                        g_config.item2Event.plugin = value;
                    } else if (key == "Amount") {
                        g_config.item2Event.amount = std::stoi(value);
                    } else if (key == "EVENT") {
                        g_config.item2Event.event = value;
                    } else if (key == "Male") {
                        g_config.item2Event.male = (value == "1" || value == "true" || value == "True");
                    } else if (key == "Female") {
                        g_config.item2Event.female = (value == "1" || value == "true" || value == "True");
                    } else if (key == "ShowNotification") {
                        g_config.item2Event.showNotification = (value == "1" || value == "true" || value == "True");
                    }
                } else if (currentSection == "Milk_EVENT") {
                    if (key == "Enabled") {
                        g_config.milkEvent.enabled = (value == "1" || value == "true" || value == "True");
                    } else if (key == "ID") {
                        g_config.milkEvent.id = value;
                    } else if (key == "Plugin") {
                        g_config.milkEvent.plugin = value;
                    } else if (key == "Amount") {
                        g_config.milkEvent.amount = std::stoi(value);
                    } else if (key == "EVENT") {
                        g_config.milkEvent.event = value;
                    } else if (key == "Male") {
                        g_config.milkEvent.male = (value == "1" || value == "true" || value == "True");
                    } else if (key == "Female") {
                        g_config.milkEvent.female = (value == "1" || value == "true" || value == "True");
                    } else if (key == "ShowNotification") {
                        g_config.milkEvent.showNotification = (value == "1" || value == "true" || value == "True");
                    }
                } else if (currentSection == "BWY_Wench_Milk_EVENT") {
                    if (key == "Enabled") {
                        g_config.milkWenchEvent.enabled = (value == "1" || value == "true" || value == "True");
                    } else if (key == "ID") {
                        g_config.milkWenchEvent.id = value;
                    } else if (key == "Plugin") {
                        g_config.milkWenchEvent.plugin = value;
                    } else if (key == "Amount") {
                        g_config.milkWenchEvent.amount = std::stoi(value);
                    } else if (key == "EVENT") {
                        g_config.milkWenchEvent.event = value;
                    } else if (key == "Male") {
                        g_config.milkWenchEvent.male = (value == "1" || value == "true" || value == "True");
                    } else if (key == "Female") {
                        g_config.milkWenchEvent.female = (value == "1" || value == "true" || value == "True");
                    } else if (key == "ShowNotification") {
                        g_config.milkWenchEvent.showNotification = (value == "1" || value == "true" || value == "True");
                    }
                } else if (currentSection == "BWY_Milk_Ethel_EVENT") {
                    if (key == "Enabled") {
                        g_config.milkEthelEvent.enabled = (value == "1" || value == "true" || value == "True");
                    } else if (key == "ID") {
                        g_config.milkEthelEvent.id = value;
                    } else if (key == "PluginItem") {
                        g_config.milkEthelEvent.pluginItem = value;
                    } else if (key == "NPC") {
                        g_config.milkEthelEvent.npc = value;
                    } else if (key == "PluginNPC") {
                        g_config.milkEthelEvent.pluginNPC = value;
                    } else if (key == "Amount") {
                        g_config.milkEthelEvent.amount = std::stoi(value);
                    } else if (key == "EVENT") {
                        g_config.milkEthelEvent.event = value;
                    } else if (key == "Male") {
                        g_config.milkEthelEvent.male = (value == "1" || value == "true" || value == "True");
                    } else if (key == "Female") {
                        g_config.milkEthelEvent.female = (value == "1" || value == "true" || value == "True");
                    } else if (key == "ShowNotification") {
                        g_config.milkEthelEvent.showNotification = (value == "1" || value == "true" || value == "True");
                    }
                } else if (currentSection == "Notification") {
                    if (key == "Enabled") {
                        g_config.notification.enabled = (value == "1" || value == "true" || value == "True");
                    }
                }
            }
        }
        
        iniFile.close();
    }
    
    return true;
}

void ValidateAndUpdatePluginsInINI() {
    auto* dataHandler = RE::TESDataHandler::GetSingleton();
    if (!dataHandler) {
        return;
    }

    bool needsUpdate = false;
    PluginConfig originalConfig = g_config;

    if (g_config.item1.enabled) {
        if (g_config.item1.plugin != "none") {
            auto* item1Plugin = dataHandler->LookupModByName(g_config.item1.plugin);
            if (!item1Plugin) {
                g_config.item1.enabled = false;
                needsUpdate = true;
                WriteToActionsLog("Plugin not found: " + g_config.item1.plugin + " - Disabled [Item1] in memory", __LINE__);
            }
        }
    }

    if (g_config.item2.enabled) {
        if (g_config.item2.plugin != "none") {
            auto* item2Plugin = dataHandler->LookupModByName(g_config.item2.plugin);
            if (!item2Plugin) {
                g_config.item2.enabled = false;
                needsUpdate = true;
                WriteToActionsLog("Plugin not found: " + g_config.item2.plugin + " - Disabled [Item2] in memory", __LINE__);
            }
        }
    }

    if (g_config.milk.enabled) {
        auto* milkPlugin = dataHandler->LookupModByName(g_config.milk.plugin);
        if (!milkPlugin) {
            g_config.milk.enabled = false;
            needsUpdate = true;
            WriteToActionsLog("Plugin not found: " + g_config.milk.plugin + " - Disabled [Milk] in memory", __LINE__);
        }
    }

    if (g_config.milkWench.enabled) {
        auto* wenchPlugin = dataHandler->LookupModByName(g_config.milkWench.plugin);
        if (!wenchPlugin) {
            g_config.milkWench.enabled = false;
            needsUpdate = true;
            WriteToActionsLog("Plugin not found: " + g_config.milkWench.plugin + " - Disabled [BWY_Wench_Milk] in memory", __LINE__);
        }
    }

    if (g_config.milkEthel.enabled) {
        auto* ethelPluginItem = dataHandler->LookupModByName(g_config.milkEthel.pluginItem);
        auto* ethelPluginNPC = dataHandler->LookupModByName(g_config.milkEthel.pluginNPC);
        if (!ethelPluginItem || !ethelPluginNPC) {
            g_config.milkEthel.enabled = false;
            needsUpdate = true;
            WriteToActionsLog("Plugin not found for Ethel - Disabled [BWY_Milk_Ethel] in memory", __LINE__);
        }
    }

    if (g_config.emotionalTearsNPC.enabled) {
        auto* emotionalPlugin = dataHandler->LookupModByName(g_config.emotionalTearsNPC.plugin);
        if (!emotionalPlugin) {
            g_config.emotionalTearsNPC.enabled = false;
            needsUpdate = true;
            WriteToActionsLog("Plugin not found: " + g_config.emotionalTearsNPC.plugin + " - Disabled [Emotional_Tears_Effect_NPC_cast] in memory", __LINE__);
        }
    }

    if (g_config.emotionalTearsPlayer.enabled) {
        auto* emotionalPlugin = dataHandler->LookupModByName(g_config.emotionalTearsPlayer.plugin);
        if (!emotionalPlugin) {
            g_config.emotionalTearsPlayer.enabled = false;
            needsUpdate = true;
            WriteToActionsLog("Plugin not found: " + g_config.emotionalTearsPlayer.plugin + " - Disabled [Emotional_Tears_Effect_PLAYER_cast] in memory", __LINE__);
        }
    }

    if (g_config.vampireTearsNPC.enabled) {
        auto* vampirePlugin = dataHandler->LookupModByName(g_config.vampireTearsNPC.plugin);
        if (!vampirePlugin) {
            g_config.vampireTearsNPC.enabled = false;
            needsUpdate = true;
            WriteToActionsLog("Plugin not found: " + g_config.vampireTearsNPC.plugin + " - Disabled [AnimatedVampireTears_Effect_NPC_cast] in memory", __LINE__);
        }
    }

    if (g_config.vampireTearsPlayer.enabled) {
        auto* vampirePlugin = dataHandler->LookupModByName(g_config.vampireTearsPlayer.plugin);
        if (!vampirePlugin) {
            g_config.vampireTearsPlayer.enabled = false;
            needsUpdate = true;
            WriteToActionsLog("Plugin not found: " + g_config.vampireTearsPlayer.plugin + " - Disabled [AnimatedVampireTears_Effect_PLAYER_cast] in memory", __LINE__);
        }
    }

    if (g_config.bloodyNoseNPC.enabled) {
        auto* bloodyPlugin = dataHandler->LookupModByName(g_config.bloodyNoseNPC.plugin);
        if (!bloodyPlugin) {
            g_config.bloodyNoseNPC.enabled = false;
            needsUpdate = true;
            WriteToActionsLog("Plugin not found: " + g_config.bloodyNoseNPC.plugin + " - Disabled [AnimatedBloody_Effect_NPC_cast] in memory", __LINE__);
        }
    }

    if (g_config.bloodyNosePlayer.enabled) {
        auto* bloodyPlugin = dataHandler->LookupModByName(g_config.bloodyNosePlayer.plugin);
        if (!bloodyPlugin) {
            g_config.bloodyNosePlayer.enabled = false;
            needsUpdate = true;
            WriteToActionsLog("Plugin not found: " + g_config.bloodyNosePlayer.plugin + " - Disabled [AnimatedBloody_Effect_PLAYER_cast] in memory", __LINE__);
        }
    }

    if (g_config.item1Event.enabled) {
        if (g_config.item1Event.plugin != "none") {
            auto* item1EventPlugin = dataHandler->LookupModByName(g_config.item1Event.plugin);
            if (!item1EventPlugin) {
                g_config.item1Event.enabled = false;
                needsUpdate = true;
                WriteToActionsLog("Plugin not found: " + g_config.item1Event.plugin + " - Disabled [Item1_EVENT] in memory", __LINE__);
            }
        }
    }

    if (g_config.item2Event.enabled) {
        if (g_config.item2Event.plugin != "none") {
            auto* item2EventPlugin = dataHandler->LookupModByName(g_config.item2Event.plugin);
            if (!item2EventPlugin) {
                g_config.item2Event.enabled = false;
                needsUpdate = true;
                WriteToActionsLog("Plugin not found: " + g_config.item2Event.plugin + " - Disabled [Item2_EVENT] in memory", __LINE__);
            }
        }
    }

    if (g_config.milkEvent.enabled) {
        auto* milkEventPlugin = dataHandler->LookupModByName(g_config.milkEvent.plugin);
        if (!milkEventPlugin) {
            g_config.milkEvent.enabled = false;
            needsUpdate = true;
            WriteToActionsLog("Plugin not found: " + g_config.milkEvent.plugin + " - Disabled [Milk_EVENT] in memory", __LINE__);
        }
    }

    if (g_config.milkWenchEvent.enabled) {
        auto* milkWenchEventPlugin = dataHandler->LookupModByName(g_config.milkWenchEvent.plugin);
        if (!milkWenchEventPlugin) {
            g_config.milkWenchEvent.enabled = false;
            needsUpdate = true;
            WriteToActionsLog("Plugin not found: " + g_config.milkWenchEvent.plugin + " - Disabled [BWY_Wench_Milk_EVENT] in memory", __LINE__);
        }
    }

    if (g_config.milkEthelEvent.enabled) {
        auto* milkEthelEventPluginItem = dataHandler->LookupModByName(g_config.milkEthelEvent.pluginItem);
        auto* milkEthelEventPluginNPC = dataHandler->LookupModByName(g_config.milkEthelEvent.pluginNPC);
        if (!milkEthelEventPluginItem || !milkEthelEventPluginNPC) {
            g_config.milkEthelEvent.enabled = false;
            needsUpdate = true;
            WriteToActionsLog("Plugin not found for Ethel EVENT - Disabled [BWY_Milk_Ethel_EVENT] in memory", __LINE__);
        }
    }

    if (needsUpdate) {
        WriteToActionsLog("Plugin validation completed - Some features disabled in memory due to missing plugins", __LINE__);
        WriteToActionsLog("User INI files preserved - NO modifications made to configuration files", __LINE__);
    }
}

std::string GetDocumentsPath() {
    try {
        wchar_t path[MAX_PATH] = {0};
        HRESULT result = SHGetFolderPathW(NULL, CSIDL_PERSONAL, NULL, SHGFP_TYPE_CURRENT, path);
        if (SUCCEEDED(result)) {
            std::wstring ws(path);
            std::string converted = SafeWideStringToString(ws);
            if (!converted.empty()) {
                return converted;
            }
        }
        std::string userProfile = GetEnvVar("USERPROFILE");
        if (!userProfile.empty()) {
            return userProfile + "\\Documents";
        }
        return "C:\\Users\\Default\\Documents";
    } catch (...) {
        return "C:\\Users\\Default\\Documents";
    }
}

bool IsValidPluginPath(const fs::path& pluginPath) {
    const std::vector<std::string> dllNames = {
        "ORisk-and-Reward-NG.dll"
    };
    
    for (const auto& dllName : dllNames) {
        fs::path dllPath = pluginPath / dllName;
        try {
            if (fs::exists(dllPath)) {
                return true;
            }
        } catch (...) {
            continue;
        }
    }
    return false;
}

fs::path BuildPathCaseInsensitive(const fs::path& basePath, const std::vector<std::string>& components) {
    try {
        fs::path currentPath = basePath;
        
        for (const auto& component : components) {
            fs::path testPath = currentPath / component;
            if (fs::exists(testPath)) {
                currentPath = testPath;
                continue;
            }
            
            std::string lowerComponent = component;
            std::transform(lowerComponent.begin(), lowerComponent.end(), 
                         lowerComponent.begin(), ::tolower);
            testPath = currentPath / lowerComponent;
            if (fs::exists(testPath)) {
                currentPath = testPath;
                continue;
            }
            
            std::string upperComponent = component;
            std::transform(upperComponent.begin(), upperComponent.end(), 
                         upperComponent.begin(), ::toupper);
            testPath = currentPath / upperComponent;
            if (fs::exists(testPath)) {
                currentPath = testPath;
                continue;
            }
            
            bool found = false;
            if (fs::exists(currentPath) && fs::is_directory(currentPath)) {
                for (const auto& entry : fs::directory_iterator(currentPath)) {
                    try {
                        std::string entryName = entry.path().filename().string();
                        std::string lowerEntryName = entryName;
                        std::transform(lowerEntryName.begin(), lowerEntryName.end(), 
                                     lowerEntryName.begin(), ::tolower);
                        
                        if (lowerEntryName == lowerComponent) {
                            currentPath = entry.path();
                            found = true;
                            break;
                        }
                    } catch (...) {
                        continue;
                    }
                }
            }
            
            if (!found) {
                currentPath = currentPath / component;
            }
        }
        
        return currentPath;
        
    } catch (...) {
        return basePath;
    }
}

fs::path GetDllDirectory() {
    try {
        HMODULE hModule = nullptr;
        static int dummyVariable = 0;

        if (GetModuleHandleExA(
            GET_MODULE_HANDLE_EX_FLAG_FROM_ADDRESS | 
            GET_MODULE_HANDLE_EX_FLAG_UNCHANGED_REFCOUNT,
            reinterpret_cast<LPCSTR>(&dummyVariable), 
            &hModule) && hModule != nullptr) {
            
            wchar_t dllPath[MAX_PATH] = {0};
            DWORD size = GetModuleFileNameW(hModule, dllPath, MAX_PATH);

            if (size > 0) {
                std::wstring wsDllPath(dllPath);
                std::string dllPathStr = SafeWideStringToString(wsDllPath);

                if (!dllPathStr.empty()) {
                    fs::path dllDir = fs::path(dllPathStr).parent_path();
                    return dllDir;
                }
            }
        }

        return fs::path();

    } catch (...) {
        return fs::path();
    }
}

std::string GetGamePath() {
    try {
        std::string mo2Path = GetEnvVar("MO2_MODS_PATH");
        if (!mo2Path.empty()) {
            fs::path testPath = BuildPathCaseInsensitive(
                fs::path(mo2Path), {"Data", "SKSE", "Plugins"}
            );
            if (IsValidPluginPath(testPath)) {
                WriteToAnimationsLog("Game path detected: MO2 Environment Variable", __LINE__);
                return mo2Path;
            }
        }

        std::string mo2Overwrite = GetEnvVar("MO_OVERWRITE_PATH");
        if (!mo2Overwrite.empty()) {
            fs::path testPath = BuildPathCaseInsensitive(
                fs::path(mo2Overwrite), {"SKSE", "Plugins"}
            );
            if (IsValidPluginPath(testPath)) {
                WriteToAnimationsLog("Game path detected: MO2 Overwrite Path", __LINE__);
                return mo2Overwrite;
            }
        }

        std::string vortexPath = GetEnvVar("VORTEX_MODS_PATH");
        if (!vortexPath.empty()) {
            fs::path testPath = BuildPathCaseInsensitive(
                fs::path(vortexPath), {"Data", "SKSE", "Plugins"}
            );
            if (IsValidPluginPath(testPath)) {
                WriteToAnimationsLog("Game path detected: Vortex Environment Variable", __LINE__);
                return vortexPath;
            }
        }

        std::string skyrimMods = GetEnvVar("SKYRIM_MODS_FOLDER");
        if (!skyrimMods.empty()) {
            fs::path testPath = BuildPathCaseInsensitive(
                fs::path(skyrimMods), {"Data", "SKSE", "Plugins"}
            );
            if (IsValidPluginPath(testPath)) {
                WriteToAnimationsLog("Game path detected: SKYRIM_MODS_FOLDER Variable", __LINE__);
                return skyrimMods;
            }
        }

        std::vector<std::string> registryKeys = {
            "SOFTWARE\\WOW6432Node\\Bethesda Softworks\\Skyrim Special Edition",
            "SOFTWARE\\Bethesda Softworks\\Skyrim Special Edition",
            "SOFTWARE\\WOW6432Node\\GOG.com\\Games\\1457087920",
            "SOFTWARE\\GOG.com\\Games\\1457087920",
            "SOFTWARE\\WOW6432Node\\Valve\\Steam\\Apps\\489830",
            "SOFTWARE\\WOW6432Node\\Valve\\Steam\\Apps\\611670"
        };

        HKEY hKey;
        char pathBuffer[MAX_PATH] = {0};
        DWORD pathSize = sizeof(pathBuffer);

        for (const auto& key : registryKeys) {
            if (RegOpenKeyExA(HKEY_LOCAL_MACHINE, key.c_str(), 0, KEY_READ, &hKey) == ERROR_SUCCESS) {
                if (RegQueryValueExA(hKey, "Installed Path", NULL, NULL, (LPBYTE)pathBuffer, &pathSize) == ERROR_SUCCESS) {
                    RegCloseKey(hKey);
                    std::string result(pathBuffer);
                    if (!result.empty()) {
                        fs::path testPath = BuildPathCaseInsensitive(
                            fs::path(result), {"Data", "SKSE", "Plugins"}
                        );
                        if (IsValidPluginPath(testPath)) {
                            WriteToAnimationsLog("Game path detected: Windows Registry", __LINE__);
                            return result;
                        }
                    }
                }
                RegCloseKey(hKey);
            }
            pathSize = sizeof(pathBuffer);
        }

        std::vector<std::string> commonPaths = {
            "C:\\Program Files (x86)\\Steam\\steamapps\\common\\Skyrim Special Edition",
            "C:\\Program Files\\Steam\\steamapps\\common\\Skyrim Special Edition",
            "D:\\Steam\\steamapps\\common\\Skyrim Special Edition",
            "D:\\SteamLibrary\\steamapps\\common\\Skyrim Special Edition",
            "E:\\Steam\\steamapps\\common\\Skyrim Special Edition",
            "E:\\SteamLibrary\\steamapps\\common\\Skyrim Special Edition",
            "F:\\Steam\\steamapps\\common\\Skyrim Special Edition",
            "F:\\SteamLibrary\\steamapps\\common\\Skyrim Special Edition",
            "G:\\Steam\\steamapps\\common\\Skyrim Special Edition",
            "G:\\SteamLibrary\\steamapps\\common\\Skyrim Special Edition",
            "C:\\GOG Games\\Skyrim Special Edition",
            "D:\\GOG Games\\Skyrim Special Edition",
            "E:\\GOG Games\\Skyrim Special Edition",
            "C:\\Games\\Skyrim Special Edition",
            "D:\\Games\\Skyrim Special Edition"
        };

        for (const auto& pathCandidate : commonPaths) {
            try {
                if (fs::exists(pathCandidate) && fs::is_directory(pathCandidate)) {
                    fs::path testPath = BuildPathCaseInsensitive(
                        fs::path(pathCandidate), {"Data", "SKSE", "Plugins"}
                    );
                    if (IsValidPluginPath(testPath)) {
                        WriteToAnimationsLog("Game path detected: Common Installation Path", __LINE__);
                        return pathCandidate;
                    }
                }
            } catch (...) {
                continue;
            }
        }

        WriteToAnimationsLog("Attempting DLL Directory Detection (Wabbajack/MO2/Portable fallback)...", __LINE__);
        fs::path dllDir = GetDllDirectory();
        
        if (!dllDir.empty()) {
            if (IsValidPluginPath(dllDir)) {
                fs::path calculatedGamePath = dllDir.parent_path().parent_path().parent_path();
                WriteToAnimationsLog("Game path detected: DLL Directory Method (Wabbajack/Portable)", __LINE__);
                WriteToAnimationsLog("Calculated game path: " + calculatedGamePath.string(), __LINE__);
                return calculatedGamePath.string();
            }
        }

        WriteToAnimationsLog("WARNING: No valid game path detected, using default fallback", __LINE__);
        return "C:\\Program Files (x86)\\Steam\\steamapps\\common\\Skyrim Special Edition";
        
    } catch (...) {
        return "";
    }
}

SKSELogsPaths GetAllSKSELogsPaths() {
    SKSELogsPaths paths;

    try {
        std::string docs = GetDocumentsPath();

        fs::path primaryBase = fs::path(docs) / "My Games" / "Skyrim Special Edition" / "SKSE";
        paths.primary = primaryBase;

        fs::path secondaryBase = fs::path(docs) / "My Games" / "Skyrim.INI" / "SKSE";
        paths.secondary = secondaryBase;

        if (fs::exists(paths.primary)) {
            logger::info("Primary path exists and is accessible");
        }

        if (fs::exists(paths.secondary)) {
            logger::info("Secondary path exists and is accessible");
        }

    } catch (const std::exception& e) {
        logger::error("Error detecting SKSE paths: {}", e.what());
    }

    return paths;
}

void SetupLog() {
    auto logsFolder = SKSE::log::log_directory();
    if (!logsFolder) {
        SKSE::stl::report_and_fail("SKSE log_directory not provided, logs disabled.");
        return;
    }
    auto pluginName = SKSE::PluginDeclaration::GetSingleton()->GetName();
    auto logFilePath = *logsFolder / std::format("{}.log", pluginName);
    auto fileLoggerPtr = std::make_shared<spdlog::sinks::basic_file_sink_mt>(logFilePath.string(), true);
    auto loggerPtr = std::make_shared<spdlog::logger>("log", std::move(fileLoggerPtr));
    spdlog::set_default_logger(std::move(loggerPtr));
    spdlog::set_level(spdlog::level::trace);
    spdlog::flush_on(spdlog::level::info);
}

void InitializePlugin() {
    try {
        LoadConfiguration();

        g_documentsPath = GetDocumentsPath();
        g_gamePath = GetGamePath();
        g_ostimLogPaths = GetAllSKSELogsPaths();

        if (g_gamePath.empty()) {
            g_gamePath = "C:\\Program Files (x86)\\Steam\\steamapps\\common\\Skyrim Special Edition";
        }

        auto logsFolder = SKSE::log::log_directory();
        if (logsFolder) {
            auto actionsLogPath = *logsFolder / "ORisk-and-Reward-NG-Actions.log";
            std::ofstream clearActions(actionsLogPath, std::ios::trunc);
            clearActions.close();

            auto animationsLogPath = *logsFolder / "ORisk-and-Reward-NG-Animations.log";
            std::ofstream clearAnimations(animationsLogPath, std::ios::trunc);
            clearAnimations.close();

            auto eventsLogPath = *logsFolder / "ORisk-and-Reward-NG-OStimEvents.log";
            std::ofstream clearEvents(eventsLogPath, std::ios::trunc);
            clearEvents.close();

            std::vector<fs::path> ostimLogPaths = {g_ostimLogPaths.primary / "OStim.log",
                                                   g_ostimLogPaths.secondary / "OStim.log"};

            bool ostimLogFound = false;
            for (const auto& ostimLogPath : ostimLogPaths) {
                if (fs::exists(ostimLogPath)) {
                    auto fileSize = fs::file_size(ostimLogPath);
                    ostimLogFound = true;
                    break;
                }
            }

            if (!ostimLogFound) {
                WriteToAnimationsLog("OStim.log not found - waiting for OStim to create it", __LINE__);
            }
        }

        WriteToAnimationsLog("ORisk-and-Reward-NG Plugin - Starting", __LINE__);
        WriteToAnimationsLog("========================================", __LINE__);
        WriteToAnimationsLog("ORisk-and-Reward-NG Plugin - v4.5.4", __LINE__);
        WriteToAnimationsLog("Started: " + GetCurrentTimeString(), __LINE__);
        WriteToAnimationsLog("========================================", __LINE__);
        WriteToAnimationsLog("Documents: " + g_documentsPath, __LINE__);
        WriteToAnimationsLog("Game Path: " + g_gamePath, __LINE__);
        WriteToAnimationsLog("Primary SKSE Path: " + g_ostimLogPaths.primary.string(), __LINE__);
        WriteToAnimationsLog("Secondary SKSE Path: " + g_ostimLogPaths.secondary.string(), __LINE__);
        WriteToAnimationsLog("Plugin initialized successfully", __LINE__);

        WriteToActionsLog("========================================", __LINE__);
        WriteToActionsLog("ORisk-and-Reward-NG Actions Monitor - v4.5.4", __LINE__);
        WriteToActionsLog("Started: " + GetCurrentTimeString(), __LINE__);
        WriteToActionsLog("========================================", __LINE__);
        WriteToActionsLog("Configuration loaded from multi-INI system", __LINE__);
        WriteToActionsLog("Systems active: Spell Effects (3 types), Milk Events, NPC Detection, Vampire Detection, BloodyNose Counter", __LINE__);

        WriteToOStimEventsLog("========================================", __LINE__);
        WriteToOStimEventsLog("ORisk-and-Reward-NG OStim Events Monitor - v4.5.4", __LINE__);
        WriteToOStimEventsLog("Started: " + GetCurrentTimeString(), __LINE__);
        WriteToOStimEventsLog("========================================", __LINE__);
        WriteToOStimEventsLog("System initialized successfully", __LINE__);
        WriteToOStimEventsLog("Active features: Multiple Spell Systems, Tags/Animation, Event Tracking, Vampire Detection, BloodyNose Counter", __LINE__);

        g_isSlowPC = DetectSlowPC();
        
        if (g_isSlowPC) {
            WriteToActionsLog("========================================", __LINE__);
            WriteToActionsLog("SLOW PC DETECTED - Compatibility mode enabled", __LINE__);
            WriteToActionsLog("Spell casting will use retry system", __LINE__);
            WriteToActionsLog("This prevents crashes on slower hardware", __LINE__);
            WriteToActionsLog("========================================", __LINE__);
            
            WriteToAnimationsLog("========================================", __LINE__);
            WriteToAnimationsLog("SLOW PC DETECTED - Applying compatibility system", __LINE__);
            WriteToAnimationsLog("Non-blocking retry system active for spell casting", __LINE__);
            WriteToAnimationsLog("========================================", __LINE__);
        } else {
            WriteToActionsLog("========================================", __LINE__);
            WriteToActionsLog("FAST PC DETECTED - Full performance mode", __LINE__);
            WriteToActionsLog("Spell casting at maximum speed", __LINE__);
            WriteToActionsLog("Retry system available if needed", __LINE__);
            WriteToActionsLog("========================================", __LINE__);
            
            WriteToAnimationsLog("========================================", __LINE__);
            WriteToAnimationsLog("FAST PC DETECTED - Maximum performance enabled", __LINE__);
            WriteToAnimationsLog("Retry system ready for edge cases", __LINE__);
            WriteToAnimationsLog("========================================", __LINE__);
        }

        g_isInitialized = true;
        WriteToAnimationsLog("PLUGIN INITIALIZED", __LINE__);
        WriteToAnimationsLog("PLUGIN FULLY ACTIVE", __LINE__);
        WriteToAnimationsLog("========================================", __LINE__);
        WriteToAnimationsLog("Starting OStim animation monitoring", __LINE__);

        auto* modEventSource = SKSE::GetModCallbackEventSource();
        if (modEventSource) {
            modEventSource->AddEventSink(&OStimModEventSink::GetSingleton());
            WriteToOStimEventsLog("OStim Mod Event Sink registered successfully", __LINE__);
            WriteToOStimEventsLog("Now listening for all ostim_ events from OStim framework", __LINE__);
        } else {
            WriteToOStimEventsLog("WARNING: Failed to register Mod Event Sink - OStim events will not be detected", __LINE__);
        }

        StartMonitoringThread();
        StartFileWatch();

    } catch (const std::exception& e) {
        logger::error("CRITICAL ERROR in Initialize: {}", e.what());
    }
}

void ShutdownPlugin() {
    WriteToAnimationsLog("PLUGIN SHUTTING DOWN", __LINE__);
    WriteToActionsLog("PLUGIN SHUTTING DOWN", __LINE__);
    WriteToOStimEventsLog("PLUGIN SHUTTING DOWN", __LINE__);

    g_isShuttingDown = true;

    CleanupSpellEffectsFromLog();
    CleanupSpellEffectsByFaction();
    DeactivateAllSpellEffects();

    auto* modEventSource = SKSE::GetModCallbackEventSource();
    if (modEventSource) {
        modEventSource->RemoveEventSink(&OStimModEventSink::GetSingleton());
        WriteToOStimEventsLog("OStim Mod Event Sink unregistered", __LINE__);
    }

    StopFileWatch();
    StopMonitoringThread();

    WriteToAnimationsLog("========================================", __LINE__);
    WriteToAnimationsLog("Plugin shutdown complete at: " + GetCurrentTimeString(), __LINE__);
    WriteToAnimationsLog("========================================", __LINE__);
    
    WriteToOStimEventsLog("========================================", __LINE__);
    WriteToOStimEventsLog("Plugin shutdown complete at: " + GetCurrentTimeString(), __LINE__);
    WriteToOStimEventsLog("========================================", __LINE__);
}

void MessageListener(SKSE::MessagingInterface::Message* message) {
    switch (message->type) {
        case SKSE::MessagingInterface::kNewGame:
            StopFileWatch();
            StopMonitoringThread();
            g_lastOStimLogPosition = 0;
            g_lastFileSize = 0;
            g_processedLines.clear();
            SetLastAnimation("");
            g_initialDelayComplete = false;
            SetInOStimScene(false);
            g_goldRewardActive = false;
            g_item1RewardActive = false;
            g_item2RewardActive = false;
            g_milkRewardActive = false;
            g_milkWenchRewardActive = false;
            g_milkEthelRewardActive = false;
            g_attributesRestorationActive = false;
            g_wenchMilkNPCDetected = false;
            g_ethelNPCDetected = false;
            g_capturedYurianaWenchNPC.captured = false;
            g_capturedYurianaWenchNPC.formID = 0;
            g_capturedEthelNPC.captured = false;
            g_capturedEthelNPC.formID = 0;
            g_cachedItemFormIDs.resolved = false;
            g_cachedItemFormIDs.item1 = 0;
            g_cachedItemFormIDs.item2 = 0;
            g_cachedItemFormIDs.milkDawnguard = 0;
            g_cachedItemFormIDs.milkWench = 0;
            g_cachedItemFormIDs.milkEthel = 0;
            g_cachedItemFormIDs.item1Event = 0;
            g_cachedItemFormIDs.item2Event = 0;
            g_cachedItemFormIDs.milkEvent = 0;
            g_cachedItemFormIDs.milkWenchEvent = 0;
            g_cachedItemFormIDs.milkEthelEvent = 0;
            g_cachedSpellFormIDs.resolved = false;
            g_cachedSpellFormIDs.emotionalTearsNPC = 0;
            g_cachedSpellFormIDs.emotionalTearsPlayer = 0;
            g_cachedSpellFormIDs.vampireTearsNPC = 0;
            g_cachedSpellFormIDs.vampireTearsPlayer = 0;
            g_cachedSpellFormIDs.bloodyNoseNPC = 0;
            g_cachedSpellFormIDs.bloodyNosePlayer = 0;
            g_cachedFactionIDs.resolved = false;
            g_cachedFactionIDs.emotionalTearsFaction = 0;
            g_cachedFactionIDs.vampireTearsFaction = 0;
            g_cachedFactionIDs.bloodyNoseFaction = 0;
            g_detectedNPCNames.clear();
            g_npcNameToRefID.clear();
            g_activeSpellEffects.clear();
            g_currentOStimTags.clear();
            g_currentOStimSpeed = 0;
            g_currentAnimationInfo = AnimationTagInfo{};
            g_sceneActors.clear();
            g_lastOrgasmTimestamps.clear();
            g_pendingSpellCasts.clear();
            g_wenchPluginChecked = false;
            g_wenchPluginExists = false;
            g_ethelPluginChecked = false;
            g_ethelPluginExists = false;
            g_lastProcessedAnimationForTags = "";
            g_vampireTearsPluginDetected = false;
            ClearOrgasmCounters();
            ClearBloodyNoseCounters();
            CheckVampireTearsPluginAvailability();
            InitializePlugin();
            break;

        case SKSE::MessagingInterface::kPostLoadGame:
            if (!g_monitoringActive) {
                StartMonitoringThread();
            }
            if (!g_fileWatchActive) {
                StartFileWatch();
            }
            g_wenchPluginChecked = false;
            g_wenchPluginExists = false;
            g_ethelPluginChecked = false;
            g_ethelPluginExists = false;
            g_cachedSpellFormIDs.resolved = false;
            g_cachedSpellFormIDs.emotionalTearsNPC = 0;
            g_cachedSpellFormIDs.emotionalTearsPlayer = 0;
            g_cachedSpellFormIDs.vampireTearsNPC = 0;
            g_cachedSpellFormIDs.vampireTearsPlayer = 0;
            g_cachedSpellFormIDs.bloodyNoseNPC = 0;
            g_cachedSpellFormIDs.bloodyNosePlayer = 0;
            g_cachedFactionIDs.resolved = false;
            g_cachedFactionIDs.emotionalTearsFaction = 0;
            g_cachedFactionIDs.vampireTearsFaction = 0;
            g_cachedFactionIDs.bloodyNoseFaction = 0;
            g_vampireTearsPluginDetected = false;
            CheckVampireTearsPluginAvailability();
            InitializeSpellCache();
            InitializeFactionCache();
            break;

        case SKSE::MessagingInterface::kDataLoaded:
            {
                auto& eventProcessor = GameEventProcessor::GetSingleton();
                RE::UI::GetSingleton()->AddEventSink<RE::MenuOpenCloseEvent>(&eventProcessor);
                WriteToAnimationsLog("Game event processor registered", __LINE__);
                WriteToActionsLog("Event monitoring system active", __LINE__);
                
                ValidateAndUpdatePluginsInINI();
                
                InitializeSpellCache();
                InitializeFactionCache();
                CheckVampireTearsPluginAvailability();
            }
            break;

        default:
            break;
    }
}

SKSEPluginLoad(const SKSE::LoadInterface* a_skse) {
    SKSE::Init(a_skse);
    SetupLog();

    logger::info("ORisk-and-Reward-NG Plugin v4.5.4 - Starting");

    InitializePlugin();

    SKSE::GetMessagingInterface()->RegisterListener(MessageListener);

    logger::info("Plugin loaded successfully");

    return true;
}

constinit auto SKSEPlugin_Version = []() {
    SKSE::PluginVersionData v;
    v.PluginVersion({4, 5, 4});
    v.PluginName("ORisk-and-Reward-NG OStim Monitor");
    v.AuthorName("John95AC");
    v.UsesAddressLibrary();
    v.UsesSigScanning();
    v.CompatibleVersions({SKSE::RUNTIME_SSE_LATEST, SKSE::RUNTIME_LATEST_VR});

    return v;
}();