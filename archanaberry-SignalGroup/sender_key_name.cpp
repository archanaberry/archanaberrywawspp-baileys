#include <string>
#include <climits>

bool isNull(const std::string& str) {
    return str.empty();
}

int intValue(long long num) {
    const long long MAX_VALUE = INT_MAX;
    const long long MIN_VALUE = INT_MIN;
    if (num > MAX_VALUE || num < MIN_VALUE) {
        return static_cast<int>(num & 0xFFFFFFFF);
    }
    return static_cast<int>(num);
}

int hashCode(const std::string& strKey) {
    long long hash = 0;
    if (!isNull(strKey)) {
        for (char ch : strKey) {
            hash = hash * 31 + ch;
            hash = intValue(hash);
        }
    }
    return hash;
}

class SenderKeyName {
private:
    std::string groupId;
    std::string senderId;
    int deviceId;

public:
    SenderKeyName(const std::string& groupId, const std::string& senderId, int deviceId)
        : groupId(groupId), senderId(senderId), deviceId(deviceId) {}

    std::string getGroupId() const {
        return groupId;
    }

    std::string getSender() const {
        return senderId;
    }

    int getDeviceId() const {
        return deviceId;
    }

    std::string serialize() const {
        return groupId + "::" + senderId + "::" + std::to_string(deviceId);
    }

    std::string toString() const {
        return serialize();
    }

    bool equals(const SenderKeyName& other) const {
        return groupId == other.groupId && senderId == other.senderId && deviceId == other.deviceId;
    }

    int hashCode() const {
        return ::hashCode(groupId) ^ ::hashCode(senderId + std::to_string(deviceId));
    }
};
