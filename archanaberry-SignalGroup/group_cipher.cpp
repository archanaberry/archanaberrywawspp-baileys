#include "GroupCipher.hpp"
#include "crypto.hpp" // Asumsikan kita memiliki file ini

GroupCipher::GroupCipher(std::shared_ptr<SenderKeyStore> senderKeyStore, const SenderKeyName& senderKeyName)
: senderKeyStore(senderKeyStore), senderKeyName(senderKeyName) {}

std::future<std::string> GroupCipher::queueJob(std::function<std::future<std::string>()> awaitable) {
    return queue_job(senderKeyName.toString(), awaitable);
}

std::future<std::string> GroupCipher::encrypt(const std::string& paddedPlaintext) {
    return queueJob([this, paddedPlaintext]() -> std::future<std::string> {
        auto record = senderKeyStore->loadSenderKey(senderKeyName);
        if (!record) {
            throw std::runtime_error("No SenderKeyRecord found for encryption");
        }
std::future<std::string> GroupCipher::encrypt(const std::string& paddedPlaintext) {
    return queueJob([this, paddedPlaintext]() -> std::future<std::string> {
        auto record = senderKeyStore->loadSenderKey(senderKeyName);
        if (!record) {
            throw std::runtime_error("No SenderKeyRecord found for encryption");
        }

        auto senderKeyState = record->getSenderKeyState();
        if (!senderKeyState) {
            throw std::runtime_error("No session to encrypt message");
        }

        int iteration = senderKeyState->getSenderChainKey().getIteration();
        auto senderKey = getSenderKey(senderKeyState, iteration == 0 ? 0 : iteration + 1);

        auto ciphertext = getCipherText(
            senderKey.getIv(),
            senderKey.getCipherKey(),
            paddedPlaintext
        );

        SenderKeyMessage senderKeyMessage(
            senderKeyState->getKeyId(),
            senderKey.getIteration(),
            ciphertext,
            senderKeyState->getSigningKeyPrivate()
        );

        senderKeyStore->storeSenderKey(senderKeyName, record);
        return senderKeyMessage.serialize();

std::future<std::string> GroupCipher::decrypt(const std::vector<uint8_t>& senderKeyMessageBytes) {
    return queueJob([this, senderKeyMessageBytes]() -> std::future<std::string> {
        auto record = senderKeyStore->loadSenderKey(senderKeyName);
        if (!record) {
            throw std::runtime_error("No SenderKeyRecord found for decryption");
        }

        SenderKeyMessage senderKeyMessage(senderKeyMessageBytes);
        auto senderKeyState = record->getSenderKeyState(senderKeyMessage.getKeyId());
        if (!senderKeyState) {
            throw std::runtime_error("No session found to decrypt message");
        }

        senderKeyMessage.verifySignature(senderKeyState->getSigningKeyPublic());
        auto senderKey = getSenderKey(senderKeyState, senderKeyMessage.getIteration());

        auto plaintext = getPlainText(
            senderKey.getIv(),
            senderKey.getCipherKey(),
            senderKeyMessage.getCipherText()
        );

        senderKeyStore->storeSenderKey(senderKeyName, record);

        return plaintext;

SenderKey GroupCipher::getSenderKey(std::shared_ptr<SenderKeyState> senderKeyState, int iteration) {
    auto senderChainKey = senderKeyState->getSenderChainKey();
    if (senderChainKey.getIteration() > iteration) {
        if (senderKeyState->hasSenderMessageKey(iteration)) {
            return senderKeyState->removeSenderMessageKey(iteration);
        }
        throw std::runtime_error(
            "Received message with old counter: " + std::to_string(senderChainKey.getIteration()) + ", " + std::to_string(iteration)
        );
    }

    if (iteration - senderChainKey.getIteration() > 2000) {
        throw std::runtime_error("Over 2000 messages into the future!");
    }

    // Generate new SenderKey if needed and update the state
    // ...

    return senderKey; // Return the appropriate SenderKey object

SenderKey GroupCipher::getSenderKey(std::shared_ptr<SenderKeyState> senderKeyState, int iteration) {
auto senderChainKey = senderKeyState->getSenderChainKey();
while (senderChainKey.getIteration() < iteration) {
senderKeyState->addSenderMessageKey(senderChainKey.getSenderMessageKey());
senderChainKey = senderChainKey.getNext();
}

senderKeyState->setSenderChainKey(senderChainKey.getNext());
return senderChainKey.getSenderMessageKey();
}

std::string GroupCipher::getPlainText(const std::vector<uint8_t>& iv, const std::vector<uint8_t>& key, const std::vector<uint8_t>& ciphertext) {
try {
// Gunakan library kriptografi untuk dekripsi
// Contoh: std::string plaintext = Crypto::decrypt(key, ciphertext, iv);
return plaintext;
} catch (const std::exception& e) {
// Handle exception
throw std::runtime_error("InvalidMessageException");
}
}

std::vector<uint8_t> GroupCipher::getCipherText(const std::vector<uint8_t>& iv, const std::vector<uint8_t>& key, const std::string& plaintext) {
try {
// Gunakan library kriptografi untuk enkripsi
// Contoh: std::vector<uint8_t> crypted = Crypto::encrypt(key, plaintext, iv);
return crypted;
} catch (const std::exception& e) {
// Handle exception
throw std::runtime_error("InvalidMessageException");
}
}
    });
}
