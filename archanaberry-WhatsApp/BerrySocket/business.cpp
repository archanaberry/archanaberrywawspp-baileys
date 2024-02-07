#include <string>
#include <vector>
#include <memory>
#include "SocketConfig.h" // You would need to create this header file
#include "Types.h" // You would need to create this header file
#include "businessUtils.h" // You would need to create this header file
#include "WABinary.h" // You would need to create this header file
#include "genericUtils.h" // You would need to create this header file
#include "messagesRecvSocket.h" // You would need to create this header file

class BusinessSocket {
public:
    BusinessSocket(const SocketConfig& config) : sock(makeMessagesRecvSocket(config)) {
        // Initialize authState, query, waUploadToServer, etc.
    }

    // Synchronous version of getCatalog
    std::vector<ProductNode> getCatalog(const GetCatalogOptions& options) {
        std::string jid = options.jid.empty() ? authState.creds.me.id : jidNormalizedUser(options.jid);
        int limit = options.limit.value_or(10);
        std::string cursor = options.cursor;

        std::vector<BinaryNode> queryParamNodes = {
            {"limit", {}, std::to_string(limit)},
            {"width", {}, "100"},
            {"height", {}, "100"}
        };

        if (!cursor.empty()) {
            queryParamNodes.push_back({"after", {}, cursor});
        }

        // Perform the query and parse the response
        // ...

// Synchronous version of getCollections
std::vector<CollectionNode> getCollections(const std::string& jid = "", int limit = 51) {
    std::string normalizedJid = jid.empty() ? authState.creds.me.id : jidNormalizedUser(jid);
    // Perform the query and parse the response
    // ...

return {};
}

    // Construct the query node
    BinaryNode queryNode = {
        "iq",
        {
            {"to", S_WHATSAPP_NET},
            {"type", "get"},
            {"xmlns", "w:biz:catalog"},
            {"smax_id", "35"}
        },
        // ... (Construct the content of the query node)
    };

    // Perform the query and get the result
    BinaryNode result = query(queryNode);

    // Parse the result into a vector of CollectionNode objects
    std::vector<CollectionNode> collections = parseCollectionsNode(result);

// Synchronous version of getCollections
std::vector<CollectionNode> getCollections(const std::string& jid = "", int limit = 51) {
    std::string normalizedJid = jid.empty() ? authState.creds.me.id : jidNormalizedUser(jid);

    // Construct the content of the query node
    std::vector<BinaryNode> collectionContent = {
        {"collection_limit", {}, std::to_string(limit)},
        {"item_limit", {}, std::to_string(limit)},
        {"width", {}, "100"},
        {"height", {}, "100"}
    };

    BinaryNode collectionsNode = {
        "collections",
        {{"biz_jid", normalizedJid}},
        collectionContent
    };

    // Construct the query node
    BinaryNode queryNode = {
        "iq",
        {
            {"to", S_WHATSAPP_NET},
            {"type", "get"},
            {"xmlns", "w:biz:catalog"}
        },
        {collectionsNode}
    };

    // Perform the query and get the result
    BinaryNode result = query(queryNode);

    // Parse the result into a vector of CollectionNode objects
    std::vector<CollectionNode> collections = parseCollectionsNode(result);

    return collections;
}

// ... (Rest of the BusinessSocket class code)

// Synchronous version of getOrderDetails
OrderDetailsNode getOrderDetails(const std::string& orderId, const std::string& tokenBase64) {
    // Construct the content of the image_dimensions node
    std::vector<BinaryNode> imageDimensionsContent = {
        {"width", {}, "100"},
        {"height", {}, "100"}
    };

    BinaryNode imageDimensionsNode = {
        "image_dimensions",
        {},
        imageDimensionsContent
    };

    // Construct the content of the order node
    std::vector<BinaryNode> orderContent = {
        imageDimensionsNode,
        {"token", {}, tokenBase64}
    };

    BinaryNode orderNode = {
        "order",
        {{"op", "get"}, {"id", orderId}},
        orderContent
    };

    // Construct the query node
    BinaryNode queryNode = {
        "iq",
        {
            {"to", S_WHATSAPP_NET},
            {"type", "get"},
            {"xmlns", "fb:thrift_iq"},
            {"smax_id", "5"}
        },
        {orderNode}
    };

    // Perform the query and get the result
    BinaryNode result = query(queryNode);

    // Parse the result into an OrderDetailsNode object
    OrderDetailsNode orderDetails = parseOrderDetailsNode(result);

    return orderDetails;
}

// ... (Rest of the BusinessSocket class code)

// ... Rest of your code ...

// Synchronous version of productUpdate
ProductNode productUpdate(const std::string& productId, ProductUpdate update) {
// Assume uploadingNecessaryImagesOfProduct is a function that handles image uploads
// and returns an updated ProductUpdate object with necessary image references
update = uploadingNecessaryImagesOfProduct(update, waUploadToServer);

// Assume toProductNode is a function that converts a ProductUpdate object into a BinaryNode
BinaryNode editNode = toProductNode(productId, update);

// Construct the content of the product_catalog_edit node
std::vector<BinaryNode> productCatalogEditContent = {
editNode,
{"width", {}, "100"},
{"height", {}, "100"}
};

BinaryNode productCatalogEditNode = {
"product_catalog_edit",
{{"v", "1"}},
productCatalogEditContent
};

// Construct the query node
BinaryNode queryNode = {
"iq",
{
{"to", S_WHATSAPP_NET},
{"type", "set"},
{"xmlns", "w:biz:catalog"}
},
{productCatalogEditNode}
};

// Perform the query and get the result
BinaryNode result = query(queryNode);

// Assume getBinaryNodeChild is a function that retrieves a child node by tag name
BinaryNode productCatalogEditResultNode = getBinaryNodeChild(result, "product_catalog_edit");
BinaryNode productNode = getBinaryNodeChild(productCatalogEditResultNode, "product");

// Assume parseProductNode is a function that parses a product node into a ProductNode object
ProductNode productDetails = parseProductNode(productNode);

// Synchronous version of productCreate
ProductNode productCreate(ProductCreate create) {
// Ensure isHidden is defined
create.isHidden = create.isHidden ? true : false;

// Assume uploadingNecessaryImagesOfProduct is a function that handles image uploads
// and returns an updated ProductCreate object with necessary image references
create = uploadingNecessaryImagesOfProduct(create, waUploadToServer);

// Assume toProductNode is a function that converts a ProductCreate object into a BinaryNode
BinaryNode createNode = toProductNode("", create);

// Construct the content of the product_catalog_add node
std::vector<BinaryNode> productCatalogAddContent = {
createNode,
{"width", {}, "100"},
{"height", {}, "100"}
};

BinaryNode productCatalogAddNode = {
"product_catalog_add",
{{"v", "1"}},
productCatalogAddContent
};

// Construct the query node
BinaryNode queryNode = {
"iq",
{
{"to", S_WHATSAPP_NET},
{"type", "set"},
{"xmlns", "w:biz:catalog"}
},
{productCatalogAddNode}
};

// Perform the query and get the result
BinaryNode result = query(queryNode);

// Assume getBinaryNodeChild is a function that retrieves a child node by tag name
BinaryNode productCatalogAddResultNode = getBinaryNodeChild(result, "product_catalog_add");
BinaryNode productNode = getBinaryNodeChild(productCatalogAddResultNode, "product");

// Assume parseProductNode is a function that parses a product node into a ProductNode object
ProductNode productDetails = parseProductNode(productNode);

return productDetails;
}

// ... (Rest of the BusinessSocket class code)

// ... Rest of your code ...

// ... (Previous BusinessSocket class code)

// Synchronous version of productDelete
int productDelete(const std::vector<std::string>& productIds) {
    // Construct the content of the product_catalog_delete node with product ids
    std::vector<BinaryNode> productNodes;
    for (const auto& id : productIds) {
        productNodes.push_back({
            "product",
            {},
            {{"id", {}, id}}
        });
    }

    BinaryNode productCatalogDeleteNode = {
        "product_catalog_delete",
        {{"v", "1"}},
        productNodes
    };

    // Construct the query node
    BinaryNode queryNode = {
        "iq",
        {
            {"to", S_WHATSAPP_NET},
            {"type", "set"},
            {"xmlns", "w:biz:catalog"}
        },
        {productCatalogDeleteNode}
    };

    // Perform the query and get the result
    BinaryNode result = query(queryNode);

    // Assume getBinaryNodeChild is a function that retrieves a child node by tag name
    BinaryNode productCatalogDelNode = getBinaryNodeChild(result, "product_catalog_delete");

    // Extract the deleted_count attribute and convert it to an integer
    int deletedCount = 0;
    if (productCatalogDelNode.hasAttribute("deleted_count")) {
        deletedCount = std::stoi(productCatalogDelNode.getAttribute("deleted_count"));
    }

    return deletedCount;
}

// ... (Rest of the BusinessSocket class code)

// ... Rest of your code ...
private:
    MessagesRecvSocket sock;
    // Other private members...
};

// ... Rest of your code ...