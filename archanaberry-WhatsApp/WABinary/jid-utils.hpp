// constants.hpp
#ifndef CONSTANTS_HPP
#define CONSTANTS_HPP

#include <string>

// Constants
constexpr char S_WHATSAPP_NET[] = "@s.whatsapp.net";
constexpr char OFFICIAL_BIZ_JID[] = "16505361212@c.us";
constexpr char SERVER_JID[] = "server@c.us";
constexpr char PSA_WID[] = "0@c.us";
constexpr char STORIES_JID[] = "status@broadcast";

#endif // CONSTANTS_HPP

// JidTypes.hpp
#ifndef JID_TYPES_HPP
#define JID_TYPES_HPP

#include <string>
#include <optional>

// Enum for JidServer
enum class JidServer {
    C_US,
    G_US,
    BROADCAST,
    S_WHATSAPP_NET,
    CALL,
    LID
};

// Struct for JidWithDevice
struct JidWithDevice {
    std::string user;
    std::optional<int> device;
};

// Struct for FullJid
struct FullJid : public JidWithDevice {
    JidServer server;
    std::optional<int> domainType;
};

#endif // JID_TYPES_HPP

// JidFunctions.hpp
#ifndef JID_FUNCTIONS_HPP
#define JID_FUNCTIONS_HPP

#include "JidTypes.hpp"
#include <sstream>

// Function to encode JID
std::string jidEncode(const std::string& user, JidServer server, std::optional<int> device = {}, std::optional<int> agent = {}) {
    std::ostringstream jid;
    jid << user;
    if (agent) {
        jid << "_" << *agent;
    }
    if (device) {
        jid << ":" << *device;
    }
    // Add server part based on JidServer enum
    // This requires a function to convert JidServer enum to string
    jid << "@" << jidServerToString(server);
    return jid.str();
}

// Function to decode JID
std::optional<FullJid> jidDecode(const std::string& jid) {
    auto sepIdx = jid.find('@');
    if (sepIdx == std::string::npos) {
        return {};
    }

    FullJid result;
    result.server = stringToJidServer(jid.substr(sepIdx + 1));
    std::string userCombined = jid.substr(0, sepIdx);

    auto deviceIdx = userCombined.find(':');
    if (deviceIdx != std::string::npos) {
        result.device = std::stoi(userCombined.substr(deviceIdx + 1));
        userCombined = userCombined.substr(0, deviceIdx);
            }

    auto agentIdx = userCombined.find('_');
    if (agentIdx != std::string::npos) {
        result.user = userCombined.substr(0, agentIdx);
        // Assuming domainType is related to the presence of an agent
        result.domainType = 1;
    } else {
        result.user = userCombined;
        result.domainType = 0;
    }
    
// Function to check if two JIDs are the same user
bool areJidsSameUser(const std::string& jid1, const std::string& jid2) {
    auto decodedJid1 = jidDecode(jid1);
    auto decodedJid2 = jidDecode(jid2);
    return decodedJid1 && decodedJid2 && decodedJid1->user == decodedJid2->user;
}

// Function to check if the JID is a user
bool isJidUser(const std::string& jid) {
    return jid.ends_with(S_WHATSAPP_NET);
}

// Function to check if the JID is a lid user
bool isLidUser(const std::string& jid) {
    return jid.ends_with("@lid");
}

// Function to check if the JID is a broadcast
bool isJidBroadcast(const std::string& jid) {
    return jid.ends_with("@broadcast");
}

// Function to check if the JID is a group
bool isJidGroup(const std::string& jid) {
    return jid.ends_with("@g.us");
}

// Function to check if the JID is the status broadcast
bool isJidStatusBroadcast(const std::string& jid) {
    return jid == STORIES_JID;
}

// Function to normalize the user JID
std::string jidNormalizedUser(const std::string& jid) {
    auto result = jidDecode(jid);
    if (!result) {
        return "";
    }

    JidServer server = result->server == JidServer::C_US ? JidServer::S_WHATSAPP_NET : result->server;
    return jidEncode(result->user, server, result->device);
    }

    return result;
}

// You will need to implement jidServerToString and stringToJidServer functions
// to convert between JidServer enum and string representations.

#endif // JID_FUNCTIONS_HPP
