#include <string>
#include <vector>
#include <future>
#include "SocketConfig.h" // Define SocketConfig according to your Types
#include "GroupMetadata.h" // Define GroupMetadata according to your Types
#include "BinaryNode.h" // Define BinaryNode according to your WABinary
#include "ChatsSocket.h" // Include the header for makeChatsSocket

// Forward declaration of necessary utility functions
std::string generateMessageID();
long unixTimestampSeconds();
GroupMetadata extractGroupMetadata(const BinaryNode& result);

// Forward declaration of makeChatsSocket
ChatsSocket makeChatsSocket(const SocketConfig& config);

class GroupsSocket {
public:
    GroupsSocket(const SocketConfig& config) : chatsSocket(makeChatsSocket(config)) {}

    std::future<BinaryNode> groupQuery(const std::string& jid, const std::string& type, const std::vector<BinaryNode>& content) {
        // Construct the iq node
        BinaryNode iqNode("iq", {{"type", type}, {"xmlns", "w:g2"}, {"to", jid}}, content);

        // Perform the query and return the future result
        return chatsSocket.query(iqNode);
    }

    std::future<GroupMetadata> groupMetadata(const std::string& jid) {
        std::vector<BinaryNode> content = {BinaryNode("query", {{"request", "interactive"}})};
        auto resultFuture = groupQuery(jid, "get", content);
        return std::async(std::launch::async, [resultFuture]() {
            auto result = resultFuture.get();
            return extractGroupMetadata(result);
        });
    }

    std::future<BinaryNode> groupFetchAllParticipating() {
        // Construct the iq node
        BinaryNode iqNode("iq", {{"to", "@g.us"}, {"xmlns", "w:g2"}, {"type", "get"}}, {});

        // Perform the query and return the future result
        return chatsSocket.query(iqNode);
    }

