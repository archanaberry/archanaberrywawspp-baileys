#include <iostream>
#include <string>
#include <map>
#include <mutex>
#include <future>
#include <string>
#include <unordered_map>
#include <functional>
#include <unordered_set>
#include <vector>
// Include other necessary C++ headers

// Define the SocketConfig class to hold configuration options
class SocketConfig {
public:
    

// Define the ChatsSocket class to encapsulate the chat socket logic
class ChatsSocket {
std::map<std::string, std::string> privacySettings;
    bool needToFlushWithAppStateSync = false;
    bool pendingAppStateSync = false;
    std::mutex processingMutex; // Mutex to ensure ordered processing
    
    // Private members for socket state, auth state, etc.
    SocketConfig config;

public:
void fetchPrivacySettings();
    void upsertMessage(const Message& msg);
    void appPatch(const Patch& patch);
    void sendPresenceUpdate(const std::string& presence);
    void presenceSubscribe(const std::vector<std::string>& contacts);
    std::string profilePictureUrl(const std::string& contactId);
    bool onWhatsApp(const std::string& contactId);
    void fetchBlocklist();
    void fetchStatus(const std::string& contactId);
    void updateProfilePicture(const std::string& picturePath);
    void removeProfilePicture();
    void updateProfileStatus(const std::string& status);
    void updateProfileName(const std::string& name);
    void updateBlockStatus(const std::string& contactId, bool blocked);
    void updateLastSeenPrivacy(const std::string& privacySetting);
    void updateOnlinePrivacy(const std::string& privacySetting);
    void updateProfilePicturePrivacy(const std::string& privacySetting);
    void updateStatusPrivacy(const std::string& privacySetting);
    void updateReadReceiptsPrivacy(const std::string& privacySetting);
    void updateGroupsAddPrivacy(const std::string& privacySetting);
    void updateDefaultDisappearingMode(const std::string& mode);
    BusinessProfile getBusinessProfile(const std::string& contactId);
    void resyncAppState(const std::vector<std::string>& patchNames, bool forceResync);
    void chatModify(const ChatModification& modification);
    void cleanDirtyBits(const std::string& type, long long lastTimestamp);
    void addChatLabel(const std::string& chatId, const std::string& labelId);
    void removeChatLabel(const std::string& chatId, const std::string& labelId);
    void addMessageLabel(const std::string& messageId, const std::string& labelId);
    // Constructor that takes a SocketConfig
    ChatsSocket(const SocketConfig& config) : config(config) {
        // Initialize the socket with the provided configuration
        // This would involve setting up the WebSocket connection, authentication, etc.
            // Helper function to fetch the given app state sync key
    std::string getAppStateSyncKey(const std::string& keyId) {
        // Implementation to fetch the app state sync key
        // This would involve interacting with the authState and keys storage
        // You would need to replace this with actual C++ code to access the storage
        
std::function<void()> logger;
    bool markOnlineOnConnect;
    bool fireInitQueries;
    bool appStateMacVerification;
    std::function<bool(const std::string&)> shouldIgnoreJid;
    bool shouldSyncHistoryMessage;

    // Constructor with default values or parameters to initialize the configuration
    SocketConfig(
        std::function<void()> logger = [](){},
        bool markOnlineOnConnect = true,
        bool fireInitQueries = true,
        bool appStateMacVerification = true,
        std::function<bool(const std::string&)> shouldIgnoreJid = [](const std::string&){ return false; },
        bool shouldSyncHistoryMessage = true
    ) : logger(logger),
        markOnlineOnConnect(markOnlineOnConnect),
        fireInitQueries(fireInitQueries),
        appStateMacVerification(appStateMacVerification),
        shouldIgnoreJid(shouldIgnoreJid),
        shouldSyncHistoryMessage(shouldSyncHistoryMessage)
    {}
};

        return ""; // Placeholder return value
    }

    // Function to fetch privacy settings
    std::map<std::string, std::string> fetchPrivacySettings(bool force = false) {
        if (privacySettings.empty() || force) {
            // Implementation to fetch privacy settings
            // This would involve sending a query to the server and parsing the response
            // You would need to replace this with actual C++ networking and parsing code
            // For example, using a WebSocket library and an XML or binary parser
        }

    }

