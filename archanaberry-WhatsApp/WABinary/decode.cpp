#include <zlib.h>
#include <vector>
#include <stdexcept>
#include <string>
#include <map>
#include "Constants.hpp" // Replace with the actual path to your constants
#include "JidUtils.hpp" // Replace with the actual path to your JID utilities
#include "Types.hpp" // Replace with the actual path to your type definitions

std::vector<uint8_t> decompressIfRequired(std::vector<uint8_t>& buffer) {
    if (2 & buffer[0]) {
        // Decompress the buffer using zlib
        uLongf decompressedSize = buffer.size() * 2; // Estimate the decompressed size
        std::vector<uint8_t> decompressedBuffer(decompressedSize);

        while (true) {
            int result = uncompress(decompressedBuffer.data(), &decompressedSize, buffer.data() + 1, buffer.size() - 1);
            if (result == Z_OK) {
                decompressedBuffer.resize(decompressedSize);
                break;
            } else if (result == Z_BUF_ERROR) {
                // Increase buffer size and try again
                decompressedSize *= 2;
                decompressedBuffer.resize(decompressedSize);
            } else {
                // Handle other zlib errors
                throw std::runtime_error("Failed to decompress buffer");
            }
        }

        return decompressedBuffer;
    } else {
        // Nodes with no compression have a 0x00 prefix, we remove that
        return std::vector<uint8_t>(buffer.begin() + 1, buffer.end());
    }
}

BinaryNode decodeDecompressedBinaryNode(
    std::vector<uint8_t>& buffer,
    const BinaryNodeCodingOptions& opts,
    size_t& index
) {
    const auto& [DOUBLE_BYTE_TOKENS, SINGLE_BYTE_TOKENS, TAGS] = opts;

    auto checkEOS = [&](size_t length) {
        if (index + length > buffer.size()) {
            throw std::runtime_error("end of stream");
        }
    };

    auto next = [&]() -> uint8_t {
        checkEOS(1);
        return buffer[index++];
    };

    auto readBytes = [&](size_t n) -> std::vector<uint8_t> {
        checkEOS(n);
        
// ... (Other includes and definitions)

BinaryNode decodeDecompressedBinaryNode(const std::vector<uint8_t>& buffer) {
    uint8_t listSizeTag = readByte();
    size_t listSize = readListSize(listSizeTag);
    std::string header = readString(readByte());
    if (listSize == 0 || header.empty()) {
        throw std::runtime_error("invalid node");
    }

    std::map<std::string, std::string> attrs;
    std::variant<std::vector<BinaryNode>, std::string, std::vector<uint8_t>> data;

    // read the attributes in
    size_t attributesLength = (listSize - 1) / 2;
    for (size_t i = 0; i < attributesLength; ++i) {
        std::string key = readString(readByte());
        std::string value = readString(readByte());
        attrs[key] = value;
    }

    if (listSize % 2 == 0) {
        uint8_t tag = readByte();
        if (isListTag(tag)) {
            data = readList(tag);
        } else {
            std::vector<uint8_t> decoded;
            switch (tag) {
                case TAGS::BINARY_8:
                    decoded = readBytes(readByte());
                    break;
                case TAGS::BINARY_20:
                    decoded = readBytes(readInt20());
                    break;
                case TAGS::BINARY_32:
                    decoded = readBytes(readInt(4));
                    break;
                default:
                    data = readString(tag);
                    break;
            }
            if (std::holds_alternative<std::vector<uint8_t>>(data)) {
                data = decoded;
            }
        }
    }

    BinaryNode node;
    node.tag = header;
    node.attrs = attrs;
    node.content = data;

    return node;
}

BinaryNode decodeBinaryNode(const std::vector<uint8_t>& buff) {
    std::vector<uint8_t> decompBuff = decompressingIfRequired(buff);
    return decodeDecompressedBinaryNode(decompBuff);
}

// ... (Other functions and logic)
// ... (Other includes and definitions)

BinaryNode decodeDecompressedBinaryNode(const std::vector<uint8_t>& buffer) {
    uint8_t listSizeTag = readByte();
    size_t listSize = readListSize(listSizeTag);
    std::string header = readString(readByte());
    if (listSize == 0 || header.empty()) {
        throw std::runtime_error("invalid node");
    }

    std::map<std::string, std::string> attrs;
    std::variant<std::vector<BinaryNode>, std::string, std::vector<uint8_t>> data;

    // read the attributes in
    size_t attributesLength = (listSize - 1) / 2;
    for (size_t i = 0; i < attributesLength; ++i) {
        std::string key = readString(readByte());
        std::string value = readString(readByte());
        attrs[key] = value;
    }

    if (listSize % 2 == 0) {
        uint8_t tag = readByte();
        if (isListTag(tag)) {
            data = readList(tag);
        } else {
            std::vector<uint8_t> decoded;
            switch (tag) {
                case TAGS::BINARY_8:
                    decoded = readBytes(readByte());
                    break;
                case TAGS::BINARY_20:
                    decoded = readBytes(readInt20());
                    break;
                case TAGS::BINARY_32:
                    decoded = readBytes(readInt(4));
                    break;
                default:
                    data = readString(tag);
                    break;
            }
        }
    }

    BinaryNode node;
    node.tag = header;
    node.attrs = attrs;
    node.content = data;

    return node;
}

// ... (Other functions and logic
