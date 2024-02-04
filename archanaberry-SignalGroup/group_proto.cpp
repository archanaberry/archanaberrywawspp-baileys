#include "groupproto.pb.h"

int main() {
    // Initialize the protobuf library (required for some protobuf versions)
    GOOGLE_PROTOBUF_VERIFY_VERSION;

    // Create a new SenderKeyDistributionMessage object
    groupproto::SenderKeyDistributionMessage distribution_message;
    distribution_message.set_id(1234);
    distribution_message.set_iteration(1);
    distribution_message.set_chainKey("chain_key_data");
    distribution_message.set_signingKey("signing_key_data");

    // Serialize the message to a string
    std::string serialized_message;
    if (!distribution_message.SerializeToString(&serialized_message)) {
        std::cerr << "Failed to serialize message." << std::endl;
        return -1;
    }

    // Deserialize the message from a string
    groupproto::SenderKeyDistributionMessage deserialized_message;
    if (!deserialized_message.ParseFromString(serialized_message)) {
        std::cerr << "Failed to parse message." << std::endl;
        return -1;
    }

    // Use the deserialized message
    std::cout << "ID: " << deserialized_message.id() << std::endl;
    std::cout << "Iteration: " << deserialized_message.iteration() << std::endl;

    // Optional: Clean up protobuf library (for some protobuf versions)
    google::protobuf::ShutdownProtobufLibrary();

    return 0;
}
