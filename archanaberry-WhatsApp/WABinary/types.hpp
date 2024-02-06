// constants.hpp
#ifndef CONSTANTS_HPP
#define CONSTANTS_HPP

// Include all the constants from the './constants' module
// ...

#endif // CONSTANTS_HPP

// BinaryNode.hpp
#ifndef BINARY_NODE_HPP
#define BINARY_NODE_HPP

#include <string>
#include <unordered_map>
#include <vector>
#include <memory>
#include "constants.hpp"

// Forward declaration of BinaryNode to resolve circular dependency
class BinaryNode;

// Define a type for attributes
using BinaryNodeAttributes = std::unordered_map<std::string, std::string>;

// Define a type for node content, which can be a string, binary data, or a list of BinaryNodes
using BinaryNodeData = std::variant<std::string, std::vector<uint8_t>, std::vector<std::shared_ptr<BinaryNode>>>;

// BinaryNode class definition
class BinaryNode {
public:
    std::string tag;
    BinaryNodeAttributes attrs;
    BinaryNodeData content;

    // Constructor
    BinaryNode(const std::string& tag, const BinaryNodeAttributes& attrs, const BinaryNodeData& content = {})
        : tag(tag), attrs(attrs), content(content) {}

    // Additional methods as needed
};

// BinaryNodeCodingOptions is a placeholder for the constants used in coding
// Assuming constants.hpp defines a namespace or struct with the necessary constants
using BinaryNodeCodingOptions = constants;

#endif // BINARY_NODE_HPP
