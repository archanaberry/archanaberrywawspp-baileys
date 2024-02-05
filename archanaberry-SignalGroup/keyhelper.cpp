#include <openssl/rand.h>
#include <openssl/bn.h>
#include <openssl/ec.h>
#include <openssl/obj_mac.h>
#include <vector>
#include <cstdint>

namespace SenderKeyUtils {

std::vector<uint8_t> generateSenderKey() {
    std::vector<uint8_t> key(32);
    if (RAND_bytes(key.data(), key.size()) != 1) {
        throw std::runtime_error("Failed to generate random bytes for sender key");
    }
    return key;
}

uint32_t generateSenderKeyId() {
    return static_cast<uint32_t>(BN_get_word(BN_new()));
}

std::pair<std::vector<uint8_t>, std::vector<uint8_t>> generateSenderSigningKey() {
    EC_KEY *key = EC_KEY_new_by_curve_name(NID_X9_62_prime256v1);
    if (!key || !EC_KEY_generate_key(key)) {
        throw std::runtime_error("Failed to generate EC key pair");
    }

    const EC_GROUP *group = EC_KEY_get0_group(key);
    const EC_POINT *pub_key = EC_KEY_get0_public_key(key);
    const BIGNUM *priv_key = EC_KEY_get0_private_key(key);

    // Convert public key to binary form
    std::vector<uint8_t> public_key(EC_POINT_point2oct(group, pub_key, POINT_CONVERSION_COMPRESSED, nullptr, 0, nullptr));
    EC_POINT_point2oct(group, pub_key, POINT_CONVERSION_COMPRESSED, public_key.data(), public_key.size(), nullptr);

    // Convert private key to binary form
    std::vector<uint8_t> private_key(BN_num_bytes(priv_key));
    BN_bn2bin(priv_key, private_key.data());

    EC_KEY_free(key);

    return {public_key, private_key};
}

} // namespace SenderKeyUtils
