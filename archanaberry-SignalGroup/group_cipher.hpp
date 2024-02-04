#ifndef GROUPCIPHER_HPP
#define GROUPCIPHER_HPP

#include "queue_job.hpp" // Asumsikan kita memiliki file ini
#include "sender_key_message.hpp" // Asumsikan kita memiliki file ini
#include <memory>
#include <future>
#include <stdexcept>

class GroupCipher {
public:
    GroupCipher(std::shared_ptr<SenderKeyStore> senderKeyStore, const SenderKeyName& senderKeyName);
    std::future<std::string> encrypt(const std::string& paddedPlaintext);

private:
    std::shared_ptr<SenderKeyStore> senderKeyStore;
    SenderKeyName senderKeyName;
    std::future<std::string> queueJob(std::function<std::future<std::string>()> awaitable);
};

#endif // GROUPCIPHER_HPP
