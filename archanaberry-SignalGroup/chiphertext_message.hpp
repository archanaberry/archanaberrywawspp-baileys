class CiphertextMessage {
public:
    static const int UNSUPPORTED_VERSION = 1;
    static const int CURRENT_VERSION = 3;
    static const int WHISPER_TYPE = 2;
    static const int PREKEY_TYPE = 3;
    static const int SENDERKEY_TYPE = 4;
    static const int SENDERKEY_DISTRIBUTION_TYPE = 5;
    static const int ENCRYPTED_MESSAGE_OVERHEAD = 53;
};
