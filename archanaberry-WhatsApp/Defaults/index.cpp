#include <openssl/md5.h>
#include <string>
#include <array>
#include <vector>
#include <fstream>
#include <string>
#include <vector>
#include <regex>
#include <map>
#include <functional>
#include <memory>
#include <nlohmann/json.hpp> // Include JSON library, such as nlohmann/json

// Constants
const std::vector<int> UNAUTHORIZED_CODES = {401, 403, 419};
const std::string DEFAULT_ORIGIN = "https://web.whatsapp.com";
const std::string MOBILE_ENDPOINT = "g.whatsapp.net";
const int MOBILE_PORT = 443;
const std::string DEF_CALLBACK_PREFIX = "CB:";
const std::string DEF_TAG_PREFIX = "TAG:";
const std::string PHONE_CONNECTION_CB = "CB:Pong";
const int WA_DEFAULT_EPHEMERAL = 7 * 24 * 60 * 60;

// Version and hash
const std::string WA_VERSION = "2.23.14.82";
const std::string WA_VERSION_HASH = createMD5Hash(WA_VERSION);

// Function to create MD5 hash
std::string createMD5Hash(const std::string& input) {
    unsigned char result[MD5_DIGEST_LENGTH];
    MD5(reinterpret_cast<const unsigned char*>(input.c_str()), input.size(), result);

    std::stringstream hexStream;
    for (auto byte : result) {
        hexStream << std::hex << std::setfill('0') << std::setw(2) << (int)byte;
    }
    return hexStream.str();
}

// Load phoneNumberMCC from JSON file
nlohmann::json phoneNumberMCC;
std::ifstream phoneNumberMCCFile("phonenumber-mcc.json");
if (phoneNumberMCCFile.is_open()) {
    phoneNumberMCCFile >> phoneNumberMCC;
    phoneNumberMCCFile.close();
}

// Convert MOBILE_TOKEN to a byte array
std::vector<uint8_t> MOBILE_TOKEN;
std::string tokenString = "0a1mLfGUIBVrMKF1RdvLI5lkRBvof6vn0fD2QRSM" + WA_VERSION_HASH;
for (size_t i = 0; i < tokenString.size(); ++i) {
    MOBILE_TOKEN.push_back(static_cast<uint8_t>(tokenString[i]));
}

// Constants
const std::string MOBILE_REGISTRATION_ENDPOINT = "https://v.whatsapp.net/v2";
const std::string MOBILE_USERAGENT = "WhatsApp/" + WA_VERSION + " iOS/15.3.1 Device/Apple-iPhone_7";

// Convert REGISTRATION_PUBLIC_KEY to a byte array
const std::vector<uint8_t> REGISTRATION_PUBLIC_KEY = {
    5, 142, 140, 15, 116, 195, 235, 197, 215, 166, 134, 92, 108, 60, 132, 56, 86, 176, 97, 33, 204, 232, 234, 119, 77,
    34, 251, 111, 18, 37, 18, 48, 45
};

const std::string NOISE_MODE = "Noise_XX_25519_AESGCM_SHA256";
const int DICT_VERSION = 2;

// Convert KEY_BUNDLE_TYPE to a byte array
const std::vector<uint8_t> KEY_BUNDLE_TYPE = {5};

// Convert NOISE_WA_HEADER to a byte array
const std::vector<uint8_t> NOISE_WA_HEADER = {87, 65, 6, static_cast<uint8_t>(DICT_VERSION)};

// Convert PROTOCOL_VERSION to a byte array
const std::vector<uint8_t> PROTOCOL_VERSION = {5, 2};

// Convert MOBILE_NOISE_HEADER to a byte array
std::vector<uint8_t> MOBILE_NOISE_HEADER = {'W', 'A'};
MOBILE_NOISE_HEADER.insert(MOBILE_NOISE_HEADER.end(), PROTOCOL_VERSION.begin(), PROTOCOL_VERSION.end());

// Regular expressions
const std::regex URL_REGEX(
    R"((http(s)?:\/\/.)?(www\.)?[-a-zA-Z0-9@:%._\+~#=]{2,256}\.[a-z]{2,6}([-a-za-z0-9@:%_\+.~#?&//=]*))"
);
const std::regex URL_EXCLUDE_REGEX(R"(.*@.*)");

// WA_CERT_DETAILS as a struct
struct WACertDetails {
    static const int SERIAL = 0;
};

