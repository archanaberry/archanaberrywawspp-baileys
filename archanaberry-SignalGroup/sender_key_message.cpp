#include <vector>
#include <memory>
#include "ciphertext_message.hpp" // You would need to create this header file
#include "curve.hpp" // You would need to create this header file or include a library that provides similar functionality
#include "protobufs.hpp" // You would need to create this header file or include a library that provides protobuf functionality

class SenderKeyMessage : public CiphertextMessage {
public:
static const size_t SIGNATURE_LENGTH = 64;

SenderKeyMessage(
int keyId = 0,
int iteration = 0,
std::vector<uint8_t> ciphertext = {},
std::shared_ptr<Curve::KeyPair> signatureKey = nullptr,
std::vector<uint8_t> serialized = {}
) : CiphertextMessage() {
if (!serialized.empty()) {
uint8_t version = serialized[0];
std::vector<uint8_t> message(serialized.begin() + 1, serialized.end() - SIGNATURE_LENGTH);
std::vector<uint8_t> signature(serialized.end() - SIGNATURE_LENGTH, serialized.end());
auto senderKeyMessage = Protobufs::SenderKeyMessage::decode(message);

this->serialized = serialized;
this->messageVersion = (version >> 4) & 0x0F;

this->keyId = senderKeyMessage.id();
this->iteration = senderKeyMessage.iteration();
this->ciphertext = senderKeyMessage.ciphertext();
this->signature = signature;
} else {
uint8_t version = ((CURRENT_VERSION << 4) | CURRENT_VERSION) & 0xFF;
auto message = Protobufs::SenderKeyMessage::encode(
Protobufs::SenderKeyMessage::create(keyId, iteration, ciphertext)
);

std::vector<uint8_t> versionedMessage = {version};
versionedMessage.insert(versionedMessage.end(), message.begin(), message.end());

this->signature = getSignature(signatureKey, versionedMessage);
// ... Continue with the rest of the constructor logic
}
}

private:
// ... Private member variables and methods

std::vector<uint8_t> getSignature(std::shared_ptr<Curve::KeyPair> signatureKey, const std::vector<uint8_t>& message) {
// Implement signature logic using the Curve library
// ...
}
};

int getKeyId() const {
    return keyId;
}

int getIteration() const {
    return iteration;
}

std::vector<uint8_t> getCipherText() const {
    return ciphertext;
}

void verifySignature(const std::shared_ptr<Curve::KeyPair>& signatureKey) {
    std::vector<uint8_t> part1(serialized.begin(), serialized.end() - SIGNATURE_LENGTH);
    std::vector<uint8_t> part2(serialized.end() - SIGNATURE_LENGTH, serialized.end());
    bool res = Curve::verifySignature(signatureKey, part1, part2);
    if (!res) {
        throw std::runtime_error("Invalid signature!");
    }
}

std::vector<uint8_t> getSignature(const std::shared_ptr<Curve::KeyPair>& signatureKey, const std::vector<uint8_t>& serialized) {
    return Curve::calculateSignature(signatureKey, serialized);
}

std::vector<uint8_t> serialize() const {
    return serialized;
}

int getType() const {
    return 4;
}

// ... Other necessary code

// End of SenderKeyMessage class

// In your main code or another appropriate location, you would include the implementation for the Curve class methods.
// For example:
namespace Curve {
    bool verifySignature(const std::shared_ptr<KeyPair>& signatureKey, const std::vector<uint8_t>& message, const std::vector<uint8_t>& signature) {
        // Implement signature verification logic
        // ...
    }

    std::vector<uint8_t> calculateSignature(const std::shared_ptr<KeyPair>& signatureKey, const std::vector<uint8_t>& message) {
        // Implement signature calculation logic
        // ...
    }
}

// You would also need to implement or include the necessary serialization and deserialization logic for protobufs.
