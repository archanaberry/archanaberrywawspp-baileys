#include <vector>
#include <stdexcept>
#include "sender_key_state.hpp" // You would need to create this header file or include a library that provides the SenderKeyState class

class SenderKeyRecord {
private:
static const int MAX_STATES = 5;
std::vector<SenderKeyState> senderKeyStates;

public:
SenderKeyRecord(const std::vector<std::string>& serialized = {}) {
for (const auto& structure : serialized) {
senderKeyStates.emplace_back(structure);
}
}

bool isEmpty() const {
return senderKeyStates.empty();
}

SenderKeyState getSenderKeyState(int keyId = 0) {
if (keyId == 0 && !senderKeyStates.empty()) {
return senderKeyStates.back();
}
for (const auto& state : senderKeyStates) {
if (state.getKeyId() == keyId) {
return state;
}
}
throw std::invalid_argument("No matching state found for key ID");
}

void addSenderKeyState(int id, int iteration, const std::string& chainKey, const std::string& signatureKey) {
senderKeyStates.emplace_back(id, iteration, chainKey, signatureKey);
if (senderKeyStates.size() > MAX_STATES) {
senderKeyStates.erase(senderKeyStates.begin());
}
}

void setSenderKeyState(int id, int iteration, const std::string& chainKey, const std::pair<std::string, std::string>& keyPair) {
senderKeyStates.clear();
senderKeyStates.emplace_back(id, iteration, chainKey, keyPair);
}

std::vector<std::string> serialize() const {
std::vector<std::string> recordStructure;
for (const auto& senderKeyState : senderKeyStates) {
recordStructure.push_back(senderKeyState.serialize());
}
return recordStructure;
}
};
