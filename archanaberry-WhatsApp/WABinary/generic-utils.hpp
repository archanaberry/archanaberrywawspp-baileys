// BinaryNodeUtilities.hpp
#include <stdexcept>
#include <sstream>
#ifndef BINARY_NODE_UTILITIES_HPP
#define BINARY_NODE_UTILITIES_HPP

#include <vector>
#include <string>
#include <memory>
#include <algorithm>
#include <iterator>

// Assuming BinaryNode is defined somewhere with the following structure:
struct BinaryNode {
    std::string tag;
    std::vector<std::shared_ptr<BinaryNode>> content; // Using shared_ptr for simplicity
};

// Function to get children of a BinaryNode with a specific tag
std::vector<std::shared_ptr<BinaryNode>> getBinaryNodeChildren(const std::shared_ptr<BinaryNode>& node, const std::string& childTag) {
    std::vector<std::shared_ptr<BinaryNode>> children;
    if (node && !node->content.empty()) {
        std::copy_if(node->content.begin(), node->content.end(), std::back_inserter(children),
                     [&childTag](const std::shared_ptr<BinaryNode>& child) { return child->tag == childTag; });
    }
    return children;
}

// Function to get all children of a BinaryNode
std::vector<std::shared_ptr<BinaryNode>> getAllBinaryNodeChildren(const BinaryNode& node) {
    return node.content;
}

// Function to get a single child of a BinaryNode with a specific tag
std::shared_ptr<BinaryNode> getBinaryNodeChild(const std::shared_ptr<BinaryNode>& node, const std::string& childTag) {
    if (node && !node->content.empty()) {
        auto it = std::find_if(node->content.begin(), node->content.end(),
                               [&childTag](const std::shared_ptr<BinaryNode>& child) { return child->tag == childTag; });
        if (it != node->content.end()) {
            return *it;
        }
    }
    return nullptr;
}

// Function to get the buffer content of a BinaryNode child with a specific tag
std::vector<uint8_t> getBinaryNodeChildBuffer(const std::shared_ptr<BinaryNode>& node, const std::string& childTag) {
    auto child = getBinaryNodeChild(node, childTag);
    if (child && child->content.size() == 1) {
            // Assuming the content of the child node is a binary buffer
        // You will need to define how to convert the content to a buffer
        return {}; // Replace with actual buffer conversion
    }
    return {};
}

// Function to get the string content of a BinaryNode child with a specific tag
std::string getBinaryNodeChildString(const std::shared_ptr<BinaryNode>& node, const std::string& childTag) {
    auto child = getBinaryNodeChild(node, childTag);
    if (child && child->content.size() == 1) {
        // Assuming the content of the child node is a string or binary buffer
        // You will need to define how to convert the content to a string
        return {}; // Replace with actual string conversion
    }
    return "";
}
// Function to convert a buffer to an unsigned integer
uint64_t bufferToUInt(const std::vector<uint8_t>& buffer, size_t length) {
    uint64_t value = 0;
    for(size_t i = 0; i < length; ++i) {
        value = (value << 8) + buffer[i];
    }
    return value;
}

// Function to get an unsigned integer from a BinaryNode child with a specific tag
uint64_t getBinaryNodeChildUInt(const std::shared_ptr<BinaryNode>& node, const std::string& childTag, size_t length) {
    auto buffer = getBinaryNodeChildBuffer(node, childTag);
    if(!buffer.empty()) {
        return bufferToUInt(buffer, length);
    }
    return 0;
}

// Function to assert that a BinaryNode is error-free
void assertNodeErrorFree(const std::shared_ptr<BinaryNode>& node) {
    auto errNode = getBinaryNodeChild(node, "error");
    if(errNode) {
        // Replace with your project's exception handling
        throw std::runtime_error(errNode->attrs["text"] + " (" + errNode->attrs["code"] + ")");
    }
}

// Function to reduce a BinaryNode to a dictionary
std::map<std::string, std::string> reduceBinaryNodeToDictionary(const std::shared_ptr<BinaryNode>& node, const std::string& tag) {
    auto nodes = getBinaryNodeChildren(node, tag);
    std::map<std::string, std::string> dict;
    for(const auto& n : nodes) {
        dict[n->attrs["name"]] = n->attrs["value"];
    }
    return dict;
}

// Function to get messages from a BinaryNode
std::vector<proto::WebMessageInfo> getBinaryNodeMessages(const BinaryNode& node) {
    std::vector<proto::WebMessageInfo> msgs;
    if(!node.content.empty()) {
        for(const auto& item : node.content) {
            if(item->tag == "message") {
                // Assuming proto::WebMessageInfo has a decode method that takes a buffer
                msgs.push_back(proto::WebMessageInfo::decode(item->content));
            }
        }
    }
    return msgs;
}
// Function to repeat tabs
std::string tabs(size_t n) {
    return std::string(n, '    ');
}

// Function to convert a BinaryNode to a string
std::string binaryNodeToString(const std::shared_ptr<BinaryNode>& node, size_t indent = 0) {
    if(!node) {
        return "null";
    }

    std::stringstream ss;
    ss << tabs(indent);

    if(node->content.empty()) {
        ss << node->tag;
    } else {
        ss << "<" << node->tag << ">
";
        for(const auto& child : node->content) {
            ss << binaryNodeToString(child, indent + 1) << "
";
        }
        ss << tabs(indent) << "</" << node->tag << ">";
    }

    return ss.str();
}

// Function to convert a BinaryNode to a string (continued)
std::string binaryNodeToString(const std::shared_ptr<BinaryNode>& node, size_t indent = 0) {
    // ... (previous code)

    std::stringstream ss;
    ss << tabs(indent);

    // Check if node is a Uint8Array (or equivalent in C++)
    if (node->isUint8Array()) {
        ss << std::string(node->uint8Array.begin(), node->uint8Array.end());
    } else if (node->isArray()) {
        // If node is an array, recursively call binaryNodeToString on each element
        for (const auto& child : node->array) {
            ss << binaryNodeToString(child, indent + 1) << "
";
        }
    } else {
        // If node has children, recursively call binaryNodeToString on the content
        std::string children = binaryNodeToString(node->content, indent + 1);
        std::string tag = "<" + node->tag;

        // Add attributes to the tag
        for (const auto& attr : node->attrs) {
            if (attr.second.has_value()) {
                tag += " " + attr.first + "='" + attr.second.value() + "'";
            }
        }

        // Determine if the tag is self-closing or not
        std::string content = children.empty() ? "/>" : ">
" + children + "
" + tabs(indent) + "</" + node->tag + ">";
        ss << tag << content;
    }

    return ss.str();
}

#endif // BINARY_NODE_UTILITIES_HPP
