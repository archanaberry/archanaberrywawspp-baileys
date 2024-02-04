#include <vector>
#include <cstdint>
#include "sender_message_key.hpp" // Include the header for the SenderMessageKey class
#include "crypto.hpp" // Include the header for the cryptographic library

class SenderChainKey {
private:
    static const uint8_t MESSAGE_KEY_SEED = 0x01;
    static const uint8_t CHAIN_KEY_SEED = 0x02;

    uint32_t iteration;
    std::vector<uint8_t> chainKey;

public:
    SenderChainKey(uint32_t iteration, const std::vector<uint8_t>& chainKey)
    : iteration(iteration), chainKey(chainKey) {}

    uint32_t getIteration() const {
        return iteration;
    }

    SenderMessageKey getSenderMessageKey() {
        std::vector<uint8_t> seed = {MESSAGE_KEY_SEED};
        std::vector<uint8_t> derivative = getDerivative(seed, chainKey);
        return SenderMessageKey(iteration, derivative);
    }

    SenderChainKey getNext() {
        std::vector<uint8_t> seed = {CHAIN_KEY_SEED};
        std::vector<uint8_t> nextChainKey = getDerivative(seed, chainKey);
        return SenderChainKey(iteration + 1, nextChainKey);
    }

    std::vector<uint8_t> getSeed() const {
        return chainKey;
    }

    std::vector<uint8_t> getDerivative(const std::vector<uint8_t>& seed, const std::vector<uint8_t>& key) {
        // Assuming the crypto library has a function calculateMAC that takes a key and a seed and returns the HMAC
        return Crypto::calculateMAC(key, seed);
    }
};