    std::map<std::string, GroupMetadata> groupFetchAllParticipating() {
    std::vector<BinaryNode> content = {
        BinaryNode("participating", {}, {
            BinaryNode("participants", {}),
            BinaryNode("description", {})
        })
    };

    auto resultFuture = groupQuery("@g.us", "get", content);
    auto result = resultFuture.get(); // Assuming synchronous for simplicity

    std::map<std::string, GroupMetadata> data;
    BinaryNode* groupsChild = getBinaryNodeChild(result, "groups");
    if (groupsChild) {
        std::vector<BinaryNode> groups = getBinaryNodeChildren(*groupsChild, "group");
        for (const auto& groupNode : groups) {
            BinaryNode resultNode("result", {}, {groupNode});
            GroupMetadata meta = extractGroupMetadata(resultNode);
            data[meta.id] = meta;
        }
    }

    // Emit event with the updated groups
    ev.emit("groups.update", data); // Assuming ev is an event emitter instance

    return data;
}

void onDirtyCallback(const BinaryNode& node) {
    const BinaryNode* dirtyNode = getBinaryNodeChild(node, "dirty");
    if (dirtyNode) {
        const std::string& type = dirtyNode->getAttr("type");
        if (type != "groups") {
            return;
        }

        auto future = groupFetchAllParticipating();
        future.wait(); // Wait for the future to complete

        // Clean dirty bits for groups
        cleanDirtyBits("groups"); // Assuming cleanDirtyBits is a method in ChatsSocket
    }
}

std::future<GroupMetadata> groupCreate(const std::string& subject, const std::vector<std::string>& participants) {
    std::string key = generateMessageID();
    std::vector<BinaryNode> participantNodes;
    for (const auto& jid : participants) {
        participantNodes.push_back(BinaryNode("participant", {{"jid", jid}}));
    }
    std::vector<BinaryNode> content = {
        BinaryNode("create", {{"subject", subject}, {"key", key}}, participantNodes)
    };

    auto resultFuture = groupQuery("@g.us", "set", content);
    return std::async(std::launch::async, [resultFuture]() {
        auto result = resultFuture.get();
        return extractGroupMetadata(result);
    });
}

void groupLeave(const std::string& id) {
    std::vector<BinaryNode> content = {
        BinaryNode("leave", {}, {
            BinaryNode("group", {{"id", id}})
        })
    };

    auto future = groupQuery("@g.us", "set", content);
    future.wait(); // Wait for the future to complete
}

void groupUpdateSubject(const std::string& jid, const std::string& subject) {
    std::vector<BinaryNode> content = {
        BinaryNode("subject", {}, std::vector<BinaryNode>{}, Buffer::from(subject, "utf-8"))
    };

    auto future = groupQuery(jid, "set", content);
    future.wait(); // Wait for the future to complete
}

std::vector<ParticipantStatus> groupRequestParticipantsUpdate(const std::string& jid, const std::vector<std::string>& participants, const std::string& action) {
    if (action != "approve" && action != "reject") {
        throw std::invalid_argument("Action must be 'approve' or 'reject'.");
    }

    std::vector<BinaryNode> participantNodes;
    for (const auto& participantJid : participants) {
        participantNodes.push_back(BinaryNode("participant", {{"jid", participantJid}}));
    }

    std::vector<BinaryNode> content = {
        BinaryNode("membership_requests_action", {}, {
            BinaryNode(action, {}, participantNodes)
        })
    };

    auto resultFuture = groupQuery(jid, "set", content);
    auto result = resultFuture.get(); // Assuming synchronous for simplicity

    BinaryNode* node = getBinaryNodeChild(result, "membership_requests_action");
    if (!node) {
        throw std::runtime_error("membership_requests_action node not found in result.");
    }

    BinaryNode* nodeAction = getBinaryNodeChild(*node, action);
    if (!nodeAction) {
        throw std::runtime_error(action + " node not found in membership_requests_action.");
    }

    std::vector<BinaryNode> participantsAffected = getBinaryNodeChildren(*nodeAction, "participant");
    std::vector<ParticipantStatus> statuses;
    for (const auto& p : participantsAffected) {
        ParticipantStatus status;
        status.jid = p.getAttr("jid");
        status.status = p.getAttr("error").empty() ? "200" : p.getAttr("error");
        statuses.push_back(status);
    }

    return statuses;
}

std::vector<ParticipantStatus> groupParticipantsUpdate(const std::string& jid, const std::vector<std::string>& participants, const ParticipantAction& action) {
    std::vector<BinaryNode> participantNodes;
    for (const auto& participantJid : participants) {
        participantNodes.push_back(BinaryNode("participant", {{"jid", participantJid}}));
    }

    std::vector<BinaryNode> content = {
        BinaryNode(action, {}, participantNodes)
    };

    auto resultFuture = groupQuery(jid, "set", content);
    auto result = resultFuture.get(); // Assuming synchronous for simplicity

    BinaryNode* node = getBinaryNodeChild(result, action);
    if (!node) {
        throw std::runtime_error(action + " node not found in result.");
    }

    std::vector<BinaryNode> participantsAffected = getBinaryNodeChildren(*node, "participant");
    std::vector<ParticipantStatus> statuses;
    for (const auto& p : participantsAffected) {
        ParticipantStatus status;
        status.jid = p.getAttr("jid");
        status.status = p.getAttr("error").empty() ? "200" : p.getAttr("error");
        // Assuming 'content' is a BinaryNode, you might need to serialize it or handle it according to your needs
        status.content = p; // This line may need to be adapted
        statuses.push_back(status);
    }

    return statuses;
}

void groupUpdateDescription(const std::string& jid, const std::optional<std::string>& description) {
    auto metadataFuture = groupMetadata(jid);
    auto metadata = metadataFuture.get(); // Assuming synchronous for simplicity
    std::string prev = metadata.descId.value_or("");

    std::vector<BinaryNode> content;
    if (description.has_value()) {
        content.push_back(BinaryNode("description", {{"id", generateMessageID()}}, {
            BinaryNode("body", {}, Buffer::from(description.value(), "utf-8"))
        }));
    } else {
    content.push_back(BinaryNode("description", {{"delete", "true"}, {"prev", prev}}));
    }

    auto future = groupQuery(jid, "set", content);
    future.wait(); // Wait for the future to complete
}

std::string groupInviteCode(const std::string& jid) {
auto resultFuture = groupQuery(jid, "get", {BinaryNode("invite", {})});
auto result = resultFuture.get(); // Assuming synchronous for simplicity

BinaryNode* inviteNode = getBinaryNodeChild(result, "invite");
if (!inviteNode) {
throw std::runtime_error("Invite node not found in result.");
}

return inviteNode->getAttr("code");
}

std::string groupRevokeInvite(const std::string& jid) {
auto resultFuture = groupQuery(jid, "set", {BinaryNode("invite", {})});
auto result = resultFuture.get(); // Assuming synchronous for simplicity

BinaryNode* inviteNode = getBinaryNodeChild(result, "invite");
if (!inviteNode) {
throw std::runtime_error("Invite node not found in result.");
}

return inviteNode->getAttr("code");
}

std::string groupAcceptInvite(const std::string& code) {
auto resultsFuture = groupQuery("@g.us", "set", {BinaryNode("invite", {{"code", code}})});
auto results = resultsFuture.get(); // Assuming synchronous for simplicity

BinaryNode* result = getBinaryNodeChild(results, "group");
if (!result) {
throw std::runtime_error("Group node not found in result.");
}

return result->getAttr("jid");
}

std::string groupAcceptInviteV4(const std::string& key, const GroupInviteMessage& inviteMessage) {
// Assuming GroupInviteMessage is a struct that holds the necessary invite message information
auto resultsFuture = groupQuery(inviteMessage.groupJid, "set", {
BinaryNode("accept", {
{"code", inviteMessage.inviteCode},
{"expiration", std::to_string(inviteMessage.inviteExpiration)},
{"admin", key}
})
});
auto results = resultsFuture.get(); // Assuming synchronous for simplicity

std::string groupAcceptInviteV4(const MessageKey& key, GroupInviteMessage& inviteMessage) {
    // Assuming MessageKey and GroupInviteMessage are structs that hold the necessary information
    if (!key.id.empty()) {
        // Update the invite message to be expired
        inviteMessage.inviteExpiration = 0;
        inviteMessage.inviteCode.clear();
        // Emit an event to update messages (you will need to implement event emission or an equivalent mechanism in your C++ project)
        emitMessagesUpdate(key, inviteMessage);
    }

    // Generate the group add message
    upsertMessage(
        {
            key.remoteJid,
            generateMessageID(),
            false,
            key.remoteJid,
            WAMessageStubType::GROUP_PARTICIPANT_ADD,
            {authState.creds.me.id},
            key.remoteJid,
            unixTimestampSeconds()
        },
        "notify"
    );

    // Perform the group query to accept the invite
    auto resultsFuture = groupQuery(inviteMessage.groupJid, "set", {
        BinaryNode("accept", {
            {"code", inviteMessage.inviteCode},
            {"expiration", std::to_string(inviteMessage.inviteExpiration)},
            {"admin", key.remoteJid}
        })
    });
    auto results = resultsFuture.get(); // Assuming synchronous for simplicity

    return results.getAttr("from");
}

GroupMetadata groupGetInviteInfo(const std::string& code) {
    auto resultsFuture = groupQuery("@g.us", "get", {BinaryNode("invite", {{"code", code}})});
    auto results = resultsFuture.get(); // Assuming synchronous for simplicity

    return extractGroupMetadata(results);
}

void groupToggleEphemeral(const std::string& jid, int ephemeralExpiration) {
BinaryNode content = ephemeralExpiration > 0 ?
BinaryNode("ephemeral", {{"expiration", std::to_string(ephemeralExpiration)}}) :
BinaryNode("not_ephemeral", {});
groupQuery(jid, "set", {content});
}

void groupSettingUpdate(const std::string& jid, const std::string& setting) {
groupQuery(jid, "set", {BinaryNode(setting, {})});
}

void groupMemberAddMode(const std::string& jid, const std::string& mode) {
groupQuery(jid, "set", {BinaryNode("member_add_mode", {}, mode)});
}

void groupJoinApprovalMode(const std::string& jid, const std::string& mode) {
groupQuery(jid, "set", {BinaryNode("membership_approval_mode", {}, {BinaryNode("group_join", {{"state", mode}})})});
}

// ... (Rest of the GroupsSocket class code)

GroupMetadata extractGroupMetadata(const BinaryNode& result) {
const BinaryNode* group = getBinaryNodeChild(result, "group");
if (!group) {
throw std::runtime_error("Group node not found in result.");
}

const BinaryNode* descChild = getBinaryNodeChild(*group, "description");
std::string desc;
std::string descId;

if (descChild) {
desc = getBinaryNodeChildString(*descChild, "body");
descId = descChild->getAttr("id");
}

// ... (Previous extractGroupMetadata function code)

GroupMetadata extractGroupMetadata(const BinaryNode& result) {
    const BinaryNode* group = getBinaryNodeChild(result, "group");
    if (!group) {
        throw std::runtime_error("Group node not found in result.");
    }

    std::string groupId = group->attrs.find("id")->second;
    if (groupId.find('@') == std::string::npos) {
        groupId = jidEncode(groupId, "g.us");
    }

    const BinaryNode* ephNode = getBinaryNodeChild(*group, "ephemeral");
    int ephemeralDuration = ephNode ? std::stoi(ephNode->attrs.find("expiration")->second) : 0;

    bool memberAddMode = getBinaryNodeChildString(*group, "member_add_mode") == "all_member_add";

    GroupMetadata metadata;
    metadata.id = groupId;
    metadata.subject = group->attrs.find("subject")->second;
    metadata.subjectOwner = group->attrs.find("s_o")->second;
    metadata.subjectTime = std::stoi(group->attrs.find("s_t")->second);
    metadata.size = getBinaryNodeChildren(*group, "participant").size();
    metadata.creation = std::stoi(group->attrs.find("creation")->second);
    metadata.owner = group->attrs.find("creator") != group->attrs.end() ? jidNormalizedUser(group->attrs.find("creator")->second) : "";

    // ... (Rest of the description extraction code)

    metadata.restrict = getBinaryNodeChild(*group, "locked") != nullptr;
    metadata.announce = getBinaryNodeChild(*group, "announcement") != nullptr;
    metadata.isCommunity = getBinaryNodeChild(*group, "parent") != nullptr;
    metadata.isCommunityAnnounce = getBinaryNodeChild(*group, "default_sub_group") != nullptr;
    metadata.joinApprovalMode = getBinaryNodeChild(*group, "membership_approval_mode") != nullptr;
    metadata.memberAddMode = memberAddMode;

    // Extract participants
    std::vector<BinaryNode> participants = getBinaryNodeChildren(*group, "participant");
    for (const auto& participant : participants) {
        GroupParticipant gp;
                gp.id = participant.attrs.find("jid")->second;
                gp.admin = participant.attrs.find("type") != participant.attrs.end() ? participant.attrs.find("type")->second : "";
        metadata.participants.push_back(gp);
    }

    metadata.ephemeralDuration = ephemeralDuration;

    return metadata;
}

// ... Rest of your code ...

private:
    ChatsSocket chatsSocket;
};

// ... Rest of your code ...