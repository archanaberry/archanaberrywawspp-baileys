#include "sender_key_store.h"
#include "sender_key_distribution_message.h"
#include "keyhelper.h"

class GroupSessionBuilder {
private:
    SenderKeyStore* senderKeyStore;

public:
    GroupSessionBuilder(SenderKeyStore* store) : senderKeyStore(store) {}

    void process(const SenderKeyName& senderKeyName, const SenderKeyDistributionMessage& senderKeyDistributionMessage) {
        // Load or create a new SenderKeyRecord for the given SenderKeyName
        SenderKeyRecord senderKeyRecord = senderKeyStore->loadSenderKey(senderKeyName);
        // Add a new SenderKeyState to the record using the distribution message details
        senderKeyRecord.addSenderKeyState(
            senderKeyDistributionMessage.getId(),
            senderKeyDistributionMessage.getIteration(),
            senderKeyDistributionMessage.getChainKey(),
            senderKeyDistributionMessage.getSignatureKey()
        );
        // Store the updated SenderKeyRecord
        senderKeyStore->storeSenderKey(senderKeyName, senderKeyRecord);
    }

    SenderKeyDistributionMessage create(const SenderKeyName& senderKeyName) {
        // Load or create a new SenderKeyRecord for the given SenderKeyName
        SenderKeyRecord senderKeyRecord = senderKeyStore->loadSenderKey(senderKeyName);

        if (senderKeyRecord.isEmpty()) {
            // Generate new key components
            int keyId = KeyHelper::generateSenderKeyId();
            std::vector<uint8_t> senderKey = KeyHelper::generateSenderKey();
            std::vector<uint8_t> signingKey = KeyHelper::generateSenderSigningKey();

            // Set the new SenderKeyState
            senderKeyRecord.setSenderKeyState(keyId, 0, senderKey, signingKey);
            // Store the updated SenderKeyRecord
            senderKeyStore->storeSenderKey(senderKeyName, senderKeyRecord);
        }

        // Retrieve the current SenderKeyState
        SenderKeyState state = senderKeyRecord.getSenderKeyState();