    // Public methods to interact with the chat socket
    // ...
};

// Main function to demonstrate usage
int main() {
    // Example usage of ChatsSocket with a SocketConfig
    SocketConfig config(
        [](){ std::cout << "Logging function" << std::endl; },
        true, // markOnlineOnConnect
        true, // fireInitQueries
        true, // appStateMacVerification
        [](const std::string& jid) { return jid == "ignore@example.com"; }, // shouldIgnoreJid
        true  // shouldSyncHistoryMessage
    );
    ChatsSocket chatsSocket(config);
    // ...
    return 0;
    }
};

// Main function to demonstrate usage
int main() {
    // Example usage of ChatsSocket
    SocketConfig config; // Populate with necessary configuration
    ChatsSocket chatsSocket(config);
    // ...

    return 0;
}

// Assuming AuthState and Query are classes you've defined to replace authState and query from TypeScript
Logger logger;
AuthState authState;
Query query;

// A map to hold privacy settings
std::unordered_map<std::string, std::string> privacySettings;
bool needToFlushWithAppStateSync = false;
bool pendingAppStateSync = false;

// A mutex to ensure that notifications are processed in order
std::mutex processingMutex;

// Helper function to fetch the given app state sync key
std::future<std::string> getAppStateSyncKey(const std::string& keyId) {
    return std::async(std::launch::async, [&]() {
        // Replace with actual C++ code to retrieve the app state sync key
        // This will depend on how you implement the AuthState class and its methods
        std::string key = authState.getAppStateSyncKey(keyId);
        return key;

// Function to resynchronize the application state
void resyncAppState(const std::vector<std::string>& collections, bool isInitialSync) {
// Map to store the initial version of each collection
std::unordered_map<std::string, int> initialVersionMap;
// Map to store global mutations
ChatMutationMap globalMutationMap;
// Map to store the number of attempts for each collection
std::unordered_map<std::string, int> attemptsMap;
// Set to keep track of collections that need to be handled
std::unordered_set<std::string> collectionsToHandle(collections.begin(), collections.end());

authState.keys.transaction([&](auto& transaction) {
while (!collectionsToHandle.empty()) {
std::unordered_map<std::string, LTHashState> states;
std::vector<BinaryNode> nodes;

for (const auto& name : collectionsToHandle) {
auto result = transaction.get("app-state-sync-version", {name});
LTHashState state = result[name];

if (state) {
if (initialVersionMap.find(name) == initialVersionMap.end()) {
initialVersionMap[name] = state.version;
}
} else {
state = LTHashState::newLTHashState();
}

states[name] = state;

logger.info("resyncing " + name + " from v" + std::to_string(state.version));
// ... rest of the resync logic
}

// ... handle the rest of the resync process
// Handle the mutations
newAppStateChunkHandler handler(isInitialSync);
for (const auto& [key, mutation] : globalMutationMap) {
    handler.onMutation(mutation);
}

// ... rest of your C++ code ...
    });
}

// Function to fetch privacy settings
std::future<std::unordered_map<std::string, std::string>> fetchPrivacySettings(bool force = false) {
    return std::async(std::launch::async, [&]() {
        std::lock_guard<std::mutex> lock(processingMutex);
        if (privacySettings.empty() || force) {
            // Replace with actual C++ code to query privacy settings
            // This will depend on how you implement the Query class and its methods
            auto content = query.fetchPrivacySettings();
            // Assume reduceBinaryNodeToDictionary is a function you've defined to parse the content
            privacySettings = reduceBinaryNodeToDictionary(content, "category");
        }
        return privacySettings;
});
}

for (const auto& name : collectionsToHandle) {
    auto result = transaction.get("app-state-sync-version", {name});
    LTHashState state = result[name];

    if (state) {
        if (initialVersionMap.find(name) == initialVersionMap.end()) {
            initialVersionMap[name] = state.version;
        }
    } else {
        state = LTHashState::newLTHashState();
    }

    states[name] = state;

    logger.info("resyncing " + name + " from v" + std::to_string(state.version));

    // Constructing the collection node
    BinaryNode collectionNode("collection", {
        {"name", name},
        {"version", std::to_string(state.version)},
        {"return_snapshot", std::to_string(!state.version)}
    });

    nodes.push_back(collectionNode);
}

// Constructing the sync node
BinaryNode syncNode("sync", {}, nodes);

// Sending the query
auto result = query(BinaryNode("iq", {
    {"to", S_WHATSAPP_NET},
    {"xmlns", "w:sync:app:state"},
    {"type", "set"}
}, {syncNode}));

// ... handle the result of the query and extract data from the binary node response ...



// Assuming extractSyncdPatches and decodeSyncdSnapshot are functions that have been defined
// Assuming config and appStateMacVerification are objects that have been defined and initialized

// Extract the synchronized patches from the binary node result
auto decoded = extractSyncdPatches(result, config.options);

for (const auto& [key, value] : decoded) {
std::string name = key;
const auto& [patches, hasMorePatches, snapshot] = value;

try {
if (snapshot) {
auto [newState, mutationMap] = decodeSyncdSnapshot(
name,
snapshot,
getAppStateSyncKey,
initialVersionMap[name],
appStateMacVerification.snapshot
);

states[name] = newState;
for (const auto& mutation : mutationMap) {
globalMutationMap[mutation.first] = mutation.second;
}

logger.info("restored state of " + name + " from snapshot to v" + std::to_string(newState.version) + " with mutations");

transaction.set("app-state-sync-version", {{name, newState}});
}

// Only process if there are synchronized patches
if (!patches.empty()) {
auto [newState, mutationMap] = decodePatches(
name,
patches,
states[name],
getAppStateSyncKey,
config.options,
initialVersionMap[name],
logger,
appStateMacVerification.patch
);

transaction.set("app-state-sync-version", {{name, newState}});
}

// ... rest of the processing logic ...

} catch (const std::exception& e) {
logger.error("Error processing patches for " + name + ": " + e.what());
}
}

// Assuming newAppStateChunkHandler is a class that has been defined and initialized
// Assuming onMutation is a method of newAppStateChunkHandler that handles mutations

for (const auto& [key, value] : decoded) {
    std::string name = key;
    const auto& [patches, hasMorePatches, snapshot] = value;

    try {
        // ... previous try block code ...

        logger.info("synced " + name + " to v" + std::to_string(newState.version));
        initialVersionMap[name] = newState.version;

        for (const auto& mutation : mutationMap) {
            globalMutationMap[mutation.first] = mutation.second;
        }

        if (hasMorePatches) {
            logger.info(name + " has more patches...");
        } else {
            // Collection is done with sync
            collectionsToHandle.erase(name);
        }
    } catch (const std::exception& e) {
        // ... previous catch block code ...

        bool isIrrecoverableError = attemptsMap[name] >= MAX_SYNC_ATTEMPTS ||
                                    e.what() == std::string("404") || // Assuming the error message contains the status code
                                    typeid(e) == typeid(std::type_error);

        logger.info("failed to sync state from version" + (isIrrecoverableError ? "" : ", removing and trying from scratch"));
        transaction.set("app-state-sync-version", {{name, nullptr}});
        // Increment number of retries
        attemptsMap[name]++;

        if (isIrrecoverableError) {
            // Stop retrying
            collectionsToHandle.erase(name);
        }
    }
}

// Assuming query and getBinaryNodeChild are functions that have been defined and initialized
// Assuming authState and ev are objects that have been defined and initialized

// Function to fetch the profile picture URL of a user/group
std::future<std::string> profilePictureUrl(const std::string& jid, const std::string& type = "preview", const std::optional<int>& timeoutMs = std::nullopt) {
std::string normalizedJid = jidNormalizedUser(jid);
return std::async(std::launch::async, [&]() {
auto result = query({
{"tag", "iq"},
{"attrs", {
{"to", normalizedJid},
{"type", "get"},
{"xmlns", "w:profile:picture"}
}},
{"content", {
{{"tag", "picture"}, {"attrs", {{"type", type}, {"query", "url"}}}}
}}
}, timeoutMs.value_or(0));
auto child = getBinaryNodeChild(result, "picture");
return child ? child->attrs["url"] : "";
});
}

// Function to send a presence update
std::future<void> sendPresenceUpdate(const std::string& type, const std::optional<std::string>& toJid = std::nullopt) {
return std::async(std::launch::async, [&]() {
auto& me = authState.creds.me;
if (type == "available" || type == "unavailable") {
if (me.name.empty()) {
logger.warn("no name present, ignoring presence update request...");
return;
}

ev.emit("connection.update", {{"isOnline", type == "available"}});
}
});
}

// Assuming sendNode and generateMessageTag are functions that have been defined and initialized
// Assuming authState is an object that has been defined and initialized

// Function to send a presence update or a chat state update
std::future<void> sendPresenceUpdate(const std::string& type, const std::optional<std::string>& toJid = std::nullopt) {
return std::async(std::launch::async, [&]() {
auto& me = authState.creds.me;
if (type == "available" || type == "unavailable") {
sendNode({
{"tag", "presence"},
{"attrs", {
{"name", me.name},
{"type", type}
}}
});
} else {
sendNode({
{"tag", "chatstate"},
{"attrs", {
{"from", me.id},
{"to", *toJid}
}},
{"content", {
{
{"tag", type == "recording" ? "composing" : type},
{"attrs", type == "recording" ? std::map<std::string, std::string>{{"media", "audio"}} : std::map<std::string, std::string>{}}
}}
});
}
});
}

// Function to subscribe to presence updates
std::future<void> presenceSubscribe(const std::string& toJid, const std::optional<std::vector<uint8_t>>& tcToken = std::nullopt) {
return std::async(std::launch::async, [&]() {
sendNode({
{"tag", "presence"},
{"attrs", {
{"to", toJid},
{"id", generateMessageTag()},
{"type", "subscribe"}
}},
{"content", tcToken ? std::vector<std::map<std::string, std::string>>{
{{"tag", "tctoken"}, {"attrs", {}}, {"content", *tcToken}}
} : std::nullopt}
});
});
}

// Assuming ev is an object that has been defined and initialized
// Assuming shouldIgnoreJid and logger are functions that have been defined and initialized

// Function to handle presence updates
void handlePresenceUpdate(const BinaryNode& node) {
PresenceData* presence = nullptr;
std::string jid = node.attrs["from"];
std::string participant = node.attrs.count("participant") ? node.attrs["participant"] : jid;

if(shouldIgnoreJid(jid)) {
return;
}

if(node.tag == "presence") {
presence = new PresenceData{
node.attrs["type"] == "unavailable" ? "unavailable" : "available",
node.attrs.count("last") && node.attrs["last"] != "deny" ? std::stoi(node.attrs["last"]) : std::nullopt
};
} else if(node.content.is_array()) {
auto firstChild = node.content[0];
std::string type = firstChild.tag;
if(type == "paused") {
type = "available";
}

if(firstChild.attrs.count("media") && firstChild.attrs["media"] == "audio") {
type = "recording";
}

presence = new PresenceData{ type };
} else {
logger.error("recv invalid presence node", node);
}

if(presence) {
ev.emit("presence.update", jid, {{participant, *presence}});
delete presence;
}
}

// Assuming processingMutex, authState, logger, resyncAppState, encodeSyncdPatch, and other relevant objects and functions have been defined and initialized

// Function to apply an application patch
void applyAppPatch(const WAPatchCreate& patchCreate) {
processingMutex.mutex([&]() {
authState.keys.transaction([&]() {
logger.debug("applying app patch", patchCreate);

resyncAppState({patchCreate.type}, false);

auto currentSyncVersion = authState.keys.get("app-state-sync-version", {patchCreate.type});
LTHashState initial = currentSyncVersion ? *currentSyncVersion : LTHashState();

auto encodeResult = encodeSyncdPatch(
patchCreate,
authState.creds.myAppStateKeyId,
initial,
getAppStateSyncKey
);
auto patch = encodeResult.patch;
auto state = encodeResult.state;

BinaryNode node = {
"iq",
{
{"to", S_WHATSAPP_NET},
{"type", "set"},
{"xmlns", "w:sync:app:state"}
},
{
{
"sync",
{},
{
{
"collection",
{
{"name", patchCreate.type},
{"version", std::to_string(state.version - 1)},
{"return_snapshot", "false"}
},
// ... rest of the BinaryNode content ...
}
}
}
};

// ... code to send the BinaryNode to the server ...
});

// Function to apply an application patch
void applyAppPatch(const WAPatchCreate& patchCreate) {
// ... previous code to set up the BinaryNode ...

// Send the BinaryNode to the server and wait for the response
query(node).then([&](auto /* response */) {
// Update the application state version
authState.keys.set("app-state-sync-version", {{patchCreate.type, state}});

if(config.emitOwnEvents) {
auto onMutation = newAppStateChunkHandler(false).onMutation;
auto mutationMap = decodePatches(
name,
{{encodeResult.patch, {encodeResult.state.version}}},
initial,
getAppStateSyncKey,
config.options,
nullptr,
logger
);
for(const auto& key_value : mutationMap) {
onMutation(key_value.second);
}
}
}).catch_error([&](auto /* error */) {
// Handle error
});
}

// Function to fetch ABT properties
void fetchAbt() {
BinaryNode abtNode = {
"iq",
{
{"to", S_WHATSAPP_NET},
{"xmlns", "abt"},
{"type", "get"}
},
{
{"props", {{"protocol", "1"}}}
};

// Send the BinaryNode to the server and wait for the response
query(abtNode).then([&](auto /* response */) {
// Handle the response
}).catch_error([&](auto /* error */) {
// Handle error
});
}

// Function to fetch ABT properties
std::map<std::string, std::string> fetchAbt() {
auto abtNode = query({
"iq",
{
{"to", S_WHATSAPP_NET},
{"xmlns", "abt"},
{"type", "get"}
},
{
{"props", {{"protocol", "1"}}}
}
}).get(); // Assuming synchronous operation for simplicity

auto propsNode = getBinaryNodeChild(abtNode, "props");

std::map<std::string, std::string> props;
if(propsNode) {
props = reduceBinaryNodeToDictionary(propsNode, "prop");
}

logger.debug("fetched abt");

return props;
}

// Function to fetch non-ABT properties
std::map<std::string, std::string> fetchProps() {
auto resultNode = query({
"iq",
{
{"to", S_WHATSAPP_NET},
{"xmlns", "w"},
{"type", "get"}
},
{
{"props", {}}
}
}).get(); // Assuming synchronous operation for simplicity

auto propsNode = getBinaryNodeChild(resultNode, "props");

std::map<std::string, std::string> props;
if(propsNode) {
props = reduceBinaryNodeToDictionary(propsNode, "prop");
}

logger.debug("fetched props");

return props;
}

// Function to modify a chat
void chatModify(const ChatModification& mod, const std::string& jid) {
auto patch = chatModificationToAppPatch(mod, jid);
return appPatch(patch);
}

// Function to star or unstar messages
void star(const std::string& jid, const std::vector<Message>& messages, bool star) {
    ChatModification mod;
    mod.star.messages = messages;
    mod.star.star = star;
    chatModify(mod, jid);
}

// Function to add a label to a chat
void addChatLabel(const std::string& jid, const std::string& labelId) {
    ChatModification mod;
    mod.addChatLabel.labelId = labelId;
    chatModify(mod, jid);
}

// Function to remove a label from a chat
void removeChatLabel(const std::string& jid, const std::string& labelId) {
    ChatModification mod;
    mod.removeChatLabel.labelId = labelId;
    chatModify(mod, jid);
}

// Function to add a label to a message
void addMessageLabel(const std::string& jid, const std::string& messageId, const std::string& labelId) {
    ChatModification mod;
    mod.addMessageLabel.messageId = messageId;
    mod.addMessageLabel.labelId = labelId;
    chatModify(mod, jid);
}

// Function to remove a label from a message
void removeMessageLabel(const std::string& jid, const std::string& messageId, const std::string& labelId) {
    ChatModification mod;
    mod.removeMessageLabel.messageId = messageId;
    mod.removeMessageLabel.labelId = labelId;
    chatModify(mod, jid);
}

// Function to execute initial queries on connection open
void executeInitQueries() {
// Assuming fetchAbt, fetchProps, fetchBlocklist, and fetchPrivacySettings are async functions
std::vector<std::future<void>> futures;
futures.push_back(std::async(std::launch::async, fetchAbt));
futures.push_back(std::async(std::launch::async, fetchProps));
futures.push_back(std::async(std::launch::async, fetchBlocklist));
futures.push_back(std::async(std::launch::async, fetchPrivacySettings));

for(auto& future : futures) {
future.get(); // Wait for all futures to complete
}
}

// Function to upsert a message
void upsertMessage(const WAMessage& msg, MessageUpsertType type) {
ev.emit("messages.upsert", { messages: {msg}, type });

if(msg.pushName.has_value()) {
std::string jid = msg.key.fromMe ? authState.creds.me.id : (msg.key.participant.value_or(msg.key.remoteJid));
jid = jidNormalizedUser(jid);

if(!msg.key.fromMe) {
ev.emit("contacts.update", { id: jid, notify: msg.pushName.value(), verifiedName: msg.verifiedBizName.value_or("") });
}

// Update our pushname too
if(msg.key.fromMe && msg.pushName.value() != authState.creds.me.name) {
authState.creds.me.name = msg.pushName.value();
ev.emit("creds.update", { me: authState.creds.me });
}
}

auto historyMsg = getHistoryMsg(msg.message);
bool shouldProcessHistoryMsg = historyMsg.has_value() &&
shouldSyncHistoryMessage(historyMsg.value()) &&
PROCESSABLE_HISTORY_TYPES.find(historyMsg.value().syncType) != PROCESSABLE_HISTORY_TYPES.end();

if(historyMsg.has_value() && !authState.creds.myAppStateKeyId.has_value()) {
logger.warn("skipping app state sync, as myAppStateKeyId is not set");
pendingAppStateSync = true;
}

// Assuming doAppStateSync is an async function
if(historyMsg.has_value() && authState.creds.myAppStateKeyId.has_value()) {
pendingAppStateSync = false;
doAppStateSync().get(); // Wait for the app state sync to complete
}
}

// Function to process a message
void processMessage(const WAMessage& msg, const ProcessContext& context) {
    // Implementation of processMessage function
    // This function should handle the message processing logic
}

// Function to handle presence updates
void handlePresenceUpdate(const BinaryNode& node) {
    // Implementation of handlePresenceUpdate function
    // This function should handle presence updates
}

// Function to clean dirty bits
void cleanDirtyBits(const std::string& type, long long lastTimestamp) {
    // Implementation of cleanDirtyBits function
    // This function should handle the cleaning of dirty bits
}

// Function to perform app state synchronization
void doAppStateSync() {
    if(!authState.creds.accountSyncCounter) {
        logger.info("doing initial app state sync");
        resyncAppState(ALL_WA_PATCH_NAMES, true);

        authState.creds.accountSyncCounter = (authState.creds.accountSyncCounter.value_or(0)) + 1;
        ev.emit("creds.update", { accountSyncCounter: authState.creds.accountSyncCounter });

        if(needToFlushWithAppStateSync) {
            logger.debug("flushing with app state sync");
            ev.flush();
        }
    }
}

// WebSocket event handlers
ws.on("CB:presence", handlePresenceUpdate);
ws.on("CB:chatstate", handlePresenceUpdate);

ws.on("CB:ib,,dirty", [&](const BinaryNode& node) {
    auto dirtyNode = getBinaryNodeChild(node, "dirty");
    if(dirtyNode) {
        const auto& attrs = dirtyNode->attrs;
        const std::string& type = attrs["type"];
        if(type == "account_sync") {
            if(attrs.find("timestamp") != attrs.end()) {
                long long lastTimestamp = std::stoll(attrs["timestamp"]);
                if(authState.creds.lastAccountSyncTimestamp) {
                    cleanDirtyBits("account_sync", authState.creds.lastAccountSyncTimestamp.value());
                }
                authState.creds.lastAccountSyncTimestamp = lastTimestamp;
                ev.emit("creds.update", { lastAccountSyncTimestamp: authState.creds.lastAccountSyncTimestamp });
            }
        }
    }
});

// Function to handle unknown sync
void handleUnknownSync(const BinaryNode& node) {
// Implementation of handleUnknownSync function
// This function should log the unknown sync
}

// Function to handle unexpected errors
void onUnexpectedError(const std::exception& error, const std::string& context) {
// Implementation of onUnexpectedError function
// This function should handle unexpected errors
}

// Function to send presence update
void sendPresenceUpdate(const std::string& presence) {
// Implementation of sendPresenceUpdate function
// This function should send the presence update
}

// WebSocket event handler for 'groups' and other sync types
ws.on("CB:ib,,dirty", [&](const BinaryNode& node) {
auto dirtyNode = getBinaryNodeChild(node, "dirty");
if(dirtyNode) {
const auto& attrs = dirtyNode->attrs;
const std::string& type = attrs["type"];
if(type == "account_sync") {
// ... handle account_sync as before ...
} else if(type == "groups") {
// Handled in groups.ts (or the equivalent C++ module)
} else {
logger.info({ node }, "received unknown sync");
}
}
});

// Event listener for connection updates
ev.on("connection.update", [&](const ConnectionUpdate& update) {
if(update.connection == "open") {
if(fireInitQueries) {
try {
executeInitQueries();
} catch(const std::exception& error) {
onUnexpectedError(error, "init queries");
}
}

try {
sendPresenceUpdate(markOnlineOnConnect ? "available" : "unavailable");
} catch(const std::exception& error) {
onUnexpectedError(error, "presence update requests");
}
}

if(update.receivedPendingNotifications) {
if(!authState.creds.myAppStateKeyId.has_value() && !config.mobile) {
ev.buffer();
needToFlushWithAppStateSync = true;
}
}
});

// ... rest of your C++ code ...
