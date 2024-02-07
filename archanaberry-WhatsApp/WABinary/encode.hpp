// BinaryNodeEncoder.hpp
#include <vector>
#include <cstdint>
#include <stdexcept>
#include "constants.hpp" // Make sure to define the constants similarly to the TypeScript constants
#include "jid_utils.hpp" // Include jidDecode and related functions
#include "types.hpp"     // Include BinaryNode and BinaryNodeCodingOptions definitions

class BinaryNodeEncoder {
public:
    static std::vector<uint8_t> encodeBinaryNode(
        const BinaryNode& node,
        const BinaryNodeCodingOptions& opts = constants::defaultCodingOptions
    ) {
        std::vector<uint8_t> buffer = {0};
        const auto& TAGS = opts.TAGS;
        const auto& TOKEN_MAP = opts.TOKEN_MAP;

        auto pushByte = [&buffer](uint8_t value) {
            buffer.push_back(value);
        };

        auto pushInt = [&buffer](uint32_t value, size_t n, bool littleEndian = false) {
            for(size_t i = 0; i < n; i++) {
                size_t curShift = littleEndian ? i : n - 1 - i;
                buffer.push_back((value >> (curShift * 8)) & 0xff);
            }
        };

        auto pushBytes = [&buffer](const std::vector<uint8_t>& bytes) {
            buffer.insert(buffer.end(), bytes.begin(), bytes.end());
        };

        auto pushInt16 = [&pushBytes](uint16_t value) {
            pushBytes({static_cast<uint8_t>((value >> 8) & 0xff), static_cast<uint8_t>(value & 0xff)});
        };

        auto pushInt20 = [&pushBytes](uint32_t value) {
            pushBytes({static_cast<uint8_t>((value >> 16) & 0x0f), static_cast<uint8_t>((value >> 8) & 0xff), static_cast<uint8_t>(value & 0xff)});
        };

        auto writeByteLength = [&pushByte, &pushInt, &pushInt20, &TAGS](size_t length) {
            if(length >= 4294967296) {
                throw std::runtime_error("string too large to encode: " + std::to_string(length));
            }

            if(length >= (1 << 20)) {
                pushByte(TAGS.BINARY_32);
                
    static void writeStringRaw(std::vector<uint8_t>& buffer, const std::string& str) {
        std::vector<uint8_t> bytes(str.begin(), str.end());
        writeByteLength(buffer, bytes.size());
        pushBytes(buffer, bytes);
    }

    static void writeJid(std::vector<uint8_t>& buffer, const FullJid& jid) {
        if (jid.device.has_value()) {
            pushByte(buffer, TAGS.AD_JID);
            pushByte(buffer, jid.domainType.value_or(0));
            pushByte(buffer, jid.device.value_or(0));
            writeString(buffer, jid.user);
        } else {
            pushByte(buffer, TAGS.JID_PAIR);
            if (!jid.user.empty()) {
                writeString(buffer, jid.user);
            } else {
                pushByte(buffer, TAGS.LIST_EMPTY);
            }
            writeString(buffer, jid.server);
        }
    }

    static uint8_t packNibble(char c) {
        switch (c) {
            case '-': return 10;
            case '.': return 11;
            case '': return 15;
            default:
                if (c >= '0' && c <= '9') {
                    return c - '0';
                }
                throw std::runtime_error(std::string("invalid byte for nibble "") + c + """);
        }
    }

    static uint8_t packHex(char c) {
        if (c >= '0' && c <= '9') {
            return c - '0';
        }
        // ... (continue with the rest of the hex packing logic)
        // ...

        // Continue with the rest of the encoding logic...
        // ...
    }

static uint8_t packHex(char c) {
    if (c >= '0' && c <= '9') {
        return c - '0';
    } else if (c >= 'A' && c <= 'F') {
        return 10 + (c - 'A');
    } else if (c >= 'a' && c <= 'f') {
        return 10 + (c - 'a');
    } else if (c == '') {
        return 15;
    }
    throw std::runtime_error(std::string("Invalid hex char "") + c + """);
}

static void writePackedBytes(std::vector<uint8_t>& buffer, const std::string& str, const std::string& type) {
    if (str.size() > TAGS.PACKED_MAX) {
        throw std::runtime_error("Too many bytes to pack");
    }

    pushByte(buffer, type == "nibble" ? TAGS.NIBBLE_8 : TAGS.HEX_8);

    size_t roundedLength = (str.size() + 1) / 2;
    if (str.size() % 2 != 0) {
        roundedLength |= 128;
    }

    pushByte(buffer, static_cast<uint8_t>(roundedLength));
    auto packFunction = type == "nibble" ? packNibble : packHex;

    for (size_t i = 0; i < str.size() / 2; ++i) {
        pushByte(buffer, (packFunction(str[2 * i]) << 4) | packFunction(str[2 * i + 1]));
    }

    if (str.size() % 2 != 0) {
        pushByte(buffer, packFunction(str[str.size() - 1]) << 4);
    }
}

static bool isNibble(const std::string& str) {
if (str.size() > TAGS.PACKED_MAX) {
return false;
}

for (char c : str) {
bool isInNibbleRange = c >= '0' && c <= '9';
if (!isInNibbleRange && c != '-' && c != '.') {
return false;
}
}

return true;
}

static bool isHex(const std::string& str) {
if (str.size() > TAGS.PACKED_MAX) {
return false;
}

for (char c : str) {
bool isInHexRange = (c >= '0' && c <= '9') || (c >= 'A' && c <= 'F') || (c >= 'a' && c <= 'f');
if (!isInHexRange) {
return false;
}
}

return true;
}

static void writeString(std::vector<uint8_t>& buffer, const std::string& str) {
auto tokenIndex = TOKEN_MAP.find(str);
if (tokenIndex != TOKEN_MAP.end()) {
if (tokenIndex->second.dict >= 0) {
pushByte(buffer, TAGS.DICTIONARY_0 + tokenIndex->second.dict);
}
pushByte(buffer, tokenIndex->second.index);
} else if (isNibble(str)) {
writePackedBytes(buffer, str, "nibble");
} else if (isHex(str)) {
writePackedBytes(buffer, str, "hex");
} else if (!str.empty()) {
auto decodedJid = jidDecode(str); // Implement jidDecode according to your project
if (decodedJid.has_value()) {
writeJid(buffer, decodedJid.value());
} else {
writeStringRaw(buffer, str);
}
}
}

static void writeListStart(std::vector<uint8_t>& buffer, size_t listSize) {
if (listSize == 0) {
pushByte(buffer, TAGS.LIST_EMPTY);
} else if (listSize < 256) {
pushByte(buffer, TAGS.LIST_8);
pushByte(buffer, static_cast<uint8_t>(listSize));
} else {
pushByte(buffer, TAGS.LIST_16);
pushInt16(buffer, static_cast<uint16_t>(listSize));
}
}

static std::vector<uint8_t> encodeBinaryNode(const std::string& tag, const std::map<std::string, std::string>& attrs, const NodeContent& content) {
std::vector<uint8_t> buffer;

// Filter out valid attributes
std::vector<std::string> validAttributes;
for (const auto& attr : attrs) {
if (!attr.second.empty()) {
validAttributes.push_back(attr.first);
}
}

writeListStart(buffer, 2 * validAttributes.size() + 1 + (!content.empty() ? 1 : 0));
writeString(buffer, tag);

for (const auto& key : validAttributes) {
writeString(buffer, key);
writeString(buffer, attrs.at(key));
}

// Handle content based on its type
if (std::holds_alternative<std::string>(content)) {
writeString(buffer, std::get<std::string>(content));
} else if (std::holds_alternative<std::vector<uint8_t>>(content)) {
const auto& bytes = std::get<std::vector<uint8_t>>(content);
writeByteLength(buffer, bytes.size());
pushBytes(buffer, bytes);
} else if (std::holds_alternative<std::vector<BinaryNode>>(content)) {
const auto& children = std::get<std::vector<BinaryNode>>(content);
writeListStart(buffer, children.size());
for (const auto& child : children) {
auto childBuffer = encodeBinaryNode(child.tag, child.attrs, child.content);
buffer.insert(buffer.end(), childBuffer.begin(), childBuffer.end());
}
} else {
throw std::runtime_error(std::string("Invalid content for tag "") + tag + """);
}

return buffer;
}
};

// End of BinaryNodeEncoder.hpp