// PROCESSABLE_HISTORY_TYPES as an enum or array
// Assuming proto is a namespace or class that has been defined elsewhere in your C++ code
enum class HistorySyncType {
    INITIAL_BOOTSTRAP = proto::Message::HistorySyncNotification::HistorySyncType::INITIAL_BOOTSTRAP,
    PUSH_NAME = proto::Message::HistorySyncNotification::HistorySyncType::PUSH_NAME,
    RECENT = proto::Message::HistorySyncNotification::HistorySyncType::RECENT,
    FULL = proto::Message::HistorySyncNotification::HistorySyncType::FULL
};

// Assuming SocketConfig, WAVersion, Browsers, AuthenticationState, and other relevant types are defined elsewhere in your C++ code.

struct SocketConfig {
    WAVersion version;
    std::string browser;
    std::string waWebSocketUrl;
    int connectTimeoutMs;
    int keepAliveIntervalMs;
    Logger logger; // Assuming Logger is a class that has been defined elsewhere
    bool printQRInTerminal;
    bool emitOwnEvents;
    int defaultQueryTimeoutMs;
    std::vector<std::string> customUploadHosts;
    int retryRequestDelayMs;
    int maxMsgRetryCount;
    bool fireInitQueries;
    std::shared_ptr<AuthenticationState> auth;
    bool markOnlineOnConnect;
    bool syncFullHistory;
    std::function<Message(Message)> patchMessageBeforeSending;
    std::function<bool()> shouldSyncHistoryMessage;
    std::function<bool()> shouldIgnoreJid;
    int linkPreviewImageThumbnailWidth;
    TransactionOpts transactionOpts; // Assuming TransactionOpts is a struct or class defined elsewhere
    bool generateHighQualityLinkPreview;
    std::map<std::string, std::string> options;
    AppStateMacVerification appStateMacVerification; // Assuming AppStateMacVerification is a struct or class defined elsewhere
    std::function<std::shared_ptr<Message>()> getMessage;
    std::function<SignalRepository()> makeSignalRepository; // Assuming SignalRepository is a class defined elsewhere
};

const SocketConfig DEFAULT_CONNECTION_CONFIG = {
    // Initialize members with appropriate values
    // ...
};

// Assuming MediaType is an enum or class that has been defined elsewhere in your C++ code.
std::map<MediaType, std::string> MEDIA_PATH_MAP = {
    {MediaType::IMAGE, "/mms/image"},
    {MediaType::VIDEO, "/mms/video"},
    {MediaType::DOCUMENT, "/mms/document"},
    {MediaType::AUDIO, "/mms/audio"},
    {MediaType::STICKER, "/mms/image"},
    {MediaType::THUMBNAIL_LINK, "/mms/image"},
    
// Assuming MediaType is an enum or class that has been defined elsewhere in your C++ code.

// MEDIA_HKDF_KEY_MAPPING as a std::map
std::map<std::string, std::string> MEDIA_HKDF_KEY_MAPPING = {
    {"audio", "Audio"},
    {"document", "Document"},
    {"gif", "Video"},
    {"image", "Image"},
    {"ppic", ""},
    {"product", "Image"},
    {"ptt", "Audio"},
    {"sticker", "Image"},
    {"video", "Video"},
    {"thumbnail-document", "Document Thumbnail"},
    {"thumbnail-image", "Image Thumbnail"},
    {"thumbnail-video", "Video Thumbnail"},
    {"thumbnail-link", "Link Thumbnail"},
    {"md-msg-hist", "History"},
    {"md-app-state", "App State"},
    {"product-catalog-image", ""},
    {"payment-bg-image", "Payment Background"},
    {"ptv", "Video"}
};

// MEDIA_KEYS as a std::vector of MediaType
std::vector<MediaType> MEDIA_KEYS = {
    MediaType::AUDIO,
    MediaType::DOCUMENT,
    MediaType::VIDEO,
    MediaType::IMAGE,
    // ... Add all the MediaType enum values that are keys in MEDIA_PATH_MAP
};

// MIN_PREKEY_COUNT as a constant
const int MIN_PREKEY_COUNT = 5;

// INITIAL_PREKEY_COUNT as a constant
const int INITIAL_PREKEY_COUNT = 30;

// DEFAULT_CACHE_TTLS as a std::map
std::map<std::string, int> DEFAULT_CACHE_TTLS = {
    {"SIGNAL_STORE", 5 * 60}, // 5 minutes
    {"MSG_RETRY", 60 * 60}, // 1 hour
    {"CALL_OFFER", 5 * 60}, // 5 minutes
    {"USER_DEVICES", 5 * 60} // 5 minutes
};
